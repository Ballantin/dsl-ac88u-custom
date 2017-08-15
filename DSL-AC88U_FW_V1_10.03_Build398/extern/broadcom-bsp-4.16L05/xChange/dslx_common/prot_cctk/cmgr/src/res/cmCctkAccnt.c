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
**      This module implements CCTK Account mapping from within the Call Manager.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CMCFG_ROUTING_DEBUG           0

/* ---- Private Variables ------------------------------------------------------- */

CMCCTKACCNT gCmCctkAccnt [ CMGRCFG_MAX_CCTK ];
CMACCNTASSOCIATION gCmAccntAssociation [ CMGRCFG_MAX_ACCNT_ASSOCIATIONS ];
CMACCNTFORCED gCmAccntForced [ CMGRCFG_MAX_ACCNT_FORCED ];

/* ---- Functions --------------------------------------------------------------- */

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
void cmCctkAccnt ( BOS_BOOL bCreate )
{
   BOS_UINT32 uIx;

   if ( bCreate )
   {
      CMGRLOG_INFORMATION (( "Creating CCTK accnt mapping" ));
   }
   else
   {
      CMGRLOG_INFORMATION (( "Terminating CCTK accnt mapping" ));
   }

   /* Both creating and destroying do the same thing. Simply wipe the table. */
   for ( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {
      gCmAccntAssociation [ uIx ].uUsr = CMGR_INVALID_HANDLE;
      gCmAccntAssociation [ uIx ].uNet = CMGR_INVALID_HANDLE;
   }

   for ( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_FORCED ; uIx++ )
   {
      gCmAccntForced [ uIx ].uNet = CMGR_INVALID_HANDLE;
      gCmAccntForced [ uIx ].uUsr = CMGR_INVALID_HANDLE;
   }

   for ( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      /* If digit map exists, free it */
      if ( gCmCctkAccnt [ uIx ].pDmDigMap )
      {
         free( gCmCctkAccnt [ uIx ].pDmDigMap );
         gCmCctkAccnt [ uIx ].pDmDigMap = NULL;
      }

      memset( &gCmCctkAccnt [ uIx ], 0, sizeof ( CMCCTKACCNT ) );
      gCmCctkAccnt [ uIx ].uNet = CMGR_INVALID_HANDLE;
      gCmCctkAccnt [ uIx ].uHdl = CMGR_INVALID_HANDLE;
      gCmCctkAccnt [ uIx ].bInSrv = BOS_FALSE;
      if( bCreate )
      {
         gCmCctkAccnt [ uIx ].pDmDigMap = (BOS_UINT8 *) malloc( CMDM_MAP_BUFFER_SIZE * sizeof(BOS_UINT8) );
         memset( gCmCctkAccnt [ uIx ].pDmDigMap, 0, CMDM_MAP_BUFFER_SIZE * sizeof(BOS_UINT8) );
      }
      gCmCctkAccnt [ uIx ].eCfgAction = eCMCFGACT_NOACTION;
   }
}

/***********************************************************************************
** FUNCTION:   cmCctkAccntRegister
**
** PURPOSE:    Registers the CCTK user associtate with a CCTK Account resources.
**
** PARAMETERS: pCctkAccnt - Pointer to a valid CMCCTKACCNT object
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntRegister ( CMCCTKACCNT* pCctkAccnt )
{
   CFGSIPPCSCF *pPcscfUsr = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bImplicitReg = BOS_FALSE;
   CFGSIPUSERCFG *pCctkUsrCfg = NULL;

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pCctkAccnt->uUid;

   /* Get the configuration associated with this CCTK account */
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) &pCctkUsrCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pCctkUsrCfg && pCctkUsrCfg->Act.bCfgStatus )
      {
         cfgIdx.pcCharIdx = pCctkUsrCfg->pcActiveDomain;
         cfgIdx.uIndex = pCctkUsrCfg->uActiveDomain;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                    &cfgIdx,
                                    (void *) &pPcscfUsr,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            /* Check if valid ip-addr/domains are set */
            if ( ( pPcscfUsr->pcPCSCFDomain && strlen( (char*)pPcscfUsr->pcPCSCFDomain ) ) ||
                  !bosIpAddrIsZero ( &pPcscfUsr->PCSCFAddr ) )
            {
               BOS_UINT8 uIpAddr[ CMGR_IP_ADDRESS_STRING_LEN + 1];
               bosIpAddrGetStr ( &pPcscfUsr->PCSCFAddr,
                                 (char *) uIpAddr,
                                 CMGR_IP_ADDRESS_STRING_LEN );

               CMGRLOG_INFORMATION (( "Starting registration for net 0x%X %s %s ",
                                       pCctkAccnt->uNet, (char *) uIpAddr, pPcscfUsr->pcPCSCFDomain  ));

               cctkUserRegister ( pCctkAccnt->uNet );

               if ( gCmCfgCb.cmgrEvtCb )
               {
                  /* Inform host that registration is in progress */
                  ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_PROGRESS,
                                           pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                                           CMGR_INVALID_HANDLE,
                                           pCctkAccnt->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                           (void *) eCMGREVT_OPER_OK );
               }

            }
            else
            {
               /* Peer-to-Peer mode */
               CMGRLOG_INFORMATION (( "Implicit registration for net 0x%X", pCctkAccnt->uNet ));
               bImplicitReg = BOS_TRUE;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                  &cfgIdx,
                                  (void *) pPcscfUsr,
                                  CFG_ITEM_DEL );
            pPcscfUsr = NULL;

            if( bImplicitReg )
            {
               /* Put the CCTK user into service right away */
               pCctkAccnt->bInSrv = BOS_TRUE;

               if ( gCmCfgCb.cmgrEvtCb )
               {
                  ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_OK,
                                           pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                                           CMGR_INVALID_HANDLE,
                                           pCctkAccnt->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                           (void *) pCctkAccnt->uUid );
               }
            }
         }
      }
      else
      {
         /* CCTK Account is not enabled */
        CMGRLOG_INFORMATION (( "Uuid %s not enabled", pCctkAccnt->uUid ));
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                            &cfgIdx,
                            (void *) pCctkUsrCfg,
                            CFG_ITEM_DEL );
      pCctkUsrCfg = NULL;
   }
}

/***********************************************************************************
** FUNCTION:   cmCctkAccntShow
**
** PURPOSE:    Dumps the CCTK Account resources.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntShow ( void )
{
   BOS_UINT32 uIx;

   CMGRLOG_INFORMATION(( "Account associations table:" ));
   CMGRLOG_INFORMATION(( "|   USR    |   NET    |" ));
   CMGRLOG_INFORMATION(( "+----------+----------+" ));
   /* Dump the account associations table */
   for ( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {
      if( gCmAccntAssociation [ uIx ].uUsr != CMGR_INVALID_HANDLE &&
          gCmAccntAssociation [ uIx ].uNet != CMGR_INVALID_HANDLE )
      {
         CMGRLOG_INFORMATION (( "| %-8X | %-8X |", gCmAccntAssociation [ uIx ].uUsr, gCmAccntAssociation [ uIx ].uNet ));
      }
   }
   CMGRLOG_INFORMATION(( "+----------+----------+\n" ));

   CMGRLOG_INFORMATION(( "Account forced table:" ));
   CMGRLOG_INFORMATION(( "|   USR    |   NET    |" ));
   CMGRLOG_INFORMATION(( "+----------+----------+" ));
   for ( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_FORCED ; uIx++ )
   {
      if( gCmAccntForced [ uIx ].uUsr != CMGR_INVALID_HANDLE &&
          gCmAccntForced [ uIx ].uNet != CMGR_INVALID_HANDLE )
      {
         CMGRLOG_INFORMATION (( "| %-8X | %-8X |", gCmAccntForced [ uIx ].uUsr, gCmAccntForced [ uIx ].uNet ));
      }
   }
   CMGRLOG_INFORMATION(( "+----------+----------+\n" ));

   CMGRLOG_INFORMATION(( "Account list:" ));
   CMGRLOG_INFORMATION(( "|   NET    |           ID           |  #Calls  |  INSRV  |" ));
   CMGRLOG_INFORMATION(( "+----------+------------------------+----------+---------+" ));
   for ( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      if( gCmCctkAccnt [ uIx ].uNet != CMGR_INVALID_HANDLE )
      {
         CMGRLOG_INFORMATION(( "| %-8X | %-22s | %-8lu | %-8d |",
                               gCmCctkAccnt [ uIx ].uNet,
                               (char*)gCmCctkAccnt [ uIx ].uUid,
                               gCmCctkAccnt [ uIx ].uNumActiveCalls,
                               gCmCctkAccnt [ uIx ].bInSrv ));
      }
   }
   CMGRLOG_INFORMATION(( "+----------+------------------------+----------+---------+" ));
}


/***********************************************************************************
** FUNCTION:   cmNumCidAssigned
**
** PURPOSE:    Finds whether this call id has any assigned CM user.
**
** PARAMETERS: bCreate - Whether resources should be initialized (BOS_TRUE) or ended
**                       (BOS_FALSE) when this call is invoked.
**
** RETURNS:    Number of users assigned to this call id.
***********************************************************************************/
BOS_UINT32 cmNumCidAssigned ( BOS_UINT32 uCid )
{
   BOS_UINT32 uIx;
   BOS_UINT32 count = 0;
   CMSESLIST pList;

   memset( &pList, 0, sizeof( CMSESLIST ) );

#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_INFORMATION (( "%s: cid [%X]", __FUNCTION__, uCid ));
#endif
   if( uCid == CMGR_INVALID_HANDLE )
   {
      return 0;
   }

   cmSesListWithCid( uCid, &pList );
   for( uIx = 0 ; uIx < pList.uLength ; uIx++ )
   {
      CMSES* pSes = cmSesValid( pList.uSes[ uIx ] );
      if( ( pSes ) && ( pSes->uNet == uCid ) && ( pSes->bEstab ) )
      {
         count++;
      }
   }

   CMGRLOG_INFORMATION (( "%x assigned sessions with cid [%X]", count, uCid ));
   return count;
}


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
void cmGetAssocUsrList( BOS_UINT32 uNet, CMUSRLIST* pList )
{
   BOS_UINT32 uIx, uIy;

#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_INFORMATION (( "%s: uNet [%X]", __FUNCTION__, uNet ));
#endif

   if( ( uNet == CMGR_INVALID_HANDLE ) || ( pList == NULL ) )
   {
      return;
   }

   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {
      if( gCmAccntAssociation[ uIx ].uNet == uNet )
      {
         BOS_BOOL bUsrExists = BOS_FALSE;

         /* Iterate through the list to make sure we haven't already added this user */
         for( uIy = 0 ; uIy < pList->uLength ; uIy++ )
         {
            if( gCmAccntAssociation[ uIx ].uUsr == pList->uUsr[ uIy ] )
            {
               bUsrExists = BOS_TRUE;
               break;
            }
         }

         /* If we didn't find the user, add it to the list */
         if( bUsrExists == BOS_FALSE )
         {
            pList->uUsr[ pList->uLength ] = gCmAccntAssociation[ uIx ].uUsr;
            pList->uLength++;
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmGetAssocActiveAccnt
**
** PURPOSE:    Finds an associated account which has an active call.
**
** PARAMETERS: pUsr(IN)  - the CMGR user to look up
**
** RETURNS:    CCTK user handle on success, CMGR_INVALID_HANDLE otherwise..
***********************************************************************************/
BOS_UINT32 cmGetAssocActiveAccnt( CMUSR* pUsr )
{
   BOS_UINT32 uIx;

#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_INFORMATION (( "%s: uHdl [%X]", __FUNCTION__, pUsr? pUsr->uHdl : CMGR_INVALID_HANDLE ));
#endif

   if( pUsr == NULL )
   {
      return CMGR_INVALID_HANDLE;
   }

   /* Find the first associated account that has an active call.
   */
   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {

      if ( gCmAccntAssociation[ uIx ].uUsr == pUsr->uHdl )
      {
         if ( cmCctkNumActiveCalls( gCmAccntAssociation[ uIx ].uNet ) )
         {
            return gCmAccntAssociation[ uIx ].uNet;
         }
      }
   }

   /* Couldn't find an active associated account
   */
   return CMGR_INVALID_HANDLE;
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


/***********************************************************************************
** FUNCTION:   cmCctkAccntAlloc
**
** PURPOSE:    Allocates a new CCTK user in the user configuration block.
**
** PARAMETERS: None.
**
** RETURNS:    Pointer to the newly created CCTK Account on success, NULL otherwise.
***********************************************************************************/
CMCCTKACCNT* cmCctkAccntAlloc( void )
{
   BOS_UINT32 uIx;
#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_INFORMATION (( "%s: Enter", __FUNCTION__ ));
#endif
   for( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      if( gCmCctkAccnt [ uIx ].uNet == CMGR_INVALID_HANDLE )
      {
         return &( gCmCctkAccnt [ uIx ] );
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmCctkAccntFree
**
** PURPOSE:    Free the specified CCTK account in the user configuration block.
**
** PARAMETERS: Pointer to the CCTK account to free
**
** RETURNS:    none
***********************************************************************************/
void cmCctkAccntFree( CMCCTKACCNT* pCctkAccnt )
{
   BOS_UINT32 uIx;

   if ( pCctkAccnt )
   {
      CMGRLOG_INFORMATION (( "cmCctkAccntFree: net 0x%X, hdl 0x%X",
                                 pCctkAccnt->uNet, pCctkAccnt->uHdl ));

      /* Clear any associations this account may have
      */
      cmCctkAccntDeinit( pCctkAccnt );

      /* Finally, clear CCTK account information */
      pCctkAccnt->uHdl = CMGR_INVALID_HANDLE;
      pCctkAccnt->uNet = CMGR_INVALID_HANDLE;

      memset( pCctkAccnt->uUid, 0, CMGR_USER_IDENTIFIER_LEN + 1 );
      memset( pCctkAccnt->pcDefDialNum, 0, CMGR_USER_IDENTIFIER_LEN + 1 );
      pCctkAccnt->bInSrv = BOS_FALSE;
      pCctkAccnt->eCfgAction = eCMCFGACT_NOACTION;
      pCctkAccnt->uNumActiveCalls = 0;
      memset( &pCctkAccnt->evt, 0, sizeof(CMEVT) );
   }
}

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
void cmCctkAccntInit( CMCCTKACCNT* pCctkAccnt )
{
   CFGSIPUSERCFG *pCctkAccntCfg = NULL;
   PROV_CFG_IDX   cfgIdx;
   BOS_UINT32     uIx = 0;

   if ( pCctkAccnt )
   {
      cfgIdx.pcCharIdx = pCctkAccnt->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                 &cfgIdx,
                                 (void *) &pCctkAccntCfg,
                                 CFG_ITEM_GET )== BOS_STATUS_OK )
      {
         if ( pCctkAccntCfg )
         {
            CFGSIPUSERENDPNTMAP *pEpt = pCctkAccntCfg->pEndPntMap;
            while ( pEpt && uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS )
            {
               if ( (pEpt->bIn || pEpt->bOut) )
               {
                  /* Add the endpoint to the routing table if there's room left */
                  while ( uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS )
                  {
                     if( gCmAccntAssociation [ uIx ].uUsr == CMGR_INVALID_HANDLE &&
                         gCmAccntAssociation [ uIx ].uNet == CMGR_INVALID_HANDLE )
                     {
                        CMGRLOG_INFORMATION (( "cmCctkAccntInit: Associated account net 0x%X user 0x%X",
                                       pCctkAccnt->uNet, pEpt->sEndpntNum ));
                        gCmAccntAssociation[ uIx ].uNet = pCctkAccnt->uNet;
                        gCmAccntAssociation[ uIx ].uUsr = pEpt->sEndpntNum;
                        break;
                     }
                     else
                     {
                        uIx++;
                     }
                  }
               }
               /* Increment the interface.
               */
               pEpt = pEpt->pNext;
            };

            if ( uIx == CMGRCFG_MAX_ACCNT_ASSOCIATIONS )
            {
               CMGRLOG_INFORMATION (( "No room in accnt association table for user %X with net %X",
                                       pEpt->sEndpntNum, pCctkAccnt->uNet ));
            }
         }

         /* Free the resources allocated.
         */
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pCctkAccntCfg,
                               CFG_ITEM_DEL );
         pCctkAccntCfg = NULL;
      }
   }
}

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
void cmCctkAccntDeinit( CMCCTKACCNT* pCctkAccnt )
{
   BOS_UINT32 uIx;

   if ( pCctkAccnt )
   {
      /* Clear any account entries from the Account association table.
      */
      for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
      {
         if( gCmAccntAssociation[ uIx ].uNet == pCctkAccnt->uNet )
         {
            CMGRLOG_DEBUG (( "cmCctkAccntDeinit: Removing assoc. account net 0x%X user 0x%X",
                             gCmAccntAssociation[ uIx ].uNet, gCmAccntAssociation[ uIx ].uUsr ));

            gCmAccntAssociation[ uIx ].uNet = CMGR_INVALID_HANDLE;
            gCmAccntAssociation[ uIx ].uUsr = CMGR_INVALID_HANDLE;
         }
      }

      /* Next, clear the account entry in the force line table if it had an entry
      ** there.
      */
      for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_FORCED ; uIx++ )
      {
         if( gCmAccntForced[ uIx ].uNet == pCctkAccnt->uNet )
         {
            CMGRLOG_DEBUG (( "cmCctkAccntDeinit: Removing forced account net 0x%X user 0x%X",
                             gCmAccntForced[ uIx ].uNet, gCmAccntForced[ uIx ].uUsr ));

            gCmAccntForced [ uIx ].uNet = CMGR_INVALID_HANDLE;
            gCmAccntForced [ uIx ].uUsr = CMGR_INVALID_HANDLE;
         }
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmCctkAccntFindByNet
**
** PURPOSE:    Gets a CCTK Account by its network handle.
**
** PARAMETERS: uNet -  The CCTK User handle.
**
** RETURNS:    Pointer to the CMCCTKACCNT if found, NULL otherwise.
***********************************************************************************/
CMCCTKACCNT* cmCctkAccntFindByNet( BOS_UINT32 uNet )
{
   BOS_UINT32 uIx;
#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_DEBUG (( "%s: net [%X]", __FUNCTION__, uNet ));
#endif
   for( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      if( gCmCctkAccnt[ uIx ].uNet == uNet )
      {
         return &( gCmCctkAccnt[ uIx ]);
      }
   }
   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmCctkAccntFindByHdl
**
** PURPOSE:    Gets a CCTK Account by its Call Manager handle.
**
** PARAMETERS: uHdl -  The user handle.
**
** RETURNS:    Pointer to the CMCCTKACCNT if found, NULL otherwise.
***********************************************************************************/
CMCCTKACCNT* cmCctkAccntFindByHdl( BOS_UINT32 uHdl )
{
   BOS_UINT32 uIx;

#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_DEBUG (( "%s: hdl [%X]", __FUNCTION__, uHdl ));
#endif
   if( uHdl == CMGR_INVALID_HANDLE )
   {
      return NULL;
   }
   for( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      if( gCmCctkAccnt[ uIx ].uHdl == uHdl )
      {
         return &( gCmCctkAccnt[ uIx ]);
      }
   }
   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmCctkAccntFindByUid
**
** PURPOSE:    Gets a CCTK Account by the Uid
**
** PARAMETERS: pUid - UId to retrieve the account for
**
** RETURNS:    Pointer to the CMCCTKACCNT if found, NULL otherwise.
***********************************************************************************/
CMCCTKACCNT* cmCctkAccntFindByUid( BOS_UINT8 *pUid )
{
   BOS_UINT32 uIx;

   if ( pUid == NULL )
   {
      return NULL;
   }

   for ( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      if( gCmCctkAccnt[ uIx ].uNet != CMGR_INVALID_HANDLE )
      {
         if ( strlen ( (const char *) gCmCctkAccnt[ uIx ].uUid ) &&
              !strcmp ( (const char *) gCmCctkAccnt[ uIx ].uUid,
                        (const char *) pUid ) )
         {
            return &gCmCctkAccnt[ uIx ];
         }
      }
   }

   return NULL;
}


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
void cmCctkAccntForce( BOS_UINT32 uNetHdl, BOS_UINT32 uUsrHdl )
{
   BOS_UINT32 uIx;
#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_DEBUG (( "%s: Net [%X], User [%X]", __FUNCTION__, uNetHdl, uUsrHdl ));
#endif
   /* First, check for any previously-defined forced user instances and replace them */
   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_FORCED ; uIx++ )
   {
      if( gCmAccntForced[ uIx ].uUsr == uUsrHdl )
      {
         gCmAccntForced[ uIx ].uNet = uNetHdl;
         gCmAccntForced[ uIx ].uUsr = uUsrHdl;
         return;
      }
   }

   /* If none were found, simply add the forced reference to the first unused spot */
   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_FORCED ; uIx++ )
   {
      if( gCmAccntForced[ uIx ].uUsr == CMGR_INVALID_HANDLE )
      {
         gCmAccntForced[ uIx ].uNet = uNetHdl;
         gCmAccntForced[ uIx ].uUsr = uUsrHdl;
         return;
      }
   }
}


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
void cmCctkAccntUnforce( BOS_UINT32 uNetHdl, BOS_UINT32 uUsrHdl )
{
   BOS_UINT32 uIx;

   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_FORCED ; uIx++ )
   {
      if( ( gCmAccntForced[ uIx ].uUsr == uUsrHdl ) &&
          ( gCmAccntForced[ uIx ].uNet == uNetHdl ) )
      {
         CMGRLOG_INFORMATION (( "%s: Removing forced entry: User [%X] -> Net [%X]", __FUNCTION__, uUsrHdl, uNetHdl ));
         gCmAccntForced[ uIx ].uNet = CMGR_INVALID_HANDLE;
         gCmAccntForced[ uIx ].uUsr = CMGR_INVALID_HANDLE;
         return;
      }
   }
}


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
void cmCctkAccntAssociate( BOS_UINT32 uNetHdl, BOS_UINT32 uUsrHdl )
{
   BOS_UINT32 uIx;

   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {
      if( gCmAccntAssociation[ uIx ].uNet == CMGR_INVALID_HANDLE )
      {
         gCmAccntAssociation[ uIx ].uNet = uNetHdl;
         gCmAccntAssociation[ uIx ].uUsr = uUsrHdl;
         CMGRLOG_INFORMATION (( "Associated account user %X with net %X\n",
                                 uUsrHdl, uNetHdl ));
         break;
      }
   }

#if (CMCFG_ROUTING_DEBUG)
   CMGRLOG_INFORMATION(( "%s: Assocations table:", __FUNCTION__ ));
   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {
      if( gCmAccntAssociation[ uIx ].uNet != CMGR_INVALID_HANDLE )
      {
         CMGRLOG_INFORMATION(( "usr [%4X]  net [%8X]", gCmAccntAssociation[ uIx ].uUsr, gCmAccntAssociation[ uIx ].uNet ));
      }
   }
#endif
}


/***********************************************************************************
** FUNCTION:   cmCctkAccntCallAdd
**
** PURPOSE:    Add a CCTK call.
**
** PARAMETERS: uNet  - CCTK user handle.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntCallAdd( BOS_UINT32 uNet )
{
   BOS_UINT32 uIx;

   CMGRLOG_DEBUG (( "%s: Net [%X]", __FUNCTION__, uNet ));
   for( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      if( gCmCctkAccnt [ uIx ].uNet == uNet )
      {
         gCmCctkAccnt [ uIx ].uNumActiveCalls++;
         CMGRLOG_INFORMATION (( "cmCctkAccntCallAdd: Net 0x%X has [%X/%X] calls",
                                uNet,
                                gCmCctkAccnt [ uIx ].uNumActiveCalls,
                                CMGRCFG_MAX_CALLS_PER_USER ));

         /* Notify the application of the active calls changing */
         if ( gCmCfgCb.cmgrEvtCb )
         {
            CMGRLINESTAT tStatus;

            tStatus.uNumActiveCalls = gCmCctkAccnt [ uIx ].uNumActiveCalls;
            tStatus.uMaxActiveCalls = CMGRCFG_MAX_CALLS_PER_USER;

            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_ACTIVECALLS,
                                     gCmCctkAccnt [ uIx ].uHdl,
#if CMGRCFG_MULTILINE
                                     CMGR_INVALID_HANDLE,
                                     gCmCctkAccnt [ uIx ].uHdl,
#endif /* CMGRCFG_MULTILINE */
                                     (void *) &tStatus );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmCctkAccntCallDelete
**
** PURPOSE:    Deletes a call from the CCTK user.
**
** PARAMETERS: uNet  - CCTK user handle
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkAccntCallDelete( BOS_UINT32 uNet )
{
   BOS_UINT32 uIx;

   for( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      if( gCmCctkAccnt [ uIx ].uNet == uNet )
      {
         if ( gCmCctkAccnt [ uIx ].uNumActiveCalls )
         {

            gCmCctkAccnt [ uIx ].uNumActiveCalls--;
            CMGRLOG_INFORMATION (( "cmCctkAccntCallDelete: Net 0x%X has [%X/%X] calls",
                                   uNet,
                                   gCmCctkAccnt [ uIx ].uNumActiveCalls,
                                   CMGRCFG_MAX_CALLS_PER_USER ));

            if ( gCmCfgCb.cmgrEvtCb )
            {
               CMGRLINESTAT tStatus;

               tStatus.uNumActiveCalls = gCmCctkAccnt [ uIx ].uNumActiveCalls;
               tStatus.uMaxActiveCalls = CMGRCFG_MAX_CALLS_PER_USER;

               ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_ACTIVECALLS,
                                        gCmCctkAccnt [ uIx ].uHdl,
#if CMGRCFG_MULTILINE
                                        CMGR_INVALID_HANDLE,
                                        gCmCctkAccnt [ uIx ].uHdl,
#endif /* CMGRCFG_MULTILINE */
                                        (void *) &tStatus );
            }
         }
         else
         {
            CMGRLOG_WARNING (( "cmCctkAccntCallDelete: Invalid - Net 0x%X has [0/%X] calls!",
                                uNet,
                                CMGRCFG_MAX_CALLS_PER_USER ));
         }
         return;
      }
   }
   CMGRLOG_WARNING (( "cmCctkAccntCallDelete: Couldn't find Net 0x%X",
                                uNet));
}


/***********************************************************************************
** FUNCTION:   cmCctkNumActiveCalls
**
** PURPOSE:    Gets the number of active VoIP calls for a CCTK user.
**
** PARAMETERS: uNet  - the CCTK user handle to use
**
** RETURNS:    numActiveCalls on success, 0 otherwise.
***********************************************************************************/
BOS_UINT32 cmCctkNumActiveCalls( BOS_UINT32 uNet )
{
   BOS_UINT32 uIx;

   if( uNet == CMGR_INVALID_HANDLE )
   {
      CMGRLOG_INFORMATION (( "cmCctkNumActiveCalls: invalid CCTK user [%X]", uNet ));
      return 0;
   }

   for( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      if( gCmCctkAccnt[ uIx].uNet == uNet )
      {
         CMGRLOG_INFORMATION (( "cmCctkNumActiveCalls: Net 0x%X has [%X/%X] calls",
                                uNet,
                                gCmCctkAccnt [ uIx ].uNumActiveCalls,
                                CMGRCFG_MAX_CALLS_PER_USER ));
         return gCmCctkAccnt[ uIx ].uNumActiveCalls;
      }
   }

   CMGRLOG_INFORMATION (( "cmCctkNumActiveCalls: Unable to find CCTK user [%X]", uNet ));
   return 0;
}


/***********************************************************************************
** FUNCTION:   cmCctkUpdateRouting
**
** PURPOSE:    Gets new CCTK user routing from provisioning.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmCctkUpdateRouting( void )
{
   CFGSIPUSERCFG *pCctkAccntCfg = NULL;
   PROV_CFG_IDX   cfgIdx;
   BOS_UINT32     uIx = 0;
   CMCCTKACCNT    *pCctkAccnt;
   BOS_UINT32     uAccntAssocIndex = 0;

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );

   /* Clean the old account association and forced account tables first */
   for ( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {
      gCmAccntAssociation [ uIx ].uUsr = CMGR_INVALID_HANDLE;
      gCmAccntAssociation [ uIx ].uNet = CMGR_INVALID_HANDLE;
   }

   for ( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_FORCED ; uIx++ )
   {
      gCmAccntForced [ uIx ].uNet = CMGR_INVALID_HANDLE;
      gCmAccntForced [ uIx ].uUsr = CMGR_INVALID_HANDLE;
   }

   /* Create the routing table from scratch */
   for ( uIx = 0; uIx < CMGRCFG_MAX_CCTK; uIx++ )
   {
      pCctkAccnt = &gCmCctkAccnt[uIx];
      if ( pCctkAccnt->uHdl != CMGR_INVALID_HANDLE )
      {
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                    &cfgIdx,
                                    (void *) &pCctkAccntCfg,
                                    CFG_ITEM_GET )== BOS_STATUS_OK )
         {
            if ( pCctkAccntCfg )
            {
               CFGSIPUSERENDPNTMAP *pEpt = pCctkAccntCfg->pEndPntMap;
               while ( pEpt )
               {
                  if ( (pEpt->bIn || pEpt->bOut) )
                  {
                     /* Add the endpoint to the routing table if there's room left */
                     if ( uAccntAssocIndex < CMGRCFG_MAX_ACCNT_ASSOCIATIONS )
                     {
                        gCmAccntAssociation[ uAccntAssocIndex ].uNet = pCctkAccnt->uNet;
                        gCmAccntAssociation[ uAccntAssocIndex ].uUsr = pEpt->sEndpntNum;
                        uAccntAssocIndex++;
                     }
                     else
                     {
                        break;
                     }
                  }
                  /* Increment the interface.
                  */
                  pEpt = pEpt->pNext;
               };
            }

            /* Free the resources allocated.
            */
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                  &cfgIdx,
                                  (void *) pCctkAccntCfg,
                                  CFG_ITEM_DEL );
            pCctkAccntCfg = NULL;
         }

         if ( uAccntAssocIndex == CMGRCFG_MAX_ACCNT_ASSOCIATIONS )
         {
            CMGRLOG_INFORMATION (( "Routing table is full" ));
            break;
         }
      }
   }
}

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
BOS_UINT32 getUsrCctkHdlFromRouting( CMUSR* pUsr, BOS_BOOL bInUse )
{
   BOS_UINT32 uIx, uIy;

   if( pUsr == NULL )
   {
      return CMGR_INVALID_HANDLE;
   }

   /* Check if the user exists at all in the routing table - if not, we can
   ** assume that the terminal is disconnected (ie, a DECT handset that is not
   ** routable shouldn't print error messages when CMGR tries to ring it).
   */
   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {
      if( gCmAccntAssociation[ uIx ].uUsr == pUsr->uHdl )
      {
         break;
      }
   }
   if( uIx >= CMGRCFG_MAX_ACCNT_ASSOCIATIONS )
   {
      return CMGR_INVALID_HANDLE;
   }

   /* Try actually routing - first, look through the forced routing table */
   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_FORCED ; uIx++ )
   {
      if( gCmAccntForced[ uIx ].uUsr == pUsr->uHdl )
      {
         CMGRLOG_DEBUG (( "Routing to forced accnt net 0x%X",
                           gCmAccntForced[ uIx ].uNet ));
         return gCmAccntForced[ uIx ].uNet;
      }
   }

   /* Next, if the user already has a session with a CCTK user and that user
   ** can support another call, re-use it.
   */
   for( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if( pUsr->pMedSes [ uIx ] != NULL &&
          pUsr->pMedSes [ uIx ]->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Find the CCTK Account in the associations list */
         for ( uIy = 0 ; uIy < CMGRCFG_MAX_CCTK ; uIy++ )
         {
            if( gCmCctkAccnt[ uIy ].uNet == pUsr->pMedSes[ uIx ]->uNetUsr )
            {
               if (bInUse) /* Return if the flag is set (the handle being used has been located)*/
               {
                  return gCmCctkAccnt[ uIy ].uNet;
               }
               break;
            }
         }
         if ( uIy < CMGRCFG_MAX_CCTK )
         {
            if( ( gCmCctkAccnt[ uIy ].uNumActiveCalls < CMGRCFG_MAX_CALLS_PER_USER ) )
            {
               CMGRLOG_DEBUG (( "Routing to existing net 0x%X",
                                 pUsr->pMedSes [ uIx ]->uNetUsr ));
               return pUsr->pMedSes [ uIx ]->uNetUsr;
            }
         }
      }
   }

   /* Finally, find the first available un-used cctk user from the
   ** user association list.
   */
   for( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
   {
      if( gCmAccntAssociation[ uIx ].uUsr == pUsr->uHdl )
      {
         /* Find the CCTK Account in the associations list */
         for ( uIy = 0 ; uIy < CMGRCFG_MAX_CCTK ; uIy++ )
         {
            if( gCmCctkAccnt[ uIy ].uNet == gCmAccntAssociation[ uIx ].uNet )
            {
#if CMGRCFG_MULTILINE
               /* for multi-line, it is possible an alarm port associated with
               ** a sip account becomes active while other ports with the same
               ** account have existing calls.  If the user requesting the
               ** CCTK user handle is an alarm port, we bypass the check
               ** for active calls and always return the handle.
               */
               if (pUsr->eAlarmCfgState != CMALGALRMCFGSTATE_NONE)
               {
                  return gCmAccntAssociation[ uIx ].uNet;
               }
#endif
               break;
            }
         }
         if ( uIy < CMGRCFG_MAX_CCTK )
         {
            if( ( gCmCctkAccnt[ uIy ].uNumActiveCalls < CMGRCFG_MAX_CALLS_PER_USER ) )
            {
#if !(CMGRCFG_MULTILINE_MODE_ACTIVE_ACCNT_MULTIPLE_USR)
               /* In "Single Line", only allow this line if the number
               ** of active calls is 0. Since we previously check for pre-
               ** associated sessions, we know that this CM user does not have
               ** any ongoing sessions with this CCTK user.
               */
               if( gCmCctkAccnt[ uIy ].uNumActiveCalls == 0 )
#endif
               {
                  CMGRLOG_DEBUG (( "Routing to associated net 0x%X",
                                   gCmAccntAssociation[ uIx ].uNet ));
                  return gCmAccntAssociation[ uIx ].uNet;
               }
            }
         }
      }
   }

   CMGRLOG_CRITICAL (( "No valid CCTK account for usr [%X]", pUsr->uHdl ));
   return CMGR_INVALID_HANDLE;
}

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
void cmCctkAccntUpdateNet( CMCCTKACCNT* pCctkAccnt, BOS_UINT32 uNewNetHdl )
{
   BOS_UINT32 uIx;

   if ( pCctkAccnt )
   {
      /* Update the user association table with the new network handle */
      for ( uIx = 0 ; uIx < CMGRCFG_MAX_ACCNT_ASSOCIATIONS ; uIx++ )
      {
         if( gCmAccntAssociation[ uIx ].uNet == pCctkAccnt->uNet )
         {
            gCmAccntAssociation [ uIx ].uNet = uNewNetHdl;
         }
      }

      /* Update the network handle for the current account */
      pCctkAccnt->uNet = uNewNetHdl;
   }
}

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
void cmCctkAccntReConfig ( CMCCTKACCNT* pCctkAccnt )
{
   BOS_UINT32 uIx;
   CMTMR tmr;

   for ( uIx = 0 ; uIx < CMGRCFG_MAX_CCTK ; uIx++ )
   {
      /* Apply to all users or just the specific one.
      */
      if ( ( pCctkAccnt == NULL ) ||
           ( ( pCctkAccnt != NULL ) && ( &gCmCctkAccnt[ uIx ] == pCctkAccnt ) ) )
      {
         switch ( gCmCctkAccnt[ uIx ].eCfgAction )
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
/* TODO! - reg delay
** When the reg delay timers are linked to accounts, make sure to terminate the timer.
*/
#if 0
               /* Terminate user activation timer if it is running since
               ** we want to reconfigure the user now.
               */
               memset ( &tmr, 0, sizeof ( CMTMR ) );
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = gCmCctkAccnt[ uIx ].uHdl;
               tmr.eType = eCMEVT_TMR_INIT_REG_DELAY;
               if ( cmTmrIsRunning ( &tmr ) )
               {
                  cmTmrStop ( &tmr );
               }
#endif
/* TODO! - reg delay */

               /* TODO! - dialing check
               ** Ensure that none of the associated users are in the dialing state before applying
               ** the digitmap reconfig.
               ** if ( gCmCctkAccnt[ uIx ]->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_DIALING )
               */
               {
                  CFGSIPDIGITMAP *pUsrDigMap = NULL;
                  PROV_CFG_IDX cfgIdx;
                  CMDMSTATUS eDmStatus = eCMDM_ERROR;

                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = gCmCctkAccnt[ uIx ].uUid;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                             &cfgIdx,
                                             (void *) &pUsrDigMap,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     memset( gCmCctkAccnt[ uIx ].pDmDigMap,
                             0,
                             CMDM_MAP_BUFFER_SIZE * sizeof(BOS_UINT8) );

                     eDmStatus = cmDmCompile (
                                      gCmCctkAccnt[ uIx ].uNet,
                                      pUsrDigMap->pABNF,
                                      (CMDMDIGITMAP *) gCmCctkAccnt[ uIx ].pDmDigMap,
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
                             "cmCctkAccntReConfig: %s - failed to get digit map!",
                             gCmCctkAccnt[ uIx ].uUid ));
                  }

                  CMGRLOG_INFORMATION ((
                          "cmCctkAccntReConfig: %s - Local reconfiguration (dm %d)",
                          gCmCctkAccnt[ uIx ].uUid, eDmStatus ));
                  gCmCctkAccnt[ uIx ].eCfgAction = eCMCFGACT_NOACTION;
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
/* TODO! - reg delay
** When the reg delay timers are linked to accounts, make sure to terminate the timer
** and configure bRegNoDelay.
*/
#if 0
               /* Terminate user activation timer if it is running since
               ** we want to reconfigure the user now.
               */
               memset ( &tmr, 0, sizeof ( CMTMR ) );
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = gCmCctkAccnt[ uIx ]->uHdl;
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
               gCmCctkAccnt[ uIx ]->bRegNoDelay = BOS_TRUE;
#endif
/* TODO! - reg delay */
            }
            /* Fall through...
            */
            case eCMCFGACT_CFGUSRTERM:
            {
               BOS_BOOL bActive = BOS_FALSE;

/* TODO! - reg delay
** When the reg delay timers are linked to accounts, make sure to terminate the timer.
*/
#if 0
               /* Terminate user activation timer if it is running since
               ** we want to reconfigure the user now.
               */
               memset ( &tmr, 0, sizeof ( CMTMR ) );
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = gCmCctkAccnt[ uIx ]->uHdl;
               tmr.eType = eCMEVT_TMR_INIT_REG_DELAY;
               if ( cmTmrIsRunning ( &tmr ) )
               {
                  cmTmrStop ( &tmr );
               }
#endif
/* TODO! - reg delay */

               /* If there are no active calls on this user.
               */
               if ( gCmCctkAccnt [ uIx ].uNumActiveCalls )
               {
                  bActive = BOS_TRUE;
               }

               if ( !bActive )
               {
                  /* Need to de-register first.
                  */
                  if ( gCmCctkAccnt[ uIx ].bInSrv &&
                       !gCmCctkAccnt[ uIx ].bBeRem )
                  {
                     cctkUserUnregister ( gCmCctkAccnt[ uIx ].uNet , BOS_TRUE );

                     if ( gCmCfgCb.cmgrEvtCb )
                     {
                        /* Inform host that unregistration is in progress */
                        ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_PROGRESS,
                                                 gCmCctkAccnt[ uIx ].uHdl,
#if CMGRCFG_MULTILINE
                                                 CMGR_INVALID_HANDLE,
                                                 gCmCctkAccnt[ uIx ].uHdl,
#endif /* CMGRCFG_MULTILINE */
                                                 (void *) eCMGREVT_OPER_NOK );
                     }
                  }
                  /* Go ahead with the deletion if not already asked for it
                  ** (deletion may be delayed due to CCTK internal asynch
                  ** process).
                  */
                  else if ( !gCmCctkAccnt[ uIx ].bBeRem )
                  {
                     gCmCctkAccnt[ uIx ].bBeRem = BOS_TRUE;
                     cctkUserRemove ( gCmCctkAccnt[ uIx ].uNet );
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
               gCmCctkAccnt[ uIx ].eCfgAction = eCMCFGACT_NOACTION;

               cmCctkAccntAdd ( (void *) &gCmCctkAccnt[ uIx ] );

               CMGRLOG_INFORMATION (( "cmCctkAccntReConfig: user %s is re-added",
                                      gCmCctkAccnt[ uIx ].uUid ));
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
