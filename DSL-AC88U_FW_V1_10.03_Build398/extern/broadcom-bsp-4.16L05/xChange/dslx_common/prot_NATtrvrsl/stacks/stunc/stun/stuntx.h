/****************************************************************************
*
* Copyright (c) 2006-2011 Broadcom
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
*  @file    stuntx.h
*
*  @brief   STUN Transaction Module Interface.
*
*/
/****************************************************************************/
#ifndef STUNTX_H    /* support nested includes */
#define STUNTX_H

/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <bosSem.h>
#include <stun.h>
#include "stunmsg.h"

/* ---- Constants and Types ---------------------------------- */
extern BOS_BOOL stuntx_Init( void );

extern BOS_BOOL stuntx_Deinit( void );

extern void stuntx_GenerateId( STUN_TXID *pTxid );

extern BOS_BOOL stuntx_Send( const BOS_UINT32             sid, 
                             const STUN_TRANSPORT         *pDest, 
                             const BOS_UINT8              *pBuf,
                             const BOS_UINT16             len,
                             const STUN_TXID              *pTxid,
                             const BOS_UINT32             reqHdl,
                             const STUN_RESCB             resCB,
                             const STUN_RETRANS           *pRetxData,
                             const STUN_HMAC_PWD          *pPassword,
                             const STUN_OPTS              options,
                             const void                   *pOpaq );

extern BOS_BOOL stuntx_Authenticate( const void        *pData, 
                                     const BOS_UINT8   *pHash,
                                     const BOS_UINT16  hmLen, 
                                     BOS_UINT32        *pReqHdl,
                                     BOS_UINT8         *pResCnt, 
                                     BOS_UINT32        *pReqSid,
                                     STUN_TRANSPORT    **ppReqDest,
                                     void              **ppOpaq,
                                     STUN_RESCB        *pResCB,
                                     STUN_OPTS         *pOptions
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                                     ,
                                     BOS_SEM           **ppSyncdResSem
#endif
                              );

extern char *stuntx_IdStr( STUN_TXID *pTxid );

extern BOS_BOOL stuntx_CancelRequest( const BOS_UINT32 uReqHdl ); 

#endif
