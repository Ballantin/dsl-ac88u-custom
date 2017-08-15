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

#if !defined( HAUS_CPU_H )
#define  HAUS_CPU_H

/*----- MIPS --------------------------------------------------------------*/

#if defined( __mips__ )

#define  HAUS_CPU_MIPS     1
#define  HAUS_CPU_RISC         1

#elif defined( _M_IX86 ) || defined( _X86 )

#define  HAUS_CPU_X86      1

/*----- ZSP ----------------------------------------------------------------*/

#elif defined( __sdsp__ )

#define  HAUS_CPU_ZSP         1

/*----- ARM ----------------------------------------------------------------*/

#elif defined( __arm__ )

#define  HAUS_CPU_ARM         1
#define  HAUS_CPU_RISC         1

/*----- Something we don't recognize ---------------------------------------*/

#else

/*
 * This no longer causes an error, since our clients host application may
 * not be running on a platform that we build for.
 */
#define  HAUS_CPU_UNKNOWN  1

#endif

#endif   /* HAUS_CPU_H  */

