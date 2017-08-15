/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom Corporation
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
**      This module implements the finite state machine that runs the Call Manager
**      application.
**
**      This is the FSM for CCTK interface events.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CM_FSM__START_CWT_DATA          "start-cwt"
#define CM_FSM__STOP_CWT_DATA           "stop-cwt"

#define CM_FSM__PLAY_TONE_DATA          "play tone"
#define CM_FSM__STOP_TONE_DATA          "stop"
#define CM_FSM__CALLING_NAME_DATA       "calling-name:"
#define CM_FSM__CALLING_NUM_DATA        "calling-number:"

/* ---- Private Variables ------------------------------------------------------- */
static const CCTKCLIDINFO defaultClid =
{
   "",      /* Name */
   "P"      /* Number */
};


/* The finite state machine for handling the CCTK events.
*/
static const CMFSMVOIDFUNCTIONPTR
   gpCmCctkFsm[eCMST_MAX][eCMEVT_CCTK_END - eCMEVT_CCTK_START - 1] =
{
   /* ---- eCMST_IDLE ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallNew,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmCallAlert,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmHoldFail,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmDlg,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmForward,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmXferAccept,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmPublishTarget,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },

   /* ---- eCMST_ACCEPTED ------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmCallAlert,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmDlg,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmForward,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmNoOp,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },

   /* ---- eCMST_DIALING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallRejected,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmCallRingback,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmCallConnect,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmCallBusy,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmCallError,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmRemHold,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmRemUnHold,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmHoldOkay,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmMedia,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmMedia,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmDlg,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmForward,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmNoOp,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },

   /* ---- eCMST_RINGBACK ------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallRejected,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmCallXfered,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmCallConnect,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmCallBusy,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmCallError,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmUnHoldOkay,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmMedia,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmMediaXfer,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmDlg,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmForward,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmXferAccept,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmNoOp,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },

   /* ---- eCMST_RINGING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnding,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnding,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmCallXfer,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmMedia,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmDlg,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmForward,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmNoOp,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },

   /* ---- eCMST_TALK ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallNew,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmCallAlert,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmCallXfered,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmCallConnect,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmCallXfer,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmRemHold,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmRemUnHold,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmHoldOkay,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmUnHoldOkay,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmHoldFail,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmUnHoldFail,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmMedia,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmMediaFail,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmMediaXfer,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmDlg,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmForward,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmXferAccept,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmOobDtmf,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmCwtInfo,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmNoOp,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },

   /* ---- eCMST_FLASH ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmRemHold,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmRemUnHold,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmHoldOkay,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmUnHoldOkay,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmHoldFail,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmUnHoldFail,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmMedia,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmDlg,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmForward,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmXferAccept,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmOobDtmf,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmNoOp,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },

   /* ---- eCMST_RELEASING ------------------------------------------------------ */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallNew,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmDlg,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmForward,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmXferAccept,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmPublishTarget,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },

   /* ---- eCMST_ERROR ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFERED_CALL ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ERROR_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFER_REQUEST ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_REMOTE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_UNHOLD_REMOTE ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_LOCAL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_UNHOLD_LOCAL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_LOCAL_FAIL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_UNHOLD_LOCAL_FAIL ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_INFO ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_FAILED ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_XFER ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MWI_INFO --------------- */ &cmFsmMwi,
      /* ---- eCMEVT_DLG_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_FORWARD_INFO ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_XFER_ACCEPT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_UPDATE ------------ */ &cmFsmUsrUpdate,
      /* ---- eCMEVT_PAU_UPDATE ------------- */ &cmFsmPauUpdate,
      /* ---- eCMEVT_OOB_DTMF --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CWT_INFO --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_USER_IN_SERVICE -------- */ &cmFsmInSrv,
      /* ---- eCMEVT_USER_OUT_OF_SERVICE ---- */ &cmFsmOutOfSrv,
      /* ---- eCMEVT_SERVICE_CONF ----------- */ &cmFsmSrvConf,
      /* ---- eCMEVT_SERVICE_END ------------ */ &cmFsmSrvEnd,
      /* ---- eCMEVT_RESET_COMP ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_OVER --------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEM_UNDER -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TARGETS_UPDATE --------- */ &cmFsmTargetsUpdate,
      /* ---- eCMEVT_CDR_INFO --------------- */ &cmFsmCdrInfo,
      /* ---- eCMEVT_PUBLISH_TARGET ----------*/ &cmFsmPublishTarget,
      /* ---- eCMEVT_SUB_STATUS --------------*/ &cmFsmSubStatus,
#if CMGRCFG_REMOTE_USER_CONTROL
      /* ---- eCMEVT_REMOTE_USER_ADD -------- */ &cmFsmUpdateRemoteUsr,
      /* ---- eCMEVT_REMOTE_USER_REMOVE ----- */ &cmFsmRemoveRemoteUsr,
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
      /* ---- eCMEVT_MESSAGE ---------------- */ &cmFsmMessage,
      /* ---- eCMEVT_MESSAGE_RESP ----------- */ &cmFsmMessageRsp,
#endif /* CMGRCFG_PASS_THRU_MSG */
   },
};

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmFsmCctkRun
**
** PURPOSE:    Main entry for running the CCTK FSM.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCctkRun ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   BOS_UINT32 uSta;
   BOS_UINT32 uEvt;

   uSta = pUsr->eSta[ CMFSM_CURRENT_STATE_IX ];
   uEvt = pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ];

   CMGRLOG_INFORMATION (( "cmFsmCctkRun: usr 0x%X - [%s (0x%X), %s (0x%X)]",
                          pUsr->uHdl,
                          (char *) cmMapGetStr ( cmMapCmState, uSta ), uSta,
                          (char *) cmMapGetStr ( cmMapCmEvent, uEvt ), uEvt ));

   if ( uEvt != eCMEVT_INVALID )
   {
      (gpCmCctkFsm [ uSta ][ uEvt - (eCMEVT_CCTK_START + 1) ]) ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmMwi
**
** PURPOSE:    FSM received MWI information.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmMwi ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CCTKMWIINFO *pMwi = NULL;
   BOS_BOOL bAlloc = BOS_FALSE;
   BOS_BOOL bVMWI = BOS_TRUE;
   BOS_BOOL bNoOffhookMWI = BOS_FALSE;
   CFGSIPMWI *pMwiCfg = NULL;
   PROV_CFG_IDX cfgIdx;

   if ( ( pMwi = (CCTKMWIINFO *) pUsr->evt.pDat3 ) != NULL )
   {
      CMGRLOG_DEBUG (( "cmFsmMwi: usr 0x%X (0x%X): %s (%d, %d, %d, %d)",
                       pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ),
                       ( pMwi->bMsgStatus ? "Msg" : "No-msg" ),
                       pMwi->voiceMsg.newMsg, pMwi->voiceMsg.oldMsg,
                       pMwi->voiceMsg.newUrgMsg, pMwi->voiceMsg.oldUrgMsg ));

      /* Save the knowledge on the current MWI status to apply further
      ** process when needed (such as stutter dial tone for example).
      */
      pUsr->bMwi = ( pMwi->bMsgStatus ) ? BOS_TRUE : BOS_FALSE;

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, MWIConcept ),
                                 &cfgIdx,
                                 (void *) &pMwiCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pMwiCfg && !pMwiCfg->Act.bCfgStatus )
         {
            bVMWI = BOS_FALSE;
         }

         if ( pMwiCfg )
         {
            bNoOffhookMWI = pMwiCfg->bNoOffhookMWI;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, MWIConcept ),
                               &cfgIdx,
                               (void *) pMwiCfg,
                               CFG_ITEM_DEL );
         pMwiCfg = NULL;
      }

      /* Only apply Visual Message Waiting Indicator if configured to do so.
      */
      if ( bVMWI )
      {
         /* if we are in error state, defer the generation of MWI until
          * we go to a non-error state
          * */
         if ( ((bNoOffhookMWI == BOS_TRUE) && (pUsr->bOffHook == BOS_TRUE)) 
            || (pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_ERROR) ) 
         {
            pUsr->bPendingMwi = BOS_TRUE;
         }
         else
         {
            /* Signal visual message waiting indication to the physical interface
             ** of interest associated with this user.
             */
            if ( pUsr->uPhys == CMGR_INVALID_HANDLE )
            {
               bAlloc = ( cmPhyAllocate ( pUsr ) == eCMGRSTATUS_COMMAND_SUCCESS );
            }

            /* If we have a valid interface to signal on.
             */
            if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
            {
               CMGREPSIG sig;

               memset ( (void *) &sig,
                     0,
                     sizeof ( CMGREPSIG ) );

               sig.eAct = ( pMwi->bMsgStatus ?
                     eCMGREPSIGACT_START : eCMGREPSIGACT_STOP );
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
               sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
               sig.uSig = eEPTSIG_VMWI;

               cmPhySignal ( pUsr->uPhys, &sig );
            }

            /* If we allocated the interface just for this purpose, release it now.
             */
            if ( bAlloc )
            {
               pUsr->uPhys = CMGR_INVALID_HANDLE;
            }
         }
      }

      if ( gCmCfgCb.cmgrEvtCb )
      {
         gCmCfgCb.cmgrEvtCb ( ( pMwi->bMsgStatus ?
                  eCMGREVT_MWI_ON : eCMGREVT_MWI_OFF ),
                              pUsr->uHdl,
#if CMGRCFG_MULTILINE
                              pUsr->uHdl,
                              CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                              NULL );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmUsrUpdate
**
** PURPOSE:    FSM user information update handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmUsrUpdate ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_UINT32 *pNewNetHdl;
   BOS_UINT32 uCurrNetHdl = cmUsrGetNtwkHdl( pUsr );

   if ( pUsr->evt.pDat3 &&
        ( pNewNetHdl = (BOS_UINT32 *) pUsr->evt.pDat3 ) != NULL )
   {
      /* Re-assign the network (CCTK) handle, note that following this action,
      ** any event coming from CCTK refering to the previous network handle would
      ** be ignored, this action is to be executed with care and as directed
      ** by the CCTK layer only.
      */
      CMGRLOG_INFORMATION (( "cmFsmUsrUpdate: usr 0x%X, net 0x%X -> net 0x%X",
                             pUsr->uHdl,
                             cmUsrGetCurrNtwkHdl( pUsr ),
                             *pNewNetHdl ));

      /* If there are sessions that need to be updated with new network handle
      ** do so now.
      */
      if ( cmNumSesWithNet(uCurrNetHdl) > 0 )
      {
            CMSESLIST sesList;
            BOS_UINT32 i;

            memset( &sesList, 0, sizeof( CMSESLIST ) );

            cmSesListWithNet( uCurrNetHdl, &sesList );
            for( i = 0 ; i < sesList.uLength ; i++ )
            {
               CMSES *pSes = cmSesValid( sesList.uSes[ i ] );

               if( pSes )
               {
                  pSes->uNetUsr = *pNewNetHdl;
               }
            }
      }

      /* Update the user's network handle
      */
      cmUsrSetNtwkHdl( pUsr, *pNewNetHdl );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmPauUpdate
**
** PURPOSE:    FSM P-Associated-URI information update handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmPauUpdate ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   CCTKPAUINFO *pPAURIs = NULL;
   CFGSIPUSERCFG *pUsrCfg = NULL;

   if ( pUsr->evt.pDat3 &&
        ( pPAURIs = (CCTKPAUINFO *) pUsr->evt.pDat3 ) != NULL )
   {
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                 &cfgIdx,
                                 (void *) &pUsrCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pUsrCfg && pUsrCfg->pcPAURIs )
         {
            /* remember our already allocated struct */
            BOS_UINT8 *pPAUtmp = pUsrCfg->pcPAURIs;
            /* point to the new value */
            pUsrCfg->pcPAURIs = pPAURIs->pau;
            /* this will have the effect of copying the data to
            ** the mib data structure */
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                  &cfgIdx,
                                  (void *) pUsrCfg,
                                  CFG_ITEM_SET );
            /* prevent memory leaks */
            pUsrCfg->pcPAURIs = pPAUtmp;
         }
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pUsrCfg,
                               CFG_ITEM_DEL );
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmInSrv
**
** PURPOSE:    FSM in-service handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmInSrv ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CCTKKAINFO *pKaInfo = NULL;
   PROV_CFG_IDX cfgIdx;
   CFGSIPUSERCFG *pUsrCfg = NULL;
   BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

   CMGRLOG_INFORMATION (( "cmFsmInSrv: usr 0x%X, net 0x%X",
                          pUsr->uHdl,  uAccount ));

   /* Validate that the network user handle is the one currently assigned
   ** to the local user.  If it is not, ignore the event and warn the user.
   ** If it is, continue with processing.  This check is in place because
   ** an 'in-service' event may be triggered at the same time as an 'update'
   ** event from the network layer but in the end at this level we want to
   ** process only the event that is associated with the correct user owning
   ** the resource.
   */
   if ( cmUsrGetNtwkHdl( pUsr ) != pUsr->evt.uDat2 )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmInSrv [IGNORE]: usr 0x%X - (net 0x%X != evt-net 0x%X)",
         pUsr->uHdl, uAccount, pUsr->evt.uDat2 ));

      /* Exit now, the event does not need to be processed such that no
      ** false information is being reported.
      */
      return;
   }

#if !CMGRCFG_MULTILINE
   /* User is in service.
   */
   pUsr->bInSrv = BOS_TRUE;

   if ( gCmCfgCb.cmgrEvtCb )
   {
      ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_OK,
                               pUsr->uHdl,
#if CMGRCFG_MULTILINE
                               pUsr->uHdl,
                               CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                               (void *) pUsr->uUid );
   }
#endif /* !CMGRCFG_MULTILINE */

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) &pUsrCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      /* If we have received extra information with this event, this is
      ** some data we need to populate the configuration dynamic information.
      */
      if ( pUsr->evt.pDat3 &&
         ( pKaInfo = (CCTKKAINFO *) pUsr->evt.pDat3 ) != NULL )
      {
         if ( pUsrCfg && pUsrCfg->Act.bCfgStatus )
         {
            CFGSIPPCSCF *pPcscfCfg;
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsrCfg->pcActiveDomain;
            cfgIdx.uIndex = pUsrCfg->uActiveDomain;
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                  &cfgIdx,
                                  (void *) &pPcscfCfg,
                                  CFG_ITEM_GET );

            pPcscfCfg->sProtIdUsed = ( pKaInfo->bDatagram ? CFG_SIP_PROT_ID_UDP :
                                     CFG_SIP_PROT_ID_TCP );
            bosIpAddrCreateFromStr( (const char *)pKaInfo->uKaPeer,
                                    &pPcscfCfg->PCSCFAddrUsed );

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                  &cfgIdx,
                                  (void *) pPcscfCfg,
                                  CFG_ITEM_SET );
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                      &cfgIdx,
                      (void *) pPcscfCfg,
                      CFG_ITEM_DEL );

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );

         }
      }

      {
         /* Get the registration expiration information
         ** and pass it to user configuration */
         CCTKEXPIREINFO expInfo;
         cctkGetParm ( uAccount, eCCTKPARM_EXPINFO, (void *)&expInfo );

         /* Update the registration expiry and re-reg info as an absolute time
         ** instead of an interval from expInfo.tUpdateTime
         */
         BOS_UINT32 uTime = 0;
         if ( expInfo.tUpdateTime != (time_t)(-1))
         {
            uTime = ( BOS_UINT32 )expInfo.tUpdateTime;
         }
         pUsrCfg->uExp = uTime + expInfo.uExp;
         pUsrCfg->uReReg = uTime + expInfo.uReReg;
         pUsrCfg->bPrimary = expInfo.bPrimary;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) pUsrCfg,
                              CFG_ITEM_SET );
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pUsrCfg,
                               CFG_ITEM_DEL );
      }
      pUsrCfg = NULL;
   }

   /*If already off-hook, and configured to not generate a tone for out of service
     call off-hook state handler so in-service behavior takes place*/
   if (pUsr->bOffHook)
   {
      CFGPROVDEVICECFG *pDevCfg = NULL;
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                              &cfgIdx,
                              (void *) &pDevCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pDevCfg && pDevCfg->bNoToneOutOfSrv )
         {
            cmFsmOffHook ( (void *) pUsr );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                   &cfgIdx,
                   (void *) pDevCfg,
                   CFG_ITEM_DEL );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmOutOfSrv
**
** PURPOSE:    FSM out-of-service handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOutOfSrv ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_UINT32 uIx;
   BOS_BOOL bFree = BOS_FALSE;
   BOS_BOOL bIncSes = BOS_FALSE;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGPROVDEVICECFG *pDevCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bEndAllCalls = BOS_FALSE;
   BOS_BOOL bReportOperNok = BOS_TRUE;
   CFGSIPUSERCFG *pUsrCfg = NULL;
   BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

   CMGRLOG_WARNING (( "cmFsmOutOfSrv: usr 0x%X, net 0x%X!!!",
                       pUsr->uHdl,  uAccount ));

   CCTKEXPIREINFO expInfo;
   cctkGetParm ( uAccount, eCCTKPARM_EXPINFO, (void *)&expInfo );

   /* update the re-reg time as an absolute time
   ** instead of an interval from expInfo.tUpdateTime
   */
   BOS_UINT32 uTime = 0;
   if ( expInfo.tUpdateTime != (time_t)(-1))
   {
      uTime = (BOS_UINT32)expInfo.tUpdateTime;
   }

#if !CMGRCFG_MULTILINE
   /* User is out of service (lost registration).
   */
   pUsr->bInSrv = BOS_FALSE;

   /* Determine how the out-of-service event should be handled.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                              &cfgIdx,
                              (void *) &pDevCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pDevCfg )
      {
         /* Check to see whether the user can always be reported as
         ** non-operational when out of service. For special cases
         ** (such as 403 failures) this flag will be overwritten regardless.
         */
         if ( pDevCfg->bOperNokIffNoTgtsAvail )
         {
            bReportOperNok = BOS_FALSE;
         }
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                            &cfgIdx,
                            (void *) pDevCfg,
                            CFG_ITEM_DEL );
      pDevCfg = NULL;
   }


   if ( gCmCfgCb.cmgrEvtCb )
   {
      CMGREVT eEvent = eCMGREVT_OPER_NOK;

      /* Determine the cause of the out-of-service and handle it accordingly.
      */
      if(pUsr->evt.pDat3)
      {
         CMGRLOG_INFORMATION (( "cmFsmOutOfSrv: usr 0x%X, net 0x%X pUsr->evt.pDat3=0x%X",
                       pUsr->uHdl,  uAccount, *(CCTKRESPCODE *)pUsr->evt.pDat3));

         if (*(CCTKRESPCODE *)pUsr->evt.pDat3 == eCCTKRESPCODE_FORBIDDEN)
         {
            /*
            **  Send event to indicate if out of service due to 403 forbidden
            */
             eEvent = eCMGREVT_OPER_NOK_FORBIDDEN;
             bReportOperNok = BOS_TRUE;
         }
         else if (*(CCTKRESPCODE *)pUsr->evt.pDat3 == eCCTKRESPCODE_ALLREGTARGETSFAILED)
         {
            bReportOperNok = BOS_TRUE;
         }
      }

      if (pUsr->eCfgAction == eCMCFGACT_CFGUSRTERM)
      {
         /*We had a configuration action to terminate this user.
           Report the out of service*/
         bReportOperNok = BOS_TRUE;
      }

      /* Report the user as non operational if the event relates to any special
      ** cases or if configuration allows all reg failures to be reported.
      ** Otherwise the user will be out-of-service but still operational.
      */
      if ( bReportOperNok )
      {
         CMGROOSINFO oosInfo;
         oosInfo.uReRegTime = uTime + expInfo.uReReg;
         memcpy(oosInfo.uUid, pUsr->uUid, sizeof(pUsr->uUid));

         ( gCmCfgCb.cmgrEvtCb ) ( eEvent,
                                  pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                  pUsr->uHdl,
                                  CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                  (void *) &oosInfo );
      }
   }
#endif /* CMGRCFG_MULTILINE */

   /* Determine whether active calls need to be terminated.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                              &cfgIdx,
                              (void *) &pBasicCall,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pBasicCall &&
           pBasicCall->bEndCallsOnOos )
      {
         bEndAllCalls = BOS_TRUE;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                            &cfgIdx,
                            (void *) pBasicCall,
                            CFG_ITEM_DEL );
      pBasicCall = NULL;
   }

   /* Update registration info
   */
   if (!bReportOperNok)
   {
      /* update the registration info if it isn't already
      ** passed to the UserConcept
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if( gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) &pUsrCfg,
                               CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         pUsrCfg->uExp = 0;
         pUsrCfg->uReReg = uTime + expInfo.uReReg;

         gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) pUsrCfg,
                              CFG_ITEM_SET );
         gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) pUsrCfg,
                              CFG_ITEM_DEL );
         pUsrCfg = NULL;
      }
   }

   /* Transient session - as well as possibly active ones depending on the
   ** desired configured behavior - for this user should be terminated now.
   **
   ** One exception - unless instructed to terminate all resources - is if
   ** we have received an incoming call for this user and we consider that
   ** it is valid scenario to stay in service.
   */
   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] &&
           ( /* Transient session(s). */
             !pUsr->pMedSes [ uIx ]->bEstab ||
             /* Or established session with configured behavior overrule. */
             ( pUsr->pMedSes [ uIx ]->bEstab && bEndAllCalls ) ) )
      {
         if ( ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGING ) &&
              ( pUsr->pMedSes [ uIx ] == cmUsrFocusSes( pUsr ) ) &&
              !bEndAllCalls )
         {
            bIncSes = BOS_TRUE;
         }
         else
         {
            CMSES *pFreeSes = pUsr->pMedSes [ uIx ];

            CMGRLOG_INFORMATION ((
               "cmFsmOutOfSrv: Terminating call 0x%x (0x%X) for out-of-service user 0x%X (0x%X)",
               pFreeSes->uHdl, pFreeSes->uNet, pUsr->uHdl,  uAccount ));

            if( pFreeSes->uNetUsr != CMGR_INVALID_HANDLE )
            {
               cmSesTerminate ( pFreeSes );
            }
            else
            {
               /* Send a "RELEASE_CALL" message */
               CMEVT evt;

               memset ( &evt, 0, sizeof( CMEVT ) );

               evt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
               evt.uDat1 = pUsr->uHdl;                  /* User handle */
               evt.uDat2 = pFreeSes->uHdl; /* Call ID */
               evt.uDat4 = CMGR_INVALID_HANDLE;         /* CCTK user id */
               evt.bLast = BOS_TRUE;                    /* Last event */

               if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
               {
                  cmSignalEvt ();
               }
               cmUsrUnLinkSes ( pUsr, pFreeSes );

               /* Reset the network handle and free the session resource.
               */
               pFreeSes->uNet = CMGR_INVALID_HANDLE;
               cmSesFree ( pFreeSes, pUsr->uHdl );
            }

            bFree = BOS_TRUE;
         }
      }
      /* If the session was configured to be maintained, then the mark
      ** it as such so that special processing can be applied later if
      ** needed.
      */
      else if (  !bEndAllCalls &&
                 pUsr->pMedSes [ uIx ] && pUsr->pMedSes [ uIx ]->bEstab )
      {
         CMGRLOG_INFORMATION ((
               "cmFsmOutOfSrv: Maintaining call 0x%x (0x%X) for out-of-service user 0x%X (0x%X)",
               pUsr->pMedSes [ uIx ]->uHdl, pUsr->pMedSes [ uIx ]->uNet, pUsr->uHdl,  uAccount ));

         pUsr->pMedSes [ uIx ]->bMaintdOnOos = BOS_TRUE;
      }
   }

   for ( uIx = 0 ; uIx < CMGRCFG_SRVSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pSrvSes [ uIx ] &&
           !pUsr->pSrvSes [ uIx ]->bEstab )
      {
         cctkEventDelete ( pUsr->pSrvSes [ uIx ]->uNet );
      }
   }

   /* Revert the out-of-service decision if needed (there is an incoming
   ** call from the network, we assume we are in service even if registration
   ** says otherwise).
   */
   if ( bIncSes )
   {
#if !CMGRCFG_MULTILINE
      pUsr->bInSrv = BOS_TRUE;

      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_OK,
                                  pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                  pUsr->uHdl,
                                  CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                  (void *) pUsr->uUid );
      }
#endif /* CMGRCFG_MULTILINE */
   }
   /* Update state if needed.
   */
   else if ( bFree )
   {
      cmUsrNewState ( pUsr, eCMST_RELEASING );

      if ( ( cmUsrActiveSes ( pUsr ) == NULL ) &&
           ( cmUsrFocusSes ( pUsr ) == NULL ) &&
           ( pUsr->bOffHook ) )
      {
         CMGREPSIG sig;
         CMTMR tmr;

         memset ( (void *) &sig,
                  0,
                  sizeof ( CMGREPSIG ) );

         memset ( (void *) &tmr,
                  0,
                  sizeof ( CMTMR ) );

         /* Terminate any possible signal being played.
         */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uSig = pUsr->uCpt;
         sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
         sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );

         /* Terminate any possible timer.
         */
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_INVALID;

         cmTmrStop ( &tmr );

         /* Play reorder tone and start off-hook warning timer.
         */
         sig.eAct = eCMGREPSIGACT_START;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
         sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = eEPTSIG_REORDER;
         pUsr->uCpt = sig.uSig;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_OHWARN;
         tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;

         cmTmrStart ( &tmr );
      }
   }

   /* Finally, if we were waiting for such event to finalize a reconfiguration
   ** of the user, apply such now.
   */
   if ( pUsr->eCfgAction != eCMCFGACT_NOACTION )
   {
      cmUsrReConfig ( pUsr );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmSrvConf
**
** PURPOSE:    FSM service confirmation handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmSrvConf ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesFindNetHdl ( pUsr->evt.uDat2, pUsr->uHdl );

   if ( pSes )
   {
      CMGRLOG_DEBUG (( "cmFsmSrvConf: srv 0x%X (0x%X), usr 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if( !pSes->bEstab )
      {
         pSes->bEstab = BOS_TRUE;
#if CMGRCFG_MULTILINE
         cmCctkAccntCallAdd( pSes->uNetUsr );
#endif
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmSrvEnd
**
** PURPOSE:    FSM service terminated handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmSrvEnd ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesFindNetHdl ( pUsr->evt.uDat2, pUsr->uHdl );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmSrvEnd: srv 0x%X (0x%X), usr 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if ( (CMAUTOCALL *)pSes->ses.srv.pAutoCall &&
           ( (CMAUTOCALL *)pSes->ses.srv.pAutoCall )->bUsed )
      {
         CMTMR tmr;

         memset ( (void *) &tmr,
                  0,
                  sizeof ( CMTMR ) );

         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_INVALID;

         cmTmrStop ( &tmr );

         /* Terminate the auto-call service at the user level.
         */
         memset( (void *) (CMAUTOCALL *)pSes->ses.srv.pAutoCall,
                 0,
                 sizeof ( CMAUTOCALL ) );
      }

      cctkEventDelete ( pSes->uNet );

      cmUsrUnLinkSes ( pUsr, pSes );
      cmSesFree ( pSes, pUsr->uHdl );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallNew
**
** PURPOSE:    FSM new incoming call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallNew ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = NULL;
   CMSES *pFocusSes = cmUsrFocusSes ( pUsr );
   BOS_BOOL bAccept = BOS_FALSE;
   BOS_BOOL bTmrRun = BOS_FALSE;
   BOS_BOOL bEmergency = BOS_FALSE;
   BOS_BOOL bCallWait = BOS_FALSE;
   BOS_BOOL bHotlineNoCW = BOS_FALSE;
   BOS_BOOL bProxy = BOS_FALSE;
   BOS_BOOL bFwd = BOS_FALSE;
   BOS_BOOL bAlertInfoRS = BOS_FALSE;
   CCTKCALLINFO *pCall = NULL;
   CMTMR tmr;
   CFGSIPCALLFWD *pFwdCfg;
   CFGSIPHOTLINE *pHotLineCfg = NULL;
   PROV_CFG_IDX cfgIdx;

#if CMGRCFG_MULTILINE
   /* If this call is assigned to another user */
   if ( ( cmNumCidAssigned( pUsr->evt.uDat2 ) == 1 ) &&
        ( cmUsrFindCid( pUsr->evt.uDat2, BOS_TRUE ) != pUsr ) &&
        ( pUsr->evt.uDat4 != CMGR_INVALID_HANDLE ) )
   {
      /* Don't process anything */
      return;
   }
#endif

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_ONHOOK;
   bTmrRun = cmTmrIsRunning ( &tmr );

   if ( pUsr->evt.pDat3 )
   {
      pCall = (CCTKCALLINFO *)pUsr->evt.pDat3;
      bEmergency = pCall->bEmergency;
   }

   /* if incoming INVITE has Alert-Info with ringsplash:
   ** - respond with 487
   ** - provide ringsplash if:
   **     1. ring reminder is set to ringsplash via INVITE with Alert-Info
   **     2. line is onhook
   */
   BOS_UINT32 uIx = 0;
   while ( !bAlertInfoRS && (uIx < CCTKCFG_MAX_ALERT_INFO) )
   {
      /* Extract tone information from the alert-info that was passed if any.
      */
      if (cmPhyToneFromString ( pCall->cAlertInfo[ uIx ] ) == eEPTSIG_SPLASH )
      {
         CMGRLOG_INFORMATION ((
            "cmFsmCallNew: 0x%X (0x%X) - rs in Alert-Info",
            pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));
         cctkCallDelete ( pUsr->evt.uDat2, eCCTKCALLDELRSN_TERMINATED );
         bAlertInfoRS = BOS_TRUE;
      }
      /* Continue to loop through the different possible tones until we find
      ** the appropriate one (if any).
      */
      uIx++;
   }

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;

   /* Check whether to apply Call Forwarding features
   */
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                              &cfgIdx,
                              (void *) &pFwdCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pFwdCfg &&
           pFwdCfg->Act.bCfgStatus )
      {
         /* Check whether to apply Call Forward ring reminder
         */
         if ( bAlertInfoRS )
         {
            /* ring reminder is set to ringsplash via INVITE Alert-Info */
            if ( (pFwdCfg->bRingReminder) &&
                  !pUsr->bOffHook )
            {

               BOS_BOOL bAlloc = BOS_FALSE;

               /* Signal ring-splash to the physical interface
               ** of interest associated with this user.
               */
               if ( pUsr->uPhys == CMGR_INVALID_HANDLE )
               {
                  bAlloc = ( cmPhyAllocate ( pUsr ) == eCMGRSTATUS_COMMAND_SUCCESS );
               }

               /* If we have a valid interface to signal on.
               */
               if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
               {
                  CMGREPSIG sig;

                  sig.eAct = eCMGREPSIGACT_START;
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = CMGR_INVALID_HANDLE;
      #if CMGRCFG_MULTILINE
                  sig.uLine = CMGR_INVALID_HANDLE; /* TODO: Fix forwarding to properly pass the cid/line */
      #endif /* CMGRCFG_MULTILINE */
                  sig.uSig = eEPTSIG_SPLASH;
                  cmPhySignal ( pUsr->uPhys, &sig );
               }

               /* If we allocated the interface just for this purpose, release it now.
               */

               if ( bAlloc )
               {
                  pUsr->uPhys = CMGR_INVALID_HANDLE;
               }
            }
         }
         else if ( pFwdCfg->bUsrSet )
         {
            /* Check whether to apply any automatic call forwarding.
            */
            bFwd = BOS_TRUE;

            CMGRLOG_WARNING ((
               "cmFsmCallNew: forward 0x%X on 0x%X (0x%X) to \'%s\'",
               pUsr->evt.uDat2, pUsr->uHdl, pUsr->uNet,
               ( pFwdCfg->bUseFwdDString ?
               ( pFwdCfg->pcFwdToDString ? pFwdCfg->pcFwdToDString : "NULL" ) :
               ( pFwdCfg->pcFwdToUri ? pFwdCfg->pcFwdToUri : "NULL" ))
               ));

            if ( !(pFwdCfg->bUseFwdDString) && (pFwdCfg->pcFwdToUri != NULL) )
            {
               cctkCallForward( pUsr->evt.uDat2,
                             pFwdCfg->pcFwdToUri );
            }
            else
            {
               if ( (pFwdCfg->bUseFwdDString) && (pFwdCfg->pcFwdToDString != NULL) )
               {
                  cmFsmCallForwardViaDialString( pUsr, pUsr->evt.uDat2, (char *)pFwdCfg->pcFwdToDString);
               }
            }
         }
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                            &cfgIdx,
                            (void *) pFwdCfg,
                            CFG_ITEM_DEL );
      pFwdCfg = NULL;
   }

   if ( bFwd || bAlertInfoRS )
   {
      /* We are done here...
      */
      return;
   }

#if CMGRCFG_MULTILINE
   /* Do not allow new calls if there is alarm call is active
   **  Reject the call with the default status of 'unavailable'.
   */
   if ( pUsr->bAlarmActive )
   {
      cmFsmCallReject ( pRes );
      /* Do not process any longer.
      */
      return;
   }
#endif
   /* Do not accept new incoming call if we are:
   **
   ** [1] Hotline is active, and is configured to not allow hookflash features.
   **
   ** [2] Cleaning up an existing call but have not yet terminated it entirely.
   **
   ** [3] We have disabled the call waiting and already have an active (focus)
   **     call ongoing.
   **
   */

   if ( pFocusSes )
   {
      /* Check hotline feature configuration if it's a hotline session.
      */
      if ( pFocusSes->ses.med.bHotline )
      {
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                          &cfgIdx,
                                          (void *) &pHotLineCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pHotLineCfg && pHotLineCfg->Act.bCfgStatus )
            {
               bHotlineNoCW = pHotLineCfg->bDisableHfFeat;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                  &cfgIdx,
                                  (void *) pHotLineCfg,
                                  CFG_ITEM_DEL );
            pHotLineCfg = NULL;
         }
      }

      /* Check call waiting feature status
      */
      bCallWait = cmFeatEnabled ( pRes, eCMDM_CWTOGGLE );

      /* See if any of the rejection conditions are met
      */
      if ( bHotlineNoCW ||
            ( bTmrRun ||
              ( !bTmrRun && (!bCallWait || (bCallWait && !pUsr->bPerCallCW) ) ) ) )
      {
         cmFsmCallRejectBusy ( pRes );
         /* Do not process any longer.
         */
         return;
      }
   }



   /* Do not accept new incoming call if we have set our **local** do-not-disturb
   ** status to 'activated'.  Reject the call with the default status of
   ** 'unavailable'.
   */
   if ( pUsr->bDnd )
   {
      cmFsmCallReject ( pRes );
      /* Do not process any longer.
      */
      return;
   }

   /* Sanity check to ensure we are not max'ed out in resources usage as well
   ** as we can still associate resources to this user.
   **
   ** [1] Cannot accept new call if one is already held (i.e. hold activate is
   **     enabled for this call).  This is because it would collide with call
   **     waiting feature usage.
   **
   ** [2] Cannot accept new call if one is already 'waiting'.
   **
   ** [3] Cannot accept new call if an auto-call is ongoing (auto recall or auto
   **     callback) because of resources limitations.
   **
   ** [4] Cannot accept new call if already engaged in a 3-way call.
   **
   ** [5] Cannot accept new call if already engaged in an emergency call and this
   **     new call is not an emergency call as well.
   **
   ** [6] Finally, cannot accept new call if we cannot allocate a session for
   **     it (obvious reason).
   */

   if (  ( pUsr->pSesHeld == NULL ) &&
         ( pUsr->pSesWait == NULL ) &&
         ( pUsr->pAutoCall == NULL ) &&
         ( !cmUsr3WayCall ( pUsr ) ) &&
         ( !cmUsrEmergCall ( pUsr ) || (cmUsrEmergCall ( pUsr ) && bEmergency) ) &&
         ( pSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl ) ) != NULL )
   {
      /* Associate this local resource with the network one.
      */
      pSes->uNet = pUsr->evt.uDat2; /* CallId handle */
      pSes->uNetUsr = pUsr->evt.uDat4; /* CCTK user handle */

      if ( cmUsrLinkSes ( pUsr, pSes ) )
      {
         bAccept = BOS_TRUE;

         /* Keep track of incoming emergency call.
         */
         pSes->ses.med.bEmergency = bEmergency;

         /* Keep track that this session was originated by the remote
         ** end as opposed to by the local user.
         */
         pSes->ses.med.bRemInit = BOS_TRUE;
      }
   }

   /* If we can accept this call, we also need to ensure we have physical
   ** resource to be able to have media working on it.
   */
   if ( bAccept && ( pUsr->uPhys == CMGR_INVALID_HANDLE ) )
   {
       bAccept = ( cmPhyAllocate ( pUsr ) == eCMGRSTATUS_COMMAND_SUCCESS );
   }

   /* Publish a 'line active' event as necessary.
   */
   if ( gCmCfgCb.cmgrEvtCb &&
        bAccept &&
        (pUsr->eType != eCMUSRTYPE_REMOTE) )
   {
      gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_ACTIVE,
                           pUsr->uPhys,
#if CMGRCFG_MULTILINE
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                           NULL );
   }
   /* Finally, if we are in a 'off-hook' situation on this user and the user
   ** is not engaged in a different call already, this could be a problem, likely
   ** an off-hook warning tone is being played anyways, reject the call (unless
   ** in special emergency case).
   */
   if ( pUsr->bOffHook &&
        ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_TALK ) )
   {
      bAccept = BOS_FALSE;

      /* If configured to configure to drop all calls and connect to incoming
      ** emergency calls we may be offhook when the emergency call is processed.
      ** If this is the case we will need to accept the call regardless.
      */
      if ( bEmergency )
      {
         CFGSIPEMERGE *pEmergCfg = NULL;
         PROV_CFG_IDX cfgIdx;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                    &cfgIdx,
                                    (void *) &pEmergCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pEmergCfg )
            {
               bAccept = pEmergCfg->bEndCallAcptIncEmerg;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                  &cfgIdx,
                                  (void *) pEmergCfg,
                                  CFG_ITEM_DEL );
            pEmergCfg = NULL;
         }
      }
   }

   /* Cannot accept the call for resources reason.
   */
   if ( !bAccept )
   {
      CMGRLOG_WARNING (( "cmFsmCallNew: Fail accept call on 0x%X (0x%X)",
                         pUsr->uHdl, pUsr->evt.uDat4 ));

#if CMGRCFG_MULTILINE
      /* If this is the last event and nobody could take the call */
      if( ( pUsr->evt.bLast ) &&
          ( cmNumSesWithCid( pUsr->evt.uDat2 ) == 1 ) )
      {
         if( ( pSes && pSes->uNetUsr != CMGR_INVALID_HANDLE ) ||
               pUsr->evt.uDat2 != CMGR_INVALID_HANDLE )
         {
            /* Release the network call.
            */
            cmFsmCallRejectBusy ( pRes );
         }
      }
      /* If this is an internal call, notify both parties to
      ** terminate the call.
      */
      else if( ( pUsr->evt.bLast ) &&
               ( cmNumSesWithCid( pUsr->evt.uDat2 ) == 2 ) &&
               ( pSes->uNetUsr == CMGR_INVALID_HANDLE ) )
      {
         CMSESLIST sesList;
         BOS_UINT32 uIx;

         memset( &sesList, 0, sizeof( CMSESLIST ) );

         cmSesListWithCid( pSes->uNet, &sesList );
         for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
         {
            CMSES* pSesRelease = cmSesValid( sesList.uSes[ uIx ] );
            CMUSR* pUsrRelease = (pSesRelease) ? cmUsrValid( pSesRelease->uUsr ) : NULL;

            if( ( pSesRelease ) && ( pUsrRelease ) )
            {
               CMEVT cmEvt;

               memset( &cmEvt, 0, sizeof( CMEVT ) );

               cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
               cmEvt.uDat1 = pSesRelease->uUsr;
               cmEvt.uDat2 = pSesRelease->uHdl;
               cmEvt.uDat4 = pSesRelease->uNetUsr;
               cmEvt.bLast = BOS_TRUE;

               if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
               {
                  cmSignalEvt ();
               }
            }
         }
      }
#else
      /* Release the network call.
      */
      cmFsmCallRejectBusy ( pRes );
#endif /* CMGRCFG_MULTILINE */

      if ( pSes )
      {
         cmUsrUnLinkSes ( pUsr, pSes );
         /* Reset the network handle and free the session resource.
         */
         pSes->uNet = CMGR_INVALID_HANDLE;
         cmSesFree ( pSes, pUsr->uHdl );
      }
   }
   else
   {
      pSes->uApp = pSes->uHdl;

      /* If the user was not in service, transition to the in-service
      ** state now.
      */
      if ( !pUsr->bInSrv )
      {
         CMGRLOG_INFORMATION ((
            "cmFsmCallNew: 0x%X (0x%X) transition to in-service",
            pUsr->uHdl, pSes->uNetUsr ));

#if !CMGRCFG_MULTILINE
         pUsr->bInSrv = BOS_TRUE;

         if ( gCmCfgCb.cmgrEvtCb )
         {
            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_OK,
                                     pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                     pUsr->uHdl,
                                     CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                     (void *) pUsr->uUid );
         }
#endif /* CMGRCFG_MULTILINE */
      }

      /* Update the media capabilities to be used for this call.
      */
      CMGRLOG_DEBUG (( "cmFsmCallNew: Accepting 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMGRCALLDATA tData;
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );

         tData.eType = eCMGRCALLTYPE_INCOMING;
         tData.uSrc = CMGR_INVALID_HANDLE;

         /* For the internal call case, find and fill in the source terminal.
         */
         if( pSes->uNetUsr == CMGR_INVALID_HANDLE )
         {
            CMSESLIST sesList;
            BOS_UINT32 uIx;

            memset( &sesList, 0, sizeof( CMSESLIST ) );

            cmSesListWithCid( pSes->uNet, &sesList );
            for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
            {
               CMSES* pSesSrc = cmSesValid( sesList.uSes[ uIx ] );

               if( ( pSesSrc ) && ( pSesSrc->bEstab == BOS_TRUE ) )
               {
                  tData.uSrc = pSesSrc->uUsr;
                  break;
               }
            }
         }

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_NEW,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pUsr->uHdl,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) &tData );
      }

      /* If this is the only call on the system and we are otherwise idle,
      ** transition to a state where can handle race condition with user
      ** action (hook event).
      */
      if ( !pUsr->bOffHook &&
           ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_IDLE ) )
      {
         cmUsrNewState ( pUsr, eCMST_ACCEPTED );
      }

#if CMGRCFG_REMOTE_USER_CONTROL
      /*
      ** Notify associated linked (remote users) in cmFsmCallAlert before ringing they phy
      */
      if ( pUsr && pUsr->uLinkedRemoteUserCnt )
      {
         bProxy = BOS_TRUE;  /* no early media when there is remote user */
      }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

      if ( pUsr->eType != eCMUSRTYPE_REMOTE &&
           pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
#if CMGRCFG_MULTILINE
         /* for multiline, only initialize the call media and send the
         ** notify if this is the only session associated with the call-id
         */
         if (cmNumSesWithCid(pSes->uNet) > 1)
         {
            pSes->ses.med.bNeedFinal = BOS_TRUE;
            /* do not init call media again */
            return;
         }
#endif
         cmCfgInitCallMedia ( pRes, pSes );

         /* Finalize capability with local user capability since there is no remote user */
         if ( !bProxy )
         {
#if !CMGRCFG_MULTILINE
            cctkCallMediaNotify( pSes->uNet, eCCTKMUDPRSN_FINALIZECAPS );
#else
            /* We cannot enable early media when multiline is enabled. If
            ** we have to ring multiple handsets, we can't setup a media
            ** session until one of the handsets answers the call.
            */
            cctkCallMediaNotify( pSes->uNet, eCCTKMUDPRSN_NOCAPS );
            pSes->ses.med.bNeedFinal = BOS_TRUE;
#endif
         }
         else
         {
            cctkCallMediaNotify( pSes->uNet, eCCTKMUDPRSN_NOCAPS );
            pSes->ses.med.bNeedFinal = BOS_TRUE;
         }
      }
      else
      {
         /* Incoming Remote user call to the outside world */
         /* call cctkCallMediaNotify .. so it will send the 180 and alert the associated local user */
         cctkCallMediaNotify( pSes->uNet, eCCTKMUDPRSN_NOCAPS );
         pSes->ses.med.bNeedFinal = BOS_TRUE;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallReject
**
** PURPOSE:    FSM reject incoming call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallReject ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

#if CMGRCFG_MULTILINE
   /* If this call is assigned to another user */
   if ( ( cmNumCidAssigned( pUsr->evt.uDat2 ) == 1 ) &&
        ( cmUsrFindCid( pUsr->evt.uDat2, BOS_TRUE ) != pUsr ) &&
        ( pUsr->evt.uDat4 != CMGR_INVALID_HANDLE ) )
   {
      /* Don't process anything */
      return;
   }
#endif

   /* If this is the last event and nobody could take the call */
   if( ( pUsr->evt.bLast ) &&
       ( cmNumSesWithCid( pUsr->evt.uDat2 ) == 0 ) &&
       ( pUsr->evt.uDat4 != CMGR_INVALID_HANDLE ) )
   {
       CMGRLOG_WARNING (( "cmFsmCallReject: 0x%X on 0x%X (0x%X)",
                         pUsr->evt.uDat2, pUsr->uHdl, pUsr->evt.uDat4 ));

      /* Instead of strictly rejecting the call which cannot be handled, we could
      ** do some forwarding to voice-mail, or third party at this time.
      */

      /*
      ** TO DO: Handle call rejection more gracefully if appropriate.
      **
      ** CCTK-API: cctkCallForward( BOS_UINT32 uCid, const BOS_UINT8 *pcFwdTo );
      */

      /* Reject the network call.
      **
      ** Note that the network handle is carried within the event information because
      ** there is no session resource that we have associated with this incoming call
      ** from the network, so there is no media session reference from the user block.
      */
      cctkCallDelete ( pUsr->evt.uDat2, eCCTKCALLDELRSN_UNAVAILABLE );
   }
   /* If this is the last event for an internal call and nobody could take the call */
   else if( ( pUsr->evt.bLast ) &&
            ( cmNumSesWithCid( pUsr->evt.uDat2 ) == 1 ) &&
            ( pUsr->evt.uDat4 == CMGR_INVALID_HANDLE ) )
   {
      CMUSR* pOtherUsr = cmUsrFindCid( pUsr->evt.uDat2, BOS_TRUE );
      CMSES* pOtherSes = cmSesFindNetHdl( pUsr->evt.uDat2, (pOtherUsr) ? pOtherUsr->uHdl : CMGR_INVALID_HANDLE );

      if( pOtherUsr && pOtherSes )
      {
         CMEVT cmEvt;

         memset( &cmEvt, 0, sizeof( CMEVT ) );

         cmEvt.uEvt = ( eCMEVT_BUSY_CALL | CMGR_EVT_CLASS__CCTK );
         cmEvt.uDat1 = pOtherUsr->uHdl;      /* User handle */
         cmEvt.uDat2 = pOtherSes->uHdl;      /* Session handle */
         cmEvt.uDat4 = CMGR_INVALID_HANDLE;  /* Network handle */
         cmEvt.bLast = BOS_TRUE;

         if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }
      }
      else
      {
         CMGRLOG_CRITICAL(( "%s: cmNumSesWithCid( %X ) == 1, but no other user has this cid!",
                            __FUNCTION__,
                            pUsr->evt.uDat2 ));
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallRejectAnonymous
**
** PURPOSE:    FSM reject incoming anonymous call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRejectAnonymous ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES * pSes = cmSesValid ( pUsr->evt.uDat2 );

   CMGRLOG_WARNING (( "cmFsmCallRejectAnonymous: Cid 0x%X on Usr 0x%X uNet (0x%X)",
                      ((pSes) ? pSes->uNet: pUsr->evt.uDat2), pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   /* Reject the network call.
   */

   if( pSes )
   {
      /* Multiline case
      ** Note that the session handle is carried within the event information because
      ** under multiline we always allocate sessions right away
      */
      cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_ANONYMOUS );
   }
   else
   {
      /* Default Case
      ** Note that the network handle is carried within the event information because
      ** there is no session resource that we have associated with this incoming call
      ** from the network, so there is no media session reference from the user block.
      */
      cctkCallDelete ( pUsr->evt.uDat2, eCCTKCALLDELRSN_ANONYMOUS );
   }

}


/***********************************************************************************
** FUNCTION:   cmFsmCallRejectBusy
**
** PURPOSE:    FSM reject busy incoming call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRejectBusy ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_BOOL bFwd = BOS_FALSE;
   CFGSIPCALLFWD *pFwdCfg;
   CFGSIPCALLFWDBUSY *pFwdBusyCfg;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bInternalCall = BOS_FALSE;

#if CMGRCFG_MULTILINE
   /* If this call is assigned to another user */
   if ( ( cmNumCidAssigned( pUsr->evt.uDat2 ) == 1 ) &&
        ( cmUsrFindCid( pUsr->evt.uDat2, BOS_TRUE ) != pUsr ) &&
        ( pUsr->evt.uDat4 != CMGR_INVALID_HANDLE ) )
   {
      /* Don't process anything */
      return;
   }
#endif

   /* Only reject if this is the last user to receive the event. */
   if( pUsr->evt.bLast )
   {
      bInternalCall = ( pUsr->evt.uDat4 == CMGR_INVALID_HANDLE ) ? BOS_TRUE : BOS_FALSE;

      /* If NOT an internal call, we can reject if:
      ** a) This call was allocated a session that belongs to this user - the owner is rejecting.
      ** b) There are no sessions belonging to this call - nobody could take the call.
      */
      if( !bInternalCall &&
          ( ( pUsr == cmUsrFindCid( pUsr->evt.uDat2, BOS_FALSE ) ) ||
               ( cmNumSesWithCid( pUsr->evt.uDat2 ) == 0 ) ) )
      {
         CMGRLOG_WARNING (( "cmFsmCallRejectBusy: 0x%X on 0x%X (0x%X)",
                               pUsr->evt.uDat2, pUsr->uHdl, pUsr->evt.uDat4 ));

         /* Reject the network call.
         **
         ** Note that the network handle is carried within the event information because
         ** there is no session resource that we have associated with this incoming call
         ** from the network, so there is no media session reference from the user block.
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;

         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdBusyConcept ),
                                    &cfgIdx,
                                    (void *) &pFwdBusyCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pFwdBusyCfg &&
                 pFwdBusyCfg->Act.bCfgStatus )
            {
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                          &cfgIdx,
                                          (void *) &pFwdCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pFwdCfg &&
                       pFwdCfg->bUsrSet )
                  {
                     bFwd = BOS_TRUE;

                     CMGRLOG_WARNING ((
                        "cmFsmCallRejectBusy: forward 0x%X on 0x%X (0x%X) to \'%s\'",
                        pUsr->evt.uDat2, pUsr->uHdl, pUsr->uNet,
                        ( pFwdCfg->bUseFwdDString ?
                        ( pFwdCfg->pcFwdToDString ? pFwdCfg->pcFwdToDString : "NULL" ) :
                        ( pFwdCfg->pcFwdToUri ? pFwdCfg->pcFwdToUri : "NULL" ))
                        ));

                     if ( !(pFwdCfg->bUseFwdDString) && (pFwdCfg->pcFwdToUri != NULL) )
                     {
                        cctkCallForward( pUsr->evt.uDat2,
                                         pFwdCfg->pcFwdToUri );
                     }
                     else
                     {
                        if ( (pFwdCfg->bUseFwdDString) && (pFwdCfg->pcFwdToDString != NULL) )
                        {
                           cmFsmCallForwardViaDialString( pUsr, pUsr->evt.uDat2,pFwdCfg->pcFwdToDString);
                        }
                     }
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                        &cfgIdx,
                                        (void *) pFwdCfg,
                                        CFG_ITEM_DEL );
                  pFwdCfg = NULL;
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdBusyConcept ),
                                  &cfgIdx,
                                  (void *) pFwdBusyCfg,
                                  CFG_ITEM_DEL );
            pFwdBusyCfg = NULL;
         }

         if ( !bFwd )
         {
            /* Reject the network call.
            **
            ** Note that the network handle is carried within the event information because
            ** there is no session resource that we have associated with this incoming call
            ** from the network, so there is no media session reference from the user block.
            */
            cctkCallDelete ( pUsr->evt.uDat2, eCCTKCALLDELRSN_BUSY );
         }

      }
      /* This is the last event for an internal call and nobody could take the call */
      else if( bInternalCall &&
               ( cmNumSesWithCid( pUsr->evt.uDat2 ) == 1 ) )
      {
         CMUSR* pOtherUsr = cmUsrFindCid( pUsr->evt.uDat2, BOS_TRUE );
         CMSES* pOtherSes = cmSesFindNetHdl( pUsr->evt.uDat2, (pOtherUsr) ? pOtherUsr->uHdl : CMGR_INVALID_HANDLE );

         if( pOtherUsr && pOtherSes )
         {
            CMEVT cmEvt;

            memset( &cmEvt, 0, sizeof( CMEVT ) );

            cmEvt.uEvt = ( eCMEVT_BUSY_CALL | CMGR_EVT_CLASS__CCTK );
            cmEvt.uDat1 = pOtherUsr->uHdl;      /* User handle */
            cmEvt.uDat2 = pOtherSes->uHdl;      /* Session handle */
            cmEvt.uDat4 = CMGR_INVALID_HANDLE;  /* Network handle */
            cmEvt.bLast = BOS_TRUE;

            if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
            {
               cmSignalEvt ();
            }
         }
         else
         {
            CMGRLOG_CRITICAL(( "%s: cmNumSesWithCid( %X ) == 1, but no other user has this cid!",
                               __FUNCTION__,
                               pUsr->evt.uDat2 ));
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallEnded
**
** PURPOSE:    FSM remote terminated call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallEnded ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMGREPSIG sig;
   CMCCTKACCNT* pCctkAccnt = NULL;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmCallEnded: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

#if CMGRCFG_MULTILINE
      pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

      if ( pSes->bEstab &&
           pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
         CCTKCALLMETRICS stats;
         BOS_UINT8 cPubTo[CMGR_IP_ADDRESS_STRING_LEN+1];
         PROV_CFG_IDX cfgIdx;
         CFGEPTCODECCFG *pCodecCfg = NULL;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cPubTo[0] = '\0';
         cfgIdx.uIndex = pUsr->uPhys;
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                    &cfgIdx,
                                    (void *) &pCodecCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pCodecCfg )
            {
               if (pCodecCfg->pcPubReportAddr)
               {
                  strcpy((char *)cPubTo, (char *)pCodecCfg->pcPubReportAddr);
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                  &cfgIdx,
                                  (void *) pCodecCfg,
                                  CFG_ITEM_DEL );
            pCodecCfg = NULL;
         }

         if ( strlen((char *)cPubTo) )
         {
            BOS_UINT8 ipAddr[CMGR_IP_ADDRESS_STRING_LEN + 7];
            BOS_IP_ADDRESS pubTo;
            BOS_STATUS status;

            status = bosIpAddrCreateFromStr( (char *)cPubTo, &pubTo);
            if ( (status == BOS_STATUS_OK) && bosIpAddrIsV6( &pubTo ) )
            {
               sprintf ( (char *)ipAddr, "sip:[" );
            }
            else
            {
               sprintf ( (char *)ipAddr, "sip:" );
            }

            strcpy((char *)&ipAddr[strlen( (const char *) ipAddr )], (char *)cPubTo);

            if ( (status == BOS_STATUS_OK) && bosIpAddrIsV6( &pubTo ) )
            {
                strcat ( (char *)ipAddr, "]" );
            }

            /* Gather and publish the statistic information for this call.
            */
            cmSesGetStats ( pSes, (void *) &stats );
            stats.uUserGuid = pSes->uNetUsr;
            cctkSetParm ( pSes->uNet,
                          eCCTKPARM_EC_CALLSTATS,
                          (void *) &stats );

            cctkPublish ( stats.uUserGuid,
                          pSes->uNet,
                          eCCTKPUBLISH_CALLMETRICS,
                          ipAddr );
         }
      }

      if( cmNumSesWithCid( pSes->uNet ) == 1 )
      {
         if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            /* Terminate the network resource.
            */
            cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
         }
         else
         {
            /* Send a release event */
            CMEVT evt;

            memset ( &evt, 0, sizeof( CMEVT ) );

            evt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
            evt.uDat1 = pSes->uUsr;          /* User handle */
            evt.uDat2 = pSes->uHdl;          /* Session handle */
            evt.uDat4 = CMGR_INVALID_HANDLE; /* CCTK user id */
            evt.bLast = BOS_TRUE;            /* Last event */

            if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
            {
               cmSignalEvt ();
            }
         }
      }

      /* If this is the primary focus call, we need to inform the user that
      ** it is terminated now, user may choose to hang up, or swap to a
      ** different call (via hook flash or hook flash + feature code).
      */
      if ( pSes->ses.med.bFocus )
      {
         /* If we are not already playing busy tone.
         */
         if ( pUsr->uCpt != eEPTSIG_BUSY )
         {
            /* Play some re-order tone such that local user hangs up.
            */
            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
            sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = eEPTSIG_REORDER;
            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );
         }
      }

      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_ENDED,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) pUsr->uUid );
      }

      cmUsrNewState ( pUsr, eCMST_RELEASING );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallConnect
**
** PURPOSE:    FSM remote connected call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallConnect ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bClidName = BOS_TRUE;
   BOS_BOOL bClidNumber = BOS_TRUE;
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   int uIx;
   CMSES *pOtherSes = NULL;

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmCallConnect: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

#if CMGRCFG_MULTILINE
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

      /* Callee identification update (if needed).
      */
      if ( pUsr->evt.pDat3 )
      {
         memcpy ( &pSes->ses.med.callInfo,
                  pUsr->evt.pDat3,
                  sizeof ( CCTKCALLINFO ) );
      }

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                 &cfgIdx,
                                 (void *) &pUsrClidDsp,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pUsrClidDsp != NULL )
         {
            bClidNumber = pUsrClidDsp->bCNDActStat;
            bClidName = pUsrClidDsp->bCNAMDActStat;

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                  &cfgIdx,
                                  (void *) pUsrClidDsp,
                                  CFG_ITEM_DEL );
            pUsrClidDsp = NULL;
         }
      }

#if !CMGRCFG_MULTILINE
      /* Issue callee identification information if necessary.
      */
      if ( pUsr->evt.pDat3 && (bClidName || bClidNumber) )
      {
         cmSesClid ( pSes, bClidName, bClidNumber );
      }
#endif /* CMGRCFG_MULTILINE */

      /* Terminate temporary monitoring of the media activity for this
      ** connection.
      */
      if ( !pSes->ses.med.bBgFocus )
      {
         cmSesPhyActMonThresh ( pSes, 0 );
      }

      if ( pSes->ses.med.bFocus ||
           (pSes->ses.med.uConf != CMGR_INVALID_HANDLE && !pSes->ses.med.bBgFocus) )
      {
         /* Terminate the ring-back tone.
         */
         CMGREPSIG sig;

         CMTMR tmr;

         memset ( &sig, 0, sizeof ( CMGREPSIG ) );
         memset ( &tmr, 0, sizeof ( CMTMR ) );

         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGBACK */
         pUsr->uCpt = eEPTSIG_NULL;

         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_RINGBACK;

         cmTmrStop ( &tmr );

         cmPhySignal ( pUsr->uPhys, &sig );

         /* If we are setting up a connection with the conference server and it was successful,
         **  move on to the next stages of the server conference call setup
         */
         if (pUsr->pConfBridgeStatus->eRequestStatus == CMCONF_STATUS_INPROGRESS)
         {
            pUsr->pConfBridgeStatus->eRequestStatus = CMCONF_STATUS_COMPLETING;

            /* If we are here, there are only two conferencing options: referring the server
            **  to participants, or the other way around
            */
            if ( pUsr->pConfBridgeStatus->eConfOption == CFG_SIP_CONFERENCING_REFER_PARTICIPANTS)
            {
               CMGRLOG_INFORMATION (( "%s: transferring to conference server %s\n",
                                      __FUNCTION__,  pUsr->pConfBridgeStatus->cConfUri));

               /* Refer participants to the conferencing server */
               cctkCallTransferBlind( pUsr->pConfBridgeStatus->pHeldSes[0]->uNet,
                                      (const BOS_UINT8 *) (pUsr->pConfBridgeStatus->cConfUri) );
               cctkCallTransferBlind( pUsr->pConfBridgeStatus->pHeldSes[1]->uNet,
                                      (const BOS_UINT8 *) (pUsr->pConfBridgeStatus->cConfUri) );
               pUsr->pConfBridgeStatus->eRequestStatus = CMCONF_STATUS_IDLE;
            }
            else
            {
               /* Refer server to participants */
               cctkCallTransfer(pUsr->pConfBridgeStatus->pServerSes->uNet, pUsr->pConfBridgeStatus->pHeldSes[0]->uNet, BOS_TRUE);
               /* if transfer is successful, another one will be initiated */
            }
         }

         /* If we are in a 3way call, terminate any ringback tone that is being
         ** generated to the peer
         */
         if ( cmUsr3WayCall ( pUsr ) )
         {
            for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
            {
               if ( pUsr->pMedSes [ uIx ] )
               {
                  if ( pUsr->pMedSes [ uIx ]->uHdl == pSes->ses.med.uConf )
                  {
                     pOtherSes = pUsr->pMedSes [ uIx ];
                     break;
                  }
               }
            }

            if ( pOtherSes && ( pOtherSes->ses.med.uPhy != CMGR_INVALID_HANDLE ) )
            {
               sig.uSes = pOtherSes->ses.med.uPhy;
               sig.uSig = eEPTSIG_RINGBACK;
               cmPhySignal ( pUsr->uPhys, &sig );
            }
         }
      }
      /* If this is an internal call, we need to create the physical
      ** session information here.
      */
      if( ( pSes->ses.med.uPhy == CMGR_INVALID_HANDLE ) &&
          ( pSes->uNetUsr == CMGR_INVALID_HANDLE ) )
      {
         cmSesPhyCreate( pSes );
      }

      if( !pSes->bEstab )
      {
         pSes->bEstab = BOS_TRUE;
#if CMGRCFG_MULTILINE
         if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            cmCctkAccntCallAdd( pSes->uNetUsr );
         }
#endif
      }

      /* If this session was setup to be used for an 'anonymous' auto-call service
      ** this is the time where we have implicit confirmation that something has
      ** happened in the network and that our feature should be ongoing properly.
      **
      ** We need to keep track of this as we will need it for feature deactivation.
      */
      if ( pSes->ses.med.pAutoCall &&
           ( (CMAUTOCALL *) pSes->ses.med.pAutoCall )->bUsed &&
           ( (CMAUTOCALL *) pSes->ses.med.pAutoCall )->bAnonymous )
      {
         CMSES *pSrvSes;

         if (  ( pSrvSes = cmSesAlloc ( eCMSESTYPE_SRV, pUsr->uHdl ) ) != NULL )
         {
            if ( cmUsrLinkSes ( pUsr, pSrvSes ) )
            {
               BOS_UINT32 uTimeOut = 0;
               CFGSIPAUTOCALL *pACall;
               PROV_CFG_IDX cfgIdx;
               CMTMR tmr;

               memset ( (void *) &tmr,
                        0,
                        sizeof ( CMTMR ) );

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( pSes->ses.med.eAction == eCMDM_ACACTIVATE )
               {
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                             &cfgIdx,
                                             (void *) &pACall,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pACall )
                     {
                        uTimeOut = pACall->uTimer;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                           &cfgIdx,
                                           (void *) pACall,
                                           CFG_ITEM_DEL );
                     pACall = NULL;
                  }
                  else
                  {
                     uTimeOut = CMGR_AUTO_CALL_TIMEOUT;
                  }
               }
               else if ( pSes->ses.med.eAction == eCMDM_ARACTIVATE )
               {
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                             &cfgIdx,
                                             (void *) &pACall,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pACall )
                     {
                        uTimeOut = pACall->uTimer;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                           &cfgIdx,
                                           (void *) pACall,
                                           CFG_ITEM_DEL );
                     pACall = NULL;
                  }
                  else
                  {
                     uTimeOut = CMGR_AUTO_CALL_TIMEOUT;
                  }
               }

               tmr.uSes = pSrvSes->uHdl;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType =
                  ( ( pSes->ses.med.eAction == eCMDM_ACACTIVATE ) ?
                           eCMEVT_TMR_AUTOCALLBACK : eCMEVT_TMR_AUTORECALL );
               tmr.uTimeOut = uTimeOut;

               cmTmrStart ( &tmr );

               pSrvSes->ses.srv.pAutoCall = pSes->ses.med.pAutoCall;
            }
         }
      }

      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) pUsr->uUid );
      }

#if CMGRCFG_REMOTE_USER_CONTROL
      /* If this is a remote user that connected, it means the call
      ** we were receiving on the local user is now connected via a
      ** remote user.
      */
      if ( (pUsr->eType == eCMUSRTYPE_REMOTE) &&
           (pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGBACK) )
      {
         /* Get the linked local user.
         */
         CMUSR *pLocalUser = cmUsrValid( pUsr->uLinkedLocalUserHdl );

         /* Cancel call leg towards possible 'other' remote users.
         */
         cmFeatProxyCalleeEnd ( pUsr );

         /* Cancel call ring and presentation on local user.
         */
         if ( pLocalUser )
         {
            cmFeatProxyMove2Bg ( pLocalUser,
                                 pSes->ses.med.pLinkedSes );
         }

         cmUsrNewState ( pUsr, eCMST_TALK );
      }
      else
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
      if ( !pSes->ses.med.bBgFocus )
      {
         cmUsrNewState ( pUsr, eCMST_TALK );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallRingback
**
** PURPOSE:    FSM remote ringing call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRingback ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bPresetMedSes = BOS_FALSE;

   if ( pSes )
   {
      CMTMR tmr;

      memset ( &tmr, 0, sizeof ( CMTMR ) );

      CMGRLOG_DEBUG (( "cmFsmCallRingback: 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

#if CMGRCFG_REMOTE_USER_CONTROL
      /* Ignore this action if the session is a background one.
      */
      if ( pSes->ses.med.bBgFocus )
      {
         cmFsmNoOp( pRes );
         return;
      }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

      if ( pSes->ses.med.bFocus && !pSes->ses.med.bMediaRx )
      {
         /* Start the ring-back tone.
         */
         CMGREPSIG sig;
#if CMGRCFG_MULTILINE
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */


         memset ( &sig, 0, sizeof ( CMGREPSIG ) );

         sig.eAct = eCMGREPSIGACT_START;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = pSes->uApp;
         sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = eEPTSIG_RINGBACK;
         pUsr->uCpt = sig.uSig;

         cmPhySignal ( pUsr->uPhys, &sig );
      }

      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_RINGBACK;
      tmr.uTimeOut = CMGR_RINGBACK_TONE_TIMEOUT;

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pBasicCall )
         {
            tmr.uTimeOut = pBasicCall->uOrigRBKTimer;
            bPresetMedSes = pBasicCall->bPresetMedSes;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }

      cmTmrStart ( &tmr );

      /* Inform application that the call is ringing remotely.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_RINGBACK,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  NULL );
      }

      /* If we are configured to preset the media session, do it now, it must
      ** be done before the state change.  Note that if we do receive early
      ** media information from the far end, the session will be updated
      ** with the early media data subsequently.
      **
      ** Preset of the media session is useful to reduce post pickup delay
      ** on call answer in the case where it is known that no early media is
      ** ever established otherwise.
      */
      if ( bPresetMedSes )
      {
         /* cmFsmMedia ( pRes ); */
         pSes->ses.med.bFastTrac = BOS_TRUE;
      }

      cmUsrNewState ( pUsr, eCMST_RINGBACK );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmMedia
**
** PURPOSE:    FSM  media information.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This handler is invoked when 'negotiated' media information is
**             present for a session.  Negotiated media information means that we
**             already have determined the end result of the actual media for this
**             session, therefore it is in fact straightforward to manage the
**             event.
***********************************************************************************/
void cmFsmMedia ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CCTKCALLINFO *pCall = NULL;
   int uIx;
   CMSES *pOtherSes = NULL;

#if CMGRCFG_REMOTE_USER_CONTROL
   /* For the time being, ignore any media information that is targetted to
   ** either:
   **
   **    - any remote user.
   **    - any background session.
   **
   ** In future, this media information can be used for purposes of making
   ** the application massage and redirect media session as desired.
   */
   if ( (pSes && (pSes->ses.med.bBgFocus)) ||
        (pUsr && (pUsr->eType == eCMUSRTYPE_REMOTE)) )
   {
      CMGRLOG_DEBUG (( "cmFsmMedia: 0x%X (0x%X) - ignoring",
                       pUsr->uHdl, pUsr->evt.uDat4 ));
      cmFsmNoOp( pRes );
      return;
   }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

   /* If we are ringing, this could be for a new incoming call, in which case
   ** the session is not set as part of the event, but it should have been created
   ** and should be the focus of the user.
   */
   if ( ( pSes == NULL ) &&
        ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGING ) )
   {
      pSes = cmUsrFocusSes( pUsr );
   }

   if ( pSes )
   {
      CMGRLOG_DEBUG (( "cmFsmMedia: 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* Mark whether the session needs to apply keep alive monitoring
      ** on the media (i.e. check for media activity as a mean of keeping
      ** alive the session), we may use other means (including signaling)
      ** to achieve the same thing, so it is not necessary to apply such
      ** media keep alive monitoring.
      */
      pSes->ses.med.bMediaKAM = BOS_FALSE;
      if ( pUsr->evt.pDat3 )
      {
         pCall = (CCTKCALLINFO *)pUsr->evt.pDat3;

         if ( pCall->bMedKeepAlive )
         {
            pSes->ses.med.bMediaKAM = BOS_TRUE;
         }
      }

      /* We do not have a physical session created yet, we need to do so now.
      */
      if ( pSes->ses.med.uPhy == CMGR_INVALID_HANDLE )
      {
         CMGREPSIG sig;
#if CMGRCFG_MULTILINE
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

         CMTMR tmr;
         BOS_BOOL bCreateSession = BOS_TRUE;

         memset ( &sig, 0, sizeof ( CMGREPSIG ) );
         memset ( &tmr, 0, sizeof ( CMTMR ) );

         /* If we are in the call establishment state, defer the physical session
         ** creation if early media is not enabled.  Ingress media will not
         ** begin until the user is ready to answer the call.
         */
         if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGING )
         {
            PROV_CFG_IDX      cfgIdx;
            CFGPROVDEVICECFG *pDevCfg = NULL;

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                    &cfgIdx,
                                    (void *) &pDevCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pDevCfg && !pDevCfg->bEarlyMedia )
               {
                  bCreateSession = BOS_FALSE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                         &cfgIdx,
                         (void *) pDevCfg,
                         CFG_ITEM_DEL );
            }
         }

         if ( bCreateSession )
         {
            cmSesPhyCreate ( pSes );

            if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGBACK )
            {
               /* Stop the ring-back tone as we should get in-band tone
               ** instead at this point (data path is created).
               */
               tmr.uSes = pSes->uHdl;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_RINGBACK;

               cmTmrStop ( &tmr );

               sig.eAct = eCMGREPSIGACT_STOP;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = pSes->uApp;
               sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
               sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
               sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGBACK */
               pUsr->uCpt = eEPTSIG_NULL;

               cmPhySignal ( pUsr->uPhys, &sig );
            }
            else if ( ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGING ) ||
                      ( ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_TALK ) &&
                                            ( pUsr->pSesWait == pSes ) ) )
            {
               /* Play ringback into the network for the remote end to hear.
               */
               sig.eAct = eCMGREPSIGACT_START;
               sig.uSes = pSes->ses.med.uPhy;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
               sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
               sig.uSig = eEPTSIG_RINGBACK;

               cmPhySignal ( pUsr->uPhys, &sig );
            }
         }
      }
      /* This is an update to an existing physical session information.
      */
      else
      {
         BOS_BOOL bUpdate = BOS_TRUE;
         CCTKMEDINFO media;

         if ( pCall )
         {
            /* We may be asked to play some howler tone if this is for
            ** an emergency service.
            */
            if ( pCall->bEmergency )
            {
               if ( strlen( (const char *) pCall->cAlertInfo[ 0 ] ) &&
                    ( cmPhyToneFromString ( pCall->cAlertInfo[ 0 ] )
                                                         == eEPTSIG_OHWARN ) )
               {
                  CMGREPSIG sig;
#if CMGRCFG_MULTILINE
                  CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

                  CMTMR tmr;
                  CFGSIPEMERGE *pEmergencyCfg;
                  PROV_CFG_IDX cfgIdx;

                  memset ( &sig, 0, sizeof ( CMGREPSIG ) );
                  memset ( &tmr, 0, sizeof ( CMTMR ) );

                  CMGRLOG_INFORMATION ((
                              "cmFsmMedia: Emergency howler for 0x%X (0x%X)",
                              pUsr->uHdl, pSes->uNetUsr ));

                  sig.eAct = eCMGREPSIGACT_START;
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
                  sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                  sig.uSig = eEPTSIG_OHWARN;
                  pUsr->uCpt = sig.uSig;

                  cmPhySignal ( pUsr->uPhys, &sig );

                  tmr.uSes = CMGR_INVALID_HANDLE;
                  tmr.uUsr = pUsr->uHdl;

                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = pUsr->uUid;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                             &cfgIdx,
                                             (void *) &pEmergencyCfg,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pEmergencyCfg && pEmergencyCfg->uHowlTimer )
                     {
                        tmr.uTimeOut = pEmergencyCfg->uHowlTimer;
                        tmr.eType = eCMEVT_TMR_EMERG_HOWLER;

                        cmTmrStart ( &tmr );
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                           &cfgIdx,
                                           (void *) pEmergencyCfg,
                                           CFG_ITEM_DEL );
                     pEmergencyCfg = NULL;
                  }
               }
            }
         }

         if ( ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) &&
              ( cctkGetParm ( pSes->uNet, eCCTKPARM_MEDINFO, (void *)&media ) ==
                   eCCTKSTATUS_SUCCESS ) )
         {
            /* Special processing when going into fax relay mode.  For that to be
            ** true, the port number for T.38 media must have been set.
            */
            if ( media.ingress.t38.uPort )
            {
               /* If this was an attempt to switch to fax relay mode.  We are
               ** successful.
               */
               if ( pSes->ses.med.faxRelay.bAttempt )
               {
                  pSes->ses.med.faxRelay.bAttempt = BOS_FALSE;
                  pSes->ses.med.faxRelay.bOngoing = BOS_TRUE;
                  CMGRLOG_INFORMATION ((
                                   "cmFsmMedia: switching to fax on 0x%X (0x%X)",
                                   pSes->uHdl, pSes->uNet ));
               }
               /* Otherwise, the remote end switched us into this mode first.
               */
               else
               {
                  pSes->ses.med.faxRelay.bOngoing = BOS_TRUE;
                  CMGRLOG_INFORMATION ((
                                   "cmFsmMedia: switched to fax on 0x%X (0x%X)",
                                   pSes->uHdl, pSes->uNet ));
               }
            }
            /* If we are in a fax relay session already and this would take us
            ** out of the fax relay session but this latter has not been confirmed
            ** terminated by our side, do not process the media information just
            ** yet.  Instead wait until we are made aware of the proper termination.
            ** This prevents race condition issues that could lead to some fax
            ** machine reporting transmission error when none occured due to the
            ** possible loss of the termination handshake on early switching out of
            ** fax relay mode.
            */
            else if ( pSes->ses.med.faxRelay.bOngoing )
            {
               pSes->ses.med.faxRelay.bDeferEnd = BOS_TRUE;
               bUpdate = BOS_FALSE;
               CMGRLOG_INFORMATION ((
                               "cmFsmMedia: defer fax end on 0x%X (0x%X)",
                               pSes->uHdl, pSes->uNet ));
            }
         }

         if ( bUpdate )
         {
            CCTKCALLMETRICS stats;
            /* Request stats update before switching to T38 */
            cmSesGetStats ( pSes, (void *) &stats );
            cmSesPhyUpdate ( pSes, BOS_FALSE );
         }

         /* If the session is in auto-recover state and we now need to play
         ** local feedback to the remote end as well, apply it.
         */
         if ( ( pSes->ses.med.eAutoRec == eCMSESAUTOREC_TPF ) &&
              ( pUsr->uCpt != eEPTSIG_NULL ) )
         {
            CMGREPSIG sig;
#if CMGRCFG_MULTILINE
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

            memset ( &sig, 0, sizeof ( CMGREPSIG ) );
            /* Play the local tone into the network for the remote end to hear.
            */
            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = pSes->ses.med.uPhy;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
            sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = pUsr->uCpt;

            cmPhySignal ( pUsr->uPhys, &sig );
         }

         /* If we have a 3way call, check if we need to apply ringback tone
         ** to the first participant.  This is required for early conference
         ** (i.e. last participant did not answer yet and the controller
         ** has hook flashed)
         */
         if ( cmUsr3WayCall ( pUsr ) )
         {
            for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
            {
               if ( pUsr->pMedSes [ uIx ] )
               {
                  if ( pUsr->pMedSes [ uIx ]->uHdl == pSes->ses.med.uConf )
                  {
                     pOtherSes = pUsr->pMedSes [ uIx ];
                     break;
                  }
               }
            }

            /* Early conference is denoted by having a session that has not
            ** been established yet.
            **
            ** The only time that we do not need to apply ringback tone to
            ** the first participant is if the last participant is already
            ** generating ringback on a connection and we accept this
            ** (due to the presence of a remote SDP).  For this case, the
            ** controller is not required to generate ringback to the first
            ** participant.
            */
            if ( pOtherSes && ( !pOtherSes->bEstab ) &&
                ( pUsr->uCpt == eEPTSIG_RINGBACK ) )
            {
               CMGREPSIG sig;
               memset ( &sig, 0, sizeof ( CMGREPSIG ) );

               for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
               {
                  if ( pUsr->pMedSes [ uIx ] )
                  {
                     if ( pUsr->pMedSes [ uIx ]->uHdl != pSes->ses.med.uConf )
                     {
                        /* Since the controller is generating ringback locally,
                        ** we need to generate ringback to the first participant
                        */
#if CMGRCFG_MULTILINE
                        CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pUsr->pMedSes [ uIx ]->uNetUsr );
#endif /* CMGRCFG_MULTILINE */
                        sig.eAct = eCMGREPSIGACT_START;
                        sig.uSes = pUsr->pMedSes [ uIx ]->ses.med.uPhy;
                        sig.uCid = pUsr->pMedSes [ uIx ]->uNet;
#if CMGRCFG_MULTILINE
                        sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                        sig.uApp = CMGR_INVALID_HANDLE;
                        sig.uSig = eEPTSIG_RINGBACK;

                        cmPhySignal ( pUsr->uPhys, &sig );
                     }
                  }
               }
            }
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmMediaFailed
**
** PURPOSE:    FSM media failure (due to failed service application or media
**             re-negotiation failure of some sort).
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmMediaFail ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_DEBUG (( "cmFsmMediaFail: 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* If this was an attempt to go into fax-relay mode, we failed.
      ** Attempt to go back to audio mode to continue with fax call.  Mark
      ** the fax relay session as failed such that we do not reattempt to
      ** switch to it.
      */
      if ( pSes->ses.med.faxRelay.bAttempt )
      {
         pSes->ses.med.faxRelay.bFailed = BOS_TRUE;

         cmFsmFaxEnd ( pRes );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmMediaXfer
**
** PURPOSE:    FSM early media information due to transfer service (on transferee).
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmMediaXfer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSesOrig = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pSesXfer = NULL;

   if ( pSesOrig )
   {
      CMGRLOG_DEBUG (( "cmFsmMediaXfer: 0x%X (0x%X) on 0x%X (0x%X)",
                       pSesOrig->uHdl, pSesOrig->uNet, pUsr->uHdl, pSesOrig->uNetUsr ));


      if ( pUsr->evt.pDat3 )
      {
         /* If a session already exists for the transferred call, perform a media
         ** update (this will create the physical session if it doesn't already exist).
         */
         if ( (pSesXfer = cmSesFindNetHdl(*((BOS_UINT32 *)pUsr->evt.pDat3 ), pUsr->uHdl)) != NULL )
         {
            CMGRLOG_DEBUG (( "cmFsmMediaXfer: Xfer session found - 0x%X (0x%X) on 0x%X (0x%X)",
                   pSesXfer->uHdl, pSesXfer->uNet, pUsr->uHdl, pSesXfer->uNetUsr ));

            /* For the media update reset the session handle to the new(transfer)
            ** session handle.
            */
            pUsr->evt.uDat2 = pSesXfer->uHdl;

            /* Media update.  To be re-evaluated
            ** for all environment supported whether this needs to be configurable
            ** behavior.
            */
            cmFsmMedia( pRes );
         }
         /* Create a new session to handle transferred call.
         */
         else
         {
            pSesXfer = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl );

            if ( pSesXfer )
            {
               /* Switch the focus.
               */
               if ( pSesOrig == cmUsrFocusSes ( pUsr ) )
               {
                  pSesOrig->ses.med.bFocus = BOS_FALSE;
               }
               pSesXfer->ses.med.bFocus = BOS_TRUE;
               pSesXfer->uNet = *( (BOS_UINT32 *) pUsr->evt.pDat3 );
               pSesXfer->uNetUsr = pUsr->evt.uDat4;

               cmUsrLinkSes ( pUsr, pSesXfer );
               cmSesPhyCreate ( pSesXfer );

               /* Move into the ringback state to properly process early media or lack thereof.
               */
               cmUsrNewState ( pUsr, eCMST_RINGBACK );

               CMGRLOG_DEBUG (( "cmFsmMediaXfer: Xfer session created - 0x%X (0x%X) on 0x%X (0x%X)",
                   pSesXfer->uHdl, pSesXfer->uNet, pUsr->uHdl, pSesXfer->uNetUsr ));
            }
         }

         /* May need to apply local ringback */
         if ( pSesXfer &&
              pSesXfer->ses.med.bFocus &&
              !pSesXfer->ses.med.bMediaRx )
         {
            /* Start the ring-back tone.
            */
            CMGREPSIG sig;
#if CMGRCFG_MULTILINE
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSesXfer->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

            memset ( &sig, 0, sizeof ( CMGREPSIG ) );

            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = pSesXfer->uApp;
            sig.uCid = pSesXfer->uNet;
#if CMGRCFG_MULTILINE
            sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = eEPTSIG_RINGBACK;
            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );
         }

         if ( pSesXfer == NULL )
         {
            CMGRLOG_CRITICAL (( "cmFsmMediaXfer: failed to update the physical session!"));
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmHoldFail
**
** PURPOSE:    FSM local user hold failure.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmHoldFail ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMTMR tmr;
   CMGREPSIG sig;
   BOS_BOOL bCwEnd = BOS_FALSE;
   BOS_BOOL bRestoreCall = BOS_FALSE;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

#if CMGRCFG_MULTILINE
   /* Any pending parallel call creation will thusly fail if the current call failed to hold. */
   pUsr->bDisableAutoCallSwitch = BOS_FALSE;
#endif /* CMGRCFG_MULTILINE */

   /* If a call waiting was pending, reject it.
   */
   if ( pUsr->pSesWait )
   {
#if CMGRCFG_MULTILINE
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pUsr->pSesWait->uNetUsr );
#endif /* CMGRCFG_MULTILINE */


      CMGRLOG_INFORMATION ((
                      "cmFsmHoldFail: terminate call waiting 0x%X (0x%X)",
                      pUsr->pSesWait->uHdl, pUsr->pSesWait->uNet ));

      tmr.uSes = pUsr->pSesWait->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_CALLWAIT;

      cmTmrStop ( &tmr );

      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uCid = pUsr->pSesWait->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;   /* eEPTSIG_CWTONE */
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      if( ( cmNumSesWithCid( pUsr->pSesWait->uNet ) == 1 ) &&
          ( pUsr->pSesWait->uNetUsr != CMGR_INVALID_HANDLE ) )
      {
         cctkCallDelete ( pUsr->pSesWait->uNet, eCCTKCALLDELRSN_BUSY );
      }

      cmUsrUnLinkSes ( pUsr, pUsr->pSesWait );
      pUsr->pSesWait = NULL;

      /* We terminated prematurely the call waiting due to failure to
      ** hold the currently active call.
      */
      bCwEnd = BOS_TRUE;
   }

   /* Process the remaining call for which hold failed.
   */
   if ( pSes )
   {
      /* If the call hold failure is due to a timeout scenario, see whether we're
      ** configure to restore the call which the hold failed for.
      */
      if ( pUsr->evt.pDat3 &&
            ( *(CCTKRESPCODE *)pUsr->evt.pDat3 == eCCTKRESPCODE_TIMEOUT ) )
      {
         CMGRLOG_INFORMATION (( "cmFsmHoldFail: usr 0x%X, net 0x%X pUsr->evt.pDat3=0x%X",
                       pUsr->uHdl, pSes->uNetUsr, *(CCTKRESPCODE *)pUsr->evt.pDat3));

         PROV_CFG_IDX cfgIdx;
         CFGSIPBASICCALL *pBasicCall = NULL;
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;

         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                    &cfgIdx,
                                    (void *) &pBasicCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pBasicCall &&
                 pBasicCall->bRestoreOnHoldTimeout )
            {
               bRestoreCall = BOS_TRUE;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                  &cfgIdx,
                                  (void *) pBasicCall,
                                  CFG_ITEM_DEL );
            pBasicCall = NULL;
         }

         /* If configured to restore the current call, stop any tones we were playing
         ** to the user.
         */
         if ( bRestoreCall )
         {
            sig.eAct = eCMGREPSIGACT_STOP;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = pUsr->uCpt;   /* eEPTSIG_DIALTONE */
            pUsr->uCpt = eEPTSIG_NULL;

            cmPhySignal ( pUsr->uPhys, &sig );

            /* Move back to the talking state.
            */
            cmUsrNewState ( pUsr, eCMST_TALK );
         }
      }

      /* Inform application of the service failure.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMGRSVC eSvc = eCMGRSVC_LOCAL_HOLD;
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_DENY,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) &eSvc );
      }

      /* If we just ended a call waiting, or we are configure to restore the current
      ** call, restore it to the last known media state information.
      */
      if ( bCwEnd || bRestoreCall )
      {
         cmFsmMedia ( pRes );
      }
      /* Terminate network call.
      */
      else
      {
         if ( pUsr->pSesHeld == pSes )
         {
            pUsr->pSesHeld = NULL;
            pUsr->bPresrvHeld = BOS_FALSE;
         }

         pSes->ses.med.bFocus = BOS_FALSE;

         if( ( cmNumSesWithCid( pSes->uNet ) == 1 ) &&
             ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
         {
            cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
         }

         cmUsrUnLinkSes ( pUsr, pSes );
         pSes = NULL;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmHoldOkay
**
** PURPOSE:    FSM local user hold successful completion.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmHoldOkay ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pCwSes = NULL;
   BOS_UINT32 uIx;

   /* Update the media for the call on hold.
   */
   if( pSes && pSes->uNetUsr != CMGR_INVALID_HANDLE )
   {
      cmFsmMedia ( pRes );
   }
   else if( pSes && pSes->uNetUsr == CMGR_INVALID_HANDLE )
   {
      /* Mute the local stream.
      */
      cmSesPhyMute ( pSes );
   }

   if ( pSes )
   {
      pSes->ses.med.bFocus = BOS_FALSE;

      /* Inform application of the service success.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMGRSVC eSvc = eCMGRSVC_LOCAL_HOLD;
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_APPLY,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) &eSvc );
         /* If this is an internal call, also send a REMOTE_HOLD */
         if( pSes->uNetUsr == CMGR_INVALID_HANDLE )
         {
            CMSESLIST sesList;
            memset ( &sesList, 0, sizeof ( CMSESLIST ) );

            eSvc = eCMGRSVC_REMOTE_HOLD;

            cmSesListWithCid( pSes->uNet, &sesList );
            for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
            {
               CMSES* pOtherSes = cmSesValid( sesList.uSes[ uIx ] );
               if( ( pOtherSes ) && ( pOtherSes != pSes ) )
               {
                  ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_APPLY,
                                           pOtherSes->uNet,
#if CMGRCFG_MULTILINE
                                           pOtherSes->uUsr,
                                           CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                           (void *) &eSvc );
               }
            }
         }
      }
   }

   /* If we are in the server-based conferencing scenario, initiate a call to the conference server */
   if (pUsr->pConfBridgeStatus->eRequestStatus == CMCONF_STATUS_INIT)
   {
      pUsr->pConfBridgeStatus->eRequestStatus = CMCONF_STATUS_INPROGRESS;

      pUsr->pConfBridgeStatus->pServerSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl );

      if ( pUsr->pConfBridgeStatus->pServerSes )
      {
         BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );
         pUsr->pConfBridgeStatus->pServerSes->ses.med.bFocus = BOS_TRUE;

         cmUsrLinkSes ( pUsr, pUsr->pConfBridgeStatus->pServerSes );

         cmUsrNewState ( pUsr, eCMST_DIALING );

         pUsr->pConfBridgeStatus->pServerSes->uNetUsr = uAccount;

         if ( cctkCallCreate ( &pUsr->pConfBridgeStatus->pServerSes->uNet,
                               uAccount,
                               pUsr->pConfBridgeStatus->cConfUri ) == eCCTKSTATUS_SUCCESS )
         {
#if CMGRCFG_MULTILINE
            if( !pUsr->pConfBridgeStatus->pServerSes->bEstab )
            {
               cmCctkAccntCallAdd( pUsr->pConfBridgeStatus->pServerSes->uNetUsr );
               pUsr->pConfBridgeStatus->pServerSes->bEstab = BOS_TRUE;
            }
#endif /* CMGRCFG_MULTILINE */

            CCTKPRIVCFG privCfg;
            if ( cmFeatAssignPrivacy ( pRes, &privCfg ) )
            {
               cctkSetParm ( pUsr->pConfBridgeStatus->pServerSes->uNet,
                             eCCTKPARM_EC_CALLPRIV,
                             (void *) &privCfg );
            }

            cmCfgInitCallMedia ( pRes, pUsr->pConfBridgeStatus->pServerSes );

            cctkCallMake ( pUsr->pConfBridgeStatus->pServerSes->uNet );

#if CMGRCFG_MULTILINE
            cmCctkAccntUnforce( pUsr->pConfBridgeStatus->pServerSes->uNetUsr, pUsr->pConfBridgeStatus->pServerSes->uUsr );
#endif /* CMGRCFG_MULTILINE */
            return;
         }
         else
         {
            cmFsmCallEnd ( pRes );
         }
      }
   }

#if CMGRCFG_MULTILINE
   /* If we are attempting to create a new parallel call, don't try accepting
    * any call waiting sessions, just stop here. */
   if( pUsr->bDisableAutoCallSwitch )
   {
      /* Clear the parallel call creation flag */
      pUsr->bDisableAutoCallSwitch = BOS_FALSE;

      /* Pending parallel call creation, so this is as far as we need to go. */
      return;
   }
#endif /* CMGRCFG_MULTILINE */

   /* If a call waiting was pending, accept it.
   */
   if ( ( pCwSes = pUsr->pSesWait ) != NULL )
   {
      CMSESLIST sesList;
      CMGREPSIG sig;
#if CMGRCFG_MULTILINE
      CMCCTKACCNT* pCctkAccnt = NULL;
#endif /* CMGRCFG_MULTILINE */

      CMTMR tmr;

      CMGRLOG_INFORMATION ((
                      "cmFsmHoldOkay: accept call waiting 0x%X (0x%X)",
                      pCwSes->uHdl, pCwSes->uNet ));

      memset ( &sesList, 0, sizeof ( CMSESLIST ) );
      memset ( &sig, 0, sizeof ( CMGREPSIG ) );
      memset ( &tmr, 0, sizeof ( CMTMR ) );

      /* Replace the call waiting (the session held is now the call waiting
      ** session).
      */
      pUsr->pSesWait = pSes;
      pCwSes->ses.med.bFocus = BOS_TRUE;

      tmr.uSes = pCwSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_CALLWAIT;

      cmTmrStop ( &tmr );

#if CMGRCFG_MULTILINE
      pCctkAccnt = cmCctkAccntFindByNet( pCwSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

      /* Initial answer of call waiting.
      */
      if ( !pCwSes->bEstab )
      {
         if ( pCwSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
         {
            /* We would be playing ringback to the peer. Terminate it.
            */
            sig.eAct = eCMGREPSIGACT_STOP;
            sig.uSes = pCwSes->ses.med.uPhy;
            sig.uCid = pCwSes->uNet;
#if CMGRCFG_MULTILINE
            sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uSig = eEPTSIG_RINGBACK;

            cmPhySignal ( pUsr->uPhys, &sig );
         }

         pCwSes->bEstab = BOS_TRUE;
#if CMGRCFG_MULTILINE
         if( pCwSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            cmCctkAccntCallAdd( pCwSes->uNetUsr );
         }
#endif

         /* Notify the application that the call has connected */
         if ( gCmCfgCb.cmgrEvtCb )
         {
            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                     pCwSes->uNet,
#if CMGRCFG_MULTILINE
                                     pUsr->uHdl,
                                     (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif
                                     (void *) pUsr->uUid );
         }

         /* If we need to finalize the media capabilities, do it now
         ** prior to call answer.
         */
         if ( pCwSes->ses.med.bNeedFinal &&
              pCwSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            pCwSes->ses.med.bNeedFinal = BOS_FALSE;
            cctkCallMediaNotify( pCwSes->uNet,
                                 eCCTKMUDPRSN_FINALIZECAPS );
         }

         if( pCwSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            cmSesListWithCid( pCwSes->uNet, &sesList );
            for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
            {
               CMSES* pSesRelease = cmSesValid( sesList.uSes[ uIx ] );
               CMUSR* pUsrRelease = (pSesRelease) ? cmUsrValid( pSesRelease->uUsr ) : NULL;

               if( ( pSesRelease ) && ( pUsrRelease ) && ( pSesRelease->uHdl != pCwSes->uHdl ) )
               {
                  CMEVT cmEvt;

                  memset( &cmEvt, 0, sizeof( CMEVT ) );

                  cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
                  cmEvt.uDat1 = pSesRelease->uUsr;
                  cmEvt.uDat2 = pSesRelease->uHdl;
                  cmEvt.uDat4 = pSesRelease->uNetUsr;

                  /* Take care of any specific data coming with the event that need
                  ** to be saved.
                  */
                  memcpy ( &pUsrRelease->evt, &cmEvt, sizeof ( CMEVT ) );

                  /* Run the state machine.
                  */
                  cmFsmCallEnding( pUsrRelease );
               }
            }
            if ( cctkCallAnswer ( pCwSes->uNet ) ==  eCCTKSTATUS_SUCCESS )
            {
               /* Pass on the provisioned privacy configuration of the local
               ** user - may be needed for transfer scenarios.
               */
               CCTKPRIVCFG privCfg;
               if ( cmFeatAssignPrivacy ( pRes, &privCfg ) )
               {
                  cctkCallPrivInfo ( pCwSes->uNet, (void *) &privCfg );
               }
            }
         }
         else
         {
            /* If any other sessions exist with this call id, we need to
            ** perform some action:
            ** [1] - if the session is not yet established, we should
            **       free it since we just took the call here.
            ** [2] - if the session is established, this means it was the
            **       originator of an internal call. We should send it a
            **       "CONNECT_CALL" event to finish setting up the call.
            */
            cmSesListWithCid( pCwSes->uNet, &sesList );
            for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
            {
               /* Release the call on all other associated sessions */
               if( sesList.uSes[ uIx ] != pCwSes->uHdl )
               {
                  CMSES* pSesClosing = cmSesValid( sesList.uSes[ uIx ] );
                  CMUSR* pUsrClosing = cmUsrValid( pSesClosing->uUsr );
                  CMEVT cmEvt;

                  /* If the session is already "established", that means that the user is in
                  ** the call on this session. The only time this can happen is if that user
                  ** is the one initiating the call, in which case we don't want to tear down
                  ** its session.
                  */
                  if( !pSesClosing->bEstab )
                  {
                     cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
                     cmEvt.uDat1 = pUsrClosing->uHdl;
                     cmEvt.uDat2 = pSesClosing->uHdl;
                     cmEvt.uDat4 = pSesClosing->uNetUsr;

                     /* Take care of any specific data coming with the event that need
                     ** to be saved.
                     */
                     memcpy ( &pUsrClosing->evt, &cmEvt, sizeof ( CMEVT ) );

                     /* Run the state machine right away.
                     */
                     cmFsmCallEnding( pUsrClosing );
                  }
                  /* If the session is already established, the user is the originator of
                  ** the call.
                  */
                  else
                  {
                     CCTKCALLINFO *pCallInfo = malloc ( sizeof ( CCTKCALLINFO ) );

                     memset( pCallInfo, 0, sizeof( CCTKCALLINFO ) );

                     strPrintf( (char*)pCallInfo->clid.cName,
                               CCTK_CLID_LEN,
                               "%s",
                               (char*)pUsr->uUid );
                     strPrintf( (char*)pCallInfo->clid.cNumber,
                               CCTK_CLID_LEN,
                               "%s",
                               (char*)pUsr->uUid );
                     pCallInfo->bEmergency = BOS_FALSE;
                     pCallInfo->bLoopBack = BOS_FALSE;

                     cmEvt.uEvt = ( eCMEVT_CONNECT_CALL | CMGR_EVT_CLASS__CCTK );
                     cmEvt.uDat1 = pUsrClosing->uHdl;
                     cmEvt.uDat2 = pSesClosing->uHdl;
                     cmEvt.pDat3 = pCallInfo;
                     cmEvt.uDat4 = pSesClosing->uNetUsr;

                     /* Post the message */
                     if( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                     {
                        cmSignalEvt();
                     }
                     else
                     {
                        CMGRLOG_CRITICAL(( "%s: cmPostEvt failed. Call cannot be established.", __FUNCTION__ ));
                     }
                  }
               }
            }
         }

         /* Generate an internal media event to setup the session properly
         ** now.
         */
         pUsr->evt.uDat2 = pCwSes->uHdl;
         cmFsmMedia ( pRes );
      }
      /* Call waiting toggle.  Unhold the already answered session.
      */
      else
      {
         /* External call */
         if( pCwSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            cctkCallUnhold ( pCwSes->uNet );
         }
         /* Internal call */
         else
         {
            CMEVT cmEvt;

            memset( &cmEvt, 0, sizeof( CMEVT ) );

            cmEvt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK );
            cmEvt.uDat1 = pUsr->uHdl;
            cmEvt.uDat2 = pCwSes->uHdl;
            cmEvt.uDat4 = pCwSes->uNetUsr;

            /* Post the message */
            if( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
            {
               cmSignalEvt();
            }
         }
      }

      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = pCwSes->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;   /* eEPT_SIG_CWTONE */
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      cmUsrNewState ( pUsr, eCMST_TALK );
   }
#if CMGRCFG_MULTILINE
   else
   {
      /* No waiting session - Need to check whether we need to repopulate the
      ** cwait flags which were cleared when the HOLD_SVC was requested by multiline
      */

      /* See if there is another session
      */
      pCwSes = NULL;
      for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
      {
         if ( pUsr->pMedSes [ uIx ] )
         {
            if ( pUsr->pMedSes [ uIx ] != pSes )
            {
               pCwSes = pUsr->pMedSes [ uIx ];
            }
         }
      }

      /* If other session is not focus session, then set it as cw session
      ** otherwise set the session that was held as the cw session
      */
      if( pCwSes )
      {
         pUsr->pSesWait = (pCwSes->ses.med.bFocus) ? NULL : pCwSes;
      }
   }
#endif /* CMGRCFG_MULTILINE */
}


/***********************************************************************************
** FUNCTION:   cmFsmUnHoldOkay
**
** PURPOSE:    FSM local user un-hold successful completion.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmUnHoldOkay ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pOtherSes = NULL;
   BOS_UINT32 uIx;

   /* Update the media for the call no longer on hold.
   */
   cmFsmMedia ( pRes );

   /* See if there is another session
   */
   for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] )
      {
         if ( pUsr->pMedSes [ uIx ] != pSes )
         {
            pOtherSes = pUsr->pMedSes [ uIx ];
         }
      }
   }

   if ( pSes )
   {
#if CMGRCFG_MULTILINE
      if ( pSes->ses.med.uConf != CMGR_INVALID_HANDLE )
      {
         /* for conference calls, if call toggling is enabled for 3wc,
         ** then the session focus needs to be updated such that the
         ** current session is in focus and the other session is set
         ** as the waiting session.
         */
         CFGSIPTHREEWAYCALL *p3WayCall = NULL;
         PROV_CFG_IDX cfgIdx;
         BOS_BOOL bToggle = BOS_FALSE;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                                    &cfgIdx,
                                    (void *) &p3WayCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( p3WayCall && p3WayCall->Act.bCfgStatus )
            {
               bToggle = p3WayCall->bToggleEnable;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                                  &cfgIdx,
                                  (void *) p3WayCall,
                                  CFG_ITEM_DEL );
            p3WayCall = NULL;
         }

         if ( bToggle )
         {
           /* Set this session as focused
            */
            pSes->ses.med.bFocus = BOS_TRUE;

            /* Make waiting session unfocused */
            if( pOtherSes )
            {
               pOtherSes->ses.med.bFocus = BOS_FALSE;
            }

            /* Set other session as waiting / clear seswait if no other session */
            pUsr->pSesWait = pOtherSes;

            /* Set talk state, if not already in talk state */
            if( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_TALK )
            {
               cmUsrNewState ( pUsr, eCMST_TALK );
            }
         }
      }
#endif /* CMGRCFG_MULTILINE */

      /* Inform application of the service success.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMGRSVC eSvc = eCMGRSVC_LOCAL_UNHOLD;

         /* Indicate to the outside world if conference was created */
         if ( pOtherSes && (pSes->ses.med.uConf == pOtherSes->uHdl) && (pOtherSes->ses.med.uConf == pSes->uHdl) )
         {
            BOS_UINT32 uConfNet = pSes->uNet;
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pOtherSes->uNetUsr );

            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CONF_ESTAB,
                                     pOtherSes->uNet,
#if CMGRCFG_MULTILINE
                                     pOtherSes->uUsr,
                                     (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                     (void *) &uConfNet );
         }
         else
         {

            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_APPLY,
                                     pSes->uNet,
#if CMGRCFG_MULTILINE
                                     pSes->uUsr,
                                     CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                     (void *) &eSvc );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmRemHold
**
** PURPOSE:    FSM remote hold of local user.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRemHold ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      pSes->ses.med.bHeld = BOS_TRUE;

      /* Inform application of the service application.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMGRSVC eSvc = eCMGRSVC_REMOTE_HOLD;
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_APPLY,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) &eSvc );
      }

      CMGRLOG_INFORMATION ((
                      "cmFsmRemHold: 0x%X (0x%X) on 0x%X (0x%X)",
                      pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));
   }

   /* Update the media for the call held.
   */
   cmFsmMedia ( pRes );
}


/***********************************************************************************
** FUNCTION:   cmFsmRemUnHold
**
** PURPOSE:    FSM remote unhold of local user.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRemUnHold ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      pSes->ses.med.bHeld = BOS_FALSE;

      /* Inform application of the service application.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMGRSVC eSvc = eCMGRSVC_REMOTE_UNHOLD;
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_APPLY,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) &eSvc );
      }

      CMGRLOG_INFORMATION ((
                      "cmFsmRemUnHold: 0x%X (0x%X) on 0x%X (0x%X)",
                      pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));
   }

   /* Update the media for the call unheld.
   */
   cmFsmMedia ( pRes );
}


/***********************************************************************************
** FUNCTION:   cmFsmUnHoldFail
**
** PURPOSE:    FSM local user unhold failure.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmUnHoldFail ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      /* Inform application of the service failure.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMGRSVC eSvc = eCMGRSVC_LOCAL_UNHOLD;
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_DENY,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) &eSvc );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallEnding
**
** PURPOSE:    FSM call ending while ringing handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallEnding ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pFocusSes = cmUsrFocusSes ( pUsr );
   CMGREPSIG sig;
   CMTMR tmr;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = NULL;
#endif /* CMGRCFG_MULTILINE */

   if ( pSes )
   {
#if CMGRCFG_MULTILINE
      pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

   }



   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   CMGRLOG_DEBUG (( "cmFsmCallEnding: 0x%X (0x%X)", pUsr->uHdl, ( pSes ) ? pSes->uNetUsr : CMGR_INVALID_HANDLE ));

   /* If there is no focus session or this is the focus session being
   ** terminated, stop the ring.  Otherwise, it is a transient session being
   ** terminated and we should not stop the ring.
   */
   if ( ( pFocusSes == NULL ) || ( pSes && ( pFocusSes == pSes ) ) )
   {
      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_RINGING;

      cmTmrStop ( &tmr );

      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGING */
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      if ( pSes && ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE ) )
      {
         /* We would be playing ringback to the peer. Terminate it.
         */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = pSes->ses.med.uPhy;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = eEPTSIG_RINGBACK;

         cmPhySignal ( pUsr->uPhys, &sig );
     }
   }

   cmFsmCallEnd ( pRes );
}


/***********************************************************************************
** FUNCTION:   cmFsmCallRejected
**
** PURPOSE:    FSM call ended in rejection while attempting handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRejected ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pFocusSes = cmUsrFocusSes ( pUsr );

   if ( pSes )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCallRejected: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* If this session was setup to be used for an auto-call service and
      ** we have been rejected, we need to terminate the auto-call service.
      */
      if ( pFocusSes && ( pFocusSes == pSes ) &&
           pSes->ses.med.pAutoCall )
      {
         /* Terminate the auto-call service at the user level.
         */
         memset( (void *) (CMAUTOCALL *)pSes->ses.med.pAutoCall,
                 0,
                 sizeof ( CMAUTOCALL ) );

         pSes->ses.med.pAutoCall = NULL;
      }

      /* Clean up this call since terminated.
      */
      cmFsmCallEnd( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallBusy
**
** PURPOSE:    FSM received busy treatment.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallBusy ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMGREPSIG sig;
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = (pSes) ? cmCctkAccntFindByNet( pSes->uNetUsr ) : NULL;
#endif /* CMGRCFG_MULTILINE */


   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCallBusy: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* If this session was setup to be used for an auto-call service and
      ** we have been rejected as 'busy', we need to activate the second
      ** aspect of the service which consist in subscribing to some information
      ** from the remote end to determine when it will be possible to place
      ** the next call.
      */
      if ( pSes->ses.med.pAutoCall &&
           ( (CMAUTOCALL *) pSes->ses.med.pAutoCall )->bUsed &&
           !( (CMAUTOCALL *) pSes->ses.med.pAutoCall )->bAnonymous )
      {
         CMAUTOCALL *pAutoCall = (CMAUTOCALL *) pSes->ses.med.pAutoCall;
         CMSES *pSrvSes;

         if (  ( pSrvSes = cmSesAlloc ( eCMSESTYPE_SRV, pUsr->uHdl ) ) != NULL )
         {
            if ( cmUsrLinkSes ( pUsr, pSrvSes ) )
            {
               BOS_UINT32 uTimeOut = 0;
               CFGSIPAUTOCALL *pACall;
               PROV_CFG_IDX cfgIdx;

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( pSes->ses.med.eAction == eCMDM_ACACTIVATE )
               {
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                             &cfgIdx,
                                             (void *) &pACall,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pACall )
                     {
                        uTimeOut = pACall->uTimer;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                           &cfgIdx,
                                           (void *) pACall,
                                           CFG_ITEM_DEL );
                     pACall = NULL;
                  }
                  else
                  {
                     uTimeOut = CMGR_AUTO_CALL_TIMEOUT;
                  }
               }
               else if ( pSes->ses.med.eAction == eCMDM_ARACTIVATE )
               {
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                             &cfgIdx,
                                             (void *) &pACall,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pACall )
                     {
                        uTimeOut = pACall->uTimer;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                           &cfgIdx,
                                           (void *) pACall,
                                           CFG_ITEM_DEL );
                     pACall = NULL;
                  }
                  else
                  {
                     uTimeOut = CMGR_AUTO_CALL_TIMEOUT;
                  }
               }

               /* Initial subscription to feature data.
               */
               if ( uTimeOut &&
                    ( cctkEventCreate ( &pSrvSes->uNet,
                                        pSes->uNetUsr,
                                        eCCTKSUBEVT_DLG,
                                        uTimeOut / CMGR_MSEC_2_SEC_DIVIDER,
                                        pAutoCall->uPeer ) ==
                                                    eCCTKSTATUS_SUCCESS ) )
               {
                  /* Start the feature timer.
                  */
                  CMTMR tmr;

                  memset ( (void *) &tmr,
                           0,
                           sizeof ( CMTMR ) );

                  tmr.uSes = pSrvSes->uHdl;
                  tmr.uUsr = pUsr->uHdl;
                  tmr.eType =
                     ( ( pSes->ses.med.eAction == eCMDM_ACACTIVATE ) ?
                              eCMEVT_TMR_AUTOCALLBACK : eCMEVT_TMR_AUTORECALL );
                  tmr.uTimeOut = uTimeOut;

                  cmTmrStart ( &tmr );


                  pSrvSes->ses.srv.pAutoCall = pSes->ses.med.pAutoCall;

                  /* Subscribe the event.
                  */
                  cctkEventSubscribe ( pSrvSes->uNet );
               }
            }
         }
      }

      /* If we are configured to do so, issue a network disconnect
      ** signal.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pBasicCall && pBasicCall->bIssueNetDisc )
         {
            cmFsmOsi ( pRes );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }

      if ( pSes->ses.med.bFocus )
      {
         /* Play some busy tone such that local user hangs up.
         */
         sig.eAct = eCMGREPSIGACT_START;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = pSes->uApp;
         sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = eEPTSIG_BUSY;
         pUsr->uCpt = sig.uSig;

         cmPhySignal ( pUsr->uPhys, &sig );
      }

      /* Terminate this call now.
      */
      cmFsmCallEnded( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmDlg
**
** PURPOSE:    FSM received DLG (dialog) information.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDlg ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMAUTOCALL *pAutoCall = (CMAUTOCALL *) pSes->ses.srv.pAutoCall;

      CMGRLOG_INFORMATION ((
         "cmFsmDlg: 0x%X (0x%X) on 0x%X (0x%X) - %s",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr,
         (*( (BOS_BOOL *) pUsr->evt.pDat3 ) ? "Busy" : "Idle") ));

      /* Check and remember the state for the pending dialog.
      */
      if ( pAutoCall && pAutoCall->bUsed )
      {
         if ( !(*( (BOS_BOOL *) pUsr->evt.pDat3 )) )
         {
            pAutoCall->bPending = BOS_TRUE;
            /* If we are idle, apply the service now.  Otherwise, the service will
            ** be delayed until we get idle again.
            */
            if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_IDLE )
            {
               cmFsmAutoCallApply ( pRes );
            }
         }
         else
         {
            pAutoCall->bPending = BOS_FALSE;
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmForward
**
** PURPOSE:    FSM received forward related information.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmForward ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_BOOL bNewCfvCnt = BOS_FALSE;
   BOS_BOOL bNewScfCnt = BOS_FALSE;
   BOS_BOOL bCfvRingReminder = BOS_FALSE;
   BOS_BOOL bScfRingReminder = BOS_FALSE;
   BOS_BOOL bCntCheck = BOS_TRUE;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bAlloc = BOS_FALSE;
   BOS_UINT8 uCfvCnt = 0;
   BOS_UINT8 uScfCnt = 0;
   BOS_BOOL bRSOnInitCfv = BOS_FALSE;

   if ( pUsr->evt.pDat3 )
   {
      CCTKFEATINFO *pFwdInfo = (CCTKFEATINFO *) pUsr->evt.pDat3;

      CFGSIPCALLFWD *pFwdCfg = NULL;
      CFGSIPSCF *pScfCfg = NULL;

      CMGRLOG_INFORMATION ((
         "cmFsmForward: 0x%X (0x%X) - Cfv %s/%d - Scf %s/%d - Dnd %s",
         pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ),
         pFwdInfo->bCfv ? "yes" : "no", pFwdInfo->uCfvCnt,
         pFwdInfo->bScf ? "yes" : "no", pFwdInfo->uScfCnt,
         pFwdInfo->bDnd ? "yes" : "no" ));

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                 &cfgIdx,
                                 (void *) &pFwdCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pFwdCfg && pFwdCfg->Act.bCfgStatus )
         {
            bCfvRingReminder = pFwdCfg->bRingReminder;
            bCntCheck = !pFwdCfg->bIgnoreCntCheck;
            bRSOnInitCfv = pFwdCfg->bRSOnInitCfv;
            uCfvCnt = pFwdCfg->uCount;

            CMGRLOG_DEBUG ((
               "cmFsmForward: 0x%X (%s) - Local CFV count %d",
               pUsr->uHdl, pUsr->uUid, uCfvCnt ));

            /* Store the new value immediately.
            */
            if ( ( pFwdInfo->uCfvCnt != CMGR_INVALID_HANDLE ) &&
                 ( pFwdInfo->uCfvCnt != uCfvCnt ) )
            {
               pFwdCfg->uCount = (BOS_UINT8) pFwdInfo->uCfvCnt;

#if CMGRCFG_MULTILINE
               if (pUsr->evt.bLast)
#endif
               {
                  CMGRLOG_DEBUG ((
                     "cmFsmForward: 0x%X (%s) - Update CFV count %d -> %d",
                     pUsr->uHdl, pUsr->uUid, uCfvCnt, pFwdInfo->uCfvCnt ));

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                        &cfgIdx,
                                        (void *) pFwdCfg,
                                        CFG_ITEM_SET );
               }
            }
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                               &cfgIdx,
                               (void *) pFwdCfg,
                               CFG_ITEM_DEL );
         pFwdCfg = NULL;
      }

      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, SCFConcept ),
                                 &cfgIdx,
                                 (void *) &pScfCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pScfCfg && pScfCfg->Act.bCfgStatus )
         {
            bScfRingReminder = pScfCfg->bRingReminder;
            uScfCnt = pScfCfg->uCount;

            CMGRLOG_DEBUG ((
               "cmFsmForward: 0x%X (%s) - Local SCF count %d",
               pUsr->uHdl, pUsr->uUid, uScfCnt ));

            /* Store the new value immediately.
            */
            if ( ( pFwdInfo->uScfCnt != CMGR_INVALID_HANDLE ) &&
                 ( pFwdInfo->uScfCnt != uScfCnt ) )
            {
               pScfCfg->uCount = (BOS_UINT8) pFwdInfo->uScfCnt;

               CMGRLOG_DEBUG ((
                  "cmFsmForward: 0x%X (%s) - Update SCF count %d -> %d",
                  pUsr->uHdl, pUsr->uUid, uScfCnt, pFwdInfo->uScfCnt ));

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, SCFConcept ),
                                     &cfgIdx,
                                     (void *) pScfCfg,
                                     CFG_ITEM_SET );
            }
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, SCFConcept ),
                               &cfgIdx,
                               (void *) pScfCfg,
                               CFG_ITEM_DEL );
         pScfCfg = NULL;
      }

      /* Check whether we have new data available.
      */
      if ( ( pFwdInfo->uCfvCnt != CMGR_INVALID_HANDLE ) &&
           ( !bCntCheck ||
             ( bCntCheck && ( (BOS_UINT8) pFwdInfo->uCfvCnt > uCfvCnt ) ) ||
             ( bRSOnInitCfv && ( (BOS_UINT8) pFwdInfo->uCfvCnt == 1 ) ) ) )
      {
         bNewCfvCnt = BOS_TRUE;
      }

      if ( ( pFwdInfo->uScfCnt != CMGR_INVALID_HANDLE ) &&
           ( !bCntCheck ||
               ( bCntCheck && ( (BOS_UINT8) pFwdInfo->uScfCnt > uScfCnt ) ) ) )
      {
         bNewScfCnt = BOS_TRUE;
      }

      /* If we received new information AND this information is identifying
      ** new forwarded calls (i.e. the counters have positive values), then
      ** if we need to ring-splash the user, do it now.
      */
      if ( ( ( bNewCfvCnt && pFwdInfo->uCfvCnt && bCfvRingReminder ) ||
             ( bNewScfCnt && pFwdInfo->uScfCnt && bScfRingReminder ) )
           && !pUsr->bOffHook )
      {
         /* Signal ring-splash to the physical interface
         ** of interest associated with this user.
         */
         if ( pUsr->uPhys == CMGR_INVALID_HANDLE )
         {
            bAlloc = ( cmPhyAllocate ( pUsr ) == eCMGRSTATUS_COMMAND_SUCCESS );
         }

         /* If we have a valid interface to signal on.
         */
         if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
         {
            CMGREPSIG sig;
            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE; /* TODO: Fix forwarding to properly pass the cid/line */
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = eEPTSIG_SPLASH;

            cmPhySignal ( pUsr->uPhys, &sig );
         }

         /* If we allocated the interface just for this purpose, release it now.
         */
         if ( bAlloc )
         {
            pUsr->uPhys = CMGR_INVALID_HANDLE;
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallXfered
**
** PURPOSE:    FSM call transfered handler (on transferee).
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallXfered ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSesOrig = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pSesXfer = NULL;

   if ( pSesOrig )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCallXfered: 0x%X (0x%X) on 0x%X (0x%X)",
         pSesOrig->uHdl, pSesOrig->uNet, pUsr->uHdl, pSesOrig->uNetUsr ));

      if (pUsr->pConfBridgeStatus->eRequestStatus == CMCONF_STATUS_COMPLETING)
      {
         if (pUsr->pConfBridgeStatus->eConfOption == CFG_SIP_CONFERENCING_REFER_PARTICIPANTS)
         {
            /* All we need to do is set the request status to idle.
            **   Connections to participants are torn down by CCTK state machine
            */
            pUsr->pConfBridgeStatus->eRequestStatus = CMCONF_STATUS_IDLE;
         }
         else if (pUsr->pConfBridgeStatus->eConfOption == CFG_SIP_CONFERENCING_REFER_SERVER)
         {
            if (pUsr->pConfBridgeStatus->pHeldSes[0])
            {
               /* Transfer the second held session */
               cmSesTerminate(pUsr->pConfBridgeStatus->pHeldSes[0]);
               pUsr->pConfBridgeStatus->pHeldSes[0] = NULL;
               cctkCallTransfer(pUsr->pConfBridgeStatus->pServerSes->uNet, pUsr->pConfBridgeStatus->pHeldSes[1]->uNet, BOS_TRUE);
            }
            else if (pUsr->pConfBridgeStatus->pHeldSes[1])
            {
               cmSesTerminate(pUsr->pConfBridgeStatus->pHeldSes[1]);
               pUsr->pConfBridgeStatus->pHeldSes[1] = NULL;
               pUsr->pConfBridgeStatus->eRequestStatus = CMCONF_STATUS_IDLE;
            }
         }
         else
         {
            ; /* we should not get here as the conf bridge state machine is enacted only in the above cases */
         }
         return;
      }

      /* The new transferred call should have it's own session. If this session already
      ** exists as it was created for early media, then perform an update now, otherwise
      ** create the physical session now that the transfered-to party has answered the call.
      */
      if ( pUsr->evt.pDat3 )
      {
         /* Find the Xfer session if it was already created.
         */
         pSesXfer = cmSesFindNetHdl ( *( (BOS_UINT32 *) pUsr->evt.pDat3 ), pUsr->uHdl );

         /* Call was transferred - stop the ringback tone if we were playing it.
         */
         if ( pUsr->uCpt == eEPTSIG_RINGBACK )
         {
            /* Terminate the local ring-back tone if playing.
            */
            CMGREPSIG sig;
            CMTMR tmr;
#if CMGRCFG_MULTILINE
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSesOrig->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

            memset ( &sig, 0, sizeof ( CMGREPSIG ) );
            memset ( &tmr, 0, sizeof ( CMTMR ) );

            sig.eAct = eCMGREPSIGACT_STOP;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = ( pSesXfer != NULL )? pSesXfer->uNet : pSesOrig->uNet;
#if CMGRCFG_MULTILINE
            sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGBACK */
            pUsr->uCpt = eEPTSIG_NULL;

            tmr.uSes = pSesOrig->uHdl;
            tmr.uUsr = pUsr->uHdl;
            tmr.eType = eCMEVT_TMR_RINGBACK;

            cmTmrStop ( &tmr );

            cmPhySignal ( pUsr->uPhys, &sig );
         }

         /* If this session already exists then perform an internal media update
         */
         if ( pSesXfer )
         {
            CMGRLOG_DEBUG (( "cmFsmCallXfered: Xfer session found - 0x%X (0x%X) on 0x%X (0x%X)",
                   pSesXfer->uHdl, pSesXfer->uNet, pUsr->uHdl, pSesXfer->uNetUsr ));
            /* Set the session update handle to the new(transfer) session handle.
            */
            pUsr->evt.uDat2 = pSesXfer->uHdl;
            /* Media update.  To be re-evaluated
            ** for all environment supported whether this needs to be configurable
            ** behavior.
            */
            cmFsmMedia( pRes );

            cmUsrNewState ( pUsr, eCMST_TALK );
         }
         /* The session has not been created for the transferred call yet - do so now.
         */
         else
         {

            /* Create a new session for the transferred call.
            */
            if ( ( pSesXfer = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl )) != NULL )
            {
               /* Switch the focus.
               */
               if ( pSesOrig == cmUsrFocusSes ( pUsr ) )
               {
                  pSesOrig->ses.med.bFocus = BOS_FALSE;
               }
               pSesXfer->ses.med.bFocus = BOS_TRUE;
               pSesXfer->uNet = *( (BOS_UINT32 *) pUsr->evt.pDat3 );
               pSesXfer->uNetUsr = pUsr->evt.uDat4;

               cmUsrLinkSes ( pUsr, pSesXfer );

               cmSesPhyCreate ( pSesXfer );

               CMGRLOG_DEBUG (( "cmFsmCallXfered: Xfer session created - 0x%X (0x%X) on 0x%X (0x%X)",
                   pSesXfer->uHdl, pSesXfer->uNet, pUsr->uHdl, pSesXfer->uNetUsr ));

               cmUsrNewState ( pUsr, eCMST_TALK );
            }
         }

         /* The transferred call has been answered, so mark the session
         ** as established.
         */
         if( pSesXfer && !pSesXfer->bEstab )
         {
            pSesXfer->bEstab = BOS_TRUE;
#if CMGRCFG_MULTILINE
            if( pSesXfer->uNetUsr != CMGR_INVALID_HANDLE )
            {
               cmCctkAccntCallAdd( pSesXfer->uNetUsr );
            }
#endif
         }
      }



      /* Send Event via Client Callback with Remote User Identifier
      ** Note: Add Remote Party Number once we figure out where it can be
      ** obtained.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSesOrig ? pSesOrig->uNetUsr : CMGR_INVALID_HANDLE) );
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_XFER,
                                  pSesOrig->uApp,
#if CMGRCFG_MULTILINE
                                  pSesOrig->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) NULL );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallAlert
**
** PURPOSE:    FSM incoming call alert handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallAlert ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
#if CMGRCFG_MULTILINE
   CMSES *pSes = cmSesFindNetHdl ( pUsr->evt.uDat2, pUsr->uHdl );
#else
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
#endif
   CMSES *pOtherSes = NULL;

#if CMGRCFG_MULTILINE
   /* If this call is assigned to another user */
   if ( ( cmNumCidAssigned( pUsr->evt.uDat2 ) == 1 ) &&
        ( cmUsrFindCid( pUsr->evt.uDat2, BOS_TRUE ) != pUsr ) &&
        ( pUsr->evt.uDat4 != CMGR_INVALID_HANDLE ) )
   {
      /* Don't process anything */
      return;
   }
#endif /* CMGRCFG_MULTILINE */

#if !CMGRCFG_MULTILINE
   /* If the event had arrived early, the handle to lookup may be the
   ** actual network handle.  Give it another chance.
   */
   if ( pSes == NULL )
   {
      pSes = cmSesFindNetHdl ( pUsr->evt.uDat2, pUsr->uHdl  );

      /* Update the event content to reflect the reality now that we
      ** have properly identified the session owner.
      */
      if ( pSes )
      {
         pUsr->evt.uDat2 = pSes->uHdl;
      }
   }
#endif /* !CMGRCFG_MULTILINE */

   if ( pSes )
   {
#if CMGRCFG_MULTILINE
      /* Update the event content to reflect the reality now that we
      ** have properly identified the session owner.
      */
      pUsr->evt.uDat2 = pSes->uHdl;
#endif /* CMGRCFG_MULTILINE */
      CMGRLOG_INFORMATION ((
         "cmFsmCallAlert: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* Copy the call information into the session block.
      */
      if ( pUsr->evt.pDat3 )
      {
         memcpy ( &pSes->ses.med.callInfo,
                  pUsr->evt.pDat3,
                  sizeof ( CCTKCALLINFO ) );
      }
      else
      {
         memset ( &pSes->ses.med.callInfo,
                  0,
                  sizeof ( CCTKCALLINFO ) );
         memcpy ( &pSes->ses.med.callInfo.clid,
                  &defaultClid,
                  sizeof ( CCTKCLIDINFO ) );
      }

      /* At this time we have enough information to confirm whether the
      ** call is anonymous or not, in such case, we can decide to reject
      ** it if warranted.
      */
      if ( pSes->ses.med.callInfo.bAnonymous && pUsr->bRejectAnonymous )
      {
         /* Terminate the local session now.
         */
         cmUsrUnLinkSes ( pUsr, pSes );
         cmSesFree ( pSes, pUsr->uHdl );

         /* Reject the network call.
         */
         cmFsmCallRejectAnonymous ( pRes );

         /* Restore a proper state if necessary.
         */
         if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_ACCEPTED )
         {
            cmUsrNewState ( pUsr, eCMST_IDLE );
         }
      }
#if CMGRCFG_REMOTE_USER_CONTROL
      /* If this is for a remote user, we need to 'proxy' this particular
      ** call to the final destination.
      */
      else if ( pUsr->eType == eCMUSRTYPE_REMOTE )
      {
         cmFeatProxyCaller ( pUsr, pSes );
      }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
      else if ( pSes->bEstab && !pSes->ses.med.bEmergency )
      /* If this is an already established session which is not an emergency call
      ** this event would be the result of a transfer on the transfer-to party
      ** side.  In which case, we can safely ignore it.
      */
      {
         CMGRLOG_DEBUG (( "cmFsmCallAlert: Ignoring (transfer) 0x%X (0x%X)",
                          pSes->uHdl, pSes->uNet ));
      }
#if !CMGRCFG_MULTILINE
      /* If this is a loopback call, we do not alert the user but we will answer
      ** the call automatically right away.
      */
      else if ( pSes->ses.med.callInfo.bLoopBack )
      {
         pSes->bEstab = BOS_TRUE;

         CMGRLOG_DEBUG (( "cmFsmCallAlert: Answering Loopback 0x%X (0x%X)",
                          pSes->uHdl, pSes->uNet ));

         if ( cctkCallAnswer ( pSes->uNet ) ==  eCCTKSTATUS_SUCCESS )
         {
            /* Pass on the provisioned privacy configuration of the local
            ** user - may be needed for transfer scenarios.
            */
            CCTKPRIVCFG privCfg;
            if ( cmFeatAssignPrivacy ( pRes, &privCfg ) )
            {
               cctkCallPrivInfo ( pSes->uNet, (void *) &privCfg );
            }
         }
         cmUsrNewState ( pUsr, eCMST_TALK );
      }
      /* If this is a barge-in call, associate it right away with the existing call
      ** of interest, the media update following the call answer will take care
      ** of conferencing in the calls.
      */
      else if ( ( pOtherSes =
                     cmSesFindNetHdl ( pSes->ses.med.callInfo.uJoin , pUsr->uHdl ) ) != NULL )
      {
         pSes->bEstab = BOS_TRUE;

         CMGRLOG_DEBUG (( "cmFsmCallAlert: Answering Barge-In 0x%X (0x%X)",
                          pSes->uHdl, pSes->uNet ));

         pSes->ses.med.uConf = pOtherSes->uHdl;
         pOtherSes->ses.med.uConf = pSes->uHdl;

         if ( cctkCallAnswer ( pSes->uNet ) ==  eCCTKSTATUS_SUCCESS )
         {
            /* Pass on the provisioned privacy configuration of the local
            ** user - may be needed for transfer scenarios.
            */
            CCTKPRIVCFG privCfg;
            if ( cmFeatAssignPrivacy ( pRes, &privCfg ) )
            {
               cctkCallPrivInfo ( pSes->uNet, (void *) &privCfg );
            }
         }
         cmUsrNewState ( pUsr, eCMST_TALK );
      }
#endif /* !CMGRCFG_MULTILINE */
      else
      {
#if CMGRCFG_REMOTE_USER_CONTROL
         /* Alert remote user if there is one
         */
         if ( (pUsr->eType != eCMUSRTYPE_REMOTE) && pUsr->uLinkedRemoteUserCnt )
         {
            cmFeatProxyCallee ( (void *) pSes );
         }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

         /* Already in active media mode, call waiting scenario.
         */
         if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_TALK )
         {
            CFGSIPCALLWAIT *pCwCfg = NULL;
            PROV_CFG_IDX cfgIdx;

            /* Keep track of the call waiting.
            */
            pUsr->pSesWait = pSes;

            cmFsmCallWait ( pRes );

            /* Check if we are configured to not keep track of call waiting
            ** explicitely.
            **
            ** This assumes that there is a upper layer application capable of
            ** proper call management.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                                       &cfgIdx,
                                       (void *) &pCwCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCwCfg && pCwCfg->bNoCallWaitTracking )
               {
                  /* Note that this MUST happen after the call to
                  ** 'cmFsmCallWait' processing above that requires
                  ** the 'pSesWait' identifier to not be NULL.
                  */
                  pUsr->pSesWait = NULL;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                                     &cfgIdx,
                                     (void *) pCwCfg,
                                     CFG_ITEM_DEL );
               pCwCfg = NULL;
            }
         }
         else
         {
            CFGSIPEMERGE *pEmergCfg = NULL;
            PROV_CFG_IDX cfgIdx;
            BOS_BOOL bAcptIncEmerg = BOS_FALSE;

            /* Make this session the focus session.
            */
            pSes->ses.med.bFocus = BOS_TRUE;

            /* Check to see if we're configure to auto connect incoming emergency
            ** calls. If so the emergency call would be accepted even if the user
            ** is offhook at the time.
            */
            if ( pSes->ses.med.bEmergency && pUsr->bOffHook )
            {

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                          &cfgIdx,
                                          (void *) &pEmergCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pEmergCfg &&
                       pEmergCfg->bEndCallAcptIncEmerg )
                  {
                     bAcptIncEmerg = BOS_TRUE;
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                        &cfgIdx,
                                        (void *) pEmergCfg,
                                        CFG_ITEM_DEL );
                  pEmergCfg = NULL;
               }
            }

            /* If required to accept the incoming emergency call when offhook, go into
            ** the ringing state and use the offhook handler to trigger call connection.
            */
            if ( bAcptIncEmerg )
            {
               cmUsrNewState ( pUsr, eCMST_RINGING );
               cmFsmOffHook ( (void *) pUsr );
            }
            /* Otherwise apply ringing */
            else
            {
               cmFsmRing ( pRes );
            }
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallXfer
**
** PURPOSE:    FSM call transfer request handler (on transferred-to party).
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallXfer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_BOOL bAccept = BOS_FALSE;
   CMSES *pSesOrig = NULL;
   CMSES *pSesXfer = NULL;

   CMCCTKACCNT* pCctkAccnt = NULL;

   /* To accept a transfer request, make sure we (the transferred-to party) already have
   ** a session(with the transferor) existing for the same user.
   */
   if ( pUsr->evt.pDat3 )
   {
      pSesOrig = cmSesFindNetHdl ( *( (BOS_UINT32 *) pUsr->evt.pDat3 ), pUsr->uHdl );

      if ( pSesOrig && ( pSesOrig->uUsr == pUsr->uHdl ) )
      {
         bAccept = BOS_TRUE;
      }
   }

   if ( !bAccept )
   {
      CMGRLOG_WARNING (( "cmFsmCallXfer: Fail accept call on 0x%X (0x%X)",
                         pUsr->uHdl, pUsr->evt.uDat4 ));

      if( ( cmNumSesWithCid( pUsr->evt.uDat2 ) == 1 ) &&
          ( pSesOrig && pSesOrig->uNetUsr != CMGR_INVALID_HANDLE ) )
      {
         /* Release the network call.
         */
         cctkCallDelete ( pUsr->evt.uDat2,
                          eCCTKCALLDELRSN_BUSY );
      }
   }
   else
   {
      /* Allocate a new session to handle transferred call. Make sure it
      ** doesn't exist already though.
      */
      if ( cmSesFindNetHdl ( pUsr->evt.uDat2, pUsr->uHdl ) == NULL)
      {
         pSesXfer = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl );
         if ( pSesXfer )
         {

            CMGRLOG_DEBUG (( "cmFsmCallXfer: Accepting 0x%X (0x%X) on 0x%X (0x%X)",
                             pSesXfer->uHdl, pUsr->evt.uDat2, pUsr->uHdl, pUsr->evt.uDat4 ));

            /* Switch the focus.
            */
            if ( pSesOrig == cmUsrFocusSes ( pUsr ) )
            {
               pSesOrig->ses.med.bFocus = BOS_FALSE;
            }
            pSesXfer->ses.med.bFocus = BOS_TRUE;
            pSesXfer->uNet = pUsr->evt.uDat2;
            pSesXfer->uNetUsr = pUsr->evt.uDat4;

            cmUsrLinkSes ( pUsr, pSesXfer );


            /* Update the media capabilities to be used for this call.
            */
            if( pSesXfer->uNetUsr != CMGR_INVALID_HANDLE )
            {
#if CMGRCFG_MULTILINE
               if ( !pSesOrig->bEstab )
               {
                  /* Blind transfer scenario. We cannot enable early media when
                  ** multiline is enabled. If we have to ring multiple handsets
                  ** , we can't setup a media session until one of the handsets
                  ** answers the call.
                  */
                  pSesXfer->ses.med.bNeedFinal = BOS_TRUE;
               }
               else
#endif
               {

                  cmCfgInitCallMedia ( pRes, pSesXfer );
                  cctkCallMediaNotify( pSesXfer->uNet, eCCTKMUDPRSN_FINALIZECAPS );
               }
            }

            /* If the original call with the transferor was answered, then accept
            ** (i.e. automatically answer) the call transfer.
            */
            if ( pSesOrig->bEstab )
            {
               CMGRLOG_DEBUG (( "cmFsmCallXfer: Answering 0x%X (0x%X) on 0x%X (0x%X)",
                             pSesXfer->uHdl, pSesXfer->uNet, pUsr->uHdl, pSesXfer->uNetUsr ));

               /* Set the new session to established as we will be answering this call.
               */
               pSesXfer->bEstab = BOS_TRUE;

               if( pSesXfer->uNetUsr != CMGR_INVALID_HANDLE )
               {
#if CMGRCFG_MULTILINE
                  cmCctkAccntCallAdd( pSesXfer->uNetUsr );
#endif
                  pCctkAccnt = cmCctkAccntFindByNet( pSesOrig->uNetUsr );
                  if ( gCmCfgCb.cmgrEvtCb )
                  {
                     ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                              pSesOrig->uNet,
#if CMGRCFG_MULTILINE
                                              pUsr->uHdl,
                                              (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                              (void *) pUsr->uUid );
                  }

                  /* External Call */
                  if ( cctkCallAnswer ( pSesXfer->uNet )  ==  eCCTKSTATUS_SUCCESS )
                  {
                     /* Pass on the provisioned privacy configuration of the local
                     ** user - may be needed for transfer scenarios.
                     */
                     CCTKPRIVCFG privCfg;
                     if ( cmFeatAssignPrivacy ( pRes, &privCfg ) )
                     {
                        cctkCallPrivInfo ( pSesXfer->uNet, (void *) &privCfg );
                     }
                  }
               }
            }
         }
      }

      /* Send Event via Client Callback with Remote User Identifier
      ** Note: Add Remote Party Number once we figure out where it can be
      ** obtained.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         pCctkAccnt = cmCctkAccntFindByNet( pSesOrig->uNetUsr );
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_XFER,
                                  pSesOrig->uApp,
#if CMGRCFG_MULTILINE
                                  pSesOrig->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) NULL );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmXferAccept
**
** PURPOSE:    FSM information that the transfer attempt originated by this
**             resource has been accepted.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This is *NOT* an indication of the status of the transfer execution
**             but only an indication of the initial request execution.
***********************************************************************************/
void cmFsmXferAccept ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMTMR tmr;
      CMGREPSIG sig;
#if CMGRCFG_MULTILINE
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */


      memset ( &tmr,
               0,
               sizeof ( CMTMR ) );

      memset ( &sig,
               0,
               sizeof ( CMGREPSIG ) );

      CMGRLOG_DEBUG (( "cmFsmXferAccept: 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* Terminate the ringback we may be generating locally.
      */
      if ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
      {
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = pSes->ses.med.uPhy;
         sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = eEPTSIG_RINGBACK;
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_RINGBACK;

         if ( cmTmrIsRunning ( &tmr ) )
         {
            cmTmrStop ( &tmr );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallError
**
** PURPOSE:    FSM received special error treatment.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This is applicable when specific error treatment information has
**             been received for a given outgoing call.
***********************************************************************************/
void cmFsmCallError ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMGREPSIG sig;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = (pSes) ? cmCctkAccntFindByNet( pSes->uNetUsr ) : NULL;
#endif /* CMGRCFG_MULTILINE */

   BOS_BOOL bFocus = BOS_FALSE;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCallError: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* Capture information about this call prior to releasing it.
      */
      bFocus = pSes->ses.med.bFocus;
      sig.uSig = cmPhyToneFromString ( pSes->ses.med.callInfo.cErrorInfo );

      /* An error condition for this call was reported, terminate this
      ** call now.
      */
      cmFsmCallEnded( pRes );

      /* If this was the focus call, see what the error information say we
      ** should do about that in terms of user action.
      */
      if ( bFocus && ( sig.uSig != CMGR_INVALID_HANDLE ) )
      {
         switch ( sig.uSig )
         {
            /* Be ready to place a new call right away.
            */
            case eEPTSIG_DIAL:
            case eEPTSIG_STUTTER:
            {
               cmFsmOutgoingCall ( pRes );
               sig.uSig = CMGR_INVALID_HANDLE;
            }
            break;

            /* Nothing more to do here, this is taken care of
            ** already by the framework.
            */
            case eEPTSIG_BUSY:
            case eEPTSIG_NULL:
            break;

            /* Re-assign to the default error handler in case we
            ** have received some unexpected tone that would not
            ** logically match the behavior to adopt in this case.
            */
            default:
            {
               sig.uSig = eEPTSIG_REORDER;
            }
            break;
         }

         /* Apply the tone of interest.
         */
         if ( sig.uSig != CMGR_INVALID_HANDLE )
         {
            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = pSes->uApp;
            sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
            sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmTargetsUpdate
**
** PURPOSE:    FSM information that the target has been updated.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       None.
***********************************************************************************/
void cmFsmTargetsUpdate ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CCTKTARGETINFO *pTargetInfo;
   CMGRTARGETINFO  cmgrTargetInfo;
   int             i;

   if ( ( pTargetInfo = (CCTKTARGETINFO *) pUsr->evt.pDat3 ) != NULL )
   {
      if ( gCmCfgCb.cmgrEvtCb )
      {
         /* Copy over targets and report to application */
         cmgrTargetInfo.uNumTargets = ( pTargetInfo->uNumTargets > CMGRCFG_TARGETS_MAX ) ?
                                       CMGRCFG_TARGETS_MAX : pTargetInfo->uNumTargets;

         for ( i = 0; i < cmgrTargetInfo.uNumTargets; i++ )
         {
            cmgrTargetInfo.uTargetPort[i] = pTargetInfo->uTargetPort[i];
            cmgrTargetInfo.targetipAddr[i] = pTargetInfo->targetipAddr[i];
         }

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_TARGETS_UPDATE,
                                  pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                  pUsr->uHdl,
                                  CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                  &cmgrTargetInfo);
      }
   }
}


#if CMGRCFG_PASS_THRU_MSG
/***********************************************************************************
** FUNCTION:   cmFsmMessage
**
** PURPOSE:    FSM received mesage indication.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
#define CM_FSM_MSG__RESPONSE_CODE_ACCEPTED      200
#define CM_FSM_MSG__RESPONSE_IMMEDIATE          0
void cmFsmMessage ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CCTKMSGPASSTHRU *pMsg = (CCTKMSGPASSTHRU *) pUsr->evt.pDat3;

   if ( pMsg )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmMessage: 0x%X on 0x%X (0x%X) - message %p (%u bytes)",
         pUsr->evt.uDat2, pUsr->uHdl, pUsr->evt.uDat4,
         pMsg->puData, pMsg->uSize ));
   }

   if ( gCmCfgCb.cmgrEvtCb )
   {
      CCTKMSGPASSTHRU msg;

      /* Note that the message content is not going to be
      ** valid beyond the callback scope, so the application should
      ** make a copy of it if processing is to take long time or if the
      ** message needs to be saved.
      **
      ** The application may issue the final response for this message
      ** within the context of the callback if it chooses to.
      */
      msg.puData = pMsg->puData;
      msg.uSize = pMsg->uSize;

      gCmCfgCb.cmgrEvtCb ( eCMGREVT_MESSAGE,
                           /* This handle is passed to the application when
                           ** the message is received, the application must
                           ** use the same handle to issue the final response
                           ** to this event.
                           */
                           pUsr->evt.uDat2,
#if CMGRCFG_MULTILINE
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                           (void *) &msg );
   }


#   if CM_FSM_MSG__RESPONSE_IMMEDIATE
   /* Respond to the message immediately from within the CMGR in this case.
   **
   ** Failure to send a final response from the application will lead to
   ** a resource leakage in CCTK and the core application.
   */
   cctkPassThruRsp ( pUsr->evt.uDat2,
                     CM_FSM_MSG__RESPONSE_CODE_ACCEPTED );
#   endif /* CM_FSM_MSG__RESPONSE_IMMEDIATE */
}


/***********************************************************************************
** FUNCTION:   cmFsmMessageRsp
**
** PURPOSE:    FSM received response to a previously sent message.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmMessageRsp ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   /* Log an event to show that we have received a response to an
   ** outstanding message.  There is no need for the CMGR to
   ** interact with the CCTK following this event (i.e. the resources
   ** at CCTK layer have now been cleared up).
   */
   CMGRLOG_INFORMATION ((
      "cmFsmMessageRsp: 0x%X on 0x%X (0x%X) - response code %u",
      pUsr->evt.uDat2, pUsr->uHdl, pUsr->evt.uDat4,
      *((BOS_UINT32 *) pUsr->evt.pDat3) ));

   if ( gCmCfgCb.cmgrEvtCb )
   {
      CMGRAPPCALLHDL hdl;

      /* The handle for this request (this is the CMGR handle that was
      ** published to the outside application when the message was sent).
      */
      hdl.uSesHdl = pUsr->evt.uDat2;
      /* This carries the final response code.
      */
      hdl.uAppIx = *((BOS_UINT32 *) pUsr->evt.pDat3);

      gCmCfgCb.cmgrEvtCb ( eCMGREVT_MSG_RSP,
                           pUsr->uHdl,
#if CMGRCFG_MULTILINE
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                           (void *) &hdl );
   }
}
#endif /* CMGRCFG_PASS_THRU_MSG */

/***********************************************************************************
** FUNCTION:   cmFsmOobDtmf
**
** PURPOSE:    FSM out-of-band DTMF (via signaling) received.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOobDtmf ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CCTKOOBDTMF *pOobDtmf = NULL;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

   if ( pSes &&
        pUsr->evt.pDat3 &&
        ( pOobDtmf = (CCTKOOBDTMF *) pUsr->evt.pDat3 ) != NULL )
   {
      BOS_UINT32 uEvt;
      CMGREPSIG sig;
      CMTMR tmr;

      memset ( &sig, 0, sizeof ( CMGREPSIG ) );
      memset ( &tmr, 0, sizeof ( CMTMR ) );

      CMGRLOG_INFORMATION ((
         "cmFsmOobDtmf: DTMF %d (%d msec) for 0x%X (0x%X) on 0x%X (0x%X)",
         pOobDtmf->uDtmf, pOobDtmf->uDuration,
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* Map the DTMF digit to a Call Manager event */
      uEvt = cmMapGetEvtFromChar( cmMapEptSig2DigCol, pOobDtmf->uDtmf );

      sig.eAct = eCMGREPSIGACT_START;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = pSes->uApp;
      sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = uEvt;
      pUsr->uCpt = sig.uSig;

      cmPhySignal ( pUsr->uPhys, &sig );

      tmr.uSes = CMGR_INVALID_HANDLE;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_DTMF;
      tmr.uTimeOut = (pOobDtmf->uDuration) ? pOobDtmf->uDuration : CMGR_DEFAULT_DTMF_DURATION;

      cmTmrStart ( &tmr );

   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCwtInfo
**
** PURPOSE:    FSM Call Waiting Tone info (via signaling) received.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCwtInfo ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CCTKCWTINFO *pCwtInfo = (CCTKCWTINFO *) pUsr->evt.pDat3;
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   CFGSIPCALLWAIT *pUsrCallWait = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bClidName = BOS_FALSE;
   BOS_BOOL bClidNumber = BOS_FALSE;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */
   BOS_UINT8    uSize, i;
   BOS_UINT8    uInfoMsgBuf[CCTK_STR_LEN];
   BOS_UINT8* puCwt;

   if ( pSes && pCwtInfo )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCwtInfo: CwtInfo for 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      CMGREPSIG sig;
      CMTMR tmr;
      BOS_UINT32 uIx;
      BOS_UINT32 uEvt;
      BOS_UINT8 uCallIdent[ CMGR_CLID_LEN + 1];

      memset ( &sig, 0, sizeof ( CMGREPSIG ) );
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      memset ( &tmr, 0, sizeof ( CMTMR ) );
      memset ( uCallIdent, 0, sizeof ( uCallIdent ));

      /* Currently, two types of INFO Call Waiting Tone are supported:
      **   1) Via the Event Header using "start-CWT" or "stop-CWT"
      **   2) Via the Content as follows:
      *              play tone <ToneName>
      *              Calling-Name: <Name>      [optional]
      *              Calling-Number:<Number>   [optional]
      *                 where <ToneName> = CallWaitingTone1, CallWaitingTone2
      *                                    CallWaitingTone3, CallWaitingTone4
      *
      *              stop CallWaitingTone
      */

      /* since the data field (Event header value or Content) of the INFO
      ** message is case-insensitive, we need to convert it to lower case
      ** before evaluating the message */
      puCwt = pCwtInfo->cwt;
      uSize = strlen( (char*) pCwtInfo->cwt );
      for ( i = 0; i < uSize; i++, puCwt++ )
      {
         uInfoMsgBuf[i] = (BOS_UINT8)tolower((BOS_UINT8)*puCwt);
      }

      /* Check if this is a stop cw tone command via the Event header or the Content */
      if (strstr( (char *)uInfoMsgBuf, CM_FSM__STOP_CWT_DATA ) ||
          strstr( (char *)uInfoMsgBuf, CM_FSM__STOP_TONE_DATA ) )
      {
         /* Stop the CW tone */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = pUsr->uCpt;
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );
         /* we are done here */
         return;
      }

      /* if it wasn't a stop cw tone INFO message, it is a start cw tone INFO message */
      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.uTimeOut = CMGR_CALL_WAIT_TIMEOUT;
      tmr.eType = eCMEVT_TMR_CALLWAIT;

      cfgIdx.pcCharIdx = pUsr->uUid;

      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                 &cfgIdx,
                                 (void *) &pUsrClidDsp,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pUsrClidDsp != NULL )
         {
            bClidNumber = pUsrClidDsp->bCNDActStat;
            bClidName = pUsrClidDsp->bCNAMDActStat;

            /* Special case of call waiting CLID overwrite to prevent
            ** CLID information to be sent with the call waiting signal.
            */
            if ( pUsrClidDsp->bCIDCWBlock )
            {
               bClidNumber = BOS_FALSE;
               bClidName = BOS_FALSE;
            }
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                               &cfgIdx,
                               (void *) pUsrClidDsp,
                               CFG_ITEM_DEL );
         pUsrClidDsp = NULL;
      }

      sig.eAct = eCMGREPSIGACT_START;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = pSes->uApp;
      sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = CMGR_INVALID_HANDLE;
      sig.pData = &uCallIdent;

      /* now parse the data to determine the cw tone and whether
      ** caller id information is provided.
      ** For "start-CWT" from the Event header, no call-waiting tone nor
      ** caller id information is specified, so the default will be used.
      ** For "play tone callwaitingX" from the Content, we will need to parse
      ** out the actual callwaiting tone, and check if caller id is present
      */
      if (strstr( (char *)uInfoMsgBuf,CM_FSM__START_CWT_DATA ))
      {
         /* no call waiting tone is specified for "start-CWT", set it to
         ** default of eEPTSIG_CALLW1
         */
         sig.uSig = eEPTSIG_CALLW1;

         /* no call id information is provided for this type as well */
         if ( bClidName || bClidNumber )
         {
            memset ( &pSes->ses.med.callInfo.clid,
                     0,
                     sizeof ( CCTKCLIDINFO ) );
            memcpy ( &pSes->ses.med.callInfo.clid,
                     &defaultClid,
                     sizeof ( CCTKCLIDINFO ) );
         }
      }
      else
      {
         BOS_UINT8 uBuf[CCTK_STR_LEN+1];
         puCwt = (BOS_UINT8*)strstr( (char *)uInfoMsgBuf, CM_FSM__PLAY_TONE_DATA );
         if ( puCwt == NULL )
         {
            /* if "play tone" is not found, then nothing else can be done as this is
            ** not a valid call waiting INFO message
            */
            return;
         }

         /* otherwise, if the "play tone" is found, then parse the rest of the content:
         ** the format of the content would be:
         **     play tone <ToneName>
         **     Calling-Name: <Name>      [optional]
         **     Calling-Number:<Number>   [optional]
         **         where <ToneName> = CallWaitingTone1, CallWaitingTone2
         **                            CallWaitingTone3, CallWaitingTone4
         */

         /* parse the tone */
         puCwt += strlen( CM_FSM__PLAY_TONE_DATA );

         memset(&uBuf[0], 0, CCTK_STR_LEN+1);
         if ( strSScanf( (char*)(puCwt), "%S",
                         (char*)(&uBuf[0]), CCTK_STR_LEN ) == 0 )
         {
            /* Failed to retrieve the tone string.  Nothing more to do */
            return;
         }

         /* Extract tone information from the parsed tone name */
         sig.uSig = cmMapGetIdFromStr ( cmCwToneMap, uBuf );

         if ( sig.uSig == CMGR_INVALID_HANDLE )
         {
            /* Unrecognized tone.  Nothing more to do */
            return;
         }

         /* next parse out the caller id info if necessary and present */
         if ( bClidName || bClidNumber )
         {
            memset ( &pSes->ses.med.callInfo.clid,
                     0,
                     sizeof ( CCTKCLIDINFO ) );
            memcpy ( &pSes->ses.med.callInfo.clid,
                     &defaultClid,
                     sizeof ( CCTKCLIDINFO ) );

            if ( bClidName )
            {
               puCwt = strstr( (char *)(uInfoMsgBuf), CM_FSM__CALLING_NAME_DATA );
               if ( puCwt )
               {
                  puCwt += strlen( CM_FSM__CALLING_NAME_DATA );

                  /* Extract the name after the Calling-Name tag.  To preserve case
                  ** sensitivity, we will extract the name from the original INFO
                  ** data message.  Locate the double quotes that identifies the name */
                  char       *pName, *pNameStart, *pNameEnd;
                  pNameStart = strchr( puCwt, '"' );
                  pNameEnd = strchr( pNameStart+1, '"' );

                  if ( pNameStart && pNameEnd )
                  {
                     /* the double quotes should be stripped from the calling-name */
                     pNameStart = pNameStart + 1;
                     pNameEnd = pNameEnd - 1;

                     uSize = pNameEnd - pNameStart + 1;
                     pName = pCwtInfo->cwt + (BOS_UINT8)(pNameStart - (char)uInfoMsgBuf);
                     memcpy( &pSes->ses.med.callInfo.clid.cName, pName,
                              (uSize > CCTK_CLID_LEN) ? CCTK_CLID_LEN : uSize);
                     if (uSize > CCTK_CLID_LEN)
                     {
                        pSes->ses.med.callInfo.clid.cName[CCTK_CLID_LEN+1] = '\0';   
                     }
					 else
                     {
                        pSes->ses.med.callInfo.clid.cName[uSize+1] = '\0';   
                     }
                  }
               }
            }

            if ( bClidNumber )
            {
               /* Extract the phone number from the message */
               puCwt = strstr( (char *)uInfoMsgBuf, CM_FSM__CALLING_NUM_DATA );
               if ( puCwt )
               {
                  puCwt += strlen( CM_FSM__CALLING_NUM_DATA );
                  memset(&uBuf[0], 0, CCTK_STR_LEN+1);
                  if ( strSScanf( (char*)(puCwt), "%S",
                                  (char*)(&uBuf[0]), CCTK_CLID_LEN ) )
                  {
                     memcpy( &pSes->ses.med.callInfo.clid.cNumber, uBuf, strlen((char*)uBuf));
                  }
               }
            }
         }
      }

      sig.uSig = cmPhyToneWithClid ( sig.uSig, ( bClidName || bClidNumber ) );
      pUsr->uCpt = sig.uSig;

      /* Insert a unique identifier for this call by means of the
      ** calling number.  Note that this information may contain no
      ** data of interest, it is just there for the application to make
      ** use of it as needed.
      */
      sprintf ( (char *) uCallIdent,
                "%s",
                pSes->ses.med.callInfo.clid.cNumber );

      cmPhySignal ( pUsr->uPhys, &sig );

      /* Issue caller identification information if necessary.
      */
      if ( bClidName || bClidNumber )
      {
         cmSesClid ( pSes, bClidName, bClidNumber );
      }
   }
}



/***********************************************************************************
** FUNCTION:   cmFsmCdrInfo
**
** PURPOSE:    FSM call data record published from CCTK layer.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCdrInfo ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMCCTKACCNT *pCctkAccnt = NULL;
   CCTKCDRINFO *pCdrInfo = NULL;
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bClidName = BOS_TRUE;
   BOS_BOOL bClidNumber = BOS_TRUE;

   /* Grab the CDR information and the CCTK line number from the handle of the
    * CMUSR. This will be used as an index for the CDR information.
    */
   if ( pUsr->evt.pDat3 &&
      ( pCdrInfo = (CCTKCDRINFO *) pUsr->evt.pDat3 ) != NULL  &&
      ( pCctkAccnt = cmCctkAccntFindByNet( pUsr->evt.uDat4 ) ) != NULL )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCdrInfo: cdr information for 0x%X (0x%X), pCctkAccnt uHdl: [%X], pCctkAccnt uNet: [%X]",
         pUsr->uHdl, pUsr->evt.uDat4, pCctkAccnt->uHdl , pCctkAccnt->uNet));

      /* Caller-id info and time in the cdr needs to be reformatted according
      ** to configuration so that it aligns with how the caller-id displayed
      ** this info at the time of the call.
      */

      /* Determine the information that can be displayed.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                 &cfgIdx,
                                 (void *) &pUsrClidDsp,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         bClidNumber = pUsrClidDsp->bCNDActStat;
         bClidName = pUsrClidDsp->bCNAMDActStat;

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                               &cfgIdx,
                               (void *) pUsrClidDsp,
                               CFG_ITEM_DEL );
         pUsrClidDsp = NULL;
      }

      /* Clear the caller-id info if configuration doesn't allow it.
      */
      if ( !bClidNumber )
      {
         memset( pCdrInfo->cdr.clid.cNumber, 0,  sizeof ( pCdrInfo->cdr.clid.cNumber ) );
      }

      if ( !bClidName )
      {
         memset( pCdrInfo->cdr.clid.cName, 0,  sizeof ( pCdrInfo->cdr.clid.cName ) );
      }

      /* If a callback is registered, publish this event out, the data passed during
      ** the callback is only valid within the scope of the callback.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         gCmCfgCb.cmgrEvtCb ( eCMGREVT_CALL_STATS,
                              pUsr->uHdl,
#if CMGRCFG_MULTILINE
                              pUsr->uHdl,
                              pCctkAccnt->uHdl,
#endif /* CMGRCFG_MULTILINE */
                              (void *) pCdrInfo );
      }
   }
}

#if CMGRCFG_REMOTE_USER_CONTROL
/***********************************************************************************
** FUNCTION:   cmFsmUpdateRemoteUsr
**
** PURPOSE:    FSM add this remote user.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmUpdateRemoteUsr ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   if ( pUsr && pUsr->eType == eCMUSRTYPE_REMOTE)
   {
      CMUSR *pLocalUsr;
      BOS_UINT32 uAssociatedUserId;
      BOS_UINT32 uIMPIUserId;
      BOS_BOOL bDynCreated;
      CFGSIPREMOTEUSERCFG *pUsrCfg;
      PROV_CFG_IDX cfgIdx;
      CFGSIPREMOTEUSERSYSCFG *pCfg;
      CCTKREMUSRINFO *pRemUsrInfo;

      CMEVT *pEvt = &(pUsr->evt);

      /* Unlink local user if we have one */
      CMUSR *pLinkedLocalUsr = cmUsrValid( pUsr->uLinkedLocalUserHdl );
      if ( pLinkedLocalUsr )
      {
         cmUsrUnlinkRemoteUser( pLinkedLocalUsr, pUsr->uHdl );
      }
      cmUsrLinkLocalUser( pUsr, CMGR_INVALID_HANDLE );

      /* Find out the base user to associate this new user with.  We always
      ** assume that the first user configured on the device is the user for
      ** which we want to allow allocation and control of remote users.
      */
      pLocalUsr = NULL;
      uAssociatedUserId = 0;
      uIMPIUserId = 0;
      bDynCreated = BOS_TRUE;

      /* look for pre-config configuration or previously created configuration.
      */
      pUsrCfg = NULL;
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserEntryConcept),
                                 &cfgIdx,
                                 (void *) &pUsrCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         uAssociatedUserId = pUsrCfg->uAssociatedIMPUIndexRef;
         uIMPIUserId = pUsrCfg->uIMPIIndexRef;
         bDynCreated = pUsrCfg->bDynCreated;
         pLocalUsr = cmUsrFindUidHdl ( pUsrCfg->pcAssociatedUserId );
         gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserEntryConcept),
                               &cfgIdx,
                               (void *) pUsrCfg,
                               CFG_ITEM_DEL );
      }


      if ( pLocalUsr == NULL )
      {
         /* Warning, previously created configuration is invalid */

         /* Get current default configuration. */
         pCfg = NULL;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                                    NULL,
                                    (void *) &pCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            pLocalUsr = cmUsrFindUidHdl ( pCfg->pcDftAssociatedUserId );
            gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                                  NULL,
                                  (void *) pCfg,
                                  CFG_ITEM_DEL );
         }

         /* If associated local user can not be found, use the first
         ** in-service local user in CMGR as the associated local user */
         if ( pLocalUsr == NULL )
         {
            pLocalUsr = cmUsrFirstLocalUser ();
         }
      }

      pRemUsrInfo = (CCTKREMUSRINFO *)pEvt->pDat3;

      /* Mark this user as a remote user, mark this user in service.
      */
      pUsr->eType = eCMUSRTYPE_REMOTE;
      pUsr->bInSrv = BOS_TRUE;

      /* Linked the new user and the base user */
      cmUsrLinkRemoteUser( pLocalUsr, pUsr->uHdl );
      cmUsrLinkLocalUser( pUsr, pLocalUsr->uHdl );

      {
         BOS_BOOL bFailed = BOS_TRUE;
         /* TO_DO: Cxc 5.0.4 stream doesn't allow new user to be created
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                    &cfgIdxNewUsr,
                                    NULL,
                                    CFG_ITEM_SET ) == BOS_STATUS_OK )
         */
         {
            CFGSIPREMOTEUSERCFG remUsrCfg;
            memset ( &remUsrCfg, 0, sizeof ( CFGSIPREMOTEUSERCFG ) );

            CMGRLOG_INFORMATION (( "cmCctkCmdHandler: new user (%s) config",
                                   pUsr->uUid ));

            /* Save the remote user target credentials, needed to send request
            ** to this remote user.
            */
            /* This requests the Application Prov setting to add/update a SIP
            ** remote user */
            remUsrCfg.pcUserId = pUsr->uUid;
            remUsrCfg.pcAddr = pRemUsrInfo->uUsrSigHost;
            remUsrCfg.uSipPort = pRemUsrInfo->uUsrSigPort;
            remUsrCfg.uAssociatedIMPUIndexRef = uAssociatedUserId;
            /* This value gets updated by application */
            /* remUsrCfg.pcAssociatedUserId = pLocalUsr->uUid; */

            remUsrCfg.uIMPIIndexRef = uIMPIUserId;
            /* These values get updated by application */
            /*
            remUsrCfg.pcPrivateUserId = NULL;
            remUsrCfg.credentials.CredType = CFG_SIP_USER_CRED_TYPE_NONE;
            remUsrCfg.credentials.CredValue = NULL;
            */

            remUsrCfg.bDynCreated = bDynCreated;
            remUsrCfg.pNext = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                                  NULL,
                                  (void *) &remUsrCfg,
                                  CFG_ITEM_SET ) == BOS_STATUS_OK )
            {
               CMGRLOG_INFORMATION (( "cmCctkCmdHandler: remote %s@%s:%d ",
                                      pUsr->uUid,
                                      pRemUsrInfo->uUsrSigHost,
                                      pRemUsrInfo->uUsrSigPort ));
               bFailed = BOS_FALSE;
            }
         }

         if ( bFailed )
         {
            CMGRLOG_CRITICAL (( "cmCctkCmdHandler: failed new remote user (%s) config",
                                pUsr->uUid ));
         }
      }
   }
   else
   {
      CMGRLOG_WARNING ((
         "cmFsmUpdateRemoteUsr: add non-remote user 0x%X (uHdl:0x%X) (uNet:0x%X)",
         pUsr, pUsr? pUsr->uHdl: 0, pUsr? pUsr->uNet: 0 ));
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmRemoveRemoteUsr
**
** PURPOSE:    FSM remove this remote user.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRemoveRemoteUsr ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   if ( pUsr && pUsr->eType == eCMUSRTYPE_REMOTE)
   {
      BOS_BOOL bRemoveAll;
      CFGSIPREMOTEUSERCFG *pRemUsrCfg;
      PROV_CFG_IDX cfgIdx;

      /* Unlink local user */
      CMUSR *pLinkedLocalUsr = cmUsrValid( pUsr->uLinkedLocalUserHdl );
      if ( pLinkedLocalUsr )
      {
         cmUsrUnlinkRemoteUser( pLinkedLocalUsr, pUsr->uHdl );
      }
      cmUsrLinkLocalUser( pUsr, CMGR_INVALID_HANDLE );

      bRemoveAll = BOS_TRUE;

      pRemUsrCfg = NULL;
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;

      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                                 &cfgIdx,
                                 (void *) &pRemUsrCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pRemUsrCfg->bDynCreated == BOS_FALSE )
         {
            bRemoveAll = BOS_FALSE;
         }
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                                 &cfgIdx,
                                 (void *) pRemUsrCfg,
                                 CFG_ITEM_DEL );
         pRemUsrCfg = NULL;
      }

      {
         /* Set NULL to clear the Prov setting */
         CMGRLOG_INFORMATION (( "cmFsmRemoveRemoteUsr: removing user (%s) config",
                                pUsr->uUid ));
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                               &cfgIdx,
                               NULL,
                               CFG_ITEM_SET );
      }

      /* Only remove the CMGR/CCTK USER if it is a dynamically created user */
      if ( bRemoveAll )
      {
         pUsr->bBeRem = BOS_TRUE;
         pUsr->eCfgAction = eCMCFGACT_CFGUSRTERM;
         cctkUserRemove ( pUsr->uNet );
      }
   }
   else
   {
      CMGRLOG_WARNING ((
         "cmFsmRemoveRemoteUsr: remove non-remote user 0x%X (uHdl:0x%X) (uNet:0x%X)",
         pUsr, pUsr? pUsr->uHdl: 0, pUsr? pUsr->uNet: 0 ));
   }
}
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

/***********************************************************************************
** FUNCTION:   cmFsmPublishTarget
**
** PURPOSE:    FSM publish target handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmPublishTarget ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_IP_ADDRESS *pPublishTarget;

   /* Always inform the application of the recent publish target */
   if ( ( pPublishTarget = (BOS_IP_ADDRESS *) pUsr->evt.pDat3 ) != NULL )
   {
      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_PUBLISH_TARGET,
                                  pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                  pUsr->uHdl,
                                  CMGR_INVALID_HANDLE,
#endif
                                  pPublishTarget );
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmSubStatus
**
** PURPOSE:    FSM subscription status handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmSubStatus ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_BOOL bSubEnabled = *(BOS_BOOL *) pUsr->evt.pDat3;

      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( bSubEnabled ? eCMGREVT_SUB_OK : eCMGREVT_SUB_NOK,
                                  pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                  pUsr->uHdl,
                                  CMGR_INVALID_HANDLE,
#endif
                                 (void *) pUsr->uUid);
      }

}

