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
**      This is the FSM for physical interface (endpoint) events.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CM_FSM_PHY__DTMF_INFO__DURATION_MSEC    250

/* ---- Private Variables ------------------------------------------------------- */

/* The finite state machine for handling the Physical Interface (endpoint) events.
*/
static const CMFSMVOIDFUNCTIONPTR
   gpCmPhyFsm[eCMST_MAX][eCMEVT_PHY_END - eCMEVT_PHY_START - 1] =
{
   /* ---- eCMST_IDLE ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmOffHook,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmEndSvc,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmInvalid,
   },

   /* ---- eCMST_ACCEPTED ------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmOffHook,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmNoOp,
   },

   /* ---- eCMST_DIALING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmFlash,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmDigitDetect,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmAudio,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmAudioDelayed,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmAudioEnd,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmInternalCallSvc,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmEndSvc,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmNoOp,
   },

   /* ---- eCMST_RINGBACK ------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmFlash,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmFaxCall,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmFaxEnd,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmFaxFail,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmSesStatus,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmAudio,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmAudioDelayed,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmAudioEnd,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmBxferSvc,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmCxferSvc,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmEndSvc,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmInvalid,
   },

   /* ---- eCMST_RINGING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmOffHook,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmFlash,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmSesStatus,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmAnsSvc,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmEndSvc,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmInvalid,
  },

   /* ---- eCMST_TALK ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmFlash,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmModemCall,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmFaxCall,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmFaxEnd,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmFaxFail,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmDigitRelay,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmSesStatus,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmNoActivity,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmHoldSvc,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmUnHoldSvc,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmConfSvc,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmBxferSvc,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmCxferSvc,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmAnsSvc,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmEndSvc,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmDtmfPtSvc,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmIngressDtmfSvc,
   },

   /* ---- eCMST_FLASH ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmFlash,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmFeature,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmFeature,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmSesStatus,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmNoActivity,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmHoldSvc,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmUnHoldSvc,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmConfSvc,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmBxferSvc,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmCxferSvc,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmEndSvc,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmDtmfPtSvc,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmIngressDtmfSvc,
   },

   /* ---- eCMST_RELEASING ------------------------------------------------------ */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmOffHook,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmFlash,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmCancelKeyTmr,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmInvalid,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmInvalid,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmEndSvc,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmInvalid,
   },

   /* ---- eCMST_ERROR ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmErrorFlash,
      /* ---- eCMEVT_MODEM_TONE ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_TONE ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_END -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_FAX_FAIL ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_0 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_1 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_2 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_3 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_4 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_5 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_6 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_7 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_8 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_9 --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_STAR ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_HASH ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_A --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_B --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_C --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMF_D --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmNoOp,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmNoOp,
   },
};


/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmFsmPhyRun
**
** PURPOSE:    Main entry for running the PHY FSM.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmPhyRun ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   BOS_UINT32 uSta;
   BOS_UINT32 uEvt;

   uSta = pUsr->eSta[ CMFSM_CURRENT_STATE_IX ];
   uEvt = pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ];

   CMGRLOG_INFORMATION (( "cmFsmPhyRun: usr 0x%X - [%s (0x%X), %s (0x%X)]",
                          pUsr->uHdl,
                          (char *) cmMapGetStr ( cmMapCmState, uSta ), uSta,
                          (char *) cmMapGetStr ( cmMapCmEvent, uEvt ), uEvt ));

   if ( uEvt != eCMEVT_INVALID )
   {
      (gpCmPhyFsm [ uSta ][ uEvt - (eCMEVT_PHY_START + 1) ]) ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmOnHook
**
** PURPOSE:    FSM on hook handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOnHook ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes;
   CMSES *pOtherSes;
   CMGREPSIG sig;
   CMTMR tmr;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGSIPEMERGE *pEmergencyCfg;
   CFGSIPCALLXFER *pXferCfg;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bXfer = BOS_FALSE;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   CMGRLOG_INFORMATION (( "cmFsmOnHook: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.uUsr = pUsr->uHdl;

   /* Terminate warmline timer if running.
   */
   tmr.eType = eCMEVT_TMR_WARMLINE;
   if ( cmTmrIsRunning ( &tmr ) )
   {
      cmTmrStop ( &tmr );
   }

   /* Terminate off-hook warning timer if we were running it.
   */
   tmr.eType = eCMEVT_TMR_OHWARN;
   cmTmrStop ( &tmr );

   /* Terminate osi timer if we were running it.
   */
   tmr.eType = eCMEVT_TMR_OSI_DELAY;
   cmTmrStop ( &tmr );

   /* Terminate auto-recover timer if we were running it.
   */
   tmr.eType = eCMEVT_TMR_AUTORECOVER;
   if ( cmTmrIsRunning ( &tmr ) )
   {
      /* If the auto-recover timer was running, there will be
      ** a focus session on which the timer would have acted.
      **
      ** In order to process the rest of the event properly
      ** in this situation we remove the focus from that session
      ** and we move to a more appropriate state.
      **
      ** The net result will be that the user should be rung back
      ** to pick up the waiting call (which will be promoted to
      ** being the focus session again).
      */
      if ( ( pSes = cmUsrFocusSes ( pUsr ) ) != NULL )
      {
         pSes->ses.med.bFocus = BOS_FALSE;
         cmUsrNewState ( pUsr, eCMST_RELEASING );
      }
   }
   cmTmrStop ( &tmr );

   /* Report the hook event to the application in case it needs it.
   */
   if ( gCmCfgCb.cmgrEvtCb )
   {
      BOS_UINT8* pDig = NULL;
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( (pBasicCall->bReportPartialDS) &&
              (pUsr->eDigDetState == CMDIGITDETSTA_INPROGRESS) &&
              ( (pSes = cmUsrFocusSes(pUsr)) != NULL ) )
         {
            /* make sure digits are actually collected */
            if (pSes->ses.med.uDigCnt > 0)
            {
               pDig = pSes->ses.med.uDigCol;
            }
         }
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }
      gCmCfgCb.cmgrEvtCb ( eCMGREVT_ONHOOK,
                           pUsr->uPhys,
#if CMGRCFG_MULTILINE
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                           pDig ? (void*)pDig : NULL);
   }

   pUsr->bOffHook = BOS_FALSE;
   pUsr->bPerCallCW = BOS_TRUE;
#if CMGRCFG_MULTILINE
   pUsr->bDisableAutoCallSwitch = BOS_FALSE;
   pUsr->bAlarmActive = BOS_FALSE;
#endif /* CMGRCFG_MULTILINE */

   if ( (pUsr->bPendingMwi == BOS_TRUE) && (pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_ERROR) )
   {
      CMGREPSIG sigMwi;

      /* Signal visual message waiting indication to the physical interface
       ** of interest associated with this user.
       */
      memset ( (void *) &sigMwi,
            0,
            sizeof ( CMGREPSIG ) );

      sigMwi.eAct = ( pUsr->bMwi ?
            eCMGREPSIGACT_START : eCMGREPSIGACT_STOP );
      sigMwi.uSes = CMGR_INVALID_HANDLE;
      sigMwi.uApp = CMGR_INVALID_HANDLE;
      sigMwi.uSig = eEPTSIG_VMWI;

      cmPhySignal ( pUsr->uPhys, &sigMwi );
      pUsr->bPendingMwi = BOS_FALSE;
   }

   /* If we were in error state, that is due to a prolong period of off-hook.
   ** Need to check what to do based on where we are at with the error indication
   ** sequence to the user.
   */
   if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_ERROR )
   {
      /* Terminate any possible signal being played (like off-hook warning).
      */
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;
      pUsr->uCpt = eEPTSIG_NULL;

      if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
      {
         cmPhySignal ( pUsr->uPhys, &sig );
      }

      /* If there is still a session in permanent hold, terminate it and apply
      ** ring to the local user.
      */
      if ( pUsr->pSesHeld )
      {
         pUsr->pSesHeld->ses.med.eAction = eCMDM_UNKNOWNACTION;
         pUsr->pSesHeld = NULL;
         pUsr->bPresrvHeld = BOS_FALSE;

         cmFsmOnHookGuard ( pRes );
      }
      else
      {
         BOS_BOOL bLockOut = BOS_FALSE;

         if ( pUsr->eError == eCMRESERROR_LOCKOUT )
         {
            /* We did reach the lock out state, we need to start a timer prior to going
            ** back to active.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               tmr.uTimeOut = pBasicCall->uLORTimer;

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_DEL );
               pBasicCall = NULL;
            }
            else
            {
               tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;
            }

            if ( tmr.uTimeOut )
            {
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_OHWARN;

               cmTmrStart ( &tmr );

               /* Lock out is applicable.
               */
               bLockOut = BOS_TRUE;
            }
         }

         if ( !bLockOut )
         {
            /* We did not reached the lock out state yet or the lock out
            ** period was not applicable, so we go back to active/idle state.
            */
            if ( pUsr->bPendingMwi == BOS_TRUE )
            {
               CMGREPSIG sig;

               /* Signal visual message waiting indication to the physical interface
                ** of interest associated with this user.
                */
               memset ( (void *) &sig,
                     0,
                     sizeof ( CMGREPSIG ) );

               sig.eAct = ( pUsr->bMwi ?
                     eCMGREPSIGACT_START : eCMGREPSIGACT_STOP );
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
               sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
               sig.uSig = eEPTSIG_VMWI;

               cmPhySignal ( pUsr->uPhys, &sig );
               pUsr->bPendingMwi = BOS_FALSE;
            }

            cmUsrNewState ( pUsr, eCMST_IDLE );

            /* Publish a 'line inactive' event.
            */
            if ( gCmCfgCb.cmgrEvtCb &&
                 (pUsr->eType != eCMUSRTYPE_REMOTE) )
            {
               gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_INACTIVE,
                                    pUsr->uPhys,
#if CMGRCFG_MULTILINE
                                    pUsr->uHdl,
                                    CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                    NULL );
            }

            /* If we are pending on an action, don't release
            ** the resource.
            */
            if ( pUsr->bPendingActPhy == BOS_FALSE )
            {
               cmUsrPhyDisassociate( pUsr );
               cmUsrCctkAccntDisassociate( pUsr );
            }
         }
      }
   }
   /* If there is a focus and established session, ensure we run proper disconnect
   ** procedures on it (including special handling for emergency call and on-hook
   ** guard timing).
   */
   else if ( ( pSes = cmUsrFocusSes ( pUsr ) ) != NULL )
   {
#if CMGRCFG_MULTILINE
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

      /* Terminate any possible signal being played (like off-hook warning).
      */
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;
      pUsr->uCpt = eEPTSIG_NULL;

      if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
      {
         cmPhySignal ( pUsr->uPhys, &sig );
      }

      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_INVALID;

      cmTmrStop ( &tmr );

      tmr.uTimeOut = 0;
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      /* If this is an emergency session and 'network hold' is supported, apply
      ** the mechanism now.  If network hold is not supported, we would apply the
      ** default on-hook guard timing.
      */
      if ( pSes->bEstab && pSes->ses.med.bEmergency )
      {
         /* Terminate emergency howler timer if we were running it.
         */
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_EMERG_HOWLER;

         cmTmrStop ( &tmr );

         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                    &cfgIdx,
                                    (void *) &pEmergencyCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pEmergencyCfg &&
                 pEmergencyCfg->uNwHoldTimer &&
                 !pEmergencyCfg->bNwHoldDisable )
            {
               CCTKHOLDCFG holdCfg;
               BOS_BOOL bLocalHold = BOS_FALSE;
               BOS_BOOL bBypassHold = BOS_FALSE;

               tmr.uSes = pSes->uHdl;
               tmr.uTimeOut = pEmergencyCfg->uNwHoldTimer;
               tmr.eType = eCMEVT_TMR_NETWORK_HOLD;

               cmTmrStart ( &tmr );

               /* Check whether local hold (i.e. hold is applied without network
               ** signaling) is enabled for this user.  If so, we need to disable
               ** 'local hold' in order to issue the signaling to the network in
               ** this particular case of 'network hold' for emergency call (this
               ** takes precedence over local hold setting).
               */
               if ( cctkGetParm ( pSes->uNet,
                                  eCCTKPARM_EC_CALLHOLD,
                                  (void *) &holdCfg ) == eCCTKSTATUS_SUCCESS )
               {
                  if ( holdCfg.bLocalHold )
                  {
                     bLocalHold = BOS_TRUE;
                     holdCfg.bLocalHold = BOS_FALSE;
                  }

                  if ( !holdCfg.bBypassHoldRspChk &&
                       pEmergencyCfg->bNwHoldBypassChk )
                  {
                     bBypassHold = BOS_TRUE;
                     holdCfg.bBypassHoldRspChk = BOS_TRUE;
                  }

                  /* Temporary overwrite of the configuration.
                  */
                  if ( bLocalHold || bBypassHold )
                  {
                     cctkSetParm ( pSes->uNet,
                                   eCCTKPARM_EC_CALLHOLD,
                                   (void *) &holdCfg );
                  }
               }

               /* Hold the session.
               */
               cmUsrNewState ( pUsr, eCMST_IDLE );
               cctkCallHold ( pSes->uNet );

               /* If the emergency session was in a 3-way conference with another
               ** session, terminate the latter.
               */
               pOtherSes = cmUsrActiveSes( pUsr );
               if ( pOtherSes && ( pOtherSes->ses.med.uConf == pSes->uHdl ) )
               {
                  cmSesTerminate ( pOtherSes );
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                  &cfgIdx,
                                  (void *) pEmergencyCfg,
                                  CFG_ITEM_DEL );
            pEmergencyCfg = NULL;
         }
      }
      /*
      ** If no timeout was set, it means we have not gone through the emergency
      ** network hold procedure, in this case, continue with normal call release
      ** procedure.
      */
      if ( !tmr.uTimeOut )
      {
         pOtherSes = cmUsrActiveSes( pUsr );

         /* Check if we are the anchor in a conference call. If so, we need to tear
         ** down the second call as well.
         */
         if( pOtherSes &&
             (pOtherSes->ses.med.uConf == pSes->uHdl) )
         {
            cmSesTerminate ( pOtherSes );
         }
         /* Check whether we should apply call transfer to this session.  In order
         ** to apply call transfer, we need to have another valid active session,
         ** which is not the session used for call waiting or the session used for
         ** call hold.
         */
         else if ( pOtherSes &&
              ( pOtherSes != pUsr->pSesWait ) &&
              /* Should not be needed, since we queried the 'active' session. */
              ( pOtherSes != pUsr->pSesHeld ) )
         {
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                       &cfgIdx,
                                       (void *) &pXferCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pXferCfg &&
                    (pSes->uNet != CMGR_INVALID_HANDLE) &&
                    (pOtherSes->uNet != CMGR_INVALID_HANDLE) &&
                     /* Make sure we do not attempt to transfer an emergency
                     ** session
                     */
                    !pSes->ses.med.bEmergency &&
                     /* and make sure we do not attempt to transfer an outgoing
                     ** call if only incoming calls can be transfered.
                     */
                    !( pXferCfg->bIncomingOnly && !pOtherSes->ses.med.bRemInit ) &&
                    pXferCfg->Act.bCfgStatus )
               {
                  /* Call Transfer ---
                  **
                  ** In each Call Transfer scenario, the following takes place:
                  **    1) A calls B; B answers
                  **    2) B hookflashes
                  **    3) B dials C; C answers
                  **    4) B goes onhook.
                  ** Normally, when B goes onhook, the call is transferred from A
                  ** to C because A is already on hold. However, if the call between
                  ** A and B is internal and the call between B and C is exteral,
                  ** then we transfer from C to A instead of from A to C. This is
                  ** because CCTK has no concept of an internal call and it would
                  ** think we are trying to transfer a call to ourselves if we did
                  ** it from A to C.
                  **
                  ** In this context, A<-->B is pOtherSes
                  **                  B<-->C is pSes
                  */

                  /* Two external calls - Let CCTK transfer the call */
                  if( ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) &&
                      ( pOtherSes->uNetUsr != CMGR_INVALID_HANDLE ) )
                  {
                     CMGRLOG_INFORMATION(( "%s: Transfer case 1 (A->B [%X] ext, B->C [%X] ext)", __FUNCTION__, pOtherSes->uNet, pSes->uNet ));
                     cctkCallTransfer( pOtherSes->uNet, pSes->uNet, BOS_FALSE );
                  }
                  /* A<-->B is external and B<-->C is internal.
                  ** Transfer A<-->B to C<-->B.
                  */
                  else if( ( pSes->uNetUsr == CMGR_INVALID_HANDLE ) &&
                           ( pOtherSes->uNetUsr != CMGR_INVALID_HANDLE ) )
                  {
                     CMGRLOG_INFORMATION(( "%s: Transfer case 2 (A->B [%X] ext, B->C [%X] int)", __FUNCTION__, pOtherSes->uNet, pSes->uNet ));
                     cmFsmXferInt( pUsr, &pOtherSes, &pSes );
                  }
                  /* A<-->B is internal and B<-->C is external */
                  else if( ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) &&
                           ( pOtherSes->uNetUsr == CMGR_INVALID_HANDLE ) )
                  {
                     CMSES* pTempSes = pSes;
                     CMGRLOG_INFORMATION(( "%s: Transfer case 3 (A->B [%X] int, B->C [%X] ext)", __FUNCTION__, pOtherSes->uNet, pSes->uNet ));
                     cmFsmFlash( pRes ); /* Perform a hookflash. This will put the B->C call on hold and unhold the A->B call. */

                     /* Switch the sessions */
                     pSes = pOtherSes;
                     pOtherSes = pTempSes;

                     cmFsmXferInt( pUsr, &pOtherSes, &pSes ); /* Always transfer external to internal */
                  }
                  /* Both A<-->B and B<-->C are internal */
                  else
                  {
                     CMGRLOG_INFORMATION(( "%s: Transfer case 4 (A->B [%X] int, B->C [%X] int)", __FUNCTION__, pOtherSes->uNet, pSes->uNet ));
                     cmFsmXferInt( pUsr, &pOtherSes, &pSes );
                  }
                  bXfer = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                     &cfgIdx,
                                     (void *) pXferCfg,
                                     CFG_ITEM_DEL );
               pXferCfg = NULL;
            }
         }

         if ( !bXfer )
         {
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pBasicCall )
               {
                  tmr.uTimeOut = pBasicCall->uByeDelay;

                  /* If the bye-delay is set to a non zero value but we are
                  ** the originator (i.e. caller) of the session, make sure
                  ** bye-delay processing will not apply (zero out the timer
                  ** value).
                  */
                  if ( tmr.uTimeOut &&
                       !pSes->ses.med.bRemInit )
                  {
                     tmr.uTimeOut = 0;
                  }
                  /*
                  ** If the bye-delay is set to a non zero value but there
                  ** was another session which was used for call waiting or
                  ** 3-way calling, make sure bye-delay processing will not
                  ** apply (zero out the timer value).
                  */
                  if ( tmr.uTimeOut &&
                       pOtherSes &&
                       (pOtherSes != pUsr->pSesHeld) )
                  {
                     tmr.uTimeOut = 0;
                  }
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_DEL );
               pBasicCall = NULL;
            }
            else
            {
               tmr.uTimeOut = CMGR_ON_HOOK_GUARD_TIMEOUT;
            }

            if ( tmr.uTimeOut && pSes->bEstab )
            {
               tmr.eType = eCMEVT_TMR_ONHOOK;

               cmTmrStart ( &tmr );
               cmUsrNewState ( pUsr, eCMST_RELEASING );
            }
            else
            {
               cmFsmOnHookGuard ( pRes );
            }
         }
         else
         {
            /* If the session still exists, set it to not in focus and
            ** put this user in the RELEASING state.
            */
            if( pSes )
            {
               pSes->ses.med.bFocus = BOS_FALSE;
               cmUsrNewState ( pUsr, eCMST_RELEASING );
            }
         }
      }
   }
   /* If there is a 'on-hold' session that has been waiting, alert the local
   ** user of the session waiting.
   */
   else
   {
      BOS_UINT32 uIx;
#if CMGRCFG_MULTILINE
      CMCCTKACCNT* pCctkAccnt = NULL;
#endif /* CMGRCFG_MULTILINE */


      /* Check or locate the pending session for this user.
      */
      for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
      {
         /* Pending session must not be a remote session */
         if ( pUsr->pMedSes [ uIx ] && pUsr->pMedSes [ uIx ]->ses.med.bBgFocus == BOS_FALSE )
         {
            pSes = pUsr->pMedSes [ uIx ];
         }
      }
      /* Terminate any possible signal being played (like off-hook warning).
      */
#if CMGRCFG_MULTILINE
      pCctkAccnt = (pSes) ? cmCctkAccntFindByNet( pSes->uNetUsr ) : NULL;
#endif /* CMGRCFG_MULTILINE */

      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = (pSes) ? pSes->uNet : CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;
      pUsr->uCpt = eEPTSIG_NULL;

      if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
      {
         cmPhySignal ( pUsr->uPhys, &sig );
      }

      if ( pSes &&
           ( ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_FLASH ) ||
             ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RELEASING ) ) )
      {
         /* If this was the call waiting session, it not longer is...
         */
         if ( pUsr->pSesWait &&
              ( pUsr->pSesWait == pSes ) )
         {
            pUsr->pSesWait = NULL;
         }

         /* Make this session the focus session.
         */
         pSes->ses.med.bFocus = BOS_TRUE;

         cmFsmRing ( pRes );
      }
      /* If there are no longer session associated here, release the
      ** physical resource.
      */
      else if ( !pSes )
      {
         cmUsrNewState ( pUsr, eCMST_IDLE );

         /* Publish a 'line inactive' event.
         */
         if ( gCmCfgCb.cmgrEvtCb &&
              (pUsr->eType != eCMUSRTYPE_REMOTE) )
         {
            gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_INACTIVE,
                                 pUsr->uPhys,
#if CMGRCFG_MULTILINE
                                 pUsr->uHdl,
                                 CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                 NULL );
         }

         /* If we are pending on an action, don't release
         ** the resource.
         */
         if ( pUsr->bPendingActPhy == BOS_FALSE )
         {
            cmUsrPhyDisassociate( pUsr );
            cmUsrCctkAccntDisassociate( pUsr );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmOffHook
**
** PURPOSE:    FSM off hook handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOffHook ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMGREPSIG sig;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = NULL;
#endif /* CMGRCFG_MULTILINE */

   CMTMR tmr;
   CMSES *pSes = NULL;
   CMSESLIST sesList;
   BOS_UINT32 uIx;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bPlayReorder = BOS_TRUE;
   BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );
   memset ( &sesList, 0, sizeof ( CMSESLIST ) );

   CMGRLOG_INFORMATION (( "cmFsmOffHook: 0x%X (0x%X)",
                          pUsr->uHdl, uAccount ));

   /* Terminate osi timer if we were running it.
   */
   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_OSI_DELAY;

   cmTmrStop ( &tmr );

#if CMGRCFG_MULTILINE
   if (pUsr->eAlarmCfgState != CMALGALRMCFGSTATE_NONE)
   {
      pUsr->bAlarmActive = BOS_TRUE;
      if (pUsr->eAlarmCfgState == CMALGALRMCFGSTATE_PREEMPTIVE)
      {
         cmFsmOffHookPreemption(pUsr);
      }
   }
#endif

   /* Report the hook event to the application in case it needs it.
   */
   if ( gCmCfgCb.cmgrEvtCb )
   {
      gCmCfgCb.cmgrEvtCb ( eCMGREVT_OFFHOOK,
                           pUsr->uPhys,
#if CMGRCFG_MULTILINE
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                           NULL );
   }

   pUsr->bOffHook = BOS_TRUE;
   pUsr->bPendingActPhy = BOS_FALSE;

   /* If the user is not in service, prevent any further action, inform of the
   ** out-of-service status of the user by playing a reorder tone.
   **
   ** Out-of-service status is derived from two variable, one representing
   ** the application layer status, one representing the data-link layer
   ** status.
   */
   if ( !pUsr->bInSrv || !pUsr->bDtLnk )
   {
      CFGPROVDEVICECFG *pDevCfg = NULL;

      CMGRLOG_WARNING (( "cmFsmOffHook: 0x%X (0x%X) - user \'out-of-service\'",
                         pUsr->uHdl, uAccount ));

      /*
      ** Play re-order tone unless configured not to.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                 &cfgIdx,
                                 (void *) &pDevCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         /*play re-order/off-hook warning*/
         if (pDevCfg && pDevCfg->bNoToneOutOfSrv)
         {
           bPlayReorder = BOS_FALSE;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                      &cfgIdx,
                      (void *) pDevCfg,
                      CFG_ITEM_DEL );

      }

      if (bPlayReorder)
      {
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
      tmr.uSes = CMGR_INVALID_HANDLE;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_INIT_REG_DELAY;

      /* If the initial registration delay was running, attempt to register
      ** now.
      */
      if ( cmTmrIsRunning ( &tmr ) )
      {
         cmFsmInitReg ( pRes );
      }
      /*
      ** Otherwise if we are not in service, but may have been in service at
      ** some point in time previously, re-attmpt registration immediately.
      */
      else if ( !pUsr->bInSrv )
      {
         cctkUserRegister ( uAccount );
      }
   }
   else
   {
      switch ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] )
      {
         case eCMST_IDLE:
         {
            /* If there was a call waiting, this means we try to resume it.
            */
            if ( (pSes = cmUsrFocusSes ( pUsr )) != NULL )
            {
#if CMGRCFG_MULTILINE
               pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */


               /* Terminate hold reminder if playing.
               */
               sig.eAct = eCMGREPSIGACT_STOP;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
               sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
               sig.uSig = pUsr->uCpt;   /* eEPTSIG_SPLASH */
               pUsr->uCpt = eEPTSIG_NULL;

               cmPhySignal ( pUsr->uPhys, &sig );

               /* Terminate network hold timer if running.
               */
               if ( pSes->ses.med.bEmergency )
               {
                  tmr.uUsr = pUsr->uHdl;
                  tmr.uSes = pSes->uHdl;
                  tmr.eType = eCMEVT_TMR_NETWORK_HOLD;

                  cmTmrStop ( &tmr );
               }

               /* If this is an external call, tell CCTK to unhold
               ** the call.
               */
               if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
                  cctkCallUnhold ( pSes->uNet );
               }
               /* If this is an internal call, post an "unhold ok"
               ** message for this user.
               */
               else
               {
                  /* Send an unhold ok event */
                  CMEVT evt;

                  memset ( &evt, 0, sizeof( CMEVT ) );

                  evt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
                  evt.uDat1 = pSes->uUsr;           /* User handle */
                  evt.uDat2 = pSes->uHdl;           /* Session handle */
                  evt.uDat4 = CMGR_INVALID_HANDLE;  /* CCTK user id */
                  evt.bLast = BOS_TRUE;             /* Last event */

                  if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
                  {
                     cmSignalEvt ();
                  }
               }
               cmUsrNewState ( pUsr, eCMST_TALK );
            }
            else
            {
#if CMGRCFG_MULTILINE
               /* Barge-in procedures apply if there is an active
               ** external call for an associated account.
               */
                CFGPROVDEVICECFG *pDevCfg = NULL;
                BOS_BOOL bBargeIn = BOS_FALSE;

                memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                           &cfgIdx,
                                           (void *) &pDevCfg,
                                           CFG_ITEM_GET ) == BOS_STATUS_OK )
                {
                   if ( pDevCfg )
                   {
                     bBargeIn = pDevCfg->bBargeInEnabled;
                   }

                   gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                &cfgIdx,
                                (void *) pDevCfg,
                                CFG_ITEM_DEL );
                }

                /*
                ** Check to see if barge-in is enabled.
                **
                ** TO-DO: Behavior has been changed from packetcable default
                **        where barge-in disabled means no outgoing calls
                **        rather than reorder tone
                **        We need to account for this behavior change when rework
                **        on intrusion feature.
                */
                if( bBargeIn &&  cmGetAssocActiveAccnt( pUsr ) != CMGR_INVALID_HANDLE) /* Associated 'active' account */
                {
                   cmFsmImplicitIntrusion ( pRes );
                }
                /* New outgoing call.
                */
                else
#endif
                {
                   cmFsmOutgoingCall ( pRes );
                }
            }
         }
         break;

         case eCMST_RINGING:
         {
            if ( (pSes = cmUsrFocusSes ( pUsr )) != NULL )
            {
#if CMGRCFG_MULTILINE
               pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

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

               if ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
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

                  /* Force a media update to ensure the connection is setup
                  ** properly since the media information was known already.
                  */
                  cmSesPhyUpdate ( pSes, BOS_FALSE );
              }
              else
              {
                  /* If this is an external call, create physical session */
                  if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
                  {
                     /* Create the physical session now if it was not created
                     ** earlier due to the absence of early media support.
                     */
                     cmSesPhyCreate ( pSes );
                  }
              }

              if ( pUsr->pAutoCall )
              {
                 CCTKPRIVCFG privCfg;

#if CMGRCFG_MULTILINE
                 if( !pSes->bEstab )
                 {
                    pSes->bEstab = BOS_TRUE;
                    cmCctkAccntCallAdd( pSes->uNetUsr );
                 }
#endif /* CMGRCFG_MULTILINE */

                 if ( gCmCfgCb.cmgrEvtCb )
                 {
                    CMGRCALLDATA tData;

                    tData.eType = eCMGRCALLTYPE_OUTGOING;
                    tData.uSrc = CMGR_INVALID_HANDLE;

                    ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_NEW,
                                              pSes->uNet,
#if CMGRCFG_MULTILINE
                                              pUsr->uHdl,
                                              (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                              (void *) &tData );
                 }

                 pSes->uNetUsr = uAccount;

                 if ( cctkCallCreate ( &pSes->uNet,
                                        uAccount,
                                        pUsr->pAutoCall->uPeer ) ==
                                                               eCCTKSTATUS_SUCCESS )
                 {
                    if ( cmFeatAssignPrivacy ( pRes, &privCfg ) )
                    {
                       cctkSetParm ( pSes->uNet,
                                     eCCTKPARM_EC_CALLPRIV,
                                     (void *) &privCfg );
                    }

                    cmCfgInitCallMedia ( pRes, pSes );

                    cctkCallMake ( pSes->uNet );
#if CMGRCFG_MULTILINE
                    cmCctkAccntUnforce( pSes->uNetUsr, pSes->uUsr );
#endif /* CMGRCFG_MULTILINE */
                 }

                 /* Terminate the subscription part of the auto-call feature.
                 */
                 cctkEventUnsubscribe ( pUsr->pAutoCall->uSes );
                 cctkEventDelete ( pUsr->pAutoCall->uSes );
                 pUsr->pAutoCall = NULL;

                 cmUsrNewState ( pUsr, eCMST_DIALING );
              }
              else
              {
                  /* Terminate network hold timer if running.
                  */
                  if ( pSes->ses.med.bEmergency )
                  {
                     tmr.uUsr = pUsr->uHdl;
                     tmr.uSes = pSes->uHdl;
                     tmr.eType = eCMEVT_TMR_NETWORK_HOLD;

                     cmTmrStop ( &tmr );
                  }

                  if ( pSes->bEstab && pSes->uNetUsr != CMGR_INVALID_HANDLE )
                  {
                     /* Clear all other calls with this call id (this can only happen if we had internal general call transfer) */
                     cmSesListWithCid( pSes->uNet, &sesList );
                     CMGRLOG_INFORMATION(( "%s: Session list length (%d)", __FUNCTION__, sesList.uLength ));
                     for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
                     {
                        /* Release the call on all other associated sessions */
                        if( sesList.uSes[ uIx ] != pSes->uHdl )
                        {
                           CMSES* pSesClosing = cmSesValid( sesList.uSes[ uIx ] );
                           CMUSR* pUsrClosing = cmUsrValid( (pSesClosing) ? pSesClosing->uUsr : CMGR_INVALID_HANDLE );
                           CMEVT cmEvt;

                           if( !pSesClosing || !pUsrClosing )
                           {
                              CMGRLOG_CRITICAL(( "%s: Invalid user [%X] or session [%X]\n", __FUNCTION__, (pSesClosing) ? pSesClosing->uUsr : CMGR_INVALID_HANDLE, sesList.uSes[ uIx ] ));
                              continue;
                           }

                           /* During an external to internal general call transfer (ie, transferring an external call to all terminals),
                           ** the session is set to established to force CMGR to not send a cctkAnswer, but if we are releasing these
                           ** sessions on all other ringing users, we have to ensure that this flag gets reset, otherwise CMGR thinks
                           ** each of these sessions has an active call on the CCTK Account and tries to remove the call from the line.
                           */
                           pSesClosing->bEstab = BOS_FALSE;

                           memset( &cmEvt, 0, sizeof( CMEVT ) );

                           cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
                           cmEvt.uDat1 = pUsrClosing->uHdl;
                           cmEvt.uDat2 = pSesClosing->uHdl;
                           cmEvt.uDat4 = pSesClosing->uNetUsr;

                           /* Take care of any specific data coming with the event that need
                           ** to be saved.
                           */
                           memcpy ( &pUsrClosing->evt, &cmEvt, sizeof ( CMEVT ) );

                           /* Run the state machine.
                           */
                           cmFsmCallEnding( pUsrClosing );
                        }
                     }

                     /* Tell CCTK to unhold the call */
                     cctkCallUnhold ( pSes->uNet );
                  }
                  else
                  {
                     pSes->bEstab = BOS_TRUE;
#if CMGRCFG_MULTILINE
                     if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
                     {
                        cmCctkAccntCallAdd( pSes->uNetUsr );
                     }
#endif /* CMGRCFG_MULTILINE */
                     if ( gCmCfgCb.cmgrEvtCb )
                     {
                        ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                                 pSes->uNet,
#if CMGRCFG_MULTILINE
                                                 pUsr->uHdl,
                                                 (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                                 (void *) pUsr->uUid );
                     }

                     /* Clear all other calls */
                     cmSesListWithCid( pSes->uNet, &sesList );
                     CMGRLOG_INFORMATION(( "%s: Session list length (%d)", __FUNCTION__, sesList.uLength ));
                     for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
                     {
                        /* Release the call on all other associated sessions */
                        if( sesList.uSes[ uIx ] != pSes->uHdl )
                        {
                           CMSES* pSesClosing = cmSesValid( sesList.uSes[ uIx ] );
                           CMUSR* pUsrClosing = cmUsrValid( (pSesClosing) ? pSesClosing->uUsr : CMGR_INVALID_HANDLE );
                           CMEVT cmEvt;

                           if( !pSesClosing || !pUsrClosing )
                           {
                              CMGRLOG_CRITICAL(( "%s: Invalid user [%X] or session [%X]\n", __FUNCTION__, (pSesClosing) ? pSesClosing->uUsr : CMGR_INVALID_HANDLE, sesList.uSes[ uIx ] ));
                              continue;
                           }

                           memset( &cmEvt, 0, sizeof( CMEVT ) );

                           /* If the session is not established yet, we can safely
                           ** close it.
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

                              /* Run the state machine.
                              */
                              cmFsmCallEnding( pUsrClosing );
                           }
                           /* If the session is already established, that means this
                           ** user is the internal call originating user. Post an event
                           ** indicating that the call was connected.
                           */
                           else if( ( pSesClosing->uNetUsr == CMGR_INVALID_HANDLE ) &&
                                    ( pSesClosing->bEstab ) )
                           {
                              CCTKCALLINFO* pCallInfo = malloc( sizeof( CCTKCALLINFO ) );

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

                              if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                              {
                                 cmSignalEvt ();
                              }
                           }
                        }
                     }
                     /* If this is an internal call, create the physical session immediately */
                     if( pSes->uNetUsr == CMGR_INVALID_HANDLE )
                     {
                        cmSesPhyCreate( pSes );
                     }

                     /* If we need to finalize the media capabilities, do it now
                     ** prior to call answer.
                     */
                     if ( pSes->ses.med.bNeedFinal && pSes->uNetUsr != CMGR_INVALID_HANDLE )
                     {
#if CMGRCFG_MULTILINE
                        /* Call control doesn't get media configuration by default, so we
                        ** need to ensure that the CCTK user gets a configuration at some
                        ** point. Calling this fixes SDP negotation for an unknown reason.
                        */
                        cmCfgInitCallMedia ( pRes, pSes );
#endif /* CMGRCFG_MULTILINE */

                        pSes->ses.med.bNeedFinal = BOS_FALSE;
                        cctkCallMediaNotify( pSes->uNet,
                                             eCCTKMUDPRSN_FINALIZECAPS );
                     }

                     if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
                     {
                        if ( cctkCallAnswer ( pSes->uNet )  ==  eCCTKSTATUS_SUCCESS )
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

                        cmFeatFaxOnly ( pRes );

#if CMGRCFG_REMOTE_USER_CONTROL
                        cmFeatProxyCalleeEnd ( pUsr );
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
                     }
                  }

                  cmUsrNewState ( pUsr, eCMST_TALK );
               }
            }
         }
         break;

         case eCMST_RELEASING:
         {
            /* If there was a call waiting, this means we try to resume it.
            */
            if ( (pSes = cmUsrFocusSes ( pUsr )) != NULL )
            {
#if CMGRCFG_MULTILINE
               pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */


               if ( pSes->bEstab )
               {
                  /* Terminate hold reminder if playing.
                  */
                  sig.eAct = eCMGREPSIGACT_STOP;
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
                 sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                  sig.uSig = pUsr->uCpt;   /* eEPTSIG_SPLASH */
                  pUsr->uCpt = eEPTSIG_NULL;

                  cmPhySignal ( pUsr->uPhys, &sig );

                  tmr.uUsr = pUsr->uHdl;
                  tmr.uSes = pSes->uHdl;
                  tmr.eType = eCMEVT_TMR_ONHOOK;

                  /* If on-hook guard timer was running, stop it and go
                  ** back to a 'resumed' call without needing to issue an
                  ** unhold command.
                  */
                   cmUsrNewState ( pUsr, eCMST_TALK );

                  if ( cmTmrIsRunning ( &tmr ) )
                  {
                     cmTmrStop ( &tmr );
                  }
                  else
                  {
                     if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
                     {
                        /* External Call */
                        cctkCallUnhold ( pSes->uNet );
                     }
                     else
                     {
                        /* Internal Call - Just copy the session handle directly and run the FSM */
                        pUsr->evt.uDat2 = pSes->uHdl;
                        cmFsmUnHoldOkay( pRes );
                     }
                  }
               }
               else
               {
                  /* Update state and re-run the state machine.
                  */
                  cmUsrNewState ( pUsr, eCMST_RINGING );
                  cmFsmOffHook ( pRes );
               }
            }
         }
         break;

         case eCMST_TALK:
         {
            /* If there was a loopback call that the user was not aware of.
            */
            if ( (pSes = cmUsrFocusSes ( pUsr )) != NULL )
            {
               if ( pSes->ses.med.callInfo.bLoopBack )
               {
                  /* Terminate the loopback call.
                  */
                  cmSesTerminate ( pSes );

                  /* Start a new outgoing call.
                  */
                  cmFsmOutgoingCall ( pRes );
               }
            }
         }
         break;

         case eCMST_ACCEPTED:
         {
            if ( (pSes = cmUsrFocusSes ( pUsr )) != NULL )
            {
#if CMGRCFG_MULTILINE
               pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */
               /* Answer the incoming call right away.
               */
               if( !pSes->bEstab )
               {
                  pSes->bEstab = BOS_TRUE;
#if CMGRCFG_MULTILINE
                  if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
                  {
                     cmCctkAccntCallAdd( pSes->uNetUsr );
                  }
#endif /* CMGRCFG_MULTILINE */
               }

               /* Notify the application that the call has connected */
               if ( gCmCfgCb.cmgrEvtCb )
               {
                  ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                           pSes->uNet,
#if CMGRCFG_MULTILINE
                                           pUsr->uHdl,
                                           ((pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                           (void *) pUsr->uUid );
               }

               /* If we need to finalize the media capabilities, do it now
               ** prior to call answer.
               */
               if ( pSes->ses.med.bNeedFinal && pSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
                  pSes->ses.med.bNeedFinal = BOS_FALSE;
                  cctkCallMediaNotify( pSes->uNet,
                                       eCCTKMUDPRSN_FINALIZECAPS );
               }

               /* Clear all other calls */
               cmSesListWithCid( pSes->uNet, &sesList );
               for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
               {
                  /* Release the call on all other associated sessions */
                  if( sesList.uSes[ uIx ] != pSes->uHdl )
                  {
                     CMSES* pSesClosing =  cmSesValid( sesList.uSes[ uIx ] );
                     CMUSR* pUsrClosing = cmUsrValid( pSesClosing->uUsr );
                     CMEVT cmEvt;

                     memset( &cmEvt, 0, sizeof( CMEVT ) );

                     if( !pSesClosing->bEstab || ( pSesClosing->uNetUsr != CMGR_INVALID_HANDLE ) )
                     {
                        cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
                        cmEvt.uDat1 = pUsrClosing->uHdl;
                        cmEvt.uDat2 = pSesClosing->uHdl;
                        cmEvt.uDat4 = pSesClosing->uNetUsr;

                        /* Take care of any specific data coming with the event that need
                        ** to be saved.
                        */
                        memcpy ( &pUsrClosing->evt, &cmEvt, sizeof ( CMEVT ) );

                        /* Run the state machine.
                        */
                        cmFsmCallEnding( pUsrClosing );
                     }
                     /* If this session is already established, send it a
                     ** call connection notification.
                     */
                     else
                     {
                        CCTKCALLINFO* pCallInfo = malloc( sizeof( CCTKCALLINFO ) );

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

                        if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                        {
                           cmSignalEvt ();
                        }
                     }
                  }
               }

               if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
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

                  cmFeatFaxOnly ( pRes );
               }

               cmUsrNewState ( pUsr, eCMST_TALK );

#if CMGRCFG_REMOTE_USER_CONTROL
               cmFeatProxyCalleeEnd ( pUsr );
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
            }
         }
         break;

         default:
         break;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmErrorFlash
**
** PURPOSE:    FSM hook flash handler while in error state.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmErrorFlash ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrActiveSes( pUsr );
   CMTMR tmr;

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   CMGRLOG_INFORMATION (( "cmFsmErrorFlash: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   /* If we have a session (active or held) for this user, then allow for retrieval of
   ** it regardless of the error state situation.
   */
   if ( (pUsr->pSesHeld != NULL) || (pSes != NULL) )
   {
      tmr.uSes = CMGR_INVALID_HANDLE;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_OHWARN;

      if ( cmTmrIsRunning ( &tmr ) )
      {
         cmTmrStop ( &tmr );
      }

      /* Move back to a more appropriate state.
      */
      cmUsrNewState ( pUsr, eCMST_RELEASING );

      /* Process the flash event now.
      */
      cmFsmFlash ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmFlash
**
** PURPOSE:    FSM hook flash handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFlash ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMSES *pOtherSes = NULL;
   CMTMR tmr;
   CMGREPSIG sig;
   BOS_UINT32 uIx;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGSIPTHREEWAYCALL *p3WayCall = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL b3WayCall = BOS_FALSE;
   CCTKCALLMETRICS stats;
   BOS_UINT8 cPubTo[CMGR_IP_ADDRESS_STRING_LEN+1];
   CFGEPTCODECCFG *pCodecCfg = NULL;
   BOS_BOOL bAutoRecover = BOS_FALSE;
   BOS_BOOL bTermOosCalls = BOS_FALSE;
   BOS_BOOL bFree = BOS_FALSE;
   BOS_BOOL bRRFlash = BOS_FALSE;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   CMGRLOG_INFORMATION (( "cmFsmFlash: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   /* If wait key flag is set for RR Flash, this means the user is
   ** already in flash state and waiting for a valid key code.
   ** Ignore the flash and revert to previous state.
   */
   if ( pUsr->rrFlash.bRRWaitKey )
   {
      /* exit now */
      CMGRLOG_INFORMATION (( "cmFsmFlash: 0x%X (0x%X) - Flash while waiting for key, revert",
                             pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));
      cmFsmRRRevert ( pRes );
      return;
   }

#if CMGRCFG_MULTILINE
   /* Ignore flash when alarm line is active */
   if ( pUsr->bAlarmActive )
   {
      /* exit now */
      CMGRLOG_INFORMATION (( "cmFsmFlash: 0x%X (0x%X) - Ignored, Alarm port is active",
                             pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));
      return;
   }
#endif
   /* If this is an emergency call, invoke the proper handler.
   */
   if ( pSes && pSes->ses.med.bEmergency )
   {
      cmFsmFlashEmergency ( pRes );
      /* Exit now.
      */
      return;
   }

   /* If there are no feature enabled for this user that would require the
   ** use of flash, then the flash should simply be ignored since it would
   ** not lead to any valid process.
   */
   if ( cmFeatIgnoreFlash ( pRes ) )
   {
      /* Exit now.
      */
      CMGRLOG_INFORMATION (( "cmFsmFlash: 0x%X (0x%X) - Ignored, no valid feature",
                             pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));
      return;
   }

   /* If the hook flash should be reported via an Information message, an
   ** INFO will be sent with the configured Content-Type and Content, and
   ** the flash will be ignored
   */
   if ( cmFeatReportFlash ( pRes ) )
   {
      /* Exit now.
      */
      CMGRLOG_INFORMATION (( "cmFsmFlash: 0x%X (0x%X) - INFO sent",
                             pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));
      return;
   }

   /* Determine if calls that were maintained on out-of-service need to be
   ** cleared now.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;

   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                              &cfgIdx,
                              (void *) &pBasicCall,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pBasicCall )
      {
         if ( pBasicCall->bHfTermOosCall )
         {
            bTermOosCalls = BOS_TRUE;
         }
         bRRFlash = pBasicCall->bRRFlash;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                            &cfgIdx,
                            (void *) pBasicCall,
                            CFG_ITEM_DEL );
      pBasicCall = NULL;
   }

   /* Check or locate other active session. If requied by configuraiton
   ** any sessions which were maintained on out-of-service
   ** need to terminated now - including the current focus session.
   */
   for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] )
      {
         if ( pUsr->pMedSes [ uIx ]->bMaintdOnOos && bTermOosCalls)
         {
            if( pUsr->pMedSes [ uIx ]->uNetUsr != CMGR_INVALID_HANDLE )
            {
               CMSES *pFreeSes = pUsr->pMedSes [ uIx ];

               CMGRLOG_INFORMATION ((
               "cmFsmFlash: Terminating call 0x%x (0x%X) - it was maintained on out of service",
               pFreeSes->uHdl, pFreeSes->uNet ));

               /* Clear the focus session knowledge if it was just deleted.
               */
               if ( pFreeSes == pSes )
               {
                  pSes = NULL;
               }

               /* Terminate the session and then free it.
               */
               cmSesTerminate ( pFreeSes );

               bFree = BOS_TRUE;

            }
         }
         else if ( pUsr->pMedSes [ uIx ] != pSes )
         {
            pOtherSes = pUsr->pMedSes [ uIx ];
         }
      }
   }


   /* If we only have one session on the system and it is not established yet,
   ** but is in progress, ignore the flash event...
   */
   if ( ( pOtherSes == NULL ) &&
        ( ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_DIALING ) ||
          ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGBACK ) ) )
   {
      /* Exit now.
      */
      CMGRLOG_INFORMATION ((
         "cmFsmFlash: 0x%X (0x%X) on 0x%X (0x%X) - Ignored, primary ongoing...",
         pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));
      return;
   }

   /* If Register Recall style flash is used, and we have two sessions on the
   ** system but is in progress and in RINGBACK, then ignore the flash event
   */
   if ( bRRFlash && pSes && pOtherSes &&
        ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGBACK ) )
   {
      /* Exit now.
      */
      CMGRLOG_INFORMATION ((
         "cmFsmFlash: 0x%X (0x%X) on 0x%X (0x%X) - Ignored, session in ringback...",
         pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));
      return;
   }

   /* If we just flash out of a dialing phase, make sure we terminate the
   ** dialing timers.
   */
   tmr.uUsr = pUsr->uHdl;
   tmr.uSes = CMGR_INVALID_HANDLE;

   tmr.eType = eCMEVT_TMR_DIALTONE;
   if ( cmTmrIsRunning ( &tmr ) )
   {
      cmTmrStop ( &tmr );
   }

   tmr.eType = eCMEVT_TMR_INTERDIGIT_SHORT;
   if ( cmTmrIsRunning ( &tmr ) )
   {
      cmTmrStop ( &tmr );
   }

   tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
   if ( cmTmrIsRunning ( &tmr ) )
   {
      cmTmrStop ( &tmr );
   }

   tmr.eType = eCMEVT_TMR_WARMLINE;
   if ( cmTmrIsRunning ( &tmr ) )
   {
      cmTmrStop ( &tmr );
   }

   /* If we just flash out of an error condition, make sure we terminate the
   ** off-hook warning timer.
   */
   tmr.eType = eCMEVT_TMR_OHWARN;
   if ( cmTmrIsRunning ( &tmr ) )
   {
      cmTmrStop ( &tmr );
   }

   /* If we just flash and we have a pending auto-recover, mark it for further
   ** processing.
   */
   tmr.eType = eCMEVT_TMR_AUTORECOVER;
   if ( cmTmrIsRunning ( &tmr ) )
   {
      bAutoRecover = BOS_TRUE;
   }

   CMGRLOG_DEBUG(( "%s: pSes: 0x%X, pOtherSes: 0x%X, pSesWait: 0x%X, pSesHeld: 0x%X\n",
            __FUNCTION__,
            (unsigned int)pSes,
            (unsigned int)pOtherSes,
            (unsigned int)pUsr->pSesWait,
            (unsigned int)pUsr->pSesHeld ));

   /* If Register Recall style flash is enabled, then set the bRRWaitKey flag if:
   **   - there are two valid sessions,
   **   - there is no held session
   */
   if ( ( bRRFlash ) &&
        ( pSes ) && ( pOtherSes ) && ( !pUsr->pSesHeld ) )
   {
      CMGRLOG_INFORMATION (( "cmFsmFlash: 0x%X (0x%X) - Enter Recall Wait State",
                             pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

      /*
      ** wait for digit input (start timer).
      */
      pUsr->eDigDetState = CMDIGITDETSTA_NONE;
      pUsr->uDigCnt = 0;
      memset ( pUsr->uDigCol, 0, sizeof ( pUsr->uDigCol ) );

      /* Reset the internal call detection flag
      */
      pUsr->bIntCallDet = BOS_FALSE;

      /* Play Special Dial tone */
      sig.eAct = eCMGREPSIGACT_START;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = eEPTSIG_SPECIAL_DIAL;
      pUsr->uCpt = sig.uSig;

      cmPhySignal ( pUsr->uPhys, &sig );

      /* start timer to wait for DTMF digit */
      tmr.uUsr = pUsr->uHdl;
      tmr.uSes = (( pSes->uNet == CMGR_INVALID_HANDLE ) ?
                                      pSes->uHdl : CMGR_INVALID_HANDLE);
      tmr.eType = eCMEVT_TMR_DIALTONE;
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pBasicCall )
         {
            tmr.uTimeOut = pBasicCall->uOrigDTTimer;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }
      else
      {
         tmr.uTimeOut = CMGR_DIAL_TONE_TIMEOUT;
      }

      cmTmrStart ( &tmr );

      /* save current FSM state for later reverting */
      pUsr->rrFlash.ePrevSta = pUsr->eSta[ CMFSM_CURRENT_STATE_IX ];
      pUsr->rrFlash.bRRWaitKey = BOS_TRUE;

      /* Move to 'flash' state */
      cmUsrNewState ( pUsr, eCMST_FLASH );

      return;
   }



   if ( pSes && ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_FLASH ) )
   {
      if ( pUsr->pSesWait && ( pUsr->pSesWait == pOtherSes ) )
      {
         /* Two sessions are active.
         **
         ** One is a call waiting session, the other is the currently active
         ** session.  Call waiting takes precedence over call conferencing, so
         ** we apply call waiting processing here.
         **
         */

         cmFsmCallSwitchActive(pRes);

      }
      else if ( pUsr->pSesHeld && ( pUsr->pSesHeld == pOtherSes ) )
      {
         /* Two sessions are active.
         **
         ** One is a call held session, the other is the currently active
         ** session.  Allow for digit collection to let the user toggle the
         ** active and held session.
         */
         if ( pSes->bEstab )
         {
            /* If this is an external call, tell CCTK to put the call on hold. */
            if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
            {
               /* Put the focus call on hold.
               */
               cctkCallHold ( pSes->uNet );
            }
            /* If this is an internal call, perform some pre-call-hold operations
            ** before signalling that we've held the call.
            */

            /* Move to 'flash' state, play special (recall) dial tone,
            ** wait for digit input (start timer).
            */
            pUsr->eDigDetState = CMDIGITDETSTA_NONE;
            pUsr->uDigCnt = 0;
            memset ( pUsr->uDigCol, 0, sizeof ( pUsr->uDigCol ) );

            /* Reset the internal call detection flag
            */
            pUsr->bIntCallDet = BOS_FALSE;

            cmUsrNewState ( pUsr, eCMST_FLASH );

            /* Reset the digit collection action information.
            */
            cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                        CMDM_ACTION_BUFFER_SIZE );

            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = eEPTSIG_SPECIAL_DIAL;
            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );

            tmr.uUsr = pUsr->uHdl;
            tmr.uSes = (( pSes->uNet == CMGR_INVALID_HANDLE ) ?
                                            pSes->uHdl : CMGR_INVALID_HANDLE);
            tmr.eType = eCMEVT_TMR_DIALTONE;
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pBasicCall )
               {
                  tmr.uTimeOut = pBasicCall->uOrigDTTimer;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_DEL );
               pBasicCall = NULL;
            }
            else
            {
               tmr.uTimeOut = CMGR_DIAL_TONE_TIMEOUT;
            }

            cmTmrStart ( &tmr );

            /* If this is an internal call, run the hold state machine now */
            if( pSes->uNetUsr == CMGR_INVALID_HANDLE )
            {
               CMEVT cmEvt;

               memset( &cmEvt, 0, sizeof( CMEVT ) );

               cmEvt.uEvt = ( eCMEVT_HOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
               cmEvt.uDat1 = pUsr->uHdl;    /* User handle */
               cmEvt.uDat2 = pSes->uHdl;    /* Session handle */
               cmEvt.uDat4 = pSes->uNetUsr; /* CCTK User */
               cmEvt.bLast = BOS_TRUE;      /* Last event */

               if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
               {
                  cmSignalEvt ();
               }
            }
         }
         else
         {
            /* Transient session must be terminated and held session be
            ** retrieved right away.
            */
            pUsr->pSesHeld = NULL;
            pUsr->bPresrvHeld = BOS_FALSE;
            pOtherSes->ses.med.eAction = eCMDM_UNKNOWNACTION;

            cmUsrUnLinkSes ( pUsr, pSes );
            cmSesFree ( pSes, pUsr->uHdl );

            cmFsmAbandonFlash ( pRes );
         }
      }
      else if ( pOtherSes )
      {
         /* Two sessions are active.
         **
         **  If conference (3-way call) is enabled and the sessions are not
         **  conferenced in yet, apply the conference now.
         **
         **  If conference (3-way call) is enabled and the sessions are in
         **  conference, the hook flash terminates the current session, the
         **  other session is restored to the focus state.
         **
         **  If conference (3-way call) is disabled, the hook flash terminated
         **  the current session and restores the other one.
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                                    &cfgIdx,
                                    (void *) &p3WayCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( p3WayCall && p3WayCall->Act.bCfgStatus )
            {
               pUsr->pConfBridgeStatus->eConfOption = p3WayCall->eConfOption;
               if (p3WayCall->pConfUri)
               {
                  strncpy((char *)pUsr->pConfBridgeStatus->cConfUri, (char *)p3WayCall->pConfUri, CMGR_URI_STRING_LEN);
               }
               b3WayCall = BOS_TRUE;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                                  &cfgIdx,
                                  (void *) p3WayCall,
                                  CFG_ITEM_DEL );
            p3WayCall = NULL;
         }

         if ( b3WayCall &&
              ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_DIALING ) )
         {
            if ( ( pSes->ses.med.uConf == CMGR_INVALID_HANDLE ) &&
                 ( pOtherSes->ses.med.uConf == CMGR_INVALID_HANDLE ) )
            {
               switch (pUsr->pConfBridgeStatus->eConfOption)
               {
                  case CFG_SIP_CONFERENCING_REFER_SERVER:
                  case CFG_SIP_CONFERENCING_REFER_PARTICIPANTS:
                  {
                     if (!strcmp( (char *)pUsr->pConfBridgeStatus->cConfUri, (const char *)"") )
                     {
                        CMGRLOG_DEBUG(( "%s: Error: Invalid conference URI!", __FUNCTION__ ));
                        return;
                     }

                     /* Place the current session on hold */
                     cctkCallHold(pSes->uNet);
                     cmSesPhyMute ( pSes );

                     /* Start initializing the server-based conference */
                     pUsr->pConfBridgeStatus->pHeldSes[0] = pSes;
                     pUsr->pConfBridgeStatus->pHeldSes[1] = pOtherSes;
                     pUsr->pConfBridgeStatus->eRequestStatus = CMCONF_STATUS_INIT;
                  }
                  break;

                  case CFG_SIP_CONFERENCING_LOCAL:
                  default:
                  {
                     if ( cmSesConferenceLocal ( pSes, pOtherSes ) )
                     {
                        pSes->ses.med.uConf = pOtherSes->uHdl;
                        pOtherSes->ses.med.uConf = pSes->uHdl;

                        if( pOtherSes->uNetUsr != CMGR_INVALID_HANDLE )
                        {
                           cctkCallUnhold ( pOtherSes->uNet );
                        }
                        else
                        {
                           /* TODO: Add handling here. */
                           CMEVT cmEvt;

                           memset( &cmEvt, 0, sizeof( CMEVT ) );

                           cmEvt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
                           cmEvt.uDat1 = pOtherSes->uUsr;    /* User handle */
                           cmEvt.uDat2 = pOtherSes->uHdl;    /* Session handle */
                           cmEvt.uDat4 = pOtherSes->uNetUsr; /* CCTK User */
                           cmEvt.bLast = BOS_TRUE;           /* Last event */

                           if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                           {
                              cmSignalEvt ();
                           }
                        }
                     }
                  }  /* case CFG_SIP_CONFERENCING_LOCAL: */
               }
            }
            else if ( (pUsr->pConfBridgeStatus->eConfOption == CFG_SIP_CONFERENCING_LOCAL)
                   && ( pSes->ses.med.uConf == pOtherSes->uHdl )
                   && ( pOtherSes->ses.med.uConf == pSes->uHdl ) )
            {
               if ( pSes->bEstab && pSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
                  cPubTo[0] = '\0';
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
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
                           strcpy((char*)cPubTo, (char*)pCodecCfg->pcPubReportAddr);
                        }
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                           &cfgIdx,
                                           (void *) pCodecCfg,
                                           CFG_ITEM_DEL );
                     pCodecCfg = NULL;
                  }

                  if ( strlen((char*)cPubTo) )
                  {
                     BOS_UINT8 ipAddr[CMGR_IP_ADDRESS_STRING_LEN + 7];
                     BOS_IP_ADDRESS pubTo;
                     BOS_STATUS status;

                     status = bosIpAddrCreateFromStr( (char*)&cPubTo, &pubTo);
                     if ( (status == BOS_STATUS_OK) && bosIpAddrIsV6( &pubTo ) )
                     {
                        sprintf ( (char *)ipAddr, "sip:[" );
                     }
                     else
                     {
                        sprintf ( (char *)ipAddr, "sip:" );
                     }

                     strcpy((char*)&ipAddr[strlen( (const char *) ipAddr )], (char*)cPubTo);

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
               if( ( cmNumSesWithCid( pSes->uNet ) == 1 ) &&
                   ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
               {
                  cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
               }
               /* If this is an internal call and one side has hung up, signal both sides
               ** with a "RELEASE_CALL" event.
               */
               else if( ( cmNumSesWithCid( pSes->uNet ) == 2 ) &&
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

                     if( ( pSesRelease ) &&
                         ( pUsrRelease ) )
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
               /* Since we are terminating a 3way call, stop any ringback tones
               ** that are in progress
               */
               if ( pUsr->uCpt == eEPTSIG_RINGBACK )
               {
                  memset ( &sig, 0, sizeof ( CMGREPSIG ) );
                  memset ( &tmr, 0, sizeof ( CMTMR ) );

                  tmr.uSes = pOtherSes->uHdl;
                  tmr.uUsr = pUsr->uHdl;
                  tmr.eType = eCMEVT_TMR_RINGBACK;

                  cmTmrStop ( &tmr );

                  sig.eAct = eCMGREPSIGACT_STOP;
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
                  sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                  sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGBACK */
                  pUsr->uCpt = eEPTSIG_NULL;

                  cmPhySignal ( pUsr->uPhys, &sig );

                  /* We would be playing ringback on the connection as well.
                  ** Terminate it.
                  */
                  sig.uSes = pOtherSes->ses.med.uPhy;
                  sig.uSig = eEPTSIG_RINGBACK;
                  cmPhySignal ( pUsr->uPhys, &sig );
               }


               pSes->ses.med.uConf = CMGR_INVALID_HANDLE;
               pOtherSes->ses.med.uConf = CMGR_INVALID_HANDLE;

               pSes->ses.med.bFocus = BOS_FALSE;
               pOtherSes->ses.med.bFocus = BOS_TRUE;

               /* Move back to the talking state
               */
               cmUsrNewState( pUsr, eCMST_TALK );
            }
         }
         else
         {
            if ( pSes->bEstab && pSes->uNetUsr != CMGR_INVALID_HANDLE )
            {
               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.uIndex = pUsr->uPhys;
               cfgIdx.pcCharIdx = pUsr->uUid;
               cPubTo[0] = '\0';
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                          &cfgIdx,
                                          (void *) &pCodecCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pCodecCfg )
                  {
                     if (pCodecCfg->pcPubReportAddr)
                     {
                        strcpy((char*)cPubTo, (char*)pCodecCfg->pcPubReportAddr);
                     }
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                        &cfgIdx,
                                        (void *) pCodecCfg,
                                        CFG_ITEM_DEL );
                  pCodecCfg = NULL;
               }

               if ( strlen((char*)cPubTo) )
               {
                  BOS_UINT8 ipAddr[CMGR_IP_ADDRESS_STRING_LEN + 7];
                  BOS_IP_ADDRESS pubTo;
                  BOS_STATUS status;

                  status = bosIpAddrCreateFromStr( (char*)&cPubTo, &pubTo);
                  if ( (status == BOS_STATUS_OK) && bosIpAddrIsV6( &pubTo ) )
                  {
                     sprintf ( (char *)ipAddr, "sip:[" );
                  }
                  else
                  {
                     sprintf ( (char *)ipAddr, "sip:" );
                  }

                  strcpy((char*)&ipAddr[strlen( (const char *) ipAddr )], (char*)cPubTo);

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

            if ( pSes->uNet != CMGR_INVALID_HANDLE )
            {
               if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
                  cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
               }
               else
               {
                  /* Signal call releasing for this user */
                  CMEVT cmEvt;

                  memset( &cmEvt, 0, sizeof( CMEVT ) );

                  cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
                  cmEvt.uDat1 = pUsr->uHdl;
                  cmEvt.uDat2 = pSes->uHdl;
                  cmEvt.uDat4 = pSes->uNetUsr;
                  cmEvt.bLast = BOS_TRUE;

                  if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                  {
                     cmSignalEvt ();
                  }
               }
            }
            else
            {
               cmUsrUnLinkSes ( pUsr, pSes );
               cmSesFree ( pSes, pUsr->uHdl );
            }

            if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_DIALING )
            {
               /* Stop dial tone and dial tone timer.
               */
               sig.eAct = eCMGREPSIGACT_STOP;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
               sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
               sig.uSig = pUsr->uCpt;
               pUsr->uCpt = eEPTSIG_NULL;

               cmPhySignal ( pUsr->uPhys, &sig );

               tmr.uUsr = pUsr->uHdl;
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.eType = eCMEVT_TMR_DIALTONE;

               cmTmrStop ( &tmr );
            }

            if( pOtherSes->uNetUsr != CMGR_INVALID_HANDLE )
            {
               cctkCallUnhold ( pOtherSes->uNet );
            }
            else
            {
               /* Send an unhold ok event */
               CMEVT cmEvt;

               memset( &cmEvt, 0, sizeof( CMEVT ) );

               cmEvt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK );
               cmEvt.uDat1 = pOtherSes->uUsr;
               cmEvt.uDat2 = pOtherSes->uHdl;
               cmEvt.uDat4 = pOtherSes->uNetUsr;
               cmEvt.bLast = BOS_TRUE;

               if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
               {
                  cmSignalEvt ();
               }
            }

            pSes->ses.med.bFocus = BOS_FALSE;
            pOtherSes->ses.med.bFocus = BOS_TRUE;

            cmUsrNewState( pUsr, eCMST_TALK );
         }
      }
      /* Only one session is active.
      */
      else
      {
         BOS_BOOL bHoldActive = BOS_FALSE;

         if ( pUsr->pSesHeld && !pSes )
         {
            pSes = pUsr->pSesHeld;
            bHoldActive = BOS_TRUE;
         }

         if ( bHoldActive )
         {
            /* Stop dial tone and dial tone timer.
            */
            sig.eAct = eCMGREPSIGACT_STOP;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = pUsr->uCpt;
            pUsr->uCpt = eEPTSIG_NULL;

            cmPhySignal ( pUsr->uPhys, &sig );

            tmr.uUsr = pUsr->uHdl;
            tmr.uSes = CMGR_INVALID_HANDLE;
            tmr.eType = eCMEVT_TMR_DIALTONE;

            cmTmrStop ( &tmr );

            if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
            {
               cctkCallUnhold ( pSes->uNet );
            }
            else
            {
               /* Send an unhold ok event */
               CMEVT cmEvt;

               memset( &cmEvt, 0, sizeof( CMEVT ) );

               cmEvt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK );
               cmEvt.uDat1 = pSes->uUsr;
               cmEvt.uDat2 = pSes->uHdl;
               cmEvt.uDat4 = pSes->uNetUsr;
               cmEvt.bLast = BOS_TRUE;

               if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
               {
                  cmSignalEvt ();
               }
            }

            pSes->ses.med.eAction = eCMDM_UNKNOWNACTION;
            pUsr->pSesHeld = NULL;

            cmUsrNewState ( pUsr, eCMST_TALK );
         }
         else
         {
            /* Apply auto-recover processing in case it is expected
            ** to be done.
            */
            if ( bAutoRecover )
            {
               switch ( pSes->ses.med.eAutoRec )
               {
                  case eCMSESAUTOREC_LCL:
                  {
                     if ( pSes->uNet != CMGR_INVALID_HANDLE )
                     {
                        /* Resume the call such that the remote end
                        ** can also hear the current user feedback.
                        */
                        cctkCallUnhold ( pSes->uNet );

                        pSes->ses.med.eAutoRec = eCMSESAUTOREC_TPF;
                     }
                  }
                  break;

                  case eCMSESAUTOREC_TPF:
                  {
                     /* No longer need to run the auto-recover timer, no
                     ** longer need to provide user feedback on the prior
                     ** failed call.
                     */
                     memset ( &tmr, 0, sizeof ( CMTMR ) );
                     tmr.uUsr = pUsr->uHdl;
                     tmr.uSes = CMGR_INVALID_HANDLE;
                     tmr.eType = eCMEVT_TMR_AUTORECOVER;

                     cmTmrStop ( &tmr );

                     if ( pUsr->uCpt != eEPTSIG_NULL )
                     {
                        memset ( (void *) &sig,
                                 0,
                                 sizeof ( CMGREPSIG ) );

                        sig.eAct = eCMGREPSIGACT_STOP;
                        sig.uSes = CMGR_INVALID_HANDLE;
                        sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
                        sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                        sig.uSig = pUsr->uCpt;
                        pUsr->uCpt = eEPTSIG_NULL;
                        /*
                        ** Terminate local tone feedback.
                        */
                        cmPhySignal ( pUsr->uPhys, &sig );
                        /*
                        ** Terminate remote tone feedback.
                        */
                        sig.uSes = pSes->ses.med.uPhy;
                        cmPhySignal ( pUsr->uPhys, &sig );
                     }

                     pSes->ses.med.eAutoRec = eCMSESAUTOREC_NONE;
                  }
                  break;

                  default:
                     /* Strange... something is wrong.
                     */
                  break;
               }
            }
            /*
            ** Default behavior.
            */
            else
            {
               if ( pSes->uNet != CMGR_INVALID_HANDLE )
               {
                  if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
                  {
                     cctkCallHold ( pSes->uNet );
                  }
                  else
                  {
                     /* Internal Call - Send an hold ok event to us and the other internal call user */
                     CMSESLIST sesList;
                     CMEVT cmEvt;
                     BOS_UINT32 uIx;
                     CMSES *pSesHolding = NULL;

                     memset( &cmEvt, 0, sizeof( CMEVT ) );

                     cmEvt.uEvt = ( eCMEVT_HOLD_LOCAL | CMGR_EVT_CLASS__CCTK );
                     cmEvt.uDat1 = pSes->uUsr;
                     cmEvt.uDat2 = pSes->uHdl;
                     cmEvt.uDat4 = pSes->uNetUsr;
                     cmEvt.bLast = BOS_TRUE;

                     if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                     {
                        cmSignalEvt ();
                     }

                     /* Find the other user and send a hold-ok to that user also */
                     memset( &sesList, 0, sizeof( CMSESLIST ) );
                     cmSesListWithCid( pSes->uNet, &sesList );
                     for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
                     {
                        pSesHolding = cmSesValid( sesList.uSes[ uIx ] );
                        if( !pSesHolding || pSesHolding->uHdl == pSes->uHdl )
                        {
                           continue;
                        }

                        memset( &cmEvt, 0, sizeof( CMEVT ) );
                        cmEvt.uEvt = ( eCMEVT_HOLD_LOCAL | CMGR_EVT_CLASS__CCTK );
                        cmEvt.uDat1 = pSesHolding->uUsr;
                        cmEvt.uDat2 = pSesHolding->uHdl;
                        cmEvt.uDat4 = pSesHolding->uNetUsr;
                        cmEvt.bLast = BOS_TRUE;

                        if( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                        {
                           cmSignalEvt();
                        }
                     }
                  }
               }

               /* Mute the local stream.
               */
               cmSesPhyMute ( pSes );
            }
         }

         if ( !bHoldActive &&
              !bAutoRecover &&
              ( pSes->uNet != CMGR_INVALID_HANDLE ) )
         {
            /* Move to 'flash' state, play special (recall) dial tone,
            ** wait for digit input (start timer).
            */
            pUsr->eDigDetState = CMDIGITDETSTA_NONE;
            pUsr->uDigCnt = 0;
            memset ( pUsr->uDigCol, 0, sizeof ( pUsr->uDigCol ) );

            /* Reset the internal call detection flag
            */
            pUsr->bIntCallDet = BOS_FALSE;

            cmUsrNewState ( pUsr, eCMST_FLASH );
         }
         else
         {
            if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_DIALING )
            {
               if ( pUsr->eDigDetState != CMDIGITDETSTA_NONE )
               {
                  /* Make sure we terminate the timer running (possibly) from the
                  ** previous dial phase.
                  */
                  tmr.uUsr = pUsr->uHdl;
                  tmr.uSes = pSes->uHdl;
                  tmr.eType = eCMEVT_TMR_INTERDIGIT_SHORT;
                  cmTmrStop ( &tmr );

                  tmr.uUsr = pUsr->uHdl;
                  tmr.uSes = pSes->uHdl;
                  tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
                  cmTmrStop ( &tmr );
               }
               else
               {
                  /* Stop dial tone and dial tone timer.
                  */
                  sig.eAct = eCMGREPSIGACT_STOP;
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
                  sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                  sig.uSig = pUsr->uCpt;
                  pUsr->uCpt = eEPTSIG_NULL;

                  cmPhySignal ( pUsr->uPhys, &sig );

                  tmr.uUsr = pUsr->uHdl;
                  tmr.uSes = CMGR_INVALID_HANDLE;
                  tmr.eType = eCMEVT_TMR_DIALTONE;

                  cmTmrStop ( &tmr );
               }
            }

            if ( !bHoldActive &&
                 !bAutoRecover )
            {
               /* Move back to 'dialing' state, restart digit collection.
               */
               pUsr->eDigDetState = CMDIGITDETSTA_NONE;
               pSes->ses.med.uDigCnt = 0;
               memset ( pSes->ses.med.uDigCol,
                        0,
                        CMGR_COLLECTED_DIGIT_LEN + 1 );

               /* Reset the internal call detection flag
               */
               pUsr->bIntCallDet = BOS_FALSE;

               cmUsrNewState ( pUsr, eCMST_DIALING );
            }
         }

         if ( !bHoldActive &&
              !bAutoRecover )
         {
            /* Reset the digit collection action information.
            */
            cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                        CMDM_ACTION_BUFFER_SIZE );

            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = eEPTSIG_SPECIAL_DIAL;
            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );

            tmr.uUsr = pUsr->uHdl;
            tmr.uSes = (( pSes->uNet == CMGR_INVALID_HANDLE ) ?
                                            pSes->uHdl : CMGR_INVALID_HANDLE);
            tmr.eType = eCMEVT_TMR_DIALTONE;
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pBasicCall )
               {
                  tmr.uTimeOut = pBasicCall->uOrigDTTimer;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_DEL );
               pBasicCall = NULL;
            }
            else
            {
               tmr.uTimeOut = CMGR_DIAL_TONE_TIMEOUT;
            }

            cmTmrStart ( &tmr );
         }
      }
   }
#if CMGRCFG_MULTILINE
   /* No focused session, but we have a held session and are attempting to create a new parallel call.
   ** This can occur if one or more sessions were held by SVC commands.
   */
   else if( !pSes && pUsr->pSesHeld && pUsr->bDisableAutoCallSwitch )
   {
      /* Move to 'flash' state, play special (recall) dial tone,
      ** wait for digit input (start timer).
      */
      pUsr->eDigDetState = CMDIGITDETSTA_NONE;
      memset ( pUsr->uDigCol, 0, sizeof ( pUsr->uDigCol ) );
      /* Reset the digit collection action information.
      */
      cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                  CMDM_ACTION_BUFFER_SIZE );

      /* Reset the internal call detection flag
      */
      pUsr->bIntCallDet = BOS_FALSE;

      cmUsrNewState ( pUsr, eCMST_DIALING );
   }
   /* No focused session but we have a session waiting and we received a flash. This
   ** covers scenarios where both sessions have been put on hold via SVC commands
   ** and when there is a session on hold via SVC command AND a session waiting.
   */
   else if( !pSes && pUsr->pSesWait && ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_RELEASING ) )
   {
      BOS_UINT32 uIx;
      CMSES *pCwSes = pUsr->pSesWait;

      /* See if there is another session */
      pOtherSes = NULL;
      for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
      {
         if ( pUsr->pMedSes [ uIx ] )
         {
            if ( pUsr->pMedSes [ uIx ] != pCwSes )
            {
               pOtherSes = pUsr->pMedSes [ uIx ];
            }
         }
      }

      /* If we have two other sessions, one of which is saved as the "held" session, then unhold one of the sessions */
      if( pOtherSes )
      {
         /* Make the non-callwaiting session focused and then post a HOLD_OK event
         ** cctk fsm will then unhold/answer the waiting session
         */
         pOtherSes->ses.med.bFocus = BOS_TRUE;
         pUsr->evt.uDat2 = pOtherSes->uHdl;
         cmFsmHoldOkay( pUsr );
      }
   }
#endif /* CMGRCFG_MULTILINE */
   /* No focus session and/or in flash state already.
   */
   else
   {
      /* We may be playing some local tone (like reorder) due to previous action
      ** failure, stop those if any.
      */
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      /* If we are still dealing with the focus session, keep it like so
      */
      if ( pSes )
      {
         pOtherSes = pSes;

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_INVALID;

         cmTmrStop ( &tmr );
      }

      if ( pOtherSes )
      {
         /* If this session is the held one and we needed to preserve it
         ** like so due to previous usage scenario (an error condition)
         ** then we need to give the user a chance to retrieve the session
         ** by issuing a proper service code command.
         */
         if ( ( pUsr->pSesHeld == pOtherSes ) &&
              pUsr->bPresrvHeld )
         {
            pUsr->bPresrvHeld = BOS_FALSE;

            cmFsmOutgoingCall ( pRes );
         }
         /* Default handler, we immediately retrieve the held call on
         ** behalf of the user.  Note that this may overwrite an hold
         ** activated session.
         */
         else
         {
            pOtherSes->ses.med.bFocus = BOS_TRUE;

            /* Reset our 'hold active' knowledge if any.
            */
            if ( pOtherSes->ses.med.eAction == eCMDM_HOLDACTIVATE )
            {
               pOtherSes->ses.med.eAction = eCMDM_UNKNOWNACTION;
               pUsr->pSesHeld = NULL;
               pUsr->bPresrvHeld = BOS_FALSE;
            }

            cmUsrNewState ( pUsr, eCMST_TALK );

            if ( pOtherSes->bEstab )
            {
               /* Unhold the local call.
               */
               if( pOtherSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
                  cctkCallUnhold ( pOtherSes->uNet );
               }
               else
               {
                  /* Send an unhold ok event */
                  CMEVT cmEvt;

                  memset( &cmEvt, 0, sizeof( CMEVT ) );

                  cmEvt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK );
                  cmEvt.uDat1 = pOtherSes->uUsr;
                  cmEvt.uDat2 = pOtherSes->uHdl;
                  cmEvt.uDat4 = pOtherSes->uNetUsr;
                  cmEvt.bLast = BOS_TRUE;

                  if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                  {
                     cmSignalEvt ();
                  }
               }
            }
            else
            {
               CMSESLIST sesList;
               BOS_UINT32 uIx;
               CMEVT cmEvt;

               memset( &cmEvt, 0, sizeof( CMEVT ) );
               memset( &sesList, 0, sizeof( CMSESLIST ) );

               /* First time answering the session.
               */
               pOtherSes->bEstab = BOS_TRUE;

               if ( pOtherSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
               {
                  /* We would be playing ringback to the peer. Terminate it.
                  */
                  sig.eAct = eCMGREPSIGACT_STOP;
                  sig.uSes = pOtherSes->ses.med.uPhy;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uSig = eEPTSIG_RINGBACK;

                  cmPhySignal ( pUsr->uPhys, &sig );
               }
               /*
               ** Terminate call waiting timer in case it was running.
               */
               tmr.uSes = pOtherSes->uHdl;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_CALLWAIT;

               cmTmrStop ( &tmr );

               /* Clear all other calls */
               cmSesListWithCid( pOtherSes->uNet, &sesList );
               CMGRLOG_INFORMATION(( "%s: Session list length (%d)", __FUNCTION__, sesList.uLength ));
               for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
               {
                  /* Release the call on all other associated sessions */
                  if( sesList.uSes[ uIx ] != pOtherSes->uHdl )
                  {
                     CMSES* pSesClosing =  cmSesValid( sesList.uSes[ uIx ] );
                     CMUSR* pUsrClosing = cmUsrValid( (pSesClosing) ? pSesClosing->uUsr : CMGR_INVALID_HANDLE );

                     if( !pSesClosing || !pUsrClosing )
                     {
                        CMGRLOG_CRITICAL(( "%s: Invalid user [%X] or session [%X]\n", __FUNCTION__, (pSesClosing) ? pSesClosing->uUsr : CMGR_INVALID_HANDLE, sesList.uSes[ uIx ] ));
                        continue;
                     }

                     memset( &cmEvt, 0, sizeof( CMEVT ) );

                     /* If the session is not established yet, we can safely
                     ** close it.
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

                        /* Run the state machine.
                        */
                        cmFsmCallEnding( pUsrClosing );
                     }
                     /* If the session is already established, that means this
                     ** user is the internal call originating user. Post an event
                     ** indicating that the call was connected.
                     */
                     else if( ( pSesClosing->uNetUsr == CMGR_INVALID_HANDLE ) &&
                              ( pSesClosing->bEstab ) )
                     {
                        CCTKCALLINFO* pCallInfo = malloc( sizeof( CCTKCALLINFO ) );

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

                        if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                        {
                           cmSignalEvt ();
                        }
                     }
                  }
               }

               /* Notify the application that the call has connected */
               if ( gCmCfgCb.cmgrEvtCb )
               {
                  CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pOtherSes->uNetUsr );
                  ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                           pOtherSes->uNet,
#if CMGRCFG_MULTILINE
                                           pUsr->uHdl,
                                           (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                           (void *) pUsr->uUid );
               }
               /* External call case - ask CCTK to answer the call */
               if( pOtherSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
                  /* Answer the network call.
                  */
                  if ( cctkCallAnswer ( pOtherSes->uNet ) ==  eCCTKSTATUS_SUCCESS )
                  {
                     /* Pass on the provisioned privacy configuration of the local
                     ** user - may be needed for transfer scenarios.
                     */
                     CCTKPRIVCFG privCfg;
                     if ( cmFeatAssignPrivacy ( pRes, &privCfg ) )
                     {
                        cctkCallPrivInfo ( pOtherSes->uNet, (void *) &privCfg );
                     }
                  }

                  cmFeatFaxOnly ( pRes );
               }
               else
               {
                  /* Send the connect event to this user also */
                  memset( &cmEvt, 0, sizeof( CMEVT ) );

                  cmEvt.uEvt = ( eCMEVT_CONNECT_CALL | CMGR_EVT_CLASS__CCTK );
                  cmEvt.uDat1 = pUsr->uHdl;
                  cmEvt.uDat2 = pOtherSes->uHdl;
                  cmEvt.uDat4 = pOtherSes->uNetUsr;

                  /* Post the message */
                  if( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                  {
                     cmSignalEvt();
                  }

                  /* Since this is an internal call, establish the session immediately */
                  cmSesPhyCreate( pOtherSes );
               }

               if ( pUsr->pSesWait &&
                   ( pUsr->pSesWait == pOtherSes ) &&
                   ( pOtherSes->uNetUsr != CMGR_INVALID_HANDLE ) )
               {
                  CCTKCALLINFO* pCallInfo = malloc( sizeof( CCTKCALLINFO ) );
                  void* pOldDat = pUsr->evt.pDat3;

                  memset( pCallInfo, 0, sizeof( CCTKCALLINFO ) );

                  strPrintf( (char*)pCallInfo->clid.cName,
                            CCTK_CLID_LEN,
                            "%s",
                            (char*)pUsr->uUid );
                  strPrintf( (char*)pCallInfo->clid.cNumber,
                            CCTK_CLID_LEN,
                            "%s",
                            (char*)pUsr->uUid );

                  /* Generate an internal media event to setup the session properly
                  ** now.
                  */
                  pUsr->evt.uDat2 = pOtherSes->uHdl;
                  pUsr->evt.pDat3 = pCallInfo;
                  cmFsmMedia ( pRes );

                  /* Free the call info and reset the old event data */
                  free( pCallInfo );
                  pUsr->evt.pDat3 = pOldDat;
               }
            }

            /* If the session restored was the call waiting one, there is
            ** no more pending call waiting.
            */
            if ( pUsr->pSesWait &&
                 ( pUsr->pSesWait == pOtherSes ) )
            {
               pUsr->pSesWait = NULL;
            }
         }
      }
      /* There is nothing left to do, play error tone. */
      /* Update state if needed.
      */
      else if ( bFree )
      {
         cmUsrNewState ( pUsr, eCMST_RELEASING );

         if ( ( cmUsrActiveSes ( pUsr ) == NULL ) &&
              ( cmUsrFocusSes ( pUsr ) == NULL ) )
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
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmDigitDetect
**
** PURPOSE:    FSM digit detection (during dialing phase) handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDigitDetect ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMTMR tmr;
   CMGREPSIG sig;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bSTmrExpNoDgt = BOS_FALSE;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT *pCctkAccnt = NULL;
   BOS_UINT8 *pDm = NULL;
#endif /* CMGRCFG_MULTILINE */

   memset ( &tmr,
            0,
            sizeof ( CMTMR ) );

   memset ( &sig,
            0,
            sizeof ( CMGREPSIG ) );

   /* Process key press and key release events.
   */
   if ( (pUsr->evt.uEvt != eCMEVT_TMR_KEYHELD) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_DIALTONE) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_SHORT) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_LONG) )
   {
      if ( pUsr &&
           ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]
                                                             >= eCMEVT_DTMF_0 ) &&
           ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]
                                                             <= eCMEVT_DTMF_D ) )
      {
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_KEYHELD;

         if ( pUsr->evt.pDat3 )
         {
            /* This is a new key being pressed.  Start the key held timer.
            */
            if ( *((EPTDTMF *)pUsr->evt.pDat3) == eEPTDTMF_TONEON )
            {
               if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmDigMap,
                                         CMDM_LONG_DURATION_TMR,
                                         &tmr.uTimeOut ) != eCMDM_SUCCESS )
               {
                  tmr.uTimeOut = CMGR_DIGIT_HELD_TIMEOUT;
               }

               cmTmrStart ( &tmr );
            }
            /* This is a key being released.  Stop the key held timer.  Note that
            ** the timer may have expired already.
            */
            else if ( *((EPTDTMF *)pUsr->evt.pDat3) == eEPTDTMF_TONEOFF )
            {
               cmTmrStop ( &tmr );

               /* Additionally, do not process those events since if anything, the
               ** timer would expire when the key is pressed for too long and that
               ** would produce the desired event/behavior.
               */
               return;
            }
         }
      }
   }

   /* If this is the first digit detection, do something special...
   */
   if ( pUsr->eDigDetState == CMDIGITDETSTA_NONE )
   {
      /* If this is a timer or digit event other than the short interdigit
      ** timer, stop the dial tone now.
      */
      if ( pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_SHORT )
      {
         pUsr->eDigDetState = CMDIGITDETSTA_INPROGRESS;

         /* Stop dial tone and dial tone timer.
         */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
         sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = pUsr->uCpt;   /* eEPTSIG_DIAL or eEPTSIG_SPECIAL_DIAL */
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_TMR_DIALTONE;

         cmTmrStop ( &tmr );

         /* Stop warm line timer in case it was running.
         */
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
         if ( pUsr->bWarmLineActive )
#else
         if ( cmFeatEnabled( pRes, eCMDM_HOTLINEINT ) )
#endif
         {
            tmr.uUsr = pUsr->uHdl;
            tmr.uSes = CMGR_INVALID_HANDLE;
            tmr.eType = eCMEVT_TMR_WARMLINE;

            cmTmrStop ( &tmr );
         }
      }
      else
      /* Short interdigit timer has expired without any digits dialed.
      ** The results of digitmap matching will determine whether
      ** the S timer expiry should be ignored in this case or not.
      */
      {
         bSTmrExpNoDgt = BOS_TRUE;
      }
   }

#if CMGRCFG_MULTILINE
   pCctkAccnt = cmCctkAccntFindByNet( getUsrCctkHdlFromRouting( pUsr, BOS_FALSE ) );
   if( pCctkAccnt )
   {
      CMGRLOG_DEBUG(( "%s: Using SIP Digit map.\n", __FUNCTION__ ));
      pDm = pCctkAccnt->pDmDigMap;
   }
   else
   {
      CMGRLOG_DEBUG(( "%s: Using INTERNAL Digit map.\n", __FUNCTION__ ));
      pDm = pUsr->pDmDigMap;
   }

   if( pUsr->pDmInUse != pDm )
   {
      CMGRLOG_DEBUG(( "%s: Reset digit state on user.\n", __FUNCTION__ ));
      /* Reset the digitmap state on the user */
      cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                  CMDM_ACTION_BUFFER_SIZE );
      pUsr->pDmInUse = pDm;
   }
#else
   if( pUsr->pDmInUse == NULL )
   {
      pUsr->pDmInUse = pUsr->pDmDigMap;
   }
#endif /* CMGRCFG_MULTILINE */

   /* Pass the event to the digit manipulation engine for analysis and
   ** to see what to do next if a dialstring match is yet to be found.
   ** If a match was already found, just ignore the dialed digit.
   **
   ** The digit manipulation engine is expected to inform in return of the
   ** action (if any) to be taken based on the digit received.
   */
   if ( pUsr->eDigDetState == CMDIGITDETSTA_MATCHED )
   {
      CMGRLOG_DEBUG (( "cmFsmDigitDetect: Ignoring digit, dialstring match has already been found." ));
   }
   else if ( pSes && ( pSes->ses.med.uDigCnt < CMGR_COLLECTED_DIGIT_LEN ) )
   {
      CMDMSTATUS dmStatus;
      BOS_BOOL tmrStop = BOS_FALSE;

      /* Stop the running timers for the time being if applicable.
      */
      if ( (pUsr->evt.uEvt != eCMEVT_TMR_DIALTONE) &&
           (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_SHORT) &&
           (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_LONG) )
      {
         tmrStop = BOS_TRUE;

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = pSes->uHdl;
         tmr.eType = eCMEVT_TMR_INTERDIGIT_SHORT;
         cmTmrStop ( &tmr );

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = pSes->uHdl;
         tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
         cmTmrStop ( &tmr );
      }

      /* Set the session handle for this event, this may be used later on but
      ** was not set because digit events are seen per user and not per session.
      */
      if ( pUsr->evt.uDat2 == CMGR_INVALID_HANDLE )
      {
         pUsr->evt.uDat2 = pSes->uHdl;
      }

      /* Update the dialled digit string and process the current string
      ** against the digit map.
      */
      pSes->ses.med.uDigCol[ pSes->ses.med.uDigCnt ] =
         cmMapGetEvt ( cmMapCmEvt2DigCol,
               pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]);
      pSes->ses.med.uDigCnt++;

      /* Apply dialed digit string update routine if needed prior to feeding
      ** this to the digit map.
      */
      if ( cmFeatDialStrManip ( pRes, (void *)pSes ) )
      {
         /* If the dial string was updated internally, reset the digit map
         ** current processing to ensure the newly fed digit string will be
         ** processed fully properly.
         */
         cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                     CMDM_ACTION_BUFFER_SIZE );
      }

      dmStatus = cmDmCheckDialString (
#if CMGRCFG_MULTILINE
                                       (pCctkAccnt) ? pCctkAccnt->uNet : pUsr->uHdl,
#else
                                       pUsr->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                       (CMDMDIGITMAP *) pUsr->pDmInUse,
                                       pSes->ses.med.uDigCol,
                                       (CMDMACTIONSTATE *) pUsr->pDmState );

      /* If the dialtone termination was skipped for the initial S timer event,
      ** stop it now unless the digit map isn't configured to match S timer
      ** expiry on it's own (in which case the event will be ignored).
      */
      if (bSTmrExpNoDgt && dmStatus != eCMDM_NOMATCH)
      {
         pUsr->eDigDetState = CMDIGITDETSTA_INPROGRESS;

         /* Stop dial tone and dial tone timer.
         */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
         sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = pUsr->uCpt;   /* eEPTSIG_DIAL or eEPTSIG_SPECIAL_DIAL */
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_TMR_DIALTONE;

         cmTmrStop ( &tmr );

         /* Stop warm line timer in case it was running.
         */
         if ( cmFeatEnabled( pRes, eCMDM_HOTLINEINT ) )
         {
            tmr.uUsr = pUsr->uHdl;
            tmr.uSes = CMGR_INVALID_HANDLE;
            tmr.eType = eCMEVT_TMR_WARMLINE;

            cmTmrStop ( &tmr );
         }
      }

      /* Take specific action on the digit map matching outcome. */
      switch ( dmStatus )
      {
         /* Process the actions associated with this user.
         */
         case eCMDM_SUCCESS:
         {
            /* Mark that a match has been found for the dialstring.
            */
            pUsr->eDigDetState = CMDIGITDETSTA_MATCHED;

            /* Process the resulting digitmap action.
            */
            cmFsmDigColAction ( pRes );
         }
         break;

         /* Wait for more digits, restart the appropriate digit collection
         ** timers.
         */
         case eCMDM_NOMATCH:
         {
            /* If the match attempt was due to the S tmr event, ignore the
            ** event and remove the S tmr from the dialstring.
            */
            if ( pUsr->evt.uEvt == eCMEVT_TMR_INTERDIGIT_SHORT &&
                 pSes->ses.med.uDigCol[ pSes->ses.med.uDigCnt - 1 ] ==
                                         (BOS_UINT8)CMDM_INTERDIGIT_TMR_SHORT )
            {
                  pSes->ses.med.uDigCnt--;
            }

            if ( tmrStop )
            {
               BOS_UINT32 tmrShort, tmrLong;

               tmrLong = CMGR_INTERDIGIT_LONG_TONE_TIMEOUT;

               tmr.uSes = pSes->uHdl;
               tmr.uUsr = pUsr->uHdl;

               if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmInUse,
                                         CMDM_INTERDIGIT_TMR_SHORT,
                                         &tmrShort ) != eCMDM_SUCCESS )
               {
                  tmrShort = CMGR_INTERDIGIT_SHORT_TONE_TIMEOUT;
               }

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                          &cfgIdx,
                                          (void *) &pBasicCall,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pBasicCall )
                  {
                     tmrLong = pBasicCall->uOrigLITimer;
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                        &cfgIdx,
                                        (void *) pBasicCall,
                                        CFG_ITEM_DEL );
                  pBasicCall = NULL;
               }

               tmr.eType = eCMEVT_TMR_INTERDIGIT_SHORT;
               tmr.uTimeOut = tmrShort;
               cmTmrStart ( &tmr );

               tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
               tmr.uTimeOut = tmrLong;
               cmTmrStart ( &tmr );
            }
         }
         break;

         /* Fail to dial for some reason.  Play re-order to the local
         ** user.
         */
         case eCMDM_ERROR:
         {
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

            cmUsrUnLinkSes ( pUsr, pSes );
            cmSesFree ( pSes, pUsr->uHdl );

            cmUsrNewState ( pUsr, eCMST_IDLE );

            /* Report the fact that there was a dial plan error detected
            ** to the application.
            */
            if ( gCmCfgCb.cmgrEvtCb )
            {
               gCmCfgCb.cmgrEvtCb ( eCMGREVT_DIALPLAN_ERROR,
                                    pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                    pUsr->uHdl,
                                    (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                    NULL );
            }
         }
         break;

         default:
         break;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmFeature
**
** PURPOSE:    FSM digit detection (outside of dialing phase) handler, used to
**             detect specific feature activation/de-activation and act in
**             consequence.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFeature ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMTMR tmr;
   CMGREPSIG sig;
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;

#if CMGRCFG_MULTILINE
   CMCCTKACCNT *pCctkAccnt = NULL;
   BOS_UINT8 *pDm = NULL;
#endif /* CMGRCFG_MULTILINE */

   memset ( &sig,
            0,
            sizeof ( CMGREPSIG ) );

   memset ( &tmr,
            0,
            sizeof ( CMTMR ) );

   /* Process key press and key release events.
   */
   if ( (pUsr->evt.uEvt != eCMEVT_TMR_KEYHELD) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_DIALTONE) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_SHORT) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_LONG) )
   {
      if ( pUsr &&
           ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]
                                                             >= eCMEVT_DTMF_0 ) &&
           ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]
                                                             <= eCMEVT_DTMF_D ) )
      {
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_KEYHELD;

         /* This is a new key being pressed.  Start the key held timer.
         */
         if ( pUsr->evt.pDat3 &&
              (*((EPTDTMF *)pUsr->evt.pDat3) == eEPTDTMF_TONEON) )
         {
            if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmDigMap,
                                      CMDM_LONG_DURATION_TMR,
                                      &tmr.uTimeOut ) != eCMDM_SUCCESS )
            {
               tmr.uTimeOut = CMGR_DIGIT_HELD_TIMEOUT;
            }

            cmTmrStart ( &tmr );
         }
         /* This is a key being released.  Stop the key held timer.  Note that
         ** the timer may have expired already.
         */
         else if ( pUsr->evt.pDat3 &&
                   (*((EPTDTMF *)pUsr->evt.pDat3) == eEPTDTMF_TONEOFF) )
         {
            cmTmrStop ( &tmr );

            /* Additionally, do not process those events since if anything, the
            ** timer would expire when the key is pressed for too long and that
            ** would produce the desired event/behavior.
            */
            return;
         }
      }
   }

   /* If this is the first digit detection, do something special...
   */
   if ( pUsr->eDigDetState == CMDIGITDETSTA_NONE )
   {
      /* Stop dial tone and dial tone timer.
      */
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;   /* eEPTSIG_SPECIAL_DIAL */
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      tmr.uUsr = pUsr->uHdl;
      tmr.uSes = CMGR_INVALID_HANDLE;
      tmr.eType = eCMEVT_TMR_DIALTONE;

      cmTmrStop ( &tmr );

      /* If user is in flash and wait for key state, process the pressed
      ** key in cmFsmRRFeature(), which will consume the event
      */
      if ( pUsr->rrFlash.bRRWaitKey )
      {
         cmFsmRRFeature(pUsr);
         return;
      }

      /* Stop warm line timer in case it was running.
      */
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
      if ( pUsr->bWarmLineActive )
#else
      if ( cmFeatEnabled( pRes, eCMDM_HOTLINEINT ) )
#endif
      {
         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_TMR_WARMLINE;

         cmTmrStop ( &tmr );
      }

      pUsr->eDigDetState = CMDIGITDETSTA_INPROGRESS;

      /* Reset the digit collection action information.
      */
      cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                  CMDM_ACTION_BUFFER_SIZE );
   }

   /* Pass the event to the digit manipulation engine for analysis and
   ** to see what to do next.
   **
   ** The digit manipulation engine is expected to inform in return of the
   ** action (if any) to be taken based on the digit received.
   */
   if ( pUsr->uDigCnt < CMGR_COLLECTED_DIGIT_LEN )
   {
      CMDMSTATUS dmStatus;
      BOS_BOOL tmrStop = BOS_FALSE;

      /* Stop the running timers for the time being if applicable.
      */
      if ( (pUsr->evt.uEvt != eCMEVT_TMR_DIALTONE) &&
           (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_SHORT) &&
           (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_LONG) )
      {
         tmrStop = BOS_TRUE;

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_TMR_INTERDIGIT_SHORT;
         cmTmrStop ( &tmr );

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
         cmTmrStop ( &tmr );
      }

      /* Update the dialled digit string and process the current string
      ** against the digit map.
      */
      pUsr->uDigCol[ pUsr->uDigCnt ] =
         cmMapGetEvt ( cmMapCmEvt2DigCol,
               pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]);
      pUsr->uDigCnt++;

      /* Apply dialed digit string update routine if needed prior to feeding
      ** this to the digit map.
      */
      if ( cmFeatDialStrManip ( pRes, NULL ) )
      {
         /* If the dial string was updated internally, reset the digit map
         ** current processing to ensure the newly fed digit string will be
         ** processed fully properly.
         */
         cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                     CMDM_ACTION_BUFFER_SIZE );
      }

#if CMGRCFG_MULTILINE

      pCctkAccnt = cmCctkAccntFindByNet( getUsrCctkHdlFromRouting( pUsr, BOS_FALSE ) );

      if( pCctkAccnt )
      {
         CMGRLOG_DEBUG(( "%s: Using SIP Digit map.\n", __FUNCTION__ ));
         pDm = pCctkAccnt->pDmDigMap;
      }
      else
      {
         CMGRLOG_DEBUG(( "%s: Using INTERNAL Digit map.\n", __FUNCTION__ ));
         pDm = pUsr->pDmDigMap;
      }

      if( pUsr->pDmInUse != pDm )
      {
         CMGRLOG_DEBUG(( "%s: Reset digit state on user.\n", __FUNCTION__ ));
         /* Reset the digitmap state on the user */
         cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                     CMDM_ACTION_BUFFER_SIZE );
         pUsr->pDmInUse = pDm;
      }
#else
      if( pUsr->pDmInUse == NULL )
      {
         pUsr->pDmInUse = pUsr->pDmDigMap;
      }
#endif /* CMGRCFG_MULTILINE */

      dmStatus = cmDmCheckDialString (
#if CMGRCFG_MULTILINE
                                       (pCctkAccnt) ? pCctkAccnt->uNet : pUsr->uHdl,
#else
                                       pUsr->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                       (CMDMDIGITMAP *) pUsr->pDmInUse,
                                       pUsr->uDigCol,
                                       (CMDMACTIONSTATE *) pUsr->pDmState );
      switch ( dmStatus )
      {
         /* Process the actions associated with this user.
         */
         case eCMDM_SUCCESS:
         {
            cmFsmFeatAction ( pRes );
         }
         break;

         /* Wait for more digits, restart the appropriate digit collection
         ** timers.
         */
         case eCMDM_NOMATCH:
         {
            if ( tmrStop )
            {
               BOS_UINT32 tmrShort, tmrLong;

               tmrLong = CMGR_INTERDIGIT_LONG_TONE_TIMEOUT;

               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = pUsr->uHdl;

               if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmInUse,
                                         CMDM_INTERDIGIT_TMR_SHORT,
                                         &tmrShort ) != eCMDM_SUCCESS )
               {
                  tmrShort = CMGR_INTERDIGIT_SHORT_TONE_TIMEOUT;
               }

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                          &cfgIdx,
                                          (void *) &pBasicCall,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pBasicCall )
                  {
                     tmrLong = pBasicCall->uOrigLITimer;
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                        &cfgIdx,
                                        (void *) pBasicCall,
                                        CFG_ITEM_DEL );
                  pBasicCall = NULL;
               }

               tmr.eType = eCMEVT_TMR_INTERDIGIT_SHORT;
               tmr.uTimeOut = tmrShort;
               cmTmrStart ( &tmr );

               tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
               tmr.uTimeOut = tmrLong;
               cmTmrStart ( &tmr );
            }
         }
         break;

         /* Fail to dial for some reason.  Play re-order to the local
         ** user (should trigger local user action).
         */
         case eCMDM_ERROR:
         {
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

            /* Report the fact that there was a dial plan error detected
            ** to the application.
            */
            if ( gCmCfgCb.cmgrEvtCb )
            {
               gCmCfgCb.cmgrEvtCb ( eCMGREVT_DIALPLAN_ERROR,
                                    pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                    pUsr->uHdl,
                                    (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                    NULL );
            }
         }
         break;

         default:
         break;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmSesStatus
**
** PURPOSE:    FSM status on a session physical action.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmSesStatus ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   BOS_BOOL bFail = BOS_TRUE;

   if ( pSes )
   {
      /* Note here that if we cannot access the information desired, we will
      ** assume that the session status is a failure.
      */
      CMGRCNX *pCnx = ( ( pUsr->evt.pDat3 == NULL ) ?
                           NULL : ( (CMGRCNX *) pUsr->evt.pDat3 ) );

      if ( pCnx )
      {
         CMGRLOG_DEBUG (( "cmFsmSesStatus: %d - 0x%X (0x%X) on 0x%X (0x%X)",
                          pCnx->eSta, pSes->uHdl, pSes->uNet,
                          pUsr->uHdl, pSes->uNetUsr ));

         /* Easy way out for now.  Any failure results in a call termination.
         */
         if ( pCnx->eSta == eCMGRSESSTA_SUCCESS )
         {
            bFail = BOS_FALSE;
         }
      }

      if ( bFail )
      {
         CMGRLOG_WARNING (( "cmFsmSesStatus: Failure 0x%X (0x%X) on 0x%X (0x%X)",
                            pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

         /* If this was the focus call, play re-order and move to the correct
         ** state.
         */
         if ( pSes->ses.med.bFocus )
         {
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
            sig.uSig = eEPTSIG_REORDER;
            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );

            cmUsrNewState ( pUsr, eCMST_RELEASING );
         }

         if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            /* Terminate network call.
            */
            cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
         }
         else
         {
            /* Signal call releasing for this user */
            CMEVT cmEvt;

            memset( &cmEvt, 0, sizeof( CMEVT ) );

            cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
            cmEvt.uDat1 = pUsr->uHdl;
            cmEvt.uDat2 = pSes->uHdl;
            cmEvt.uDat4 = pSes->uNetUsr;
            cmEvt.bLast = BOS_TRUE;

            if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
            {
               cmSignalEvt ();
            }
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmOutgoingCall
**
** PURPOSE:    FSM generic handle to create a new outgoing call.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOutgoingCall ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMGREPSIG sig;
   CMTMR tmr;
   CMSES *pSes = NULL;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGSIPHOTLINE *pHotLineCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   CCTKFEATINFO featInfo;
#if !CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
   BOS_BOOL bWarmLine = BOS_FALSE;
#endif
   BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );
   memset ( &featInfo, 0, sizeof ( CCTKFEATINFO ) );

   CMGRLOG_INFORMATION (( "cmFsmOutgoingCall: 0x%X (0x%X)",
                          pUsr->uHdl, uAccount ));

   pSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl );
   if ( pSes )
   {
      pSes->ses.med.bFocus = BOS_TRUE;

      cmUsrLinkSes ( pUsr, pSes );

      if ( pUsr->uPhys == CMGR_INVALID_HANDLE )
      {
         cmPhyAllocate ( pUsr );
      }

      if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
      {
         /* Publish a 'line active' event.
         */
         if ( gCmCfgCb.cmgrEvtCb &&
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

         /* If the hotline feature is enabled (but not warmline),
         ** apply it now.
         */
         if ( cmFeatEnabled ( pRes, eCMDM_HOTLINEINT ) &&
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
              !(pUsr->bWarmLineActive) )
#else
              ((bWarmLine = cmFeatEnabled ( pRes, eCMDM_WARMLINEINT )) == BOS_FALSE) )
#endif
         {
            cmFeatAccept ( pRes, eCMDM_HOTLINEINT );
            cmUsrNewState ( pUsr, eCMST_DIALING );
         }
         /* No hotline enabled for this user, just continue with standard
         ** call creation.
         */
         else
         {
            tmr.uSes = pSes->uHdl;
            tmr.uUsr = pUsr->uHdl;

            /* If the warmline feature is enabled, run the warmline timer
            ** instead.
            */
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
            if ( pUsr->bWarmLineActive )
#else
            if ( bWarmLine )
#endif
            {
               tmr.eType = eCMEVT_TMR_WARMLINE;

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                          &cfgIdx,
                                          (void *) &pHotLineCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pHotLineCfg )
                  {
                     tmr.uTimeOut = pHotLineCfg->uOffHookTimer;

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                           &cfgIdx,
                                           (void *) pHotLineCfg,
                                           CFG_ITEM_DEL );
                     pHotLineCfg = NULL;
                  }
                  else
                  {
                     tmr.uTimeOut = CMGR_WARM_LINE_TIMEOUT;
                  }
               }
            }
            else
            {
               tmr.eType = eCMEVT_TMR_DIALTONE;

               /* Shorten (due to feature activation) dial tone, use the short
               ** interdigit timer.
               */
               if ( pUsr->bShortDT )
               {
                  if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmDigMap,
                                            CMDM_INTERDIGIT_TMR_SHORT,
                                            &tmr.uTimeOut ) != eCMDM_SUCCESS )
                  {
                     tmr.uTimeOut = CMGR_INTERDIGIT_SHORT_TONE_TIMEOUT;
                  }
               }
               /* Normal length dial tone.
               */
               else
               {
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = pUsr->uUid;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                             &cfgIdx,
                                             (void *) &pBasicCall,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     tmr.uTimeOut = pBasicCall->uOrigDTTimer;

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                           &cfgIdx,
                                           (void *) pBasicCall,
                                           CFG_ITEM_DEL );
                     pBasicCall = NULL;
                  }
                  else
                  {
                     tmr.uTimeOut = CMGR_DIAL_TONE_TIMEOUT;
                  }
               }
            }

            cmTmrStart ( &tmr );

            cmUsrNewState ( pUsr, eCMST_DIALING );

            pUsr->eDigDetState = CMDIGITDETSTA_NONE;
            pSes->ses.med.uDigCnt = 0;
            memset ( pSes->ses.med.uDigCol,
                     0,
                     CMGR_COLLECTED_DIGIT_LEN + 1 );

            /* Reset the digit collection action information.
            */
            cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                        CMDM_ACTION_BUFFER_SIZE );

            /* Reset the internal call detection flag
            */
            pUsr->bIntCallDet = BOS_FALSE;

            /* Start dial tone.
            */
            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = eEPTSIG_DIAL;

            if ( cctkGetParm ( uAccount,
                               eCCTKPARM_FEATINFO,
                               (void *)&featInfo ) == eCCTKSTATUS_SUCCESS )
            {
               /* If call forward variable is enabled. */
               if ( featInfo.bCfv )
               {
                  CFGSIPCALLFWD *pCfgCallFwd = NULL;
                  /* Check for special condition dial tone that could override the
                  ** default one.
                  */
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = pUsr->uUid;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                             &cfgIdx,
                                             (void *) &pCfgCallFwd,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pCfgCallFwd && pCfgCallFwd->bSpecialDialTone )
                     {
                        sig.uSig = eEPTSIG_SPECIAL_DIAL;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                           &cfgIdx,
                                           (void *) pCfgCallFwd,
                                           CFG_ITEM_DEL );
                     pCfgCallFwd = NULL;
                  }
               }

               /* If the do-not-disturb feature is enabled.
               **
               ** Note: Do-Not-Disturb takes precedence over Call-Forward-Variable,
               **       hence the order of the checks.
               */
               if ( featInfo.bDnd )
               {
                  sig.uSig = eEPTSIG_SPECIAL_DIAL;
               }
            }
            /* If none of the above feature is currently enabled but we have a
            ** message waiting indication, see if we need to apply stutter dial
            ** tone.
            */
            if ( ( sig.uSig == eEPTSIG_DIAL ) && pUsr->bMwi )
            {
               CFGSIPMWI *pMwiCfg = NULL;

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, MWIConcept ),
                                          &cfgIdx,
                                          (void *) &pMwiCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pMwiCfg && pMwiCfg->bStutterTone )
                  {
                     sig.uSig = eEPTSIG_STUTTER;
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, MWIConcept ),
                                        &cfgIdx,
                                        (void *) pMwiCfg,
                                        CFG_ITEM_DEL );
                  pMwiCfg = NULL;
               }
            }
            /* Finally if we have a condition of local do-not-disturb, apply
            ** stutter dial tone unconditionaly.
            */
            if ( ( sig.uSig == eEPTSIG_DIAL ) && pUsr->bDnd )
            {
               sig.uSig = eEPTSIG_STUTTER;
            }

            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );
         }
      }
   }

   /* Reset any possible temporary setting that may have been
   ** left untouched due to error condition.
   */
   pUsr->bShortDT = BOS_FALSE;
}

#if CMGRCFG_MULTILINE
/***********************************************************************************
** FUNCTION:   cmFsmImplicitIntrusion
**
** PURPOSE:    FSM generic handle to intrude into an external call on the same
**             account.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmImplicitIntrusion ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSESLIST sesList;
   CMSES *pActSes = NULL;
   CMSES *pSes = NULL;
   BOS_UINT32 uAccount = cmGetAssocActiveAccnt( pUsr ); /* Associated 'active' account */
   BOS_UINT32 uIx = 0;
   BOS_BOOL bBargeIn = BOS_TRUE;

   /* Cannot barge-in if there isn't an associated account with an active call.
   */
   if ( uAccount == CMGR_INVALID_HANDLE )
   {
      bBargeIn = BOS_FALSE;
   }

   /* Barge-in is allowed
   */
   if( bBargeIn )
   {
       CMGRLOG_INFORMATION (( "cmFsmImplicitIntrusion: 0x%X (0x%X)",
                          pUsr->uHdl, uAccount ));

      /* Create a new session to barge-in with
      */
      pSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl );
      if ( pSes )
      {
         bBargeIn = BOS_FALSE;

         pSes->ses.med.bFocus = BOS_TRUE;
         cmUsrLinkSes ( pUsr, pSes );

         if ( pUsr->uPhys == CMGR_INVALID_HANDLE )
         {
            cmPhyAllocate ( pUsr );
         }

         if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
         {
            /* Publish a 'line active' event.
            */
            if ( gCmCfgCb.cmgrEvtCb &&
                 (pUsr->eType != eCMUSRTYPE_REMOTE) )
            {
               gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_ACTIVE,
                                    pUsr->uPhys,
                                    pUsr->uHdl,
                                    CMGR_INVALID_HANDLE,
                                    NULL );
            }

            /* Find the session to intrude.
            */
            memset( &sesList, 0, sizeof( CMSESLIST ) );
            cmSesListWithNet( uAccount, &sesList );

            for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
            {
               /* We can only intrude into one session, so intrude
               ** into the first external focus session with a physical connection.
               */
               pActSes = cmSesValid( sesList.uSes[ uIx ] );

               if ( ( pActSes &&
                     ( pActSes->ses.med.bFocus == BOS_TRUE )) &&
                      (( pActSes->uNetUsr != CMGR_INVALID_HANDLE ) &&
                        ( pActSes->ses.med.uPhy != CMGR_INVALID_HANDLE )) )
               {

                  CMGRLOG_INFORMATION (( "cmFsmImplicitIntrusion: usr(0x%X) ses(0x%X) -> ses(0x%X) accnt(0x%X)",
                             pUsr->uHdl, pSes->uHdl, pActSes->uHdl, pActSes->uNetUsr ));

                  /* Setup the barge-in session */
                  pSes->uNet = cctkGenerateId();
                  pSes->bEstab = BOS_TRUE;

                  /* Link the intrusion session to the existing session, and create the
                  ** physical connection.
                  */
                  pSes->ses.med.uBargeIn = pActSes->uHdl;
                  cmSesPhyCreate( pSes );

                  cmUsrNewState ( pUsr, eCMST_TALK );

                  /* Indicate that barge in was succesful.
                  */
                  bBargeIn = BOS_TRUE;

                  break;
               }
            }
         }
      }
   }

   /* Not allowed to barge-in - lockout the physical events from this user.
   */
   if ( !bBargeIn )
   {
      CMGREPSIG sig;
      CMGRLOG_WARNING (( "cmFsmImplicitIntrusion: Not valid for 0x%X (0x%X)",
                          pUsr->uHdl, uAccount ));

      /* If barge-in was allowed but failed for some reason, free
      ** up the allocated session.
      */
      if( pSes )
      {
         cmUsrUnLinkSes ( pUsr, pSes );
         cmSesFree ( pSes, pUsr->uHdl );
         pSes->uNet = CMGR_INVALID_HANDLE;
      }

      memset ( &sig, 0, sizeof ( CMGREPSIG ) );

      /* Play reorder and go into the error state.
      */
      sig.eAct = eCMGREPSIGACT_START;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
      sig.uLine = CMGR_INVALID_HANDLE;
      sig.uSig = eEPTSIG_REORDER;
      pUsr->uCpt = sig.uSig;

      cmPhySignal ( pUsr->uPhys, &sig );

      cmUsrNewState( pUsr, eCMST_ERROR );
   }
}
#endif

/***********************************************************************************
** FUNCTION:   cmFsmModemCall
**
** PURPOSE:    FSM fax calling handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmModemCall ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   /* Ensure the modem event is for a valid session and this session is the
   ** focus one.  Otherwise, this does not make sense, ignore the event.
   ** Also ignore the event if this is an internal call.
   */
   if ( pSes && ( pSes == cmUsrFocusSes ( pUsr ) ) &&
        ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
   {
      CCTKMEDLST cctkMedLst;
      CCTKMEDCFG cctkMedCfg;

      memset ( &cctkMedLst,
               0,
               sizeof( CCTKMEDLST ) );

      memset ( &cctkMedCfg,
               0,
               sizeof( CCTKMEDCFG ) );

      CMGRMEDINFO media;
      memset( &media, 0, sizeof( CMGRMEDINFO ) );


      if ( cctkGetParm ( pSes->uNet,
                         eCCTKPARM_MEDINFO,
                         (void *)&media.media ) ==
           eCCTKSTATUS_SUCCESS )
      {
         /* check if right codec is used. If yes, do nothing as the DSP and endpoint driver 
         ** has done all the jobs when DSP switches VBD mode.  Otherwise, we needs to switch 
         ** the codec to G.711 and turn off comfort noise and silent suppression */
         if ( ( (media.media.egress.audio.codec[0].eType != eCCTKCODEC_PCMU) &&
                (media.media.egress.audio.codec[0].eType != eCCTKCODEC_PCMA) ) ||
              ( (media.media.ingress.audio.codec[0].eType != eCCTKCODEC_PCMU) &&
                (media.media.ingress.audio.codec[0].eType != eCCTKCODEC_PCMA) ) )
         {
            CMGRLOG_INFORMATION ((
               "cmFsmModemCall: usr 0x%X (0x%X) - forcing vbd on 0x%X (0x%X)",
               pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));

            /* Setup the media information to forcefully use PCMU or PCMA for 
            ** voice band data configuration.
            */
            cctkMedLst.mMediaTypes = CCTK_MEDIA_AUDIO_MASK;
            cctkMedLst.audCodeLst[ cctkMedLst.uNumAudCodec ] = eCCTKCODEC_PCMU;
            cctkMedLst.uNumAudCodec++;
            cctkMedLst.audCodeLst[ cctkMedLst.uNumAudCodec ] = eCCTKCODEC_PCMA;
            cctkMedLst.uNumAudCodec++;


            if ( cctkGetParm ( pSes->uNet,
                         eCCTKPARM_MC_MEDIACFG,
                         (void *)&cctkMedCfg ) ==
                 eCCTKSTATUS_SUCCESS )
               {

            cctkMedCfg.pMediaLst = &cctkMedLst;

            /* Turn off silence suppression and comfort noise for VBD */
            cctkMedCfg.pAudio->gen.bCn = BOS_FALSE;
            cctkMedCfg.pAudio->gen.eSilSupp = eCCTKSILSUPP_OFF;
            
            cctkSetParm ( pSes->uNet, 
                            eCCTKPARM_MC_MEDIACFG,
                            (void *)&cctkMedCfg ); 

            /* Ask for modification of the media.  Upon success, the cmFsmMedia
            ** FSM function will be invoked directly.
            */
            cctkCallMediaNotify ( pSes->uNet,
                                  eCCTKMUPDRSN_REFRESHSDP );
         }
            else
            {
               CMGRLOG_WARNING (( "cmFsmModemCall: Failed to get CCTKMEDCFG" ));
            }
         }
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmFaxCall
**
** PURPOSE:    FSM fax calling handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFaxCall ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   /* Ensure the fax event is for a valid session and this session is the
   ** focus one.  Otherwise, this does not make sense, ignore the event.
   ** Also ignore the event if this is an internal call.
   */
   if ( pSes && ( pSes == cmUsrFocusSes ( pUsr ) ) &&
        ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
   {
      /* Check whether we can support fax-relay.
      */
      CFGEPTCODECCFG *pEptCodecCfg = NULL;
      PROV_CFG_IDX cfgIdx;
      BOS_BOOL faxRelay = BOS_FALSE;
      CMTMR tmr;
      CCTKMEDLST cctkMedLst;

      memset ( &cctkMedLst,
               0,
               sizeof( CCTKMEDLST ) );

      memset ( &tmr,
               0,
               sizeof ( CMTMR ) );

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.uIndex = pUsr->uPhys;
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                 &cfgIdx,
                                 (void *) &pEptCodecCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         faxRelay = pEptCodecCfg->bT38Enable;
         gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                               &cfgIdx,
                               (void *) pEptCodecCfg,
                               CFG_ITEM_DEL );
         pEptCodecCfg = NULL;
      }

      /* If we were running a time awaiting for fax event, cancel it
      ** now since we got one.
      */
      tmr.eType = eCMEVT_TMR_FAX_EVENT;
      tmr.uUsr = pUsr->uHdl;
      tmr.uSes = pSes->uHdl;

      if ( cmTmrIsRunning ( &tmr ) )
      {
         cmTmrStop ( &tmr );
      }

      /* Choose fax relay, attempt to apply it.
      */
      if ( faxRelay &&
           !pSes->ses.med.faxRelay.bFailed &&
           !pSes->ses.med.faxRelay.bAttempt &&
           !pSes->ses.med.faxRelay.bOngoing )
      {
         CMGRLOG_INFORMATION ((
            "cmFsmFaxCall: usr 0x%X (0x%X) - attempt faxrelay on 0x%X (0x%X)",
            pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));

         pSes->ses.med.faxRelay.bAttempt = BOS_TRUE;

         /* Setup the media information to forcefully use fax relay
         ** configuration.
         */
         cctkMedLst.mMediaTypes = CCTK_MEDIA_T38_MASK;
         cctkSetParm ( pSes->uNet,
                       eCCTKPARM_MC_MEDIALIST,
                       (void *) &cctkMedLst );

         /* Ask for modification of the media.  Upon success, the cmFsmMedia
         ** FSM function will be invoked directly.
         */
         cctkCallMediaNotify ( pSes->uNet,
                               eCCTKMUPDRSN_REFRESHSDP );
      }
      /* Fax relay is not supported, see whether we need to fallback to
      ** a voice band data mode by re-negotiating one of the voice band
      ** data codec.
      */
      else if ( !faxRelay )
      {
         /* Treat it as VBD modem call */
         cmFsmModemCall ( pRes );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmFaxEnd
**
** PURPOSE:    FSM fax call ending handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFaxEnd ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   BOS_BOOL bFailedAttempt = BOS_FALSE;

   /* Ensure the fax event is for a valid session and this session is the
   ** focus one.  Otherwise, this does not make sense, ignore the event.
   ** Also ignore the event if this is an internal call.
   */
   if ( pSes && ( pSes == cmUsrFocusSes ( pUsr ) ) &&
        ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
   {
      /* Keep track of a failed fax-relay attempt.
      */
      bFailedAttempt =
         ( pSes->ses.med.faxRelay.bAttempt &&
           pSes->ses.med.faxRelay.bFailed );

      if ( pSes->ses.med.faxRelay.bOngoing ||
           pSes->ses.med.faxRelay.bAttempt )
      {
         CMGRLOG_INFORMATION ((
            "cmFsmFaxEnd: usr 0x%X (0x%X) - ses 0x%X (0x%X)",
            pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));

         /* Roll back to a non fax relay media.
         */
         pSes->ses.med.faxRelay.bAttempt = BOS_FALSE;
         pSes->ses.med.faxRelay.bOngoing = BOS_FALSE;

         /* If we were just waiting for this event to apply the new
         ** media information, do it now.
         */
         if ( pSes->ses.med.faxRelay.bDeferEnd )
         {
            pSes->ses.med.faxRelay.bDeferEnd = BOS_FALSE;

            cmFsmMedia ( pRes );
         }
         /* We need to renegotiate the media information to go back to
         ** non fax relay.
         */
         else
         {
            CCTKMEDLST cctkMedLst;
            CCTKMEDAUD cctkMedAud;
            BOS_UINT32 uIx;

            memset ( &cctkMedLst,
                     0,
                     sizeof( CCTKMEDLST ) );

            if ( bFailedAttempt )
            {
               /* If this was a failure to attempt T.38 fax-relay, try to roll
               ** back to a voice band data type of fax transmission.
               */
               cctkMedLst.audCodeLst[ cctkMedLst.uNumAudCodec ] = eCCTKCODEC_PCMU;
               cctkMedLst.uNumAudCodec++;
               cctkMedLst.audCodeLst[ cctkMedLst.uNumAudCodec ] = eCCTKCODEC_PCMA;
               cctkMedLst.uNumAudCodec++;
            }
            else
            {
               /* Copy the audio configuration because we need to reset the
               ** list of codec to be used within the cctkMedLst.
               */
               cmCfgAudio( pRes, pSes, &cctkMedAud );

               /* Setup the media information to forcefully use audio
               ** configuration.
               */
               cctkMedLst.uNumAudCodec = cctkMedAud.gen.uNumCodec;
               for ( uIx = 0 ; uIx < cctkMedLst.uNumAudCodec ; uIx++ )
               {
                  cctkMedLst.audCodeLst[ uIx ] = cctkMedAud.gen.codec[ uIx ].eType;
               }
            }

            cctkMedLst.mMediaTypes = CCTK_MEDIA_AUDIO_MASK;

            cctkSetParm ( pSes->uNet,
                          eCCTKPARM_MC_MEDIALIST,
                          (void *) &cctkMedLst );

            /* Ask for modification of the media.  Upon success, the cmFsmMedia
            ** FSM function will be invoked directly.
            */
            cctkCallMediaNotify ( pSes->uNet,
                                  eCCTKMUPDRSN_REFRESHSDP );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmFaxFail
**
** PURPOSE:    FSM fax call failure handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFaxFail ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   /* Ensure the fax event is for a valid session and this session is the
   ** focus one.  Otherwise, this does not make sense, ignore the event.
   ** Also ignore the event if this is an internal call.
   */
   if ( pSes && ( pSes == cmUsrFocusSes ( pUsr ) ) &&
        ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
   {
      if ( pSes->ses.med.faxRelay.bOngoing ||
           pSes->ses.med.faxRelay.bAttempt )
      {
         CMGRLOG_WARNING ((
            "cmFsmFaxFail: usr 0x%X (0x%X) - ses 0x%X (0x%X)",
            pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));

         pSes->ses.med.faxRelay.bFailed = BOS_TRUE;

         /* Apply similar processing as for a fax ending.  The difference here
         ** is that we will not allow further fax events detected during this
         ** session to trigger fax-relay processing.
         */
         cmFsmFaxEnd ( pRes );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmAudio
**
** PURPOSE:    FSM audio data received for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmAudio ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_DEBUG (( "cmFsmAudio: 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      pSes->ses.med.bMediaRx = BOS_TRUE;

      if ( (pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_DIALING) ||
           (pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGBACK) )
      {
         cmSesPhyActMonThresh ( pSes,
                                CMSES_MEDIA_ACT_MON_THRESH_MSEC );
      }

      if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGBACK )
      {
         CCTKMEDINFO mediaInfo;
         if ( cctkGetParm ( pSes->uNet,
                            eCCTKPARM_MEDINFO,
                            (void *)&mediaInfo ) ==
              eCCTKSTATUS_SUCCESS )
         {
            if ( !bosIpAddrIsZero( &mediaInfo.ingress.addr ) &&
                 mediaInfo.ingress.audio.uNumCodec &&
                 mediaInfo.ingress.audio.codec[ 0 ].uRtpPort )
            {
               /* If remote SDP is available, stop the ring-back tone as we should
               ** get in-band tone instead at this point (data path is created).
               ** If remote SDP is not available, we will ignore the media event
               ** and continue to generate local ringback.
               */
               CMGREPSIG sig;
#if CMGRCFG_MULTILINE
               CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

               CMTMR tmr;
               int uIx;
               CMSES *pOtherSes = NULL;

               memset ( &sig, 0, sizeof ( CMGREPSIG ) );
               memset ( &tmr, 0, sizeof ( CMTMR ) );

               tmr.uSes = pSes->uHdl;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_RINGBACK;

               cmTmrStop ( &tmr );

               sig.eAct = eCMGREPSIGACT_STOP;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
               sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
               sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGBACK */
               pUsr->uCpt = eEPTSIG_NULL;

               cmPhySignal ( pUsr->uPhys, &sig );

               /* Terminate any ringback generated to the peer as well
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

                  if ( pOtherSes )
                  {
                     sig.uSes = pOtherSes->ses.med.uPhy;
                     sig.uSig = eEPTSIG_RINGBACK;
                     cmPhySignal ( pUsr->uPhys, &sig );
                  }
               }
            }
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmAudioEnd
**
** PURPOSE:    FSM audio no longer received for this resource (superceeds a
**             cmFsmAudio action).
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This function is expected to be invoked primarely in the scenario
**             of an outgoing call establishment when as part of the call setup,
**             the session may have been redirected initially to an announcement
**             server that provided information to the user.  Now the announcement
**             is terminated and the call continue to proceed, the end of the
**             announcement would have triggered this event due to lack of audio.
***********************************************************************************/
void cmFsmAudioEnd ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_DEBUG (( "cmFsmAudioEnd: 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      pSes->ses.med.bMediaRx = BOS_FALSE;

      if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGBACK )
      {
         CMTMR tmr;

         memset ( &tmr, 0, sizeof ( CMTMR ) );

         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_RINGBACK;

         if ( cmTmrIsRunning ( &tmr ) )
         {
            cmTmrStop ( &tmr );
         }

         /* Invoke the ring-back handler to ensure we apply proper processing,
         ** and in particular restart the local ringback as we would no longer
         ** get in-band tone at this point (data path is created but there is
         ** no received audio on it).  There is no other expected way to get
         ** into the ringback state that would otherwise trigger this same
         ** behavior.
         */
         cmFsmCallRingback ( pRes );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmAudioDelayed
**
** PURPOSE:    FSM audio delayed data received for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       Audio 'delayed' data as opposed to audio data (cmFsmAudio function)
**             corresponds to indication of audio information received for a
**             resource at the time the resource was not completely setup due
**             to thread executions (the audio information is coming through
**             a higher priority thread).  However we are still giving this event
**             a chance since it was queued up in the proper thread context, so
**             now it should make sense to be processed.
***********************************************************************************/
void cmFsmAudioDelayed ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   /* The data passed in the case of a delayed media event is the actual
   ** physical handle reporting the event which we need to match now to
   ** an existing session.
   */
   CMSES *pSes = cmSesFindPhyHdl ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_DEBUG (( "cmFsmAudioDelayed: 0x%X on 0x%X (0x%X)",
                       pUsr->evt.uDat2, pUsr->uHdl, pSes->uNetUsr ));

      /* This is a valid session, update the event and run the appropriate
      ** state machine functionality for it.
      */
      pUsr->evt.uDat2 = pSes->uHdl;

      cmFsmAudio ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCancelKeyTmr
**
** PURPOSE:    FSM cancel the key held timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCancelKeyTmr ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMTMR tmr;

   CMGRLOG_DEBUG (( "cmFsmCancelKeyTmr: 0x%X (0x%X)",
                    pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   if ( pUsr &&
        ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]
                                                          >= eCMEVT_DTMF_0 ) &&
        ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]
                                                          <= eCMEVT_DTMF_D ) )
   {
      tmr.uSes = CMGR_INVALID_HANDLE;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_KEYHELD;

      if ( pUsr->evt.pDat3 )
      {
         /* If this is a key being released.  Stop the key held timer.
         */
         if ( *((EPTDTMF *)pUsr->evt.pDat3) == eEPTDTMF_TONEOFF )
         {
            cmTmrStop ( &tmr );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmFlashEmergency
**
** PURPOSE:    FSM hook flash handler for an emergency session.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFlashEmergency ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMSES *pOtherSes = NULL;
   BOS_UINT32 uIx;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL b3WayCall = BOS_FALSE;
   CFGSIPEMERGE *pEmergCfg = NULL;
   CFGSIPBASICCALL *pBasicCall = NULL;
   BOS_BOOL bIgnoreHFOosCall = BOS_FALSE;

   /* Make sure this is for an emergency session.
   ** Not applicable to internal calls.
   */
   if ( pSes && pSes->ses.med.bEmergency &&
        ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
   {
      CMGRLOG_INFORMATION (( "cmFsmFlashEmergency: 0x%X (0x%X)",
                             pUsr->uHdl, pSes->uNetUsr ));
      /* In general, we would ignore flash event during an emergency session, this
      ** is to prevent any misbehavior from the user and to avoid putting such
      ** emergency session on hold.  There is however one special case to consider
      ** which is when we already have another session active on the user and we
      ** are now trying to create a 3-way conference including this session and the
      ** emergency session.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                 &cfgIdx,
                                 (void *) &pEmergCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pEmergCfg &&
              pEmergCfg->bEstab3WC )
         {
            b3WayCall = BOS_TRUE;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                               &cfgIdx,
                               (void *) pEmergCfg,
                               CFG_ITEM_DEL );
         pEmergCfg = NULL;
      }

      /* Determine if hook flash processing requires termination of
      ** out-of-service calls. If so instead of termination, mark that
      ** the hookflash needs to be ignored because we're in an emergency
      ** call.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;

      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pBasicCall &&
              pBasicCall->bHfTermOosCall )
         {
            bIgnoreHFOosCall = BOS_TRUE;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }

      /* Check or locate other active session.
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

      if ( pOtherSes && b3WayCall)
      {
         if ( ( pSes->ses.med.uConf == CMGR_INVALID_HANDLE ) &&
              ( pOtherSes->ses.med.uConf == CMGR_INVALID_HANDLE ) )
         {
            if( !bIgnoreHFOosCall ||
                ( bIgnoreHFOosCall && !pOtherSes->bMaintdOnOos ))
            {
               if ( cmSesConferenceLocal ( pSes, pOtherSes ) )
               {
                  pSes->ses.med.uConf = pOtherSes->uHdl;
                  pOtherSes->ses.med.uConf = pSes->uHdl;

                  cctkCallUnhold ( pOtherSes->uNet );
               }
            }
            else
            {
               CMGRLOG_WARNING ((
               "cmFsmFlashEmergency: ignoring, call 0x%x (0x%X) was maintained on out of service",
               pOtherSes->uHdl, pOtherSes->uNet ));
            }

         }
      }
      else
      {
         CMGRLOG_WARNING ((
            "cmFsmFlashEmergency: ignoring, emergency call 0x%x (0x%X)",
            pSes->uHdl, pSes->uNet ));
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmNoActivity
**
** PURPOSE:    FSM no activity detected on a connection.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmNoActivity ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmNoActivity: 0x%X (0x%X) on 0x%X (0x%X)",
                             pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));

      /* We treat this event the same as having encountered a session
      ** event failure, which in this case since there is no session
      ** related information will lead to assuming the session has
      ** failed and which will terminate the call right away, which is
      ** the intended behavior in the situation where no activity was
      ** reported.
      */
      cmFsmSesStatus ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmHoldSvc
**
** PURPOSE:    FSM received an HOLD service request from controlling application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmHoldSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmHoldSvc: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet,
                             pUsr->uHdl, pSes->uNetUsr ));

#if CMGRCFG_MULTILINE
      /* Check if there is another session */
      CMSES *pOtherSes = NULL;
      BOS_UINT32 uIx;
      for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
      {
         if ( pUsr->pMedSes [ uIx ]  && pUsr->pMedSes [ uIx ] != pSes )
         {
            pOtherSes = pUsr->pMedSes [ uIx ];
            break;
         }
      }

      /* For multiline, clear the CW session flag so that
      ** we dont automatically switch over to cwsession
      */
      pUsr->pSesWait = NULL;
#endif /* CMGRCFG_MULTILINE */

      if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Apply service via the service framework.
         */
         cctkCallHold (  pSes->uNet );
      }
      else
      {
         /* Send a hold ok event */
         CMEVT cmEvt;

         memset( &cmEvt, 0, sizeof( CMEVT ) );

         cmEvt.uEvt = ( eCMEVT_HOLD_LOCAL | CMGR_EVT_CLASS__CCTK );
         cmEvt.uDat1 = pSes->uUsr;
         cmEvt.uDat2 = pSes->uHdl;
         cmEvt.uDat4 = pSes->uNetUsr;
         cmEvt.bLast = BOS_TRUE;

         if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }
      }

      /* Make sure we are in the proper state to manage the response to this
      ** service request.
      */
#if CMGRCFG_MULTILINE
      /* For multiline, set to flash state since we might
      ** want to start digit collection
      */
      if ( pOtherSes == NULL )
      {
         /* If no other session go into flash state */
         cmUsrNewState ( pUsr, eCMST_FLASH );
      }
#else
      cmUsrNewState ( pUsr, eCMST_TALK );
#endif /* CMGRCFG_MULTILINE */
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmUnHoldSvc
**
** PURPOSE:    FSM received an UN-HOLD service request from controlling application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmUnHoldSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmUnHoldSvc: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet,
                             pUsr->uHdl, pSes->uNetUsr ));

      if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Apply service via the service framework.
         */
         cctkCallUnhold (  pSes->uNet );
      }
      else
      {
         /* Send an unhold ok event */
         CMEVT cmEvt;

         memset( &cmEvt, 0, sizeof( CMEVT ) );

         cmEvt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK );
         cmEvt.uDat1 = pSes->uUsr;
         cmEvt.uDat2 = pSes->uHdl;
         cmEvt.uDat4 = pSes->uNetUsr;
         cmEvt.bLast = BOS_TRUE;

         if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }
      }

      /* Make sure we are in the proper state to manage the response to this
      ** service request.
      */
      cmUsrNewState ( pUsr, eCMST_TALK );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmConfSvc
**
** PURPOSE:    FSM received an CONF service request from controlling application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This service is used for local and network conference. The service is
**             only applicable to two sessions that are controlled by the same
**             user.
**
** DESIGN:     Contrary to the primary line behavior which is directly handled
**             through the flash event framework, local conference service takes
**             no part into potential unholding of a media session to ensure
**             the media flows properly between all conference participants.  The
**             external application is responsible to do so as needed, and the
**             the bLocalConf event flag must not be enabled.
***********************************************************************************/
void cmFsmConfSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_BOOL bValid = BOS_FALSE;
   CMSES *pSes_1 = NULL;
   CMSES *pSes_2 = NULL;
   BOS_BOOL bLocal = BOS_FALSE;

   if ( pUsr->evt.pDat3 )
   {
      pSes_1 = cmSesValid ( ((CMGRCONFSVC *) pUsr->evt.pDat3)->uAppIx );
      pSes_2 = cmSesValid ( ((CMGRCONFSVC *) pUsr->evt.pDat3)->uSesHdl );


      /* Determine whether local conferencing is only needed or whether
      ** media needs to be managed for potential held streams as well.
      */
      bLocal = ((CMGRCONFSVC *) pUsr->evt.pDat3)->bLocalConf;

      if ( pSes_1 && pSes_2 )
      {
         CFGSIPTHREEWAYCALL *p3WayCall = NULL;
         CFGSIPHOTLINE *pHotLineCfg = NULL;
         PROV_CFG_IDX cfgIdx;
         BOS_BOOL b3WayCall = BOS_FALSE;

         CMGRLOG_INFORMATION ((
            "cmFsmConfSvc: 0x%X (0x%X) and 0x%X (0x%X) on 0x%X (0x%X)",
            pSes_1->uHdl, pSes_1->uNet,
            pSes_2->uHdl, pSes_2->uNet,
            pUsr->uHdl, pSes_1->uNetUsr ));

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                                    &cfgIdx,
                                    (void *) &p3WayCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( p3WayCall && p3WayCall->Act.bCfgStatus )
            {
               b3WayCall = BOS_TRUE;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                                  &cfgIdx,
                                  (void *) p3WayCall,
                                  CFG_ITEM_DEL );
            p3WayCall = NULL;
         }

         /* Do not allow the 3WC if Hotline is active and is configured to disable
         ** traditional hookflash features.
         */
         if ( b3WayCall &&
            ( pSes_1->ses.med.bHotline || pSes_2->ses.med.bHotline ) )
         {
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                       &cfgIdx,
                                       (void *) &pHotLineCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pHotLineCfg && pHotLineCfg->Act.bCfgStatus )
               {
                  b3WayCall = !pHotLineCfg->bDisableHfFeat;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                     &cfgIdx,
                                     (void *) pHotLineCfg,
                                     CFG_ITEM_DEL );
               pHotLineCfg = NULL;
            }
         }

         /* Cannot conference a session that is on permanent-hold.
         */
         if ( b3WayCall &&
               pUsr->pSesHeld &&
                  (( pUsr->pSesHeld == pSes_1 ) || ( pUsr->pSesHeld == pSes_2 )) )
         {
            b3WayCall = BOS_FALSE;
         }

         /* Cannot conference a waiting session.
         */
         if ( b3WayCall &&
               pUsr->pSesWait &&
                  (( pUsr->pSesWait == pSes_1 ) || ( pUsr->pSesWait == pSes_2 )) )
         {
            b3WayCall = BOS_FALSE;
         }

         /* The sessions can be conferenced.
         */
         if ( b3WayCall &&
              ( pSes_1->ses.med.uConf == CMGR_INVALID_HANDLE ) &&
              ( pSes_2->ses.med.uConf == CMGR_INVALID_HANDLE ) )
         {
            CMSES *pOtherSes = NULL;

            /* If this is not a local conference we will need to
            ** manage any sessions that are on hold/muted.
            */
            if ( !bLocal )
            {
               /* The held session won't be the focus.
               */
               if ( pSes_1->ses.med.bFocus &&
                    !pSes_2->ses.med.bFocus)
               {
                  pOtherSes = pSes_2;
               }
               else if ( !pSes_1->ses.med.bFocus &&
                         pSes_2->ses.med.bFocus )
               {
                  pOtherSes = pSes_1;
               }
            }

            /* Conference these sessions.
            */
            if ( cmSesConferenceLocal ( pSes_1, pSes_2 ) )
            {
               pSes_1->ses.med.uConf = pSes_2->uHdl;
               pSes_2->ses.med.uConf = pSes_1->uHdl;


               /* Apply an unhold so that all parties can
               ** participate.
               */
               if ( pOtherSes )
               {
                  if( pOtherSes->uNetUsr != CMGR_INVALID_HANDLE )
                  {
                     cctkCallUnhold ( pOtherSes->uNet );
                  }
                  else
                  {
                     CMEVT cmEvt;

                     memset( &cmEvt, 0, sizeof( CMEVT ) );

                     cmEvt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
                     cmEvt.uDat1 = pOtherSes->uUsr;    /* User handle */
                     cmEvt.uDat2 = pOtherSes->uHdl;    /* Session handle */
                     cmEvt.uDat4 = pOtherSes->uNetUsr; /* CCTK User */
                     cmEvt.bLast = BOS_TRUE;           /* Last event */

                     if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                     {
                        cmSignalEvt ();
                     }
                  }
                  /* Move to the talking state
                  */
                  cmUsrNewState( pUsr, eCMST_TALK );
               }

               bValid = BOS_TRUE;
            }
         }
      }
   }

   /* Issue a service status indication back to the application.
   */
   if ( gCmCfgCb.cmgrEvtCb )
   {
      CMGRSVC eSvc = eCMGRSVC_CONF;
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes_1->uNetUsr );

      ( gCmCfgCb.cmgrEvtCb ) ( (bValid ? eCMGREVT_SVC_APPLY : eCMGREVT_SVC_DENY),
                               (pSes_1 ? pSes_1->uNet : CMGR_INVALID_HANDLE),
#if CMGRCFG_MULTILINE
                               pUsr->uHdl,
                               (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                               (void *) &eSvc );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmBxferSvc
**
** PURPOSE:    FSM received an BLIND TRANSFER service request from controlling
**             application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmBxferSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CFGSIPCALLXFER *pXferCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bXfer = BOS_FALSE;

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmBxferSvc: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet,
                             pUsr->uHdl, pSes->uNetUsr ));

      memset ( &cfgIdx, 0 , sizeof ( PROV_CFG_IDX ) );
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                    &cfgIdx,
                                    (void *) &pXferCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pXferCfg && pXferCfg->Act.bCfgStatus &&
              /* do not allow call transfer if bIncomingOnly is set and
              ** the call is not incoming
              */
              !( pXferCfg->bIncomingOnly && !pSes->ses.med.bRemInit )
            )
         {
            bXfer = BOS_TRUE;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                               &cfgIdx,
                               (void *) pXferCfg,
                               CFG_ITEM_DEL );
         pXferCfg = NULL;
      }

      /* Apply blind transfer on this session.
      */
      if ( bXfer )
      {
         CMGRLOG_INFORMATION (( "cmFsmBxferSvc: transfer to \'%s\'",
                                ((CMGRPLACECALL *) pUsr->evt.pDat3)->uDest ));

         cctkCallTransferBlind( pSes->uNet,
                                (const BOS_UINT8 *) (
                                    ((CMGRPLACECALL *) pUsr->evt.pDat3)->uDest) );
      }
   }

   /* If we failed to transfer, return the status now, otherwise final status
   ** will be published with the result of the transfer.
   */
   if ( !bXfer && gCmCfgCb.cmgrEvtCb )
   {
      CMGRSVC eSvc = eCMGRSVC_BXFER;
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

      ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_DENY,
                               (pSes ? pSes->uNet : CMGR_INVALID_HANDLE),
#if CMGRCFG_MULTILINE
                               pUsr->uHdl,
                               (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                               (void *) &eSvc );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCxferSvc
**
** PURPOSE:    FSM received an CONSULTATIVE TRANSFER service request from
**             controlling application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmCxferSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes_1 = NULL;
   CMSES *pSes_2 = NULL;
   CFGSIPCALLXFER *pXferCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bXfer = BOS_FALSE;

   if ( pUsr->evt.pDat3 )
   {
      pSes_1 = cmSesValid ( ((CMGRAPPCALLHDL *) pUsr->evt.pDat3)->uAppIx );
      pSes_2 = cmSesValid ( ((CMGRAPPCALLHDL *) pUsr->evt.pDat3)->uSesHdl );
   }

   if ( pSes_1 && pSes_2 )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCxferSvc: 0x%X (0x%X) and 0x%X (0x%X) on 0x%X (0x%X)",
         pSes_1->uHdl, pSes_1->uNet,
         pSes_2->uHdl, pSes_2->uNet,
         pUsr->uHdl, pSes_1->uNetUsr ));

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                    &cfgIdx,
                                    (void *) &pXferCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pXferCfg && pXferCfg->Act.bCfgStatus &&
              /* do not allow call transfer if bIncomingOnly is set and
              ** the call is not incoming
              */
              !( pXferCfg->bIncomingOnly && !pSes_2->ses.med.bRemInit )
            )
         {
            bXfer = BOS_TRUE;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                               &cfgIdx,
                               (void *) pXferCfg,
                               CFG_ITEM_DEL );
         pXferCfg = NULL;
      }

      /* Apply consultative transfer on this session.
      */
      if ( bXfer )
      {
         cctkCallTransfer( pSes_2->uNet, pSes_1->uNet, BOS_FALSE );
      }
   }

   /* If we failed to transfer, return the status now, otherwise final status
   ** will be published with the result of the transfer.
   */
   if ( !bXfer && gCmCfgCb.cmgrEvtCb )
   {
      CMGRSVC eSvc = eCMGRSVC_CXFER;
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes_1 ? pSes_1->uNetUsr : CMGR_INVALID_HANDLE) );

      ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_DENY,
                               (pSes_1 ? pSes_1->uNet : CMGR_INVALID_HANDLE),
#if CMGRCFG_MULTILINE
                               pUsr->uHdl,
                               (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                               (void *) &eSvc );
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmInternalCallSvc
**
** PURPOSE:    FSM received an INTERNAL CALL service request from
**             controlling application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmInternalCallSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMGRINTERNALCALL* pIntCallInfo =  pUsr->evt.pDat3;

   /* The user can only make an internal call if
   ** 1) not already setting up an internal call
   ** 2) the destination user was specified
   ** 3) a session was succesfully allocated to service the call
   */
   if ( ( !pUsr->bIntCallDet ) &&
         (( pIntCallInfo != NULL ) &&  ( pSes != NULL )))
   {

      CMGRLOG_INFORMATION (( "cmFsmInternalCallSvc: for usr(0x%X) on ses(0x%X) to dstPhy(0x%X)",
                             pUsr->uHdl, pSes->uHdl, pIntCallInfo->uPhyRemote ));

      CMUSRLIST usrList;
      BOS_BOOL bCallAll = BOS_FALSE;
      CMGREPSIG sig;
      CMTMR tmr;

      memset ( &sig, 0, sizeof ( CMGREPSIG ) );
      memset( &usrList, 0, sizeof( CMUSRLIST ) );


      /* Stop dial tone and dial tone timer.
      */
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;   /* eEPTSIG_DIAL or eEPTSIG_SPECIAL_DIAL */
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      tmr.uUsr = pUsr->uHdl;
      tmr.uSes = CMGR_INVALID_HANDLE;
      tmr.eType = eCMEVT_TMR_DIALTONE;

      cmTmrStop ( &tmr );

      /* Stop warm line timer in case it was running.
      */
      if ( cmFeatEnabled( pRes, eCMDM_HOTLINEINT ) )
      {
         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_TMR_WARMLINE;

         cmTmrStop ( &tmr );
      }

      /* Setup the internal call session
      */
      pSes->uNetUsr = CMGR_INVALID_HANDLE; /* No CCTK user */
      pSes->uNet = cctkGenerateId(); /* Generate a unique call id */
      if( !pSes->bEstab )
      {
         pSes->bEstab = BOS_TRUE;
      }

      pUsr->bIntCallDet = BOS_TRUE;

      /* If a specific user is not specfied then create a list of
      ** all users to ring.
      */
      if ( pIntCallInfo->uPhyRemote == CMGR_INVALID_HANDLE )
      {
         /* Get a list of all users, exluding ourselves */
         cmUsrGetAll( &usrList, pUsr );
         bCallAll = BOS_TRUE;
      }
      /* Otherwise add only the specifed user to user list
      */
      else
      {
         CMUSR *pUsrDst = cmUsrFindPhyHdl( pIntCallInfo->uPhyRemote );

         if( pUsrDst &&
            ( pUsrDst->uHdl != pUsr->uHdl ) )
         {
            usrList.uUsr[ 0 ] = pUsrDst->uHdl;
            usrList.uLength++;
         }
      }

      /* Call the required users.
      */
      cmFeatCallInt( pRes, &usrList, bCallAll);

   }
   else
   {
       CMGRLOG_DEBUG (( "cmFsmInternalCallSvc: Failed - %s%s%s for usr(0x%X)",
                             pSes? "" : "no session, ",
                             pIntCallInfo? "": "no destination info, ",
                             pUsr->bIntCallDet? "internal call in progress, " : "",
                             pUsr->uHdl));
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmAnsSvc
**
** PURPOSE:    FSM received an ANSWER service request from controlling
**             application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmAnsSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pSesFocus = cmUsrFocusSes ( pUsr );
   CMTMR tmr;
   CMGREPSIG sig;

   memset ( &tmr, 0, sizeof ( CMTMR ) );
   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmAnsSvc: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet,
                             pUsr->uHdl, pSes->uNetUsr ));

      /* If this is the only call on the system, apply 'off-hook' processing to
      ** control the answer behavior.
      */
      if ( (pSes == pSesFocus) && (cmUsrActiveSes ( pUsr ) == NULL) )
      {
         cmFsmOffHook ( pRes );
      }
      /*
      ** Otherwise, this is to answer a call waiting (most probably).
      */
      else
      {
#if CMGRCFG_MULTILINE
          CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */
         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_CALLWAIT;
         if ( cmTmrIsRunning ( &tmr ) )
         {
            cmTmrStop ( &tmr );
         }

         /* Swap the focus session if needed.
         */
         if ( pSesFocus )
         {
            if ( pSesFocus != pSes )
            {
               pSesFocus->ses.med.bFocus = BOS_FALSE;
               pSes->ses.med.bFocus = BOS_TRUE;
            }
         }

         pSes->bEstab = BOS_TRUE;

         if ( pSes->ses.med.bNeedFinal )
         {
            pSes->ses.med.bNeedFinal = BOS_FALSE;
            cctkCallMediaNotify( pSes->uNet,
                                 eCCTKMUDPRSN_FINALIZECAPS );
         }

         if ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
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

            /* Force a media update to ensure the connection is setup
            ** properly since the media information was known already.
            */
            cmSesPhyUpdate ( pSes, BOS_FALSE );
        }

        /* Notify the application that the call has connected */
        if ( gCmCfgCb.cmgrEvtCb )
        {
          ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                   pSes->uNet,
#if CMGRCFG_MULTILINE
                                   pUsr->uHdl,
                                   ((pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                   (void *) pUsr->uUid );
         }
         /* Answer the call.
         */
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
         cmFeatFaxOnly ( pRes );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmEndSvc
**
** PURPOSE:    FSM received an ENDCALL service request from controlling
**             application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmEndSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMTMR tmr;

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmEndSvc: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet,
                             pUsr->uHdl, pSes->uNetUsr ));

      /* Terminate processing associated with the session as applicable.
      */
      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_RINGING;
      if ( cmTmrIsRunning ( &tmr ) )
      {
         cmTmrStop ( &tmr );
      }

      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_CALLWAIT;
      if ( cmTmrIsRunning ( &tmr ) )
      {
         cmTmrStop ( &tmr );
      }

      if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Terminate the session as asked for.
         */
         cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
      }
      cmSesPhyDelete ( pSes );
      cmUsrUnLinkSes ( pUsr, pSes );
      /*
      ** Reset the network handle and free the session resource.
      */
      pSes->uNet = CMGR_INVALID_HANDLE;
      cmSesFree ( pSes, pUsr->uHdl );

      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_ENDED,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pUsr->uHdl,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) pUsr->uUid );
      }

      /* If we ended the focus session, do a bit more work...
      */
      if ( cmUsrFocusSes ( pUsr ) == NULL )
      {
         /* If there is an active session remaining, promote it to be the
         ** focus session.
         */
         pSes = cmUsrActiveSes ( pUsr );

         if ( pSes != NULL )
         {
            pSes->ses.med.bFocus = BOS_TRUE;
         }
         /*
         ** If there are no other session associated with this user, clean
         ** up completely.
         */
         else
         {
            cmFsmOnHook ( pRes );
            cmUsrNewState ( pUsr, eCMST_IDLE );
         }
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmDigitRelay
**
** PURPOSE:    FSM digit detect handler while in TALK
**
** PARAMETERS: pRes - the pointer on which to the resource to run the FSM.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmDigitRelay ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   /* Cancel any key timers */
   cmFsmCancelKeyTmr( pRes );

   /* Process key press and key release events.
   */
   if ( (pUsr->evt.uEvt != eCMEVT_TMR_KEYHELD) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_DIALTONE) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_SHORT) &&
        (pUsr->evt.uEvt != eCMEVT_TMR_INTERDIGIT_LONG) )
   {
      if ( pUsr &&
           ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]
                                                             >= eCMEVT_DTMF_0 ) &&
           ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ]
                                                             <= eCMEVT_DTMF_D ) )
      {
         if ( pUsr->evt.pDat3 && *((EPTDTMF *)pUsr->evt.pDat3) == eEPTDTMF_TONEON )
         {
            /* Only process tone-off events */
            return;
         }
      }
   }

   if ( pSes )
   {
      CFGSIPBASICCALL *pBasicCall = NULL;
      char digit;
      PROV_CFG_IDX cfgIdx;
      CMGRLOG_DEBUG (( "cmFsmDigitRelay: 0x%X (0x%X) on 0x%X ",
                       pSes->uHdl, pSes->uNet,
                       pUsr->uHdl ));

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         /* Issue in dialog request containing the DTMF information if configured
         ** to do so.
         */
         if ( pBasicCall && pBasicCall->bInDialogDtmf )
         {
            switch( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ] )
            {
               case eCMEVT_DTMF_STAR:
               {
                  digit = '*';
               }
               break;

               case eCMEVT_DTMF_HASH:
               {
                  digit = '#';
               }
               break;

               default:
               {
                  digit = pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ] - eCMEVT_DTMF_0 + '0';
               }
               break;
            }

            CMGRLOG_INFORMATION ((
                  "cmFsmDigitRelay: 0x%X (0x%X) on 0x%X - snd DTMF %c",
                  pSes->uHdl, pSes->uNet,
                  pUsr->uHdl,
                  digit ));

            cctkCallDtmfInfo ( pSes->uNet,
                               digit,
                               /* Pass default tone duration if no other information
                               ** on the actual duration has been given. */
                               (BOS_UINT32) CM_FSM_PHY__DTMF_INFO__DURATION_MSEC );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmDtmfPtSvc
**
** PURPOSE:    FSM received a DTMF passthrough service request from controlling
**             application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmDtmfPtSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMGRAPPCALLHDL *pDtmfSvc = (CMGRAPPCALLHDL *) pUsr->evt.pDat3;

   if ( pSes && pDtmfSvc &&
        ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
   {
      CFGSIPBASICCALL *pBasicCall = NULL;
      PROV_CFG_IDX cfgIdx;

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         /* Issue in dialog request containing the DTMF information if configured
         ** to do so.
         */
         if ( pBasicCall && pBasicCall->bInDialogDtmf )
         {
            CMGRLOG_INFORMATION ((
                  "cmFsmDtmfPtSvc: 0x%X (0x%X) on 0x%X (0x%X) - snd DTMF %c",
                  pSes->uHdl, pSes->uNet,
                  pUsr->uHdl, pSes->uNetUsr,
                  pDtmfSvc->uSesHdl ));

            cctkCallDtmfInfo ( pSes->uNet,
                               pDtmfSvc->uSesHdl,
                               /* Pass default tone duration if no other information
                               ** on the actual duration has been given. */
                               (BOS_UINT32) CM_FSM_PHY__DTMF_INFO__DURATION_MSEC );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmIngressDtmfSvc
**
** PURPOSE:    FSM received an ingress DTMF service request from controlling
**             application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmIngressDtmfSvc( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   BOS_UINT32 uDigit = (BOS_UINT32)pUsr->evt.pDat3;
   CMGREPSIG sig;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = (pSes) ? cmCctkAccntFindByNet( pSes->uNetUsr ) : NULL;
#endif /* CMGRCFG_MULTILINE */

   if ( pSes )
   {
      sig.eAct = eCMGREPSIGACT_START;
      sig.uSes = pSes->ses.med.uPhy;
      sig.uApp = pSes->uApp;
      sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = eEPTSIG_INGRESS_DTMF;
      sig.pData = (void*)uDigit;

      CMGRLOG_INFORMATION ((
               "%s: 0x%X (0x%X) on 0x%X - ingress DTMF %X",
               __FUNCTION__,
               pSes->uHdl, pSes->uNet,
               pUsr->uHdl,
               uDigit ));

      cmPhySignal ( pUsr->uPhys, &sig );
   }
}


#if CMGRCFG_MULTILINE
/***********************************************************************************
** FUNCTION:   cmFsmOffHookPreemption
**
** PURPOSE:    FSM OffHook handler for alarm preemption
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This function will be invoked when a port that is configured as
**             alarm preemptive is offhook.  This function will terminate any
**             existing calls associated with the same account.
***********************************************************************************/
void cmFsmOffHookPreemption ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMUSR *pOtherUsr;
   CMUSRLIST cmUsrList;
   BOS_UINT32 uIx, uIy;
   BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

   /* release or terminate any existing call to the PacketCable network
   ** associated with the same TN assigned to the preemptive alarm line
   */
   BOS_UINT32 uActiveCalls = cmCctkNumActiveCalls( uAccount );
   if (uActiveCalls > 0)
   {
      memset( &cmUsrList, 0, sizeof( CMUSRLIST ) );
      cmGetAssocUsrList( uAccount, &cmUsrList );

      /* Process the list of users for this event that we have compile so far. */
      for( uIx = 0 ; uIx < cmUsrList.uLength ; uIx++ )
      {
         /* Get the user from the list */
         pOtherUsr = cmUsrValid( cmUsrList.uUsr[ uIx ] );

         if (pOtherUsr == NULL)
         {
            /* user not valid */
            continue;
         }

         /* clean up any other media sessions */
         for ( uIy = 0 ; uIy < (CMGRCFG_MEDSES_MAX + 1) ; uIy++ )
         {
            if ( pOtherUsr->pMedSes [ uIy ] )
            {
               cctkCallDelete ( pOtherUsr->pMedSes [ uIy ]->uNet,
                                eCCTKCALLDELRSN_NORMAL );
               cmUsrUnLinkSes ( pOtherUsr, pOtherUsr->pMedSes [ uIy ] );
               cmUsrNewState ( pOtherUsr, eCMST_RELEASING );
            }
         }
      }
   }
}
#endif

/***********************************************************************************
** FUNCTION:   cmFsmRRFeature
**
** PURPOSE:    FSM digit detection (outside of dialing phase) handler for
**             Register Recall style flash handling, used to detect specific
**             feature activation/de-activation and act in consequence.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This function is invoked when a key is pressed while the user
**             is in Register Recall Wait State.
**
**             The following are valid Key Codes:
**             R1 - If 3pty call is active, will disconnect the last call;
**                  If 3pty is not active, will hang up on current call and
**                  switch back to waiting call.
**             R2 - If there is an incoming call, will put current call on hold
**                  and answer the call waiting call.
**                  If there is an active call and a call that is on hold, will
**                  toggle between the calls.
**             R3 - Merge the two calls for conference calling.
**
***********************************************************************************/
void cmFsmRRFeature ( void *pRes )
{
   CFGEPTCODECCFG *pCodecCfg = NULL;
   CCTKCALLMETRICS stats;
   BOS_UINT8 cPubTo[CMGR_IP_ADDRESS_STRING_LEN+1];
   CFGSIPTHREEWAYCALL *p3WayCall = NULL;
   PROV_CFG_IDX cfgIdx;
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMSES *pOtherSes = NULL;
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] )
      {
         if ( pUsr->pMedSes [ uIx ] != pSes )
         {
            pOtherSes = pUsr->pMedSes [ uIx ];
         }
      }
   }

   /* Check the keycode */
   switch ( CMGR_EVT_CLASS__EVT_MASK & pUsr->evt.uEvt )
   {
      /* 1.  [R1] will disconnect the last call (if 3pty call is active)
      ** OR
      ** 2.  [R1] will disconnect the current call
      **     (if another call is waiting -> switch back to the waiting call)
      */
      case eCMEVT_DTMF_1:
      {
         /* only allow R1 if both sessions are established */
         if ( !pSes->bEstab || !pOtherSes->bEstab )
         {
            CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R1 abandoned due to unestablished call",
                                    pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

            cmFsmRRRevert ( pRes );
            return;
         }

         if ( pSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            cPubTo[0] = '\0';
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
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

               status = bosIpAddrCreateFromStr( (char *)&cPubTo, &pubTo);
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
                   sprintf ( (char *)ipAddr, "]" );
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

         if ( ( pSes->ses.med.uConf == pOtherSes->uHdl ) &&
              ( pOtherSes->ses.med.uConf == pSes->uHdl ) )
         {
            CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R1: Hang up the last call",
                                   pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

            /* 3pty call is active, R1 will disconnect the last call  */
            if( ( cmNumSesWithCid( pSes->uNet ) == 1 ) &&
                ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
            {
               cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
            }
            /* If this is an internal call and one side has hung up, signal both sides
            ** with a "RELEASE_CALL" event.
            */
            else if( ( cmNumSesWithCid( pSes->uNet ) == 2 ) &&
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

                  if( ( pSesRelease ) &&
                      ( pUsrRelease ) )
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

            pSes->ses.med.uConf = CMGR_INVALID_HANDLE;
            pOtherSes->ses.med.uConf = CMGR_INVALID_HANDLE;

            pSes->ses.med.bFocus = BOS_FALSE;
            pOtherSes->ses.med.bFocus = BOS_TRUE;

            /* Move back to the talking state
            */
            cmUsrNewState( pUsr, eCMST_TALK );
         }
         else
         {
            /*  Not a 3pty call.  R1 will disconnect the current call
            ** (if another call is waiting -> switch back to the waiting call)
            */
            CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R1: Hang up the current call",
                                   pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

            cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );

            /* Move back to the talking state
            */
            cmUsrNewState( pUsr, eCMST_TALK );
         }
      }
      break;

      case eCMEVT_DTMF_2:
      {
         /*
         ** For the RR Feature, R2 is not allowed when there is a 3 party call
         */
         if ( ( pSes->ses.med.uConf == pOtherSes->uHdl ) &&
              ( pOtherSes->ses.med.uConf == pSes->uHdl ) )
         {
            CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R2 abandoned due to 3pty call in progress",
                                   pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));
            cmFsmRRRevert ( pRes );
            return;
         }

         if ( pUsr->pSesWait )
         {
            /* Two sessions are active.
            **
            ** One is a call waiting session, the other is the currently active
            ** session.
            **
            */
            CMTMR tmr;
            memset ( &tmr, 0, sizeof ( CMTMR ) );

            tmr.uSes = pUsr->pSesWait->uHdl;
            tmr.uUsr = pUsr->uHdl;
            tmr.eType = eCMEVT_TMR_CALLWAIT;

            cmTmrStop ( &tmr );
         }
         else
         {
            /* No current pSesWait, make sure both sessions have been established
            ** do not allow toggling two calls when there is an unestablished call
            */
            if ( !(pSes->bEstab) || !(pOtherSes->bEstab) )
            {
               CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R2 abandoned due to unestablished call",
                                       pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

               cmFsmRRRevert ( pRes );
               return;
            }
            /* If we need to toggle the calls, we should
            ** set the other session as the call waiting session
            */
            pUsr->pSesWait = pOtherSes;
         }

         CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R2 Toggle calls: pSes=0x%X, pOtherSes=0x%X, pSesWait=0x%X",
                                pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ), pSes, pOtherSes, pUsr->pSesWait ));

         /* If this is an external call, tell CCTK to put the call on hold. */
         if( cmUsrGetNtwkHdl(pUsr) != CMGR_INVALID_HANDLE )
         {
            /* Put the focus call on hold.
            */
            cctkCallHold ( pSes->uNet );
         }
         /* If this is an internal call, run the hold state machine right away */
         else
         {
            /* Send a hold event */
            CMEVT evt;

            memset ( &evt, 0, sizeof( CMEVT ) );

            evt.uEvt = ( eCMEVT_HOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
            evt.uDat1 = pSes->uUsr;     /* User handle */
            evt.uDat2 = pSes->uHdl;     /* Session handle */
            evt.uDat4 = pSes->uNetUsr;  /* CCTK user id */
            evt.bLast = BOS_TRUE;       /* Last event */

            if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
            {
               cmSignalEvt ();
            }
         }
      }
      break;

      case eCMEVT_DTMF_3:
      {
         /* For the RR Feature, R3 is only allowed to establish a 3 party call when
         ** there is an active call and a call that is on hold (regardless how the two
         ** call legs are made up).
         */

         /* do not allow merging two calls for conference when there is an
         ** unanswered call waiting call
         */
         if ( pUsr->pSesWait && !pUsr->pSesWait->bEstab  )
         {
            CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R3 abandoned due to unanswerd CW call",
                                    pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

            cmFsmRRRevert ( pRes );
            return;
         }

         /* do not allow merging two calls for conference when there is an
         ** un-established call
         */
         if ( !pSes->bEstab || !pOtherSes->bEstab  )
         {
            CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R3 abandoned due to unestablished call",
                                    pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

            cmFsmRRRevert ( pRes );
            return;
         }

         CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - R3 Merge two calls for conference",
                                pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

         /* for the purpose of hook flash with key processing, we will only process key code 3 if
         ** conference (3-way call) is enabled and the sessions are not conferenced in yet.
         ** Otherwise, the key pressed is ignored
         */
         BOS_BOOL b3WayCall = BOS_FALSE;
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                                    &cfgIdx,
                                    (void *) &p3WayCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( p3WayCall && p3WayCall->Act.bCfgStatus )
            {
               b3WayCall = BOS_TRUE;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                                  &cfgIdx,
                                  (void *) p3WayCall,
                                  CFG_ITEM_DEL );
            p3WayCall = NULL;
         }

         if ( b3WayCall )
         {
            if ( ( pSes->ses.med.uConf == CMGR_INVALID_HANDLE ) &&
                 ( pOtherSes->ses.med.uConf == CMGR_INVALID_HANDLE ) )
            {
               if ( cmSesConferenceLocal ( pSes, pOtherSes ) )
               {
                  pSes->ses.med.uConf = pOtherSes->uHdl;
                  pOtherSes->ses.med.uConf = pSes->uHdl;

                  if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
                  {
                     cctkCallUnhold ( pOtherSes->uNet );
                  }
                  else
                  {
                     CMEVT cmEvt;

                     memset( &cmEvt, 0, sizeof( CMEVT ) );

                     cmEvt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
                     cmEvt.uDat1 = pOtherSes->uUsr;    /* User handle */
                     cmEvt.uDat2 = pOtherSes->uHdl;    /* Session handle */
                     cmEvt.uDat4 = pOtherSes->uNetUsr; /* CCTK User */
                     cmEvt.bLast = BOS_TRUE;           /* Last event */

                     if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                     {
                        cmSignalEvt ();
                     }
                  }
                  /* Move back to the talking state
                  */
                  cmUsrNewState( pUsr, eCMST_TALK );
               }
            }
         }
      }
      break;

      default:
      {
         CMGRLOG_INFORMATION (( "cmFsmRRFeature: 0x%X (0x%X) - Unsupported Key Pressed",
                                pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

         /* restore to previous state */
         cmFsmRRRevert ( pRes );
      }
      break;

   }
   /* reset bRRWaitKey to false */
   pUsr->rrFlash.bRRWaitKey = BOS_FALSE;

}

/***********************************************************************************
** FUNCTION:   cmFsmRRRevert
**
** PURPOSE:    Handler for Register Recall style flash, used to revert the flash
**             feature behavior upon error or cancellation
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRRRevert ( void *pRes )
{
   CMTMR tmr;
   CMGREPSIG sig;
   CMUSR *pUsr = (CMUSR *) pRes;

   CMGRLOG_INFORMATION (( "cmFsmRRRevert: 0x%X (0x%X) - Revert Recall Wait State",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   /* Stop stop dial tone and dial tone timer.
   */
   if ( pUsr->uCpt == eEPTSIG_SPECIAL_DIAL )
   {
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );
   }

   tmr.uUsr = pUsr->uHdl;
   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.eType = eCMEVT_TMR_DIALTONE;

   if ( cmTmrIsRunning ( &tmr ) )
   {
      cmTmrStop ( &tmr );
   }

   /* restore user state */
   cmUsrNewState ( pUsr, pUsr->rrFlash.ePrevSta );
   pUsr->rrFlash.bRRWaitKey = BOS_FALSE;
}

