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
*    Filename: memset.c
*
****************************************************************************
*    Description:
*
*     Implements standard C run-time library function memset().
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
** FUNCTION:   memset
**
** PURPOSE:    Standard C run-time library function memset().
**
** PARAMETERS: dest  (out) Memory to fill.
**             c     (in)  Character to fill memory with.
**             count (in)  Number of characters to fill.
**
** RETURNS:    Pointer to destination memory.
**
** NOTE:
*****************************************************************************
*/
void *memset( void *dest, int c, size_t count )
{
   char *s = (char *) dest;

   while (count-- != 0)
   {
      *s++ = (char) c;
   }

   return dest;
}
