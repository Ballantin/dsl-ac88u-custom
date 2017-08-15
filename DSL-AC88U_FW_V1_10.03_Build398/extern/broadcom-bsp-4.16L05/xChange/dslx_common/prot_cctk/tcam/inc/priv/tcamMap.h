/**********************************************************************************
** Copyright © 2009 - 2010 Broadcom
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
**      This file contains the TCAM mapping functionality.
**
***********************************************************************************/

#ifndef __TCAM_MAP__H__INCLUDED__
#define __TCAM_MAP__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>

/* ---- Constants and Types ----------------------------------------------------- */

typedef struct
{
   BOS_UINT32 uId;
   BOS_UINT32 uMap;

} TCAMMAPID;

typedef struct
{
   BOS_UINT32 uId;
   BOS_UINT8  *pMap;

} TCAMMAPSTR;


/* ---- Public variables -------------------------------------------------------- */

extern TCAMMAPID tcamMapCmgrEptSig2OutEvt[];
extern TCAMMAPID tcamMapCmgrEvt2OutEvt[];
extern TCAMMAPID tcamMapCmgrSvc2OutEvt[];

extern TCAMMAPSTR tcamMapOutEvtStr[];


/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   tcamMapGetId
**
** PURPOSE:    Maps an identified resource to another one in an ID based map.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT32 tcamMapGetId ( TCAMMAPID *pMap, BOS_UINT32 uId );


/***********************************************************************************
** FUNCTION:   tcamMapGetStr
**
** PURPOSE:    Maps an identified resource to its human readable expression.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, NULL pointer if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT8 *tcamMapGetStr ( TCAMMAPSTR *pMap, BOS_UINT32 uId );


/***********************************************************************************
** FUNCTION:   tcamMapGetIdFromStr
**
** PURPOSE:    Maps a human readable resource to its internal machine equivalent.
**
** PARAMETERS: pMap - the map to lookup.
**             puStr - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if the
**             mapping can not be obtained.
***********************************************************************************/
BOS_UINT32 tcamMapGetIdFromStr ( TCAMMAPSTR *pMap, BOS_UINT8 *puStr );

#if defined(__cplusplus)
}
#endif

#endif /* __TCAM_MAP__H__INCLUDED__ */

