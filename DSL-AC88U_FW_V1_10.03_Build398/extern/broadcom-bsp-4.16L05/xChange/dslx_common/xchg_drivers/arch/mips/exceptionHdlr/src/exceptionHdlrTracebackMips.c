//****************************************************************************
//
//  Copyright (c) 2004  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of:
//          Broadcom Corporation
//          16215 Alton Parkway
//          Irvine, California 92618
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
//****************************************************************************
//  $Id: traceback.c 1.4 2005/02/22 15:31:40 msieweke Exp $
//
//  Filename:       traceback.c
//  Author:         Mike Sieweke
//  Creation Date:  February 13, 2004
//
//****************************************************************************
//  Description:
//      Here we implement a call trace function for the MIPS.  It's pretty
//      basic, and there are cases where a function may be untraceable.
//
//****************************************************************************

//********************** Include Files ***************************************

#include <xdrvLog.h>
#include <xdrvTypes.h>
#include <exceptionHdlrMips.h>

//********************** Local Constants *************************************

#define REG_RA      (31)
#define REG_SP      (29)
#define RSSHIFT     (21)
#define RTSHIFT     (16)
#define RDSHIFT     (11)
#define RSMASK      (0x1f<<RSSHIFT)
#define RTMASK      (0x1f<<RTSHIFT)
#define RDMASK      (0x1f<<RDSHIFT)
#define BMASK       (0xfc1f0000) // ((0x3f<<26) + (0x1f<<16))
#define BGEZAL      (0x04110000)
#define BGEZALL     (0x04130000)
#define BLTZAL      (0x04100000)
#define BLTZALL     (0x04120000)
#define JALMASK     (0xfc000000)
#define JAL         (0x0c000000)
#define JALRMASK    (0xfc1f003f)
#define JALR        (0x00000009)
#define ADDIUSPMASK (0xffff0000)
#define ADDIUSP     (0x27bd0000)
#define ADDISP      (0x23bd0000)
#define SWSPMASK    (0xffe00000)
#define SWSP        (0xafa00000)

/*
** Printing these logs may not be safe. If it is not safe to print from 
** the context of an exception these logs should be compiled out
*/
#ifndef EXCEPTION_MIPS_LOG
#define EXCEPTION_MIPS_LOG    XDRV_LOG_FORCED
#endif

extern XDRV_UINT32 _etext;

//********************** Local Functions *************************************

// GetfuncEntryFromRa() tries to find the current function's entry point by
// looking at the calling instruction, which should be 2 instructions before
// the return address.  If
static XDRV_UINT32 GetfuncEntryFromRa( XDRV_UINT32 ra )
{
    XDRV_UINT32  functionEntry    = 0x80000000;

    if (( ra > 0x80010000 ) &&
        ( ra < 0x84000000 ))
    {
        // The calling instruction is 2 instructions (8 bytes) before the
        // return address.  We can use this instruction to find where the
        // current function starts.
        XDRV_UINT32 *callInstrPtr  = (XDRV_UINT32 *)( ra - 8 );
        XDRV_UINT32  callInstr     = *callInstrPtr;
        XDRV_UINT32  branchInstr   = callInstr & BMASK;
        XDRV_UINT32  jalInstr      = callInstr & JALMASK;
//        XDRV_UINT32  jalrInstr     = callInstr & JALRMASK;
        XDRV_SINT32   instrOffset;

        // We hope the calling instruction is either "bal" or "jal", because
        // that gives us an absolute address.  If it's "jalr", it's most likely
        // a virtual function call, so we can't find the function entry.
        if ((branchInstr == BGEZAL)  ||
            (branchInstr == BGEZALL) ||
            (branchInstr == BLTZAL)  ||
            (branchInstr == BLTZALL))
        {
            instrOffset = callInstr & 0xffff;
            // If the offset is a negative number, sign extend it.
            if ((instrOffset & 0x8000) != 0)
            {
                instrOffset = (XDRV_SINT32) ( (XDRV_UINT32) instrOffset | 0xffff0000 );
            }
            // The offset is a word offset...
            instrOffset = instrOffset << 2;
            functionEntry = ra + instrOffset;
        }
        else if (jalInstr == JAL)
        {
            // The offset is a word offset...
            instrOffset = (callInstr & 0x3ffffff) << 2;
            functionEntry = (ra & 0xf0000000) | instrOffset;
        }
        else
        {
            EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "Current function called through jalr.  Entry not known." ));
        }
    }

    return functionEntry;
}


// Test whether the current instruction is a stack decrement (ADDI or ADDIU sp,#).
static XDRV_BOOL IsStackDecrement( XDRV_UINT32 currentInstruction )
{
    if ((( currentInstruction & ADDIUSPMASK ) == ADDIUSP ) ||
        (( currentInstruction & ADDIUSPMASK ) == ADDISP ))
    {
        return XDRV_TRUE;
    }
    else
    {
        return XDRV_FALSE;
    }
}


// Test whether the current instruction is a stack store (SW reg,offset(sp))
// and "reg" is the return address register.
static XDRV_BOOL IsStoreRa( XDRV_UINT32 currentInstruction )
{
    if ((( currentInstruction & SWSPMASK ) == SWSP ) &&
        (( currentInstruction & RTMASK ) == (REG_RA<<RTSHIFT)))
        {
            return XDRV_TRUE;
        }
        else
        {
            return XDRV_FALSE;
        }
}


// This is our exported interface for generating a stack trace.  We use the
// current PC, stack pointer, and return address register to look through the
// code and stack to glean some useful info.  When the compiler is in "full
// optimize" mode, the frame pointer isn't used, so we have to look for stack
// manipulation instructions.
void exceptionHdlrStackTraceMips( XDRV_UINT32 pc, XDRV_UINT32 sp, XDRV_UINT32 ra )
{
    XDRV_UINT32  currentRa        = ra;
    XDRV_UINT32  currentPc        = pc;
    XDRV_UINT32  functionEntry;
    XDRV_UINT32  currentStack     = sp;

    // I can't use the linker command script variable '_etext' because
    // this function might be used to trace an exception that occurred on
    // a MIPS thread processor that is different from the thread processor
    // that this function is executing on. '_etext' is only valid for for the
    // image that was built for the thread processor that this is executing
    // on. Instead, let's choose a hard-coded number - 64MB from the start
    // of RAM seems reasonable.
    //XDRV_UINT32  endText          = (XDRV_UINT32) &_etext;
    XDRV_UINT32  endText          = (XDRV_UINT32) 0x84000000;
    int         i;

    // Try to find our entry point from the call instruction at the return
    // address.  If the current function called another function, the call
    // won't be for our function.  But that doesn't cause a problem.
    // After this point we know every function in the chain has called another
    // function, so we know the return address will be on the stack.  Only
    // the top (current) function is in question.
    functionEntry = GetfuncEntryFromRa( ra );
    EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "Computed function entry is %08lx", functionEntry ));
    if (functionEntry > (currentPc))
    {
        functionEntry = 0x80000000;
    }

    // Iterate for up to 16 function calls.  This protects us from an error
    // which could put us into an infinite loop.
    for (i = 0; i < 16; i++)
    {
        XDRV_UINT32  currentInstruction;
        XDRV_SINT16  stackSize               = 0;
        XDRV_SINT16  stackOffsetToRa         = -1;

        // We have an idea where the current function may start, so start
        // looking backward from the PC to find stack size and where the RA
        // register was stored on the stack.
        while ((currentPc) >= functionEntry)
        {
            // Look at the instruction to see if it's a stack decrement.
            currentInstruction = *(XDRV_UINT32*)currentPc;
            if (IsStackDecrement( currentInstruction ))
            {
                // We only really care about stack decrement (add a negative
                // number).  If we find one, we're at the beginning of the
                // function.  Ignore a stack increment, because our starting
                // point could be after a function return.
                // We're assuming there won't be multiple stack decrement
                // opcodes in a single function.
                stackSize = currentInstruction & 0xffff;
                if ( stackSize < 0 )
                {
                    stackSize = - stackSize;
                    break;
                }
            }
            // If the return address was stored on the stack, remember where.
            // We'll pull the value off later.
            else if (IsStoreRa( currentInstruction ))
            {
                stackOffsetToRa = ( currentInstruction & 0xffff );
            }
            currentPc -= 4;
        }
        // It's possible for us to back up past the entry point if the current
        // function doesn't use the stack (no stack decrement).
        if (currentPc < functionEntry)
        {
            currentPc = functionEntry;
        }

        // We've reached what we think is a function entry point.
        EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "entry %08lx  ", currentPc ));

        // If we found where the return address was stored on the stack, use it.
        if (stackOffsetToRa != -1)
        {
            currentRa = *((XDRV_UINT32*)( currentStack + stackOffsetToRa ));
        }

        // Make sure the return address is within the code.
        if ((currentRa > endText) || (currentRa < 0x80000000))
        {
            EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "Return address (%08lx) invalid or not found.  Trace stops.", currentRa ));
            break;
        }
        EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "  called from %08lx", currentRa - 8 ));
        if (stackSize > 0)
        {
            currentStack += stackSize;
        }
        if ((currentStack > 0x84000000) || (currentStack < 0x80000000))
        {
            EXCEPTION_MIPS_LOG(( XDRV_LOG_MOD_XDRV, "New stack invalid.  Trace stops." ));
            break;
        }

        functionEntry = 0x80000000;
        currentPc     = currentRa;
        currentRa     = 0;
    }
}
