/****************************************************************************
*
* Copyright � 2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************/


/***********************************************************************************
**
**    Description:
**
**      Performs case insensitive string compare on the specified amount of bytes.
**
***********************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "str.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   strCmpNoCaseSize
**
**  PURPOSE:    Performs case insensitive string comparison of the first 'n'
**              bytes of a given string.
**
**  PARAMETERS:
**              str1 - string to compare.
**              str2 - string to compare.
**              n    - number of bytes to be compared.
**
**  RETURNS:    0 if the strings are the same.
**              > 0 if str1 > str2.
**              < 0 if str1 < str2.
***********************************************************************************/
int strCmpNoCaseSize( const char *str1, const char *str2, size_t n )
{
   int       char1, char2;
   size_t    cnt = 1;

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

       cnt++;

   } while ( char1 && ( char1 == char2 ) && (cnt <= n) );


   return ( char1 - char2 );
}
