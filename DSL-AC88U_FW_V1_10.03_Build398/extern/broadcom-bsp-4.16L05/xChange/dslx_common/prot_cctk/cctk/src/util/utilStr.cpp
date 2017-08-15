/**********************************************************************************
** Copyright © 2007-2009 Broadcom Corporation
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
**      This file implements the String utility functions.
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
**  FUNCTION:   utilStrDup
**
**  PURPOSE:    Duplicate (and dynamically allocate) a character string.
**
**  PARAMETERS:
**              pcSrc   - Character string to be duplicated.
**
**
**  RETURNS:    Pointer of the duplicated character string. NULL if duplication
**              fails. Memory ownership is given to caller of the function.
**
**  NOTES:      None.
***********************************************************************************/
BOS_UINT8 *utilStrDup( const BOS_UINT8 *pcSrc )
{
   BOS_UINT8 *pcRes = NULL;

   if ( pcSrc != NULL )
   {
      int sLen = strlen( reinterpret_cast< const char * >( pcSrc ) );

      pcRes = BRCM_NEW_ARRAY( BOS_UINT8, sLen + 1 );
      CCTK_ASSERT( pcRes != NULL );
      if ( pcRes != NULL )
      {
         strncpy( reinterpret_cast< char * >( pcRes ),
                  reinterpret_cast< const char * >( pcSrc ), 
                  sLen + 1 );
      }
   }

   return pcRes;
}


/***********************************************************************************
**  FUNCTION:   utilStrDupSize
**
**  PURPOSE:    Duplicate (and dynamically allocate) a character string.
**
**  PARAMETERS:
**              pcSrc   - Character string to be duplicated.
**              uSize   - Size of the character string to be duplicated.
**
**
**  RETURNS:    Pointer of the duplicated character string. NULL if duplication
**              fails. Memory ownership is given to caller of the function.
**
**  NOTES:      None.
***********************************************************************************/
BOS_UINT8 *utilStrDupSize( const BOS_UINT8 *pcSrc, const BOS_UINT32 uSize )
{
   BOS_UINT8 *pcRes = NULL;

   if ( pcSrc != NULL )
   {
      pcRes = BRCM_NEW_ARRAY( BOS_UINT8, uSize + 1 );
      CCTK_ASSERT( pcRes != NULL );
      if ( pcRes != NULL )
      {
         memset ( static_cast< void * >( pcRes ),
                  0,
                  uSize + 1 );
         strncpy( reinterpret_cast< char * >( pcRes ),
                  reinterpret_cast< const char * >( pcSrc ), 
                  uSize );
      }
   }

   return pcRes;
}


/***********************************************************************************
**  FUNCTION:   utilStrArrayDup
**
**  PURPOSE:    Duplicate (and dynamically allocate) a NULL-terminated string array.
**
**  PARAMETERS:
**              ppcSrc - Character string array to be duplicated (NULL-terminated).
**
**
**  RETURNS:    Pointer of the duplicated character string array (NULL-terminated).
**              NULL if duplication fails. Memory ownership is given to caller.
**
**  NOTES:      None.
***********************************************************************************/
BOS_UINT8 **utilStrArrayDup( const BOS_UINT8 **ppcSrc )
{
   BOS_UINT8 **ppcRes = NULL;

   if ( ppcSrc != NULL )
   {
      BOS_UINT32 uTemp = 1;
      const BOS_UINT8 **ppcTemp = ppcSrc;
      while ( *ppcTemp != NULL )
      {
         uTemp++;
         ppcTemp++;
      }

      ppcRes = BRCM_NEW_ARRAY( BOS_UINT8 *, uTemp );

      uTemp = 0;
      ppcTemp = ppcSrc;
      while ( *ppcTemp != NULL )
      {
         ppcRes[uTemp] = utilStrDup( *ppcTemp );
         uTemp++;
         ppcTemp++;
      }
      ppcRes[uTemp] = NULL;
   }

   return ppcRes;
}

/***********************************************************************************
**  FUNCTION:   utilStrArrayDel
**
**  PURPOSE:    Delete (and de-allocate) a NULL-terminated string array.
**
**  PARAMETERS:
**              rppcStr - Character string array to be deleted.
**                        Ownership is given to the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void utilStrArrayDel( BOS_UINT8 **&rppcStr )
{
   if ( rppcStr != NULL )
   {
      BOS_UINT8 **ppcTemp = rppcStr;
      while ( *ppcTemp != NULL )
      {
         BRCM_DELETE( *ppcTemp );
         ppcTemp++;
      }

      BRCM_DELETE_ARRAY( rppcStr );
      rppcStr = NULL;
   }
}
