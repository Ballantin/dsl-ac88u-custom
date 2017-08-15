/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  haus_types.h - HausWare type definitions
*
*  PURPOSE:
*     This file contains type and macro definitions for HAPI.
*
*
*        (c) Copyright 1999 Broadcom Canada Limited
*                      All Rights Reserved
*
****************************************************************************/

#ifndef HAUS_TYPES_H
#define HAUS_TYPES_H

#include <hapi_types.h>

/*-------------------------------------------------------------------------
* Figure out what environment we're running in.
*/

#if !defined( HAUS_COMPILER_H )
#include <haus_compiler.h>
#endif

#include <stddef.h>        /* ANSI-C Standard Definitions   */
#include <limits.h>        /* C-type limit defines          */

/*
 * Code that uses CHAR should be changed to use char. The pSOS apdialog.h
 * file references CHAR in a manner that conflicts with this.
 *
 * typedef char            CHAR;       * WARNING: CHAR is platform dependant *
 */


#if !defined( HAUS_UINT16_DEFINED )
#define HAUS_UINT16_DEFINED
typedef HAPI_UINT16  UINT16;       /* UINT16 is platform independant */
#endif

#if !defined( HAUS_SINT16_DEFINED )
#define HAUS_SINT16_DEFINED
typedef HAPI_SINT16    SINT16;     /* SINT16 is platform independant */
#endif

#if !defined( HAUS_UINT32_DEFINED )
#  define HAUS_UINT32_DEFINED
typedef HAPI_UINT32   UINT32;      /* UINT32 is platform independant */
#endif

#if !defined( HAUS_SINT32_DEFINED )
#  define HAUS_SINT32_DEFINED
typedef HAPI_SINT32     SINT32;    /* SINT32 is platform independant */
#endif

#define MAX_UINT16      USHRT_MAX
#define MIN_SINT16      SHRT_MIN
#define MAX_SINT16      SHRT_MAX
#define MAX_UINT32      ULONG_MAX
#define MIN_SINT32      LONG_MIN
#define MAX_SINT32      LONG_MAX


#if defined( HAUS_OS_LINUXKERNEL )

#include <linux/kernel.h>

#undef   HAUS_UINT8_DEFINED
#undef   HAUS_UINT16_DEFINED
#undef   HAUS_UINT32_DEFINED
#undef   HAUS_BOOL_DEFINED

#else
#if (MAX_UINT16 != 0xffff)
#error Maximum UINT16 != 65535
#endif

#if (MAX_SINT16 != 0x7fff)
#error Maximum SINT16 != 32767
#endif

#if (MAX_UINT32 != 0xffffffff)
#error Maximum UINT32 != 4294967295
#endif

#if (MAX_SINT32 != 0x7fffffff)
#error Maximum SINT32 != 2147483647
#endif

#endif /* HAUS_OS_LINUX_KERNEL */


#define VECTOR                      /* only have Vector Register File in Spice C */
#define RESTRICT                    /* make restrict keyword platform independent */


/* We can't always use "typedef void VOID", since void has no size.        */

#define  VOID  void


#if !defined( HAUS_UINT8_DEFINED )
#define HAUS_UINT8_DEFINED
typedef unsigned char   UINT8;      /* platform may not support 8-bit physical representation */
#endif

#if !defined( HAUS_SINT8_DEFINED )
#define HAUS_SINT8_DEFINED
typedef signed char     SINT8;      /* platform may not support 8-bit physical representation */
#endif

#if !defined( HAUS_FASTINT_DEFINED )
#define HAUS_FASTINT_DEFINED
typedef int             FASTINT;    /* Used for loop constructs, platform independent */
#endif

#if !defined( HAUS_FASTUINT_DEFINED )
#define HAUS_FASTUINT_DEFINED
typedef unsigned int    FASTUINT;   /* Used for loop constructs, platform independent */
#endif

#if defined( HAUS_COMPILER_GNU ) || defined ( HAUS_COMPILER_ARM )

#if !defined( HAUS_SINT40_DEFINED )
#define HAUS_SINT40_DEFINED
typedef long long       SINT40;     /* at least 40 bits. */
#endif

#if !defined( HAUS_SINT64_DEFINED )
#define HAUS_SINT64_DEFINED
typedef long long       SINT64;
#endif

#define __int64 long


#elif defined( HAUS_COMPILER_VC )

#if !defined( HAUS_SINT40_DEFINED )
#define HAUS_SINT40_DEFINED
typedef __int64         SINT40;     /* Microsoft compiler specific, signed 40 bit type defined as 64 bit value */
#endif

#if !defined( HAUS_SINT64_DEFINED )
#define HAUS_SINT64_DEFINED
typedef __int64         SINT64;
#endif

#endif


/*-------------------------------------------------------------------------
* Definitions for TRUE and FALSE
*/

/*
 * The TI Compiler doesn't think that statements like
 *
 * BOOL foo( int x )
 * {
 *    return x == 3;
 * }
 *
 * are compatible with an enumerated type, so BOOL has been changed to
 * an int.
 */

#if !defined( HAUS_BOOL_DEFINED )
#define HAUS_BOOL_DEFINED
typedef int   BOOL;
#endif

#if !defined( TRUE ) || ( TRUE != 1 )
#define  TRUE  1
#endif

#if !defined( FALSE ) || ( FALSE != 0 )
#define  FALSE 0
#endif


#define SIZE_IN_WORDS( size ) (((size) + sizeof( UINT16 ) - 1 ) / sizeof( UINT16 ))
#define BYTES_TO_SIZE_T( bytes )   ( ((bytes)+ (3 - sizeof( UINT16)) - 1) / ( 3 - sizeof( UINT16 )))
#define SIZE_T_TO_BYTES( size )   ((size)* (3 - sizeof( UINT16)))
#define WORDS_TO_SIZE_T( words )   ( (words) *   sizeof( UINT16 ))
#define WORDS_TO_BYTES( size )   ( (size) << 1)


/*-------------------------------------------------------------------------
* ARRAY_LEN calculates the number of elements in a statically declared
* array (i.e. it doesn't work with pointers)
*/

#define ARRAY_LEN(array) ( sizeof( array ) / sizeof( array[ 0 ]))


/*-------------------------------------------------------------------------
* Macros to read and write 32 bit values into 16 bit holders
*/

#define WRITE_UINT32(src16, dst32) \
{ \
   dst32 = ((UINT32)src16##High<<16) + (UINT32)src16##Low; \
}

#define READ_UINT32(dst16, src32) \
{ \
   dst16##High = src32 >> 16;   \
   dst16##Low  = (UINT16)src32; \
}

#endif   /* HAUS_TYPES_H */
