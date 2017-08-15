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
*    Filename: intCtrlRegsMips.s
*
****************************************************************************
*    Description:
*
*     Implements MIPS interrupt control interface for manipulating the 
*     CP0 registers for masking and unmasking interrupts.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <mipsDefines.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   intCtrlUnmaskMips
**
** PURPOSE:    To unmask (enable) a specific MIPS core interrupt.
**
** PARAMETERS: Interrupt number to unmask (0-indexed).
**
** RETURNS:    Nothing
**
** NOTE:       void intCtrlMipsUnmask( unsigned int interruptId );
*****************************************************************************
*/
ASM_START( intCtrlUnmaskMips )
   .set noreorder
   
   /* Retrieve the status register. */
   mfc0  t1, C0_STATUS
   nop
   
   /* Unmask interrupt. */
   la    t0, 0x00000100
   sllv  t0,t0,a0
   or    t1,t1,t0
   
   mtc0  t1, C0_STATUS
   nop
   nop
   nop
   
   jr    ra
   nop

ASM_END( intCtrlUnmaskMips )


/*
*****************************************************************************
** FUNCTION:   intCtrlMaskMips
**
** PURPOSE:    To mask (disable) a specific MIPS core interrupt.
**
** PARAMETERS: Interrupt number to unmask (0-indexed).
**
** RETURNS:    Nothing
**
** NOTE:       void intCtrlMipsMask( unsigned int interruptId );
*****************************************************************************
*/
ASM_START( intCtrlMaskMips )
   .set noreorder
   
   /* Retrieve the status register. */
   mfc0  t1, C0_STATUS
   nop
   
   /* Mask interrupt. */
   la    t0, 0x00000100
   sllv  t0,t0,a0
   nor   t0,t0,zero
   and   t1,t1,t0
   
   mtc0  t1, C0_STATUS
   nop
   nop
   nop
   
   jr    ra
   nop

ASM_END( intCtrlMaskMips )


/*
*****************************************************************************
** FUNCTION:   intCtrlClearMips
**
** PURPOSE:    To clear a specific MIPS core interrupt.
**
** PARAMETERS: Interrupt number to clear (0-indexed).
**
** RETURNS:    Nothing
**
** NOTE:       void intCtrlMipsClear( unsigned int interruptId );
*****************************************************************************
*/
ASM_START( intCtrlClearMips )
   .set noreorder
   
   /* Retrieve the cause register. */
   mfc0  t1, C0_CAUSE
   nop
   
   /* Clear interrupt. */
   la    t0, 0x00000100
   sllv  t0,t0,a0
   nor   t0,t0,zero
   and   t1,t1,t0
   
   mtc0  t1, C0_CAUSE
   nop
   nop
   nop
   
   jr    ra
   nop

ASM_END( intCtrlClearMips )


/*
*****************************************************************************
** FUNCTION:   intCtrlGenerateSoftInterruptMips
**
** PURPOSE:    Generate the specified software interrupt.
**
** PARAMETERS: interruptId (in)  Software interrupt id (0 or 1).
**
** RETURNS:    Nothing.
**
** NOTE:       It is the user's responsibility to disable interrupts during
**             execution of this function.
** 
**             void intCtrlMipsGenerateSoftInterrupt( unsigned int interruptId );
*****************************************************************************
*/
ASM_START(intCtrlGenerateSoftInterruptMips)
   .set noreorder

   /* Retrieve the cause register. */
   mfc0  t1, C0_CAUSE
   nop

   /* Trigger software interrupt. */
   la    t0, 0x00000100
   sllv  t0, t0, a0
   or    t1, t1, t0

   mtc0  t1, C0_CAUSE
   
   j     ra
   nop
   
ASM_END(intCtrlGenerateSoftInterruptMips)
