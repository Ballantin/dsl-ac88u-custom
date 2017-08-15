/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom
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
**      This file contains the Call Manager finite state machine.
**
***********************************************************************************/

#ifndef __CM_FSM__H__INCLUDED__
#define __CM_FSM__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>

/* ---- Constants and Types ----------------------------------------------------- */

#define CMFSM_CURRENT_STATE_IX                                    0
#define CMFSM_CURRENT_EVENT_IX                                    0

typedef void ( *CMFSMVOIDFUNCTIONPTR ) ( void *pRes );

/* Defines the FSM type.
*/
typedef enum
{
   eCMFSM_CCTK,
   eCMFSM_PHY,
   eCMFSM_TMR,

} CMFSMTYPE;

/* Defines the FSM state.
*/
typedef enum
{
   eCMST_IDLE,                /* Default state - must be first. */
   eCMST_ACCEPTED,
   eCMST_DIALING,
   eCMST_RINGBACK,
   eCMST_RINGING,
   eCMST_TALK,
   eCMST_FLASH,
   eCMST_RELEASING,
   eCMST_ERROR,

   eCMST_MAX,                 /* Unused state - must be last. */

} CMFSMSTATE;

/* Defines the FSM event.
*/
typedef enum
{
   eCMEVT_INVALID,

   /* Begins events generated due to CCTK action.
   */
   eCMEVT_CCTK_START,
   eCMEVT_INCOMING_CALL,
   eCMEVT_CLEANED_CALL,
   eCMEVT_ALERT_CALL,
   eCMEVT_RINGING_CALL,
   eCMEVT_CONNECT_CALL,
   eCMEVT_RELEASE_CALL,
   eCMEVT_BUSY_CALL,
   eCMEVT_CCTK_END,
   /* Ends events generated due to CCTK action.
   */

   /* Begins events generated due to endpoint (physical interface) action.
   */
   eCMEVT_PHY_START,
   eCMEVT_ONHOOK,
   eCMEVT_OFFHOOK,
   eCMEVT_FLASH,
   eCMEVT_MODEM_TONE,
   eCMEVT_FAX_TONE,
   eCMEVT_FAX_END,
   eCMEVT_FAX_FAIL,
   eCMEVT_DTMF_0,
   eCMEVT_DTMF_1,
   eCMEVT_DTMF_2,
   eCMEVT_DTMF_3,
   eCMEVT_DTMF_4,
   eCMEVT_DTMF_5,
   eCMEVT_DTMF_6,
   eCMEVT_DTMF_7,
   eCMEVT_DTMF_8,
   eCMEVT_DTMF_9,
   eCMEVT_DTMF_STAR,
   eCMEVT_DTMF_HASH,
   eCMEVT_DTMF_A,
   eCMEVT_DTMF_B,
   eCMEVT_DTMF_C,
   eCMEVT_DTMF_D,
   eCMEVT_SES_STATUS,
   eCMEVT_MEDIA,
   eCMEVT_MEDIA_DELAY,
   eCMEVT_MEDIA_END,
   eCMEVT_NO_ACTIVITY,
   eCMEVT_HOLD_SVC,
   eCMEVT_UNHOLD_SVC,
   eCMEVT_CONF_SVC,
   eCMEVT_BXFER_SVC,
   eCMEVT_CXFER_SVC,
   eCMEVT_INTCALL_SVC,
   eCMEVT_ANSWER,
   eCMEVT_ENDCALL,
   eCMEVT_DTMFPT,
   eCMEVT_INGRESS_DTMF,
   eCMEVT_PHY_END,
   /* Ends events generated due to endpoint (physical interface) action.
   */

   /* Begins events generated due to timer action.
   */
   eCMEVT_TMR_START,
   eCMEVT_TMR_ONHOOK,
   eCMEVT_TMR_DIALTONE,
   eCMEVT_TMR_RINGING,
   eCMEVT_TMR_RINGBACK,
   eCMEVT_TMR_INTERDIGIT_SHORT,
   eCMEVT_TMR_INTERDIGIT_LONG,
   eCMEVT_TMR_RENEW_SVC,
   eCMEVT_TMR_WAITCONF_SVC,
   eCMEVT_TMR_OHWARN,
   eCMEVT_TMR_CALLWAIT,
   eCMEVT_TMR_AUTOCALLBACK,
   eCMEVT_TMR_KEYHELD,
   eCMEVT_TMR_FAX_EVENT,
   eCMEVT_TMR_OSI_DELAY,
   eCMEVT_TMR_AUTORECOVER,
   eCMEVT_TMR_DTMF,
   eCMEVT_TMR_END,
   /* Ends events generated due to timer action.
   */

   eCMEVT_MAX,                 /* Unused event - must be last. */

} CMFSMEVT;

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmFsmRun
**
** PURPOSE:    Main entry for running the FSM.
**
** PARAMETERS: eType - the FSM type to run.
**             pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRun ( CMFSMTYPE eType, void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCctkRun
**
** PURPOSE:    Main entry for running the CCTK FSM.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCctkRun ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmPhyRun
**
** PURPOSE:    Main entry for running the PHY FSM.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmPhyRun ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmTmrRun
**
** PURPOSE:    Main entry for running the TMR FSM.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmTmrRun ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmInvalid
**
** PURPOSE:    FSM invalid state/event function prototype.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmInvalid ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmNoOp
**
** PURPOSE:    FSM no-operation state/event function prototype.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmNoOp ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmDlg
**
** PURPOSE:    FSM received DLG (dialog) information.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDlg ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmForward
**
** PURPOSE:    FSM received forward related information.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmForward ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmUsrUpdate
**
** PURPOSE:    FSM user information update handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmUsrUpdate ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmFsmPauUpdate
**
** PURPOSE:    FSM P-Associated-URI information update handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmPauUpdate ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmFsmInSrv
**
** PURPOSE:    FSM in-service handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmInSrv ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmOutOfSrv
**
** PURPOSE:    FSM out-of-service handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOutOfSrv ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmSrvConf
**
** PURPOSE:    FSM service confirmation handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmSrvConf ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmSrvEnd
**
** PURPOSE:    FSM service terminated handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmSrvEnd ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallEnd
**
** PURPOSE:    FSM call terminated handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallEnd ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallNew
**
** PURPOSE:    FSM new incoming call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallNew ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallReject
**
** PURPOSE:    FSM reject incoming call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallReject ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallRejectAnonymous
**
** PURPOSE:    FSM reject incoming anonymous call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRejectAnonymous ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallBusy
**
** PURPOSE:    FSM reject busy incoming call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRejectBusy ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallEnded
**
** PURPOSE:    FSM remote terminated call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallEnded ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallConnect
**
** PURPOSE:    FSM remote connected call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallConnect ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallRingback
**
** PURPOSE:    FSM remote ringing call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRingback ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallBusy
**
** PURPOSE:    FSM received busy treatment.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallBusy ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmOnHook
**
** PURPOSE:    FSM on hook handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOnHook ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmOffHook
**
** PURPOSE:    FSM off hook handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOffHook ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmDigitDetect
**
** PURPOSE:    FSM digit detection (during dialing phase) handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDigitDetect ( void *pRes );


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
void cmFsmFeature ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmOnHookGuard
**
** PURPOSE:    FSM on-hook guard timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOnHookGuard ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmRenewSvc
**
** PURPOSE:    FSM renewal of service timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRenewSvc ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmTimeoutSvc
**
** PURPOSE:    FSM waiting for confirmation of a service timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmTimeoutSvc ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmDialFailure
**
** PURPOSE:    FSM waiting for initial digit dialing timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDialFailure ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmConnectFailure
**
** PURPOSE:    FSM waiting for remote connection answer timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmConnectFailure ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmNoAnswer
**
** PURPOSE:    FSM waiting for local user call answer timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmNoAnswer ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmDigitTimer
**
** PURPOSE:    FSM waiting for digit information timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDigitTimer ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmFlashTimer
**
** PURPOSE:    FSM waiting for information during flash timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFlashTimer ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmAbandonFlash
**
** PURPOSE:    FSM digit collection timer expiry during flash processing.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmAbandonFlash ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmSesStatus
**
** PURPOSE:    FSM status on a session physical action.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmSesStatus ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmMediaXfer
**
** PURPOSE:    FSM media information due to transfer service.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmMediaXfer ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmAudio
**
** PURPOSE:    FSM audio data received for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmAudio ( void *pRes );


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
void cmFsmAudioDelayed ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmAudioEnd
**
** PURPOSE:    FSM audio no longer received for this resource (superceeds a
**             cmFsmAudio action).
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmAudioEnd ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmRemHold
**
** PURPOSE:    FSM remote hold of local user.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRemHold ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmRemUnHold
**
** PURPOSE:    FSM remote un-hold of local user.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRemUnHold ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmHoldFail
**
** PURPOSE:    FSM local user hold failure.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmHoldFail ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmHoldOkay
**
** PURPOSE:    FSM local user hold successful completion.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmHoldOkay ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmUnHoldFail
**
** PURPOSE:    FSM local user unhold failure.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmUnHoldFail ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmOutgoingCall
**
** PURPOSE:    FSM generic handle to create a new outgoing call.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOutgoingCall ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmDigColAction
**
** PURPOSE:    FSM generic handle to process an action as the result of digit
**             collection.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDigColAction ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmFeatAction
**
** PURPOSE:    FSM specific handle to process an action as the result of digit
**             collection while waiting for a feature execution.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFeatAction ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallEnding
**
** PURPOSE:    FSM call ending while ringing handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallEnding ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallRejected
**
** PURPOSE:    FSM call ended in rejection while attempting handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRejected ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmOffHookWarn
**
** PURPOSE:    FSM off-hook warning timer expiry processing.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOffHookWarn ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmRing
**
** PURPOSE:    FSM ring this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRing ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallWait
**
** PURPOSE:    FSM call waiting for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallWait ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallWaitEnd
**
** PURPOSE:    FSM terminate call waiting for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallWaitEnd ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmKeyTimer
**
** PURPOSE:    FSM key hold monitoring timer expired for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmKeyTimer ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmNetHoldTimer
**
** PURPOSE:    FSM network hold timer expired for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmNetHoldTimer ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallXfered
**
** PURPOSE:    FSM call transfered handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallXfered ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallXfer
**
** PURPOSE:    FSM call transfer request handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallXfer ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCallAlert
**
** PURPOSE:    FSM incoming call alert handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallAlert ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmCancelKeyTmr
**
** PURPOSE:    FSM cancel the key held timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCancelKeyTmr ( void *pRes );


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
void cmFsmXferAccept ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmWarmLine
**
** PURPOSE:    FSM received warm line timeout.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmWarmLine ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmOsi
**
** PURPOSE:    FSM received OSI indication timeout.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmOsi ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmDtmfTimer
**
** PURPOSE:    FSM time out when playing out DTMF.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDtmfTimer ( void *pRes );


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
void cmFsmMessage ( void *pRes );


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
void cmFsmMessageRsp ( void *pRes );
#endif /* CMGRCFG_PASS_THRU_MSG */


/***********************************************************************************
** FUNCTION:   cmFsmConfSvc
**
** PURPOSE:    FSM received an CONF service request from controlling application.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This service is for local conference only, local conference is
**             only applicable to two sessions that are controlled by the same
**             user.
***********************************************************************************/
void cmFsmConfSvc ( void *pRes );


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
void cmFsmBxferSvc ( void *pRes );


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
void cmFsmCxferSvc ( void *pRes );


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
void cmFsmInternalCallSvc ( void *pRes );


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
void cmFsmAnsSvc ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmOobDtmf
**
** PURPOSE:    FSM out-of-band DTMF (via signaling) received.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOobDtmf ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmFsmCwtInfo
**
** PURPOSE:    FSM Call Waiting Tone info (via signaling) received.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCwtInfo ( void *pRes );

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
void cmFsmIngressDtmfSvc( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFsmXferInt
**
** PURPOSE:    FSM internal transfer handler.
**
** PARAMETERS: pRes       - the pointer to the user on which to run the FSM.
**             ppSesFrom  - pointer to pointer to session to be transferred
**             ppSesTo    - pointer to pointer to internal session to transfer to
**
** RETURNS:    None.
**
** NOTES:      This function assumes two things:
**                1) pSesTo points to an INTERNAL session. pSesFrom can be either internal
**                      or external.
**                2) pSesFrom is already put on hold.
***********************************************************************************/
void cmFsmXferInt ( void* pRes, void* ppSesFrom, void* ppSesTo );


/***********************************************************************************
** FUNCTION:   cmFsmCdrInfo
**
** PURPOSE:    FSM call data record published from CCTK layer.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCdrInfo ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmFsmCallForwardViaDialString
**
** PURPOSE:    Call forwarding via dialstring match.
**
** PARAMETERS: pRes - pointer to forwarding cmUsr
**             cid  - cid of the call to be forwarded
**             dialString - the fwd to number.
** RETURNS:    None.
**
** NOTES:      
***********************************************************************************/
void cmFsmCallForwardViaDialString( void * pRes, BOS_UINT32 cid, char * dialString );

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
void cmFsmPublishTarget ( void *pRes );

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
void cmFsmSubStatus ( void *pRes );


#if defined(__cplusplus)
}
#endif

#endif /* __CM_FSM__H__INCLUDED__ */

