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
*  @file    icecndt.h
*
*  @brief   ICE Candidate gathering module.
*
*  This file contains software interface information for the ICE 
*  Candidate gathering Module.
*/
/****************************************************************************/

#ifndef ICECNDT_H    /* support nested includes */
#define ICECNDT_H

/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <ice.h>
#include <stun.h>
#include <stunc.h>

/* ---- Constants and Types ---------------------------------- */
/* Foundation Id assignment scheme (stored as string) */
#define ICECNDT_FOUNDATION_LEN_MAX  31
/*
 *     <reserved>
 *       ||
 *       VV
 *       XXXXXXXX
 *         ||||||
 *         vv||||
 *     <ifid>||||
 *           vv||
 *      <srvid>||
 *             vv
 *           <type>
 */
#define ICECNDT_FOUNDATION_HOST     (BOS_UINT8)1
#define ICECNDT_FOUNDATION_SRFLX    (BOS_UINT8)2 
#define ICECNDT_FOUNDATION_PRFLX    (BOS_UINT8)3 
#define ICECNDT_FOUNDATION_RELAYED  (BOS_UINT8)4 

/* Candidate list state */
typedef enum
{
   CNDTLST_OFFLINE = 0,    /* List not needed for candidate gathering */
   CNDTLST_PENDING,        /* Pending for candidate gathering */
   CNDTLST_GATHERING,      /* Gathering candidates */
   CNDTLST_TIMEOUT,        /* Timeout */
   CNDTLST_COMPLETE        /* Finished gathering candidates */
} CNDTLST_STATE;

/* Candidate types */
typedef enum
{
   ICECNDT_HOST,     /* Host candidate */
   ICECNDT_SRVRFLX,  /* Server reflexive candidate */
   ICECNDT_RELAYED,  /* Server relayed candidate */
   ICECNDT_PEERRFLX  /* Peer reflexive candidate */
} ICECNDT_TYPE;

/* Candidate states */
typedef enum
{
   ICECNDT_INUSE,
   ICECNDT_FROZEN
} ICECNDT_STATE;

/* Candidate Transport address */
typedef struct
{
   BOS_BOOL          bValid;     /* SocAddr is set and valid */
   STUN_TRANSPORT    socAddr;    /* Socket Address */
} ICECNDT_ADDR;

/* Candidate Data Block */
typedef struct ICECNDT
{
   BOS_UINT16           cndtId;              /* Candidate ID */
   ICECNDT_STATE        state;               /* Candidate state */
   ICECNDT_TYPE         type;                /* Candidate type (host, 
                                              * srvrflx or relayed ) 
                                              */
   struct ICECNDT       *pBaseCndt;          /* Base candidate */
   BOS_UINT32           priority;            /* Priority */
   BOS_BOOL             bInUse;              /* Whether a candidate is in 
                                              * m/c line 
                                              */
   ICECNDT_ADDR         transport;           /* Transport address */
   ICECNDT_ADDR         *pRelated;           /* Related transport address */

   char                 foundation[ICECNDT_FOUNDATION_LEN_MAX+1];          
                                             /* Foundation string */

   struct ICECNDT       *pPrev;              /* Previous candidate */
   struct ICECNDT       *pNext;              /* Next candidate */
} ICECNDT;

/* Candidate List Data Block */
typedef struct ICECNDT_LST
{
   CNDTLST_STATE        state;               /* Candidate list state */
   ICE_TYPEPREF         priTypePref;         /* Priority - Type preference 
                                              * settings 
                                              */
   BOS_UINT16           priLocalPref;        /* Priority - Local preference 
                                              */
   BOS_TIME_MS          maxWait;             /* Candidate gathering time 
                                              * budget 
                                              */
   ICECNDT              *pCndtHead;          /* Linked list of candidates */
   BOS_UINT16           cndtCount;           /* Candidate count */
   BOS_UINT32           reqHdl;              /* STUN Request handle */

   ICE_COMP_ID          compId;              /* Component ID */
   const ICE_LOCAL_IF   *pLocalIf;           /* Local interface */
   ICE_STUN_SERVER      *pStunServer;        /* STUN Server */
   ICE_TRANS_TYPE       transType;           /* Port transport type UDP or 
                                              * TCP 
                                              */
   BOS_UINT32           sid;                 /* Socket id */
   BOS_SOCKET_IP_PORT   port;                /* Local Port number */
   STUNC_SHARED_SECRET  secret;              /* STUN Shared secret */
} ICECNDT_LST;

extern ICECNDT_LST *icecndt_NewList( void );
extern void icecndt_DeleteList( ICECNDT_LST *pCndtLst );
extern ICECNDT *icecndt_NewCndt( void );
extern void icecndt_DeleteCndt( ICECNDT *pCndt );
extern BOS_BOOL icecndt_AddCndtToList( ICECNDT *pCndt, 
                                       ICECNDT_LST *pCndtLst );
extern BOS_BOOL icecndt_GatherHostCndt( ICECNDT_LST           *pCndtLst,
                                        const ICE_LOCAL_IF    *pLocalIf, 
                                        const BOS_UINT16      port, 
                                        const ICE_COMP_ID     compId );
extern BOS_BOOL icecndt_GatherStunCndt( ICECNDT_LST            *pCndtLst,
                                        const STUNC_EVT_DATA   *pEvtData );
#if NTCFG_TURN_SUPPORT
extern BOS_BOOL icecndt_GatherTurnCndts( ICECNDT_LST           *pCndtLst,
                                         const TURNC_EVT_DATA  *pEvtData );
#endif
extern void icecndt_FinalizeCndtLst( ICECNDT_LST  *pCndtLst );
extern void icecndt_SendRequest( ICECNDT_LST *pCndtLst );
extern BOS_UINT32 icecndt_ComputePriority( const BOS_UINT8 typePref, const BOS_UINT16 localPref, const BOS_UINT8 compId);

#endif
