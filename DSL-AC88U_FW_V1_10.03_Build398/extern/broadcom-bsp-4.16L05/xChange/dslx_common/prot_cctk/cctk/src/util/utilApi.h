/**********************************************************************************
** Copyright © 2007-2009 Broadcom
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
**      This file contains the Utility API.
**
***********************************************************************************/

#ifndef __UTIL_API__H__INCLUDED__
#define __UTIL_API__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Constants and Types ----------------------------------------------------- */
/* Bitmap control macros */
#define UTIL_BMP_IS_ENABLED_ALL( sBmp, sMask ) ((( sBmp ) & ( sMask )) == ( sMask ))
#define UTIL_BMP_IS_ENABLED( sBmp, sMask )     ((( sBmp ) & ( sMask )) != 0 )

/* Return status */
typedef enum UTILSTATUS
{
   eUTILSTATUS_SUCCESS,                /* Success */
   eUTILSTATUS_BADSTATE,               /* Bad state */
   eUTILSTATUS_BADPARM,                /* Bad parameter(s) */
   eUTILSTATUS_NORSC,                  /* Not enough resource */
   eUTILSTATUS_INTERR,                 /* Internal error */
   eUTILSTATUS_MAX                     /* Status count (not used) */
} UTILSTATUS;

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

/* Utility module (de)initialization routines */
UTILSTATUS  utilInit( void );
UTILSTATUS  utilDeInit( void );

/* String manipulation functions */
BOS_UINT8   *utilStrDup( const BOS_UINT8 *pcSrc );
BOS_UINT8   *utilStrDupSize( const BOS_UINT8 *pcSrc, const BOS_UINT32 uSize );
BOS_UINT8   **utilStrArrayDup( const BOS_UINT8 **ppcSrc );
void        utilStrArrayDel( BOS_UINT8 **&rppcStr );

/* Data manipulation functions */
BOS_UINT8   *utilDataDup( const BOS_UINT8 *puSrc, BOS_UINT32 uSize );

/* Telephone number manipulation functions */
void        utilTelRemNonDig( const BOS_UINT8 *pcSrc,
                              BOS_UINT8 *pcDst,
                              BOS_UINT32 uDst );

#if CCTKCFG_MEMORY_MONITORING
/* Memory management functions */
void        *utilMemEvtObsGet( void );
#if CCTKCFG_DEBUG
BOS_UINT32  utilMemAllocGet( BOS_BOOL bCctkOnly, BOS_BOOL bTraceTable );
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* __UTIL_API__H__INCLUDED__ */
