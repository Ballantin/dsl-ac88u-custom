/****************************************************************************
*
*  Copyright (c) 2003 Broadcom
*        All Rights Reserved
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom
*        16215 Alton Parkway
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  ldx_icp.h - Idle CPU Profiler (ICP) header file
*
*  PURPOSE:
*     This file contains the function prototypes for ICP.
****************************************************************************/

#ifndef LDX_ICP_H
#define LDX_ICP_H

/* ---- Include Files ---------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

void icpProfile( void );
void icpCount( volatile unsigned *exitFlag );
void icpSetCount( unsigned int count );
unsigned int icpGetCount( void );

#endif /* LDX_ICP_H */
