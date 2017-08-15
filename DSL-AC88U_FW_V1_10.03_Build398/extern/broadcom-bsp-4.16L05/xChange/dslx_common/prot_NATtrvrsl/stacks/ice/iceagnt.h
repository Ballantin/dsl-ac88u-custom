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
*  @file    iceagnt.h
*
*  @brief   ICE Agent module.
*
*  This file contains software interface information for the ICE 
*  Candidate gathering Module.
*/
/****************************************************************************/

#ifndef ICEAGNT_H    /* support nested includes */
#define ICEAGNT_H

/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <ice.h>
#include "icecndt.h"
#include "icectty.h"

/* ---- Constants and Types ---------------------------------- */

/* ICE Agent states */
typedef enum
{
   ICEAGNT_OFFLINE = 0,             /* Offline
                                     *    - Not doing anything 
                                     */

   ICEAGNT_CNDT_GATHERING,          /* Gathering candidates 
                                     *    - Sends STUN requests to servers
                                     *    - Awaits for responses from servers
                                     */

   ICEAGNT_CTTY_CHECK_READY,        /* Base on SDP offer/answer info
                                     *    - Determines ICE Agent role
                                     *    - Pairs up offer/answer candidates
                                     *    - Computes candidate pair priority
                                     *    - responds to peer connectivity 
                                     *      checks
                                     */

   ICEAGNT_CTTY_CHECK_IN_PROGRESS,  /* Performing connectivity checks 
                                     *    - For ice-lite agent, it responds 
                                     *       to connectivity checks
                                     *    - For full mode agent, it also
                                     *       initiates connectivity checks
                                     */

   ICEAGNT_CONCLUDED                /* ICE concluded
                                     *    - All connectivity checks completed
                                     *    - Agent now has an ordered set of
                                     *      src/dst ip/port for exchanging 
                                     *      media.
                                     *    - It still responds to trigger 
                                     *       checks
                                     *    - It sends "keepalives" to keep NAT
                                     *      binding active.
                                     */
} ICEAGNT_STATE;

/* ICE Agent Data Block */
typedef struct ICEAGNT
{
   BOS_UINT32        agentId;          /* Agent Identifier */
   ICE_STUN_SERVER_LIST serverList;    /* STUN Server List */
   ICEAGNT_STATE     state;            /* Agent state */
   ICE_MODE          mode;             /* Agent mode */
   ICE_MODE          rmtMode;          /* Remote agent mode */
   ICECTTY_ROLE      role;             /* Agent role */
   ICE_NOM           nom;              /* Nomination type */
   ICE_TYPEPREF      priTypePref;      /* Type preference settings */
   BOS_UINT16        priLocalPref;     /* Local preference */
   ICE_CREDENTIAL    lclCredential;    /* Local short term credential */
   ICE_CREDENTIAL    rmtCredential;    /* Remote short term credential */
   BOS_BOOL          bLclSdpValid;     /* Local SDP is valid */
   BOS_BOOL          bRmtSdpValid;     /* Remote SDP is valid */
   ICECNDT_LST       *pLclCndtLst[ICE_COMP_NUM_MAX]; /* Local candidates list */     
   ICECNDT_LST       *pRmtCndtLst[ICE_COMP_NUM_MAX]; /* Remote candidates list */
   BOS_TIME_MS       maxWait;          /* max wait time */
   BOS_TIME_MS       eTime;            /* State dependent elaspsed time */
   ICECTTY_CHKLST    chkLst;           /* Connectivity check list */
   struct ICEAGNT    *pPrev;           /* Pointer to previous agent */
   struct ICEAGNT    *pNext;           /* Pointer to next agent */

} ICEAGNT;

/* Connectivity Check status codes */
#define ICEAGNT_CHECK_SUCCESS          0
#define ICEAGNT_CHECK_FAILURE          1

/* ICE agent events */
typedef enum
{
   ICEAGNT_CTTYCHECK_STATUS,           /* op1 = reqHdl 
                                        * op2 = check status 
                                        * op3 = the mapped address 
                                        *       STUN_TRANSPORT *, if op2 
                                        *       is ICEAGNT_CHECK_SUCCESS 
                                        */

   ICEAGNT_CTTYCHECK_REQUEST           /* op1 = STUN message type
                                        * op2 = not used
                                        * op3 = STUN_REQ_DATA*
                                        */

} ICEAGNT_EVT;


extern BOS_BOOL iceagnt_Init( void );
extern BOS_BOOL iceagnt_Deinit( void );
extern BOS_BOOL iceagnt_Create( const ICE_STUN_SERVER_LIST *pServerList,
                                const ICE_AGENT_OPTIONS   *pOptions,
                                BOS_UINT32                *pAgentId );
extern BOS_BOOL iceagnt_Delete( const BOS_UINT32 agentId );
extern BOS_BOOL iceagnt_SetPortParms( const BOS_UINT32       agentId,
                                      const ICE_PORT_PARMS   *pPortParms[] );
extern BOS_BOOL iceagnt_GatherCndts( const BOS_UINT32         agentId,
                                     const BOS_UINT16         maxWait );
extern ICEAGNT *iceagnt_GetAgentFromId( const BOS_UINT32 agentId );
extern BOS_BOOL iceagnt_Notify( const ICEAGNT_EVT evt, const BOS_UINT32 op1, const BOS_UINT32 op2, const void *pData );
extern void iceagnt_ProcessStuncRes( const BOS_UINT32 reqHdl, const STUNC_EVT_DATA *pEvtData );
extern BOS_BOOL iceagnt_EncodeSdp( const BOS_UINT32 agentId, const ICE_SDP_AGENT sdpAgent, ICE_SDP_PARMS *pSdpParms );
extern BOS_BOOL iceagnt_ParseSdp( const BOS_UINT32 agentId, const ICE_SDP_AGENT sdpAgent, const ICE_SDP_PARMS *pSdpParms );

#endif
