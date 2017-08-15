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
*****************************************************************************/
#ifndef ASSERT_LDX_H
#define ASSERT_LDX_H

#include <xcfg_assert.h>

/*-------------------------------------------------------------------------
* Definitions for ASSERT, VERIFY, VERIFY_IF
* ASSERT_2, ASSERT_3, ASSERT_4 and ASSERT_VERBOSE
*
* ASSERT should never contain any code which does useful work.
*
* VERIFY is like ASSERT, but it can contain code which does useful work.
*
* VERIFY_IF behaves like: VERIFY(expr); if (expr)
* This allows a block of code to follow it and only be executed if the
* condition is true.
*
* Example:
*
*     VERIFY_IF(( ptr = SomeAlloc( len )) != NULL )
*     {
*        ...Code that uses ptr...
*     }
*
* ASSERT_n are n parameter asserts. These asserts are added to optimize
* code space.
*
* ASSERT_VERBOSE is like ASSERT, except that by default is disabled.
* To enable this class of asserts, you need to pass the symbol
* HAUS_ENABLE_ASSERT_VERBOSE on the build line.
*/
#if defined( HAUS_NO_ASSERT ) && HAUS_NO_ASSERT
#  undef  ASSERT
#  undef  VERIFY
#  undef  VERIFY_IF

#  define ASSERT(expr)      ((void)0)
#  define VERIFY(expr)      ((void)(expr))
#  define VERIFY_IF(expr)   if (expr)
#endif   /* HAUS_NO_ASSERT */


/*
 * Define verbose assert
 */

#if defined( HAUS_ENABLE_ASSERT_VERBOSE ) && HAUS_ENABLE_ASSERT_VERBOSE
#  define ASSERT_VERBOSE( expr )    ASSERT( expr )
#else
#  define ASSERT_VERBOSE( expr )    ((void)(0))
#endif

/* Compiler assert  */
#define CompilerAssert(expr, x) extern UINT16 _CompilerAssertVar ## x[(expr)? 1:-1];


#endif /* ASSERT_LDX_H */
