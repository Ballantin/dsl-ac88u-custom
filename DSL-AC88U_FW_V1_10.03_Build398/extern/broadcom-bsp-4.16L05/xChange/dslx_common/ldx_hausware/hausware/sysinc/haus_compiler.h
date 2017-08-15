/****************************************************************************
*
*  haus_compiler.h - Figures out which compiler we're running under
*
*  PURPOSE:
*     This file uses macros from the compiler to determine which compiler
*     we're running under and generate an appropriate HAUS_COMPILER_xxx
*     macro.
*
*  NOTE:
*
*            (c) Copyright 1999-2000 Broadcom Canada Ltd.
*                             All Rights Reserved
*
****************************************************************************/

#if !defined( HAUS_COMPILER_H )
#define  HAUS_COMPILER_H

/*----- DCC (used on pSOS) ------------------------------------------------*/

#if defined( __DCC__ )
 
#define  HAUS_COMPILER_DCC    1
#define  HAUS_COMPILER_STRING "DCC"

/*----- GCC and variants --------------------------------------------------*/

#elif defined( __GNUC__ )
 
#define  HAUS_COMPILER_GNU    1
#define  HAUS_COMPILER_STRING "GNU"

/*----- Visual Studio C/C++ -----------------------------------------------*/

#elif defined( __arm__ )

#define  HAUS_COMPILER_ARM    1
#define  HAUS_COMPILER_STRING "ARM"

/*----- SPICE -------------------------------------------------------------*/

#elif defined( __SPICE_C__ )

#define  HAUS_COMPILER_SPICE  1
#define  HAUS_COMPILER_STRING "SPICE"

/*----- TI ----------------------------------------------------------------*/

#elif defined( _TMS320C5XX )

#error C54x no longer supported

/*----- Visual Studio C/C++ -----------------------------------------------*/

#elif defined( _MSC_VER )

#define  HAUS_COMPILER_VC     1
#define  HAUS_COMPILER_STRING "VC"
#define  LITTLE_ENDIAN        1

/*----- Something we don't recognize --------------------------------------*/

#elif defined( __DSP563C__ )
#define HAUS_COMPILER_MT 1
#define  HAUS_COMPILER_STRING "Mot56311"

#else

/*
 * This no longer causes an error, since our clients host application may
 * not be running on a platform that we build for.
 */

#define  HAUS_COMPILER_UNKNOWN   1
#define  HAUS_COMPILER_STRING    "Unknown"

#endif

/****************************************************************************
*
*/

#endif   /* HAUS_COMPILER_H   */

