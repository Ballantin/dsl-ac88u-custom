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
*    Filename: debugCmtInterface.c
*
****************************************************************************
*    Description:
*
*     This file contains functions related to the Debug Module.
*     Debug module is used to retrieve info of interest from the DSP,
*     IPC, and potentially other peripherals.
*
*     This instance of the debug module will be used on a concurrent, multi-
*     threaded (CMT) processor (e.g. dual-core MIPS) to obtain DSP debug
*     information. Therefore, the APIs provided in this file are platform
*     specific, and should be used by the HAL to instantiate the debug module.
*     The platform-independant API to obtain debug information by the
*     application is provided by debug.h.
*
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <string.h>
#include <itpcSharedCmds.h>
#include <debugCmtInterface.h>
#include <exceptionHdlrMips.h>
#include "debugIpcPrivate.h"
#include <xdrvDebug.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static XDRV_ITPC *gItpcDrv;


/* ---- Private Function Prototypes -------------------------------------- */

static int debugDspReadCnt( void );
static int debugDspForceAssert( void );
static int debugDspLogAssertContext( void  );
static unsigned int debugDspGetAssertStackPtr( void );
static int debugDspGetRegs( void );
static void debugDspDumpDataMem( unsigned int dspAddr, unsigned int amountToRead );
static void debugDspStackShow( void );

/* Debug "meta-driver" interface */
static const XDRV_DEBUG_FUNCS gDebugCmtDrvFuncs =
{
   debugDspReadCnt,
   debugDspForceAssert,
   debugDspLogAssertContext,
   debugDspGetAssertStackPtr,
   debugDspGetRegs,
   debugDspDumpDataMem,
   debugDspStackShow
};



/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   debugCmtInterfaceInit
**
** PURPOSE:    Initialize the Debug module. This instance of the debug module
**             will be used on a concurrent, multi-threaded (CMT) processor
**             (e.g. dual-core MIPS) to obtain DSP debug information.
**
** PARAMETERS: pItpcDrv (in)  Inter-thread processor communicaiton driver.
**             pDrv     (out) Debug driver object.
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
void debugCmtInterfaceInit( XDRV_ITPC *pItpcDrv, XDRV_DEBUG *pDrv )
{
   gItpcDrv = pItpcDrv;

   /* Init sub-modules. */
   debugIpcInit();

   /* Set the "meta-driver" interface */
   memset( pDrv, 0, sizeof( XDRV_DEBUG ) );
   pDrv->pDrvFuncs = &gDebugCmtDrvFuncs;
}


/*
*****************************************************************************
** FUNCTION:   debugCmtInterfaceDeinit
**
** PURPOSE:    Shutdown the Debug module
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void debugCmtInterfaceDeinit( void )
{
   debugIpcDeinit();

   gItpcDrv = NULL;
}


/*
*****************************************************************************
** FUNCTION:   debugDspReadCnt
**
** PURPOSE:    To read the value of the DSP's background & interrupt counters.
**
** PARAMETERS: none
**
** RETURNS:    0  - DSP is moving
**             -1 - DSP counters are static
**
** NOTE:
*****************************************************************************
*/
static int debugDspReadCnt( void )
{
   ITPC_SHARED_COUNTERS counters[3];
   unsigned int         i;

   /* Iterate a few times to make sure that the "I-am-alive" counter values are changing. */
   for ( i = 0; i < 3; i++ )
   {
      if ( XDRV_STATUS_OK == xdrvItpcSendSyncCommand( gItpcDrv,
                                                      ITPC_GET_COUNTERS_CMD_ID,
                                                      NULL,
                                                      &counters[i] ) )
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Background count (%d)  0x%x",   i, counters[i].backgroundCounter ));
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Interrupt  count (%d)  0x%x\n", i, counters[i].interruptCounter ));
      }
      else
      {
         return ( -1 );
      }
   }

   /* Check if the counters are static.  If so, we have a problem */
   if ( ( counters[0].backgroundCounter == counters[1].backgroundCounter ) &&
        ( counters[0].backgroundCounter == counters[2].backgroundCounter ) &&
        ( counters[0].interruptCounter == counters[1].interruptCounter ) &&
        ( counters[0].interruptCounter == counters[2].interruptCounter ) )
   {
      return ( -1 );
   }

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Overflow exception count: %d\n", counters[2].overflowExceptionCount ));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   debugDspForceAssert
**
** PURPOSE:    Force DSP assert.
**
** PARAMETERS: none.
**
** RETURNS:    0 on success
**
** NOTE:
*****************************************************************************
*/
static int debugDspForceAssert( void )
{
   /* Send async to avoid dead-lock. Asserting on the DSP will cause an
   ** exception ITPC command to be sent to us. In response to which, we
   ** will initiate another ITPC command to retrieve the exception context. */
   if ( XDRV_STATUS_OK != xdrvItpcSendAsyncCommand( gItpcDrv, ITPC_FORCE_ASSERT_CMD_ID ) )
   {
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "Forced assert failed!" ));
      return ( -1 );
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   debugDspGetRegs
**
** PURPOSE:    Get a snapshot of DSP registers.
**
** PARAMETERS: none.
**
** RETURNS:    0 on success
**
** NOTE:       When the DSP receives the getreg command, it places a copy
**             of its registers into an internal memory region, and then place
**             the address of that place into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
static int debugDspGetRegs( void )
{
   ITPC_SHARED_CONTEXT_REGS   regs;

   /*
   ** Get context registers.
   */
   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( gItpcDrv,
                                                   ITPC_GET_CONTEXT_REGS_CMD_ID,
                                                   NULL,
                                                   &regs ) )
   {
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "Get registers command failed!" ));
      return ( -1 );
   }


   /*
   ** Dump register contents.
   */
   exceptionHdlrPrintContextMips( (MIPS_CONTEXT_REGS *) &regs, XDRV_FALSE );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   debugDspGetAssertStackPtr
**
** PURPOSE:    Get DSP assert stack Ptr.
**
** PARAMETERS:
**
** RETURNS:    Assert stack pointer.
**
** NOTE:       When the DSP asserts, it overwrites the "doing fine" indicator
**             with the actual stack pointer. This routine can be used to
**             Check if the DSP is doing fine
*****************************************************************************
*/
static unsigned int debugDspGetAssertStackPtr( void )
{
   ITPC_SHARED_CONTEXT_REGS  *regs;

   /*
   ** Get exception context registers.
   */
   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( gItpcDrv,
                                                   ITPC_GET_EXCEPTION_CONTEXT_CMD_ID,
                                                   NULL,
                                                   &regs ) )
   {
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "Get exception registers command failed!" ));
      return ( -1 );
   }


   if ( regs->vector != (MIPS_REGISTER) -1 )
   {
      /*
      ** Log registers and call-stack.
      */
      exceptionHdlrPrintContextMips( (MIPS_CONTEXT_REGS *) regs, XDRV_TRUE );
      exceptionHdlrStackTraceMips( regs->pc, regs->gpRegs[29], regs->gpRegs[31] );
      return ( -1 );
   }
   else
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "Exception or assert did NOT occur!\n" ));
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   debugDspLogAssertContext
**
** PURPOSE:    To log the state of the DSP register set and stack context. This
**             function is intented to be called after the DSP asserts.
**
** PARAMETERS: pDebugCom         (mod) Debug-com interface used to read DSP
**                                     regitser set and stack context.
**
** RETURNS:    0 on success.
**
** NOTE:       When the DSP asserts, it is assumed that it will push the contents
**             of it's register set onto the stack, and then place the value
**             of the stack pointer into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
static int debugDspLogAssertContext( void )
{
   debugDspGetAssertStackPtr();
   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   debugDspDumpDataMem
**
** PURPOSE:    To log DSP data memory to the console.
**
** PARAMETERS: dspAddr        (in)  DSP data address to start log from.
**             amountToRead   (in)  Amount of data to dump.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void debugDspDumpDataMem( unsigned int dspAddr, unsigned int amountToRead )
{
   /*
   ** Not supported. There should be no need for this function for a CMT core.
   ** RAM is shared by both thread processors; therefore, we don't need a
   ** special function to access "DSP" memory.
   */
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DSP memory read not supported!\n" ));
}


/*
*****************************************************************************
** FUNCTION:   debugDspGetStackStats
**
** PURPOSE:    To display stack usage statistics for the DSP.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void debugDspStackShow( void )
{
   ITPC_SHARED_STACK_STATS stats;

   if ( XDRV_STATUS_OK != xdrvItpcSendSyncCommand( gItpcDrv,
                                                   ITPC_GET_STACK_STATS_CMD_ID,
                                                   NULL,
                                                   &stats ) )
   {
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "Get stack stats failed!" ));
      return;
   }

   XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "Stack size: %d", stats.stackSizeBytes ));
   XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "Stack used: %d", stats.stackUsedBytes ));
}
