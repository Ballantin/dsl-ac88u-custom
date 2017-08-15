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
*    Filename: printf.c
*
****************************************************************************
*    Description:
*
*     Implements standard run-time library functions that are required by
*     the application since there is no run-time library linked into the
*     3368 image. Probably, LDX or some other common module should provide
*     an implementation of these. The versions provided here are not
*     optimized.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdio.h>
#include <str.h>
#include <string.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   printf
**
** PURPOSE:    Standard C run-time library function printf().
**
** PARAMETERS: format   (out) Output string format control.
**
** RETURNS:    Number of chars written, -1 on error.
**
** NOTE:
*****************************************************************************
*/
int printf( const char *format, ... )
{
   va_list  varArgList;
   char     outStr[ 256 ];
   int      numCharsOutput;

   va_start( varArgList, format );

   vStrPrintf( outStr, sizeof( outStr ), format, varArgList );

   va_end( varArgList );

   numCharsOutput = crtWrite( 0, /* file descriptor not used currently */
                              outStr,
                              strlen( outStr ));

   return ( numCharsOutput );
}
