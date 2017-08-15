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
*    Filename: strlen.c
*
****************************************************************************
*    Description:
*
*     Implements standard C run-time library function strlen.
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
** FUNCTION:   strlen
**
** PURPOSE:    Standard C run-time library function strlen().
**
** PARAMETERS: string   (in) NULL terminated string.
**
** RETURNS:    Number of characters in string.
**
** NOTE:
*****************************************************************************
*/
size_t strlen( const char *string )
{
   int n;

   if ( string == NULL )
   {
     return ( 0 );
   }

   for ( n = 0; *string; string++ )
   {
      n++;
   }

   return( n );
}
