/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: intCtrlRegsMips.h
*
****************************************************************************
*    Description:
*
*     Interface for MIPS interrupt control interface for manipulating the
*     CP0 registers for masking and unmasking interrupts.
*
****************************************************************************/

#ifndef INT_CTRL_REGS_MIPS_H
#define INT_CTRL_REGS_MIPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



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
** NOTE:
*****************************************************************************
*/
void intCtrlUnmaskMips( unsigned int interruptId );


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
** NOTE:
*****************************************************************************
*/
void intCtrlMaskMips( unsigned int interruptId );


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
** NOTE:
*****************************************************************************
*/
void intCtrlClearMips( unsigned int interruptId );


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
*****************************************************************************
*/
void intCtrlGenerateSoftInterruptMips( unsigned int interruptId );


#ifdef __cplusplus
    }
#endif

#endif  /* INT_CTRL_REGS_MIPS_H  */
