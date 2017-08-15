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
*    Filename: memcpy.c
*
****************************************************************************
*    Description:
*
*     Implements standard C run-time library function memcpy.
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
** FUNCTION:   memcpy
**
** PURPOSE:    Standard C run-time library function memcpy().
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
void *memcpy( void *dst, const void *src, size_t count )
{
   char        *cdst = (char *)       dst;
   const char  *csrc = (const char *) src;

   for ( ; count > 0; count-- )
   {
      *cdst++ = *csrc++;
   }

   return dst;
}
