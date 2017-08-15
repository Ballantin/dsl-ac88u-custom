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
*  @file    strCmpNoCase.c
*
*  @brief   Implements strCmpNoCase, a case insensitive string comparison.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "str.h"


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
*  Performs case insensitive string compare.
*
*  @param   str1  (in)  String to compare.
*  @param   str2  (in)  String to compare.
*
*  @return  return 0 if the strings are the same,
**          > 0 if str1 > str2 and < 0 if str1 < str2.
*/
int strCmpNoCase( const char *str1, const char *str2 )
{
   int      char1, char2;

   if (( str1 == NULL ) && ( str2 != NULL ))
   {
      return ( -1 );
   }

   if (( str1 != NULL ) && ( str2 == NULL ))
   {
      return ( 1 );
   }

   if (( str1 == NULL ) && ( str2 == NULL ))
   {
      return ( 0 );
   }

   do
   {
       if ( ( ( char1 = (unsigned char)(*( str1++ ))) >= 'A' ) && ( char1 <= 'Z' ) )
       {
           char1 -= ( 'A' - 'a' );
       }

       if ( ( ( char2 = (unsigned char)(*( str2++ ))) >= 'A' ) && ( char2 <= 'Z' ) )
       {
           char2 -= ( 'A' - 'a' );
       }

   } while ( char1 && ( char1 == char2 ) );


   return ( char1 - char2 );
}
/** @} */
