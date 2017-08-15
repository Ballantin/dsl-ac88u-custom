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
*    Filename: exceptionHdlrMaskMips.c
*
****************************************************************************
*    Description:
*
*     Implements a MIPS exception handler that can be registered
*     with the low-level exception vector module.
*
*     The "mask" MIPS exception handler provided here is a high level (C language)
*     function that can be used to ignore exceptions. It will step-over the
*     instruction that generated the exception, and continue execution of
*     the application.
*
*     This function will NOT emulate the exception victim instruction. The
*     effect of the exception is dependent upon the exception-type.
*     e.g. for arithmetic overflow exceptions, the destination register is
*     not updated.
*
*     This exception handler can also be used as a building-block for
*     instruction emulation. i.e. Another software exception handler
*     can be written to emulate instructions that generated an exception.
*     After the instruction has been emulated in software, this exception
*     handler can be called to step-over the emulated instruction.
*
*     The low-level, processor context saves/restores are handled by the
*     exception vector module.
*
*     The generic exception handler can be registered with the low-level
*     exception vector module via the xdrvExceptionSetHandler() API.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvException.h>
#include <exceptionHdlrMips.h>
#include <xchgAssert.h>
#include <mipsContext.h>
#include <mipsDefines.h>
#include <cp0Mips.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* Counts number of times exception handler is invoked. */
static unsigned int gExceptionCount = 0;


/* ---- Private Function Prototypes -------------------------------------- */

static unsigned int GetTargetAddressFromBranch
(
   unsigned int branchInstruction,
   unsigned int pc
);


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   GetTargetAddressFromBranch
**
** PURPOSE:    To calculate the effective target address of a branch instruction.
**
** PARAMETERS: branchInstruction (in) Branch instruction encoding.
**             pc                (in) Program counter of branch instruction.
**
** RETURNS:    Branch target address.
**
** NOTE:
*****************************************************************************
*/
static unsigned int GetTargetAddressFromBranch
(
   unsigned int branchInstruction,
   unsigned int pc
)
{
   unsigned int   targetAddress;

   /* The target address is formed by adding the signed 18-bit branch offset
   ** (16-bit value embedded in the instruction, left shifted by 2), with the
   ** address of the instruction in the branch delay slot. */


   /* Get the branch-offset. Use cast to 'short' to ensure proper sign-extension. */
   targetAddress = ((short)( branchInstruction & MIPS_OPERAND_BROFFSET_MASK )) << 2;


   /* Add to instruction in branch delay slot. */
   targetAddress += ( pc + 4 );

   return ( targetAddress );
}


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
)
{
   unsigned int   branchInstruction;
   unsigned int   majorOpcodeId;
   unsigned int   minorOpcodeId;
   unsigned int   targetAddress;
   unsigned int   rs, rt;
   int            rsValue, rtValue;
   XDRV_BOOL      bTakeCondBranch;


   /* The 'exceptionInfo' is actually a pointer to the MIPS context
   ** registers that were saved when the exception occurred. */
   MIPS_CONTEXT_REGS *exceptionContext = (MIPS_CONTEXT_REGS *) exceptionInfo;

   /* Not used. */
   (void) data;
   (void) exceptionId;

   /* Keep track of the number of times exception handler is invoked for debug. */
   gExceptionCount++;


   /*
   ** We simply want to step-over the instruction the caused the exception
   ** - the 'exception victim'. In most cases, the exception program
   ** counter (EPC) will point to the exception victim. Therefore, we simply
   ** need to add 4 to the EPC in the saved context state to step-over this
   ** instruction. When the exception context is restored, we will jump to
   ** instruction following the victim.
   **
   ** If the exception victim occurs in a branch delay slot, then the EPC
   ** will point to the branch instruction. In this case, we need to emulate
   ** the branch instruction. If the branch would not have occurred, we need
   ** to add 8 to the EPC (to skip the branch and victim instuctions). If the
   ** branch would have occurred, then we need to load the EPC with the branch
   ** destination.
   */


   if ( ( cp0GetCauseReg() & CAUSE_BD ) != CAUSE_BD )
   {
      /*
      ** This is the easy case. The exception victim is not in the branch
      ** delay slot. We simply need to add 4 to the exception program counter
      ** to step over the exception victim instruction.
      */
      exceptionContext->pc += 4;
   }
   else
   {
      /*
      ** This is more complicated. The exception occurred in a branch delay
      ** slot. We need to decode the branch instruction and emulate it.
      ** If it would not have occurred, then jump over it and the victim
      ** instruction. Otherwise, we need to jump to the branch destination.
      */

      targetAddress     = 0;
      bTakeCondBranch   = XDRV_FALSE;
      branchInstruction = *((unsigned int *)exceptionContext->pc);

      majorOpcodeId = ( branchInstruction & MIPS_OPCODE_MAJOR_MASK ) >> MIPS_OPCODE_MAJOR_SHIFT;
      minorOpcodeId = ( branchInstruction & MIPS_OPCODE_MINOR_MASK ) >> MIPS_OPCODE_MINOR_SHIFT;

      rs = ( branchInstruction & MIPS_OPERAND_RS_MASK ) >> MIPS_OPERAND_RS_SHIFT;
      rt = ( branchInstruction & MIPS_OPERAND_RT_MASK ) >> MIPS_OPERAND_RT_SHIFT;

      rsValue = (int) exceptionContext->gpRegs[ rs ];
      rtValue = (int) exceptionContext->gpRegs[ rt ];


      switch ( majorOpcodeId )
      {
         case ( MIPS_OPCODE_MAJOR_SPECIAL ):
         {
            if (( minorOpcodeId == MIPS_OPCODE_MINOR_JR   ) ||
                ( minorOpcodeId == MIPS_OPCODE_MINOR_JALR ) )
            {
               /* Jump to the target address found in general purpose register rs. */
               targetAddress = rsValue;
            }
            else
            {
               /* Should never reach here. */
               XCHG_ASSERT( 0 );
            }

         }
         break;


         case ( MIPS_OPCODE_MAJOR_J ):
         case ( MIPS_OPCODE_MAJOR_JAL ):
         {
            /* 'j' and 'jal' instructions. The target address is formed by shifing
            ** the 26-bit instruction index field by 2 to form the lower 28-bits.
            ** The upper 4 bits are set to the corresponding bits from the
            ** instruction in the delay slot. */
            targetAddress = ( branchInstruction & MIPS_OPERAND_INSTR_INDEX_MASK ) << 2;

            exceptionContext->pc = ( exceptionContext->pc + 4 ) & 0xf0000000;
            targetAddress |= exceptionContext->pc;
         }
         break;


         case ( MIPS_OPCODE_MAJOR_BEQ ):
         {
            /* 'beq' instruction. If rs and rt are equal, then branch. */
            bTakeCondBranch = ( rsValue == rtValue );
         }
         break;


         case ( MIPS_OPCODE_MAJOR_BNE ):
         {
            /* 'bne' instruction. If rs and rt are equal, then branch. */
            bTakeCondBranch = ( rsValue != rtValue );
         }
         break;


         case ( MIPS_OPCODE_MAJOR_BLEZ ):
         {
            /* 'blez' instruction. If rs is less than or equal to zero, then branch. */
            bTakeCondBranch = ( rsValue <= 0 );
         }
         break;


         case ( MIPS_OPCODE_MAJOR_BGTZ ):
         {
            /* 'bgtz' instruction. If rs is greater than zero, then branch. */
            bTakeCondBranch = ( rsValue > 0 );
         }
         break;


         case ( MIPS_OPCODE_MAJOR_REGIMM ):
         {
            switch ( rt )
            {
               case ( MIPS_OPCODE_REGIMM_BLTZ ):
               case ( MIPS_OPCODE_REGIMM_BLTZAL ):
               {
                  /* Branch if rs is less than zero. */
                  bTakeCondBranch = ( rsValue < 0 );
               }
               break;

               case ( MIPS_OPCODE_REGIMM_BGEZ ):
               case ( MIPS_OPCODE_REGIMM_BGEZAL ):
               {
                  /* Branch if rs is greater than or equal to zero. */
                  bTakeCondBranch = ( rsValue >= 0 );
               }
               break;

               /* Branch "likely" instructions. */
               case ( MIPS_OPCODE_REGIMM_BLTZL ):
               case ( MIPS_OPCODE_REGIMM_BGEZL ):
               case ( MIPS_OPCODE_REGIMM_BLTZALL ):
               case ( MIPS_OPCODE_REGIMM_BGEZALL ):
               {
                  /* The branch "likely" instructions only execute the delay slot
                  ** instruction if the branch is taken. Since an exception
                  ** occurred in the delay slot, this means that the branch should
                  ** be taken. */
                  bTakeCondBranch = XDRV_TRUE;
               }
               break;


               default:
               {
                  /* Should never reach here. */
                  XCHG_ASSERT( 0 );
               }
               break;
            }
         }
         break;


         /* Branch "likely" instructions. */
         case ( MIPS_OPCODE_MAJOR_BEQL  ):
         case ( MIPS_OPCODE_MAJOR_BNEL ):
         case ( MIPS_OPCODE_MAJOR_BLEZL ):
         case ( MIPS_OPCODE_MAJOR_BGTZL ):
         {
            /* The branch "likely" instructions only execute the delay slot
            ** instruction if the branch is taken. Since an exception
            ** occurred in the delay slot, this means that the branch should
            ** be taken. */
            bTakeCondBranch = XDRV_TRUE;
         }
         break;

         default:
         {
            /* Should never reach here. */
            XCHG_ASSERT( 0 );
         }
         break;
      }


      if ( targetAddress != 0 )
      {
         /* Jump instruction - set target address. */
         exceptionContext->pc = targetAddress;
      }
      else
      {
         if ( bTakeCondBranch )
         {
            /* Branch would have been taken, jump to branch destination. */
            exceptionContext->pc = GetTargetAddressFromBranch( branchInstruction,
                                                               exceptionContext->pc );
         }
         else
         {
            /* Branch would NOT have been taken, skip branch and victim instructions. */
            exceptionContext->pc += 8;
         }
      }
   }
}


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
unsigned int exceptionHdlrMaskGetCountMips( void )
{
   return ( gExceptionCount );
}


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
void exceptionHdlrMaskResetCountMips( void )
{
   gExceptionCount = 0;
}
