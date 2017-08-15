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
**      This file contains the Call Manager CCTK Account mapping resource.
**
***********************************************************************************/

#ifndef __CM_CCTK_ACCNT__H__INCLUDED__
#define __CM_CCTK_ACCNT__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>
#include <cmUsr.h>

/* ---- Constants and Types ----------------------------------------------------- */
#ifndef CMGRCFG_MAX_CALLS_PER_USER
#define CMGRCFG_MAX_CALLS_PER_USER               2
#endif

/* Defines the CMUSR to CCTKUSR handle association.
*/
typedef struct
{
   BOS_UINT32 uUsr;
   BOS_UINT32 uNet;

} CMACCNTASSOCIATION;

typedef struct
{
   BOS_UINT32 uUsr;    /* The CMUSR handle */
   BOS_UINT32 uNet;    /* The CCTK user network handle */
} CMACCNTFORCED;

typedef struct
{
   BOS_UINT32 uHdl;                                        /* CM's unique handle */
   BOS_UINT32 uNet;                                        /* Call Control's unique user handle */
   BOS_UINT8  uUid[ CMGR_USER_IDENTIFIER_LEN + 1 ];        /* CM's user identifier string */
   BOS_UINT32 uNumActiveCalls;                             /* Number of active calls on this account */
   BOS_UINT8  pcDefDialNum[ CMGR_USER_IDENTIFIER_LEN + 1]; /* Default src dialed number to use if none specified */
   BOS_BOOL   bInSrv;                                      /* Whether or not the account is in service */
   CMEVT      evt;                                         /* The currently processed event information. */

   BOS_UINT8  *pDmDigMap;

   CMCFGACT   eCfgAction;                                  /* Cfg action to apply - reconfig, remove..etc */
   BOS_BOOL   bBeRem;                                      /* Account marked as being removed */

} CMCCTKACCNT;

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif


/***********************************************************************************
** FUNCTION:   cmCctkAccnt
**
** PURPOSE:    Manages the CCTK Account resources.
**
** PARAMETERS: bCreate - Whether resources should be initialized (BOS_TRUE) or ended
**                       (BOS_FALSE) when this call is invoked.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccnt ( BOS_BOOL bCreate );

/***********************************************************************************
** FUNCTION:   cmCctkAccntRegister
**
** PURPOSE:    Registers the CCTK user associtate with a CCTK Account resources.
**
** PARAMETERS: pCctkAccnt - Pointer to a valid CMCCTKACCNT object
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntRegister ( CMCCTKACCNT* pCctkAccnt );

/***********************************************************************************
** FUNCTION:   cmCctkAccntShow
**
** PURPOSE:    Dumps the CCTK Account resources.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntShow ( void );


/***********************************************************************************
** FUNCTION:   cmNumCidAssigned
**
** PURPOSE:    Finds whether this call id has any assigned CM user.
**
** PARAMETERS: bCreate - Whether resources should be initialized (BOS_TRUE) or ended
**                       (BOS_FALSE) when this call is invoked.
**
** RETURNS:    BOS_TRUE when .
***********************************************************************************/
BOS_UINT32 cmNumCidAssigned ( BOS_UINT32 uCid );


/***********************************************************************************
** FUNCTION:   cmGetAssocUsrList
**
** PURPOSE:    Finds the list of users with the associated network handle.
**
** PARAMETERS: uNet  (IN)  - the network handle to use
**             pList (OUT) - the pointer to the CMUSRLIST that we need to populate
**
** RETURNS:    None.
***********************************************************************************/
void cmGetAssocUsrList( BOS_UINT32 uNet, CMUSRLIST* pList );


/***********************************************************************************
** FUNCTION:   cmGetAssocActiveAccnt
**
** PURPOSE:    Finds an associated account which has an active call.
**
** PARAMETERS: pUsr(IN)  - the CMGR user to look up
**
** RETURNS:    CCTK user handle on success, CMGR_INVALID_HANDLE otherwise..
***********************************************************************************/
BOS_UINT32 cmGetAssocActiveAccnt( CMUSR* pUsr );


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
** FUNCTION:   cmCctkAccntAlloc
**
** PURPOSE:    Allocates a new CCTK user in the user configuration block.
**
** PARAMETERS: None.
**
** RETURNS:    Pointer to the newly created CCTK Account on success, NULL otherwise.
***********************************************************************************/
CMCCTKACCNT* cmCctkAccntAlloc( void );


/***********************************************************************************
** FUNCTION:   cmCctkAccntFree
**
** PURPOSE:    Free the specified CCTK account in the user configuration block.
**
** PARAMETERS: Pointer to the CCTK account to free
**
** RETURNS:    none
***********************************************************************************/
void cmCctkAccntFree( CMCCTKACCNT* pCctkAccnt );


/***********************************************************************************
** FUNCTION:   cmCctkAccntInit
**
** PURPOSE:    Creates the endpoint/cmuser associations for the specified CCTK  
**             account in the user configuration block.
**
** PARAMETERS: Pointer to the CCTK account to initialize
**
** RETURNS:    none
***********************************************************************************/
void cmCctkAccntInit( CMCCTKACCNT* pCctkAccnt );


/***********************************************************************************
** FUNCTION:   cmCctkAccntDeinit
**
** PURPOSE:    Removes all endpoint/cmuser mapping for the specified CCTK  
**             account in the user configuration block.
**
** PARAMETERS: Pointer to the CCTK account to deinitialize
**
** RETURNS:    none
***********************************************************************************/
void cmCctkAccntDeinit( CMCCTKACCNT* pCctkAccnt );


/***********************************************************************************
** FUNCTION:   cmCctkAccntFindByNet
**
** PURPOSE:    Gets a CCTK Account by its network handle.
**
** PARAMETERS: uNet -  The CCTK User handle.
**
** RETURNS:    Pointer to the CMCCTKACCNT if found, NULL otherwise.
***********************************************************************************/
CMCCTKACCNT* cmCctkAccntFindByNet( BOS_UINT32 uNet );


/***********************************************************************************
** FUNCTION:   cmCctkAccntFindByHdl
**
** PURPOSE:    Gets a CCTK Account by its Call Manager handle.
**
** PARAMETERS: uHdl -  The user handle.
**
** RETURNS:    Pointer to the CMCCTKACCNT if found, NULL otherwise.
***********************************************************************************/
CMCCTKACCNT* cmCctkAccntFindByHdl( BOS_UINT32 uHdl );


/***********************************************************************************
** FUNCTION:   cmCctkAccntForce
**
** PURPOSE:    Forces a CM user to use a specific CCTK user.
**
** PARAMETERS: uNetHdl  - CCTK user handle.
**             uUsrHdl  - CMUSR handle.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntForce( BOS_UINT32 uNetHdl, BOS_UINT32 uUsrHdl );


/***********************************************************************************
** FUNCTION:   cmCctkAccntUnforce
**
** PURPOSE:    Removes the CM user's forced CCTK user entry.
**
** PARAMETERS: uNetHdl  - CCTK user handle.
**             uUsrHdl  - CMUSR handle.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntUnforce( BOS_UINT32 uNetHdl, BOS_UINT32 uUsrHdl );


/***********************************************************************************
** FUNCTION:   cmCctkAccntAssociate
**
** PURPOSE:    Associates a CCTK user handle to a CM user handle.
**
** PARAMETERS: uNetHdl  - CCTK user handle.
**             uUsrHdl  - CMUSR handle.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntAssociate( BOS_UINT32 uNetHdl, BOS_UINT32 uUsrHdl );


/***********************************************************************************
** FUNCTION:   cmCctkAccntCallAdd
**
** PURPOSE:    Add a CCTK call. 
**
** PARAMETERS: uNet  - CCTK user handle.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntCallAdd( BOS_UINT32 uNet );


/***********************************************************************************
** FUNCTION:   cmCctkAccntCallDelete
**
** PURPOSE:    Deletes a call from the CCTK user.
**
** PARAMETERS: uNet  - CCTK user handle
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntCallDelete( BOS_UINT32 uNet );


/***********************************************************************************
** FUNCTION:   cmCctkNumActiveCalls
**
** PURPOSE:    Gets the number of active VoIP calls for a CCTK user.
**
** PARAMETERS: uNet  - the CCTK user handle to use
**
** RETURNS:    numActiveCalls on success, 0 otherwise.
***********************************************************************************/
BOS_UINT32 cmCctkNumActiveCalls( BOS_UINT32 uNet );


/***********************************************************************************
** FUNCTION:   cmCctkUpdateRouting
**
** PURPOSE:    Gets new CCTK user routing from provisioning.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmCctkUpdateRouting( void );


/***********************************************************************************
** FUNCTION:   getUsrCctkHdlFromRouting
**
** PURPOSE:    Gets a CCTK user handle based on routing rules and forced user rules.
**
** PARAMETERS: pUsr  - the user on which to run the routing lookup
**             bInUse - whether the function is being called to retreive 
**                      a handle in use (BOS_TRUE), or not (BOS_FALSE)
**
** RETURNS:    CCTK user handle on success, CMGR_INVALID_HANDLE otherwise.
***********************************************************************************/
BOS_UINT32 getUsrCctkHdlFromRouting( CMUSR* pUsr, BOS_BOOL bInUse );


/***********************************************************************************
** FUNCTION:   cmCctkAccntUpdateNet
**
** PURPOSE:    Updates the net handle associated with a cctk account
**
** PARAMETERS: pCcctkAccnt - pointer to the CCTK account to update
**             uNewNetHdl  - new net handle
**
** RETURNS:    none
***********************************************************************************/
void cmCctkAccntUpdateNet( CMCCTKACCNT* pCctkAccnt, BOS_UINT32 uNewNetHdl );

/***********************************************************************************
** FUNCTION:   cmCctkAccntReConfig
**
** PURPOSE:    Checks whether any of the CCTK accounts needs to be reconfigured.
**
** PARAMETERS: pCctkAccnt - the CCTK account to reconfigure, or NULL (apply to all
**             accounts).
**
** RETURNS:    Nothing.  An action will be taken if necessary.
***********************************************************************************/
void cmCctkAccntReConfig ( CMCCTKACCNT* pCctkAccnt );

/***********************************************************************************
** FUNCTION:   cmCctkAccntFindByUid
**
** PURPOSE:    Gets a CCTK Account by the Uid
**
** PARAMETERS: pUid - UId to retrieve the account for
**
** RETURNS:    Pointer to the CMCCTKACCNT if found, NULL otherwise.
***********************************************************************************/
CMCCTKACCNT* cmCctkAccntFindByUid( BOS_UINT8 *pUid );


#if defined(__cplusplus)
}
#endif

#endif /* __CM_CCTK_ACCNT__H__INCLUDED__ */

