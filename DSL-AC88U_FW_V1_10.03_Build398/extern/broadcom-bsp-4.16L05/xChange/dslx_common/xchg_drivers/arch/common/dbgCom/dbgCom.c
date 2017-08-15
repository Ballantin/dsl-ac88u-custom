/****************************************************************************
*
* Copyright © 2000-2008 Broadcom Corporation
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
****************************************************************************
*
*    Filename: dbgCom.h
*
****************************************************************************
*    Description:
*
*     Implenents shared memory debug communications interface that allows
*     the host to read and write to the DSP's memory space.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <string.h>
#include <dbgCom.h>
#include <xdrvLog.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Maximum number of times to query the DSP for counter activity */
#define  DBGCOM_DSP_MAX_READ_CNT    3

/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   dbgComCreate
**
** PURPOSE:    To create a debug communications drivers.
**
** PARAMETERS: pDbgComMem  (in)  Pointer to shared memory com-interface.
**             pMboxDrv    (in)  Mailbox that used to signal DSP.
**             pDbgcom     (out) Created dbgCom object.
**
** RETURNS:    0 on success, else error.
**
** NOTE:
*****************************************************************************
*/
int dbgComCreate
(
   volatile DBGCOM_MEM    *pDbgComMem,
   XDRV_MAILBOX_TX        *pMboxDrv,
   DBGCOM                 *pDbgcom
)
{
   pDbgcom->pDbgComMem = pDbgComMem;
   pDbgcom->pMboxDrv   = pMboxDrv;

   return( 0 );
}


/*
*****************************************************************************
** FUNCTION:   dbgComDestroy
**
** PURPOSE:    To de-init a dbgCom object.
**
** PARAMETERS: pDbgcom  (mod) dbgCom object to de-allocate.
**
** RETURNS:    0 on success, else error.
**
** NOTE:
*****************************************************************************
*/
int dbgComDestroy
(
   DBGCOM                 *pDbgcom
)
{
   memset( pDbgcom, 0, sizeof( DBGCOM ) );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   dbgComDspRead
**
** PURPOSE:    To read data from the DSP's memory space.
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**             pBuf        (out) Data read from the DSP is placed in this buffer.
**             dspAddr     (in)  DSP address to read from.
**             length      (in)  Number of 16-bit words to read.
**
** RETURNS:    Length of message read in 16-bit words.
**
** NOTE:
*****************************************************************************
*/
int dbgComDspRead
(
   DBGCOM        *pDbgComDrv,
   XDRV_UINT16   *pBuf,
   XDRV_UINT16    dspAddr,
   unsigned int   length
)
{
   volatile unsigned int   i;
   unsigned int            count;
   volatile DBGCOM_MEM    *pDbgcom;

   pDbgcom = pDbgComDrv->pDbgComMem;

   for( i = 0; i < length; i++ )
   {
      pDbgcom->command = DBGCOM_CMD_READDSPDATA;
      pDbgcom->addr    = dspAddr + i;

      xdrvMailboxTxSet( pDbgComDrv->pMboxDrv );

      /*
      ** Wait for the dsp's response
      */
      count = 0;
      while( ( count < 100 * 1000 ) && ( pDbgcom->command != DBGCOM_ACTIONDONE ) )
      {
         count++;
      }

      if( count >= 100 * 1000 )
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "READ TIMEOUT. Request %u of %u", i + 1, length ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   addr     0x%x", pDbgcom->addr ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   data     0x%x", pDbgcom->data ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   dspTrace 0x%x", pDbgcom->dspTrace ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   command  0x%x", pDbgcom->command ));

         return( 0 );
      }
      else
      {
         *(pBuf + i) = pDbgcom->data;
      }
   }

   return( length );
}


/*
*****************************************************************************
** FUNCTION:   dbgComDspWrite
**
** PURPOSE:    To write data to the DSP's memory space.
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**             pBuf        (in)  Data buffer to write to DSP.
**             dspAddr     (in)  DSP address to write to.
**             length      (in)  Number of 16-bit words to write.
**
** RETURNS:    Number of 16-bit words written.
**
** NOTE:
*****************************************************************************
*/
int dbgComDspWrite
(
   DBGCOM              *pDbgComDrv,
   const XDRV_UINT16   *pBuf,
   XDRV_UINT16          dspAddr,
   unsigned int         length
)
{
   volatile unsigned int   i;
   unsigned int            count;
   volatile DBGCOM_MEM    *pDbgcom;

   pDbgcom = pDbgComDrv->pDbgComMem;

   for( i = 0; i < length; i++ )
   {
      pDbgcom->command = DBGCOM_CMD_WRITEDSPDATA;
      pDbgcom->addr    = dspAddr + i;
      pDbgcom->data    = *(pBuf + i);

      xdrvMailboxTxSet( pDbgComDrv->pMboxDrv );

      /*
      ** Wait for the dsp's response
      */
      count = 0;
      while( ( count < 100 * 1000 ) && ( pDbgcom->command != DBGCOM_ACTIONDONE ) )
      {
         count++;
      }

      if( count >= 100 * 1000 )
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "WRITE TIMEOUT. Request %u of %u", i+1, length ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   addr     0x%x", pDbgcom->addr ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   data     0x%x", pDbgcom->data ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   dspTrace 0x%x", pDbgcom->dspTrace ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   command  0x%x", pDbgcom->command ));

         return( i );
      }
   }
   return( length );
}


/*
*****************************************************************************
** FUNCTION:   dspComTraceWrite
**
** PURPOSE:    To write the value of the trace register.
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**             value       (in)  Value to write to trace register.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void dbgComTraceWrite( DBGCOM *pDbgComDrv, XDRV_UINT16 value )
{
   (pDbgComDrv->pDbgComMem)->dspTrace = value;
}


/*
*****************************************************************************
** FUNCTION:   dbgComTraceRead
**
** PURPOSE:    To read the value of the trace register.
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**
** RETURNS:    Value of trace register.
**
** NOTE:
*****************************************************************************
*/
XDRV_UINT16 dbgComTraceRead( DBGCOM *pDbgComDrv )
{
   return( (pDbgComDrv->pDbgComMem)->dspTrace );
}


/*
*****************************************************************************
** FUNCTION:   dbgComDspReadAssertStackPtr
**
** PURPOSE:    To read the value of the DSP's stack pointer after a DSP assert.
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**
** RETURNS:    Stack pointer value.
**
** NOTE:
*****************************************************************************
*/
XDRV_UINT16 dbgComDspReadAssertStackPtr( DBGCOM *pDbgComDrv )
{
   volatile unsigned int   count;
   volatile DBGCOM_MEM    *pDbgcom;

   pDbgcom = pDbgComDrv->pDbgComMem;

   pDbgcom->command = DBGCOM_CMD_GETASSERTSTACKPTR;

   xdrvMailboxTxSet( pDbgComDrv->pMboxDrv );

   /*
   ** Wait for the dsp's response
   */
   count = 0;
   while( (count < 100 * 1000) && (pDbgcom->command != DBGCOM_ACTIONDONE) )
   {
      count++;
   }

   if( count >= 100 * 1000 )
   {
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "COMMAND TIMEOUT " ));
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "   addr     0x%x", pDbgcom->addr ));
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "   data     0x%x", pDbgcom->data ));
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "   dspTrace 0x%x", pDbgcom->dspTrace ));
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "   command  0x%x", pDbgcom->command ));

      return( 0 );
   }
   else
   {
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "dbgcomDspReadAssertStackPtr COMMAND Success " ));
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "   addr     0x%x", pDbgcom->addr ));
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "   data     0x%x", pDbgcom->data ));
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "   dspTrace 0x%x", pDbgcom->dspTrace ));
      XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "   command  0x%x", pDbgcom->command ));
   }

   return( pDbgcom->data );
}

/*
*****************************************************************************
** FUNCTION:   dbgComDspReadCnt
**
** PURPOSE:    To read the value of the DSP's background & interrupt counters.
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**
** RETURNS:    0  - DSP is moving
**             -1 - DSP counters are static
**
** NOTE:
*****************************************************************************
*/
int dbgComDspReadCnt( DBGCOM *pDbgComDrv )
{
   volatile unsigned int   count;
   volatile DBGCOM_MEM    *pDbgcom;
   XDRV_UINT16             backgroundCounter[ DBGCOM_DSP_MAX_READ_CNT ];
   XDRV_UINT16             interruptCounter[ DBGCOM_DSP_MAX_READ_CNT ];

   int i;

   pDbgcom = pDbgComDrv->pDbgComMem;

   /* Do this a couple of times so we see if counters change */
   for ( i = 0; i < DBGCOM_DSP_MAX_READ_CNT; i++ )
   {
      pDbgcom->command = DBGCOM_CMD_GETALIVECOUNTERS;

      xdrvMailboxTxSet( pDbgComDrv->pMboxDrv );

      /*
      ** Wait for the dsp's response
      */
      count = 0;
      while( (count < 100 * 1000) && (pDbgcom->command != DBGCOM_ACTIONDONE) )
      {
         count++;
      }

      if( count >= 100 * 1000 )
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "COMMAND TIMEOUT " ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   addr     0x%x", pDbgcom->addr ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   data     0x%x", pDbgcom->data ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   dspTrace 0x%x", pDbgcom->dspTrace ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   command  0x%x", pDbgcom->command ));
         return( -1 );
      }
      else
      {
         backgroundCounter[i] = pDbgcom->data;
         interruptCounter[i] = pDbgcom->dspTrace;
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   background cnt (%d)  0x%x", i, backgroundCounter[i] ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   interrupt  cnt (%d)  0x%x", i, interruptCounter[i] ));
      }
   }

   /* Check if the counters are static.  If so, we have a problem */
   if ( ( backgroundCounter[0] == backgroundCounter[1] ) &&
        ( backgroundCounter[1] == backgroundCounter[2] ) &&
        ( interruptCounter[0] == interruptCounter[1] ) &&
        ( interruptCounter[1] == interruptCounter[2] ) )
   {
      return ( -1 );
   }


   return( 0 );
}

/*
*****************************************************************************
** FUNCTION:   dbgComDspGetRegs
**
** PURPOSE:    Send a request to have a snapshot of the DSP registers
**             stored into DSP's internal memory, and retrive the address
**             of that snapshot.
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**
** RETURNS:    The DSP internal memory address where the register snapshot
**             gets stored.
**
** NOTE:
*****************************************************************************
*/
XDRV_UINT16 dbgComDspGetRegs( DBGCOM *pDbgComDrv )
{
   volatile unsigned int   count;
   volatile DBGCOM_MEM    *pDbgcom;

   pDbgcom = pDbgComDrv->pDbgComMem;

   pDbgcom->command = DBGCOM_CMD_GETREGS;

   xdrvMailboxTxSet( pDbgComDrv->pMboxDrv );

   /*
   ** Wait for the dsp's response
   */
   count = 0;
   while( (count < 100 * 1000) && (pDbgcom->command != DBGCOM_ACTIONDONE) )
   {
      count++;
   }

   if( count >= 100 * 1000 )
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "COMMAND TIMEOUT " ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   addr     0x%x", pDbgcom->addr ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   data     0x%x", pDbgcom->data ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   dspTrace 0x%x", pDbgcom->dspTrace ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   command  0x%x", pDbgcom->command ));

      return( 1 );
   }
   else
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "dbgComDspGetRegs COMMAND Success " ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   addr     0x%x", pDbgcom->addr ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   data     0x%x", pDbgcom->data ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   dspTrace 0x%x", pDbgcom->dspTrace ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   command  0x%x", pDbgcom->command ));
   }

   return( pDbgcom->data );
}

/*
*****************************************************************************
** FUNCTION:   dbgComDspReadCnt
**
** PURPOSE:    Force DSP assert.
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**
** RETURNS:    0 on success
**
** NOTE:
*****************************************************************************
*/
int dbgComForceAssert( DBGCOM *pDbgComDrv )
{
   volatile DBGCOM_MEM    *pDbgcom;

   pDbgcom = pDbgComDrv->pDbgComMem;

   pDbgcom->command = DBGCOM_CMD_FORCEASSERT;

   xdrvMailboxTxSet( pDbgComDrv->pMboxDrv );

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Forced assert sent!"));

   return( 0 );
}


/*****************************************************************************
** FUNCTION:   dbgComDspReadTimer
**
** PURPOSE:    Send a request to Read one of the dsp timers
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**             timerNum   (0 or 1)
**
** RETURNS:    The timer value
*****************************************************************************/
XDRV_UINT16 dbgComDspReadTimer( DBGCOM *pDbgComDrv, XDRV_UINT16 timerNum )
{
   volatile unsigned int   count;
   volatile DBGCOM_MEM    *pDbgcom;

   pDbgcom = pDbgComDrv->pDbgComMem;

   pDbgcom->command = DBGCOM_CMD_READTIMER;
   pDbgcom->addr    = timerNum;

   xdrvMailboxTxSet( pDbgComDrv->pMboxDrv );

   /*
   ** Wait for the dsp's response
   */
   count = 0;
   while( (count < 100 * 1000) && (pDbgcom->command != DBGCOM_ACTIONDONE) )
   {
      count++;
   }

   if( count >= 100 * 1000 )
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "dbgComDspReadTimer COMMAND TIMEOUT \n" ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   addr     0x%x\n", pDbgcom->addr ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   data     0x%x\n", pDbgcom->data ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   dspTrace 0x%x\n", pDbgcom->dspTrace ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   command  0x%x\n", pDbgcom->command ));

      return( 0 );
   }

   return( pDbgcom->data );
}

