/**********************************************************************************
** Copyright © 2008 Broadcom Corporation
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file implements the Telephone utility functions.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <utilApi.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */
/***********************************************************************************
**  FUNCTION:   utilTelRemNonDig
**
**  PURPOSE:    Convert a telephone number by removing non digit information such
**              as separator like '+' or '-'.
**
**  PARAMETERS:
**              pcSrc - Source telephone number to be converted.
**              pcDst - Destination telephone number after conversion.
**              uDst  - The maximum size of the destination number.
**
**
**  RETURNS:    Nothing.  The destination phone number is updated.
**
**  NOTES:      None.
***********************************************************************************/
void utilTelRemNonDig( const BOS_UINT8 *pcSrc, BOS_UINT8 *pcDst, BOS_UINT32 uDst )
{
   BOS_UINT32 uLen = 0;
   BOS_UINT32 uIx = 0;

   if ( (pcSrc != NULL) && (pcDst != NULL) && (uDst > 0) )
   {
      while ( (uLen <= uDst) &&
              (uLen < strlen ( reinterpret_cast<const char *> ( pcSrc )) ) )
      {
         if ( ((static_cast<const char> ( pcSrc[uLen] ) >= '0') &&
               (static_cast<const char> ( pcSrc[uLen] ) <= '9')) ||
              ((static_cast<const char> ( pcSrc[uLen] ) >= 'A') &&
               (static_cast<const char> ( pcSrc[uLen] ) <= 'D')) )
         {
            pcDst[uIx] = pcSrc[uLen];
            ++uIx;
         }

         ++uLen;
      };
   }
}
