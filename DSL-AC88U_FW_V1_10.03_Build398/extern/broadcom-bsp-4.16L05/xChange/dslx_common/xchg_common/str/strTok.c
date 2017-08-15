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
*  @file    strToken.c
*
*  @brief   Implements strToken, a thread-safe version of the standard ANSI
*           strtok function.
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
*  Tokenizes a string. This function is conceptually similiar to ANSI C strtok(),
*  but allows strToken() to be used by different strings or callers at the same
*  time. Each call modifies '*string' by substituting a NULL character for the
*  first delimiter that is encountered, and updates 'string' to point to the char
*  after the delimiter. Leading delimiters are skipped.
*
*  @param   string      (mod) Ptr to string ptr, updated by token.
*  @param   delimiters  (in)  Set of delimiter characters.
*  @param   tokdelim    (out) Character that delimits the returned token. (May
**                            be set to NULL if token delimiter is not required).
*
* @return  Pointer to the next token found. NULL when no more tokens are found.
*
*/
char* strTokDelim( char **string, const char *delimiters, char *tokdelim )
{
   char *str;
   unsigned long map[8];
   int count;
   char *nextoken;

   if ( tokdelim != NULL )
   {
      /* Prime the token delimiter */
      *tokdelim = '\0';
   }

   /* Clear control map */
   for (count = 0; count < 8; count++)
   {
      map[count] = 0;
   }

   /* Set bits in delimiter table */
   do
   {
      map[*delimiters >> 5] |= (1 << (*delimiters & 31));
   }
   while (*delimiters++);

   str = *string;

   /* Find beginning of token (skip over leading delimiters). Note that
   ** there is no token iff this loop sets str to point to the terminal
   ** null (*str == '\0')
   */
   while (( (map[*str >> 5] & (1 << (*str & 31))) && *str) || (*str == ' '))
   {
      str++;
   }

   nextoken = str;

   /* Find the end of the token. If it is not the end of the string,
   ** put a null there.
   */
   for ( ; *str ; str++ )
   {
      if ( map[*str >> 5] & (1 << (*str & 31)) )
      {
         if ( tokdelim != NULL )
         {
            *tokdelim = *str;
         }

         *str++ = '\0';
         break;
      }
   }

   *string = str;

   /* Determine if a token has been found. */
   if ( nextoken == (char *) str )
   {
      return NULL;
   }
   else
   {
      return nextoken;
   }
}

/** @} */
