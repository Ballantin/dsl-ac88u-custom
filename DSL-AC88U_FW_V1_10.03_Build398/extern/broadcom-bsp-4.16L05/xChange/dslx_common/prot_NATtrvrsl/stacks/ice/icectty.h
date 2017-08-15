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
*  @file    icectty.h
*
*  @brief   ICE Connectivity Check module.
*
*  This file contains software interface information for the ICE 
*  Connectivity check Module.
*/
/****************************************************************************/

#ifndef ICECTTY_H    /* support nested includes */
#define ICECTTY_H

/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <stun.h>
#include "icecndt.h"

/* ---- Constants and Types ---------------------------------- */

/* Maximum number of pairs 
 * (number of possible combinations of 3 candidate types and 2 components) 
 */
#define ICECTTY_CNDT_PAIR_MAX    (4*4*2)

/* 
 * Check States: (draft-ietf-mmusic-ice-13.txt)
 *
 * Waiting: This check has not been performed, and can be performed as
 *    soon as it is the highest priority Waiting check on the check
 *    list.
 *
 * In-Progress: A request has been sent for this check, but the
 *    transaction is in progress.
 *
 * Succeeded: This check was already done and produced a successful
 *    result.
 *
 * Failed: This check was already done and failed, either never
 *    producing any response or producing an unrecoverable failure
 *    response.
 *
 * Frozen: This check hasn't been performed, and it can't yet be
 *    performed until some other check succeeds, allowing it to move
 *    into the Waiting state.
 */
typedef enum
{
   ICECTTY_FROZEN,
   ICECTTY_WAITING,
   ICECTTY_IN_PROGRESS,
   ICECTTY_SUCCEEDED,
   ICECTTY_FAILED
} ICECTTY_CHECK_STATE;

/* 
 *  Running:  In this state, ICE checks are still in progress for this
 *    media stream.
 *
 * Completed:  In this state, ICE checks have completed successfully for
 *    this media stream.
 *
 * Failed:  In this state, the ICE checks have not completed
 *    successfully for this media stream.
 */
typedef enum
{
   ICECTTY_CL_INACTIVE,    /* No checks to be sent */
   ICECTTY_CL_RUNNING,
   ICECTTY_CL_COMPLETED,
   ICECTTY_CL_FAILED

} ICECTTY_CHECKLIST_STATE;

typedef struct
{
   BOS_UINT32  high;
   BOS_UINT32  low;
} ICECTTY_UINT64;

/* Connectivity check candidate pair */
typedef struct ICECTTY_CHECK
{
   ICECNDT_TYPE         lclCndtType;      /* Local candidate type */
   ICECNDT              *pLclCndt;        /* Local candidate but 
                                             converted to base */
   ICECNDT              *pRmtCndt;        /* Remote candidate */
   ICECTTY_CHECK_STATE  state;            /* Check state */
   ICECTTY_UINT64       pairPri;          /* Pair priority 64-bit value */
   BOS_UINT32           sid;              /* Socket id sending STUN req */
   BOS_UINT32           reqHdl;           /* STUN Request handle */

   ICE_COMP_ID          compId;           /* Component ID */
   ICE_TRANS_TYPE       transType;        /* Port transport type */

   BOS_BOOL             bRedundant;       /* If pair is redundant */

   /* Valid pair */
   BOS_BOOL             bValidated;       /* Pair has been validated */
   BOS_BOOL             bNominated;       /* The nominated Pair */
   BOS_BOOL             bUseCndtChk;      /* Use-Candidate Check */
   BOS_BOOL             bSelected;        /* Selected pair */

   struct ICECTTY_CHECK *pPrev;
   struct ICECTTY_CHECK *pNext;

} ICECTTY_CHECK;

/* Connectivity Check list */
typedef struct
{
   BOS_UINT8                  num;              /* Number of check pairs in 
                                                   the check list */
   ICECTTY_CHECK              *pHead;           /* Check pair list head */
   ICECTTY_CHECKLIST_STATE    state;            /* Check list state */
   BOS_BOOL                   bNewSelected;     /* New check pair has been 
                                                   selected */
   BOS_BOOL                   bAllChecksSent;   /* All checks have been 
                                                   sent */
} ICECTTY_CHKLST;

/* ICE Agent role of operation */
typedef enum
{
   ICECTTY_CONTROLLING,
   ICECTTY_CONTROLLED
} ICECTTY_ROLE;

extern BOS_BOOL icectty_FormCheckList( ICECNDT_LST    *pLclCndtLst[],
                                       ICECNDT_LST    *pRmtCndtLst[],
                                       ICECTTY_CHKLST       *pChkLst,
                                       const ICECTTY_ROLE   role );

extern BOS_BOOL icectty_PeriodicCheck( ICECTTY_CHKLST        *pChkLst, 
                                       const ICE_CREDENTIAL *pLclCredential,
                                       const ICE_CREDENTIAL *pRmtCredential,
                                       const ICE_TYPEPREF   *pPriTypePref,
                                       const BOS_UINT16     *pPriLocalPref,
                                       const ICE_NOM        nom,
                                       const ICECTTY_ROLE   role );

extern BOS_BOOL icectty_TriggeredCheck( const STUN_TRANSPORT  *pLclTransport,
                                        const STUN_TRANSPORT  *pRmtTransport,
                                        const BOS_UINT32      sid,
                                        const BOS_UINT32    rmtPriority,
                                        ICECTTY_CHKLST      *pChkLst, 
                                        const ICE_CREDENTIAL *pLclCredential,
                                        const ICE_CREDENTIAL *pRmtCredential,
                                        const ICE_TYPEPREF  *pPriTypePref,
                                        const BOS_UINT16    *pPriLocalPref,
                                        const ICE_NOM       nom,
                                        const ICECTTY_ROLE  role,
                                        const BOS_BOOL      bUseCndt,
                                        ICECNDT_LST         *pLclCndtLst[],
                                        ICECNDT_LST         *pRmtCndtLst[]);
extern BOS_BOOL icectty_LiteUpdateCheckList( 
                              const STUN_TRANSPORT     *pLclTransport,
                              const STUN_TRANSPORT     *pRmtTransport, 
                              const BOS_UINT32         sid,
                              ICECNDT_LST              *pLclCndtLst[],
                              ICECNDT_LST              *pRmtCndtLst[],
                              ICECTTY_CHKLST           *pChkLst );

extern void icectty_UnfreezeCndtPairs( ICECTTY_CHKLST *pChkLst, 
                                       const char *pLclFoundation,
                                       const char *pRmtFoundation );

extern BOS_BOOL icectty_Process( const BOS_UINT32         reqHdl,
                                 const STUN_RES_DATA      *pResData );
extern BOS_BOOL icectty_SendConnectivityCheck( 
                                    const ICE_CREDENTIAL    *pLclCredential,
                                    const ICE_CREDENTIAL    *pRmtCredential,
                                    const ICE_TYPEPREF      *pPriTypePref,
                                    const BOS_UINT16        *pLocalPref,
                                    const ICECTTY_CHECK     *pChk,
                                    const BOS_BOOL          bUseCndt,
                                    const ICECTTY_ROLE      role,
                                    BOS_UINT32              *pReqHdl );

extern void icectty_DeleteChkLst( ICECTTY_CHKLST *pChkLst );

#endif
