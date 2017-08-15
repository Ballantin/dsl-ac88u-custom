/****************************************************************************
*
* Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom Corporation
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom Corporation
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    strMaxCpy.c 
*
*  @brief   Implements strMaxCpy, a bounded string copy routine.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "str.h"
#include <string.h>

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
*  Copies the source to the destination, but makes sure that the 
*  destination string (including terminating null), doesn't exceed 
*  maxLen.
*
*  @param   dst      (out) Place to store the string copy.
*  @param   src      (in)  String to copy.
*  @param   maxLen   (in)  Maximum number of characters to copy into @a dst.
*
*  @return  A pointer to the destination string.
*/

char *strMaxCpy( char *dst, const char *src, size_t maxLen )
{
   if ( maxLen < 1 )
   {
      /*
       * There's no room in the buffer?
       *
       * VxWorks complains if the cast isn't present (it considers "" to 
       * be const char *
       */

      return (char *)"";
   }

   if ( maxLen == 1 )
   {
      /*
       * There's only room for the terminating null character
       */

      dst[ 0 ] = '\0';
      return dst;
   }

   /*
    * The Visual C++ version of strncpy writes to every single character
    * of the destination buffer, so we use a length one character smaller
    * and write in our own null (if required).
    */

   strncpy( dst, src, maxLen - 1 );
   if (( strlen( src ) + 1 ) >= maxLen )
   {
      /*
       * The string exactly fits, or probably overflows the buffer.
       * Write in the terminating null character since strncpy doesn't in
       * this particular case.
       *
       * We don't do this arbitrarily so that the caller can use a sentinel 
       * in the very end of the buffer to detect buffer overflows.
       */

      dst[ maxLen - 1 ] = '\0';
   }

   return dst;

} /* strMaxCpy */

/** @} */

