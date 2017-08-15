/* crypto/aes/aes.h -*- mode:C; c-file-style: "eay" -*- */
/* ====================================================================
 * Copyright (c) 1998-2002 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 */

///////////////////////////////////////////////////////////////////////////////
// Third-Party Code Modification
// Executed by M5T
///////////////////////////////////////////////////////////////////////////////
//
// Date : 2007-10-11
// Problem Description: Symbols appearing in API ref.
// Modification Description: Put the entire file between documentation comments.
//
///////////////////////////////////////////////////////////////////////////////
//M5T_INTERNAL_USE_BEGIN
///////////////////////////////////////////////////////////////////////////////
// End of Modification
///////////////////////////////////////////////////////////////////////////////

#ifndef HEADER_AES_LOCL_H
#define HEADER_AES_LOCL_H

///////////////////////////////////////////////////////////////////////////////
// Third-Party Code Modification
// Executed by M5T
///////////////////////////////////////////////////////////////////////////////
//
// Date : 2004-10-05
// Problem Description: This code is extracted from openssl, therefore,
//     dependencies need to be updated
// Modification Description: Dependencies were commented out and
//     managed another way.
//
///////////////////////////////////////////////////////////////////////////////
#if 0 /////////////////////////////////// START_COMMENTING_OUT
    #include <openssl/e_os2.h>

    #ifdef OPENSSL_NO_AES
    #error AES is disabled.
    #endif

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

//    #if defined(_MSC_VER) && !defined(_M_IA64) && !defined(OPENSSL_SYS_WINCE)
#endif ////////////////////////////////// END_COMMENTING_OUT

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#if defined(MXD_OS_WINDOWS)
///////////////////////////////////////////////////////////////////////////////
// End of Modification
///////////////////////////////////////////////////////////////////////////////

# define SWAP(x) (_lrotl(x, 8) & 0x00ff00ff | _lrotr(x, 8) & 0xff00ff00)
# define GETU32(p) SWAP(*((u32 *)(p)))
# define PUTU32(ct, st) { *((u32 *)(ct)) = SWAP((st)); }
#else
# define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
# define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }
#endif

///////////////////////////////////////////////////////////////////////////////
// Third-Party Code Modification
// Executed by M5T
///////////////////////////////////////////////////////////////////////////////
//
// Date : 2004-10-05
// Problem Description: This code run on architectures where unsigned long
//     is 40 bits which increase memory requirements.
// Modification Description:  This var has to be at least 32 bits wide.
//     Therefore, unsigned long was changed to unsigned int.
//
///////////////////////////////////////////////////////////////////////////////
#if 0 /////////////////////////////////// START_COMMENTING_OUT
    typedef unsigned long u32;
#endif ////////////////////////////////// END_COMMENTING_OUT
typedef unsigned int u32;
///////////////////////////////////////////////////////////////////////////////
// End of Modification
///////////////////////////////////////////////////////////////////////////////

typedef unsigned short u16;
typedef unsigned char u8;

#define MAXKC   (256/32)
#define MAXKB   (256/8)
#define MAXNR   14

/* This controls loop-unrolling in aes_core.c */
#undef FULL_UNROLL

#endif /* !HEADER_AES_LOCL_H */

///////////////////////////////////////////////////////////////////////////////
// Third-Party Code Modification
// Executed by M5T
///////////////////////////////////////////////////////////////////////////////
//
// Date : 2007-10-11
// Problem Description: Symbols appearing in API ref.
// Modification Description: Put the entire file between documentation comments.
//
///////////////////////////////////////////////////////////////////////////////
//M5T_INTERNAL_USE_END
///////////////////////////////////////////////////////////////////////////////
// End of Modification
///////////////////////////////////////////////////////////////////////////////
