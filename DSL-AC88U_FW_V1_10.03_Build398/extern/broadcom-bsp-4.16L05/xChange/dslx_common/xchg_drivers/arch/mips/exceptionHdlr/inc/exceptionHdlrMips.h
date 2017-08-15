/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: exceptionHandlerMips.h
*
****************************************************************************
*    Description:
*
*     Interface for high-level MIPS exception handlers that can be registered
*     with the low-level exception vector module.
*
*     The MIPS exception handlers are high level (C language) functions.
*     The low-level, processor context saves/restores are handled by the
*     exception vector module.
*
*     The exception handlers can be registered with the low-level
*     exception vector module via the xdrvExceptionSetHandler() API.
*
****************************************************************************/

#ifndef EXCEPTION_HDLR_MIPS_H
#define EXCEPTION_HDLR_MIPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvException.h>
#include <mipsContext.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

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
);


/*
*****************************************************************************
** FUNCTION:   exceptionHdlrMaskMips
**
** PURPOSE:    MIPS exception handler that can be registered with the
**             the low-level exception vector module via the
**             xdrvExceptionSetHandler() API.
**
**             This can be used to ignore (mask) exceptions. It will step-over
**             the instruction that generated the exception, and continue
**             execution of the application.
**
**             This function will NOT emulate the exception victim instruction.
**             The effect of the exception is dependent upon the exception-type.
**             e.g. for arithmetic overflow exceptions, the destination register
**             is not updated.
**
** PARAMETERS: exceptionId    (in)  Exception number.
**             data           (in)  User registered data value.
**             exceptionInfo  (in)  Information about exception that was raised.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void exceptionHdlrMaskMips
(
   XDRV_EXCEPTION_ID       exceptionId,
   XDRV_EXCEPTION_PARM     data,
   XDRV_EXCEPTION_INFO     exceptionInfo
);


/*
*****************************************************************************
** FUNCTION:   exceptionHdlrMaskGetCountMips
**
** PURPOSE:    Returns number of times the "mask" exception handler
**             has been executed.
**
** PARAMETERS: None.
**
** RETURNS:    Exception count.
**
** NOTE:
*****************************************************************************
*/
unsigned int exceptionHdlrMaskGetCountMips( void );


/*
*****************************************************************************
** FUNCTION:   exceptionHdlrMaskResetCountMips
**
** PURPOSE:    Resets the "mask" exception handler counter.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void exceptionHdlrMaskResetCountMips( void );


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
void exceptionHdlrPrintContextMips( MIPS_CONTEXT_REGS *regs, XDRV_BOOL isCrash );


/*
*****************************************************************************
** FUNCTION:   exceptionHdlrStackTraceMips
**
** PURPOSE:    Interface for generating a stack trace.  We use the
**             current PC, stack pointer, and return address register to look
**             through the code and stack to glean some useful info.  When the
**             compiler is in "full optimize" mode, the frame pointer isn't used,
**             so we have to look for stack manipulation instructions.
**
** PARAMETERS: pc (in)  Program counter.
**             sp (in)  Stack pointer.
**             ra (in)  Return address.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void exceptionHdlrStackTraceMips( XDRV_UINT32 pc, XDRV_UINT32 sp, XDRV_UINT32 ra );


#ifdef __cplusplus
    }
#endif

#endif  /* EXCEPTION_HDLR_MIPS_H  */
