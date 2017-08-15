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
*    Filename: handshake.h
*
****************************************************************************
*    Description:
*
*     Implements a primitive "hand-shaking" API that can be used to
*     send messages between the MIPS thread-processors on the 6368 and 
*     other dual-core MIPS processors (e.g. 6358, 6362, 6816). The
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

#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


void handshakeSendMsg( int msg );
void handshakeGetMsg( int *msg );



#ifdef __cplusplus
    }
#endif

#endif  /* HANDSHAKE_H  */
