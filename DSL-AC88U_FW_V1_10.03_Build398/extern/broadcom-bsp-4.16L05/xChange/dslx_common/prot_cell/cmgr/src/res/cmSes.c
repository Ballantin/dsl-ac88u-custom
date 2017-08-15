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
**      This module implements session control from within the Call Manager.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* Note that we use a single dimensional global array rather than a two dimensional
** array indexed per user because we want to be flexible in attributing session to
** users such that in some specific scenario one may want to apply some temporary
** load balancing of extra available resources for vital services from one lightly
** loaded user to a fully loaded one.
*/
CMSES *gCmSesMed[ CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX ];
CMSES *gCmSesSrv[ CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX ];


/* ---- Private Function Prototypes --------------------------------------------- */
#if CMGRCFG_FIREWALL_CONTROL
void cmSesHandleMediaFirewallAccess( CMSES * pSes, CMGRMEDINFO * media );
static void cmSesHandleMediaFwPort( BOS_UINT32 uSesHdl, BOS_UINT32 * pCurrPort, BOS_UINT32 newPort, BOS_IP_ADDRESS * pCurrIP, BOS_IP_ADDRESS newIP );
static void cmSesMediaPortFwCMD( CMGRFWCMD cmd, BOS_UINT32 uSesHdl, BOS_UINT32 port, BOS_IP_ADDRESS ipAddr );
#endif /* CMGRCFG_FIREWALL_CONTROL */
/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmSes
**
** PURPOSE:    Manages the session resources.
**
** PARAMETERS: bCreate - Whether resources should be created (BOS_TRUE) or ended
**                       (BOS_FALSE) when this call is invoked.
**
** RETURNS:    None.
***********************************************************************************/
void cmSes ( BOS_BOOL bCreate )
{
   BOS_UINT32 uIx;

   if ( bCreate )
   {
      CMGRLOG_INFORMATION (( "cmSes: creating session support." ));

      for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
      {
         gCmSesMed[ uIx ] = (CMSES *) malloc ( sizeof(CMSES) );
         memset ( gCmSesMed[ uIx ], 0, sizeof(CMSES) );

         gCmSesMed[ uIx ]->uHdl = (uIx | CMSES_MEDIA_OFFSET);
         gCmSesMed[ uIx ]->uUsr = CMGR_INVALID_HANDLE;
         gCmSesMed[ uIx ]->eType = eCMSESTYPE_MEDIA;
         gCmSesMed[ uIx ]->ses.med.uPhy = CMGR_INVALID_HANDLE;
         gCmSesMed[ uIx ]->ses.med.eAction = eCMDM_UNKNOWNACTION;
      }

      for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX) ; uIx++ )
      {
         gCmSesSrv[ uIx ] = (CMSES *) malloc ( sizeof(CMSES) );
         memset ( gCmSesSrv[ uIx ], 0, sizeof(CMSES) );

         gCmSesSrv[ uIx ]->uHdl = (uIx | CMSES_SERVICE_OFFSET);
         gCmSesSrv[ uIx ]->uUsr = CMGR_INVALID_HANDLE;
         gCmSesSrv[ uIx ]->eType = eCMSESTYPE_SRV;
      }
   }
   else
   {
      CMGRLOG_INFORMATION (( "cmSes: terminating session support." ));

      for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
      {
         /* Clean up check to ensure we do not leave alive physical session.
         */
         if ( gCmSesMed[ uIx ] )
         {
            if ( gCmSesMed[ uIx ]->ses.med.uPhy != CMGR_INVALID_HANDLE )
            {
               cmSesPhyDelete ( gCmSesMed[ uIx ] );
            }

            free ( gCmSesMed[ uIx ] );
            gCmSesMed[ uIx ] = NULL;
         }
      }

      for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX) ; uIx++ )
      {
         if ( gCmSesSrv[ uIx ] )
         {
            free ( gCmSesSrv[ uIx ] );
            gCmSesSrv[ uIx ] = NULL;
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmSesAlloc
**
** PURPOSE:    Allocates a session resources to a specific user for usage by the
**             application
**
** PARAMETERS: eType - the type of session to be used.
**             uHdl - the user handle for which this resource is allocated.
**
** RETURNS:    A valid pointer to the resource allocated, or NULL if no resource
**             could be allocated.
**
** NOTE:       This function does *not* allocate physical memory, it fetches a
**             free resource from a pool.
***********************************************************************************/
CMSES *cmSesAlloc ( CMSESTYPE eType, BOS_UINT32 uHdl )
{
   if ( uHdl == CMGR_INVALID_HANDLE )
   {
      CMGRLOG_WARNING (( "cmSesAlloc: invalid input parameter (%d, 0x%X).",
                         eType, uHdl ));
      return NULL;
   }

   /* We need to check whether the user handle is a valid user handle as well
   ** as how many resources this user has allocated already.
   */
   if ( cmUsrValid ( uHdl ) != NULL )
   {
      CMSES *pFree = NULL;
      BOS_UINT32 uIdx = 0;
      BOS_UINT32 uCnt = 0;

      switch ( eType )
      {
         case eCMSESTYPE_MEDIA:
         {
            for ( uIdx = 0 ; uIdx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIdx++ )
            {
               if ( gCmSesMed[ uIdx ]->uUsr == uHdl )
               {
                  uCnt++;
               }

               if ( (gCmSesMed[ uIdx ]->uUsr == CMGR_INVALID_HANDLE) &&
                    (pFree == NULL) )
               {
                  pFree = gCmSesMed[ uIdx ];
               }
            }
         }
         break;

         case eCMSESTYPE_SRV:
         {
            for ( uIdx = 0 ; uIdx < (CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX) ; uIdx++ )
            {
               if ( gCmSesSrv[ uIdx ]->uUsr == uHdl )
               {
                  uCnt++;
               }

               if ( (gCmSesSrv[ uIdx ]->uUsr == CMGR_INVALID_HANDLE) &&
                    (pFree == NULL) )
               {
                  pFree = gCmSesSrv[ uIdx ];
               }
            }
         }
         break;

         default:
         break;
      }

      /* Now decide what to do.
      */
      if ( pFree == NULL )
      {
         CMGRLOG_WARNING (( "cmSesAlloc: (usr 0x%X) res pool empty!",
                            uHdl ));
         return NULL;
      }
      else
      {
         BOS_UINT32 uMax = 0;

         /* Initialize the free handle with proper data.
         */
         pFree->bEstab = BOS_FALSE;
         pFree->bMaintdOnOos = BOS_FALSE;
         pFree->uNet = CMGR_INVALID_HANDLE;
         pFree->uNetUsr = CMGR_INVALID_HANDLE;
         pFree->uApp = CMGR_INVALID_HANDLE;

#if CMGRCFG_FIREWALL_CONTROL
         memset(&pFree->sesMedFwInfo, 0, sizeof(CMSESMEDFWINFO));
#endif /* CMGRCFG_FIREWALL_CONTROL */

         switch ( eType )
         {
            case eCMSESTYPE_MEDIA:
            {
               pFree->ses.med.bFocus = BOS_FALSE;
               pFree->ses.med.bBgFocus = BOS_FALSE;
               pFree->ses.med.bEmergency = BOS_FALSE;
               pFree->ses.med.bEmgExpired = BOS_FALSE;
               pFree->ses.med.bMediaRx = BOS_FALSE;
               pFree->ses.med.bHeld = BOS_FALSE;
               pFree->ses.med.bRemInit = BOS_FALSE;
               pFree->ses.med.bNeedFinal = BOS_FALSE;
               pFree->ses.med.uConf = CMGR_INVALID_HANDLE;
               pFree->ses.med.uBargeIn = CMGR_INVALID_HANDLE;
               pFree->ses.med.uPhy = CMGR_INVALID_HANDLE;
               pFree->ses.med.eAction = eCMDM_UNKNOWNACTION;
               pFree->ses.med.bCfv = BOS_FALSE;
               pFree->ses.med.bHotline = BOS_FALSE;
               pFree->ses.med.pAutoCall = NULL;
               pFree->ses.med.pLinkedSes = NULL;
               uMax = CMGRCFG_MEDSES_MAX;
            }
            break;

            case eCMSESTYPE_SRV:
            {
               uMax = CMGRCFG_MEDSES_MAX;
            }
            break;

            default:
            break;
         }

         if ( uCnt < uMax )
         {
            /* No brainer.  Allocate the resource for this user to use.
            */
            CMGRLOG_DEBUG (( "cmSesAlloc: usr 0x%X - res 0x%X",
                             uHdl, pFree ));
            pFree->uUsr = uHdl;
            return pFree;
         }
         else if ( uCnt == uMax )
         {
            /* If there is an available resource, we allow this one time usage of
            ** the resource by the user as we assume this is for emergency use.
            **
            ** The current allocation decision is nothing fancy, but potentially one
            ** could decide using different criteria whether to grant the resource
            ** or not at this time.
            */
            CMGRLOG_WARNING (( "cmSesAlloc: usr 0x%X last chance res 0x%X.",
                               uHdl, pFree ));
            pFree->uUsr = uHdl;
            return pFree;
         }
         else
         {
            CMGRLOG_WARNING (( "cmSesAlloc: usr 0x%X has too many resources.",
                               uHdl ));
            return NULL;
         }
      }
   }
   else
   {
      CMGRLOG_WARNING (( "cmSesAlloc: invalid usr 0x%X.",
                         uHdl ));
      return NULL;
   }
}


/***********************************************************************************
** FUNCTION:   cmSesFree
**
** PURPOSE:    Frees a session resources from a specific user.
**
** PARAMETERS: pSes - the session resource to be freed.
**             uHdl - the user handle freeing this resource.
**
** RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
**
** NOTE:       This function does *not* free physical memory, it returns a
**             resource to a pool.
***********************************************************************************/
BOS_BOOL cmSesFree ( CMSES *pSes, BOS_UINT32 uHdl )
{
   CMUSR *pUsr = cmUsrValid ( pSes->uUsr ) ;
   BOS_BOOL    bSesTypeMedia;
   CMAUTOCALL *pAutoCall = pSes->ses.med.pAutoCall ;

   if ( (pSes == NULL) || (uHdl == CMGR_INVALID_HANDLE) )
   {
      CMGRLOG_WARNING (( "cmSesFree: invalid input parameters (0x%X, 0x%X), pUsr(0x%X).",
                         pSes, uHdl, pUsr ));
      return BOS_FALSE;
   }

/* In multiline, always free a session when asked to. We check if any
** other session contains this call id first, so it's safe to delete it.
*/
   if ( pSes->uUsr != uHdl )
   {
      CMGRLOG_WARNING ((
         "cmSesFree: Failed ses 0x%X user (0x%X) not owner (0x%X)!",
         pSes, uHdl, pSes->uUsr ));
      return BOS_FALSE;
   }

   // if ( pSes->uNet != CMGR_INVALID_HANDLE )
   // {
      // CMGRLOG_WARNING ((
         // "cmSesFree: Failed ses 0x%X valid net (0x%X) resource!",
         // pSes, pSes->uNet ));
      // return BOS_FALSE;
   // }

   /* If everything is fine, free up the resource!
   */
   CMGRLOG_DEBUG ((
      "cmSesFree: ses 0x%X valid net (0x%X) on usr (0x%X)",
      pSes, pSes->uNet, pSes->uUsr ));

   /* Free up autocall resource if present
   */
   if( pAutoCall )
   {
      pAutoCall->bUsed = BOS_FALSE;
      pAutoCall->bPending = BOS_FALSE;
      pAutoCall->uSes = CMGR_INVALID_HANDLE;
      pSes->ses.med.pAutoCall = NULL;
      pSes->ses.med.eAction = eCMDM_UNKNOWNACTION;
   }

   if( pSes->bEstab )
   {
      pSes->bEstab = BOS_FALSE;
   }

   pSes->bMaintdOnOos = BOS_FALSE;
   pSes->uUsr    = CMGR_INVALID_HANDLE;
   pSes->uApp    = CMGR_INVALID_HANDLE;
   pSes->uNetUsr = CMGR_INVALID_HANDLE;
   pSes->uNet    = CMGR_INVALID_HANDLE;
   bSesTypeMedia = ( pSes->eType == eCMSESTYPE_MEDIA ) ? BOS_TRUE : BOS_FALSE;
   memset ( &pSes->ses, 0, sizeof(pSes->ses) );

   /* If we are freeing a media session, we need to reset the physical handle
   ** accordingly */
   if ( bSesTypeMedia )
   {
      pSes->ses.med.uPhy = CMGR_INVALID_HANDLE;
   }

   return BOS_TRUE;
}


/***********************************************************************************
** FUNCTION:   cmSesFindNetHdl
**
** PURPOSE:    Finds a session based on its network handle.
**
** PARAMETERS: uNet - the network handle to check for.
**             uUsr - the user handle for which to check.
**
** RETURNS:    Pointer to the session block if found, NULL otherwise.
**
** NOTE:       The network handle and user handle is the only way we can uniquely
**             identify events for a given session when reported by the CCTK layer.
***********************************************************************************/
CMSES * cmSesFindNetHdl ( BOS_UINT32 uNet, BOS_UINT32 uUsr )
{
   BOS_UINT32 uIx;

   if ( uNet == CMGR_INVALID_HANDLE || uUsr == CMGR_INVALID_HANDLE )
   {
      return NULL;
   }

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] &&
           gCmSesMed[ uIx ]->uNet == uNet &&
           gCmSesMed[ uIx ]->uUsr == uUsr )
      {
         return gCmSesMed[ uIx ];
      }
   }

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX) ; uIx++ )
   {
      if ( gCmSesSrv[ uIx ] &&
           gCmSesSrv[ uIx ]->uNet == uNet &&
           gCmSesSrv[ uIx ]->uUsr == uUsr )
      {
         return gCmSesSrv[ uIx ];
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmSesFindPhyHdl
**
** PURPOSE:    Finds a session based on its physical (endpoint layer) handle.
**
** PARAMETERS: uPhy - the physical handle to check for.
**
** RETURNS:    Pointer to the session block if found, NULL otherwise.
**
** NOTE:       The physical handle is the only way we can identify events for a
**             given session when reported by the endpoint (physical) layer.
***********************************************************************************/
CMSES * cmSesFindPhyHdl ( BOS_UINT32 uPhy )
{
   BOS_UINT32 uIx;

   /* Such search is only valid on media session.  Service session do not have
   ** physical association since they do not allocate or make use of media.
   */
   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] && gCmSesMed[ uIx ]->ses.med.uPhy == uPhy )
      {
         return gCmSesMed[ uIx ];
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmSesFindAppHdl
**
** PURPOSE:    Finds a session based on its application handle.
**
** PARAMETERS: uApp - the application handle to check for.
**
** RETURNS:    Pointer to the session block if found, NULL otherwise.
**
** NOTE:       The application handle is the only way we can identify events for a
**             given session when reported by the application (external) layer.
***********************************************************************************/
CMSES * cmSesFindAppHdl ( BOS_UINT32 uApp )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] &&
           (gCmSesMed[ uIx ]->uApp != CMGR_INVALID_HANDLE) &&
           (gCmSesMed[ uIx ]->uApp == uApp) )
      {
         return gCmSesMed[ uIx ];
      }
   }

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX) ; uIx++ )
   {
      if ( gCmSesSrv[ uIx ] &&
           (gCmSesMed[ uIx ]->uApp != CMGR_INVALID_HANDLE) &&
           (gCmSesSrv[ uIx ]->uApp == uApp) )
      {
         return gCmSesSrv[ uIx ];
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmSesValid
**
** PURPOSE:    Checks whether a session handle is for a valid session.
**
** PARAMETERS: uHdl - the session handle to check for.
**
** RETURNS:    Pointer to valid session or NULL if not found.
***********************************************************************************/
CMSES * cmSesValid ( BOS_UINT32 uHdl )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] &&
           ( gCmSesMed[ uIx ]->uHdl == uHdl ) )
      {
            return gCmSesMed[ uIx ];
         }
   }

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX) ; uIx++ )
   {
      if ( gCmSesSrv[ uIx ] &&
           ( gCmSesSrv[ uIx ]->uHdl == uHdl ) )
      {
         return gCmSesSrv[ uIx ];
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmSesMedValid
**
** PURPOSE:    Checks whether a session handle is for a valid media session.
**
** PARAMETERS: uHdl - the session handle to check for.
**
** RETURNS:    Pointer to valid session or NULL if not found.
***********************************************************************************/
CMSES * cmSesMedValid ( BOS_UINT32 uHdl )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] &&
           ( gCmSesMed[ uIx ]->uHdl == uHdl ) )
      {
         return gCmSesMed[ uIx ];
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmSesSrvValid
**
** PURPOSE:    Checks whether a session handle is for a valid service session.
**
** PARAMETERS: uHdl - the session handle to check for.
**
** RETURNS:    Pointer to valid session or NULL if not found.
***********************************************************************************/
CMSES * cmSesSrvValid ( BOS_UINT32 uHdl )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX) ; uIx++ )
   {
      if ( gCmSesSrv[ uIx ] &&
           ( gCmSesSrv[ uIx ]->uHdl == uHdl ) )
      {
         return gCmSesSrv[ uIx ];
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   cmSesPhyCreate
**
** PURPOSE:    Creates the physical layer to support this session (i.e. the data
**             stream).
**
** PARAMETERS: pSes - the session of interest.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyCreate ( CMSES *pSes )
{
   CMUSR *pUsr = cmUsrValid ( pSes->uUsr );
   CMGRMEDINFO media;

   memset( &media, 0, sizeof( CMGRMEDINFO ) );

   if ( !pUsr )
   {
      CMGRLOG_CRITICAL (( "cmSesPhyCreate: 0x%X - invalid user 0x%X!!",
                          pSes->uHdl, pSes->uUsr ));
      return;
   }

   CMGRLOG_INFORMATION (( "cmSesPhyCreate: 0x%X (0x%X) for user 0x%X",
                          pSes->uHdl, pSes->uNet, pSes->uUsr ));
                          
   /* Hard-coded information for cellular calls. This will be
   **  re-negotiated between DSP and the modem.
   */
   media.media.ingress.audio.mode = eCCTKMEDMODE_ACTIVE;
   media.media.ingress.audio.uNumCodec++;
   media.media.ingress.audio.codec[0].eType = eCCTKCODEC_GSMAMR;
   media.media.ingress.audio.codec[0].parm.amrCfg.bOctetAlign = eCCTKAMR_YES;
   media.media.ingress.audio.codec[0].uRtpCode = 100;
   media.media.egress.audio.mode = eCCTKMEDMODE_ACTIVE;
   media.media.egress.audio.uNumCodec++;
   media.media.egress.audio.codec[0].eType = eCCTKCODEC_GSMAMR;
   media.media.egress.audio.codec[0].parm.amrCfg.bOctetAlign = eCCTKAMR_YES;
   media.media.egress.audio.codec[0].uRtpCode = 100;
   


   /* Setup media information which we get locally.
   */
   media.uDscp = cmSesGetDscp ( pSes );
   media.bEmergency = pSes->ses.med.bEmergency;
   media.uAppIx = pSes->uApp;
   media.bActKeepAlive = pSes->ses.med.bMediaKAM;

#if CMGRCFG_FIREWALL_CONTROL
   cmSesHandleMediaFirewallAccess( pSes, &media );
#endif /* CMGRCFG_FIREWALL_CONTROL */

   pSes->ses.med.uPhy = gCmCfgCb.cmgrSesCtlCb ( eCMGRSESCMD_CREATE,
                                                pUsr->uPhys,
                                                CMGR_INVALID_HANDLE,
                                                pSes->uNet,
                                                CMGR_INVALID_HANDLE,
                                                (void *) &media );
}


/***********************************************************************************
** FUNCTION:   cmSesPhyDelete
**
** PURPOSE:    Deletes the physical layer to support this session (i.e. the data
**             stream).
**
** PARAMETERS: pSes - the session of interest.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyDelete ( CMSES *pSes )
{
   CMUSR *pUsr = cmUsrValid ( pSes->uUsr );
   
   if ( !pUsr )
   {
      /* If this is an internal call with no user assigned, it's possible the user
       * unlinked the session first, so don't print this error message. */
      if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
         CMGRLOG_INFORMATION (( "cmSesPhyDelete: 0x%X - invalid user 0x%X!!",
                             pSes->uHdl, pSes->uUsr ));
      }
      return;
   }

   CMGRLOG_INFORMATION (( "cmSesPhyDelete: 0x%X (0x%X) for user 0x%X",
                          pSes->uHdl, pSes->uNet, pSes->uUsr ));

   if ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
   {
      if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
         gCmCfgCb.cmgrSesCtlCb ( eCMGRSESCMD_TERMINATE,
                                 pUsr->uPhys,
                                 pSes->ses.med.uPhy,
                                 pSes->uNet,
                                 CMGR_INVALID_HANDLE,
                                 NULL );

#if CMGRCFG_FIREWALL_CONTROL
         cmSesHandleMediaFirewallAccess( pSes, NULL );
#endif /* CMGRCFG_FIREWALL_CONTROL */
      }
      /* Barge-in session
      */
      else if ( pSes->ses.med.uBargeIn != CMGR_INVALID_HANDLE )
      {
         CMSES* pOtherSes = cmSesValid( pSes->ses.med.uBargeIn );
         CMUSR* pOtherUsr = NULL;

         if( pOtherSes && ( pOtherSes != pSes ) )
         {
            pOtherUsr = cmUsrValid( pOtherSes->uUsr );
         }


         CMGRLOG_INFORMATION (( "cmSesPhyDelete: 0x%X (0x%X) barge in to 0x%X",
                          pSes->uHdl, pSes->uUsr, pSes->ses.med.uBargeIn ));

         gCmCfgCb.cmgrSesCtlCb ( eCMGRSESCMD_TERMINATE_BARGEIN,
                                 pUsr->uPhys,
                                 pSes->ses.med.uPhy,
                                 pSes->uNet,
                                 CMGR_INVALID_HANDLE,
                                 (void*)(( pOtherUsr ) ? pOtherUsr->uPhys : CMGR_INVALID_HANDLE ));

         /* Reset the barge-in hdl so we don't try to delete the connection twice.
         */
         pSes->ses.med.uBargeIn = CMGR_INVALID_HANDLE;

      }
      /* Internal call case */
      else
      {
         CMSESLIST sesList;
         BOS_UINT32 uIx;
         BOS_BOOL bFound = BOS_FALSE;

         memset( &sesList, 0, sizeof( CMSESLIST ) );

         cmSesListWithCid( pSes->uNet, &sesList );
         CMGRLOG_INFORMATION(( "%s: Session list length %d\n", __FUNCTION__, sesList.uLength ));
         for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
         {
            CMSES* pOtherSes = cmSesValid( sesList.uSes[ uIx ] );

            if( pOtherSes && ( pOtherSes != pSes ) )
            {
               if( pOtherSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
               {
                  CMUSR* pOtherUsr = cmUsrValid( pOtherSes->uUsr );
                  gCmCfgCb.cmgrSesCtlCb ( eCMGRSESCMD_TERMINATE_INT,
                                          pUsr->uPhys,
                                          pSes->ses.med.uPhy,
                                          pSes->uNet,
                                          CMGR_INVALID_HANDLE,
                                          (void*)(( pOtherUsr ) ? pOtherUsr->uPhys : CMGR_INVALID_HANDLE ));
                  /* Leave the session up, but set the phy handle to 0
                  ** so we don't try to delete the connection twice.
                  */
                  pOtherSes->ses.med.uPhy = CMGR_INVALID_HANDLE;
                  bFound = BOS_TRUE;
               }
            }
         }
         if( bFound == BOS_FALSE )
         {
            CMGRLOG_CRITICAL(( "%s: Unable to find other session with cid [%X]\n", __FUNCTION__, pSes->uHdl ));
            gCmCfgCb.cmgrSesCtlCb ( eCMGRSESCMD_TERMINATE_INT,
                                    pUsr->uPhys,
                                    pSes->ses.med.uPhy,
                                    pSes->uNet,
                                    CMGR_INVALID_HANDLE,
                                    (void*)CMGR_INVALID_HANDLE );
         }
      }

      pSes->ses.med.uPhy = CMGR_INVALID_HANDLE;
   }
}


/***********************************************************************************
** FUNCTION:   cmSesPhyMute
**
** PURPOSE:    Mutes the physical layer taht support this session (i.e. the data
**             stream).
**
** PARAMETERS: pSes - the session of interest.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyMute ( CMSES *pSes )
{
   CMUSR *pUsr = cmUsrValid ( pSes->uUsr );
   if ( !pUsr )
   {
      CMGRLOG_CRITICAL (( "cmSesPhyMute: 0x%X - invalid user 0x%X!!",
                          pSes->uHdl, pSes->uUsr ));
      return;
   }

   CMGRLOG_INFORMATION (( "cmSesPhyMute: 0x%X (0x%X) for user 0x%X",
                          pSes->uHdl, pSes->uNet, pSes->uUsr ));

   if ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
   {
      if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
         gCmCfgCb.cmgrSesCtlCb ( eCMGRSESCMD_MUTE,
                                 pUsr->uPhys,
                                 pSes->ses.med.uPhy,
                                 pSes->uNet,
                                 CMGR_INVALID_HANDLE,
                                 NULL );
      }
      else
      {
         CMSESLIST sesList;
         BOS_UINT32 uIx;

         memset( &sesList, 0, sizeof( CMSESLIST ) );

         cmSesListWithCid( pSes->uNet, &sesList );
         for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
         {
            CMSES* pOtherSes = cmSesValid( sesList.uSes[ uIx ] );

            if( pOtherSes && ( pOtherSes->uHdl != pSes->uHdl ) )
            {
               if( pOtherSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
               {
                  CMUSR* pOtherUsr = cmUsrValid( pOtherSes->uUsr );
                  gCmCfgCb.cmgrSesCtlCb ( eCMGRSESCMD_MUTE_INT,
                                          pUsr->uPhys,
                                          pSes->ses.med.uPhy,
                                          pSes->uNet,
                                          CMGR_INVALID_HANDLE,
                                          (void*)((pOtherUsr) ? pOtherUsr->uPhys : CMGR_INVALID_HANDLE ));
               }
            }
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmSesClid
**
** PURPOSE:    Formats and creates the caller identification information for a
**             session.
**
** PARAMETERS: pSes - the session of interest.
**             bName - whether the name information should be passed with the CLID
**             bNumber - whether the number information should be passing with
**                       the CLID
**
** RETURNS:    None.
***********************************************************************************/
void cmSesClid ( CMSES *pSes, BOS_BOOL bName, BOS_BOOL bNumber )
{
   CMUSR *pUsr = cmUsrValid ( pSes->uUsr );
   BOS_UINT8 uClid[ CMGR_CLID_LEN + 1];
   CMGREPSIG sig;
   struct tm locTime;
   time_t calTime;
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   PROV_CFG_IDX cfgIdx;

   if ( pUsr && pSes )
   {
      memset ( &sig,
               0,
               sizeof ( CMGREPSIG ) );

      memset( uClid,
              0,
              sizeof ( uClid ));

      sig.eAct = eCMGREPSIGACT_START;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = pSes->uNet;
      sig.uSig = eEPTSIG_CALLID;
      sig.pData = &uClid;

      calTime = time ( NULL );
      bosTimeGetGmTime ( &calTime, &locTime );

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                 &cfgIdx,
                                 (void *) &pUsrClidDsp,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pUsrClidDsp )
         {
            /* Check whether we need to adjust the local time based on the
            ** time adjustement offset.
            */
            if ( pUsrClidDsp->sTimeAdj )
            {
               cmDateOffsetMin ( &locTime, pUsrClidDsp->sTimeAdj );
            }

            /* Check whether we need to adjust the local time based on the
            ** DST (daylight saving time) offset.
            */
            if ( pUsrClidDsp->sDSTTimeAdj )
            {
               cmDateOffsetMin ( &locTime, pUsrClidDsp->sDSTTimeAdj );
            }
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                               &cfgIdx,
                               (void *) pUsrClidDsp,
                               CFG_ITEM_DEL );
         pUsrClidDsp = NULL;
      }

      /* Set caller identification time.
      */
      sprintf ( (char *) uClid,
                "%02d/%02d/%02d/%02d, ",
                locTime.tm_mon + 1,
                locTime.tm_mday,
                locTime.tm_hour,
                locTime.tm_min );

      /* Set caller identification number.
      */
      if ( bNumber )
      {
         CFGPROVDEVICECFG *pDevCfg = NULL;
         PROV_CFG_IDX cfgIdx;
         BOS_UINT32 uStrip = 0;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                    &cfgIdx,
                                    (void *) &pDevCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            /* If a country code was configured.
            */
            if ( pDevCfg &&
                 pDevCfg->pcCountryCode &&
                 strlen ( (const char *) pDevCfg->pcCountryCode ) )
            {
               /* Check whether we should remove the country code from the
               ** leading number information.
               */
               if ( !strCmpNoCaseSize(
                        (const char *) pDevCfg->pcCountryCode,
                        (const char *) pSes->ses.med.callInfo.clid.cNumber,
                        strlen ( (const char *) pDevCfg->pcCountryCode ) ) )
               {
                  uStrip = strlen ( (const char *) pDevCfg->pcCountryCode );
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                  &cfgIdx,
                                  (void *) pDevCfg,
                                  CFG_ITEM_DEL );
            pDevCfg = NULL;
         }

         sprintf ( (char *) &uClid[ strlen( (char *) uClid ) ],
                   "\"%s\", ",
                   &pSes->ses.med.callInfo.clid.cNumber[ uStrip ] );
      }
      else
      {
         sprintf ( (char *) &uClid[ strlen( (char *) uClid ) ],
                   ", " );
      }
      /* Set caller identification name.
      */
      if ( bName )
      {
         sprintf ( (char *) &uClid[ strlen( (char *) uClid ) ],
                   "\"%s\"",
                   pSes->ses.med.callInfo.clid.cName );
      }
      cmPhySignal ( pUsr->uPhys, &sig );

      CMGRLOG_INFORMATION (( "cmSesClid: \'%s\' (\'%s\', \'%s\') on 0x%X (0x%X)",
                             uClid, pSes->ses.med.callInfo.clid.cName,
                             pSes->ses.med.callInfo.clid.cNumber,
                             pSes->uHdl, pSes->uNet ));
   }
}


/***********************************************************************************
** FUNCTION:   cmSesTerminate
**
** PURPOSE:    Terminates a session being used.
**
** PARAMETERS: pSes - the session resource to be freed.
**
** RETURNS:    None.
**
** NOTE:       This is only used for media session.
***********************************************************************************/
void cmSesTerminate ( CMSES *pSes )
{
   CMUSR *pUsr = cmUsrValid ( pSes->uUsr );
   BOS_UINT32 uNumSesWithCid = 0;

   if ( pUsr && pSes )
   {
      uNumSesWithCid = cmNumSesWithCid( pSes->uNet );
      /* Terminate the network call if required.
      */

      CMGRLOG_DEBUG ((
         "cmSesTerminate: Terminate ses 0x%X, %u valid net(0x%X) on uUsr (0x%X)",
         pSes, uNumSesWithCid, pSes->uNet, pSes->uUsr ));

      if ( pSes->uNet != CMGR_INVALID_HANDLE )
      {
         cmAppNetCallRelease( pSes->uNet );
      }
      else
      {
         cmSesFree ( pSes, pUsr->uHdl );
      }

      if( pSes->bEstab )
      {
         pSes->bEstab = BOS_FALSE;
      }

      /* Terminate the physical connection if required.
      */
      if ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
      {
         cmSesPhyDelete ( pSes );
         pSes->ses.med.uPhy = CMGR_INVALID_HANDLE;
      }


      pSes->bMaintdOnOos = BOS_FALSE;
      pSes->ses.med.bEmergency = BOS_FALSE;
      pSes->ses.med.bEmgExpired = BOS_FALSE;
      pSes->ses.med.bHotline = BOS_FALSE;
      pSes->ses.med.bMediaRx = BOS_FALSE;
      pSes->ses.med.bHeld = BOS_FALSE;
      pSes->ses.med.bRemInit = BOS_FALSE;
      pSes->ses.med.bFocus = BOS_FALSE;
      pSes->ses.med.bBgFocus = BOS_FALSE;
      pSes->ses.med.uConf = CMGR_INVALID_HANDLE;
      pSes->ses.med.uBargeIn = CMGR_INVALID_HANDLE;
      pSes->ses.med.pLinkedSes = NULL;


      cmUsrUnLinkSes ( pUsr, pSes );
   }
}


/***********************************************************************************
** FUNCTION:   cmSesGetDscp
**
** PURPOSE:    Gets the DSCP configuration information for this session.
**
** PARAMETERS: pSes - the session resource to be queried for.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
BOS_UINT32 cmSesGetDscp ( CMSES *pSes )
{
   CMUSR *pUsr = cmUsrValid ( pSes->uUsr );
   BOS_UINT32 uDscp = 0;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGSIPEMERGE *pEmerCall = NULL;
   PROV_CFG_IDX cfgIdx;

   if ( pUsr && pSes )
   {
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( pSes->ses.med.bEmergency )
      {
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                    &cfgIdx,
                                    (void *) &pEmerCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            uDscp = pEmerCall->uMediaDSCPVal;

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                  &cfgIdx,
                                  (void *) pEmerCall,
                                  CFG_ITEM_DEL );
            pEmerCall = NULL;
         }
      }
      else
      {
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                    &cfgIdx,
                                    (void *) &pBasicCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            uDscp = pBasicCall->uNEMDSCPValueMedia;

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                  &cfgIdx,
                                  (void *) pBasicCall,
                                  CFG_ITEM_DEL );
            pBasicCall = NULL;
         }
      }
   }

   return uDscp;
}


/***********************************************************************************
** FUNCTION:   cmSesPhyActMonThresh
**
** PURPOSE:    Setup the activity monitor threshold on the session, when set, the
**             session would report an event when media is no longer seen passed
**             the configured threshold.
**
** PARAMETERS: pSes - the session of interest.
**             uMsec - the threshold in milliseconds, 0 to disable monitoring.
**
** RETURNS:    None.
***********************************************************************************/
void cmSesPhyActMonThresh ( CMSES *pSes, BOS_UINT32 uMsec )
{
   if ( pSes )
   {
      CMUSR *pUsr = cmUsrValid ( pSes->uUsr );

      if ( pUsr )
      {
         CMGRLOG_INFORMATION ((
            "cmSesPhyActMonThresh: 0x%X (0x%X) for 0x%X (0x%X) - threshold %d msec",
            pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr, uMsec ));

         if ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
         {
            gCmCfgCb.cmgrSesCtlCb ( eCMGRSESCMD_ACTMONTHRES,
                                    pUsr->uPhys,
                                    pSes->ses.med.uPhy,
                                    pSes->uNet,
                                    CMGR_INVALID_HANDLE,
                                    (void *) &uMsec );
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmNumSesWithCid
**
** PURPOSE:    Finds the number of sessions with this call-id
**
** PARAMETERS: uCid  - the network call id of interest.
**
** RETURNS:    Number of sessions having this network call id.
***********************************************************************************/
BOS_UINT32 cmNumSesWithCid( BOS_UINT32 uCid )
{
   BOS_UINT32 uIx;
   BOS_UINT32 count = 0;

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] &&
           ( gCmSesMed[ uIx ]->uNet == uCid ) )
      {
         count++;
      }
   }
   return count;
}


/***********************************************************************************
** FUNCTION:   cmNumSesWithNet
**
** PURPOSE:    Finds the number of sessions with this network handle
**
** PARAMETERS: uNet  - the network id of interest.
**
** RETURNS:    Number of sessions having this network id.
***********************************************************************************/
BOS_UINT32 cmNumSesWithNet( BOS_UINT32 uNet )
{
   BOS_UINT32 uIx;
   BOS_UINT32 count = 0;

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] &&
           ( gCmSesMed[ uIx ]->uNetUsr == uNet ) )
      {
         count++;
      }
   }
   return count;
}

/***********************************************************************************
** FUNCTION:   cmNumSesBargedIn
**
** PURPOSE:    Finds the number of sessions that have barged in to this session
**
** PARAMETERS: uBargeHdl  - the session to find.
**
** RETURNS:    Number of sessions barged in to this session.
***********************************************************************************/
BOS_UINT32 cmNumSesBargedIn( BOS_UINT32 uBargeHdl )
{
   BOS_UINT32 uIx;
   BOS_UINT32 count = 0;

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] &&
            gCmSesMed[ uIx ]->ses.med.uBargeIn == uBargeHdl )
      {
         count++;
      }
   }
   return count;
}


/***********************************************************************************
** FUNCTION:   cmSesListWithCid
**
** PURPOSE:    Finds the list of sessions with the associated call id.
**
** PARAMETERS: uCid  (IN)  - the call id to use
**             pList (OUT) - the pointer to the CMSESLIST that we need to populate
**
** RETURNS:    None.
***********************************************************************************/
void cmSesListWithCid( BOS_UINT32 uCid, CMSESLIST* pList )
{
   BOS_UINT32 uIx, uIy;

   CMGRLOG_INFORMATION (( "%s: Cid [%X], pList [0x%X]", __FUNCTION__, uCid, pList ));
   if( ( uCid == CMGR_INVALID_HANDLE ) || ( pList == NULL ) )
   {
      return;
   }

   for( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if( gCmSesMed[ uIx ] &&
          ( gCmSesMed[ uIx ]->uNet == uCid ) )
      {
         BOS_BOOL bSesExists = BOS_FALSE;

         /* Iterate through the list to make sure we haven't already added this session */
         for( uIy = 0 ; uIy < pList->uLength ; uIy++ )
         {
            if( gCmSesMed[ uIx ]->uHdl == pList->uSes[ uIy ] )
            {
               bSesExists = BOS_TRUE;
               break;
            }
         }

         /* If we didn't find the user, add it to the list */
         if( bSesExists == BOS_FALSE )
         {
            pList->uSes[ pList->uLength ] = gCmSesMed[ uIx ]->uHdl;
            pList->uLength++;
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmSesListWithNet
**
** PURPOSE:    Finds the list of sessions with the associated network id.
**
** PARAMETERS: uNet  (IN)  - the network id to use
**             pList (OUT) - the pointer to the CMSESLIST that we need to populate
**
** RETURNS:    None.
***********************************************************************************/
void cmSesListWithNet( BOS_UINT32 uNet, CMSESLIST* pList )
{
   BOS_UINT32 uIx, uIy;

   CMGRLOG_INFORMATION (( "%s: Net [%X], pList [0x%X]", __FUNCTION__, uNet, pList ));
   if( ( uNet == CMGR_INVALID_HANDLE ) || ( pList == NULL ) )
   {
      return;
   }

   for( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if( gCmSesMed[ uIx ] &&
          ( gCmSesMed[ uIx ]->uNetUsr == uNet ) )
      {
         BOS_BOOL bSesExists = BOS_FALSE;

         /* Iterate through the list to make sure we haven't already added this session */
         for( uIy = 0 ; uIy < pList->uLength ; uIy++ )
         {
            if( gCmSesMed[ uIx ]->uHdl == pList->uSes[ uIy ] )
            {
               bSesExists = BOS_TRUE;
               break;
            }
         }

         /* If we didn't find the user, add it to the list */
         if( bSesExists == BOS_FALSE )
         {
            pList->uSes[ pList->uLength ] = gCmSesMed[ uIx ]->uHdl;
            pList->uLength++;
         }
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmSesListWithBargeIn
**
** PURPOSE:    Finds the list of sessions that have barged-in to the session.
**
** PARAMETERS: uBargeHdl (IN)  - the barge-in session to find
**             pList (OUT) - the pointer to the CMSESLIST that we need to populate
**
** RETURNS:    None.
***********************************************************************************/
void cmSesListWithBargeIn( BOS_UINT32 uBargeHdl, CMSESLIST* pList )
{
   BOS_UINT32 uIx, uIy;

   if( ( uBargeHdl == CMGR_INVALID_HANDLE ) || ( pList == NULL ) )
   {
      return;
   }

   CMGRLOG_DEBUG (( "cmSesListWithBargeIn: uHdl[%X], pList[0x%X]", uBargeHdl, pList ));

   for ( uIx = 0 ; uIx < (CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX) ; uIx++ )
   {
      if ( gCmSesMed[ uIx ] && gCmSesMed[ uIx ]->ses.med.uBargeIn == uBargeHdl )
      {

         BOS_BOOL bSesExists = BOS_FALSE;

         /* Iterate through the list to make sure we haven't already added this session */
         for( uIy = 0 ; uIy < pList->uLength ; uIy++ )
         {
            if( gCmSesMed[ uIx ]->uHdl == pList->uSes[ uIy ] )
            {
               bSesExists = BOS_TRUE;
               break;
            }
         }

         /* If we didn't find the user, add it to the list */
         if( bSesExists == BOS_FALSE )
         {
            pList->uSes[ pList->uLength ] = gCmSesMed[ uIx ]->uHdl;
            pList->uLength++;
         }
      }
   }
}


#if CMGRCFG_FIREWALL_CONTROL
/***********************************************************************************
** FUNCTION:   cmSesMediaPortFwCMD
**
** PURPOSE:    Invokes firewall callback for a given port/IP-addr combination.
**
** PARAMETERS: cmd     - firewall call back command.
**             uSesHdl - session handle.
**             port    - new port to be used
**             ipAddr  - ipAddr IP to be used
**
** RETURNS:
**
** NOTE:
**
***********************************************************************************/
static void cmSesMediaPortFwCMD( CMGRFWCMD cmd, BOS_UINT32 uSesHdl, BOS_UINT32 port, BOS_IP_ADDRESS ipAddr )
{
   if( gCmCfgCb.cmgrFwCb != NULL )
   {
      CMGRFWINFO cmgrFwInfo;

      /* Open the RTP port on the firewall - Allow traffic from remote IP on local port.
      */
      cmgrFwInfo.uPort = port;
      cmgrFwInfo.eProt = eCMGRFWPROT_UDP;
      bosIpAddrCreateCopy(&ipAddr, &cmgrFwInfo.ipAdd);

      gCmCfgCb.cmgrFwCb( cmd, uSesHdl, &cmgrFwInfo );
   }
}

/***********************************************************************************
** FUNCTION:   cmSesHandleMediaFwPort
**
** PURPOSE:    Updates firewall setttings for a media port.
**
** PARAMETERS: uSesHdl - session handle.
**             pCurrPort - current media port in use.
**             newPort - new port to be used
**             pCurrIP - current IP in use
**             newIP - new IP to be used
**
** RETURNS:
**
** NOTE: Updates to in-use IP-addresses takes place in  cmSesHandleMediaFirewallAccess
**       after all the ports have been processed
**
***********************************************************************************/
static void cmSesHandleMediaFwPort( BOS_UINT32 uSesHdl, BOS_UINT32 * pCurrPort, BOS_UINT32 newPort, BOS_IP_ADDRESS * pCurrIP, BOS_IP_ADDRESS newIP )
{
   if( *pCurrPort && !newPort )
   {
      /* Port set to zero, close firewall */
      cmSesMediaPortFwCMD( eCMGRFWCMD_CLOSE, uSesHdl, *pCurrPort, *pCurrIP );
   }
   else if ( !(*pCurrPort) && newPort )
   {
      /* Port set to valid port, open firewall */
      cmSesMediaPortFwCMD( eCMGRFWCMD_OPEN, uSesHdl, newPort, newIP );
   }
   else if ( ( *pCurrPort && newPort ) && ( *pCurrPort != newPort || !bosIpAddrIsEqual(pCurrIP, &newIP) ) )
   {
      /* Port/IP changed, Close old port */
      cmSesMediaPortFwCMD( eCMGRFWCMD_CLOSE, uSesHdl, *pCurrPort, *pCurrIP );

      /* Port/IP changed, Open new port */
      cmSesMediaPortFwCMD( eCMGRFWCMD_OPEN, uSesHdl, newPort, newIP );
   }

   /* Update current Port ( Curr IP update happens outside function ) */
   *pCurrPort = newPort;
}

/***********************************************************************************
** FUNCTION:   cmSesHandleMediaFirewallAccess
**
** PURPOSE:    Updates firewall setttings for negotatied media.
**
** PARAMETERS: pSes - the session resource with negotiated media.
**             media - the media information.
**
** RETURNS:
**
** NOTE:
**
***********************************************************************************/
void cmSesHandleMediaFirewallAccess( CMSES * pSes, CMGRMEDINFO * media )
{
   if( gCmCfgCb.cmgrFwCb != NULL && pSes )
   {
      /* Update firewall ports based on port/IP changes, update cached used ports fields */
      cmSesHandleMediaFwPort( pSes->uHdl, &pSes->sesMedFwInfo.localRtpPort,  ( media ? media->media.egress.audio.codec[0].uRtpPort : 0 ),
                              &pSes->sesMedFwInfo.remoteAddr, ( media ? media->media.ingress.addr:pSes->sesMedFwInfo.remoteAddr ) );

      cmSesHandleMediaFwPort( pSes->uHdl, &pSes->sesMedFwInfo.localRtcpPort, ( media ? media->media.egress.audio.codec[0].uRtcpPort : 0 ),
                              &pSes->sesMedFwInfo.remoteAddr, ( media ? media->media.ingress.addr:pSes->sesMedFwInfo.remoteAddr ) );

      cmSesHandleMediaFwPort( pSes->uHdl, &pSes->sesMedFwInfo.localT38Port,  ( media ? media->media.egress.t38.uPort : 0 ),
                              &pSes->sesMedFwInfo.remoteAddr, ( media ? media->media.ingress.addr:pSes->sesMedFwInfo.remoteAddr ) );

      /* Update cached used ip-address field */
      if( !media || ( (media->media.egress.audio.codec[0].uRtpPort  == 0) &&
                      (media->media.egress.audio.codec[0].uRtcpPort == 0) &&
                      (media->media.egress.t38.uPort == 0) ) )
      {
         /* Clear ip-address field */
         memset( &pSes->sesMedFwInfo.remoteAddr, 0, sizeof(BOS_IP_ADDRESS) );
      }
      else
      {
         /* Set ip-address field now that we have handled all port changes */
         bosIpAddrCreateCopy(&media->media.ingress.addr, &pSes->sesMedFwInfo.remoteAddr);
      }
   }
}
#endif /* CMGRCFG_FIREWALL_CONTROL */
