/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: memmove.c
*
****************************************************************************
*    Description:
*
*     Implements standard C run-time library function memmove().
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <string.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   memmove
**
** PURPOSE:    Standard C run-time library function memmove().
**
** PARAMETERS: dest     (out) Destination buffer.
**             source   (in)  Source buffer.
**             length   (in)  Buffer length to copy.
**
** RETURNS:    Pointer to destination buffer.
**
** NOTE:
*****************************************************************************
*/
void *memmove(void *dest, const void *source, size_t length)
{
   char       *dst = dest;
   const char *src = source;

   if (src < dst && dst < src + length)
   {
      /* Have to copy backwards */
      src += length;
      dst += length;
      while (length--)
      {
          *--dst = *--src;
      }
   }
   else
   {
      while (length--)
      {
         *dst++ = *src++;
      }
   }

   return ( dest );
}
