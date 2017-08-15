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
*    Filename: exceptionHdlrOverflowTestMips.s
*
****************************************************************************
*    Description:
*
*     Tests the MIPS "mask" exception handler that is used to ignore
*     specific exception types. The arithmetic overflow exception is
*     used to test the exception handler. The exceptions should be 
*     ignored, and the application should continue to run (instead of
*     the default exception behaviour - crashing).
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <mipsDefines.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

    
   .text
   
/*
*****************************************************************************
** FUNCTION:   exceptionHdlrOverflowTest
**
** PURPOSE:    Tests the MIPS "mask" exception handler that is used to ignore
**             specific exception types. The arithmetic overflow exception is
**             used to test the exception handler. The exceptions should be 
**             ignored, and the application should continue to run (instead of
**             the default exception behaviour - crashing).
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
**             void exceptionHdlrOverflowTest( void );
*****************************************************************************
*/
ASM_START( exceptionHdlrOverflowTest )   
   .set noreorder
   
   /* Create stack frame. Save stack pointer. */
   addiu sp, sp, -16
   sw    ra, 0(sp)

   
   /* -----------------------------------------------------------------------
   ** Register usage (don't use these for anything else):
   **    t0 - Destination register of arithmetic operation.
   **    t1 - Arithmetic operand that will generate exception (0x7ffffff)
   **    t2 - Debug counter.
   ** 
   **    t8 - Branch return address. 
   */
   
   
   /* -----------------------------------------------------------------------
   ** This is the easy case - overflow instructions do not occur in 
   ** branch-delays slots.
   */
   
   /* Clear the counter. */
   move  t2, zero
   
   /* Prime the destination register. On an overflow, the destination register
   ** should not be modified. */
   li    t0, 0xC0FFEE
   
   /* This should generate an arithmetic overflow exception. */
   li    t1, 0x7fffffff
   addi  t0, t1, 2
   
   /* Increment counter. If we crashed, we'll never reach here. */
   addiu t2, 1
   
   /* Verify count.*/
   li    t4, 1
   bne   t2, t4, exceptionTestFailed
   nop
   
   

   
   /* -----------------------------------------------------------------------
   ** 'j' instruction - branch-delay test.
   */
   
   /* Clear the counter. */
   move  t2, zero
   
   /* Set return address. */
   la    t8, 1f

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   j     exceptionTestJumpPoint
   addi  t0, t1, 2
1:   


   /* -----------------------------------------------------------------------
   ** 'jal' instruction - branch-delay test.
   */
   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   jal   exceptionTestJumpAndLinkPoint
   addi  t0, t1, 2

   
   /* -----------------------------------------------------------------------
   ** 'jr' instruction - branch-delay test.
   */
   
   /* Set target and return address. */
   la    t7, exceptionTestJumpPoint
   la    t8, 1f

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   jr    t7
   addi  t0, t1, 2
1:   


   /* -----------------------------------------------------------------------
   ** 'jalr' instruction - branch-delay test.
   */
   
   /* Set target address. */
   la    t7, exceptionTestJumpAndLinkPoint
   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   jalr  t7
   addi  t0, t1, 2
   

   /* -----------------------------------------------------------------------
   ** 'beq' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should NOT occur. */
   li    t6, 5
   li    t7, 6

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   beq   t6, t7, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   

   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, 6
   li    t7, 6

   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   beq   t6, t7, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'beql' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, 6
   li    t7, 6

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   beql  t6, t7, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'bltz' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should NOT occur. */
   li    t6, 5

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bltz  t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   

   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, -1
   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bltz  t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'bltzl' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, -1

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bltzl t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'bltzal' instruction - branch-delay test.
   */
   
   /* Branch should NOT occur. */
   li       t6, 5

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bltzal   t6, exceptionTestJumpAndLinkPoint
   addi     t0, t1, 2

   /* Branch should occur. */
   li       t6, -1
   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bltzal   t6, exceptionTestJumpAndLinkPoint
   addi     t0, t1, 2
   

   /* -----------------------------------------------------------------------
   ** 'bltzall' instruction - branch-delay test.
   */
   
   /* Branch should occur. */
   li       t6, -1
   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bltzall  t6, exceptionTestJumpAndLinkPoint
   addi     t0, t1, 2
   
   
   /* -----------------------------------------------------------------------
   ** 'bgez' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should NOT occur. */
   li    t6, -5

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgez  t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   

   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, 0
   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgez  t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'bgezl' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, 5

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgezl t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'bgezal' instruction - branch-delay test.
   */
   
   /* Branch should NOT occur. */
   li       t6, -5

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgezal   t6, exceptionTestJumpAndLinkPoint
   addi     t0, t1, 2

   /* Branch should occur. */
   li       t6, 0
   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgezal   t6, exceptionTestJumpAndLinkPoint
   addi     t0, t1, 2
   

   /* -----------------------------------------------------------------------
   ** 'bgezall' instruction - branch-delay test.
   */
   
   /* Branch should occur. */
   li       t6, 5
   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgezall  t6, exceptionTestJumpAndLinkPoint
   addi     t0, t1, 2
   
   
   /* -----------------------------------------------------------------------
   ** 'bgtz' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should NOT occur. */
   li    t6, -6

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgtz  t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   

   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, 1

   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgtz  t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'bgtzl' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, 6

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bgtzl t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'blez' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should NOT occur. */
   li    t6, 7

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   blez  t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   

   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, 0

   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   blez  t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'blezl' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, -2

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   blezl t6, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'bne' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should NOT occur. */
   li    t6, -1
   li    t7, -1

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bne   t6, t7, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   

   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, 17
   li    t7, -17

   
   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bne   t6, t7, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   
   
   
   /* -----------------------------------------------------------------------
   ** 'bnel' instruction - branch-delay test.
   */
   
   /* Set return address. */
   la    t8, 1f
   
   /* Branch should occur. */
   li    t6, -2
   li    t7, 22

   /* Generate an arithmetic overflow exception in the branch-delay slot. */
   bnel  t6, t7, exceptionTestJumpPoint
   addi  t0, t1, 2
1:   

   
   
   /* -----------------------------------------------------------------------
   ** Error checks.
   */
   
   
   /* Verify count.*/
   li    t4, 20
   bne   t2, t4, exceptionTestFailed
   nop
   
   
   /* Verify destination register is unmodified. This should validate that
   ** all the exceptions actually occurred. */
   li    t4, 0xC0FFEE
   bne   t0, t4, exceptionTestFailed
   nop
   
   
   /* All tests passed. */
   b     exceptionTestPassed
   nop
   
   
   
   
exceptionTestJumpPoint:

   /* Increment counter. If we crashed, we'll never reach here. */
   addiu t2, 1
   
   /* Return. */
   jr    t8
   nop
   
   
exceptionTestFailed:
   /* Force a crash. */
   lw    zero, 0(zero)   
   

exceptionTestPassed:
   lw    ra, 0(sp)
   jr    ra
   addiu sp, sp, 16
   
ASM_END( exceptionHdlrOverflowTest )   





/*
*****************************************************************************
** FUNCTION:   exceptionTestJumpAndLinkPoint
**
** PURPOSE:    Used to test exceptions that occur in branch delay slots of
**             jump and branch "link" (ra is loaded with return-address) 
**             instructions.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
ASM_START( exceptionTestJumpAndLinkPoint )

   /* Increment counter. If we crashed, we'll never reach here. */
   addiu t2, 1

   jr    ra
   nop

ASM_END( exceptionTestJumpAndLinkPoint )
