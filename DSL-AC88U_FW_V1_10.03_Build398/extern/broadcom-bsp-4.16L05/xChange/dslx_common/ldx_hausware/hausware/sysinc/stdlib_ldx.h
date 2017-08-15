#ifndef STDLIB_LDX_H
#define STDLIB_LDX_H

#include <xcfg_crt.h>

#if XCFG_CRT_USE_EXTERNAL_HEADER
#include <stdlib.h>
#else
extern int abs( int n );
extern int labs( long n );
#endif

#endif /* STDLIB_LDX_H */

