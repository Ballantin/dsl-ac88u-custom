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
*  @file    strMaxCat.c 
*
*  @brief   Implements strMaxCat, a bounded string concatenation routine.
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
*  Concatenates source to the destination, but makes sure that the 
*  destination string (including terminating null), doesn't exceed maxLen.
*
*  @param   dst      (mod) String to concatnate onto.
*  @param   src      (in)  String to being added to the end of @a dst.
*  @param   maxLen   (in)  Maximum length that @a dst is allowed to be.
*
*  @return  A pointer to the destination string.
*/

char *strMaxCat( char *dst, const char *src, size_t maxLen )
{
   size_t   dstLen = strlen( dst );

   if ( dstLen < maxLen )
   {
      strMaxCpy( &dst[ dstLen ], src, maxLen - dstLen );
   }

   return dst;

} /* strMaxCat */

/** @} */

