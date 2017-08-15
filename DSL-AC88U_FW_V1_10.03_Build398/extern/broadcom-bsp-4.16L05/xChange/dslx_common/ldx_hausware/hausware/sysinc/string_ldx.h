#ifndef STRING_LDX_H
#define STRING_LDX_H

#include <xcfg_crt.h>

#if XCFG_CRT_USE_EXTERNAL_HEADER
#include <string.h>
#else
#include <inline_ldx.h>

extern void LDX_memset( void *, int, int );
extern void LDX_memcpy( void *, const void *, int );

#define memset(x, y, z)   LDX_memset(x, y, z)
#define memcpy(x, y, z)   LDX_memcpy(x, y, z)
extern void *memmove(void *dest, const void *src, int length);
#endif

#endif /* STRING_LDX_H */

