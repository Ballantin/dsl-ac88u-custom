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
*    Filename: string.h
*
****************************************************************************
*    Description:
*
*     C run-time library string functions.
*
****************************************************************************/

#ifndef STRING_H
#define STRING_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <stddef.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


void *memmove(void *dest, const void *source, size_t length);
void *memset( void *dest, int c, size_t count );
void *memcpy( void *dest, const void *src, size_t count );

size_t strlen( const char *string );


#ifdef __cplusplus
    }
#endif

#endif  /* STRING_H  */
