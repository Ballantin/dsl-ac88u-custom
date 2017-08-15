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
*    Filename: cp0Mips.s
*
****************************************************************************
*    Description:
*
*     MIPS-specific get and set functions for co-processor 0 (CP0) registers.
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
** FUNCTION:   cp0GetRegMips
**
** PURPOSE:    Get specified MIPS co-processor 0 (CP0) register.
**
** PARAMETERS: reg   (in)  CP0 register to get.
**             sel   (in)  Register select number (default is 0).
**
** RETURNS:    Register value.
**
** NOTE:
**    unsigned int cp0GetRegMips( unsigned int reg, unsigned int sel );
*****************************************************************************
*/
ASM_START(cp0GetRegMips)
   .set noreorder
        
   andi    a0, 31          # Limit reg to 0-31
   andi    a1, 7           # Limit sel to 0-7
   sll     a0, a0, 11      # Shift CP0 register number to correct place
   or      a0, a1          # Or in select number
   lw      v0, 1f          # Load "mfc0" instruction
   or      v0, a0          # Or in register number and select number
   la      a1, 2f          # Load address of temporary location
   lui     a0, 0xa000
   or      a1, a0          # Make it an un-cached address
   sw      v0, 0(a1)       # Store "mfc0" instruction
   sync
   jr      a1              # Jump to "mfc0" instruction
   nop
1:
   mfc0    v0, $0
2:
   nop
   nop
   nop
   jr      ra
   nop
ASM_END(cp0GetRegMips)


/*
*****************************************************************************
** FUNCTION:   cp0GetRegMips
**
** PURPOSE:    Set value of specified MIPS co-processor 0 (CP0) register.
**
** PARAMETERS: reg   (in)  CP0 register to set.
**             sel   (in)  Register select number (default is 0).
**             value (in)  New register value.
**
** RETURNS:    Nothing.
**
** NOTE:
**    void cp0SetRegMips( unsigned int reg, unsigned int sel, unsigned int value );
*****************************************************************************
*/
ASM_START(cp0SetRegMips)
   .set noreorder

   andi    a0, 31          # Limit reg to 0-31
   andi    a1, 7           # Limit sel to 0-7
   sll     a0, a0, 11      # Shift CP0 register number to correct place
   or      a0, a1          # Or in select number
   lw      v0, 1f          # Load "mtc0" instruction
   or      v0, a0          # Or in register number and select number
   la      a1, 2f          # Load address of temporary location
   lui     a0, 0xa000
   or      a1, a0          # Make it an un-cached address
   sw      v0, 0(a1)       # Store "mfc0" instruction
   sync
   jr      a1              # Jump to "mfc0" instruction
   nop
1:
   mtc0    a2, $0
2:
   nop
   nop
   nop
   jr      ra
   nop
ASM_END(cp0SetRegMips)
