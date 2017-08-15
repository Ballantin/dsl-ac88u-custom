/**********************************************************************************
** Copyright (c) 2012 Broadcom Corporation
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
**      This file implements the base finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmBase.h>
#include <ceResBase.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* Defines the name of each FSM in a human readable format.
*/
const BOS_UINT8 *gpcCeFsmName[] =
{
   CCTK_STR( "INC" ),
   CCTK_STR( "OUT" ),
   CCTK_STR( "CALL" ),
#if CCTKCFG_REMOTE_USER_CONTROL
   CCTK_STR( "PROXY_IN" ),
   CCTK_STR( "PROXY_OUT" ),
#endif
   CCTK_STR( "HOLD" ),
   CCTK_STR( "XFER" ),
   CCTK_STR( "PUB" ),
   CCTK_STR( "SUB" ),
   CCTK_STR( "REG" ),
   CCTK_STR( "INV" ),
};

/* Defines the name of each FSM state in a human readable format.
*/
const BOS_UINT8 *gpcCeFsmState[] =
{
   CCTK_STR( "IDLE" ),
   CCTK_STR( "REG_SET_BEGIN" ),
   CCTK_STR( "REG_IDLE" ),
   CCTK_STR( "REG_REGING" ),
   CCTK_STR( "REG_REGED" ),
   CCTK_STR( "REG_SET_FINISH" ),
   CCTK_STR( "CALLIN_SET_BEGIN" ),
   CCTK_STR( "CALLIN_IDLE" ),
   CCTK_STR( "CALLIN_ALERT" ),
   CCTK_STR( "CALLIN_SET_FINISH" ),
   CCTK_STR( "CALLOUT_SET_BEGIN" ),
   CCTK_STR( "CALLOUT_IDLE" ),
   CCTK_STR( "CALLOUT_ATTEMPT" ),
   CCTK_STR( "CALLOUT_ALERT" ),
   CCTK_STR( "CALLOUT_SET_FINISH" ),
   CCTK_STR( "CALL_SET_BEGIN" ),
   CCTK_STR( "CALL_IDLE" ),
   CCTK_STR( "CALL_CONNECT" ),
   CCTK_STR( "CALL_CLEAN" ),
   CCTK_STR( "CALL_SET_FINISH" ),
   CCTK_STR( "XFER_SET_BEGIN" ),
   CCTK_STR( "XFER_IDLE" ),
   CCTK_STR( "XFER_ACCEPT" ),
   CCTK_STR( "XFER_SET_FINISH" ),
   CCTK_STR( "HOLD_SET_BEGIN" ),
   CCTK_STR( "HOLD_IDLE" ),
   CCTK_STR( "HOLD_PENDING" ),
   CCTK_STR( "HOLD_ACCEPT" ),
   CCTK_STR( "HOLD_SET_FINISH" ),
   CCTK_STR( "SUB_SET_BEGIN" ),
   CCTK_STR( "SUB_IDLE" ),
   CCTK_STR( "SUB_SUBING" ),
   CCTK_STR( "SUB_UNSUBING" ),
   CCTK_STR( "SUB_SUBED" ),
   CCTK_STR( "SUB_SET_FINISH" ),
   CCTK_STR( "PUB_SET_BEGIN" ),
   CCTK_STR( "PUB_IDLE" ),
   CCTK_STR( "PUB_PUBING" ),
   CCTK_STR( "PUB_SET_FINISH" ),
};

/* Defines the name of each FSM event in a human readable format.
*/
const BOS_UINT8 *gpcCeFsmEvent[] =
{
   CCTK_STR( "INV" ),
   CCTK_STR( "DNP" ),
   CCTK_STR( "USR_ADD" ),
   CCTK_STR( "USR_REM" ),
   CCTK_STR( "CALL_ADD" ),
   CCTK_STR( "CALL_REM" ),
   CCTK_STR( "SUB_ADD" ),
   CCTK_STR( "SUB_REM" ),
   CCTK_STR( "NOT_ADD" ),
   CCTK_STR( "NOT_REM" ),
   CCTK_STR( "STARTUP" ),
   CCTK_STR( "RESET" ),
   CCTK_STR( "SUSPEND_NEW" ),
   CCTK_STR( "RESUME_ALL") ,
   CCTK_STR( "REG_SET_BEGIN" ),
   CCTK_STR( "REG_NET_UNREG" ),
   CCTK_STR( "REG_NET_TRYING" ),
   CCTK_STR( "REG_NET_OK" ),
   CCTK_STR( "REG_NET_REFRESH" ),
   CCTK_STR( "REG_NET_FAIL" ),
   CCTK_STR( "REG_NET_ENDED" ),
   CCTK_STR( "REG_NET_TIMEOUT" ),
   CCTK_STR( "REG_NET_CHALLENGE" ),
   CCTK_STR( "REG_NET_SRV_FAIL" ),
   CCTK_STR( "REG_NET_NEXT_NONCE" ),
   CCTK_STR( "REG_NET_OPTIONS" ),
   CCTK_STR( "REG_NET_REFER" ),
   CCTK_STR( "REG_NET_REFENDED" ),
   CCTK_STR( "REG_NET_PAU_ID" ),
   CCTK_STR( "REG_NET_USR_ADD" ),
   CCTK_STR( "REG_NET_USR_REMOVE" ),
   CCTK_STR( "REG_USR_REG" ),
   CCTK_STR( "REG_USR_UNREG" ),
   CCTK_STR( "REG_USR_ENDED" ),
   CCTK_STR( "REG_USR_KA_OK" ),
   CCTK_STR( "REG_USR_KA_FAIL" ),
   CCTK_STR( "REG_USR_KA_RECOVERY" ),
   CCTK_STR( "REG_USR_PUBLISH" ),
   CCTK_STR( "REG_USR_ADD_OK" ),
   CCTK_STR( "REG_USR_SVC_ENA" ),
   CCTK_STR( "REG_USR_SVC_DIS" ),
   CCTK_STR( "REG_USR_TARGET_FAIL" ),
   CCTK_STR( "REG_USR_MSG" ),
   CCTK_STR( "REG_USR_INBOUND_TARGET" ),
   CCTK_STR( "REG_SET_FINISH" ),
   CCTK_STR( "CALL_SET_BEGIN" ),
   CCTK_STR( "CALL_NET_CALLED" ),
   CCTK_STR( "CALL_NET_TERMUSR" ),
   CCTK_STR( "CALL_NET_TERMNET" ),
   CCTK_STR( "CALL_NET_FAILED" ),
   CCTK_STR( "CALL_NET_ANSWER" ),
   CCTK_STR( "CALL_NET_RING" ),
   CCTK_STR( "CALL_NET_EARLYMEDIA" ),
   CCTK_STR( "CALL_NET_REPLACE" ),
   CCTK_STR( "CALL_NET_MEDIA" ),
   CCTK_STR( "CALL_NET_MEDIA_FAIL" ),
   CCTK_STR( "CALL_NET_HOLD" ),
   CCTK_STR( "CALL_NET_XFER" ),
   CCTK_STR( "CALL_NET_PEND_OFFER_SDP" ),
   CCTK_STR( "CALL_NET_PEND_ANSWER_SDP" ),
   CCTK_STR( "CALL_NET_ANSWER_SDP" ),
   CCTK_STR( "CALL_NET_INFO" ),
   CCTK_STR( "CALL_USR_CREATE" ),
   CCTK_STR( "CALL_USR_MAKE" ),
   CCTK_STR( "CALL_USR_MEDIA" ),
   CCTK_STR( "CALL_USR_ANSWER" ),
   CCTK_STR( "CALL_USR_HOLD" ),
   CCTK_STR( "CALL_USR_UNHOLD" ),
   CCTK_STR( "CALL_USR_FORWARD" ),
   CCTK_STR( "CALL_USR_XFER" ),
   CCTK_STR( "CALL_USR_BXFER" ),
   CCTK_STR( "CALL_USR_XFER_NOTRM" ),
   CCTK_STR( "CALL_USR_ENDED" ),
   CCTK_STR( "CALL_USR_PROXY" ),
   CCTK_STR( "CALL_USR_DTMF" ),
   CCTK_STR( "CALL_USR_FLASH" ),
   CCTK_STR( "CALL_SET_FINISH" ),
   CCTK_STR( "XFER_SET_BEGIN" ),
   CCTK_STR( "XFER_NET_XFER" ),
   CCTK_STR( "XFER_NET_OKAY" ),
   CCTK_STR( "XFER_NET_FAIL" ),
   CCTK_STR( "XFER_NET_PROC" ),
   CCTK_STR( "XFER_NET_ONGOING" ),
   CCTK_STR( "XFER_NET_TERMNET" ),
   CCTK_STR( "XFER_NET_REFENDED" ),
   CCTK_STR( "XFER_NET_ANSWER" ),
   CCTK_STR( "XFER_NET_RING" ),
   CCTK_STR( "XFER_NET_EARLYMEDIA" ),
   CCTK_STR( "XFER_NET_ACCEPT" ),
   CCTK_STR( "XFER_USR_XFER" ),
   CCTK_STR( "XFER_USR_BXFER" ),
   CCTK_STR( "XFER_USR_XFER_NOTRM" ),
   CCTK_STR( "XFER_USR_NTTO" ),
   CCTK_STR( "XFER_USR_ENDED" ),
   CCTK_STR( "XFER_SET_FINISH" ),
   CCTK_STR( "HOLD_SET_BEGIN" ),
   CCTK_STR( "HOLD_NET_MEDIA" ),
   CCTK_STR( "HOLD_NET_HOLD" ),
   CCTK_STR( "HOLD_NET_UNHOLD" ),
   CCTK_STR( "HOLD_NET_RING" ),
   CCTK_STR( "HOLD_NET_TERMNET" ),
   CCTK_STR( "HOLD_NET_TERMUSR" ),
   CCTK_STR( "HOLD_NET_XFER" ),
   CCTK_STR( "HOLD_USR_HOLD" ),
   CCTK_STR( "HOLD_USR_UNHOLD" ),
   CCTK_STR( "HOLD_USR_HOLD_OK" ),
   CCTK_STR( "HOLD_USR_UNHOLD_OK" ),
   CCTK_STR( "HOLD_USR_HOLD_FAIL" ),
   CCTK_STR( "HOLD_USR_UNHOLD_FAIL" ),
   CCTK_STR( "HOLD_USR_ENDED" ),
   CCTK_STR( "HOLD_USR_XFER" ),
   CCTK_STR( "HOLD_USR_BXFER" ),
   CCTK_STR( "HOLD_USR_XFER_NOTRM" ),
   CCTK_STR( "HOLD_SET_FINISH" ),
   CCTK_STR( "SUB_SET_BEGIN" ),
   CCTK_STR( "SUB_NET_OK" ),
   CCTK_STR( "SUB_NET_SUBTRYING" ),
   CCTK_STR( "SUB_NET_REFRESH" ),
   CCTK_STR( "SUB_NET_FAIL" ),
   CCTK_STR( "SUB_NET_ENDED" ),
   CCTK_STR( "SUB_NET_UNSUBTRYING" ),
   CCTK_STR( "SUB_NET_UNSUB" ),
   CCTK_STR( "SUB_NET_RETRY" ),
   CCTK_STR( "SUB_NET_NOTIFY" ),
   CCTK_STR( "SUB_NET_REQ" ),
   CCTK_STR( "SUB_NET_REQEND" ),
   CCTK_STR( "SUB_USR_CREATE" ),
   CCTK_STR( "SUB_USR_SUBSCRIBE" ),
   CCTK_STR( "SUB_USR_UNSUBSCRIBE" ),
   CCTK_STR( "SUB_USR_NOTIFY" ),
   CCTK_STR( "SUB_USR_ENDED" ),
   CCTK_STR( "SUB_SET_FINISH" ),
   CCTK_STR( "NOT_SET_BEGIN" ),
   CCTK_STR( "NOT_NET_OK" ),
   CCTK_STR( "NOT_NET_FAIL" ),
   CCTK_STR( "NOT_NET_MSG" ),
   CCTK_STR( "NOT_USR_PUBLISH" ),
   CCTK_STR( "NOT_USR_ENDED" ),
   CCTK_STR( "NOT_USR_RSP" ),
   CCTK_STR( "NOT_USR_MSG" ),
   CCTK_STR( "NOT_SET_FINISH" ),
};

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmBase::fsmNoOp
**
**  PURPOSE:    Generic 'no-operation' handler.
**
**  PARAMETERS:
**              rRes - the resource to run this operation on.
**
**
**  RETURNS:    None
**
**  NOTES:      This function does nothing, this is not an error, it is instead
**              voluntary ignorance.
***********************************************************************************/
void CeFsmBase::fsmNoOp( CeResBase &rRes )
{
   CCTK_TRACED4( "fsmNoOp (0x%lX - FSM %s) - state (%s)/event (%s) ignored...",
                 rRes.resourceGuid(),
                 gpcCeFsmName[rRes.resourceFsm()],
                 gpcCeFsmState[rRes.resourceState()],
                 gpcCeFsmEvent[static_cast<BOS_UINT32>(rRes.resourceEvent())
                                                               - eCEEVT_INV] );
}


/***********************************************************************************
**  FUNCTION:   CeFsmBase::fsmInvalid
**
**  PURPOSE:    Generic 'invalid' handler.
**
**  PARAMETERS:
**              rRes - the resource to run this operation on.
**
**
**  RETURNS:    None
**
**  NOTES:      This function is run when the state/event combination seen is
**              invalid.  It essentially does nothing, but will log an error.
***********************************************************************************/
void CeFsmBase::fsmInvalid( CeResBase &rRes )
{
   CCTK_TRACEI4( "fsmInvalid (0x%lX - FSM %s) - invalid event (%s) in state (%s).",
                 rRes.resourceGuid(),
                 gpcCeFsmName[rRes.resourceFsm()],
                 gpcCeFsmEvent[static_cast<BOS_UINT32>(rRes.resourceEvent())
                                                               - eCEEVT_INV],
                 gpcCeFsmState[rRes.resourceState()] );
}


/***********************************************************************************
**  FUNCTION:   CeFsmBase::fsmError
**
**  PURPOSE:    Generic 'error' handler.
**
**  PARAMETERS:
**              rRes - the resource to run this operation on.
**
**
**  RETURNS:    None
**
**  NOTES:      This function is run when the fsm/state/event combination seen is
**              invalid.  This situation is critical because it is due to one of
**              the following things:
**
**                  [I] The resource is not associated with its proper FSM, hence
**                      it cannot be processed or used properly.
**
**                 [II] The resource is not whom one may think it is (a slight
**                      variation from [I]).
**
**                [III] The resource may have been engaged in a specific service
**                      without proper update of its FSM for that service.
**
**                 [IV] The resource FSM has been updated for a given service but
**                      the resource state was not updated accordingly.
***********************************************************************************/
void CeFsmBase::fsmError( CeResBase &rRes )
{
   CCTK_TRACEI4( "fsmError (0x%lX - FSM %s) - erroneous state (%s)/event (%s).",
                 rRes.resourceGuid(),
                 gpcCeFsmName[rRes.resourceFsm()],
                 gpcCeFsmState[rRes.resourceState()],
                 gpcCeFsmEvent[static_cast<BOS_UINT32>(rRes.resourceEvent())
                                                               - eCEEVT_INV] );
}
