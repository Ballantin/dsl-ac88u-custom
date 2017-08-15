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
**      This file contains the Call Manager feature processing functionality.
**
***********************************************************************************/

#ifndef __CM_FEAT__H__INCLUDED__
#define __CM_FEAT__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cmDm.h>

/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmFeatEnabled
**
** PURPOSE:    Utility function to check whether a given feature is enabled
**             or not.
**
** PARAMETERS: pRes - the resource to apply this check on.
**             eAction - the action for which we need to check the enable status.
**
** RETURNS:    BOS_TRUE if the feature is enabled and therefore should be
**             processed by the FSM.
**             BOS_FALSE otherwise.  In such case, some error handling is signaled
**             to the user.
***********************************************************************************/
BOS_BOOL cmFeatEnabled ( void *pRes, CMDMACTIONTYPE eAction );


/***********************************************************************************
** FUNCTION:   cmFeatAccept
**
** PURPOSE:    Feature accepted function handler.
**
** PARAMETERS: pRes - the resource to apply this feature on.
**             eAction - the action to be taken.
**
** RETURNS:    BOS_TRUE if the action applied due to the feature allows for further
**             pending action(s) to be processed as follow up to this action.
**             BOS_FALSE otherwise.  This means that following the end of this
**             action processing there is no need to process the rest of the
**             pending action(s).
***********************************************************************************/
BOS_BOOL cmFeatAccept ( void *pRes, CMDMACTIONTYPE eAction );


/***********************************************************************************
** FUNCTION:   cmFeatReject
**
** PURPOSE:    Feature rejected function handler.
**
** PARAMETERS: pRes - the resource to apply this feature on.
**             eAction - the action being rejected.
**
** RETURNS:    Nothing.  Depending on the actual action taken, the function
**             exercizes the correct processing.
***********************************************************************************/
void cmFeatReject ( void *pRes, CMDMACTIONTYPE eAction );


/***********************************************************************************
** FUNCTION:   cmFeatRecall
**
** PURPOSE:    Generic handler to play 'recall' dial tone when a feature is
**             activated.  May be used by any features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatRecall ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatConf
**
** PURPOSE:    Generic handler to play 'confirmation' tone when a feature is
**             activated.  May be used by any features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatConf ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatDeny
**
** PURPOSE:    Generic handler to play 'deny' tone when a feature is
**             not activated or cannot be applied but the processing is not
**             dependent on a specific user action to continue successfully.
**             May be used by any features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatDeny ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatSend
**
** PURPOSE:    Generic handler to send a feature request to the network when the
**             corresponding feature is activated.  May be used by any features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**             eAction - the action from the digit map that is currently processed.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatSend ( void *pRes, CMDMACTIONTYPE eAction );


/***********************************************************************************
** FUNCTION:   cmFeatError
**
** PURPOSE:    Generic handler to manage an error reported against a feature when
**             this latter is not activated.  This is a specific handler which
**             assumes there is way to handle such error as configured by the digit
**             map.  If no such handling is possible, the error handling is left up
**             to the application.
**
** PARAMETERS: pRes - the resource to apply this action on.
**             eAction - the action from the digit map that is currently processed.
**
** RETURNS:    BOS_TRUE if the error on the action was handled according to the
**             digit map rules.
**             BOS_FALSE otherwise, the error on the action is to be handled
**             locally.
***********************************************************************************/
BOS_BOOL cmFeatError ( void *pRes, CMDMACTIONTYPE eAction );


/***********************************************************************************
** FUNCTION:   cmFeatApplyAction
**
** PURPOSE:    Feature handler to apply an action from the digit manipulation
**             output.
**
** PARAMETERS: pRes - the resource to apply this action on.
**             eAction - the action to execute.
**
** RETURNS:    BOS_TRUE if the action is successfully processed and one can
**             continue with other successive actions following.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatApplyAction ( void *pRes, CMDMACTIONTYPE eAction );


/***********************************************************************************
** FUNCTION:   cmFeatConfRecall
**
** PURPOSE:    Special handler to play 'confirmation' tone followed by a 'recall'
**             dial tone when a is activated.  May be used by any features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatConfRecall ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatIgnoreFlash
**
** PURPOSE:    Check whether a recorded flash event can be applied to any of the
**             configured features for this user.
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if the flash event can be ignored, which means there is
**             no valid feature at this time for the user that would require the
**             use of flash, hence it should be ignored.
**             BOS_FALSE otherwise.
**
** NOTE:       This function does not make assumption on the current status of
**             the user, i.e. if the flash occurs for the first time on the first
**             active call or if it happens for a subsequent call with the initial
**             call already on hold.  It does not seem like we need to make such
**             fine decision right now, but perhaps in the future we will if the
**             user application requires this kind of smarts.
***********************************************************************************/
BOS_BOOL cmFeatIgnoreFlash ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatReportFlash
**
** PURPOSE:    Check whether a recorded flash event should be reported via SIP INFO
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if the flash event is notified via SIP INFO instead of being
**             processed
**             BOS_FALSE otherwise.
**
** NOTE:       
***********************************************************************************/
BOS_BOOL cmFeatReportFlash ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatCcw
**
** PURPOSE:    Check whether we are in a special situation for applying the
**             'cancel call waiting' feature.  The 'cancel call waiting'
**             feature is applicable when the user is already engaged in an
**             existing stable two-ways call and it attempts to toggle the
**             call waiting indication.
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if we are in the case where 'cancel call waiting' should
**             apply.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatCcw ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatFaxOnly
**
** PURPOSE:    Check whether we are dealing with a physical resource that is setup
**             for fax only purpose.  In such case, when a call is either placed
**             or received using this resource, we only expect to handle fax call
**             as such, we would start a timer waiting for valid fax event on the
**             resource.  If none is detected following expiration of the timer, we
**             would terminate the call to prevent fraudulent audio call.
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatFaxOnly ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatDialStrManip
**
** PURPOSE:    Apply feature specific digit string manipulation.  This function
**             is meant to be used to see whether a given dial string needs to
**             be morphed to a different one prior to digit map processing.
**
** PARAMETERS: pUsrRes - the user resource to apply this on.
**             pSesRes - the session resource associated to the user (can be
**                       NULL) to apply this on.  Only relevant when the dialing
**                       is associated with a session rather than with the user.
**
** RETURNS:    BOS_TRUE if the dial string was manipulated (i.e. changed in any
**             ways within this function call), BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatDialStrManip ( void *pUsrRes, void *pSesRes );


/***********************************************************************************
** FUNCTION:   cmFeatCtrl
**
** PURPOSE:    Utility function to change the state of a specific feature and
**             apply corresponding service change.
**
** PARAMETERS: pRes - the resource to apply this on.
**             uFeatIx - the feature reference to apply this on.
**             bEnable - whether to enable or disable the feature
**
** RETURNS:    Nothing, however the status of the identified feature may be
**             changed following this action.
***********************************************************************************/
void cmFeatCtrl ( void *pRes, BOS_UINT32 uFeatIx, BOS_BOOL bEnable );

/***********************************************************************************
** FUNCTION:   cmFeatFlashCwOnly
**
** PURPOSE:    Check whether 'call waiting' (and its associated features such as
**             'cancel call waiting') is the only feature available following a
**             hook flash or not.
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if call waiting is the only feature associated with the
**             hook flash, BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatFlashCwOnly ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatFlashNonCw
**
** PURPOSE:    Check for enabled status of any 'hook flash' feature that is not
**             call waiting (or its associated feature such as 'cancel call
**             waiting').
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if some feature other than call waiting can be associated
**             with the hook flash.  BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatFlashNonCw ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatFlashEscAct
**
** PURPOSE:    Escape any action processing while in 'hook flash' mode.  Escaping
**             the process will trigger a feature deny tone to be applied to the
**             local user as well as will resume the current active call.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatFlashEscAct ( void *pRes );



/***********************************************************************************
** FUNCTION:   cmFeatAssignPrivacy
**
** PURPOSE:    Gets the privacy information associated with a user.
**
** PARAMETERS: pRes - the resource to apply this check on.
**             pPrivCfg - the privacy configuration updated to the latest
**                        user specific information.
**
** RETURNS:    BOS_TRUE on success, BOS_FALSE otherwise - if so pPrivCfg must
**             be ignored.
***********************************************************************************/
BOS_BOOL cmFeatAssignPrivacy ( void *pRes, CCTKPRIVCFG *pPrivCfg );


/***********************************************************************************
** FUNCTION:   cmFeatFormatCallerID
**
** PURPOSE:    Format the caller-id date and number according to configuration
**
** PARAMETERS: pRes - the user id to apply this check on.
**             pClidInfo - the caller-id info to be reformated. 
**             pTime - the time to be formated into a string.
**             puStr - the string to hold the formatted time info.
**
** RETURNS:    Nothing,
***********************************************************************************/
void cmFeatFormatCallerID ( BOS_UINT8 *puUid, CCTKCLIDINFO *pClidInfoSrc, time_t *pTime, BOS_UINT8 *puStr );


/***********************************************************************************
** FUNCTION:   cmFeatCallInt
**
** PURPOSE:    Initiates an internal call to the specified users
**
** PARAMETERS: pRes - the resource to apply this check on.
**             pUsrList - list of users to call 
**             bGeneralCall - indicates whether the call is for all users on the
**                            system.
**
** RETURNS:    Nothing,
***********************************************************************************/
void cmFeatCallInt ( void* pRes, CMUSRLIST* pUsrList, BOS_BOOL bGeneralCall );


#if CMGRCFG_REMOTE_USER_CONTROL
/***********************************************************************************
** FUNCTION:   cmFeatProxyCaller
**
** PURPOSE:    Proxy an incoming call on the remote user as a new outgoing
**             call to the first associate local user.
**
** PARAMETERS: pRes - the resource user to apply this feature on.
**             pResSes - the resource session associated to apply this feature on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatProxyCaller ( void *pResUsr, void *pResSes );


/***********************************************************************************
** FUNCTION:   cmFeatProxyCallee
**
** PURPOSE:    Proxy an incoming call on the local user as a new outgoing
**             call to any of the associated remote user(s).
**
** PARAMETERS: pRes - the resource session to apply this feature on.
**
** RETURNS:    BOS_TRUE if we successfully proxy this call to at least one remote
**             user, BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatProxyCallee ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatProxyCalleeEnd
**
** PURPOSE:    End a call that was being proxy'ed on remote users.
**
** PARAMETERS: pRes - the resource user associated with this call.
**
** RETURNS:    If pRes is NULL or is the 'first-user', cancel the associated call
**             on all remote user(s).  If pRes is a remote user identification,
**             cancel the call on all but the identified remote user(s).
***********************************************************************************/
void cmFeatProxyCalleeEnd ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmFeatProxyMove2Bg
**
** PURPOSE:    Move the current session in progress to be a background one on
**             the identified user resource.
**
** PARAMETERS: pRes - the resource user associated with this call.
**             pResSes - the resource session associated with this user.
**
** RETURNS:    .
***********************************************************************************/
void cmFeatProxyMove2Bg ( void *pResUsr, void *pResSes );
#endif /* CMGRCFG_REMOTE_USER_CONTROL */


#if defined(__cplusplus)
}
#endif

#endif /* __CM_FEAT__H__INCLUDED__ */

