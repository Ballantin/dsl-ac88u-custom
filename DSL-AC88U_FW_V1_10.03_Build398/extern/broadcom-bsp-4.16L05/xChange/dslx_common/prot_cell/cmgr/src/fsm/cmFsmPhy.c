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
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmNoOp,
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
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmNoOp,
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
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmInvalid,
   },

   /* ---- eCMST_RINGBACK ------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmNoOp,
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
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_MEDIA ---------- */ &cmFsmAudio,
      /* ---- eCMEVT_MEDIA_DELAY ---- */ &cmFsmAudioDelayed,
      /* ---- eCMEVT_MEDIA_END ------ */ &cmFsmAudioEnd,
      /* ---- eCMEVT_NO_ACTIVITY ---- */ &cmFsmInvalid,
      /* ---- eCMEVT_HOLD_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_UNHOLD_SVC ----- */ &cmFsmInvalid,
      /* ---- eCMEVT_CONF_SVC ------- */ &cmFsmInvalid,
      /* ---- eCMEVT_BXFER_SVC ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_CXFER_SVC ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_INTCALL_SVC ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmInvalid,
   },

   /* ---- eCMST_RINGING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmOffHook,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmNoOp,
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
      /* ---- eCMEVT_SES_STATUS ----- */ &cmFsmNoOp,
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
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmInvalid,
  },

   /* ---- eCMST_TALK ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmNoOp,
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

   /* ---- eCMST_FLASH ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmInvalid,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmNoOp,
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
      /* ---- eCMEVT_ANSWER --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmNoOp,
   },

   /* ---- eCMST_RELEASING ------------------------------------------------------ */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmOffHook,
      /* ---- eCMEVT_FLASH ---------- */ &cmFsmNoOp,
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
      /* ---- eCMEVT_ENDCALL -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_DTMFPT --------- */ &cmFsmInvalid,
      /* ---- eCMEVT_INGRESS_DTMF --- */ &cmFsmInvalid,
   },

   /* ---- eCMST_ERROR ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_ONHOOK --------- */ &cmFsmOnHook,
      /* ---- eCMEVT_OFFHOOK -------- */ &cmFsmNoOp,
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

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   CMGRLOG_INFORMATION (( "cmFsmOnHook: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.uUsr = pUsr->uHdl;
   
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
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
                           pDig ? (void*)pDig : NULL);
   }

   pUsr->bOffHook = BOS_FALSE;
   pUsr->bPerCallCW = BOS_TRUE;

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
                                    pUsr->uHdl,
                                    CMGR_INVALID_HANDLE,
                                    NULL );
            }

            /* If we are pending on an action, don't release
            ** the resource.
            */
            if ( pUsr->bPendingActPhy == BOS_FALSE )
            {
               cmUsrPhyDisassociate( pUsr );
            }
         }
      }
   }
   /* If there is a focus and established session, ensure we run proper disconnect
   ** procedures on it.
   */
   else if ( ( pSes = cmUsrFocusSes ( pUsr ) ) != NULL )
   {

      /* Terminate any possible signal being played (like off-hook warning).
      */
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = pSes->uNet;
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
   }
   /* If there is a 'on-hold' session that has been waiting, alert the local
   ** user of the session waiting.
   */
   else
   {
      BOS_UINT32 uIx;

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

      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = (pSes) ? pSes->uNet : CMGR_INVALID_HANDLE;
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
                                 pUsr->uHdl,
                                 CMGR_INVALID_HANDLE,
                                 NULL );
         }

         /* If we are pending on an action, don't release
         ** the resource.
         */
         if ( pUsr->bPendingActPhy == BOS_FALSE )
         {
            cmUsrPhyDisassociate( pUsr );
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

   /* Report the hook event to the application in case it needs it.
   */
   if ( gCmCfgCb.cmgrEvtCb )
   {
      gCmCfgCb.cmgrEvtCb ( eCMGREVT_OFFHOOK,
                           pUsr->uPhys,
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
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
   else
   {
      switch ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] )
      {
         case eCMST_IDLE:
         {
            /* If there was a call waiting, this means we try to resume it.
            */
            {
               cmFsmOutgoingCall ( pRes );
            }
         }
         break;

         case eCMST_RINGING:
         {
            if ( (pSes = cmUsrFocusSes ( pUsr )) != NULL )
            {
               tmr.uSes = pSes->uHdl;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_RINGING;

               cmTmrStop ( &tmr );

               sig.eAct = eCMGREPSIGACT_STOP;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = pSes->uNet;
               sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGING */
               pUsr->uCpt = eEPTSIG_NULL;

               cmPhySignal ( pUsr->uPhys, &sig );

               if ( pSes->ses.med.uPhy == CMGR_INVALID_HANDLE )
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

               pSes->bEstab = BOS_TRUE;
               if ( gCmCfgCb.cmgrEvtCb )
               {
                  ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                           pSes->uNet,
                                           pUsr->uHdl,
                                           CMGR_INVALID_HANDLE,
                                           (void *) pUsr->uUid );
               }

               if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
                  cmAppNetCallAnswer( pSes->uNet );
               }

               cmUsrNewState ( pUsr, eCMST_TALK );
            }
         }
         break;

         case eCMST_RELEASING:
         {
            /* If there was a call waiting, this means we try to resume it.
            */
            if ( (pSes = cmUsrFocusSes ( pUsr )) != NULL )
            {
               if ( pSes->bEstab )
               {
                  /* Terminate hold reminder if playing.
                  */
                  sig.eAct = eCMGREPSIGACT_STOP;
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = pSes->uNet;
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
                        /* TODO: unhold call. */
                        
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

         case eCMST_ACCEPTED:
         {
            if ( (pSes = cmUsrFocusSes ( pUsr )) != NULL )
            {
               /* Answer the incoming call right away.
               */
               if( !pSes->bEstab )
               {
                  pSes->bEstab = BOS_TRUE;
               }

               /* Notify the application that the call has connected */
               if ( gCmCfgCb.cmgrEvtCb )
               {
                  ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                           pSes->uNet,
                                           pUsr->uHdl,
                                           CMGR_INVALID_HANDLE,
                                           (void *) pUsr->uUid );
               }

               if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
               {
                  cmAppNetCallAnswer( pSes->uNet );
               }

               cmUsrNewState ( pUsr, eCMST_TALK );
            }
         }
         break;

         default:
         break;
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
         sig.uSig = pUsr->uCpt;   /* eEPTSIG_DIAL or eEPTSIG_SPECIAL_DIAL */
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_TMR_DIALTONE;

         cmTmrStop ( &tmr );
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

   if( pUsr->pDmInUse == NULL )
   {
      pUsr->pDmInUse = pUsr->pDmDigMap;
   }

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
                                       pUsr->uHdl,
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
         sig.uSig = pUsr->uCpt;   /* eEPTSIG_DIAL or eEPTSIG_SPECIAL_DIAL */
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.eType = eCMEVT_TMR_DIALTONE;

         cmTmrStop ( &tmr );
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

               if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmDigMap,
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
                                    pUsr->uHdl,
                                    CMGR_INVALID_HANDLE,
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
      sig.uSig = pUsr->uCpt;   /* eEPTSIG_SPECIAL_DIAL */
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      tmr.uUsr = pUsr->uHdl;
      tmr.uSes = CMGR_INVALID_HANDLE;
      tmr.eType = eCMEVT_TMR_DIALTONE;

      cmTmrStop ( &tmr );

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

      if( pUsr->pDmInUse == NULL )
      {
         pUsr->pDmInUse = pUsr->pDmDigMap;
      }

      dmStatus = cmDmCheckDialString (
                                       pUsr->uHdl,
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

               if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmDigMap,
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
                                    pUsr->uHdl,
                                    CMGR_INVALID_HANDLE,
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
#if !CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
   BOS_BOOL bWarmLine = BOS_FALSE;
#endif
   BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

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
                                 pUsr->uHdl,
                                 CMGR_INVALID_HANDLE,
                                 NULL );
         }

         {
            tmr.uSes = pSes->uHdl;
            tmr.uUsr = pUsr->uHdl;

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
            sig.uSig = eEPTSIG_DIAL;

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
