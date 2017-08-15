/**********************************************************************************
** Copyright (c) 2011-2012 Broadcom
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
**      This file contains the Call Manager mapping functionality.
**
***********************************************************************************/

#ifndef __CM_MAP__H__INCLUDED__
#define __CM_MAP__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>
#include <cmFsm.h>

/* ---- Constants and Types ----------------------------------------------------- */

typedef struct
{
   BOS_UINT32 uId;
   BOS_UINT32 uMap;

} CMMAPID;

typedef struct
{
   BOS_UINT32 uId;
   BOS_UINT8  *pMap;

} CMMAPSTR;

typedef struct
{
   CMFSMEVT  eEvt;
   BOS_UINT8 uId;

} CMMAPEVT;

/* ---- Public variables -------------------------------------------------------- */

extern CMMAPID cmMapCctkEvt2CmEvt[];
extern CMMAPID cmMapNetEvt2CmEvt[];
extern CMMAPID cmMapPhysEvt2CmEvt[];
extern CMMAPID cmMapTone2Clid[];
extern CMMAPID cmMapToneRing2CallWait[];
extern CMMAPID cmMapCfgCodec2CctkCodec[];
#if CMGRCFG_VIDEO_MEDIA
extern CMMAPID cmMapCfgH264Lvl2CctkH264Lvl[];
extern CMMAPID cmMapCfgH264PktMode2CctkH264PktMode[];
extern CMMAPID cmMapCfgH264Prof2CctkH264Prof[];
#endif /* CMGRCFG_VIDEO_MEDIA */

extern CMMAPSTR cmMapCmState[];
extern CMMAPSTR cmMapCmEvent[];
extern CMMAPSTR cmMapDmAction[];
extern CMMAPSTR cmMapCmTone2Name[];

extern CMMAPEVT cmMapCmEvt2DigCol[];
extern CMMAPEVT cmMapEptSig2DigCol[];

extern CMMAPSTR cmCwToneMap[];

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmMapGetId
**
** PURPOSE:    Maps an identified resource to another one in an ID based map.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT32 cmMapGetId ( CMMAPID *pMap, BOS_UINT32 uId );


/***********************************************************************************
** FUNCTION:   cmMapGetStr
**
** PURPOSE:    Maps an identified resource to its human readable expression.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, NULL pointer if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT8 *cmMapGetStr ( CMMAPSTR *pMap, BOS_UINT32 uId );


/***********************************************************************************
** FUNCTION:   cmMapGetEvt
**
** PURPOSE:    Maps an identified event to its human usable character value.
**
** PARAMETERS: pMap - the map to lookup.
**             eEvt - the identified event to lookup.
**
** RETURNS:    The mapped identifier if valid, '?' is no valid mapped event is
**             found.
***********************************************************************************/
BOS_UINT8 cmMapGetEvt ( CMMAPEVT *pMap, CMFSMEVT eEvt );


/***********************************************************************************
** FUNCTION:   cmMapGetIdFromStr
**
** PURPOSE:    Maps a human readable resource to its internal machine equivalent.
**
** PARAMETERS: pMap - the map to lookup.
**             puStr - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if the
**             mapping can not be obtained.
***********************************************************************************/
BOS_UINT32 cmMapGetIdFromStr ( CMMAPSTR *pMap, BOS_UINT8 *puStr );


/***********************************************************************************
** FUNCTION:   cmMapGetEvtFromChar
**
** PURPOSE:    Maps a character to its event value.
**
** PARAMETERS: pMap  - the map to lookup.
**             uChar - the character to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if no valid
**             mapped event is found.
***********************************************************************************/
BOS_UINT32 cmMapGetEvtFromChar ( CMMAPEVT *pMap, BOS_UINT8 uChar );

#if defined(__cplusplus)
}
#endif

#endif /* __CM_MAP__H__INCLUDED__ */

