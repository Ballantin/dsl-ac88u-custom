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
*    Filename: timerClockMips.s
*
****************************************************************************
*    Description:
*
*     Implements MIPS timer control functions using the CP0 count
*     and compare registers.
*
*     Count and compare are 32-bit registers. Count increments at half the
*     CPU clock frequency (and wraps back to 0 on overflow). When count 
*     increments to a value equal to the compare register, and interrupt
*     occurs. The interrupt can be cleared by writing to the compare register.
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
** FUNCTION:   timerResetMips
**
** PURPOSE:    To re-initialize the MIPS core timers to generate the next
**             interrupt.
**
** PARAMETERS: period   (in)  Number of cycles after which a timer interrupt
**                            will be generated. The period must be calculated
**                            based upon the fact that the count register
**                            increments at half the CPU clock frequency.
**
** RETURNS:    Nothing.
**
** NOTE:       void timerResetMips( unsigned int period );
*****************************************************************************
*/
ASM_START( timerResetMips )
   .set noreorder
   
   /* Account for interrupt service latency by only incrementing the compare
   ** register. Do not touch the count register. Also, other timer functions
   ** rely upon the fact that the count register is not reset. */
   mfc0 v1, C0_COUNT
   mfc0 v0, C0_COMPARE
   nop
   nop
   nop
   
   /* Increment the compare register by the timer period. If there was a large
   ** interrupt service latency, and the count register has already incremented
   ** passed this value, then set the compare register equal to the current
   ** count + timer period. 
   ** 
   ** Writing to the compare register has the side-effect of clearing the timer
   ** interrupt. */
   addu v0, a0, v0
   subu t0, v0, v1
   bgtz t0, 1f
   nop
   addu v0, a0, v1

1:
   mtc0 v0, C0_COMPARE
   nop
   
   
   /* We need to re-read the count register. It's possible that there was
   ** a large interrupt service latency, but the count value was marginally
   ** less than the new compare value when it was originally read above. 
   ** However, it's possible that the count register has now passed the 
   ** new compare between the original read above and now. In this case,
   ** the next interrupt would not occur until the count register wraps 
   ** all the way around, since the write to the compare register directly
   ** above this comment clears the interrupt, and the count register has 
   ** already passed this value. To avoid this situation, we will set the
   ** compare register again (incrementing it again by the timer period)
   ** when this corner case is detected. */
   mfc0 v1, C0_COUNT
   nop
   nop
   nop
   
   subu t0, v0, v1
   bgtz t0, 2f
   nop
   addu v0, a0
   
   mtc0 v0, C0_COMPARE
   nop
   
   
2:   
   jr    ra
   nop

ASM_END( timerResetMips )



/*
*****************************************************************************
** FUNCTION:   timerGetCountMips
**             timerSetCountMips
**             timerGetCompareMips
**             timerSetCountMips
**
** PURPOSE:    Accessor functions for MIPS CP0 count and compare registers.
**
** PARAMETERS: 
**
** RETURNS:    
**
** NOTE:       
**          unsigned int timerGetCountMips( void );
**          void         timerSetCountMips( unsigned int );
**          unsigned int timerGetCompareMips( void );
**          void         timerSetCompareMips( unsigned int );
*****************************************************************************
*/

ASM_START( xdrvTimerRead )
   
   // xdrvTimerRead() is just an alias for timerMipsGetCount().
   j     timerGetCountMips
   
ASM_END( xdrvTimerRead )


ASM_START( timerGetCountMips )
   .set noreorder
   
   mfc0  v0, C0_COUNT
   nop
   nop
   nop
   
   jr    ra
   nop
   
ASM_END( timerGetCountMips )


ASM_START( timerSetCountMips )
   .set noreorder
   
   mtc0  a0, C0_COUNT
   nop
   nop
   nop
   
   jr    ra
   nop
   
ASM_END( timerSetCountMips )


ASM_START( timerGetCompareMips )
   .set noreorder
   
   mfc0  v0, C0_COMPARE
   nop
   nop
   nop
   
   jr    ra
   nop
   
ASM_END( timerGetCompareMips )


ASM_START( timerSetCompareMips )
   .set noreorder
   
   mtc0  a0, C0_COMPARE
   nop
   nop
   nop
   
   jr    ra
   nop
   
ASM_END( timerSetCompareMips )
