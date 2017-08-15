/****************************************************************************
*
* Copyright © 2006-2008 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
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

/****************************************************************************/
/**
*  @file    icedbg.h
*
*  @brief   ICE Debug/Testing module.
*
*/
/****************************************************************************/

#ifndef ICEDBG_H    /* support nested includes */
#define ICEDBG_H

/* ---- Include Files ---------------------------------------- */
/* Enable debug logs */
#include <bosTypes.h>

#define ICEDBG_LOG  1

/* ---- Constants and Types ---------------------------------- */
/* ICEDBG_LOCAL_RESPONSE is used for debugging the ICE module in
 * concert with the STUN module, where Response is generated locally in
 * the STUN layer. When this flag is set to 1, ICE uses a hardcoded
 * password, instead of a randomly generated one, for peer request
 * authentication.
 */
#define ICEDBG_LOCAL_RESPONSE    0
/* ICEDBG_CANNED_CREDENTIAL is used for fixing local and remote 
 * credential used in peer connectivity check instead of the randomly
 * generated values
 */
#define ICEDBG_CANNED_CREDENTIAL    0

#if ICEDBG_CANNED_CREDENTIAL
   #define ICEDBG_UFRAG    "garf"
   #if ICEDBG_LOCAL_RESPONSE
      #define ICEDBG_PWD   "p+a+s+s+w+o+r+d+"
   #else
      #define ICEDBG_PWD   "abcdefghijklmnopqrstuv"
   #endif
#endif

#if ICEDBG_LOG

#undef DLOG
#define DLOG( x ) if ( NULL != iceLogCB && gIcedbg_RuntimeDLog ) {(*iceLogCB) x;} else {printf x;}

#define UNKNOWN (-1)
#define STR( arg) { arg, #arg }

#define TMAP(x,y)   icedbgTxtMapById(x,y)

typedef struct ICETXTMAP 
{
   int id;
   char *txt;
} ICETXTMAP;

extern int gIcedbg_RuntimeDLog;

extern const ICETXTMAP icecndtLstStateMap[];
extern const ICETXTMAP icecndtTypeMap[];
extern const ICETXTMAP icecndtStateMap[];
extern const ICETXTMAP icecttyChkStateMap[];
extern const ICETXTMAP icecttyChkLstStateMap[];
extern const ICETXTMAP icecttyRoleMap[];
extern const ICETXTMAP iceagntStateMap[];
extern const ICETXTMAP iceagntEvtMap[];
extern const ICETXTMAP iceModeMap[];
extern const ICETXTMAP iceNomMap[];
extern const ICETXTMAP iceEvtMap[];
extern const ICETXTMAP iceTransportMap[];
extern const ICETXTMAP iceCompIdMap[];
extern const ICETXTMAP iceSdpAgentMap[];

extern char *icedbgTxtMapById( const ICETXTMAP *map, int id);

#else

#undef DLOG
#define DLOG( x )

#endif
#endif
