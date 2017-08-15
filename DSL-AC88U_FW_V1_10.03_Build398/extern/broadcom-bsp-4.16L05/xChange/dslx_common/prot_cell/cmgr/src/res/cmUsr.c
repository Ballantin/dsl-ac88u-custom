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
   if ( pUsr )
   {
      /* Disassociation is only required for non-multi-line applications since
      ** for multiline, there's always a 1-1 relationship between the CM user and
      ** a phy.
      */
      pUsr->uPhys = CMGR_INVALID_HANDLE;
   }
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
   handle = pUsr->uNet;

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
   handle = pUsr->uNet;

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
CMUSR* cmUsrFindCid ( BOS_UINT32 uNetCid, BOS_BOOL bEstabSesOnly )
{
   CMUSR* pUsr = NULL;
   BOS_UINT32 uIx;
   CMSESLIST pList;

   memset( &pList, 0, sizeof( CMSESLIST ) );

#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_INFORMATION (( "%s: cid [%X] - %s", __FUNCTION__, uNetCid,
                           bEstabSesOnly? "established-only" : "unrestricted" ));
#endif
   if( uNetCid == CMGR_INVALID_HANDLE )
   {
      return NULL;
   }

   cmSesListWithCid( uNetCid, &pList );
   for( uIx = 0 ; uIx < pList.uLength ; uIx++ )
   {
      CMSES* pSes = cmSesValid( pList.uSes[ uIx ] );
      if( ( pSes ) && ( pSes->uNet == uNetCid ) &&
            (!bEstabSesOnly || ( bEstabSesOnly && ( pSes->bEstab ) )))
      {
         pUsr = cmUsrValid( pSes->uUsr );
         CMGRLOG_DEBUG (( "Found user [%X]", pUsr->uHdl ));
         return pUsr;
      }
   }

   CMGRLOG_WARNING (( "Unable to find user" ));
   return NULL;
}