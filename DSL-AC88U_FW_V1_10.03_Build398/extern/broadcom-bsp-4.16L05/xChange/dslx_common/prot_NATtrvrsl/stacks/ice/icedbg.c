/****************************************************************************
*
* Copyright © 2006-2008 Broadcom Corporation
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
****************************************************************************
*
*    Description:
*
*      ICE Debug/Testing module.
*      
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include "icedbg.h"
#include "iceagnt.h"
#include "icecndt.h"
#include "icectty.h"
/* ---- Public Variables ------------------------------------- */
#if ICEDBG_LOG

const ICETXTMAP icecndtLstStateMap[] =
{
   STR( CNDTLST_OFFLINE ),
   STR( CNDTLST_PENDING ),
   STR( CNDTLST_GATHERING ),
   STR( CNDTLST_TIMEOUT ),
   STR( CNDTLST_COMPLETE ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP icecndtTypeMap[] =
{
   STR( ICECNDT_HOST ),
   STR( ICECNDT_SRVRFLX ),
   STR( ICECNDT_RELAYED ),
   STR( ICECNDT_PEERRFLX ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP icecndtStateMap[] =
{
   STR( ICECNDT_INUSE ),
   STR( ICECNDT_FROZEN ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP icecttyChkStateMap[] =
{
   STR( ICECTTY_WAITING ),
   STR( ICECTTY_IN_PROGRESS ),
   STR( ICECTTY_SUCCEEDED ),
   STR( ICECTTY_FAILED ),
   STR( ICECTTY_FROZEN ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP icecttyChkLstStateMap[] =
{
   STR( ICECTTY_CL_INACTIVE ),
   STR( ICECTTY_CL_RUNNING ),
   STR( ICECTTY_CL_COMPLETED ),
   STR( ICECTTY_CL_FAILED ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP icecttyRoleMap[] =
{
   STR( ICECTTY_CONTROLLING ),
   STR( ICECTTY_CONTROLLED ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP iceagntStateMap[] =
{
   STR( ICEAGNT_OFFLINE ),
   STR( ICEAGNT_CNDT_GATHERING ),
   STR( ICEAGNT_CTTY_CHECK_READY ),
   STR( ICEAGNT_CTTY_CHECK_IN_PROGRESS ),
   STR( ICEAGNT_CONCLUDED ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP iceagntEvtMap[] =
{
   STR( ICEAGNT_CTTYCHECK_STATUS ),
   STR( ICEAGNT_CTTYCHECK_REQUEST ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP iceModeMap[] =
{
   STR( ICE_FULL ),
   STR( ICE_LITE ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP iceNomMap[] =
{
   STR( ICE_REGULAR ),
   STR( ICE_AGGRESSIVE ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP iceEvtMap[] =
{
   STR( ICE_EVT_CANDIDATES_GATHER_COMPLETE ),
   STR( ICE_EVT_CANDIDATES_GATHER_TIMEOUT ),
   STR( ICE_EVT_CONCLUDED_SUCCESS ),
   STR( ICE_EVT_CONCLUDED_FAILURE ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP iceTransportMap[] =
{
   STR( ICE_TRANS_UDP ),
   STR( ICE_TRANS_TCP ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP iceCompIdMap[] =
{
   STR( ICE_COMP_RTP ),
   STR( ICE_COMP_RTCP ),
   { UNKNOWN, "**Unknown**" }
};

const ICETXTMAP iceSdpAgentMap[] =
{
   STR( ICE_SDP_OFFERER ),
   STR( ICE_SDP_ANSWERER ),
   { UNKNOWN, "**Unknown**" }
};
#endif 
/* ---- Private Constants and Types -------------------------- */

/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */
#if ICEDBG_LOG
/*****************************************************************************
* FUNCTION:   icedbgTxtMapById
*
* PURPOSE:    Translate id to application text string
*
* PARAMETERS: 
*            
* RETURNS:
*
* NOTE:       
*****************************************************************************/
char *icedbgTxtMapById( const ICETXTMAP *map, int id)
{
   while ( map->id != UNKNOWN && map->id != id)
   {
      map++;
   }
   return map->txt;
}
#endif
