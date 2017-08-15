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
*    Filename: exceptionMips.h
*
****************************************************************************
*    Description:
*
*     Interface for MIPS specific exception module. It implements to the
*     architecture-independent API - xdrvException.h.
*
*     This module allows clients to register custom exception handlers that
*     allow the events to be handled in an application specific manner.
*
*     User registered exception handlers are high level (C language)
*     functions. Low-level, architecture specific, processor context saves
*     will be performed prior to invoking registered handlers. Like-wise,
*     a context restore will be performed after the registered handler returns.
*
****************************************************************************/

#ifndef EXCEPTION_MIPS_H
#define EXCEPTION_MIPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvException.h>
#include <mipsContext.h>


/* ---- Constants and Types ---------------------------------------------- */

/*
** List of MIPS exception codes. These should be used to register user
** handlers via the xdrvExceptionSetHandler API.
*/
typedef enum EXCEPTION_MIPS_ID
{
   EXCEPTION_MIPS_TLB_MOD              = 1,
   EXCEPTION_MIPS_TLB_LOAD             = 2,
   EXCEPTION_MIPS_TLB_STORE            = 3,
   EXCEPTION_MIPS_ADDRESS_LOAD         = 4,
   EXCEPTION_MIPS_ADDRESS_STORE        = 5,
   EXCEPTION_MIPS_BUS_ERROR_I          = 6,
   EXCEPTION_MIPS_BUS_ERROR_D          = 7,
   EXCEPTION_MIPS_SYSCALL              = 8,
   EXCEPTION_MIPS_BREAKPOINT           = 9,
   EXCEPTION_MIPS_RESERVED_INSTRUCTION = 10,
   EXCEPTION_MIPS_COPROCESSOR          = 11,
   EXCEPTION_MIPS_OVERFLOW             = 12,
   EXCEPTION_MIPS_TRAP                 = 13,
   EXCEPTION_MIPS_VCEI                 = 14,
   EXCEPTION_MIPS_RESERVED15           = 15, /* Don't support floating point. */
   EXCEPTION_MIPS_RESERVED16           = 16, /* Don't support CP2. */
   EXCEPTION_MIPS_RESERVED17           = 17,
   EXCEPTION_MIPS_RESERVED18           = 18,
   EXCEPTION_MIPS_RESERVED19           = 19,
   EXCEPTION_MIPS_RESERVED20           = 20,
   EXCEPTION_MIPS_RESERVED21           = 21,
   EXCEPTION_MIPS_RESERVED22           = 22,
   EXCEPTION_MIPS_WATCH                = 23,
   EXCEPTION_MIPS_RESERVED24           = 24,
   EXCEPTION_MIPS_RESERVED25           = 25,
   EXCEPTION_MIPS_RESERVED26           = 26,
   EXCEPTION_MIPS_RESERVED27           = 27,
   EXCEPTION_MIPS_RESERVED28           = 28,
   EXCEPTION_MIPS_RESERVED29           = 29,
   EXCEPTION_MIPS_RESERVED30           = 30,
   EXCEPTION_MIPS_VCED                 = 31,

   /* This must be last. */
   EXCEPTION_MIPS_NUM_IDS

} EXCEPTION_MIPS_ID;



/*
** This is the state saved by the low level exception handler when an exception
** occurs. This information is passed to the user registered exception handler
** (XDRV_EXCEPTION_INFO).
*/
typedef MIPS_CONTEXT_REGS  EXCEPTION_MIPS_STATE;


/* ---- Variable Externs ------------------------------------------------- */

/*
** These are the low-level interrupt handlers. The interrupt controller
** should register handlers for interrupts that it is interested in.
*/
extern unsigned int   exc_interrupt_handlers[8];
extern unsigned int   exc_interrupt_data[8];

/*
** When an interrupt occurs a context save of the MIPS processor state is
** performed and placed on the stack. The location of this state is stored
** in 'hal_interrupt_context'.
*/
extern unsigned int   exc_interrupt_context;


/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   exceptionInitMips
**
** PURPOSE:    Architecture specific init function. Setup default, high-level
**             exception handlers.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:       It is assumed that the low-level exception vector handlers
**             have already been installed. This is performed by calling
**             exceptionMipsInstallVectors. The vectors are not installed
**             by this function because this should be done very early
**             in the chip initialization sequence, likely prior to setting up
**             the C-environment (stack pointer, bss, data-sections etc).
*****************************************************************************
*/
void exceptionInitMips( void );


/*
*****************************************************************************
** FUNCTION:   exceptionDeinitMips
**
** PURPOSE:    Architecture specific deinit function. Re-install default MIPS
**             exception handlers.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void exceptionDeinitMips( void );


/*
*****************************************************************************
** FUNCTION:   exceptionNotifyMips
**
** PURPOSE:    This function should be called by the low-level assembly
**             exception code after saving the necessary processor state.
**             This function will invoke the high-level user registered
**             exception handler.
**
** PARAMETERS: exceptionInfo  (in)  Architecture specific exception information.
**
** RETURNS:    None.
**
** NOTE:
*****************************************************************************
*/
void exceptionNotifyMips
(
   EXCEPTION_MIPS_STATE   *exceptionInfo
);


#ifdef __cplusplus
    }
#endif

#endif  /* EXCEPTION_MIPS_H  */
