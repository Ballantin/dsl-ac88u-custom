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
*    Filename: stdio.h
*
****************************************************************************
*    Description:
*
*     C run-time library input/output functions.
*
****************************************************************************/

#ifndef STDIO_H
#define STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <stddef.h>


/* ---- Constants and Types ---------------------------------------------- */
typedef int FILE;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

int printf( const char *format, ... );



/* --------------------------------------------------------------------------
** Low-level I/O functions that must be implemented by the user.
*/

int crtWrite( int fd, char *buf, unsigned int lenBytes );



#ifdef __cplusplus
    }
#endif

#endif  /* STDIO_H  */
