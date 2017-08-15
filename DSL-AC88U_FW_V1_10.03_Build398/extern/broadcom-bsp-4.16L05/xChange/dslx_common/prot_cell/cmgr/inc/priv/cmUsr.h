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
**      This file contains the Call Manager user resource.
**
***********************************************************************************/

#ifndef __CM_USR__H__INCLUDED__
#define __CM_USR__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>
#include <cmFsm.h>
#include <cmSes.h>
#include <cmMain.h>
#include <cmDm.h>

/* ---- Constants and Types ----------------------------------------------------- */
/* Maximum number of sessions that can be transferred to conference bridge */
#define CMGR_MAX_NETCONF_SESSIONS        2

/* Defines the user resource type.
*/
typedef enum
{
   eCMUSRTYPE_NONE,
   eCMUSRTYPE_LINE,
   eCMUSRTYPE_SHAREDLINE,
   eCMUSRTYPE_PSTN,
   eCMUSRTYPE_REMOTE,

} CMUSRTYPE;

/* Defines the advancement in the error status for this resource.  This information
** is only used when in the error state.
*/
typedef enum
{
   eCMRESERROR_ENTRY,
   eCMRESERROR_LEVEL1,
   eCMRESERROR_LEVEL2,
   eCMRESERROR_LEVEL3,
   eCMRESERROR_LEVEL4,
   eCMRESERROR_LOCKOUT,

} CMRESERROR;

/* Define some information relative to auto-call feature.
*/
typedef struct
{
   BOS_BOOL bUsed;
   CMDMACTIONTYPE eType;
   BOS_BOOL bAnonymous;
   BOS_BOOL bPending;
   BOS_UINT8 uPeer[ CMGR_URI_STRING_LEN + 1 ];
   BOS_UINT32 uSes;
   BOS_TIME_MS time;

} CMAUTOCALL;

typedef enum
{
   CMCONF_STATUS_IDLE,
   CMCONF_STATUS_INIT,         /* Initiating a hold request with the current caller */
   CMCONF_STATUS_INPROGRESS,   /* Initiating a request for a conference call to the conference bridge */
   CMCONF_STATUS_COMPLETING    /* Sending conference request to the held parties */

} CMCONFSTATUS;

typedef struct
{
   CMCONFSTATUS       eRequestStatus;   /* Indicates the progress of conference call request */
   CFGSIPCONFOPTION   eConfOption;
   BOS_UINT8          cConfUri[CMGR_URI_STRING_LEN+1];
   CMSES              *pServerSes;
   CMSES              *pHeldSes[CMGR_MAX_NETCONF_SESSIONS];
} CMCONFBRIDGE;

/* Defines the digit collection state.
*/
typedef enum
{
   CMDIGITDETSTA_NONE,           /* No digits have been detected */
   CMDIGITDETSTA_INPROGRESS,     /* Digit collection has been started */
   CMDIGITDETSTA_MATCHED,        /* The collected dialstring has found a digitmap match */

} CMDIGITDETSTA;

/* Defines Analog Alarm Cfg State.
*/
typedef enum
{
   CMALGALRMCFGSTATE_NONE = 1,           /* no analog alarm configuration */
   CMALGALRMCFGSTATE_PREEMPTIVE,     /* analog alarm configured in preemptive mode */
   CMALGALRMCFGSTATE_SIMULTANEOUS,   /* simultaneous regular phone call and alarm call allowed */

} CMALGALRMCFGSTATE;


/* Define some information relative to register-recall style flash feature.
*/
typedef struct
{
   BOS_BOOL       bRRWaitKey;
   CMFSMSTATE     ePrevSta;

} CMRRFLASH;


/* Defines the user resource control block.
*/
typedef struct
{
   BOS_BOOL           bInUse;    /* marks the user block as allocated */
   BOS_UINT32         uHdl;      /* The local handle for this resource. */

   BOS_UINT32         uNet;      /* The network handle for this resource. */
   CMCFGACT           eCfgAction;

    /* CCTK account identifier */
   BOS_UINT8          uUid[ CMGR_USER_IDENTIFIER_LEN + 1 ];

   /* CCTK account index */
   BOS_UINT32         uUindex;

   BOS_BOOL           bInSrv;         /* 'in service' flag. */
   BOS_BOOL           bBeRem;         /* 'being removed' flag. */
   BOS_BOOL           bOffHook;       /* 'off-hook' flag. */
   BOS_BOOL           bShortDT;       /* short dial tone flag. */
   CMDIGITDETSTA      eDigDetState;   /* digit detection state */
   BOS_BOOL           bPerCallCW;     /* per-call call waiting enabled flag. */
   BOS_BOOL           bPendingActPhy; /* pending action at physical int. point */
   CMSES              *pSesWait;      /* call waiting reference. */
   CMSES              *pSesHeld;      /* call held reference. */
   BOS_BOOL           bPresrvHeld;    /* preserve held call flag. */
   CMAUTOCALL         *pAutoCall;     /* auto-call pending reference. */
   BOS_BOOL           bMwi;           /* message waiting indication flag */
   BOS_BOOL           bPendingMwi;    /* pending message waiting indication */
   BOS_BOOL           bDnd;           /* local do-not-disturb setting */
   BOS_BOOL           bRejectAnonymous; /* local Anonymous call rejection setting */
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
   BOS_BOOL           bWarmLineActive; /* local warmline activation setting */
#endif
   BOS_BOOL           bDtLnk;         /* 'data link' flag. */
   BOS_BOOL           bRegNoDelay;    /* 'register with no delay' flag. */
   BOS_BOOL           bIntCallDet;    /* 'internal call' in process flag. */

   CMRRFLASH          rrFlash;      /* Register Recall style Flash reference */

   CMUSRTYPE          eType;
   BOS_UINT32         uPhys;     /* The physical handle for this resource. */
   BOS_UINT32         uCpt;
   BOS_UINT32         uCfgTone;

   CMFSMSTATE         eSta[ CMGRCFG_STATE_MAX ];
   CMFSMEVT           eEvt[ CMGRCFG_STATE_MAX ][ CMGRCFG_EVENT_MAX ];

   CMEVT              evt;       /* The currently processed event information. */

   CMSES             *pMedSes[ CMGRCFG_MEDSES_MAX + 1 ];
   CMSES             *pSrvSes[ CMGRCFG_SRVSES_MAX + 1 ];

   BOS_UINT8         uDigCol[CMGR_COLLECTED_DIGIT_LEN + 1];
   BOS_UINT32        uDigCnt;

   BOS_UINT8         *pDmState;
   BOS_UINT8         *pDmDigMap;

   BOS_UINT8         *pDmInUse;

   CMAUTOCALL        *pAc[ CMGRCFG_AUTOCALL_MAX / 2 ];
   CMAUTOCALL        *pAr[ CMGRCFG_AUTOCALL_MAX / 2 ];

   CMRESERROR        eError;
   BOS_UINT32        uLinkedLocalUserHdl;  // association between local user and remote users
   BOS_UINT32        uLinkedRemoteUserHdl[CMGRCFG_USER_MAX];
   BOS_UINT32        uLinkedRemoteUserCnt;
   CMCONFBRIDGE      *pConfBridgeStatus;
} CMUSR;

typedef struct
{
   BOS_UINT32 uLength;                      /* The length of the returned array */
   BOS_UINT32 uUsr[ CMGRCFG_USER_MAX ];     /* Array of call manager user handles */
} CMUSRLIST;

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmUsr
**
** PURPOSE:    Manages the user resources.
**
** PARAMETERS: bCreate - Whether resources should be created (BOS_TRUE) or ended
**                       (BOS_FALSE) when this call is invoked.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsr ( BOS_BOOL bCreate );


/***********************************************************************************
** FUNCTION:   cmUsrAlloc
**
** PURPOSE:    Allocates a free user resources.
**
** PARAMETERS: None.
**
** RETURNS:    Pointer to the allocated user on success, NULL otherwise.
***********************************************************************************/
CMUSR * cmUsrAlloc ( void );


/***********************************************************************************
** FUNCTION:   cmUsrNewState
**
** PURPOSE:    Update the state of the user resource.
**
** PARAMETERS: pUsr - the user on which to apply this new state.
**             eState - the new state for this user.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrNewState ( CMUSR *pUsr, CMFSMSTATE eState );


/***********************************************************************************
** FUNCTION:   cmUsrNewEvent
**
** PURPOSE:    Update the event of the user resource.
**
** PARAMETERS: pUsr - the user on which to apply this new event.
**             eEvent - the new event for this user.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrNewEvent ( CMUSR *pUsr, CMFSMEVT eEvent );


/***********************************************************************************
** FUNCTION:   cmUsrValid
**
** PURPOSE:    Checks whether a user handle is for a valid user.
**
** PARAMETERS: uHdl - the user handle to check for.
**
** RETURNS:    Pointer to valid user or NULL if not found.
***********************************************************************************/
CMUSR * cmUsrValid ( BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmUsrFindNetHdl
**
** PURPOSE:    Finds a user based on its network handle.
**
** PARAMETERS: uNet - the network handle to check for.
**
** RETURNS:    Pointer to the user block if found, NULL otherwise.
**
** NOTE:       The network handle is the only way we can identify events for a
**             given user when reported by the CCTK layer.
***********************************************************************************/
CMUSR * cmUsrFindNetHdl ( BOS_UINT32 uNet );


/***********************************************************************************
** FUNCTION:   cmUsrFindPhyHdl
**
** PURPOSE:    Finds a user based on its physical (endpoint) handle.
**
** PARAMETERS: uPhy - the physical handle to check for.
**
** RETURNS:    Pointer to the user block if found, NULL otherwise.
**
** NOTE:       The physical handle is the only way we can identify events for a
**             given user when reported by the endpoint (physical) layer.
***********************************************************************************/
CMUSR * cmUsrFindPhyHdl ( BOS_UINT32 uPhy );


/***********************************************************************************
** FUNCTION:   cmUsrFindUidHdl
**
** PURPOSE:    Finds a user based on its user-identification handle.
**
** PARAMETERS: pUid - the user identification to check for.
**
** RETURNS:    Pointer to the user block if found, NULL otherwise.
**
** NOTE:       .
***********************************************************************************/
CMUSR * cmUsrFindUidHdl ( BOS_UINT8 *pUid );


/***********************************************************************************
** FUNCTION:   cmUsrLinkSes
**
** PURPOSE:    Links a session to a user.
**
** PARAMETERS: pUsr - the user to link to.
**             pSes - the session to link up.
**
** RETURNS:    BOS_TRUE is a linkage was possible, which means the resource was
**             correctly associated with the user.
**
**             BOS_FALSE otherwise.  The resource cannot be associated with the
**             user, it generally is because the user is already using too many
**             resources, therefore it cannot afford to manage more.
***********************************************************************************/
BOS_BOOL cmUsrLinkSes ( CMUSR *pUsr, CMSES *pSes );


/***********************************************************************************
** FUNCTION:   cmUsrUnLinkSes
**
** PURPOSE:    Un-links a session from a user.
**
** PARAMETERS: pUsr - the user to un-link from.
**             pSes - the session to un-link.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrUnLinkSes ( CMUSR *pUsr, CMSES *pSes );


/***********************************************************************************
** FUNCTION:   cmUsrFocusSes
**
** PURPOSE:    Determines the focus session associated with a user.
**
** PARAMETERS: pUsr - the user of interest.
**
** RETURNS:    The focus session for this user, or NULL if not applicable.
***********************************************************************************/
CMSES * cmUsrFocusSes ( CMUSR *pUsr );


/***********************************************************************************
** FUNCTION:   cmUsrActiveSes
**
** PURPOSE:    Determines the active (but not focus) session associated
**             with a user.
**
** PARAMETERS: pUsr - the user of interest.
**
** RETURNS:    The active session for this user, or NULL if not applicable.  This
**             function will NOT return the focus session even if this latter is
**             valid.  To retrieve the focus session, one MUST invoke the
**             cmUsrFocusSes API.
***********************************************************************************/
CMSES * cmUsrActiveSes ( CMUSR *pUsr );

/***********************************************************************************
** FUNCTION:   cmUsrLinkLocalUser
**
** PURPOSE:    Set the local user to this user.
**
** PARAMETERS: pCurUsr - the user of interest.
**             uLocalUsrHdl - the handle of the local user to be linked with
**             the current user.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrLinkLocalUser ( CMUSR *pCurUsr, BOS_UINT32 uLocalUsrHdl );

/***********************************************************************************
** FUNCTION:   cmUsrFindCid
**
** PURPOSE:    Finds the user with the assigned callId.
**
** PARAMETERS: uNetCid  - the call id to use
**             bEstabSesOnly - whether to return the user only if the session was
**                             established.
**
** RETURNS:     .
***********************************************************************************/
CMUSR* cmUsrFindCid ( BOS_UINT32 uNetCid, BOS_BOOL bEstabSesOnly );


/***********************************************************************************
** FUNCTION:   cmUsrPhyCreate
**
** PURPOSE:    Creates the physical interface support for the user(s).
**
** PARAMETERS: uHdl - the handle for a specific user or CMGR_INVALID_HANDLE
**                    to apply this action for all known users.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrPhyCreate ( BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmUsrPhyRelease
**
** PURPOSE:    Mark a physical interface has released by the framework, the user
**             can no longer make use of it.
**
** PARAMETERS: uHdl - the physical interface handle.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrPhyRelease ( BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmUsrPhyAssociate
**
** PURPOSE:    Associates a user to a physical interface.
**
** PARAMETERS: uPhy - the physical interface to be associated with.
**
** RETURNS:    The associated user, or NULL if none can be associated.
***********************************************************************************/
CMUSR * cmUsrPhyAssociate ( BOS_UINT32 uPhy );

/***********************************************************************************
** FUNCTION:   cmUsrPhyDisassociate
**
** PURPOSE:    Disassociates a physical interface from a user
**
** PARAMETERS: pUsr - the user of interest for the disassociation
**
** RETURNS:    none
***********************************************************************************/
void cmUsrPhyDisassociate ( CMUSR *pUsr );


/***********************************************************************************
** FUNCTION:   cmUsrIdle
**
** PURPOSE:    Check for all users that should be idled.
**
** PARAMETERS: .
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrIdle ( void );

/***********************************************************************************
** FUNCTION:   cmUsrGetAll
**
** PURPOSE:    Returns a list of all CM users in the system.
**
** PARAMETERS: pUsrList - the users of interest.
**             pExcludeUsr - If not NULL exclude this user from the list
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmUsrGetAll ( CMUSRLIST* pUsrList, CMUSR *pExcludeUsr );

/***********************************************************************************
 * ** FUNCTION:   cmUsrGetNtwkHdl
 * **
 * ** PURPOSE:    Returns the CCTK network handle for this user.
 * **
 * ** PARAMETERS: pUsr - the user of interest.
 * **
 * ** RETURNS:    Returns the CCTK network handle for this user..
 * ***********************************************************************************/
BOS_UINT32 cmUsrGetNtwkHdl ( CMUSR *pUsr );

/***********************************************************************************
 * ** FUNCTION:   cmUsrGetCurrNtwkHdl
 * **
 * ** PURPOSE:    Returns the current CCTK network handle for this user (used in logging usually).
 * **
 * ** PARAMETERS: pUsr - the user of interest.
 * **
 * ** RETURNS:    Returns the CCTK network handle for this user..
 * ***********************************************************************************/
BOS_UINT32 cmUsrGetCurrNtwkHdl ( CMUSR *pUsr );


/***********************************************************************************
 * ** FUNCTION:   cmUsrSetNtwkHdl
 * **
 * ** PURPOSE:    Sets the CCTK network handle for this user.
 * **
 * ** PARAMETERS: pUsr - Pointer to user
 * **             handle - CCTK network handle for this user.
 * **
 * ** RETURNS:    Nothing
 * ***********************************************************************************/
void  cmUsrSetNtwkHdl ( CMUSR *pUsr, BOS_UINT32 handle );

#if defined(__cplusplus)
}
#endif

#endif /* __CM_USR__H__INCLUDED__ */

