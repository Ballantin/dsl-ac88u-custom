/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: exceptionHdlrGenericMips.c
*
****************************************************************************
*    Description:
*
*     Implements a generic MIPS exception handler that can be registered
*     with the low-level exception vector module.
*
*     The generic MIPS exception handler provide here is a high level (C language)
*     function that provides debug information about the exception cause.
*     The low-level, processor context saves/restores are handled by the
*     exception vector module.
*
*     The generic exception handler can be registered with the low-level
*     exception vector module via the xdrvExceptionSetHandler() API.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <xdrvException.h>
#include <mipsContext.h>
#include <exceptionHdlrMips.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/*
** Printing these logs may not be safe. If it is not safe to print from 
** the context of an exception these logs should be compiled out
*/
#ifndef EXCEPTION_MIPS_LOG
#define EXCEPTION_MIPS_LOG    XDRV_LOG_FORCED
#endif

/* ---- Private Variables ------------------------------------------------ */

static const char *gExceptionStr[] =
{
    "Interrupt",
    "TLB modification",
    "TLB (load/fetch)",
    "TLB (store)",
    "Address error (load/fetch)",
    "Address error (store)",
    "Bus error (fetch)",
    "Bus error (load/store)",
    "Syscall",
    "Breakpoint",
    "Reserved instruction",
    "Coprocessor unusable",
    "Arithmetic overflow",
    "Trap",
    "",
    "Floating point",
    "",
    "",
    "Coprocessor 2",
    "",
    "",
    "",
    "MMDX",
    "Watch",
    "Machine check",
    "",
    "",
    "",
    "",
    "",
    "Cache error",
    ""
};


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   exceptionHdlrGenericMips
**
** PURPOSE:    Generic MIPS exception handler that can be registered with the
**             the low-level exception vector module via the
**             xdrvExceptionSetHandler() API.
**
** PARAMETERS: exceptionId    (in)  Exception number.
**             data           (in)  User registered data value.
**             exceptionInfo  (in)  Information about exception that was raised.
**
** RETURNS:    Nothing.
**
** NOTE:       This function does not return.
*****************************************************************************
*/
void exceptionHdlrGenericMips
(
   XDRV_EXCEPTION_ID       exceptionId,
   XDRV_EXCEPTION_PARM     data,
   XDRV_EXCEPTION_INFO     exceptionInfo
)
{
   MIPS_CONTEXT_REGS *exceptionContext = (MIPS_CONTEXT_REGS *) exceptionInfo;


   /*
   ** Log processor state when exception occurred.
   */
   exceptionHdlrPrintContextMips( exceptionContext, XDRV_TRUE );


   /*
   ** Log call-stack trace.
   */
   exceptionHdlrStackTraceMips( exceptionContext->pc,
                                exceptionContext->gpRegs[29],
                                exceptionContext->gpRegs[31] );



   /*
   ** If we simply return, the exception instruction will execute over and over
   ** again. Not good. We could step over the offending instruction and continue
   ** executing, but that might lead to unexpected/misleading behaviour. Also
   ** not good. Let's just hang out....
   */
   while ( 1 )
   {
      ;
   }
}


/*
*****************************************************************************
** FUNCTION:   exceptionHdlrPrintContextMips
**
** PURPOSE:    To log MIPS specific exception state for debugging purposes.
**
** PARAMETERS: regs     (in)  MIPS processor state.
**             isCrash  (in)  Indicates if state corresponds to a crash.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void exceptionHdlrPrintContextMips( MIPS_CONTEXT_REGS *regs, XDRV_BOOL isCrash )
{
   /* Note: use of intermediate variable to allow compilation of SLIM build
            (build with no EXCEPTION_MIPS_LOG defined). */
   const char *exceptionStr;

   if ( isCrash )
   {
      unsigned int index;

      /* The exception vector field contains the exception-code from the
      ** cause register, including the least significant 2 bits of the cause
      ** register, which are always 0. Isolate the true exception-code to
      ** get the exception index. */
      index = (regs->vector >> 2);
      exceptionStr = gExceptionStr[index];

      EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "\n******************** CRASH ********************" ));
      EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "\nException code/type: %d / %s",
                           index,
                           exceptionStr ));
   }


   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "r0/zero=%08x r1/at  =%08x r2/v0  =%08x r3/v1  =%08x",
          0,              regs->gpRegs[1], regs->gpRegs[2],  regs->gpRegs[3] ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "r4/a0  =%08x r5/a1  =%08x r6/a2  =%08x r7/a3  =%08x",
          regs->gpRegs[4], regs->gpRegs[5], regs->gpRegs[6], regs->gpRegs[7] ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "r8/t0  =%08x r9/t1  =%08x r10/t2 =%08x r11/t3 =%08x",
          regs->gpRegs[8], regs->gpRegs[9],  regs->gpRegs[10], regs->gpRegs[11] ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "r12/t4 =%08x r13/t5 =%08x r14/t6 =%08x r15/t7 =%08x",
          regs->gpRegs[12], regs->gpRegs[13], regs->gpRegs[14], regs->gpRegs[15] ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "r16/s0 =%08x r17/s1 =%08x r18/s2 =%08x r19/s3 =%08x",
          regs->gpRegs[16], regs->gpRegs[17], regs->gpRegs[18], regs->gpRegs[19] ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "r20/s4 =%08x r21/s5 =%08x r22/s6 =%08x r23/s7 =%08x",
          regs->gpRegs[20], regs->gpRegs[21], regs->gpRegs[22], regs->gpRegs[23] ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "r24/t8 =%08x r25/t9 =%08x r26/k0 =%08x r27/k1 =%08x",
          regs->gpRegs[24], regs->gpRegs[25], regs->gpRegs[26], regs->gpRegs[27] ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "r28/gp =%08x r29/sp =%08x r30/fp =%08x r31/ra =%08x",
          regs->gpRegs[28], regs->gpRegs[29], regs->gpRegs[30], regs->gpRegs[31] ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "" ));

   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "PC   : 0x%08x    error addr: 0x%08x",   regs->pc,    regs->badvr ));
   EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "cause: 0x%08x    status:     0x%08x\n", regs->cause, regs->status ));
}
