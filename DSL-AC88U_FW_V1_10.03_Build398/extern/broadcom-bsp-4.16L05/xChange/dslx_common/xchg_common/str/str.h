/****************************************************************************
*
* Copyright (c) 2002 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    str.h
*
*  @brief   String Manipulation routines.
*
*  Definitions for a variety of string manipulation routine. These routines
*  are all bounded, to ensure that memory overwrites don't occur.
*
****************************************************************************/
/**
*  @defgroup   str   String Manipulation
*
*  @brief   Provides a variety of string manipulation routines.
*/

#if !defined( STR_H )
#define STR_H                 /**< Include Guard                           */

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

/* ---- Include Files ---------------------------------------------------- */

#if defined( __cplusplus )
extern "C"
{
#endif

/**
 * @addtogroup str
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * Pointer to a function which outputs a single character. This function
 * is called by the strXPrintf()/vStrXPrintf() functions to output a
 * character.
 */

typedef int (*strXPrintfFunc)( void *outParm, int ch );

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

char *strMaxCpy( char *dst, const char *src, size_t maxLen );
char *strMaxCat( char *dst, const char *src, size_t maxLen );
char *strCatSize( char *dst, const char *src, size_t len );

int strPrintf( char *outStr, int maxLen, const char *fmt, ... );
int vStrPrintf( char *outStr, int maxLen, const char *fmt, va_list args );

int strXPrintf( strXPrintfFunc func, void *userParm, const char *fmt, ... );
int vStrXPrintf( strXPrintfFunc func, void *userParm, const char *fmt, va_list args );

char* strCpyLower( char *dst, const char *src, size_t maxLen );
char* strCpyUpper( char *dst, const char *src, size_t maxLen );

int strCmpNoCase( const char *str1, const char *str2 );
int strCmpNoCaseSize( const char *str1, const char *str2, size_t n );

#define strTok( str, delim )   strTokDelim( str, delim, NULL )
char* strTokDelim( char **string, const char *delimiters, char *tokdelim );

int strScanf(const char *format, ...);
int strFScanf(FILE *stream, const char *format, ...);
int strSScanf(char *s, const char *format, ...);

/** @} */

#if defined( __cplusplus )
}
#endif

#endif /* STR_H */

