/***************************************************************************
*    Broadcom Corporation
*
****************************************************************************
*
*    Filename: exceptionVectorMips.s.
*
****************************************************************************
*    Description:
*
*     Implements MIPS low level exception/interrupt handling routines.
*     This code is based upon original source from eCos (hal_interrupt_vector.s), 
*     the Embedded Configurable Operating System.
*
****************************************************************************/

##=============================================================================
##
##   vectors.S
##
##   MIPS exception vectors
##
##=============================================================================
#####ECOSGPLCOPYRIGHTBEGIN####
## -------------------------------------------
## This file is part of eCos, the Embedded Configurable Operating System.
## Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
##
## eCos is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation; either version 2 or (at your option) any later version.
##
## eCos is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
## for more details.
##
## You should have received a copy of the GNU General Public License along
## with eCos; if not, write to the Free Software Foundation, Inc.,
## 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
##
## As a special exception, if other files instantiate templates or use macros
## or inline functions from this file, or you compile this file and link it
## with other works to produce a work based on this file, this file does not
## by itself cause the resulting work to be covered by the GNU General Public
## License. However the source code for this file must still be made available
## in accordance with section (3) of the GNU General Public License.
##
## This exception does not invalidate any other reasons why a work based on
## this file might be covered by the GNU General Public License.
##
## Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
## at http://sources.redhat.com/ecos/ecos-license/
## -------------------------------------------
#####ECOSGPLCOPYRIGHTEND####
##=============================================================================
#######DESCRIPTIONBEGIN####
##
## Author(s):    nickg
## Contributors:   nickg, dmoseley
## Date:   1998-02-04
## Purpose:   MIPS exception vectors
## Description:   This file defines the code placed into the exception
##              vectors. It also contains the first level default VSRs
##      that save and restore state for both exceptions and
##      interrupts.
##
######DESCRIPTIONEND####
##
##=============================================================================


##-----------------------------------------------------------------------------
## 
   
#include "exceptionVectorMips.inc"
#include <mipsDefines.h>   

   
#ifdef at
#undef at
#endif


##-----------------------------------------------------------------------------
## Startup code

   .text


##-----------------------------------------------------------------------------
## When using runtime vector setup, the exception initialization routine
## copies this vector into locations 0x80000000, 0x80000100, 0x80000180. and
## possibly 0x80000200.  This allows the image to be extracted into a binary
## which loads at _start and runs at the load address.
            
FUNC_START(__exc_default_exception_vector)
        hal_general_exception
FUNC_END(__exc_default_exception_vector)


##-----------------------------------------------------------------------------
## This function copies the exception vector function to each of the MIPS
## entry points. A pointer to the list of entry points may be specified
## by the first argument (a0). The list must be NULL terminated. 
## If a0 equals 0, then the default list of entry points will be used.
##
## It is expected that this function will be called very early in the chip
## initialization sequence. It does not assume that the C-environment is
## setup (stack pointer initialized, bss cleared, data section initialized etc).
##      
FUNC_START(exceptionInstallVectorsMips)

        ## Use the default exception entry point list if the user passed in
        ## a NULL parameter.
        bnez    a0, 5f
        move    t4, a0                             # Branch delay, load user specified list.
        la      t4, exc_ram_default_exception_list # This will only be excecuted if a NULL list was specified.

5:        
        ## Calculate size of exception vector entry point.
        la      t1, __exc_default_exception_vector
        la      t8, __exc_default_exception_vector_end
        subu    t8, t8, t1
        
3:
        lw      t0, 0(t4)
        beqz    t0, 2f
        addi    t4, 4

        ## Copy the default exception handler to the appropriate entry point.
        la      t1, __exc_default_exception_vector
        move    t2, t8  
1:
        lw      t3, 0(t1)
        sw      t3, 0(t0)
        addi    t0, 4
        sub     t2, 4
        bnez    t2, 1b
        addi    t1, 4

        ## Now invalidate the i-cache for the exception entry point.
        lw      t5, -4(t4)
        move    t6, t8
        
        li      t7, ICACHE_LINE_SIZE_BYTES
4:
        cache   0x10 /*icacheop( CACHE_ADDR_INVALIDATE )*/, 0(t5)
        addu    t5, t7
        subu    t6, t7
        bgtz    t6, 4b
        
        
        b       3b
        nop
2:      
        ## This generates the vector service routine (VSR) table. This is the
        ## lookup table of exception handlers. It is possible for this to be
        ## generated at compile-time, rather than run-time, in which case
        ## this call will no longer be needed.
        # Install default VSR table.
        hal_mon_init
         
        jr      ra
        nop
FUNC_END(exceptionInstallVectorsMips)
      
      
exc_ram_default_exception_list:
        .long   0xa0000000
        .long   0xa0000100
        .long   0xa0000180
        .long   0x00000000




##-----------------------------------------------------------------------------
## Default exception VSR.
## Saves machine state and calls external handling code.
   
FUNC_START(__exc_default_exception_vsr)

   # We enter here with all of the CPU state still
   # in its registers except:
   # K0 = vector index
   # K1 = address of this function

   move  k1,sp         # K1 = original SP
   
   addi  sp,sp,-mips_exception_decrement
            # space for registers + safety margin

   sw    k0,mipsreg_vector(sp)   # store vector

   # store GPRs
   .set   noat
   sgpr   0,sp
   sgpr   1,sp
   sgpr   2,sp
   sgpr   3,sp
   sgpr   4,sp
   sgpr   5,sp
   sgpr   6,sp
   sgpr   7,sp
   sgpr   8,sp
   sgpr   9,sp
   sgpr   10,sp
   sgpr   11,sp
   sgpr   12,sp
   sgpr   13,sp
   sgpr   14,sp
   sgpr   15,sp
   sgpr   16,sp
   sgpr   17,sp
   sgpr   18,sp
   sgpr   19,sp
   sgpr   20,sp
   sgpr   21,sp
   sgpr   22,sp
   sgpr   23,sp
   sgpr   24,sp
   sgpr   25,sp
#   sgpr   26,sp    # == K0
#   sgpr   27,sp    # == K1
   sgpr   28,sp     # == GP
#   sgpr   29,sp    # == SP
   sgpr   30,sp     # == FP
   sgpr   31,sp     # == RA
   .set   at
   
   mfhi  a0
   mflo  a1
   shi   a0,sp
   slo   a1,sp

   # K1 contains original SP
   ssp   k1,sp         # store in reg dump   
   
   # save remaining machine state registers   
   mfc0     t0,cause
   mfc0     t1,status
   mfc0     t2,cachectrl
   mvafc0   t3,badvr
   mfc0     t4,config
   mfc0     t5,prid
   mvafc0   t6,epc
   
   sw    t0,mipsreg_cause(sp)
   sw    t1,mipsreg_sr(sp)
   sw    t2,mipsreg_cachectrl(sp)
   sva   t3,mipsreg_badvr(sp)
   sw    t4,mipsreg_config(sp)
   sw    t5,mipsreg_prid(sp)
   sva   t6,mipsreg_pc(sp)

   # FPU not supported.
   #hal_fpu_save sp
   
   # The machine state is now all saved on the stack.

   
   # Load Global Pointer register.
   la    gp,_gp
   
   move  s0,sp            # save pointer to saved state

   addi   sp,sp,-mips_stack_frame_size   # make a null frame   

   # Need to set up back pointers etc. ???

   hal_cpu_except_enable         # reenable exceptions

   .extern   exceptionNotifyMips
   jal   exceptionNotifyMips   # call C code
   move  a0,s0            # arg0 = register dump (delay slot)
      
   
   j   exc_restore_state
   nop
            
FUNC_END(__exc_default_exception_vsr)



##------------------------------------------------------------------------------
## Default interrupt VSR.  Jump here from other_vector.
## Saves machine state and calls appropriate ISR. When done, calls
## interrupt_end() to finish up and possibly reschedule.

FUNC_START(__exc_default_interrupt_vsr)

   # We enter here with all of the CPU state still
   # in its registers except:
   # K0 = vector index
   # K1 = address of this function

   move  k1,sp         # K1 = original SP
      
   addi  sp,sp,-mips_exception_decrement
            # space for registers + safety margin

   sw    k0,mipsreg_vector(sp)   # store vector

   # store GPRs
   .set   noat
   sgpr   0,sp
   sgpr   1,sp
   sgpr   2,sp
   sgpr   3,sp
   sgpr   4,sp
   sgpr   5,sp
   sgpr   6,sp
   sgpr   7,sp
   sgpr   8,sp
   sgpr   9,sp
   sgpr   10,sp
   sgpr   11,sp
   sgpr   12,sp
   sgpr   13,sp
   sgpr   14,sp
   sgpr   15,sp
   sgpr   16,sp
   sgpr   17,sp
   sgpr   18,sp
   sgpr   19,sp
   sgpr   20,sp
   sgpr   21,sp
   sgpr   22,sp
   sgpr   23,sp
   sgpr   24,sp
   sgpr   25,sp
#   sgpr   26,sp   # == K0
#   sgpr   27,sp   # == K1
   sgpr   28,sp    # == GP
#   sgpr   29,sp   # == SP
   sgpr   30,sp    # == FP
   sgpr   31,sp    # == RA
   .set   at
   
   mfhi  a0
   mflo  a1
   shi   a0,sp
   slo   a1,sp

   # K1 contains original SP
   ssp   k1,sp         # store in reg dump   
      
   mfc0     t0,cause
   mfc0     t1,status
   mfc0     t2,cachectrl
   mvafc0   t3,epc
   
   sw    t0,mipsreg_cause(sp)
   sw    t1,mipsreg_sr(sp)
   sw    t2,mipsreg_cachectrl(sp)
   sva   t3,mipsreg_pc(sp)

   # FPU not supported.
   #hal_fpu_save sp
      
   # The machine state is now all saved on the stack.

   # Load Global Pointer register.
   la   gp,_gp
   

   move  s0,sp            # save pointer to saved state

   subu   sp,sp,mips_stack_frame_size   # make a null frame   

   ## Save pointer to processor context state. This may be used by ISRs
   ## for debugging purposes.
   la    t2, exc_interrupt_context
   sw    s0, 0(t2)
   
   # Need to set up back pointers etc. ???

   # Decode external interrupt via interrupt controller

   hal_intc_decode   s2

   # Here, s2 contains the number of the interrupt being serviced,
   # we need to derive from that the vector number to call in the ISR
   # table.
   
   hal_intc_translate s2,s1
   
   # Here s1 is the number of the vector to be called and s2 is
   # the number of the interrupt being serviced. 

     
#if 0

   # Call cyg_instrument to record that this interrupt is being raised.
      
   li    a0,0x0301         # a0 = type = INTR,RAISE
   move  a1,s1            # a1 = vector number
   jal   cyg_instrument         # call instrument function
   move  a2,s2            # a2 = interrupt number
#endif

   sll   s1,s1,2            # s1 = byte offset of vector

   hal_cpu_except_enable         # reenable exceptions
         
   la    t2,exc_interrupt_handlers   # handler table
   add   t2,t2,s1         # address of ISR ptr
   lw    t2,0(t2)         # ISR pointer

   la    a1,exc_interrupt_data      # data table
   add   a1,a1,s1         # address of data ptr
   lw    a1,0(a1)         # Data pointer

   move  a0,s2            # pass interrupt number

   jalr  t2            # call ISR via t2
   nop               # (delay slot)

   
exc_restore_state:

   # All done, restore CPU state and continue

   addi  sp,sp,mips_stack_frame_size   # retrieve CPU state ptr

   # Disable interrupts again while we restore state. 
   hal_cpu_int_disable

   # FPU not supported.
   #hal_fpu_load sp

   lw    t0,mipsreg_cachectrl(sp)
   lhi   t1,sp
   llo   t2,sp

   mtc0  t0,cachectrl
   mthi  t1
   mtlo  t2

   # load GPRs
   .set   noat
#   lgpr   0,sp
   lgpr   1,sp
   lgpr   2,sp
   lgpr   3,sp
   lgpr   4,sp
   lgpr   5,sp
   lgpr   6,sp
   lgpr   7,sp
   lgpr   8,sp
   lgpr   9,sp
   lgpr   10,sp
   lgpr   11,sp
   lgpr   12,sp
   lgpr   13,sp
   lgpr   14,sp
   lgpr   15,sp
   lgpr   16,sp
   lgpr   17,sp
   lgpr   18,sp
   lgpr   19,sp
   lgpr   20,sp
   lgpr   21,sp
   lgpr   22,sp
   lgpr   23,sp
   lgpr   24,sp
   lgpr   25,sp
#   lgpr   26,sp   # == K0
#   lgpr   27,sp   # == K1
   lgpr   28,sp   # == GP
#   lgpr   29,sp   # == SP
   lgpr   30,sp   # == FP
   lgpr   31,sp   # == RA
   .set   at

   lw   k1,mipsreg_sr(sp)      # K1 = saved SR

#if 1  /* HAL_MIPS_INTERRUPT_RETURN_KEEP_SR_IM */
   # Keep the current settings of the IM[7:0] bits within the status
   # register.  These may be used as interrupt masks, so if an ISR or
   # DSR masks interrupts they must be preserved.
   # If they are not used, then this does no harm.
   ori   k0,zero,0xff00
   nor   k0,k0,k0         # 0xffff00ff
   and   k1,k1,k0         # all interrupts disabled

   mfc0  k0,status         # V0 = current SR
   nop
   nop
   andi  k0,k0,0xff00         # preserve interrupt set
   or    k1,k1,k0         # insert into "saved SR"
#endif /* HAL_MIPS_INTERRUPT_RETURN_KEEP_SR_IM */
   
   lva   k0,mipsreg_pc(sp)      # K0 = return PC
   lsp   sp,sp            # load SP

   # Invoke CPU specific mechanism for returning from this
   # exception
   
   hal_cpu_eret k0,k1
      
FUNC_END(__exc_default_interrupt_vsr)

   hal_intc_decode_data

   

##-----------------------------------------------------------------------------
## Default interrupt handler. This is invoked by the interrupt VSR, which 
## is responsible for the context save/restore of register values.
##
FUNC_START(exc_default_isr)

   ## If we reach here, it likely means that we have received a spurious interrupt.
   ## We should probably keep a counter, or some other mechanism to keep track
   ## of this type of event...for now, just keep going...
   
   jr ra
   nop


FUNC_END(exc_default_isr)


##------------------------------------------------------------------------------
## Interrupt vector tables.
## These tables contain the isr, and data used to deliver interrupts to user code.

   
   .extern exc_default_isr
   
   .data

   .globl   exc_interrupt_handlers
exc_interrupt_handlers:
   .long   exc_default_isr
   .long   exc_default_isr
   .long   exc_default_isr
   .long   exc_default_isr
   .long   exc_default_isr
   .long   exc_default_isr
   .long   exc_default_isr
   .long   exc_default_isr


   .globl   exc_interrupt_data
exc_interrupt_data:
   .rept   8
   .long   0
   .endr

   
   .globl   exc_interrupt_context
exc_interrupt_context:
   .long   0
   
   

## This could be used to generate the vector service routine (VSR) table at
## compile time. Currently, it is generated at run-time. The VSR table 
## contains the lookup table of exception handlers.
#if 1
##-----------------------------------------------------------------------------
## VSR table.
## The main interrupt code indirects through here to find the VSR
## to execute for each architecture defined interrupt.
   
##   .section ".vsr_table","a"
   
   .data
         
   .globl   exc_vsr_table

exc_vsr_table:   
   .long   __exc_default_interrupt_vsr   
   .rept   31
   .long   __exc_default_exception_vsr   
   .endr
#endif
   

   
##-----------------------------------------------------------------------------
## end of vectors.S
