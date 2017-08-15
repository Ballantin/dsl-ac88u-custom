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
**      This module implements user control from within the Call Manager.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

CMUSR *gCmUsr[ CMGRCFG_USER_MAX ];
BOS_BOOL gCmUsr2Phy [ CMGRCFG_PHY_MAX ];

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

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
void cmUsr ( BOS_BOOL bCreate )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;
   BOS_UINT8  uKx;

   if ( bCreate )
   {
      CMGRLOG_INFORMATION (( "cmUsr: creating user support." ));

      for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
      {
         gCmUsr[ uIx ] = (CMUSR *) malloc ( sizeof( CMUSR ) );
         memset ( gCmUsr[ uIx ], 0, sizeof( CMUSR ) );

         gCmUsr[ uIx ]->uHdl = uIx;
         gCmUsr[ uIx ]->uPhys = CMGR_INVALID_HANDLE;
         cmUsrSetNtwkHdl( gCmUsr[ uIx ], CMGR_INVALID_HANDLE );
         gCmUsr[ uIx ]->uCfgTone = CMGR_INVALID_HANDLE;
         gCmUsr[ uIx ]->eCfgAction = eCMCFGACT_NOACTION;
         gCmUsr[ uIx ]->bDtLnk = BOS_TRUE;
         gCmUsr[ uIx ]->uUindex = CMGR_INVALID_HANDLE;

         gCmUsr[ uIx ]->pSesWait = NULL;
         gCmUsr[ uIx ]->pSesHeld = NULL;
         gCmUsr[ uIx ]->pAutoCall = NULL;
         gCmUsr[ uIx ]->uLinkedLocalUserHdl = CMGR_INVALID_HANDLE;
         gCmUsr[ uIx ]->uLinkedRemoteUserCnt = 0;
         for ( uJx = 0 ; uJx < CMGRCFG_USER_MAX ; uJx++ )
         {
            gCmUsr[ uIx ]->uLinkedRemoteUserHdl[uJx] = CMGR_INVALID_HANDLE;
         }

         /* Initial user state is out of service.
         */
         gCmUsr[ uIx ]->bInSrv = BOS_FALSE;

         gCmUsr[ uIx ]->pConfBridgeStatus = (CMCONFBRIDGE *) malloc ( sizeof(CMCONFBRIDGE) );
         memset( gCmUsr[ uIx ]->pConfBridgeStatus, 0, sizeof(CMCONFBRIDGE) );
         gCmUsr[ uIx ]->pConfBridgeStatus->eRequestStatus = CMCONF_STATUS_IDLE;
         gCmUsr[ uIx ]->pConfBridgeStatus->eConfOption = CFG_SIP_CONFERENCING_LOCAL; /* default conferencing option */

#if CMGRCFG_MULTILINE
         gCmUsr[ uIx ]->bAlarmActive = BOS_FALSE;
         gCmUsr[ uIx ]->eAlarmCfgState = CMALGALRMCFGSTATE_NONE;
#endif

         if ( gCmUsr[ uIx ] )
         {
            gCmUsr[ uIx ]->pDmState =
               (BOS_UINT8 *) malloc ( CMDM_ACTION_BUFFER_SIZE * sizeof(BOS_UINT8) );
            memset( gCmUsr[ uIx ]->pDmState,
                    0,
                    CMDM_ACTION_BUFFER_SIZE * sizeof(BOS_UINT8) );
            gCmUsr[ uIx ]->pDmDigMap =
               (BOS_UINT8 *) malloc ( CMDM_MAP_BUFFER_SIZE * sizeof(BOS_UINT8) );
            memset( gCmUsr[ uIx ]->pDmDigMap,
                    0,
                    CMDM_MAP_BUFFER_SIZE * sizeof(BOS_UINT8) );
            gCmUsr[ uIx ]->pDmInUse = NULL;

            for ( uJx = 0 ; uJx < (CMGRCFG_AUTOCALL_MAX / 2) ; uJx++ )
            {
               gCmUsr[ uIx ]->pAc[ uJx ] =
                  (CMAUTOCALL *) malloc ( sizeof ( CMAUTOCALL ) );
               memset ( gCmUsr[ uIx ]->pAc[ uJx ], 0, sizeof( CMAUTOCALL ) );
               gCmUsr[ uIx ]->pAc[ uJx ]->eType = eCMDM_ACACTIVATE;
            }
            for ( uJx = 0 ; uJx < (CMGRCFG_AUTOCALL_MAX / 2) ; uJx++ )
            {
               gCmUsr[ uIx ]->pAr[ uJx ] =
                  (CMAUTOCALL *) malloc ( sizeof ( CMAUTOCALL ) );
               memset ( gCmUsr[ uIx ]->pAr[ uJx ], 0, sizeof( CMAUTOCALL ) );
               gCmUsr[ uIx ]->pAr[ uJx ]->eType = eCMDM_ARACTIVATE;
            }
         }
      }

      for ( uIx = 0 ; uIx < CMGRCFG_PHY_MAX ; uIx++ )
      {
         gCmUsr2Phy [ uIx ] = BOS_FALSE;
      }
   }
   else
   {
      CMGRLOG_INFORMATION (( "cmUsr: terminating user support." ));

      for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
      {
         if ( gCmUsr[ uIx ] )
         {
            free ( gCmUsr[ uIx ]->pDmState );
            gCmUsr[ uIx ]->pDmState = NULL;

            free ( gCmUsr[ uIx ]->pDmDigMap );
            gCmUsr[ uIx ]->pDmDigMap = NULL;
            gCmUsr[ uIx ]->pDmInUse = NULL;

            for ( uJx = 0 ; uJx < (CMGRCFG_AUTOCALL_MAX / 2) ; uJx++ )
            {
               free ( gCmUsr[ uIx ]->pAc[ uJx ] );
               gCmUsr[ uIx ]->pAc[ uJx ] = NULL;
            }
            for ( uJx = 0 ; uJx < (CMGRCFG_AUTOCALL_MAX / 2) ; uJx++ )
            {
               free ( gCmUsr[ uIx ]->pAr[ uJx ] );
               gCmUsr[ uIx ]->pAr[ uJx ] = NULL;
            }

            free( gCmUsr[ uIx ]->pConfBridgeStatus );

            memset ( gCmUsr[ uIx ], 0, sizeof(CMUSR) );

            free ( gCmUsr[ uIx ] );
            gCmUsr[ uIx ] = NULL;
         }
      }
   }
}


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
void cmUsrNewState ( CMUSR *pUsr, CMFSMSTATE eState )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;

#if CMGRCFG_DEBUG_INFO
   if ( gCmCfgCb.cmgrEvtCb )
   {
      gCmCfgCb.cmgrEvtCb ( eCMGREVT_DEBUG_INFO,
                           pUsr->uPhys,
#if CMGRCFG_MULTILINE
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                           (void *) eState );
   }
#endif

   /* Push the information down the stack by one.  Push both the state
   ** and the event information (since event information is kept per state
   ** for better accuracy).
   */
   for ( uIx = CMGRCFG_STATE_MAX - 1 ; uIx >= 1 ; uIx-- )
   {
      pUsr->eSta[ uIx ] = pUsr->eSta[ uIx - 1 ];
      for ( uJx = 0 ; uJx < CMGRCFG_EVENT_MAX ; uJx++ )
      {
         pUsr->eEvt[ uIx ][ uJx ] = pUsr->eEvt[ uIx - 1 ][ uJx ];
      }
   }

   /* Set the new state information on top.
   */
   pUsr->eSta[ 0 ] = eState;
   for ( uJx = 0 ; uJx < CMGRCFG_EVENT_MAX ; uJx++ )
   {
      pUsr->eEvt[ 0 ][ uJx ] = eCMEVT_INVALID;
   }

   /* Check to apply features triggered immediately when a state
   ** transition applies.
   */
   switch ( eState )
   {
      case eCMST_IDLE:
      {
         /* Check whether any of the auto-call feature is pending.  If so,
         ** we invoke it now.
         */
         for ( uIx = 0 ; uIx < (CMGRCFG_AUTOCALL_MAX / 2) ; uIx++ )
         {
            if ( pUsr->pAc[ uIx ] && pUsr->pAc[ uIx ]->bUsed &&
                 pUsr->pAc[ uIx ]->bPending )
            {
               cmFsmAutoCallApply ( (void *) pUsr );
               break;
            }
            else if ( pUsr->pAr[ uIx ] && pUsr->pAr[ uIx ]->bUsed &&
                      pUsr->pAr[ uIx ]->bPending )
            {
               cmFsmAutoCallApply ( (void *) pUsr );
               break;
            }
         }
      }
      break;

      case eCMST_ERROR:
      {
         pUsr->eError = eCMRESERROR_ENTRY;
      }
      break;

      default:
      break;
   }
}


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
void cmUsrNewEvent ( CMUSR *pUsr, CMFSMEVT eEvent )
{
   BOS_UINT32 uIx;

   /* Push the information down the stack by one.
   */
   for ( uIx = CMGRCFG_EVENT_MAX - 1 ; uIx >= 1 ; uIx-- )
   {
      pUsr->eEvt[ 0 ][ uIx ] = pUsr->eEvt[ 0 ][ uIx - 1 ];
   }

   /* Set the new event information on top.
   */
   pUsr->eEvt[ 0 ][ 0 ] = eEvent;
}


/***********************************************************************************
** FUNCTION:   cmUsrValid
**
** PURPOSE:    Checks whether a user handle is for a valid user.
**
** PARAMETERS: uHdl - the user handle to check for.
**
** RETURNS:    Pointer to valid user or NULL if not found.
***********************************************************************************/
CMUSR * cmUsrValid ( BOS_UINT32 uHdl )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] && gCmUsr[ uIx ]->uHdl == uHdl )
      {
         return gCmUsr[ uIx ];
      }
   }

   return NULL;
}


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
CMUSR * cmUsrFindNetHdl ( BOS_UINT32 uNet )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] && ( cmUsrGetNtwkHdl( gCmUsr[ uIx ] ) == uNet ) )
      {
         return gCmUsr[ uIx ];
      }
   }

   return NULL;
}


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
CMUSR * cmUsrFindPhyHdl ( BOS_UINT32 uPhy )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] && ( gCmUsr[ uIx ]->uPhys == uPhy ) )
      {
         return gCmUsr[ uIx ];
      }
   }

   return NULL;
}


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
CMUSR * cmUsrFindUidHdl ( BOS_UINT8 *pUid )
{
   BOS_UINT32 uIx;

   if ( pUid == NULL )
   {
      return NULL;
   }

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] &&
           strlen ( (const char *) gCmUsr[ uIx ]->uUid ) &&
           !strcmp ( (const char *) gCmUsr[ uIx ]->uUid,
                     (const char *) pUid ) )
      {
         return gCmUsr[ uIx ];
      }
   }

   return NULL;
}


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
BOS_BOOL cmUsrLinkSes ( CMUSR *pUsr, CMSES *pSes )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uSize;

   BOS_ASSERT ( pUsr != NULL );
   BOS_ASSERT ( pSes != NULL );

   uSize = ( ( pSes->eType == eCMSESTYPE_MEDIA ) ?
               ( CMGRCFG_MEDSES_MAX + 1 ) : ( CMGRCFG_SRVSES_MAX + 1 ) );

   for ( uIx = 0 ; uIx < uSize ; uIx++ )
   {
      if ( ( pSes->eType == eCMSESTYPE_MEDIA ) &&
           ( pUsr->pMedSes [ uIx ] == NULL ) )
      {
         pUsr->pMedSes [ uIx ] = pSes;
         pSes->uUsr = pUsr->uHdl;
         return BOS_TRUE;
      }
      else if ( ( pSes->eType == eCMSESTYPE_SRV ) &&
           ( pUsr->pSrvSes [ uIx ] == NULL ) )
      {
         pUsr->pSrvSes [ uIx ] = pSes;
         pSes->uUsr = pUsr->uHdl;
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
** FUNCTION:   cmUsrUnLinkSes
**
** PURPOSE:    Un-links a session to a user.
**
** PARAMETERS: pUsr - the user to un-link from.
**             pSes - the session to un-link.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrUnLinkSes ( CMUSR *pUsr, CMSES *pSes )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uSize;

   BOS_ASSERT ( pUsr != NULL );
   BOS_ASSERT ( pSes != NULL );

   uSize = ( ( pSes->eType == eCMSESTYPE_MEDIA ) ?
               ( CMGRCFG_MEDSES_MAX + 1 ) : ( CMGRCFG_SRVSES_MAX + 1 ) );

   for ( uIx = 0 ; uIx < uSize ; uIx++ )
   {
      if ( ( pSes->eType == eCMSESTYPE_MEDIA ) &&
           ( pUsr->pMedSes [ uIx ] == pSes ) )
      {
         if ( pSes->ses.med.uConf != CMGR_INVALID_HANDLE )
         {
            cmUsrUnLinkConfSes ( pUsr, pSes->ses.med.uConf );
            pSes->ses.med.uConf = CMGR_INVALID_HANDLE;
         }

         if ( pSes->ses.med.uBargeIn != CMGR_INVALID_HANDLE )
         {
            pSes->ses.med.uBargeIn = CMGR_INVALID_HANDLE;
         }

         pUsr->pMedSes [ uIx ] = NULL;
         return;
      }
      else if ( ( pSes->eType == eCMSESTYPE_SRV ) &&
           ( pUsr->pSrvSes [ uIx ] == pSes ) )
      {
         pUsr->pSrvSes [ uIx ] = NULL;
         return;
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmUsrUnLinkConfSes
**
** PURPOSE:    Un-links a conferenced session from a user.
**
** PARAMETERS: pUsr - the user to un-link from.
**             uSes - the conference session handle to un-link.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrUnLinkConfSes ( CMUSR *pUsr, BOS_UINT32 uSes )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] &&
           ( pUsr->pMedSes [ uIx ]->uHdl == uSes ) )
      {
         if ( pUsr->pMedSes [ uIx ]->ses.med.uConf != CMGR_INVALID_HANDLE )
         {
            pUsr->pMedSes [ uIx ]->ses.med.uConf = CMGR_INVALID_HANDLE;
            /* Update the media information now that the conference is over.
            */
            cmSesPhyUpdate ( pUsr->pMedSes [ uIx ], BOS_FALSE );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmUsrFocusSes
**
** PURPOSE:    Determines the focus session associated with a user.
**
** PARAMETERS: pUsr - the user of interest.
**
** RETURNS:    The focus session for this user, or NULL if not applicable.
***********************************************************************************/
CMSES * cmUsrFocusSes ( CMUSR *pUsr )
{
   BOS_UINT32 uIx;

   BOS_ASSERT ( pUsr != NULL );

   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] &&
           pUsr->pMedSes [ uIx ]->ses.med.bFocus &&
           !pUsr->pMedSes [ uIx ]->ses.med.bBgFocus )
      {
         return pUsr->pMedSes [ uIx ];
      }
   }

   return NULL;
}


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
CMSES * cmUsrActiveSes ( CMUSR *pUsr )
{
   BOS_UINT32 uIx;

   BOS_ASSERT ( pUsr != NULL );

   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] &&
           !pUsr->pMedSes [ uIx ]->ses.med.bFocus &&
           !pUsr->pMedSes [ uIx ]->ses.med.bBgFocus &&
           ( pUsr->pMedSes [ uIx ]->ses.med.eAction != eCMDM_HOLDACTIVATE ) )
      {
         return pUsr->pMedSes [ uIx ];
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmUsrAlloc
**
** PURPOSE:    Allocates a free user resources.
**
** PARAMETERS: None.
**
** RETURNS:    Pointer to the allocated user on success, NULL otherwise.
***********************************************************************************/
CMUSR * cmUsrAlloc ( void )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] && !gCmUsr[ uIx ]->bInUse )
      {
         gCmUsr[ uIx ]->bInUse = BOS_TRUE;
         gCmUsr[ uIx ]->bInSrv = BOS_FALSE;
         gCmUsr[ uIx ]->uPhys = CMGR_INVALID_HANDLE;
         gCmUsr[ uIx ]->uLinkedLocalUserHdl = CMGR_INVALID_HANDLE;
         gCmUsr[ uIx ]->uLinkedRemoteUserCnt = 0;
         for ( uJx = 0 ; uJx < CMGRCFG_USER_MAX ; uJx++ )
         {
            gCmUsr[ uIx ]->uLinkedRemoteUserHdl[uJx] = CMGR_INVALID_HANDLE;
         }
         return gCmUsr[ uIx ];
      }
   }

   return NULL;
}

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
void cmUsrPhyCreate ( BOS_UINT32 uHdl )
{
   BOS_UINT32 uIx;
   CFGSIPUSERCFG *pUsrCfg = NULL;
   PROV_CFG_IDX cfgIdx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
#if CMGRCFG_MULTILINE
      /* Since this is a 1-1 relationship between a CM user and uPhy, initialize
      ** the uPhy now.
      */
      if ( gCmUsr[ uIx ] &&
           ( ( uHdl == CMGR_INVALID_HANDLE ) &&
             ( gCmUsr[ uIx ]->uPhys != CMGR_INVALID_HANDLE ) ) ||
           ( ( uHdl != CMGR_INVALID_HANDLE ) &&
               ( gCmUsr[ uIx ]->uHdl == uHdl ) ) )
      {
         if ( !gCmUsr2Phy [ gCmUsr[uIx]->uPhys ] )
         {
            cmPhyCreate ( gCmUsr[uIx]->uPhys );
            gCmUsr2Phy [ gCmUsr[uIx]->uPhys ] = BOS_TRUE;
         }
      }
#else
      /* If the user is valid, retrieve the physical interface(s) it is allowed
      ** to use, make sure all of them are created properly.
      **
      ** For interface shared across multiple users, only create it once.
      */
      if ( gCmUsr[ uIx ] &&
           ( ( uHdl == CMGR_INVALID_HANDLE ) ||
              ( ( uHdl != CMGR_INVALID_HANDLE ) &&
                  ( gCmUsr[ uIx ]->uHdl == uHdl ) ) ) &&
           strlen ( (char *)gCmUsr[ uIx ]->uUid ) )
      {
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = gCmUsr[ uIx ]->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                    &cfgIdx,
                                    (void *) &pUsrCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            CFGSIPUSERENDPNTMAP *pEpt = pUsrCfg->pEndPntMap;
            while ( pEpt )
            {
               if ( (pEpt->bIn || pEpt->bOut) &&
                    !gCmUsr2Phy [ pEpt->sEndpntNum ] )
               {
                  cmPhyCreate ( pEpt->sEndpntNum );
                  gCmUsr2Phy [ pEpt->sEndpntNum ] = BOS_TRUE;
               }
               /* Increment the interface.
               */
               pEpt = pEpt->pNext;
            };

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                  &cfgIdx,
                                  (void *) pUsrCfg,
                                  CFG_ITEM_DEL );
            pUsrCfg = NULL;
         }
      }
#endif
   }
}


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
void cmUsrPhyRelease ( BOS_UINT32 uHdl )
{
   if ( uHdl < CMGRCFG_PHY_MAX )
   {
      gCmUsr2Phy [ uHdl ] = BOS_FALSE;
   }
}


/***********************************************************************************
** FUNCTION:   cmUsrRegister
**
** PURPOSE:    Registers a user with the network layer.
**
** PARAMETERS: uHdl - the user handle of interest.
**
** RETURNS:    None.
**
** NOTE:       If the user handle is set to 'CMGR_INVALID_HANDLE', then the action
**             applies to all of the users.
***********************************************************************************/
void cmUsrRegister ( BOS_UINT32 uHdl )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( ( gCmUsr[ uIx ] &&
             ( gCmUsr[ uIx ]->uHdl == uHdl ) )
           || ( gCmUsr[ uIx ] && ( uHdl == CMGR_INVALID_HANDLE ) &&
                strlen ( (char *)gCmUsr[ uIx ]->uUid ) ) )
      {
         CFGSIPUSERCFG *pUsrCfg = NULL;
         PROV_CFG_IDX cfgIdx;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = gCmUsr[ uIx ]->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                    &cfgIdx,
                                    (void *) &pUsrCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pUsrCfg && pUsrCfg->Act.bCfgStatus )
            {
               if ( pUsrCfg->uInitRegDelay &&
                    !gCmUsr[ uIx ]->bRegNoDelay )
               {
                  CMTMR tmr;

                  memset ( &tmr, 0, sizeof ( CMTMR ) );

                  tmr.uSes = CMGR_INVALID_HANDLE;
                  tmr.uUsr = uHdl;
                  tmr.eType = eCMEVT_TMR_INIT_REG_DELAY;
                  tmr.uTimeOut = ( rand() % pUsrCfg->uInitRegDelay );

                  cmTmrStart ( &tmr );

                  CMGRLOG_INFORMATION ((
                     "cmUsrRegister: 0x%X (0x%X) - delay for %d msec",
                     gCmUsr[ uIx ]->uHdl, cmUsrGetCurrNtwkHdl( gCmUsr[ uIx ] ),
                     tmr.uTimeOut ));
               }
               else
               {
#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_RFC3261 && !(CMGRCFG_MULTILINE))
                  CFGSIPPCSCF *pPcscfUsr = NULL;
                  PROV_CFG_IDX cfgIdx;
                  BOS_BOOL bImplicitReg = BOS_FALSE;
#endif

                  CMGRLOG_INFORMATION ((
                     "cmUsrRegister: 0x%X (0x%X) - 'no-delay' (%s)",
                     gCmUsr[ uIx ]->uHdl, gCmUsr[ uIx ]->uNet,
                     (gCmUsr[ uIx ]->bRegNoDelay ? "yes->no" : "no") ));

                  if ( gCmUsr[ uIx ]->bRegNoDelay )
                  {
                     gCmUsr[ uIx ]->bRegNoDelay = BOS_FALSE;
                  }
#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_STANDALONE) || ( CMGRCFG_MULTILINE )
                  /* cmUsrs get put in service right away in CATIQ mode because a cmUSR
                  ** refers to a terminal and not the sip account. The terminal needs to
                  ** be in service for internal calls even if its associated sip account
                  ** ( CCTK user ) is not in service
                  */
                  gCmUsr[ uIx ]->bInSrv = BOS_TRUE;
#else
                  /* When in RFC 3261 mode, a special case is to be considered for
                  ** implicit registration which means the user does not need to be
                  ** registered to be set 'in-service' automatically.
                  */
#   if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_RFC3261)
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = pUsrCfg->pcActiveDomain;
                  cfgIdx.uIndex = pUsrCfg->uActiveDomain;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                             &cfgIdx,
                                             (void *) &pPcscfUsr,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( bosIpAddrIsZero ( &pPcscfUsr->PCSCFAddr ) )
                     {
                        bImplicitReg = BOS_TRUE;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                           &cfgIdx,
                                           (void *) pPcscfUsr,
                                           CFG_ITEM_DEL );
                     pPcscfUsr = NULL;
                  }


                  if ( bImplicitReg )
                  {
                     gCmUsr[ uIx ]->bInSrv = BOS_TRUE;

                     if ( gCmCfgCb.cmgrEvtCb )
                     {
                        ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_OK,
                                                 gCmUsr[ uIx ]->uHdl,
#if CMGRCFG_MULTILINE
                                                 gCmUsr[ uIx ]->uHdl,
                                                 cmCctkAccntFindByNet( cmUsrGetNtwkHdl( gCmUsr[ uIx ]) )->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                                 (void *) gCmUsr[ uIx ]->uUid );
                     }
                  }
                  else
#   endif
                  {
                     cctkUserRegister ( cmUsrGetNtwkHdl( gCmUsr[ uIx ] ) );

                     if ( gCmCfgCb.cmgrEvtCb )
                     {
                        /* Inform host that registration is in progress */
                        ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_PROGRESS,
                                                 gCmUsr[ uIx ]->uHdl,
#if CMGRCFG_MULTILINE
                                                 gCmUsr[ uIx ]->uHdl,
                                                 cmCctkAccntFindByNet( cmUsrGetNtwkHdl( gCmUsr[ uIx ]) )->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                                 (void *) eCMGREVT_OPER_OK );
                     }
                  }
#endif
               }
            }
            else
            {
               CMGRLOG_WARNING (( "cmUsrRegister: 0x%X (0x%X) - Config Disabled",
                                  gCmUsr[ uIx ]->uHdl, cmUsrGetCurrNtwkHdl( gCmUsr[ uIx ] ) ));

               gCmUsr[ uIx ]->bInSrv = BOS_FALSE;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                  &cfgIdx,
                                  (void *) pUsrCfg,
                                  CFG_ITEM_DEL );
            pUsrCfg = NULL;
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmUsrPhyAssociate
**
** PURPOSE:    Associates a user to a physical interface.
**
** PARAMETERS: uPhy - the physical interface to be associated with.
**
** RETURNS:    The associated user, or NULL if none can be associated.
***********************************************************************************/
CMUSR * cmUsrPhyAssociate ( BOS_UINT32 uPhy )
{
   CFGSIPUSERCFG *pUsrCfg = NULL;
   CMUSR *pUsr = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
#if CMGRCFG_MULTILINE
      /* Nothing to do since there is a 1-1 mapping between CM users and uPhy */
      if ( gCmUsr[ uIx ] && gCmUsr[ uIx ]->uPhys == uPhy )
      {
         pUsr = gCmUsr[ uIx ];
         break;
      }
#else
      /* Check if a non associated user can be associated with this
      ** interface, if so do it now.
      */
      if ( ( pUsr == NULL ) && gCmUsr[ uIx ] &&
           strlen ( (const char *) gCmUsr[ uIx ]->uUid ) &&
           ( gCmUsr[ uIx ]->uPhys == CMGR_INVALID_HANDLE ) )
      {
         switch ( gCmUsr[ uIx ]->eType )
         {
            case eCMUSRTYPE_LINE:
            case eCMUSRTYPE_PSTN:
            {
               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = gCmUsr[ uIx ]->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                          &cfgIdx,
                                          (void *) &pUsrCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  CFGSIPUSERENDPNTMAP *pEpt = pUsrCfg->pEndPntMap;
                  while ( pEpt )
                  {
                     if ( ( pUsr == NULL ) &&
                          ( pEpt->sEndpntNum == uPhy ) &&
                          ( pEpt->bIn || pEpt->bOut ) )
                     {
                        /* Make sure this physical resource is not already used
                        ** by someone else.
                        */
                        if ( cmUsrFindPhyHdl ( pEpt->sEndpntNum ) == NULL )
                        {
                           pUsr = gCmUsr[ uIx ];
                           pUsr->uPhys = uPhy;
                           break;
                        }
                     }
                     /* Increment the interface.
                     */
                     pEpt = pEpt->pNext;
                  };

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                        &cfgIdx,
                                        (void *) pUsrCfg,
                                        CFG_ITEM_DEL );
                  pUsrCfg = NULL;
               }
            }
            break;

            case eCMUSRTYPE_SHAREDLINE:
            {
               /* Shared line is a tricky case.  One should ensure that the
               ** resource can be used at this particular time.
               */

               /* Unfinished at this time... */
            }
            break;

            default:
            break;
         }
      }
#endif
   }

   CMGRLOG_INFORMATION (( "cmUsrPhyAssociate: phy 0x%X -> usr 0x%X",
                          uPhy, pUsr ));

   return pUsr;
}

/***********************************************************************************
** FUNCTION:   cmUsrPhyDisassociate
**
** PURPOSE:    Disassociates a physical interface from a user
**
** PARAMETERS: pUsr - the user of interest for the disassociation
**
** RETURNS:    none
***********************************************************************************/
void cmUsrPhyDisassociate ( CMUSR *pUsr )
{
#if !CMGRCFG_MULTILINE
   if ( pUsr )
   {
      /* Disassociation is only required for non-multi-line applications since
      ** for multiline, there's always a 1-1 relationship between the CM user and
      ** a phy.
      */
      pUsr->uPhys = CMGR_INVALID_HANDLE;
   }
#endif
}


/***********************************************************************************
** FUNCTION:   cmUsrFreeAc
**
** PURPOSE:    Locates a free auto-call block from the user in order to manage
**             an auto-call feature.
**
** PARAMETERS: pUsr - the user of interest.
**             eAction - the action type (can be auto-callback or auto-recall).
**
** RETURNS:    The associated auto-call information, or NULL if none can be
**             associated.
***********************************************************************************/
CMAUTOCALL * cmUsrFreeAc( CMUSR *pUsr, CMDMACTIONTYPE eAction )
{
   BOS_UINT32 uIx;
   CMAUTOCALL *pAutoCall = NULL;

   for ( uIx = 0 ; uIx < ( CMGRCFG_AUTOCALL_MAX / 2 ) ; uIx++ )
   {
      if ( ( eAction == eCMDM_ACACTIVATE ) &&
           ( pUsr->pAc[ uIx ] != NULL ) &&
           ( !pUsr->pAc[ uIx ]->bUsed ) )
      {
         pAutoCall = pUsr->pAc[ uIx ];
         break;
      }

      if ( ( eAction == eCMDM_ARACTIVATE ) &&
           ( pUsr->pAr[ uIx ] != NULL ) &&
           ( !pUsr->pAr[ uIx ]->bUsed ) )
      {
         pAutoCall = pUsr->pAr[ uIx ];
         break;
      }
   }

   return pAutoCall;
}


/***********************************************************************************
** FUNCTION:   cmUsrMostRecentAc
**
** PURPOSE:    Locates the most recent auto-call block for the user.
**
** PARAMETERS: pUsr - the user of interest.
**             eAction - the action type (can be auto-callback or auto-recall).
**
** RETURNS:    The associated auto-call information, or NULL if none can be
**             associated.
***********************************************************************************/
CMAUTOCALL * cmUsrMostRecentAc( CMUSR *pUsr, CMDMACTIONTYPE eAction )
{
   BOS_UINT32 uIx;
   CMAUTOCALL *pAutoCall = NULL;
   BOS_TIME_MS time;

   bosTimeGetMs ( &time );

   for ( uIx = 0 ; uIx < ( CMGRCFG_AUTOCALL_MAX / 2 ) ; uIx++ )
   {
      if ( ( eAction == eCMDM_ACDEACTIVATE ) &&
           ( pUsr->pAc[ uIx ] != NULL ) &&
           ( pUsr->pAc[ uIx ]->bUsed ) )
      {
         if ( pAutoCall )
         {
            BOS_TIME_MS delTime_1;
            BOS_TIME_MS delTime_2;

            bosTimeCalcDeltaMs( &time, &pAutoCall->time, &delTime_1 );
            bosTimeCalcDeltaMs( &time, &pUsr->pAc[ uIx ]->time, &delTime_2 );

            if ( delTime_2 < delTime_1 )
            {
               pAutoCall = pUsr->pAc[ uIx ];
            }
         }
         else
         {
            pAutoCall = pUsr->pAc[ uIx ];
         }
      }

      if ( ( eAction == eCMDM_ARDEACTIVATE ) &&
           ( pUsr->pAr[ uIx ] != NULL ) &&
           ( pUsr->pAr[ uIx ]->bUsed ) )
      {
         if ( pAutoCall )
         {
            BOS_TIME_MS delTime_1;
            BOS_TIME_MS delTime_2;

            bosTimeCalcDeltaMs( &time, &pAutoCall->time, &delTime_1 );
            bosTimeCalcDeltaMs( &time, &pUsr->pAr[ uIx ]->time, &delTime_2 );

            if ( delTime_2 < delTime_1 )
            {
               pAutoCall = pUsr->pAr[ uIx ];
            }
         }
         else
         {
            pAutoCall = pUsr->pAr[ uIx ];
         }
      }
   }

   return pAutoCall;
}


/***********************************************************************************
** FUNCTION:   cmUsrReactivateAc
**
** PURPOSE:    Checks whether the last auto-call action is in fact a reactivation
**             of an already existing auto-call action.
**
** PARAMETERS: pUsr - the user of interest.
**             eAction - the action type (can be auto-callback or auto-recall).
**
** RETURNS:    BOS_TRUE - if this is a reactivation.
**             BOS_FALSE - otherwise.
***********************************************************************************/
BOS_BOOL cmUsrReactivateAc ( CMUSR *pUsr, CMDMACTIONTYPE eAction )
{
   BOS_BOOL bRet = BOS_FALSE;
   BOS_UINT32 uIx;
   CCTKCDR *pCdr = cmCdrGet();
   CFGSIPAUTOCALL *pACall;
   PROV_CFG_IDX cfgIdx;
   CMTMR tmr;
   CCTKPARM eParm =
      ( ( eAction == eCMDM_ARACTIVATE ) ? eCCTKPARM_LASTCALLIN :
                                             eCCTKPARM_LASTCALLOUT );

   memset ( (void *) &tmr,
            0,
            sizeof ( CMTMR ) );

   memset ( (void *) pCdr,
            0,
            sizeof ( CCTKCDR ) );

   if ( pCdr && ( cctkGetParm ( cmUsrGetNtwkHdl( pUsr ),
                                eParm,
                                (void *) pCdr )
                                       == eCCTKSTATUS_SUCCESS ) )
   {
      for ( uIx = 0 ; uIx < ( CMGRCFG_AUTOCALL_MAX / 2 ) ; uIx++ )
      {
         if ( ( eAction == eCMDM_ACACTIVATE ) &&
              ( pUsr->pAc[ uIx ] != NULL ) &&
              ( pUsr->pAc[ uIx ]->bUsed ) &&
              ( !strlen ( (const char *) pUsr->pAc[ uIx ]->uPeer ) ||
              ( strlen ( (const char *) pUsr->pAc[ uIx ]->uPeer ) &&
              ( !strcmp( (const char *) pUsr->pAc[ uIx ]->uPeer,
                         (const char *) pCdr->uPeer ) ) ) ) )
         {
            if (  !pUsr->pAc[ uIx ]->bAnonymous )
            {
               CMSES *pSes = cmSesValid ( pUsr->pAc[ uIx ]->uSes );
               /* Reactivation equals refresh the subscription.
               */
               if ( pSes )
               {
                  cctkEventSubscribe ( pSes->uHdl );

                  tmr.uSes = pSes->uHdl;
                  tmr.uUsr = pUsr->uHdl;
                  tmr.eType = eCMEVT_TMR_AUTOCALLBACK;

                  cmTmrStop ( &tmr );

                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = pUsr->uUid;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                             &cfgIdx,
                                             (void *) &pACall,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     tmr.uTimeOut = pACall->uTimer;
                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                           &cfgIdx,
                                           (void *) pACall,
                                           CFG_ITEM_DEL );
                     pACall = NULL;
                  }
                  else
                  {
                     tmr.uTimeOut = CMGR_AUTO_CALL_TIMEOUT;
                  }

                  cmTmrStart ( &tmr );
               }
            }
            bRet = BOS_TRUE;
         }

         if ( ( eAction == eCMDM_ARACTIVATE ) &&
              ( pUsr->pAr[ uIx ] != NULL ) &&
              ( pUsr->pAr[ uIx ]->bUsed ) &&
              ( !strlen ( (const char *) pUsr->pAc[ uIx ]->uPeer ) ||
              ( strlen ( (const char *) pUsr->pAr[ uIx ]->uPeer ) &&
              ( !strcmp( (const char *) pUsr->pAr[ uIx ]->uPeer,
                         (const char *) pCdr->uPeer ) ) ) ) )
         {
            if (  !pUsr->pAr[ uIx ]->bAnonymous )
            {
               CMSES *pSes = cmSesValid ( pUsr->pAr[ uIx ]->uSes );
               /* Reactivation equals refresh the subscription.
               */
               if ( pSes )
               {
                  cctkEventSubscribe ( pSes->uHdl );

                  tmr.uSes = pSes->uHdl;
                  tmr.uUsr = pUsr->uHdl;
                  tmr.eType = eCMEVT_TMR_AUTORECALL;

                  cmTmrStop ( &tmr );

                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = pUsr->uUid;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                             &cfgIdx,
                                             (void *) &pACall,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     tmr.uTimeOut = pACall->uTimer;
                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                           &cfgIdx,
                                           (void *) pACall,
                                           CFG_ITEM_DEL );
                     pACall = NULL;
                  }
                  else
                  {
                     tmr.uTimeOut = CMGR_AUTO_CALL_TIMEOUT;
                  }

                  cmTmrStart ( &tmr );
               }
            }

            bRet = BOS_TRUE;
         }
      }
   }

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmUsrReConfig
**
** PURPOSE:    Checks whether any of the user needs to be reconfigured.
**
** PARAMETERS: pUsr - the user to reconfigure, or NULL (apply to all users).
**
** RETURNS:    Nothing.  An action will be taken if necessary.
***********************************************************************************/
void cmUsrReConfig ( CMUSR *pUsr )
{
   BOS_UINT32 uIx;
   CMTMR tmr;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      /* Apply to all users or just the specific one.
      */
      if ( ( pUsr == NULL ) ||
           ( ( pUsr != NULL ) && ( gCmUsr[ uIx ] == pUsr ) ) )
      {
         switch ( gCmUsr[ uIx ]->eCfgAction )
         {
            /* This reconfiguration does not require the user to impact its network
            ** status, which means stable calls are not affected, etc...
            **
            ** The only piece of configuration which falls into this category at
            ** this time is the digit map configuration.  We therefore only need to
            ** make sure we are not trying to dial at this time.
            */
            case eCMCFGACT_CFGUSRNONET:
            {
               /* Terminate user activation timer if it is running since
               ** we want to reconfigure the user now.
               */
               memset ( &tmr, 0, sizeof ( CMTMR ) );
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = gCmUsr[ uIx ]->uHdl;
               tmr.eType = eCMEVT_TMR_INIT_REG_DELAY;
               if ( cmTmrIsRunning ( &tmr ) )
               {
                  cmTmrStop ( &tmr );
               }

               if ( gCmUsr[ uIx ]->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_DIALING )
               {
                  CFGSIPDIGITMAP *pUsrDigMap = NULL;
                  PROV_CFG_IDX cfgIdx;
                  CMDMSTATUS eDmStatus = eCMDM_ERROR;

                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = gCmUsr[ uIx ]->uUid;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                             &cfgIdx,
                                             (void *) &pUsrDigMap,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     memset( gCmUsr[ uIx ]->pDmState,
                             0,
                             CMDM_ACTION_BUFFER_SIZE * sizeof(BOS_UINT8) );
                     memset( gCmUsr[ uIx ]->pDmDigMap,
                             0,
                             CMDM_MAP_BUFFER_SIZE * sizeof(BOS_UINT8) );

                     eDmStatus = cmDmCompile (
                                      gCmUsr[ uIx ]->uHdl,
                                      pUsrDigMap->pABNF,
                                      (CMDMDIGITMAP *) gCmUsr[ uIx ]->pDmDigMap,
                                      CMDM_MAP_BUFFER_SIZE );

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                           &cfgIdx,
                                           (void *) pUsrDigMap,
                                           CFG_ITEM_DEL );
                     pUsrDigMap = NULL;
                  }
                  else
                  {
                     CMGRLOG_CRITICAL ((
                             "cmUsrReConfig: %s - failed to get digit map!",
                             gCmUsr[ uIx ]->uUid ));
                  }

                  CMGRLOG_INFORMATION ((
                          "cmUsrReConfig: %s - Local reconfiguration (dm %d)",
                          gCmUsr[ uIx ]->uUid, eDmStatus ));
                  gCmUsr[ uIx ]->eCfgAction = eCMCFGACT_NOACTION;
               }
            }
            break;

            /* This reconfiguration requires the user to impact its network status.
            ** This means we are going to have to de-register if we were registered
            ** and additionally we need to ensure there is no active calls because
            ** those could be affected.  Note that we would prevent the user to
            ** create new outgoing or answer new incoming calls while under this
            ** condition to speed up the process.
            */
            case eCMCFGACT_CFGUSRWITHNET:
            {
               /* Terminate user activation timer if it is running since
               ** we want to reconfigure the user now.
               */
               memset ( &tmr, 0, sizeof ( CMTMR ) );
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = gCmUsr[ uIx ]->uHdl;
               tmr.eType = eCMEVT_TMR_INIT_REG_DELAY;
               if ( cmTmrIsRunning ( &tmr ) )
               {
                  cmTmrStop ( &tmr );
               }

               /* Set this flag to know that we do not want to
               ** apply any delay to re-register following the
               ** de-registration associated with this user
               ** reconfiguration.
               */
               gCmUsr[ uIx ]->bRegNoDelay = BOS_TRUE;
            }
            /* Fall through...
            */
            case eCMCFGACT_CFGUSRTERM:
            {
               BOS_BOOL bActive = BOS_FALSE;

               /* Terminate user activation timer if it is running since
               ** we want to reconfigure the user now.
               */
               memset ( &tmr, 0, sizeof ( CMTMR ) );
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = gCmUsr[ uIx ]->uHdl;
               tmr.eType = eCMEVT_TMR_INIT_REG_DELAY;
               if ( cmTmrIsRunning ( &tmr ) )
               {
                  cmTmrStop ( &tmr );
               }

               /* If there are no active calls on this user.
               */
               if ( ( cmUsrActiveSes ( gCmUsr[ uIx ] ) != NULL ) ||
                    ( cmUsrFocusSes ( gCmUsr[ uIx ] ) != NULL ) )
               {
                  bActive = BOS_TRUE;
               }

               if ( !bActive )
               {
                  /* Need to de-register first.
                  */
                  if ( gCmUsr[ uIx ]->bInSrv &&
                       !gCmUsr[ uIx ]->bBeRem )
                  {
                     cctkUserUnregister ( cmUsrGetNtwkHdl( gCmUsr[ uIx ] ), BOS_TRUE );

                     if ( gCmCfgCb.cmgrEvtCb )
                     {
                        /* Inform host that unregistration is in progress */
                        ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_PROGRESS,
                                                 gCmUsr[ uIx ]->uHdl,
#if CMGRCFG_MULTILINE
                                                 gCmUsr[ uIx ]->uHdl,
                                                 cmCctkAccntFindByNet( cmUsrGetNtwkHdl( gCmUsr[ uIx ]) )->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                                 (void *) eCMGREVT_OPER_NOK );
                     }
                  }
                  /* Go ahead with the deletion if not already asked for it
                  ** (deletion may be delayed due to CCTK internal asynch
                  ** process).
                  */
                  else if ( !gCmUsr[ uIx ]->bBeRem )
                  {
                     gCmUsr[ uIx ]->bBeRem = BOS_TRUE;
                     cctkUserRemove ( cmUsrGetNtwkHdl( gCmUsr[ uIx ] ) );
                  }
               }
            }
            break;

            /* This is a trigger for a user 're-add' which happens when a user
            ** addition failed due to a valid possible race condition on CCTK
            ** resources and we now need to attempt adding the user again.
            */
            case eCMCFGACT_CFGUSRREADD:
            {
               gCmUsr[ uIx ]->eCfgAction = eCMCFGACT_NOACTION;

               cmCfgUserAdd ( (void *) gCmUsr[ uIx ] );

               CMGRLOG_INFORMATION (( "cmUsrReConfig: user %s is re-added",
                                      gCmUsr[ uIx ]->uUid ));
            }
            break;

            /* Do nothing.
            */
            default:
            break;
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmUsr3WayCall
**
** PURPOSE:    Checks whether the user is engaged in a three way conference call
**             or not.  The 3-way conference call would be locally setup by the
**             user.
**
** PARAMETERS: pUsr - the user of interest.
**
** RETURNS:    BOS_TRUE - if the user is ongoing a 3-way call.
**             BOS_FALSE - otherwise.
***********************************************************************************/
BOS_BOOL cmUsr3WayCall ( CMUSR *pUsr )
{
   CMSES *pSes = cmUsrFocusSes( pUsr );

   if ( pSes )
   {
      if ( pSes->ses.med.uConf != CMGR_INVALID_HANDLE )
      {
         CMSES *pConfSes = NULL;

         if ( ( pConfSes = cmSesValid ( pSes->ses.med.uConf ) ) != NULL )
         {
            if ( ( pConfSes->ses.med.uConf == pSes->uHdl ) &&
                 ( pSes->ses.med.uConf == pConfSes->uHdl ) )
            {
               CMGRLOG_INFORMATION ((
                       "cmUsr3WayCall: 0x%x - Conference 0x%x and 0x%x",
                       pUsr->uHdl, pSes->uHdl, pConfSes->uHdl ));

               return BOS_TRUE;
            }
         }
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
** FUNCTION:   cmUsrEmergCall
**
** PURPOSE:    Checks whether the user is engaged in an emergency conference call
**             or not.
**
** PARAMETERS: pUsr - the user of interest.
**
** RETURNS:    BOS_TRUE - if the user is ongoing an emergency call.
**             BOS_FALSE - otherwise.
***********************************************************************************/
BOS_BOOL cmUsrEmergCall ( CMUSR *pUsr )
{
   CMSES *pSes = cmUsrFocusSes( pUsr );

   if ( pSes && pSes->ses.med.bEmergency )
   {
      return BOS_TRUE;
   }

   return BOS_FALSE;
}


/***********************************************************************************
** FUNCTION:   cmUsrDtLnk
**
** PURPOSE:    Sets the data link information associated with a user.
**
** PARAMETERS: bDtLnk - Whether data link is enabled (BOS_TRUE) or not
**                      (BOS_FALSE) when this call is invoked.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrDtLnk ( BOS_BOOL bDtLnk )
{
   BOS_UINT32 uIx;

   CMGRLOG_INFORMATION (( "cmUsrDtLnk: users notified of %s data link",
                          bDtLnk ? "UP" : "DOWN" ));

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] && ( gCmUsr[ uIx ]->bDtLnk != bDtLnk ) )
      {
         gCmUsr[ uIx ]->bDtLnk = bDtLnk;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmUsrIdle
**
** PURPOSE:    Check for all users that should be idled.
**
** PARAMETERS: .
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrIdle ( void )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] )
      {
         /* If there are no other session associated with this user, clean
         ** up completely.
         */
         if ( (cmUsrFocusSes ( gCmUsr[ uIx ] ) == NULL) &&
              (cmUsrActiveSes ( gCmUsr[ uIx ] ) == NULL) )
         {
            CMGRLOG_INFORMATION (( "cmUsrIdle: idling \'%s\' (0x%lX)",
                                   gCmUsr[ uIx ]->uUid, gCmUsr[ uIx ]->uHdl ));

            cmFsmOnHook ( (void *) gCmUsr[ uIx ] );
            cmUsrNewState ( gCmUsr[ uIx ], eCMST_IDLE );
         }
      }
   }
}


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
void cmUsrLinkLocalUser ( CMUSR *pCurUsr, BOS_UINT32 uLocalUsrHdl )
{
   BOS_ASSERT ( pCurUsr != NULL );
   pCurUsr->uLinkedLocalUserHdl = uLocalUsrHdl;
}

/***********************************************************************************
** FUNCTION:   cmUsrLinkRemoteUser
**
** PURPOSE:    Link a remote user to this user.
**
** PARAMETERS: pCurUsr - the user of interest.
**             uRemoteUserHdl - the handle of the remote user to be linked with
**             the current user.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrLinkRemoteUser ( CMUSR *pCurUsr, BOS_UINT32 uRemoteUserHdl )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uFreeIx = CMGR_INVALID_HANDLE;
   BOS_ASSERT ( pCurUsr != NULL );

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( pCurUsr->uLinkedRemoteUserHdl[uIx] == uRemoteUserHdl )
      {
         /* already linked */
         return;
      }
      if ( uFreeIx == CMGR_INVALID_HANDLE && pCurUsr->uLinkedRemoteUserHdl[uIx] == CMGR_INVALID_HANDLE )
      {
         uFreeIx = uIx;
      }
   }
   pCurUsr->uLinkedRemoteUserHdl[uFreeIx] = uRemoteUserHdl;
   pCurUsr->uLinkedRemoteUserCnt ++;
}

/***********************************************************************************
** FUNCTION:   cmUsrUnlinkRemoteUser
**
** PURPOSE:    Unlink a remote user from this user.
**
** PARAMETERS: pCurUsr - the user of interest.
**             uRemoteUsrHdl - the handle of the remote user to be unlinked with
**             the current user.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrUnlinkRemoteUser ( CMUSR *pCurUsr, BOS_UINT32 uRemoteUsrHdl )
{
   BOS_UINT32 uIx;
   BOS_ASSERT ( pCurUsr != NULL );
   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( pCurUsr->uLinkedRemoteUserHdl[uIx] == uRemoteUsrHdl )
      {
         pCurUsr->uLinkedRemoteUserHdl[uIx] = CMGR_INVALID_HANDLE;
         pCurUsr->uLinkedRemoteUserCnt --;
         return;
      }
   }
}

#if CMGRCFG_REMOTE_USER_CONTROL
/***********************************************************************************
** FUNCTION:   cmUsrFirstLocalUser
**
** PURPOSE:    Gets the first local user created on the device.
**
** PARAMETERS: None.
**
** RETURNS:    Pointer to the allocated user on success, NULL otherwise.
***********************************************************************************/
CMUSR * cmUsrFirstLocalUser ( void )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] && ( gCmUsr[ uIx ]->eType != eCMUSRTYPE_NONE) &&
           !cmUsrIsRemote(uIx) )
      {
         return gCmUsr[ uIx ];
      }
   }
   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmUsrIsRemote
**
** PURPOSE:    Checks whether the user referenced by the index is a remote one
**             or not.
**
** PARAMETERS: uIx - the remote user index.
**
** RETURNS:    Pointer to the user on success, NULL otherwise.
***********************************************************************************/
CMUSR * cmUsrIsRemote ( BOS_UINT32 uIx )
{
   return ( (gCmUsr[ uIx ]->eType == eCMUSRTYPE_REMOTE) ? gCmUsr[ uIx ] : NULL );
}


/***********************************************************************************
** FUNCTION:   cmUsrBgSes
**
** PURPOSE:    Determines the background session for this user.
**
** PARAMETERS: pUsr - the user of interest.
**
** RETURNS:    The background session for this user, or NULL if not applicable.
***********************************************************************************/
CMSES * cmUsrBgSes ( CMUSR *pUsr )
{
   BOS_UINT32 uIx;

   BOS_ASSERT ( pUsr != NULL );

   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] &&
           pUsr->pMedSes [ uIx ]->ses.med.bBgFocus &&
           !pUsr->pMedSes [ uIx ]->ses.med.bFocus )
      {
         return pUsr->pMedSes [ uIx ];
      }
   }

   return NULL;
}
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

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
void cmUsrGetAll ( CMUSRLIST* pUsrList, CMUSR *pExcludeUsr )
{
   BOS_UINT32 uIx;

   BOS_ASSERT ( pUsrList != NULL );

   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( gCmUsr[ uIx ] &&
           ( gCmUsr[ uIx ] != pExcludeUsr ) )
      {
         pUsrList->uUsr[ pUsrList->uLength ] = gCmUsr[ uIx ]->uHdl;
         pUsrList->uLength++;
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmUsrGetNtwkHdl
**
** PURPOSE:    Returns the available CCTK network handle for this user.
**
** PARAMETERS: pUsr - the user of interest.
**
** RETURNS:    Returns the CCTK network handle for this user..
***********************************************************************************/
BOS_UINT32 cmUsrGetNtwkHdl ( CMUSR *pUsr )
{
   BOS_UINT32 handle = CMGR_INVALID_HANDLE;

#if CMGRCFG_MULTILINE
   handle = getUsrCctkHdlFromRouting( pUsr, BOS_FALSE );
#else
   handle = pUsr->uNet;
#endif

   return handle;
}

/***********************************************************************************
** FUNCTION:   cmUsrGetCurrNtwkHdl
**
** PURPOSE:    Returns the current CCTK network handle for this user.
**
** PARAMETERS: pUsr - the user of interest.
**
** RETURNS:    Returns the CCTK network handle for this user..
***********************************************************************************/
BOS_UINT32 cmUsrGetCurrNtwkHdl ( CMUSR *pUsr )
{
   BOS_UINT32 handle = CMGR_INVALID_HANDLE;

#if CMGRCFG_MULTILINE
   handle = getUsrCctkHdlFromRouting( pUsr, BOS_TRUE );
#else
   handle = pUsr->uNet;
#endif

   return handle;
}


/***********************************************************************************
** FUNCTION:   cmUsrSetNtwkHdl
**
** PURPOSE:    Sets the CCTK network handle for this user.
**
** PARAMETERS: pUsr - Pointer to user
**             handle - CCTK network handle for this user.
**
** RETURNS:    Nothing
***********************************************************************************/
void  cmUsrSetNtwkHdl ( CMUSR *pUsr, BOS_UINT32 handle )
{
   pUsr->uNet = handle;
}

/***********************************************************************************
** FUNCTION:   cmUsrCctkAccntAssociate
**
** PURPOSE:    Associates a CCTK account for a CM user.  This assigns the uUid and
**             the uUindex for the CM user.
**
** PARAMETERS: pUsr - the user of interest for the account association
**
** RETURNS:    TRUE if an account was associated properly.  FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmUsrCctkAccntAssociate ( CMUSR *pUsr )
{
#if CMGRCFG_MULTILINE
   CMCCTKACCNT   *pCctkAccnt = NULL;
   BOS_UINT32     strLen;
   BOS_UINT32     uCctkHdl;

   /* Retrieve the first available account for this user */
   uCctkHdl = getUsrCctkHdlFromRouting( pUsr, BOS_FALSE );
   if ( uCctkHdl != CMGR_INVALID_HANDLE )
   {
      pCctkAccnt = cmCctkAccntFindByNet( uCctkHdl );
      if ( pCctkAccnt )
      {
         cmUsrUpdateCctkAccnt( pUsr, pCctkAccnt->uUid, pCctkAccnt->uHdl );
         return ( BOS_TRUE );
      }
   }

   return ( BOS_FALSE );
#else
   return ( BOS_TRUE );
#endif
}

/***********************************************************************************
** FUNCTION:   cmUsrCctkAccntDisassociate
**
** PURPOSE:    Disssociates a CCTK account for a user.  This clears the uUid and
**             the uUindex for the CM user.
**
** PARAMETERS: pUsr - the user of interest for the account disassociation
**
** RETURNS:    none
***********************************************************************************/
void cmUsrCctkAccntDisassociate ( CMUSR *pUsr )
{
#if CMGRCFG_MULTILINE
   memset ( (void *) pUsr->uUid,
            0,
            CMGR_USER_IDENTIFIER_LEN + 1 );
   pUsr->uUindex = CMGR_INVALID_HANDLE;
#endif
}

/***********************************************************************************
** FUNCTION:   cmUsrUpdateCctkAccnt
**
** PURPOSE:    Updates the CCTK account info for the user
**
** PARAMETERS: pUsr    - the user of interest for the account update
**             puUid   - new user ID to update to
**             uUindex - new user index to update to
**
** RETURNS:    none
***********************************************************************************/
void cmUsrUpdateCctkAccnt( CMUSR *pUsr, BOS_UINT8 *puUid, BOS_UINT32 uUindex )
{
#if CMGRCFG_MULTILINE
   BOS_UINT32 strLen;

   if ( pUsr && puUid )
   {
      strLen =
         ( ( strlen ( (char *)puUid ) > CMGR_USER_IDENTIFIER_LEN ) ?
              CMGR_USER_IDENTIFIER_LEN : strlen ( (char *)puUid ) );
      strncpy ( (char *)pUsr->uUid, (char *)puUid, strLen );
      pUsr->uUindex = uUindex;
   }
#endif
}
