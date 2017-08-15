/****************************************************************************
*
* Copyright © 2000-2008 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
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
*     Interface for shared memory debug communications interface that allows
*     the host to read and write to the DSP's memory space.
*
****************************************************************************/

#ifndef DBG_COM_H
#define DBG_COM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvMailbox.h>


/* ---- Constants and Types ---------------------------------------------- */

#define  DBGCOM_CMD_READDSPDATA        0xA500
#define  DBGCOM_CMD_WRITEDSPDATA       0xA501
#define  DBGCOM_CMD_READDSPPROG        0xA502
#define  DBGCOM_CMD_WRITEDSPPROG       0xA503
#define  DBGCOM_CMD_GETASSERTSTACKPTR  0xA504
#define  DBGCOM_CMD_GETALIVECOUNTERS   0xA505
#define  DBGCOM_CMD_FORCEASSERT        0xA506
#define  DBGCOM_CMD_GETREGS            0xA507
#define  DBGCOM_CMD_EXTSRAM_DISABLE    0xA508
#define  DBGCOM_CMD_EXTSRAM_ENABLE     0xA509
#define  DBGCOM_CMD_READTIMER          0xA50A


#define  DBGCOM_ACTIONDONE             0x5555
#define  DBGCOM_ACTIONERROR            0x5556


/*
** Shared memory interface.
**
** The host will set the mailbox interrupt when it wants to get the DSP's
** attention. The following predefined locations in the shared sram will set
** aside for the basic communication:
**
**   Command Reg:  Contains the command to/from the DSP.
**   Addr Reg:     Contains DSP addr to be read/written.
**   Data Reg:     If the command is a write command this reg will contain
**                 the word to be written.
**                 If the command is a read command this reg will contain
**                 the word that was read.  The host will have to poll
**                 the command reg to determine when the DSP has written
**                 the requested word.
*/
typedef struct
{
   XDRV_UINT16  command;
   XDRV_UINT16  addr;
   XDRV_UINT16  data;
   XDRV_UINT16  dspTrace;

} DBGCOM_MEM;


typedef struct
{
   XDRV_MAILBOX_TX        *pMboxDrv;
   volatile DBGCOM_MEM    *pDbgComMem;

} DBGCOM;



/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

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
);

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
);

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
);

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
);

/*
*****************************************************************************
** FUNCTION:   dspComTraceWrite
**
** PURPOSE:
**
** PARAMETERS: pDbgComDrv  (mod) dbgCom object.
**             value       (in)  Value to write to trace register.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void dbgComTraceWrite( DBGCOM *pDbgComDrv, XDRV_UINT16 value );

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
XDRV_UINT16 dbgComTraceRead( DBGCOM *pDbgComDrv );


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
XDRV_UINT16 dbgComDspReadAssertStackPtr( DBGCOM *pDbgComDrv );

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
int dbgComDspReadCnt( DBGCOM *pDbgComDrv );

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
int dbgComForceAssert( DBGCOM *pDbgComDrv );

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
XDRV_UINT16 dbgComDspGetRegs( DBGCOM *pDbgComDrv );


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
XDRV_UINT16 dbgComDspReadTimer
(
   DBGCOM        *pDbgComDrv,
   XDRV_UINT16   timerNum
);


#ifdef __cplusplus
    }
#endif

#endif  /* DBG_COM_H  */
