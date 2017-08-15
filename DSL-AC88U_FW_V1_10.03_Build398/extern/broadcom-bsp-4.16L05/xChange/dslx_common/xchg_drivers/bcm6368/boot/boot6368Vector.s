/***************************************************************************
*
*  Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom Corporation
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom Corporation
*  company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: boot6368Vector.c
*
****************************************************************************
*    Description:
*
*     This file contains the exception/interrupt vector (and installation
*     code) used to bootload the secondary thread-processor on the 6368.
*
*     The secondary thread-processor will be signalled to reset by the
*     primary thread-processor via a software interrupt. This will cause
*     the secondary thread-processor to start executing the installed exception
*     vector. The exception vector will cause the thread-processor to jump
*     to it's application entry point.
*
****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <mipsDefines.h>


/* ---- Public Variables -------------------------------------------------- */

.extern TP0halResetEntry; 
.extern itpcMailbox; 

/* ---- Private Constants and Types --------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
/* ---- Private Function Prototypes --------------------------------------- */
/* ---- Functions --------------------------------------------------------- */


/*
** This code was lifted from the 6368 BFC BSP. It's currently not required 
** because the BSP installs the exception vector used for bootloading the
** secondary thread-processor. The code has been left in place for completeness,
** and in case other applications require it.
*/
#if 1

    .globl InstallExcpnHandler
    .ent InstallExcpnHandler
InstallExcpnHandler:
    .set    noreorder


    # TP0 will use a different exception/interrupt vector (0x80000200) so that
    # there is no risk of conflicts with the eCos vector at 0x80000180.  TP0
    # has already cleared the BEV bit and set the IV bit, so all I need to do
    # is install the alternate exception vector (copy the code).
    lui     t0, 0xa000
    ori     t0, t0, 0x0200
    la      t1, Tp0ExcpnHandler
    la      t2, EndOfTp0ExcpnHandler

InstallExcpnHandlerLoop:

    lw      t3, 0(t1)
    sw      t3, 0(t0)
    addiu   t0, t0, 4
    bne     t1, t2, InstallExcpnHandlerLoop
    addiu   t1, t1, 4
    
    jr       ra
    nop


//************************************************************************
    
Tp0ExcpnHandler:

    mfc0    k0, $13, 0
    xori    k0, k0, 0x0100
    mtc0    k0, $13, 0
    nop

    la        t8, TP0halResetEntry

    // Modify epc to point to DSP entrypoint. TP will jump to entrypoint after returning from interrupt
    // This code basically eliminates the need to copy the entrypoint to t8 along with the wait and jump
    // code executing in the ROM on the DSP Tp
    mtc0    t8, $14, 0 
        
    // Return from the exception.
    eret
    
EndOfTp0ExcpnHandler:
    nop
    
//************************************************************************

    .set    reorder
    .end    InstallExcpnHandler    

#endif /* #if 0 */

    
    
#************************************************************************
#*     boot6368SendIpc:                                                 *
#*                                                                      *
#*      SYNTAX: void boot6368SendIpc(void *pIpcStruct);                 *
#*     RETURNS: Nothing                                                 *
#*                                                                      *
#************************************************************************
    .globl  boot6368SendIpc
    .ent    boot6368SendIpc
boot6368SendIpc:
    .set    noreorder
    
    // Store the passed argument to shared itpc mailbox
    la      k0, itpcMailbox
    sw      a0, 0(k0)
    
    // Use s/w interrupt 0 to wake up TP0.
    mfc0    t0, $13, 0
    ori     t0, t0, 0x0100
    mtc0    t0, $13, 0
    
    j       ra
    nop

    .set    reorder
    .end    boot6368SendIpc    
