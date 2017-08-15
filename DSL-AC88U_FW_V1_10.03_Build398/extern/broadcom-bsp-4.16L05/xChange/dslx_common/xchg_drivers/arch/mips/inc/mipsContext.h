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
*    Filename: mipsContext.h
*
****************************************************************************
*    Description:
*
*     Defines data stuctures for MIPS specific registers that are saved
*     and restored during context switches (e.g. when an exception/interrupt
*     occurs).
*
*
****************************************************************************/

#ifndef MIPS_CONTEXT_H
#define MIPS_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/* MIPS register. */
typedef unsigned int MIPS_REGISTER;


/*
** Registers that define the state of the MIPS core.
*/
typedef struct MIPS_CONTEXT_REGS
{
   /* 32 general purpose registers (r0 - r31). */
   MIPS_REGISTER  gpRegs[32];

   /* CP0 registers. */
   MIPS_REGISTER  hi;
   MIPS_REGISTER  low;
   MIPS_REGISTER  vector;
   MIPS_REGISTER  status;
   MIPS_REGISTER  pc;
   MIPS_REGISTER  cachecontrol;
   MIPS_REGISTER  cause;
   MIPS_REGISTER  badvr;
   MIPS_REGISTER  prid;
   MIPS_REGISTER  config;

} MIPS_CONTEXT_REGS;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* MIPS_CONTEXT_H  */
