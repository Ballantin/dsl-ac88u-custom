/****************************************************************************
*
*  haus_cpu.h - Figures out which processor is being targeted
*
*  PURPOSE:
*     This file uses macros from the compiler to determine which CPU
*     we're running under and generate an appropriate HAUS_CPU_xxx
*     macro.
*
*            (c) Copyright 2004 Broadcom Canada Ltd.
*                       All Rights Reserved
*
****************************************************************************/

#if !defined( STDIO_LDX_H )
#define  STDIO_LDX_H

#include <xcfg_crt.h>

#if XCFG_CRT_USE_EXTERNAL_HEADER
#include <stdio.h>
#else
#include <stddef.h>

extern void printf(void);
#endif

/*----- MIPS --------------------------------------------------------------*/

#endif   /* STDIO_LDX.H  */

