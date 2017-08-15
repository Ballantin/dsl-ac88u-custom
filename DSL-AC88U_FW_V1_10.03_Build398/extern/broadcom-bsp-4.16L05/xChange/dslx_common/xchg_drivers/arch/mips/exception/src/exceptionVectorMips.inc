/***************************************************************************
*    Broadcom Corporation
*
****************************************************************************
*
*    Filename: exceptionVectorMips.inc
*
****************************************************************************
*    Description:
*
*     Implements MIPS low level exception/interrupt handling routines.
*     This code is based upon original source from eCos (hal_arch.inc), 
*     the Embedded Configurable Operating System.
*
****************************************************************************/

#ifndef EXCEPTION_VECTOR_MIPS_INC
#define EXCEPTION_VECTOR_MIPS_INC
##=============================================================================
##
##   arch.inc
##
##   MIPS assembler header file
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
## Date:   1997-10-16
## Purpose:   Architecture definitions.
## Description:   This file contains various definitions and macros that are
##              useful for writing assembly code for the MIPS CPU family.
## Usage:
##      #include <cyg/hal/arch.inc>
##      ...
##      
##
######DESCRIPTIONEND####
##
##=============================================================================


#include <mipsDefines.h>

          
/* Map name-space for MIPS CP0 regs. */
#define cause        C0_CAUSE          
#define status       C0_STATUS
#define cachectrl    $7
#define badvr        C0_BADVADDR
#define prid         C0_PRID
#define config       $3
#define epc          C0_EPC
                           
          


##-----------------------------------------------------------------------------
## CPU specific macros. These provide a common assembler interface to
## operations that may have CPU specific implementations on different
## variants of the architecture.

   # Disable interrupts
   .macro hal_cpu_int_disable
   mfc0  v0,status
   la    v1,0xFFFFFFFE
   and   v0,v0,v1
   mtc0  v0,status
   nop
   nop
   nop
   .endm   


   # Enable further exception processing, and disable
   # interrupt processing.
   .macro hal_cpu_except_enable
   mfc0  v0,status
   la    v1,0xFFFFFFF0
   and   v0,v0,v1      # clear EXL, ERL and IE bits   
   mtc0  v0,status
   nop
   nop
   nop
   .endm
   
   # Return from exception.
   .macro   hal_cpu_eret pc,sr
   .set mips3
   ori      \sr,\sr,2                       # prevent interrupts until eret
   mtc0     \sr,status         # put SR back
   nop
   nop
   nop
   mvatc0   \pc,epc            # put PC in EPC
   nop
   nop
   nop
   sync                 # settle things down
   eret                 # return
   nop                  # just to be safe
   .set mips0
   .endm

          
   # General exception or interrupt vector
   .macro hal_general_exception
   mfc0  k0,cause             # K0 = exception cause
   nop
   andi  k0,k0,0x7F           # isolate exception code
   srl   k0,k0,2              # drop 2 least significant bits
   la    k1,exc_vsr_table     # address of VSR table
   add   k1,k1,k0             # offset of VSR entry
   lw    k1,0(k1)             # k1 = pointer to VSR
   jr    k1                   # go there
   nop                        # (delay slot)
   .endm

   
##-----------------------------------------------------------------------------
## Default MIPS interrupt decoding macros. This uses the basic interrupt
## support provided by CP0 in the cause and status registers.
##

   .macro   hal_intc_decode vnum
   mfc0  v1,status                     # get status register (interrupt mask)
   nop                                 # delay slot
   mfc0  v0,cause                      # get cause register
   nop                                 # delay slot
   and   v0,v0,v1                      # apply interrupt mask
   srl   v0,v0,8                       # shift interrupt bits down
   andi  v0,v0,0xff                    # isolate 8 interrupt bits
   la    v1,exc_intc_translation_table
   add   v0,v0,v1                      # index into table
   lb    \vnum,0(v0)                   # pick up vector number
   .endm


   .macro   hal_intc_translate inum,vnum
   move   \vnum,\inum         # Vector == interrupt number
   .endm

    ## Software interrupt 0 has the highest priority. Followed by sw1, external
    ## interrupt0, 1, 2, 3, 4, and the timer interrupt, which has the lowest priority.
   .macro   hal_intc_decode_data
exc_intc_translation_table:   
   .byte   0, 0, 1, 0    
   .byte   2, 0, 1, 0    
   .byte   3, 0, 1, 0    
   .byte   2, 0, 1, 0    
   .byte   4, 0, 1, 0    
   .byte   2, 0, 1, 0    
   .byte   3, 0, 1, 0    
   .byte   2, 0, 1, 0    
   .byte   5, 0, 1, 0    
   .byte   2, 0, 1, 0    
   .byte   3, 0, 1, 0    
   .byte   2, 0, 1, 0    
   .byte   4, 0, 1, 0    
   .byte   2, 0, 1, 0    
   .byte   3, 0, 1, 0    
   .byte   2, 0, 1, 0    
   .byte   6, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   4, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   5, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   4, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   7, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   4, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   5, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   4, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   6, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   4, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   5, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   4, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .byte   3, 0, 1, 0              
   .byte   2, 0, 1, 0              
   .endm
   
   
   
##-----------------------------------------------------------------------------
## MIPS thread and interrupt saved state. This must match the layout of the
## HAL_SavedRegisters in hal_arch.h. Do not change this without changing the
## layout there, or viceversa.   

# Size of registers that change size between 32 and 64 bit implementations
#if 0
# define mips_regsize    8
#else
# define mips_regsize    4
#endif

# Size of registers that stay the same size in all implementations
# define mips_regsize32   4

#define  mipsreg_regs         0
#define  mipsreg_hi           (mips_regsize*32)
#define  mipsreg_lo           (mipsreg_hi+mips_regsize)
#define  mipsreg_vector       (mipsreg_lo+mips_regsize)
#define  mipsreg_sr           (mipsreg_vector+mips_regsize32)
#define  mipsreg_pc           (mipsreg_sr+mips_regsize32)
#define  mipsreg_cachectrl    (mipsreg_pc+mips_regsize)
#define  mipsreg_cause        (mipsreg_cachectrl+mips_regsize32)
#define  mipsreg_badvr        (mipsreg_cause+mips_regsize32)
#define  mipsreg_prid         (mipsreg_badvr+mips_regsize)
#define  mipsreg_config       (mipsreg_prid+mips_regsize32)
#define  mipsreg_size         (mipsreg_config+mips_regsize32)

# The following expression ensures that the decrement is always a
# multiple of 16 bytes. This is a requirement of the MEABI used in
# MIPS32/64 targets.

#define mips_exception_decrement   ((mipsreg_size*2)&~0xF)
   

##-----------------------------------------------------------------------------
## Minimal stack frame size uses to call functions from asm.

#define mips_stack_frame_size    32 // 4 (64 bit) args worth


##------------------------------------------------------------------------------
## Register save and restore macros. These expect a pointer to a CPU save state
## area in the register \ptr. The GPR indicated by \reg will be saved into its
## slot in that structure.


   .macro sgpr reg,ptr
   sw   $\reg,(mipsreg_regs+\reg*mips_regsize)(\ptr)
   .endm

   .macro lgpr reg,ptr
   lw   $\reg,(mipsreg_regs+\reg*mips_regsize)(\ptr)
   .endm

   .macro slo reg,ptr
   sw   \reg,(mipsreg_lo)(\ptr)
   .endm

   .macro shi reg,ptr
   sw   \reg,(mipsreg_hi)(\ptr)
   .endm

   .macro llo reg,ptr
   lw   \reg,(mipsreg_lo)(\ptr)
   .endm

   .macro lhi reg,ptr
   lw   \reg,(mipsreg_hi)(\ptr)
   .endm

   .macro ssp reg,ptr
   sw   \reg,(mipsreg_regs+29*mips_regsize)(\ptr)
   .endm

   .macro lsp reg,ptr
   lw   \reg,(mipsreg_regs+29*mips_regsize)(\ptr)
   .endm

   .macro sva reg,val
   sw   \reg,\val
   .endm

   .macro lva reg,val
   lw   \reg,\val
   .endm

   .macro mvafc0 gpr,cpr
   mfc0   \gpr,\cpr
   .endm

   .macro mvatc0 gpr,cpr
   mtc0   \gpr,\cpr
   .endm

   .macro lpc reg,ptr
   lw   \reg,(mipsreg_pc)(\ptr)
   .endm

   .macro spc reg,ptr
   sw   \reg,(mipsreg_pc)(\ptr)
   .endm
   
    
##------------------------------------------------------------------------------
## Monitor initialization.

   # If we are starting up from ROM, or we are starting in
   # RAM and NOT using a ROM monitor, initialize the VSR table.

   .macro   hal_mon_init
        # Set default exception VSR for all vectors
   ori    a0,zero,32  # Number of exceptions supported.
   la    a1,__exc_default_exception_vsr
   la    a2,exc_vsr_table
1: sw    a1,0(a2)
   addi  a2,a2,4
   addi  a0,a0,-1
   bne   a0,zero,1b
   nop

   # Now set special VSRs
   la   a0,exc_vsr_table
   
   # Set interrupt VSR
   la    a1,__exc_default_interrupt_vsr
   sw    a1,0*4(a0)  # CYGNUM_HAL_VECTOR_INTERRUPT
   
   # Add special handler on breakpoint vector to allow GDB and
   # GCC to both use 'break' without conflicts.
   #la   a1,__break_vsr_springboard
   #sw   a1,9*4(a0)  # CYGNUM_HAL_VECTOR_BREAKPOINT
   
   # Set exception handler on special vectors
   # FIXME: Should use proper definitions
   #la   a1,__default_exception_vsr
   #sw   a1,32*4(a0)             # debug
   #sw   a1,33*4(a0)             # utlb
   #sw   a1,34*4(a0)             # nmi
   .endm


/* --------------------------------------------------------------------------
** 
*/
   
#define FUNC_START(name)         \
        .type name,@function;    \
        .set push ;              \
        .globl name;             \
        .ent   name;             \
        .set noreorder ;         \
name:

#define FUNC_END(name)           \
name##_end:                      \
        .set pop;                \
        .end name
   
   
#endif /* EXCEPTION_VECTOR_MIPS_INC */
