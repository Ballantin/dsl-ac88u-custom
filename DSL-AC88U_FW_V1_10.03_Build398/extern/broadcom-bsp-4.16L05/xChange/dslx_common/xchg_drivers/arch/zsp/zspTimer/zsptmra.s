!-----------------------------------------------------------------------------
!
!     Copyright (c) 2001 Broadcom Corporation
!           All Rights Reserved
!
!     No portions of this material may be reproduced in any form without the
!     written permission of:
!
!           Broadcom Corporation
!           16215 Alton Parkway
!           Irvine, California 92619-7013
!
!     All information contained in this document is Broadcom Corporation
!     company private, proprietary, and trade secret.
!
!-----------------------------------------------------------------------------
!
!  zsptmra.s - ZSP timer access functions
!
!  PURPOSE:
!
!     This file contains the assembly source code functions for 
!
!  NOTES:
!
!     1. This file implements the following global functions:
!
!  zspTimer0SetTimer - Set timer0 register
!  zspTimer0SetTimer - Set timer1 register
!  zspTimer0GetTimer - Get value of timer0 register
!  zspTimer1GetTimer - Get value of timer1 register
!  zspTimer0SetTimerPrescaler - set timer0 prescale value
!  zspTimer1SetTimerPrescaler - set timer1 prescale value
!  zspTimer0GetTimerPrescaler - get timer0 prescale value
!  zspTimer1GetTimerPrescaler - get timer1 prescale value
!
!-----------------------------------------------------------------------------

!------ Include Files --------------------------------------------------------
!------ Public Variables -----------------------------------------------------

   .global _zspTimer0SetTimer
   .global _zspTimer0GetTimer
   .global _zspTimer0SetTimerPrescaler
   .global _zspTimer0GetTimerPrescaler

#if ZSPTIMER_GETTCREG
   .global _zspTimerGetTCReg
#endif

   .global _zspTimer1SetTimer
   .global _zspTimer1GetTimer
   .global _zspTimer1SetTimerPrescaler
   .global _zspTimer1GetTimerPrescaler   

!------ Private Constants and Types ------------------------------------------

#define ZSPTIMER_GETTCREG  0

!------ Private Variables ----------------------------------------------------
!------ Private Function Prototypes ------------------------------------------
!------ Functions ------------------------------------------------------------

   .section    ".text"

#if ZSPTIMER_GETTCREG
!-----------------------------------------------------------------------------
!
!  zspTimerGetTCReg - 
!
!  PURPOSE:
!
!     This function returns the timer control register
!
!  PROTOTYPE:
!
!     UINT16 zspTimerGetTCReg( void );
!
!  PARAMETERS:
!
!     none
!
!  RETURNS:
!
!     ZSP control register tc
!
!  PSEUDOCODE:
!
!     return %tc;
!
!  NOTES:
!     
!     1. - 
!
!-----------------------------------------------------------------------------
   .walign 4

_zspTimerGetTCReg:

! - - - START PROLOGUE - - -
 
! No registers to save to stack

! No local frame to allocate (no local variables, no argument block)

! - - - END PROLOGUE - - -

! Move timer count to r4

   mov      r4, %tc                    !
 
! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! No registers to restore from stack
   
! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimerGetTCReg
#endif /* #if ZSPTIMER_GETTCREG */


!-----------------------------------------------------------------------------
!
!  zspTimer0SetTimer - Set timer0 register
!  zspTimer0SetTimer - Set timer1 register
!
!  PURPOSE:
!
!     This function sets the value of on-chip timer X.
!
!  PROTOTYPE:
!
!     void zspTimer0SetTimer( UINT16 timerValue );
!     void zspTimer1SetTimer( UINT16 timerValue );
!
!  PARAMETERS:
!
!     timerValue     => value to set timer register to
!
!  RETURNS:
!
!     Nothing
!
!  PSEUDOCODE:
!
!     Initialize %timerX to timerVal
!
!  NOTES:
!     
!     1. - 
!
!-----------------------------------------------------------------------------
   .walign 4

_zspTimer0SetTimer:

! - - - START PROLOGUE - - -

! No registers to save to stack

! No local frame to allocate (no local variables, no argument block)

! - - - END PROLOGUE - - -

! Move the initial value to the timer0 register

   mov      %timer0, r4               !

! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! No registers to restore from stack

! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimer0SetTimer
   .walign 4

_zspTimer1SetTimer:

! - - - START PROLOGUE - - -

! No registers to save to stack

! No local frame to allocate (no local variables, no argument block)

! - - - END PROLOGUE - - -

! Move the initial value to the timer0 register

   mov      %timer1, r4               !

! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! No registers to restore from stack

! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimer1SetTimer


!-----------------------------------------------------------------------------
!
!  zspTimer0GetTimer - Get value of timer0 register
!  zspTimer1GetTimer - Get value of timer1 register
!
!  PURPOSE:
!
!     This function gets the value of on-chip timer X.
!
!  PROTOTYPE:
!
!     UINT16 zspTimer0GetTimer( void );
!     UINT16 zspTimer1GetTimer( void );
!
!  PARAMETERS:
!
!     None.
!
!  RETURNS:
!
!     timerX register value
!
!  PSEUDOCODE:
!
!     Return %timerX
!
!  NOTES:
!
!-----------------------------------------------------------------------------
   .walign 4

_zspTimer0GetTimer:

! - - - START PROLOGUE - - -
 
! No registers to save to stack

! No local frame to allocate (no local variables, no argument block)

! - - - END PROLOGUE - - -

! Move timer count to r4

   mov      r4, %timer0                !
 
! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! No registers to restore from stack
   
! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimer0GetTimer
   .walign 4

_zspTimer1GetTimer:

! - - - START PROLOGUE - - -
 
! No registers to save to stack

! No local frame to allocate (no local variables, no argument block)

! - - - END PROLOGUE - - -

! Move timer count to r4

   mov      r4, %timer1                !
 
! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! No registers to restore from stack
   
! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimer1GetTimer


!-----------------------------------------------------------------------------
!
!  zspTimer0SetTimerPrescaler - set timer0 prescale value
!  zspTimer1SetTimerPrescaler - set timer1 prescale value
!
!  PURPOSE:
!
!     This function initializes on-chip timer Xs prescale value
!
!  PROTOTYPE:
!
!     void zspTimer0SetTimerPrescaler( UINT16 prescaleValue );
!     void zspTimer1SetTimerPrescaler( UINT16 prescaleValue );
!
!  PARAMETERS:
!
!     prescaleValue => 6-bit prescale value
!
!  RETURNS:
!
!     Nothing
!
!  PSEUDOCODE:
!
!     Set tc register tmrdivX field to masked prescaleValue.
!
!  NOTES:
!
!-----------------------------------------------------------------------------
   .walign 4

_zspTimer0SetTimerPrescaler:

! - - - START PROLOGUE - - -

! Save registers to stack.

   stu      r5, r12, -1                !

! No local frame to allocate (no local variables, no argument block).

! - - - END PROLOGUE - - -

! Get timer control register

   mov      r5, %tc                    !

! Move prescale mask to r13

   mov      r13, 0x003F                !

! Mask prescaleValue

   and      r4, r13                    !

! Mov tc mask to r13

   mov     r13, 0xFFC0                 !

! Clear tmrdiv0 field

   and      r5, r13                    !

! Set timrdiv0 field

   or       r4, r5                     !

! MOve tc value back to tc

   mov      %tc, r4                    !

! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! Restore saved registers.

   add      r12, 1                     !
   ld       r5, r12                    !
   
! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimer0SetTimerPrescaler
   .walign 4

_zspTimer1SetTimerPrescaler:

! - - - START PROLOGUE - - -

! Save registers to stack.

   stu      r5, r12, -1                !

! No local frame to allocate (no local variables, no argument block).

! - - - END PROLOGUE - - -

! Get timer control register

   mov      r5, %tc                    !

! Move prescale mask to r13

   mov      r13, 0x003F                !

! Mask prescaleValue

   and      r4, r13                    !

! Shift prescale value left by 8

   shll     r4, 8                      !

! Mov tc mask to r13

   mov     r13, 0xC0FF                 !

! Clear tmrdiv1 field

   and      r5, r13                    !

! Set timrdiv1 field

   or       r4, r5                     !

! MOve tc value back to tc

   mov      %tc, r4                    !

! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! Restore saved registers.

   add      r12, 1                     !
   ld       r5, r12                    !
   
! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimer1SetTimerPrescaler


!-----------------------------------------------------------------------------
!
!  zspTimer0GetTimerPrescaler - get timer0 prescale value
!  zspTimer1GetTimerPrescaler - get timer1 prescale value
!
!  PURPOSE:
!
!     This function gets on-chip timer X prescale value
!
!  PROTOTYPE:
!
!     UINT16 zspTimer0GetTimerPrescaler( void );
!     UINT16 zspTimer1GetTimerPrescaler( void );
!
!  PARAMETERS:
!
!     None. 
!
!  RETURNS:
!
!     prescaleValue => 6-bit prescale value
!
!  PSEUDOCODE:
!
!     Return tc register tmrdivX field
!
!  NOTES:
!
!-----------------------------------------------------------------------------
   .walign 4

_zspTimer0GetTimerPrescaler:

! - - - START PROLOGUE - - -

! No registers to save to stack

! No local frame to allocate (no local variables, no argument block).

! - - - END PROLOGUE - - -

! Get timer control register

   mov      r4, %tc                    !

! Move prescale mask to r13

   mov      r13, 0x003F                !

! Mask prescaleValue

   and      r4, r13                    !

! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! No registers to restore from stack
   
! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimer0GetTimerPrescaler
   .walign 4

_zspTimer1GetTimerPrescaler:

! - - - START PROLOGUE - - -

! No registers to save to stack

! No local frame to allocate (no local variables, no argument block).

! - - - END PROLOGUE - - -

! Get timer control register

   mov      r4, %tc                    !

! Shift down tmrdiv1 field

   shrl     r4, 8                      !

! Move prescale mask to r13

   mov      r13, 0x003F                !

! Mask prescaleValue

   and      r4, r13                    !

! - - - START EPILOGUE - - -

! No local frame to deallocate (no local variables, no argument block)

! No registers to restore from stack
   
! - - - END EPILOGUE - - -

   ret                                 !
    
                                       ! _zspTimer1GetTimerPrescaler
