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
*    Filename: handshake.s
*
****************************************************************************
*    Description:
*
*     Implements a primitive "hand-shaking" API that can be used to 
*     send messages between the MIPS thread-processors on the 6368. The
*     6368 uses a BMIPS 4350 core which contains shared CP0 register 31. 
*     This single register is used to send messages in both directions.
*     Therefore, it is important for the application code to use a handshake
*     method to send messages to prevent the thread-processors from trampling
*     each other's messages. Also, there is no concurrency protection provided
*     by this module. Message should be sent from the context of a single task.
* 
*     CP0 register 31 is the debug exception save register. Therefore, using
*     this module may prevent a debugger from operating correctly. It would
*     have been nice to have dedicated registers for thread-processor 
*     communication... 
*
****************************************************************************/


/* ---- Include Files ----------------------------------------------------- */

#include <mipsDefines.h>

##** 
##** The variable itcpMailbox is a shared variable for 
##** communication between the two TPs. The handshaking procedure 
##** on both TPs use itpcMailbox to pass data.
##**
.extern itpcMailbox; 


/* ---- Public Variables -------------------------------------------------- */
/* ---- Private Constants and Types --------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
/* ---- Private Function Prototypes --------------------------------------- */
/* ---- Functions --------------------------------------------------------- */


##************************************************************************
##*     handshakeSendMsg:                                                *
##*                                                                      *
##*      SYNTAX: void handshakeSendMsg( int msg );                       *
##*     RETURNS: Nothing                                                 *
##*                                                                      *
##************************************************************************
ASM_START(handshakeSendMsg)
    .set    noreorder
    
    // Copy messages to the itpc mailbox variable shared between the two TPs
    la      t0, itpcMailbox
    sw      a0, 0(t0)
    nop
    nop
    nop
    
    j       ra
    nop

ASM_END(handshakeSendMsg)
    

##************************************************************************
##*     handshakeGetMsg:                                                 *
##*                                                                      *
##*      SYNTAX: void handshakeGetMsg( int *msg );                       *
##*     RETURNS: Nothing                                                 *
##*                                                                      *
##************************************************************************
ASM_START(handshakeGetMsg)
    .set    noreorder
    
    // Retrieve messages from the itpc mailbox variable shared between the two TPs
    la      t0, itpcMailbox
    lw      t1, 0(t0)
    sw      t1, 0(a0)
    j       ra
    nop

ASM_END(handshakeGetMsg)
