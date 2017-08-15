/***************************************************************************
*    Copyright 2000-2008  Broadcom Corporation
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
*    Filename: globalInterruptMips.s
*
****************************************************************************
*    Description:
*
*     MIPS-specific interrupt enable/disable implementation of 
*     global interrupt API defined by xdrvGlobalInterrupt.h.
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
** FUNCTION:   xdrvGlobalInterruptDisable
**
** PURPOSE:    Disable global interrupts
**
** PARAMETERS: lockKey - pointer to XDRV_GLOBAL_INTERRUPT_LOCK
**
** RETURNS:    nothing
**
** NOTES:      On some OSs locking out interrupts does not prevent rescheduling.
**             Thus, if a task locks out interrupts and invokes kernel services
**             that cause the task to block (e.g., taskSuspend() or taskDelay())
**             or that cause a higher priority task to be ready (e.g., semGive()
**             or taskResume()), then rescheduling occurs and interrupts are
**             unlocked while other tasks run. Therefore, caller has to make
**             sure that no kernel services are used in between the
**             xdrvGlobalInterruptDisable/Restore pair.
**
**             The lockKey value is passed back to globalInterruptRestore to
**             re-enable CPU interrupts.
**
**             Calls to xdrvGlobalInterruptDisable and globalInterruptRestore
**             are nestable.
** 
**             void xdrvGlobalInterruptDisable( XDRV_GLOBAL_INTERRUPT_LOCK *lockKey );
*****************************************************************************
*/

ASM_START( xdrvGlobalInterruptDisable )
   .set noreorder
   
   /* Retrieve the status register. */
   mfc0  t0, C0_STATUS
   nop
   
   /* Save the current value of the status register to return to caller. */
   move  t1, t0
   
   /* Disable interrupts. */
   and   t0, (~SR_IE)
   mtc0  t0, C0_STATUS
   nop
   nop
   nop
   
   /* Only return interrupt status bit value to caller. */
   and   t1, SR_IE
   sw    t1, 0(a0)
   
   jr    ra
   nop

ASM_END( xdrvGlobalInterruptDisable )


/*
*****************************************************************************
** FUNCTION:   xdrvGlobalInterruptRestore
**
** PURPOSE:    Restore previous global interrupt state
**
** PARAMETERS: lockKey - input - value set by xdrvGlobalInterruptDisable
**
** RETURNS:    nothing
**
** NOTES:      On some OSs locking out interrupts does not prevent rescheduling.
**             Thus, if a task locks out interrupts and invokes kernel services
**             that cause the task to block (e.g., taskSuspend() or taskDelay())
**             or that cause a higher priority task to be ready (e.g., semGive()
**             or taskResume()), then rescheduling occurs and interrupts are
**             unlocked while other tasks run. Therefore, caller has to make
**             sure that no kernel services are used in between the
**             xdrvGlobalInterruptDisable/Enable pair.
**
**             Calls to xdrvGlobalInterruptDisable and xdrvGlobalInterruptRestore
**             are nestable.
** 
**             void xdrvGlobalInterruptRestore( XDRV_GLOBAL_INTERRUPT_LOCK *lockKey );
*****************************************************************************
*/
ASM_START( xdrvGlobalInterruptRestore )
   .set noreorder
   
   /* Retrieve the status register. */
   mfc0  t0, C0_STATUS
   nop
   
   /* Isolate the interrupt enable bit of the user supplied value, and 
   ** merge with the current contents of the status register. */
   lw    t1, 0(a0)
   nop
   and   t1, SR_IE
   or    t0, t1
   
   /* Set new status register value. */
   mtc0  t0, C0_STATUS
   nop
   nop
   nop
   
   jr    ra
   nop

ASM_END( xdrvGlobalInterruptRestore )

/*
*****************************************************************************
** FUNCTION:   xdrvGlobalInterruptEnable
**
** PURPOSE:    Blindly re-enables interrupts, independent of how many times
**             xdrvGlobalInterruptDisable has been called previously.
**
** PARAMETERS: none
**
** RETURNS:    nothing
**
** NOTES:      Calls to xdrvGlobalInterruptDisable and xdrvGlobalInterruptRestore
**             are nestable and should be used as a function pair to protect
**             critical sections of code.
**
**             This function should really only be used in special situations
**             where interrupts need to be enabled independent of any
**             previous state.
** 
**             void xdrvGlobalInterruptEnable( void );
*****************************************************************************
*/
ASM_START( xdrvGlobalInterruptEnable )
   .set noreorder
   
   /* Retrieve the status register. */
   mfc0  t0, C0_STATUS
   nop
   
   /* Set the interrupt enable bit in the status register. */
   or    t0, SR_IE
   mtc0  t0, C0_STATUS
   nop
   nop
   nop
   
   jr    ra
   nop

ASM_END( xdrvGlobalInterruptEnable )
