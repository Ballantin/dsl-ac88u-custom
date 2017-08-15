/****************************************************************************
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
****************************************************************************/

#if !defined( INLINE_LDX_H )
#define  INLINE_LDX_H

#ifndef HAUS_COMPILER_H
#include <haus_compiler.h>
#endif

#if (defined(HAUS_COMPILER_VC) && (HAUS_COMPILER_VC==1)) || (defined(HAUS_COMPILER_ARM) && (HAUS_COMPILER_ARM==1))

/*
 * inline isn't supported when compiling C files, but __inline is (since
 * strictly speaking inline isn't a C feature)
 */

#if !defined( __cplusplus )
#define inline   __inline
#endif

#endif

#if !defined( INLINE )
#define INLINE   static inline
#endif


#endif
