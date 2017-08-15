/**********************************************************************************
** Copyright (c) 2011-2013 Broadcom
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
**      This file defines the API (application programming interface) to the
**      Call Control Toolkit Call Engine module.
**
***********************************************************************************/
#ifndef __CE_API__H__INCLUDED__
#define __CE_API__H__INCLUDED__

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Include Files ----------------------------------------------------------- */

/* ---- Constants and Types ----------------------------------------------------- */

/*
** This defines the different events that can be managed by the Call Engine and
** its finite state machines (FSMs).
**
** Beyond the generic events, which are handled directly by the Call Engine
** framework, each FSM handles a specific set of events based on its usage.
**
** Such event set for a given FSM are enclosed within a XXX_SET_BEGIN and
** a XXX_SET_FINISH markers and any new event to be handled by the specific
** FSM should be placed within those markers or it will be ignored.
*/
enum CEEVT
{
   eCEEVT_INV = eCCTKEVT_MAX,  /* Invalid. */
   eCEEVT_DNP,                 /* Do-not-process.  Special, ignore event. */

   eCEEVT_USR_ADD,             /* Events used to control resource. */
   eCEEVT_USR_REM,
   eCEEVT_CALL_ADD,
   eCEEVT_CALL_REM,
   eCEEVT_SUB_ADD,
   eCEEVT_SUB_REM,
   eCEEVT_NOT_ADD,
   eCEEVT_NOT_REM,
   eCEEVT_STARTUP,
   eCEEVT_RESET,
   eCEEVT_SUSPEND_NEW,
   eCEEVT_RESUME_ALL,

   eCEEVT_REG_SET_BEGIN,       /* Event marker for REG FSM - start. */
   eCEEVT_REG_NET_UNREG,
   eCEEVT_REG_NET_TRYING,
   eCEEVT_REG_NET_OK,
   eCEEVT_REG_NET_REFRESH,
   eCEEVT_REG_NET_FAIL,
   eCEEVT_REG_NET_ENDED,
   eCEEVT_REG_NET_TIMEOUT,
   eCEEVT_REG_NET_CHALLENGE,
   eCEEVT_REG_NET_SRV_FAIL,
   eCEEVT_REG_NET_NEXT_NONCE,
   eCEEVT_REG_NET_OPTIONS,
   eCEEVT_REG_NET_REFER,
   eCEEVT_REG_NET_REFENDED,
   eCEEVT_REG_NET_PAU_ID,
   eCEEVT_REG_NET_USR_ADD,
   eCEEVT_REG_NET_USR_REMOVE,
   eCEEVT_REG_USR_REG,
   eCEEVT_REG_USR_UNREG,
   eCEEVT_REG_USR_ENDED,
   eCEEVT_REG_USR_KA_OK,
   eCEEVT_REG_USR_KA_FAIL,
   eCEEVT_REG_USR_KA_RECOVERY,
   eCEEVT_REG_USR_PUBLISH,
   eCEEVT_REG_USR_ADD_OK,
   eCEEVT_REG_USR_SVC_ENA,
   eCEEVT_REG_USR_SVC_DIS,
   eCEEVT_REG_USR_TARGET_FAIL,
   eCEEVT_REG_USR_MSG,
   eCEEVT_REG_USR_INBOUND_TARGET,
   eCEEVT_REG_SET_FINISH,      /* Event marker for REG FSM - end. */

   eCEEVT_CALL_SET_BEGIN,      /* Event marker for CALL FSM - start. */
   eCEEVT_CALL_NET_CALLED,     /* INVITE received */
   eCEEVT_CALL_NET_TERMUSR,
   eCEEVT_CALL_NET_TERMNET,
   eCEEVT_CALL_NET_FAILED,
   eCEEVT_CALL_NET_ANSWER,
   eCEEVT_CALL_NET_RING,       /* 180 Ringing received / sent */
   eCEEVT_CALL_NET_EARLYMEDIA,
   eCEEVT_CALL_NET_REPLACE,
   eCEEVT_CALL_NET_MEDIA,
   eCEEVT_CALL_NET_MEDIA_FAIL,
   eCEEVT_CALL_NET_HOLD,
   eCEEVT_CALL_NET_XFER,
   eCEEVT_CALL_NET_PEND_OFFER_SDP,
   eCEEVT_CALL_NET_PEND_ANSWER_SDP,
   eCEEVT_CALL_NET_ANSWER_SDP,
   eCEEVT_CALL_NET_INFO,
   eCEEVT_CALL_USR_CREATE,
   eCEEVT_CALL_USR_MAKE,
   eCEEVT_CALL_USR_MEDIA,
   eCEEVT_CALL_USR_ANSWER,
   eCEEVT_CALL_USR_HOLD,
   eCEEVT_CALL_USR_UNHOLD,
   eCEEVT_CALL_USR_FORWARD,
   eCEEVT_CALL_USR_XFER,
   eCEEVT_CALL_USR_BXFER,
   eCEEVT_CALL_USR_XFER_NOTERM,  /* Call transfer without call termination */
   eCEEVT_CALL_USR_ENDED,
   eCEEVT_CALL_USR_PROXY,
   eCEEVT_CALL_USR_DTMF,
   eCEEVT_CALL_USR_FLASH,
   eCEEVT_CALL_SET_FINISH,     /* Event marker for CALL FSM - end. */

   eCEEVT_XFER_SET_BEGIN,      /* Event marker for XFER FSM - start. */
   eCEEVT_XFER_NET_XFER,
   eCEEVT_XFER_NET_OKAY,
   eCEEVT_XFER_NET_FAIL,
   eCEEVT_XFER_NET_PROC,
   eCEEVT_XFER_NET_ONGOING,
   eCEEVT_XFER_NET_TERMNET,
   eCEEVT_XFER_NET_REFENDED,
   eCEEVT_XFER_NET_ANSWER,
   eCEEVT_XFER_NET_RING,
   eCEEVT_XFER_NET_EARLYMEDIA,
   eCEEVT_XFER_NET_ACCEPT,
   eCEEVT_XFER_USR_XFER,
   eCEEVT_XFER_USR_BXFER,
   eCEEVT_XFER_USR_XFER_NOTERM,
   eCEEVT_XFER_USR_NTTO,
   eCEEVT_XFER_USR_ENDED,
   eCEEVT_XFER_SET_FINISH,     /* Event marker for XFER FSM - end. */

   eCEEVT_HOLD_SET_BEGIN,      /* Event marker for HOLD FSM - start. */
   eCEEVT_HOLD_NET_MEDIA,
   eCEEVT_HOLD_NET_HOLD,
   eCEEVT_HOLD_NET_UNHOLD,
   eCEEVT_HOLD_NET_RING,
   eCEEVT_HOLD_NET_TERMNET,
   eCEEVT_HOLD_NET_TERMUSR,
   eCEEVT_HOLD_NET_XFER,
   eCEEVT_HOLD_USR_HOLD,
   eCEEVT_HOLD_USR_UNHOLD,
   eCEEVT_HOLD_USR_HOLD_OK,
   eCEEVT_HOLD_USR_UNHOLD_OK,
   eCEEVT_HOLD_USR_HOLD_FAIL,
   eCEEVT_HOLD_USR_UNHOLD_FAIL,
   eCEEVT_HOLD_USR_ENDED,
   eCEEVT_HOLD_USR_XFER,
   eCEEVT_HOLD_USR_BXFER,
   eCEEVT_HOLD_USR_XFER_NOTERM,
   eCEEVT_HOLD_SET_FINISH,     /* Event marker for HOLD FSM - end. */

   eCEEVT_SUB_SET_BEGIN,       /* Event marker for SUB FSM - start. */
   eCEEVT_SUB_NET_OK,
   eCEEVT_SUB_NET_SUBTRYING,
   eCEEVT_SUB_NET_REFRESH,
   eCEEVT_SUB_NET_FAIL,
   eCEEVT_SUB_NET_ENDED,
   eCEEVT_SUB_NET_UNSUBTRYING,
   eCEEVT_SUB_NET_UNSUB,
   eCEEVT_SUB_NET_RETRY,
   eCEEVT_SUB_NET_NOTIFY,
   eCEEVT_SUB_NET_REQ,
   eCEEVT_SUB_NET_REQEND,
   eCEEVT_SUB_USR_CREATE,
   eCEEVT_SUB_USR_SUBSCRIBE,
   eCEEVT_SUB_USR_UNSUBSCRIBE,
   eCEEVT_SUB_USR_NOTIFY,
   eCEEVT_SUB_USR_ENDED,
   eCEEVT_SUB_SET_FINISH,      /* Event marker for SUB FSM - end. */

   eCEEVT_NOT_SET_BEGIN,       /* Event marker for PUB FSM - start. */
   eCEEVT_NOT_NET_OK,
   eCEEVT_NOT_NET_FAIL,
   eCEEVT_NOT_NET_MSG,
   eCEEVT_NOT_USR_PUBLISH,
   eCEEVT_NOT_USR_ENDED,
   eCEEVT_NOT_USR_RSP,
   eCEEVT_NOT_USR_MSG,
   eCEEVT_NOT_SET_FINISH,      /* Event marker for PUB FSM - end. */
};

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */

void ceInit( void );
void ceDeInit( void );
CCTKSTATUS ceShutDown( void );
CCTKSTATUS ceNotify( BOS_UINT32 uHdl, CEEVT eEvt, void *pData );
CCTKSTATUS ceOutEvtInfo( BOS_UINT32 uUsrHdl, BOS_UINT32 uEvtHdl, CCTKEVT eEvt );

void ceShow( void );

BOS_UINT32 ceGuid( void );
BOS_BOOL ceResAlloc( void );

CCTKMEDINFO *ceGetMediaInfo( BOS_UINT32 uCid );
CCTKEXPIREINFO *ceGetExpInfo( BOS_UINT32 uCid);
CCTKCDR *ceGetLastCallIn( BOS_UINT32 uUid );
CCTKCDR *ceGetLastCallOut( BOS_UINT32 uUid );
BOS_UINT32 ceGetPmRes( void *pData );
void ceSetCdrInfo( BOS_UINT32 uResGuid,
                   const BOS_UINT8 *puCid,
                   const BOS_UINT8 *puFromTag,
                   const BOS_UINT8 *puToTag );
BOS_UINT32 ceGetRegId( BOS_UINT32 uUid );
BOS_UINT32 ceGetLocInfo( BOS_UINT8 *puData,
                         BOS_UINT32 uData,
                         BOS_UINT32 uResGuid );
void ceGetProfInfo( BOS_UINT32 uResGuid,
                    BOS_UINT32 *puParentGuid,
                    BOS_UINT32 *puProfIx );
void ceGetFeatInfo( BOS_UINT32 uResGuid,
                    CCTKFEATINFO *pFeatInfo );
BOS_BOOL ceSetCallInfo( BOS_UINT32 uResGuid,
                        CCTKCALLMETRICS *pStats );
BOS_BOOL ceSetPrivInfo( BOS_UINT32 uResGuid, 
                        CCTKPRIVCFG *pPriv );
BOS_BOOL ceGetPrecond( BOS_UINT32 uUid );
BOS_BOOL ceUsrAuthChg( BOS_UINT32 uResGuid );
void ceUsrAuthRem( BOS_UINT32 uResGuid, BOS_BOOL bRem );
void *ceGetNetHdl( BOS_UINT32 uResGuid );
BOS_UINT32 ceGetUsrGuid( BOS_UINT32 uGuid );
MXD_GNS::CSocketAddr *ceGetResRstrictAddr( BOS_UINT32 uResGuid );
BOS_BOOL ceRemMgmtRes( BOS_UINT32 uResGuid );
void ceSetPublishAddr( BOS_UINT32 uResGuid, MX_NS CSocketAddr *pPubAddr );
BOS_BOOL ceGetCallXRtpStat( BOS_UINT32 uResGuid, CCTKXRTPSTAT* pStat);

#ifdef __cplusplus
}
#endif

#endif /* __CE_API__H__INCLUDED__ */
