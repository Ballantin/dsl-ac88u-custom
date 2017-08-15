/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    strCpyUpper.c
*
*  @brief   Implements strCpyUpper, an upper case string copy.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "str.h"
#include <ctype.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup str
 * @{
 */

/***************************************************************************/
/**
*  Performs an upper case string copy.
*
*  @param   dst      (out) String to copy into.
*  @param   src      (in)  String to copy from.
*  @param   maxLen   (in)  Maximum length that 'dst' is allowed to be.
*
*  @return  None.
*/
char* strCpyUpper( char *dst, const char *src, size_t maxLen )
{
   char *p = dst;

   while ( ( *src != '\0' ) && ( ( --maxLen ) > 0 ) )
   {
      *p++ = (char)toupper( *src );
      src++;
   }

   *p = '\0';

   return ( dst );
}

/** @} */
