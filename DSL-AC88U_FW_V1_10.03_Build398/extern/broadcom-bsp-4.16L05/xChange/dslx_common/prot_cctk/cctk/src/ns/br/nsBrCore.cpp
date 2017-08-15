/**********************************************************************************
** Copyright (c) 2009-2012 Broadcom Corporation
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
**      This file implements the core functionality of the Backup and Recovery
**      module.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <nsDlm.h>
#include <nsBr.h>

#if CCTKCFG_PROXY_FAILOVER

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* Default backup and recovery backoff timer value in seconds.
*/
static const BOS_UINT32 guNsBrBackoffTmr = 300;


/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   NsBr::NsBr
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
NsBr::NsBr()
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < NS_BR_MAX_MAP ; uIx++ )
   {
      mpNsBrMapBlk [ uIx ] = BRCM_NEW( NSBRMAPBLK );
      if ( mpNsBrMapBlk [ uIx ] )
      {
         memset ( mpNsBrMapBlk [ uIx ],
                  0,
                  sizeof ( NSBRMAPBLK ) );
         mpNsBrMapBlk [ uIx ]->uGuid = guCeInvalidGuid;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_BR_USR ; uIx++ )
   {
      mpNsBrBlk [ uIx ] = BRCM_NEW( NSBRBLK );
      if ( mpNsBrBlk [ uIx ] )
      {
         memset ( mpNsBrBlk [ uIx ],
                  0,
                  sizeof ( NSBRBLK ) );
         mpNsBrBlk [ uIx ]->uGuid = guCeInvalidGuid;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::~NsBr
**
**  PURPOSE:    Class Destructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
NsBr::~NsBr()
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;

   for ( uIx = 0 ; uIx < NS_BR_MAX_MAP ; uIx++ )
   {
      if ( mpNsBrMapBlk [ uIx ] )
      {
         BRCM_DELETE ( mpNsBrMapBlk [ uIx ] );
         mpNsBrMapBlk [ uIx ] = NULL;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_BR_USR ; uIx++ )
   {
      if ( mpNsBrBlk [ uIx ] )
      {
         for ( uJx = 0 ; uJx < mpNsBrBlk [ uIx ]->uItemCnt ; uJx++ )
         {
            if ( mpNsBrBlk [ uIx ]->nsBr[ uJx ].host.pcAddr )
            {
               BRCM_DELETE_ARRAY( mpNsBrBlk [ uIx ]->nsBr[ uJx ].host.pcAddr );
               mpNsBrBlk [ uIx ]->nsBr[ uJx ].host.pcAddr = NULL;
            }
         }
         
         mpNsBrBlk [ uIx ]->nsBrRestrict.restrictSockAddr.Reset();
         mpNsBrBlk [ uIx ]->prefPrimSockAddr.Reset();

         BRCM_DELETE ( mpNsBrBlk [ uIx ] );
         mpNsBrBlk [ uIx ] = NULL;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrGetOpq
**
**  PURPOSE:    Retrieves an opaque information associated with the GUID
**              parameter.
**
**  PARAMETERS:
**              uGuid - the GUID of interest.
**              opq - reference to the opaque to retrieve.
**
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrGetOpq( BOS_UINT32 uGuid,
                           mxt_opaque &opq  )
{
   BOS_BOOL bRet = BOS_FALSE;
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < NS_BR_MAX_MAP ; uIx++ )
   {
      if ( mpNsBrMapBlk [ uIx ] &&
           ( mpNsBrMapBlk [ uIx ]->uGuid != guCeInvalidGuid ) &&
           ( mpNsBrMapBlk [ uIx ]->uGuid == uGuid ) )
      {
         opq = mpNsBrMapBlk [ uIx ]->opq;
         
         bRet = BOS_TRUE;
         break;
      }
   }

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrSetOpq
**
**  PURPOSE:    Associates an opaque information with a GUID resource.
**
**  PARAMETERS:
**              uGuid - the GUID of interest.
**              opq - the opaque informaiton to set.
**
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
void NsBr::nsBrSetOpq( BOS_UINT32 uGuid,
                       mxt_opaque opq  )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uFreeIx = guCeInvalidGuid;

   for ( uIx = 0 ; uIx < NS_BR_MAX_MAP ; uIx++ )
   {
      if ( mpNsBrMapBlk [ uIx ] &&
           ( mpNsBrMapBlk [ uIx ]->uGuid != guCeInvalidGuid ) &&
           ( mpNsBrMapBlk [ uIx ]->uGuid == uGuid ) )
      {
         /* Found it, update the opaque.
         */
         mpNsBrMapBlk [ uIx ]->opq = opq;
         break;
      }
      else if ( mpNsBrMapBlk [ uIx ] &&
                ( mpNsBrMapBlk [ uIx ]->uGuid == guCeInvalidGuid ) &&
                ( uFreeIx == guCeInvalidGuid ) )
      {
         uFreeIx = uIx;
      }
   }

   /* Not located, but a free index is available, likely this is a newly
   ** created context, if so set is up accordingly.
   */
   if ( ( uIx == NS_BR_MAX_MAP ) &&
        ( uFreeIx != guCeInvalidGuid ) )
   {
      mpNsBrMapBlk [ uFreeIx ]->uGuid = uGuid;
      mpNsBrMapBlk [ uFreeIx ]->opq = opq;
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrGetBlk
**
**  PURPOSE:    Associate the resource identifier with the actual resource block
**              containing the information regarding the backup and recovery data
**              for that resource.
**
**  PARAMETERS:
**              uGuid - the GUID of interest.
**
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
NSBRBLK *NsBr::nsBrGetBlk ( BOS_UINT32 uGuid )
{
   BOS_UINT32 uIx;
   NSBRBLK *pBlk = NULL;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_BR_USR ; uIx++ )
   {
      if ( ( uGuid != guCeInvalidGuid ) &&
           ( mpNsBrBlk [ uIx ]->uGuid == uGuid ) )
      {
         pBlk = mpNsBrBlk [ uIx ];
         break;
      }
   }

   return pBlk;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrClean
**
**  PURPOSE:    Dissociates a resource block from the resource.  This should happen
**              as part of the framework when the resource in question has been
**              terminated.
**
**  PARAMETERS:
**              uGuid - the GUID of interest.
**
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
void NsBr::nsBrClean ( BOS_UINT32 uGuid )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < NS_BR_MAX_MAP ; uIx++ )
   {
      if ( mpNsBrMapBlk [ uIx ] &&
           ( mpNsBrMapBlk [ uIx ]->uGuid != guCeInvalidGuid ) &&
           ( mpNsBrMapBlk [ uIx ]->uGuid == uGuid ) )
      {
         /* Free the resource block.  It can now be reused.
         */
         mpNsBrMapBlk [ uIx ]->uGuid = guCeInvalidGuid;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrFreeResBlk
**
**  PURPOSE:    Checks whether we have free block information to associate a
**              resource with.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    BOS_TRUE on success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrFreeResBlk( void )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < NS_BR_MAX_MAP ; uIx++ )
   {
      if ( mpNsBrMapBlk [ uIx ] &&
           ( mpNsBrMapBlk [ uIx ]->uGuid == guCeInvalidGuid ) )
      {
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrExists
**
**  PURPOSE:    Checks whether a resource block already exists for this GUID.
**
**  PARAMETERS:
**              uGuid - the resource GUID to check against.
**
**
**  RETURNS:    BOS_TRUE on success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrExists( BOS_UINT32 uGuid )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < NS_BR_MAX_MAP ; uIx++ )
   {
      if ( mpNsBrMapBlk [ uIx ] &&
           ( mpNsBrMapBlk [ uIx ]->uGuid == uGuid ) )
      {
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrRegister
**
**  PURPOSE:    Registers a given user to make use of the backup and recovery
**              functionality.  Only registered user can use this service, only
**              CCTK 'user' resource can register for this service.
**
**  PARAMETERS:
**              uGuid - the GUID of interest.
**
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrRegister( BOS_UINT32 uGuid )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;
   BOS_UINT32 uFreeIx = guCeInvalidGuid;

   /* Only CCTK user resource can directly register for this feature.  Other
   ** resources are bound to the user itself which created the resource (the
   ** parent) and they will therefore automatically be applied the selection
   ** made by the user resource.
   */
   if ( cfgGetType ( uGuid ) != eCFGTYPE_USR )
   {
      CCTK_TRACEI1( "nsBrRegister - fail! 0x%X not user resource",
                    uGuid );
      return BOS_FALSE;
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_BR_USR ; uIx++ )
   {
      if ( mpNsBrBlk [ uIx ] &&
           ( uGuid != guCeInvalidGuid ) &&
           ( mpNsBrBlk [ uIx ]->uGuid == uGuid ) )
      {
         /* Already registered user resource.
         */
         return BOS_TRUE;
      }
      else if ( mpNsBrBlk [ uIx ] &&
                ( mpNsBrBlk [ uIx ]->uGuid == guCeInvalidGuid ) &&
                ( uFreeIx == guCeInvalidGuid ) )
      {
         uFreeIx = uIx;
      }
   }

   if ( uFreeIx != guCeInvalidGuid )
   {
      CCTK_TRACEI1( "nsBrRegister - success (0x%X)",
                    uGuid );
      
      /* Remove any previous knowledge from the block information.  The following
      ** should not be necessary, but just in case.
      */
      for ( uJx = 0 ; uJx < mpNsBrBlk [ uFreeIx ]->uItemCnt ; uJx++ )
      {
         if ( mpNsBrBlk [ uFreeIx ]->nsBr[ uJx ].host.pcAddr )
         {
            BRCM_DELETE_ARRAY( mpNsBrBlk [ uFreeIx ]->nsBr[ uJx ].host.pcAddr );
            mpNsBrBlk [ uFreeIx ]->nsBr[ uJx ].host.pcAddr = NULL;
         }
      }

      mpNsBrBlk [ uFreeIx ]->nsBrRestrict.restrictSockAddr.Reset();
      mpNsBrBlk [ uFreeIx ]->prefPrimSockAddr.Reset();
      memset ( mpNsBrBlk [ uFreeIx ],
               0,
               sizeof ( NSBRBLK ) );
      /* Assign.
      */
      mpNsBrBlk [ uFreeIx ]->uGuid = uGuid;

      return BOS_TRUE;
   }
   else
   {
      CCTK_TRACEI1( "nsBrRegister - fail! no more resource for 0x%X",
                    uGuid );
      return BOS_FALSE;
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrDeRegister
**
**  PURPOSE:    De-registers a given user from using the backup and recovery
**              functionality.
**
**  PARAMETERS:
**              uGuid - the GUID of interest.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void NsBr::nsBrDeRegister( BOS_UINT32 uGuid )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_BR_USR ; uIx++ )
   {
      if ( mpNsBrBlk [ uIx ] &&
           ( uGuid != guCeInvalidGuid ) &&
           ( mpNsBrBlk [ uIx ]->uGuid == uGuid ) )
      {
         /* Free up the block.
         */
         mpNsBrBlk [ uIx ]->uGuid = guCeInvalidGuid;
         mpNsBrBlk [ uIx ]->nsBrRestrict.restrictSockAddr.Reset();
         mpNsBrBlk [ uIx ]->prefPrimSockAddr.Reset();

         /* Free up allocated information associated to this block.
         */
         for ( uJx = 0 ; uJx < mpNsBrBlk [ uIx ]->uItemCnt ; uJx++ )
         {
            if ( mpNsBrBlk [ uIx ]->nsBr[ uJx ].host.pcAddr )
            {
               BRCM_DELETE_ARRAY( mpNsBrBlk [ uIx ]->nsBr[ uJx ].host.pcAddr );
               mpNsBrBlk [ uIx ]->nsBr[ uJx ].host.pcAddr = NULL;
            }
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrSelectDest
**
**  PURPOSE:    Selects the destination from the passed in list of possible
**              destinations that we want to use for this resource signaling path.
**
**  PARAMETERS:
**              uUsrGuid - the user resource which information is to be used
**                         to apply this process against.
**              uGuid - the resource GUID to apply this process against.
**              rFqdn - the FQDN that this callback is being invoked for.
**              rlstNaptrRecord - the set of resolved records which correspond to
**                                the DNS resolution of the 'rFqdn' information.
**
**
**  RETURNS:    Nothing, however the 'rlstNaptrRecord' is updated in consequence.
**
**  NOTES:      This is the core functionality from the Backup and Recovery
**              module which picks the right target based on the currently known
**              information set.
***********************************************************************************/
void NsBr::nsBrSelectDest ( BOS_UINT32 uUsrGuid,
                            BOS_UINT32 uGuid,
                            const MXD_GNS::CHostPort& rFqdn,
                            MXD_GNS::CList<MXD_GNS::SNaptrRecord>
                                                        &rlstNaptrRecord )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uUsrGuid );

   CCTK_TRACED2( "NsBr::nsBrSelectDest - 0x%lX (0x%lX)",
                 uUsrGuid, uGuid );

   /* If we cannot locate the corresponding block information to apply the
   ** backup and recovery feature to this resource, simply exit now.
   */
   if ( pBrBlk == NULL )
   {
      CCTK_TRACEI2( "nsBrSelectDest - no backup-recovery info for 0x%X (0x%X)",
                    uUsrGuid, uGuid );
      return;
   }

   /* The destination is associated with the user resource.  We need to validate
   ** the content of the record list against our internal knowledge for this user
   ** first, then we apply the actual selection process.
   **
   ** For other resource (non user), this step is not necessary because we just
   ** need to grab the destination information which is managed by the associated
   ** user of this resource (i.e. its parent).
   */
   if ( uGuid == uUsrGuid )
   {
      nsBrManage ( pBrBlk, rFqdn, rlstNaptrRecord );
   }

   /* Apply the actual modification to the record list such that the stack
   ** knows what to do.
   */
   if ( pBrBlk->uItemCnt )
   {
      switch ( pBrBlk->eStatus )
      {
         /* We are in failure mode, prevent any target selection until
         ** some other process clears us up from this state.
         */
         case eNSBRSTA_FAILED:
         {
            rlstNaptrRecord.EraseAll();
         }
         break;

         /* Select the most appropriate target from our list.
         */
         default:
         {
            pBrBlk->eStatus = eNSBRSTA_IDLE;
            nsBrChoose ( uUsrGuid, uGuid, pBrBlk, rlstNaptrRecord );
         }
         break;
      }
   }
   else
   {
      pBrBlk->eStatus = eNSBRSTA_FAILED;
      CCTK_TRACEI2( "nsBrSelectDest - no destination for 0x%X (0x%X)",
                    uUsrGuid, uGuid );
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrManage
**
**  PURPOSE:    Given a backup and recovery information block and given the set of
**              records to be associated with the destination, manage the backup
**              and recovery module knowledge in order to ensure we always select
**              the proper target according to the expectations of the feature.
**
**  PARAMETERS:
**              pBrBlk - the backup and recovery block information.
**              rFqdn - the FQDN that this callback is being invoked for.
**              rlstNaptrRecord - the set of resolved records which correspond to
**                                the DNS resolution of the 'rFqdn' information.
**
**
**  RETURNS:    Nothing, however the content (item list) associated with the BR
**              block is updated according to the 'rlstNaptrRecord'.
**
**  NOTES:      
***********************************************************************************/
void NsBr::nsBrManage ( NSBRBLK *pBrBlk,
                        const MXD_GNS::CHostPort& rFqdn, 
                        MXD_GNS::CList<MXD_GNS::SNaptrRecord> &rlstNaptrRecord )
{
   /* Note that we do not care about the entity being resolved, we always apply
   ** the same principles in all cases, at least in the current architecture.
   */
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rFqdn );

   BOS_UINT32 uIx;
   BOS_UINT32 uJx;
   BOS_UINT32 uNaptrCurIx;
   BOS_UINT32 uSrvCurIx;
   BOS_UINT32 uSocketCurIx;
   BOS_UINT16 uDefaultPort;
   BOS_BOOL bInsert;
   BOS_UINT8 uAddress[ CCTK_STR_LEN ];
   BOS_UINT32 uCapacity = CCTK_STR_LEN;
   NSBRBLK tmpBlk;
   MXD_GNS::ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res;
   BOS_BOOL bProcessRecord = BOS_TRUE;

   memset ( &tmpBlk,
            0,
            sizeof ( NSBRBLK ) );

   /* Create a temporary block containing the relevant information we want
   ** to manage.
   */
   CCTK_TRACEI2( "nsBrManage - %d NAPTR record(s) for 0x%lX",
                 rlstNaptrRecord.GetSize(),
                 pBrBlk->uGuid );
   for ( uNaptrCurIx = 0 ;
         ( uNaptrCurIx < rlstNaptrRecord.GetSize() ) && ( bProcessRecord == BOS_TRUE ) ;
         uNaptrCurIx++ )
   {
      MXD_GNS::CList<MXD_GNS::SSrvRecord>& rlstSrv =
                     rlstNaptrRecord[ uNaptrCurIx ].lstSrvRecord;
      CCTK_TRACEI3( "nsBrManage - %d SRV record(s) for NAPTR-%d on 0x%lX",
                    rlstSrv.GetSize(),
                    uNaptrCurIx,
                    pBrBlk->uGuid );
      for ( uSrvCurIx = 0 ;
            ( uSrvCurIx < rlstSrv.GetSize() ) && ( bProcessRecord == BOS_TRUE );
            uSrvCurIx++ )
      {
         MXD_GNS::CList<MXD_GNS::CSocketAddr>& rlstAddress =
                                       rlstSrv[ uSrvCurIx ].lstSocketAddr;
         uDefaultPort = rlstSrv[ uSrvCurIx ].uPort;
         CCTK_TRACEI4( "nsBrManage - %d A record(s) for NAPTR-%d/SRV-%d on 0x%lX",
                       rlstAddress.GetSize(),
                       uNaptrCurIx,
                       uSrvCurIx,
                       pBrBlk->uGuid );
         for ( uSocketCurIx = 0 ;
               ( uSocketCurIx < rlstAddress.GetSize() ) && ( bProcessRecord == BOS_TRUE );
               uSocketCurIx++ )
         {
            MXD_GNS::CSocketAddr& rCurrentSocket =
                                          rlstAddress[ uSocketCurIx ];
            if ( rCurrentSocket.GetPort() == 0 )
            {
               rCurrentSocket.SetPort( uDefaultPort );
            }

            bInsert = BOS_TRUE;
            for ( uIx = 0 ; uIx < tmpBlk.uItemCnt ; uIx++ )
            {
               if ( nsBrEqual ( &tmpBlk,
                                uIx,
                                rCurrentSocket ) )
               {
                  rCurrentSocket.GetAddress( static_cast<unsigned int> (uCapacity),
                                             reinterpret_cast<char *> (uAddress) );
                  CCTK_TRACEI3( "nsBrManage - tmp-rejected %s:%d for 0x%lX",
                                reinterpret_cast<const char *> (uAddress),
                                static_cast<uint16_t> (rCurrentSocket.GetPort()),
                                pBrBlk->uGuid );

                  bInsert = BOS_FALSE;
               }
            }

            if ( bInsert )
            {
               rCurrentSocket.GetAddress( static_cast<unsigned int> (uCapacity),
                                          reinterpret_cast<char *> (uAddress) );
               tmpBlk.nsBr[ tmpBlk.uItemCnt ].host.pcAddr =
                  utilStrDup ( reinterpret_cast<const BOS_UINT8 *> (uAddress) );
               tmpBlk.nsBr[ tmpBlk.uItemCnt ].host.uPort =
                           static_cast<BOS_UINT16> (rCurrentSocket.GetPort());
               tmpBlk.nsBr[ tmpBlk.uItemCnt ].bPrimary = BOS_FALSE;
               tmpBlk.nsBr[ tmpBlk.uItemCnt ].eState = eNSBRSTATE_IDLE;

               CCTK_TRACEI3( "nsBrManage - tmp-inserted %s:%d for 0x%lX",
                             reinterpret_cast<const char *> (
                                       tmpBlk.nsBr[ tmpBlk.uItemCnt ].host.pcAddr),
                             static_cast<uint16_t> (tmpBlk.nsBr[ tmpBlk.uItemCnt ].host.uPort),
                             pBrBlk->uGuid );

               /* Increment the counter.
               */
               tmpBlk.uItemCnt++;
               /* Only take up to CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS address entries */
               if ( tmpBlk.uItemCnt == CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS )
               {
                  CCTK_TRACEI1( "nsBrManage - stop processing addition. Reached max records %d.", 
                        CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS );
                  bProcessRecord = BOS_FALSE;
               }
            }
         }
      }
   }

   /* Create a new intersection list of the new targets and current targets (if
   ** any).
   ** 
   ** We simply take the new list and use that as base of the processing but we
   ** transfer over the states information from the current list.
   */
   for ( uIx = 0 ; uIx < tmpBlk.uItemCnt ; uIx++ )
   {
      for ( uJx = 0 ; uJx < pBrBlk->uItemCnt ; uJx++ )
      {
         if ( nsBrEqual ( &tmpBlk,
                          uIx,
                          pBrBlk,
                          uJx ) )
         {
            tmpBlk.nsBr[ uIx ].bPrimary = pBrBlk->nsBr[ uJx ].bPrimary;  
            tmpBlk.nsBr[ uIx ].eState = pBrBlk->nsBr[ uJx ].eState;  
         }
      }
   }

   /* Check for targets that may have been blacklisted by the blacklist
   ** stack service.  The service is run prior to the backup and recovery service
   ** being run and it may have modified the record list according to its purpose
   ** and knowledge, however the backup and recovery service should be the one
   ** deciding what to do with targets that are unreachable or otherwise under
   ** blacklist.  Therefore we may want to override the blacklist decision from
   ** the other service.
   */
   res = CreateEComInstance( MXD_GNS::CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_S( res ) && ( pCoreCfg != NULL ) )
   {
      MXD_GNS::CSipConnectionBlacklist *pBlkLst =
                           pCoreCfg->GetConnectionBlacklistInstance();

      if ( pBlkLst )
      {
         for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
         {
            BOS_BOOL bTrUdp = BOS_FALSE;
            BOS_BOOL bTrTcp = BOS_FALSE;
            BOS_BOOL bTrTls = BOS_FALSE;
            MXD_GNS::CSocketAddr
               socketAddr (
                  reinterpret_cast<const char *> (pBrBlk->nsBr[ uIx ].host.pcAddr),
                  static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort) );

            CCTK_TRACEI3( "nsBrManage - black list check for %s:%d on 0x%lX",
                          reinterpret_cast<const char *> (pBrBlk->nsBr[ uIx ].host.pcAddr),
                          static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort),
                          pBrBlk->uGuid );

            /* Note: We do not distinguish transport at the backup and recovery
            **       service layer, so any transport should be checked for.
            */
            if ( ((bTrUdp =
                     static_cast<BOS_BOOL> (
                           pBlkLst->IsBlacklisted( socketAddr, MXD_GNS::eUDP )))
                                                                     == BOS_TRUE) ||
                 ((bTrTcp =
                     static_cast<BOS_BOOL> (
                           pBlkLst->IsBlacklisted( socketAddr, MXD_GNS::eTCP )))
                                                                     == BOS_TRUE) ||
                 ((bTrTls =
                     static_cast<BOS_BOOL> (
                           pBlkLst->IsBlacklisted( socketAddr, MXD_GNS::eTLS )))
                                                                     == BOS_TRUE) )
            {
               CCTK_TRACEI3( "nsBrManage - blacklisted %s:%d on 0x%lX",
                             reinterpret_cast<const char *> (pBrBlk->nsBr[ uIx ].host.pcAddr),
                             static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort),
                             pBrBlk->uGuid );

               /* Remove from the blacklist such that it does not interfere
               ** with the intent of the failover feature.
               */
               if ( bTrUdp )
               {
                  pBlkLst->RemoveFromBlacklist( socketAddr, MXD_GNS::eUDP );
               }

               if ( bTrTcp )
               {
                  pBlkLst->RemoveFromBlacklist( socketAddr, MXD_GNS::eTCP );
               }

               if ( bTrTls )
               {
                  pBlkLst->RemoveFromBlacklist( socketAddr, MXD_GNS::eTLS );
               }

               /* Because the target was blacklisted and because the black list
               ** service is run before the failover service, the target would not
               ** have appeared in the target list associated with this request,
               ** in such case, we need to see whether to re-add such as part of
               ** the failover information block.
               */
               bInsert = BOS_TRUE;
               for ( uJx = 0 ; uJx < tmpBlk.uItemCnt ; uJx++ )
               {
                  if ( nsBrEqual ( &tmpBlk,
                                   uJx,
                                   socketAddr ) )
                  {
                     socketAddr.GetAddress( static_cast<unsigned int> (uCapacity),
                                            reinterpret_cast<char *> (uAddress) );
                     CCTK_TRACEI3( "nsBrManage - bl-rejected %s:%d for 0x%lX",
                                   reinterpret_cast<const char *> (uAddress),
                                   static_cast<uint16_t> (socketAddr.GetPort()),
                                   pBrBlk->uGuid );

                     bInsert = BOS_FALSE;
                  }
               }

               if ( bInsert && ( bProcessRecord == BOS_TRUE ) )
               {
                  tmpBlk.nsBr[ tmpBlk.uItemCnt ].host.pcAddr =
                              utilStrDup ( pBrBlk->nsBr[ uIx ].host.pcAddr );
                  tmpBlk.nsBr[ tmpBlk.uItemCnt ].host.uPort =
                                             pBrBlk->nsBr[ uIx ].host.uPort;
                  tmpBlk.nsBr[ tmpBlk.uItemCnt ].bPrimary =
                                             pBrBlk->nsBr[ uIx ].bPrimary;
                  tmpBlk.nsBr[ tmpBlk.uItemCnt ].eState =
                                             pBrBlk->nsBr[ uIx ].eState;

                  CCTK_TRACEI3( "nsBrManage - bl-inserted %s:%d for 0x%lX",
                                reinterpret_cast<const char *> (
                                          tmpBlk.nsBr[ tmpBlk.uItemCnt ].host.pcAddr),
                                static_cast<uint16_t> (tmpBlk.nsBr[ tmpBlk.uItemCnt ].host.uPort),
                                pBrBlk->uGuid );

                  /* Increment the counter.
                  */
                  tmpBlk.uItemCnt++;
                  /* Only take up to CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS address entries */
                  if ( tmpBlk.uItemCnt == CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS )
                  {
                     CCTK_TRACEI1( "nsBrManage - stop inserting black list. Reached max records %d.", 
                           CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS );
                     bProcessRecord = BOS_FALSE;
                  }
               }
            }
         }
      }

      pCoreCfg->ReleaseIfRef();
      pCoreCfg = NULL;
   }

   /* If we had information already present in the current list, clean it
   ** up now so we can recreate it new.
   */
   if ( pBrBlk->uItemCnt )
   {
      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         CCTK_TRACEI3( "nsBrManage - removing %s:%d for 0x%lX",
                       pBrBlk->nsBr[ uIx ].host.pcAddr ?
                           reinterpret_cast<const char *> (pBrBlk->nsBr[ uIx ].host.pcAddr) :
                           "(null)",
                       static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort),
                       pBrBlk->uGuid );

         if ( pBrBlk->nsBr[ uIx ].host.pcAddr )
         {
            BRCM_DELETE_ARRAY( pBrBlk->nsBr[ uIx ].host.pcAddr );
            pBrBlk->nsBr[ uIx ].host.pcAddr = NULL;
         }
      }

      /* Reset the list.
      */
      pBrBlk->uItemCnt = 0;
   }

   /* Copy over the new information.
   */
   for ( uIx = 0 ; uIx < tmpBlk.uItemCnt ; uIx++ )
   {
      pBrBlk->nsBr[ pBrBlk->uItemCnt ].host.pcAddr =
                  utilStrDup ( tmpBlk.nsBr[ uIx ].host.pcAddr );
      pBrBlk->nsBr[ pBrBlk->uItemCnt ].host.uPort =
                  tmpBlk.nsBr[ uIx ].host.uPort;
      pBrBlk->nsBr[ pBrBlk->uItemCnt ].bPrimary = tmpBlk.nsBr[ uIx ].bPrimary;
      pBrBlk->nsBr[ pBrBlk->uItemCnt ].eState = tmpBlk.nsBr[ uIx ].eState;

      CCTK_TRACEI2( "nsBrManage - full-item %d for 0x%lX",
                    pBrBlk->uItemCnt,
                    pBrBlk->uGuid );
      CCTK_TRACEI4( "nsBrManage - %s:%d:%s:%d",
                    reinterpret_cast<const char *> (
                              pBrBlk->nsBr[ pBrBlk->uItemCnt ].host.pcAddr),
                    static_cast<uint16_t> (pBrBlk->nsBr[ pBrBlk->uItemCnt ].host.uPort),
                    pBrBlk->nsBr[ pBrBlk->uItemCnt ].bPrimary ? "Primary" : "Secondary",
                    pBrBlk->nsBr[ pBrBlk->uItemCnt ].eState );

      pBrBlk->uItemCnt++;
   }
   
   /* Setup 'Primary' and 'Active' targets in case there are none which could
   ** be the case if either:
   **
   **   - The current block was originally empty (new user registered with backup
   **     and recovery feature)
   **   - Some targets that were present before in the backup and recovery block
   **     are no longer valid and have been eliminated by the process above.
   */
   BOS_UINT32 uActiveIx = guCeInvalidGuid;
   BOS_UINT32 uPrimaryIx = guCeInvalidGuid;
   
   for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
   {
      if ( pBrBlk->nsBr[ uIx ].eState == eNSBRSTATE_ACTIVE )
      {
         uActiveIx = uIx;
      }

      if ( pBrBlk->nsBr[ uIx ].bPrimary )
      {
         uPrimaryIx = uIx;
      }
   }

   /* No active target, select one.
   */
   if ( uActiveIx == guCeInvalidGuid )
   {
      /* Choose the primary if one is available.
      */
      if ( uPrimaryIx != guCeInvalidGuid )
      {
         pBrBlk->nsBr[ uPrimaryIx ].eState = eNSBRSTATE_ACTIVE;
         uActiveIx = uPrimaryIx;
      }
      /* Else, promote the first 'Idle'.
      */
      else
      {
         for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
         {
            if ( pBrBlk->nsBr[ uIx ].eState == eNSBRSTATE_IDLE )
            {
               pBrBlk->nsBr[ uIx ].eState = eNSBRSTATE_ACTIVE;
               uActiveIx = uIx;
               break;
            }
         }
      }
   }

   /* No primary target, transform the 'Active' target as 'Primary' one.
   **
   ** Note that the active target should have been determined by the process above
   ** already.  If none is present at this time, we have some bigger problems.
   */
   if ( ( uPrimaryIx == guCeInvalidGuid ) &&
        ( uActiveIx != guCeInvalidGuid ) )
   {
      pBrBlk->nsBr[ uActiveIx ].bPrimary = BOS_TRUE;
   }

   /* Clean up the temporary block that was created.
   */
   for ( uIx = 0 ; uIx < tmpBlk.uItemCnt ; uIx++ )
   {
      BRCM_DELETE_ARRAY( tmpBlk.nsBr[ uIx ].host.pcAddr );
      tmpBlk.nsBr[ uIx ].host.pcAddr = NULL;
   }

   /* If there are still no active targets at this point, it means we are in a
   ** failure scenario where we have no possible non failed targets, and/or no
   ** new targets to attempt at all.
   */
   if ( uActiveIx == guCeInvalidGuid )
   {
      pBrBlk->eStatus = eNSBRSTA_FAILED;
   }
   else
   {
      CCTKTARGETINFO    targetInfo;

      /* Since we have an updated list of targets, inform the application of 
      ** this change.
      */
      memset( &targetInfo, 0, sizeof( CCTKTARGETINFO ) );
      targetInfo.uNumTargets = static_cast<BOS_UINT8> (pBrBlk->uItemCnt);

      for ( uIx = 0; uIx < pBrBlk->uItemCnt; uIx++ )
      {
         targetInfo.uTargetPort[uIx] = pBrBlk->nsBr[uIx].host.uPort;
         bosIpAddrCreateFromStr ( reinterpret_cast< const char* >( pBrBlk->nsBr[uIx].host.pcAddr ),
                                  &targetInfo.targetipAddr[uIx] );
      }

      tkmEvent( pBrBlk->uGuid, 
                guCeInvalidGuid,
                eCCTKEVT_TARGETS_UPDATE, 
                reinterpret_cast<void *> ( &targetInfo ) );
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrChoose
**
**  PURPOSE:    Reduce the passed in list of possible record (target) down to a
**              single entity based on the information about the backup and
**              recovery block.
**
**  PARAMETERS:
**              uUsrGuid - the user resource which information is to be used
**                         to apply this process against.
**              uGuid - the resource GUID to apply this process against.
**              pBrBlk - the backup and recovery block information.
**              rlstNaptrRecord - the set of resolved records which correspond to
**                                the DNS resolution of the 'rFqdn' information.
**
**
**  RETURNS:    Nothing, however the 'rlstNaptrRecord' list of records is reduced
**              to a single entity, this will force the stack to use that target
**              no matter what.
**
**  NOTES:      
***********************************************************************************/
void NsBr::nsBrChoose ( BOS_UINT32 uUsrGuid,
                        BOS_UINT32 uGuid,
                        NSBRBLK *pBrBlk,
                        MXD_GNS::CList<MXD_GNS::SNaptrRecord> &rlstNaptrRecord )
{
   BOS_UINT32 uIx = guCeInvalidGuid;
   BOS_UINT32 uSelectIx = guCeInvalidGuid;
   BOS_BOOL bSelect = BOS_FALSE;
   MXD_GNS::CSocketAddr *pResSigAddr = NULL;

   /* First need to check if this is a non-user resource with signalling 
   ** restrictions. If so, we may need to pick the specified target 
   ** instead of the one assosciated to the user resource.
   */
   if ( uUsrGuid != uGuid &&
        ( pResSigAddr = ceGetResRstrictAddr( uGuid ) )!= NULL ) 
   {
      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         if ( nsBrEqual(pBrBlk, uIx, *pResSigAddr ) )
         {
            /* Selected the destination.
            */
            uSelectIx = uIx;
            bSelect = BOS_TRUE;
            
            CCTK_TRACEI3( "nsBrChoose - selection for 0x%lX is restricted to %s:%d ",
                          uGuid,
                          reinterpret_cast<const char *> (
                                    pBrBlk->nsBr[ uSelectIx ].host.pcAddr),
                          static_cast<uint16_t> (pBrBlk->nsBr[ uSelectIx ].host.uPort) );
                    
            break;
         }
      }
   }
   
   /* If non-user signalling restrictions didn't find an appropriate 
   ** selection, attempt to find one using other user associated means.
   */
   if ( !bSelect )
   {
      /* Check to see if there any restrictions for the block's selection process.
      */
      if ( pBrBlk->nsBrRestrict.eType !=  eNSBRRESTRICTIONTYPE_NONE)
      {
         /* Always select the restricted one and that one alone even if we know it may
         ** be a filed target.
         */
         for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
         {
            if ( ( pBrBlk->nsBrRestrict.eType == eNSBRRESTRICTIONTYPE_PRIMONLY && 
                   pBrBlk->nsBr[ uIx ].bPrimary ) ||
                     ( pBrBlk->nsBrRestrict.eType == eNSBRRESTRICTIONTYPE_ADDRSPECIFIED && 
                       nsBrEqual(pBrBlk, uIx, pBrBlk->nsBrRestrict.restrictSockAddr ) ))
            {
               pBrBlk->nsBr[ uIx ].eState = eNSBRSTATE_ACTIVE;
               /* Selected the destination.
               */
               uSelectIx = uIx;
               bSelect = BOS_TRUE;
               
               CCTK_TRACEI4( "nsBrChoose - selection for 0x%lX is restricted to %s - %s:%d ",
                          uGuid,
                          ( pBrBlk->nsBrRestrict.eType == eNSBRRESTRICTIONTYPE_PRIMONLY ) ? 
                              "primary" : "address", 
                          reinterpret_cast<const char *> (
                                    pBrBlk->nsBr[ uSelectIx ].host.pcAddr),
                          static_cast<uint16_t> (pBrBlk->nsBr[ uSelectIx ].host.uPort) );
            }
            else
            {
               pBrBlk->nsBr[ uIx ].eState = eNSBRSTATE_FAILED;
            }
         }
      }
      else
      {
         for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
         {
            if ( pBrBlk->nsBr[ uIx ].eState == eNSBRSTATE_ACTIVE )
            {
               /* Selected the destination.
               */
               uSelectIx = uIx;
               bSelect = BOS_TRUE;
               break;
            }
         }
      }
   }
   
   /* If no target was selected mark it invalid.
   */
   if ( !bSelect )
   {
      uSelectIx = guCeInvalidGuid;
   }
   else
   {
      CCTK_TRACEI3( "nsBrChoose - selected %s:%d from 0x%lX",
                    reinterpret_cast<const char *> (
                              pBrBlk->nsBr[ uSelectIx ].host.pcAddr),
                    static_cast<uint16_t> (pBrBlk->nsBr[ uSelectIx ].host.uPort),
                    pBrBlk->uGuid );
   }

   /* Eliminate all targets to only keep the unique one that we want to
   ** enforce.
   ** 
   ** This process is the single most important characteristic of the backup
   ** and recovery feature.
   */
   if ( bSelect )
   {
      BOS_SINT32 uNaptrCurIx = rlstNaptrRecord.GetSize() - 1;
      for ( ; uNaptrCurIx >= 0 ; uNaptrCurIx-- )
      {
         MXD_GNS::CString &rstrServices =
                  rlstNaptrRecord[ uNaptrCurIx ].strServices;
         MXD_GNS::ESipTransport eCurrentTransport = MXD_GNS::eINVALID;
         if ( rstrServices == MXD_GNS::szURIPARAM_TRANSPORT_UDP_NAPTR )
         {
            eCurrentTransport = MXD_GNS::eUDP;
         }
         else if ( rstrServices == MXD_GNS::szURIPARAM_TRANSPORT_TCP_NAPTR )
         {
            eCurrentTransport = MXD_GNS::eTCP;
         }
         else if ( rstrServices == MXD_GNS::szURIPARAM_TRANSPORT_TLS_NAPTR )
         {
            eCurrentTransport = MXD_GNS::eTLS;
         }

         MXD_GNS::CList<MXD_GNS::SSrvRecord>& rlstSrv =
                        rlstNaptrRecord[ uNaptrCurIx ].lstSrvRecord;
         BOS_SINT32 uSrvCurIx = rlstSrv.GetSize() - 1;
         for ( ; uSrvCurIx >= 0 ; uSrvCurIx-- )
         {
            BOS_UINT16 uDefaultPort = rlstSrv[ uSrvCurIx ].uPort;

            MXD_GNS::CList<MXD_GNS::CSocketAddr>& rlstAddress =
                                          rlstSrv[ uSrvCurIx ].lstSocketAddr;
            BOS_SINT32 uSocketCurIx = rlstAddress.GetSize() - 1;
            for ( ; uSocketCurIx >= 0 ; uSocketCurIx-- )
            {
               MXD_GNS::CSocketAddr& rCurrentSocket =
                                          rlstAddress[ uSocketCurIx ];
               if ( rCurrentSocket.GetPort() == 0 )
               {
                  rCurrentSocket.SetPort( uDefaultPort );
               }

               if ( !nsBrEqual ( pBrBlk,
                                 uSelectIx,
                                 rCurrentSocket,
                                 eCurrentTransport ) )
               {
                  rlstAddress.Erase( uSocketCurIx );
               }
            }

            /* Remove entries left with no data to show for.
            */
            if ( rlstAddress.GetSize() == 0 )
            {
               rlstSrv.Erase( uSrvCurIx );
            }
         }

         /* Remove entries left with no data to show for.
         */
         if ( rlstSrv.GetSize() == 0 )
         {
            rlstNaptrRecord.Erase( uNaptrCurIx );
         }
      }
   }

   /* At this point, we may need to check for additional target selection. Two
   ** cases are to be considered:
   **
   **  - There are no target left in the record list, but we have selected one
   **    from the backup and recovery block.  This is the case when a target has
   **    been blacklisted by the stack blacklist service and therefore would
   **    not have appeared in the passed in record list (the blacklist service
   **    is run prior to the backup and recovery one).  Note however that in such
   **    situation, the target is likely invalid (because it has been blacklisted
   **    for a reason) but if it is the one we are meant to choose because the
   **    backup and recovery service decided so, we need to honour this.
   **
   **  - There are no target left in the record list and we have not selected any
   **    other one from the backup and recovery block.  This is a strange situation
   **    as it would mean we eliminated all possible targets for some reason.  It
   **    could be the sign of some inconsistency between the backup and recovery
   **    service knowledge and other services from the stack.  Presumably in this
   **    case the request will fail anyways and the backup and recovery service will
   **    kick in and revive valid target later on.
   */
   if ( !rlstNaptrRecord.GetSize() )
   {
      if ( bSelect )
      {
         /* Create and insert the selected target.
         **
         ** Note that we only insert a 'udp' target in this case, this is the
         ** default protocol support so it should work.  When protocol tracking
         ** is added to the backup and recovery as needed, we can be more
         ** intelligent in that regards.
         */
         MXD_GNS::SNaptrRecord naptrRecord;
         MXD_GNS::SSrvRecord srvRecord;
         MXD_GNS::CSocketAddr socketAddr (
                reinterpret_cast<const char *> (
                        pBrBlk->nsBr[ uSelectIx ].host.pcAddr),
                static_cast<uint16_t> (pBrBlk->nsBr[ uSelectIx ].host.uPort) );

         srvRecord.uPort =
            static_cast<uint16_t> (pBrBlk->nsBr[ uSelectIx ].host.uPort);
         srvRecord.lstSocketAddr.Append( socketAddr );
         
         naptrRecord.strServices = MXD_GNS::szURIPARAM_TRANSPORT_UDP_NAPTR;         
         naptrRecord.lstSrvRecord.Append( srvRecord );
         
         rlstNaptrRecord.Append( naptrRecord );
      }
      else
      {
         CCTK_TRACEI1( "nsBrChoose - 0x%X removed all destinations!!",
                       pBrBlk->uGuid );
         pBrBlk->eStatus = eNSBRSTA_FAILED;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrEqual
**
**  PURPOSE:    Checks whether the socket information associated with a backup and
**              recovery block is equal (i.e. same target) to the one provided.
**
**  PARAMETERS:
**              pBrBlk - the backup and recovery block information.
**              uIx - the index of the active item from the backup and recovery
**                    block information.
**              rSocketAddr - the socket address to compare against.
**              eTransport - the transport type to compare against.
**
**
**  RETURNS:    BOS_TRUE if the entry located at the index specified corresponds
**              to the socket address passed in, BOS_FALSE otherwise.
**
**  NOTES:      
***********************************************************************************/
BOS_BOOL NsBr::nsBrEqual ( NSBRBLK *pBrBlk,
                           BOS_UINT32 uIx,
                           const MXD_GNS::CSocketAddr& rSocketAddr,
                           MXD_GNS::ESipTransport eTransport )
{
   /* Note: We ignore the transport type for the time being.
   */
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( eTransport );

   BOS_BOOL bEqual = BOS_FALSE;
   /* Create the socket address information for comparison.
   */
   MXD_GNS::CSocketAddr
      socketAddr ( reinterpret_cast<const char *> (pBrBlk->nsBr[ uIx ].host.pcAddr),
                   static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort) );

   /* Compare now...
   */
   if ( socketAddr.IsEqualAddress( rSocketAddr ) )
   {
      bEqual = BOS_TRUE;
   }
                   
   return bEqual;
}

/***********************************************************************************
**  FUNCTION:   NsBr::nsBrAddrEqual
**
**  PURPOSE:    Checks whether the socket address associated with a backup and
**              recovery block is equal (i.e. same target) to the one provided.
**
**  PARAMETERS:
**              pBrBlk - the backup and recovery block information.
**              uIx - the index of the active item from the backup and recovery
**                    block information.
**              rSocketAddr - the socket address to compare against.
**
**
**  RETURNS:    BOS_TRUE if the entry located at the index specified corresponds
**              to the socket address passed in, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrAddrEqual ( NSBRBLK *pBrBlk,
                               BOS_UINT32 uIx,
                               const MXD_GNS::CSocketAddr& rSocketAddr )
{
   BOS_BOOL bEqual = BOS_FALSE;
   /* Create the socket address information for comparison.
   */
   MXD_GNS::CSocketAddr
      socketAddr ( reinterpret_cast<const char *> (pBrBlk->nsBr[ uIx ].host.pcAddr),
                   static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort) );

   /* Compare now...
   */
   if ( socketAddr.IsEqualAddress( rSocketAddr ) )
   {
      bEqual = BOS_TRUE;
   }

   return bEqual;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrEqual
**
**  PURPOSE:    Checks whether two socket information associated with a backup and
**              recovery block are equal (i.e. same target).
**
**  PARAMETERS:
**              pBrBlk_1 - the first backup and recovery block information.
**              uIx_1 - the index of the item to compare from the backup and
**                      recovery block information.
**              pBrBlk_2 - the second backup and recovery block information.
**              uIx_2 - the index of the item to compare from the backup and
**                      recovery block information.
**              eTransport - the transport type to compare against.
**
**
**  RETURNS:    BOS_TRUE if both data information are equal, BOS_FALSE otherwise.
**
**  NOTES:      
***********************************************************************************/
BOS_BOOL NsBr::nsBrEqual ( NSBRBLK *pBrBlk_1,
                           BOS_UINT32 uIx_1,
                           NSBRBLK *pBrBlk_2,
                           BOS_UINT32 uIx_2,
                           MXD_GNS::ESipTransport eTransport )
{
   /* Note: we ignore the transport type for the time being.
   */
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( eTransport );

   BOS_BOOL bEqual = BOS_FALSE;
   
   if ( ( uIx_1 < pBrBlk_1->uItemCnt ) &&
        ( uIx_2 < pBrBlk_2->uItemCnt ) )
   {
      /* Create the socket address information for comparison.
      */
      MXD_GNS::CSocketAddr
         socketAddr_1 (
            reinterpret_cast<const char *> (pBrBlk_1->nsBr[ uIx_1 ].host.pcAddr),
            static_cast<uint16_t> (pBrBlk_1->nsBr[ uIx_1 ].host.uPort) );

      MXD_GNS::CSocketAddr
         socketAddr_2 (
            reinterpret_cast<const char *> (pBrBlk_2->nsBr[ uIx_2 ].host.pcAddr),
            static_cast<uint16_t> (pBrBlk_2->nsBr[ uIx_2 ].host.uPort) );

      /* Compare now...
      */
      if ( socketAddr_1 == socketAddr_2 )
      {
         bEqual = BOS_TRUE;
      }
   }

   return bEqual;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrStatus
**
**  PURPOSE:    Queries the status of the backup and recovery service for the
**              given resource of interest.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS NsBr::nsBrStatus( BOS_UINT32 uGuid )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_UINT32 uIx;
   NSBRSTATUS eStatus = eNSBRSTA_INVALID;

   if ( pBrBlk )
   {
      /* Assume failure.
      */
      if ( pBrBlk->uItemCnt )
      {
         eStatus = eNSBRSTA_FAILED;
      }

      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         if ( (pBrBlk->nsBr[ uIx ].eState == eNSBRSTATE_ACTIVE) ||
              (pBrBlk->nsBr[ uIx ].eState == eNSBRSTATE_IDLE) )
         {
            /* At least one target is active or idle, we can run the backup
            ** and recovery feature as intended.
            */
            eStatus = eNSBRSTA_IDLE;
            break;
         }
      }
   }

   return eStatus;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrBackupOnFail
**
**  PURPOSE:    Asks the backup and recovery module to provide a backup solution in
**              case of a failure detected.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**              bBkoffTmr - specifies whether the back off timer needs to assigned 
**                          if there are no available targets left.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:      This method will update the knowledge of the backup and recovery
**              target selection, it will in particular select the next target to
**              be attempted (if any is available) and it will demote the current
**              active target as no longer available.  Hence the application
**              invoking this method should be careful and aware of the choice.
***********************************************************************************/
NSBRSTATUS NsBr::nsBrBackupOnFail( BOS_UINT32 uGuid, BOS_BOOL bBkoffTmr)
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_UINT32 uIx;
   BOS_UINT32 uBackupIx = guCeInvalidGuid;
   BOS_BOOL bBackup = BOS_FALSE;
   NSBRSTATUS eStatus = eNSBRSTA_INVALID;
   const CFGUSR *pCfgUsr;
   BOS_BOOL bBackOffOnAnyFail= BOS_FALSE;

   if ( pBrBlk )
   {
      /* Assume failure.
      */
      eStatus = eNSBRSTA_FAILED;
      
      /* Determine whether the recover mode requires the backoff timer
      ** to be applied on every failure instead of only when all targets
      ** have failed.
      */
      if ( ( pCfgUsr = CFG_USR( uGuid ) ) != NULL )
      {
         if ( pCfgUsr->xtl.usr.net.brCfg.eBackoffMode == eCCTKBRBKOFFMODE_ANYFAIL )
         {
            bBackOffOnAnyFail = BOS_TRUE;
         }
      }

      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         if ( pBrBlk->nsBr[ uIx ].eState == eNSBRSTATE_ACTIVE )
         {
            /* The target that was active has failed.
            */
            pBrBlk->nsBr[ uIx ].eState = eNSBRSTATE_FAILED;
         }

         if ( pBrBlk->nsBr[ uIx ].eState == eNSBRSTATE_IDLE )
         {
            /* There is a possibility of backup target.
            */
            if ( uBackupIx == guCeInvalidGuid )
            {
               uBackupIx = uIx;
            }

            bBackup = BOS_TRUE;
         }
      }
      
      if ( bBackup )
      {
         eStatus = eNSBRSTA_BACKUP_AVAILABLE;
         /* Promote the target to being the new active one.
         */
         pBrBlk->nsBr[ uBackupIx ].eState = eNSBRSTATE_ACTIVE;
         
         /* Calculate the backoff timer if required to do so
         */
         if ( bBkoffTmr && bBackOffOnAnyFail )
         {
            nsBrGetTmr ( uGuid );
         }
      }
      else if ( bBkoffTmr )
      {
         /* No longer any possibility for backup, assign the backoff
         ** timer to the proper value for this backup and recovery
         ** block, the timer is used as part of the processing of the
         ** feature.
         */
         nsBrGetTmr ( uGuid );
      }
   }

   return eStatus;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrRestart
**
**  PURPOSE:    Asks the backup and recovery module to start up fresh again in case
**              there has been a failure previously.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS NsBr::nsBrRestart( BOS_UINT32 uGuid )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_UINT32 uIx;
   NSBRSTATUS eStatus = eNSBRSTA_INVALID;

   if ( pBrBlk )
   {
      CCTK_TRACEI1( "nsBrRestart - success (0x%X)", uGuid );

      eStatus = eNSBRSTA_IDLE;

      /* Reset the status of the block itself that should have been marked
      ** as 'failed' to prevent any further re-attempts.
      */
      pBrBlk->eStatus = eStatus;
      pBrBlk->prefPrimSockAddr.Reset();
      
      /* Remove any restrictions that were present.
      */
      pBrBlk->nsBrRestrict.eType = eNSBRRESTRICTIONTYPE_NONE;
      pBrBlk->nsBrRestrict.restrictSockAddr.Reset();

      /* Idle out all the targets.  Target selection will happen upon first
      ** attempt to send out a request following this clean up condition.
      */
      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         pBrBlk->nsBr[ uIx ].eState = eNSBRSTATE_IDLE;
         pBrBlk->nsBr[ uIx ].bPrimary = BOS_FALSE;
      }
   }

   return eStatus;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrBackOff
**
**  PURPOSE:    Get the value of the back-off timer that the backup and recovery
**              module decides should be applied for the current backup situation.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    The timer, or 0 if none applicable (error scenario).
**
**  NOTES:
***********************************************************************************/
BOS_UINT32 NsBr::nsBrBackOff( BOS_UINT32 uGuid )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_UINT32 uTmr = 0;

   if ( pBrBlk )
   {
      uTmr = pBrBlk->uBackOffTmr;
   }

   return uTmr;
}

/***********************************************************************************
**  FUNCTION:   NsBr::nsBrResetBackOff
**
**  PURPOSE:    Reset the value of the back-off timer.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    BOS_TRUE if the timer was succesfully restarted. BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrResetBackOff( BOS_UINT32 uGuid )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );

   if ( pBrBlk )
   {
      pBrBlk->uBackOffTmr = 0;
      pBrBlk->uConsecFail = 0;
      return BOS_TRUE;
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrGetTmr
**
**  PURPOSE:    Assign the next value for the backoff timer based on the actual
**              value and the configuration information.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    Nothing, but the timer value in the backup and recovery block
**              is updated accordingly.
**
**  NOTES:
***********************************************************************************/
void NsBr::nsBrGetTmr( BOS_UINT32 uGuid )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   const CFGUSR *pCfgUsr;
   BOS_UINT32 uTmr = guNsBrBackoffTmr;

   if ( pBrBlk )
   {
      if ( ( pCfgUsr = CFG_USR( uGuid ) ) != NULL )
      {
         BOS_BOOL bOutboundBackOff = pCfgUsr->xtl.usr.net.brCfg.bOutboundBackoff;
         BOS_UINT32 uTmrMin = pCfgUsr->xtl.usr.net.brCfg.uBackoffMinTmr;
         BOS_UINT32 uTmrMax = pCfgUsr->xtl.usr.net.brCfg.uBackoffMaxTmr;

         /* Take care of special cases.
         */
         if ( !uTmrMin )
         {
            uTmr = guNsBrBackoffTmr;
         }
         else if ( !uTmrMax )
         {
            uTmr = uTmrMin;
         }
         else if ( bOutboundBackOff )
         {
            /* Required to use the RFC 5626 method of calculating
            ** the wait time on each failure based on the number of consecutive
            ** failures (or the flow recovery time) via the following formula:
            ** 
            **    W = min( max-time, ( base-time * ( 2 ^ consecutive-failures) )
            ** 
            ** where:
            **    max-time with a default of 1800 sec
            **    base-time (if all failed) with a default of 30 sec
            **    base-time (if all have not failed) with a default of 90 sec
            **
            ** Since we only attempt one target at a time, each failure is treated as
            ** if all flows failed.
            */

            /* Set the timer to the max initially
            */
            uTmr = uTmrMax;
            
            /* Set the base timer to the configure min value
            */
            BOS_UINT32  uBaseTmrS = uTmrMin;

            /* Increment and track the number of consecutive failure counts.
            */
            BOS_UINT32  uFailureCount = ++pBrBlk->uConsecFail;


            /* If failure count is bigger than 31, the multiplication factor will be 0,
            ** so no need to do a costly operation in that case */
            if ( uFailureCount < 31 )
            {
                BOS_UINT32 uTempS = uBaseTmrS * (1 << uFailureCount);

                /* Use the minimum of the max-time or base-time calculation */
                if ( uTempS < uTmr )
                {
                    uTmr = uTempS;
                }
                
                /* If this the first failure, the timer must be less than 5 minutes
                */
                if (( uFailureCount == 1) && ( uTmr > guNsBrBackoffTmr ))
                {
                  uTmr = guNsBrBackoffTmr;
                }
            }

            /* Compute the actual backoff time by selecting a uniform random time
            ** between 50% and 100% of the upper-bound wait time.  It is logical 
            ** to only apply this random range if the selected timer is at least 
            ** larger than a certain value (i.e. 2) */
            if ( uTmr > 2 )
            {
               BOS_UINT32 uBackoffTmrHalf = uTmr / 2;
               uTmr = uBackoffTmrHalf + ( rand() % uBackoffTmrHalf );
            }
         }
         else
         {
            /* Use simple exponential backoff. Basically just double the current
            ** backoff timer to produce the end of result of:
            ** W = min(max_time, min_time*(2^(consec-allfailed - 1)))
            */
            if ( !pBrBlk->uBackOffTmr )
            {
               /* Initial value.
               */
               uTmr = uTmrMin;
            }
            else if ( pBrBlk->uBackOffTmr >= uTmrMax )
            {
               /* Reached the maximum already, reset to the initial
               ** value.
               */
               uTmr = uTmrMin;
            }
            else
            {
               /* Assign next value.
               */
               uTmr = 2 * pBrBlk->uBackOffTmr;
               
               if ( uTmr > uTmrMax )
               {
                  uTmr = uTmrMax;
               }
            }
         }
      }

      /* Assign the new timer value.
      */
      pBrBlk->uBackOffTmr = uTmr;
   }
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrFailedPrimary
**
**  PURPOSE:    Asks the backup and recovery module whether the resource has failed
**              with its primary target or not.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**
**  RETURNS:    BOS_TRUE if the primary target is in failure, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrFailedPrimary( BOS_UINT32 uGuid )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_BOOL bRet = BOS_FALSE;
   BOS_UINT32 uIx;

   if ( pBrBlk )
   {
      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         if ( pBrBlk->nsBr[ uIx ].bPrimary &&
              ( pBrBlk->nsBr[ uIx ].eState != eNSBRSTATE_ACTIVE ) )
         {
            bRet = BOS_TRUE;
         }
      }
   }

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrGetActiveTgt
**
**  PURPOSE:    Retrieves the active target information associated with the GUID
**              parameter.
**
**  PARAMETERS:
**              uGuid - the GUID of interest.
**              rActSockAddr - reference to the active target address to retrieve.
**
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrGetActiveTgt( BOS_UINT32 uGuid, MXD_GNS::CSocketAddr& rActSockAddr )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_BOOL bRet = BOS_FALSE;
   BOS_UINT32 uIx;

   if ( pBrBlk )
   {
      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         if ( pBrBlk->nsBr[ uIx ].eState == eNSBRSTATE_ACTIVE )
         {
            rActSockAddr.SetAddress ( 
               reinterpret_cast<const char *> (pBrBlk->nsBr[ uIx ].host.pcAddr),
               static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort) );
            
            bRet = BOS_TRUE;
            break;
         }
      }
   }

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrPrimaryStillPreferred
**
**  PURPOSE:    Asks the backup and recovery module whether the current primary
**              is still the most preferred target.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    BOS_TRUE if the marked primary target equals the saved preferred 
**              target, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrPrimaryStillPreferred( BOS_UINT32 uGuid )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_BOOL bRet = BOS_FALSE;
   BOS_UINT32 uIx;
   

   if ( pBrBlk )
   {
      /* Check to see if the preferred primary (the target with
      ** the highest preference acccording to target resolution),
      ** is equal to the user's currently known primary address.
      */
      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         if ( pBrBlk->nsBr[ uIx ].bPrimary )
         {
            if (nsBrEqual ( pBrBlk, uIx, pBrBlk->prefPrimSockAddr ))
            {
               bRet = BOS_TRUE;
               break;
            }
         }
      }
   }
   
   return bRet;
}



/***********************************************************************************
**  FUNCTION:   NsBr::nsBrApplyRestriction
**
**  PURPOSE:    Asks the backup and recovery module to restrict or not its knowledge
**              set to a specific target alone, either to the primary or to a given
**              address. This can be used in some failover scenario when we want to
**              ensure that only a certain target is used.
**
**  PARAMETERS:
**              uResGuid - the resource GUID.
**              bRestrict - whether to restrict (BOS_TRUE) or lift the restriction
**                          (BOS_FALSE)
**              eRstrictToApply - the restriction type. eNSBRRESTRICTIONTYPE_NONE
**                                removes all restrictions. All other types can
**                                be used to remove or apply a specific restriction
**                                if it already exists for the resource.
**              pRstrictAddr - The address which the resource should be restricted to. 
**                             Only used in conjunction with 
**                             eNSBRRESTRICTIONTYPE_ADDRSPECIFIED.
**
**  RETURNS:    BOS_TRUE, if the restriction was applied or lifted successfully,
**              BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrApplyRestriction( BOS_UINT32 uGuid, 
                                     BOS_BOOL bRestrict,
                                     NSBRRESTRICTIONTYPE eRstrictToApply, 
                                     MXD_GNS::CSocketAddr *pRstrictAddr )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_BOOL bSuccess = BOS_FALSE;
   BOS_UINT32 uIx = guCeInvalidGuid;

   if ( pBrBlk )
   {
      /* If a restriction needs to be applied, make sure that one
      ** is not already present, and that we're not trying to remove all
      ** restrictions.
      */
      CCTK_TRACEI3( "nsBrApplyRestriction - for 0x%lX, %s : type %d", uGuid,
                                bRestrict? "apply" : "remove", eRstrictToApply );
      if ( bRestrict && 
          ( pBrBlk->nsBrRestrict.eType == eNSBRRESTRICTIONTYPE_NONE ) &&
          ( eRstrictToApply != eNSBRRESTRICTIONTYPE_NONE ) )
      {
         /* Apply restriction and indicate success.
         */
         if (eRstrictToApply == eNSBRRESTRICTIONTYPE_ADDRSPECIFIED )
         {
            
            if ( pRstrictAddr )
            {
               BOS_UINT8 uAddress[ CCTK_STR_LEN ];
               BOS_UINT32 uCapacity = CCTK_STR_LEN;
               pRstrictAddr->GetAddress( static_cast<unsigned int> (uCapacity),
                                                reinterpret_cast<char *> (uAddress) );
               CCTK_TRACEI3( "nsBrApplyRestriction - target %s:%d for 0x%lX",
                                reinterpret_cast<const char *> (uAddress),
                                static_cast<uint16_t> (pRstrictAddr->GetPort()),
                                uGuid);
               pBrBlk->nsBrRestrict.restrictSockAddr = *pRstrictAddr;
            }
            else
            {
               /* Not valid. Cannot restrict target selection to a non-specified address!
               */
               return BOS_FALSE;
            }
         }
         pBrBlk->nsBrRestrict.eType = eRstrictToApply; 
         bSuccess = BOS_TRUE; 
      }
      /* Otherwise if we're trying to remove a specific or just any restriction
      ** then do so now, if one is actually present.
      */
      else if ( ( !bRestrict || ( eRstrictToApply == eNSBRRESTRICTIONTYPE_NONE )) &&
                ( pBrBlk->nsBrRestrict.eType != eNSBRRESTRICTIONTYPE_NONE ))
      {
         /* Regardless of what's being requested we can only remove the current
         ** restriction.
         */
         if ( eRstrictToApply != eNSBRRESTRICTIONTYPE_NONE &&
              eRstrictToApply != pBrBlk->nsBrRestrict.eType )
         {
            /* Not valid. Restrictions don't match.
            */
            return BOS_FALSE;
         }
         
         /* Valid to remove current restrictions. Clean up the other targets.
         */
         for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
         {
            if ( ( pBrBlk->nsBrRestrict.eType == eNSBRRESTRICTIONTYPE_PRIMONLY &&
                     !(pBrBlk->nsBr[ uIx ].bPrimary) ) ||
                        ( pBrBlk->nsBrRestrict.eType == eNSBRRESTRICTIONTYPE_ADDRSPECIFIED &&
                           !nsBrEqual( pBrBlk, uIx, pBrBlk->nsBrRestrict.restrictSockAddr ) ))
            {
               pBrBlk->nsBr[ uIx ].eState = eNSBRSTATE_IDLE;
            }
         }
         
         /* Lift the restriction and indicate success.
         */
         pBrBlk->nsBrRestrict.eType = eNSBRRESTRICTIONTYPE_NONE;
         pBrBlk->nsBrRestrict.restrictSockAddr.Reset();
         bSuccess = BOS_TRUE;
      }
   }

   return bSuccess;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrCopyTgt
**
**  PURPOSE:    Asks the backup and recovery module to copy a list of targets from
**              one resource to the next.  This is to be used in cases where one
**              want an exact same set to be applied to different resources.
**
**  PARAMETERS:
**              uResGuidFrom - the resource GUID to copy information from.
**              uResGuidTo - the resource GUID to copy information to.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:
***********************************************************************************/
NSBRSTATUS NsBr::nsBrCopyTgt( BOS_UINT32 uGuidFrom, BOS_UINT32 uGuidTo )
{
   NSBRBLK *pBrBlkFrom = nsBrGetBlk ( uGuidFrom );
   NSBRBLK *pBrBlkTo = nsBrGetBlk ( uGuidTo );
   NSBRSTATUS eStatus = eNSBRSTA_INVALID;

   if ( pBrBlkFrom && pBrBlkTo )
   {
      BOS_UINT32 uIx;

      eStatus = eNSBRSTA_IDLE;

      pBrBlkTo->uItemCnt = pBrBlkFrom->uItemCnt;

      for ( uIx = 0 ; uIx < pBrBlkTo->uItemCnt ; uIx++ )
      {
         pBrBlkTo->nsBr[ uIx ].bPrimary = pBrBlkFrom->nsBr[ uIx ].bPrimary;
         pBrBlkTo->nsBr[ uIx ].eState = pBrBlkFrom->nsBr[ uIx ].eState;
         pBrBlkTo->nsBr[ uIx ].host.uPort = pBrBlkFrom->nsBr[ uIx ].host.uPort;
         pBrBlkTo->nsBr[ uIx ].host.pcAddr =
            utilStrDup ( pBrBlkFrom->nsBr[ uIx ].host.pcAddr );
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   NsBr::nsBrGetAvailTgt
**
**  PURPOSE:    Retrieves the first target address which is not in the failed state.
**
**  PARAMETERS:
**              uGuid - the GUID of interest.
**              rHostPort - hostport reference to set to target address.
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsBr::nsBrGetAvailTgt( BOS_UINT32 uGuid,
                                CCTKHOSTPORT& rHostPort )
{
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );
   BOS_BOOL bRet = BOS_FALSE;
   BOS_UINT32 uIx;
   BOS_UINT8 *pBrAddr = NULL;

   if ( pBrBlk )
   {
      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         if ( pBrBlk->nsBr[ uIx ].eState != eNSBRSTATE_FAILED )
         {
            rHostPort = pBrBlk->nsBr[ uIx ].host;

            bRet = BOS_TRUE;
            break;
         }
      }
   }

   return bRet;
}

/***********************************************************************************
**  FUNCTION:   NsBr::nsBrSave
**
**  PURPOSE:    Save a backup and recovery block content to the passed in
**              location.
**
**  PARAMETERS:
**              uGuid - the resource GUID to copy information from.
**              pSaveTo - where to save the information to.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:      nsBrSave and nsBrPrime are paired function, they MUST be invoked
**              together and within the same execution context to prevent memory
**              leakage.  They are used for temporary hold of some information
**              only.
***********************************************************************************/
NSBRSTATUS NsBr::nsBrSave( BOS_UINT32 uGuid, NSBRBLK *pSaveTo )
{
   NSBRBLK *pBrBlkFrom = nsBrGetBlk ( uGuid );
   NSBRSTATUS eStatus = eNSBRSTA_INVALID;

   if ( pBrBlkFrom && pSaveTo )
   {
      BOS_UINT32 uIx;

      eStatus = eNSBRSTA_IDLE;

      pSaveTo->uItemCnt = pBrBlkFrom->uItemCnt;

      for ( uIx = 0 ; uIx < pSaveTo->uItemCnt ; uIx++ )
      {
         pSaveTo->nsBr[ uIx ].bPrimary = pBrBlkFrom->nsBr[ uIx ].bPrimary;
         pSaveTo->nsBr[ uIx ].eState = pBrBlkFrom->nsBr[ uIx ].eState;
         pSaveTo->nsBr[ uIx ].host.uPort = pBrBlkFrom->nsBr[ uIx ].host.uPort;
         pSaveTo->nsBr[ uIx ].host.pcAddr =
                              utilStrDup ( pBrBlkFrom->nsBr[ uIx ].host.pcAddr );
      }
   }

   return eStatus;
}


/***********************************************************************************
**  FUNCTION:   NsBr::nsBrPrime
**
**  PURPOSE:    Prime a backup and recovery block content using the passed
**              in information, then remove the prime'd information content.
**
**  PARAMETERS:
**              uGuid - the resource GUID to copy information to.
**              pPrimeFrom - where to prime the information from.
**
**  RETURNS:    The status, see the NSBRSTATUS enum definition.
**
**  NOTES:      nsBrSave and nsBrPrime are paired function, they MUST be invoked
**              together and within the same execution context to prevent memory
**              leakage.  They are used for temporary hold of some information
**              only.
***********************************************************************************/
NSBRSTATUS NsBr::nsBrPrime( BOS_UINT32 uGuid, NSBRBLK *pPrimeFrom )
{
   NSBRBLK *pBrBlkTo = nsBrGetBlk ( uGuid );
   NSBRSTATUS eStatus = eNSBRSTA_INVALID;

   if ( pBrBlkTo && pPrimeFrom )
   {
      BOS_UINT32 uIx;

      eStatus = eNSBRSTA_IDLE;

      pBrBlkTo->uItemCnt = pPrimeFrom->uItemCnt;

      for ( uIx = 0 ; uIx < pBrBlkTo->uItemCnt ; uIx++ )
      {
         pBrBlkTo->nsBr[ uIx ].bPrimary = pPrimeFrom->nsBr[ uIx ].bPrimary;
         pBrBlkTo->nsBr[ uIx ].eState = pPrimeFrom->nsBr[ uIx ].eState;
         pBrBlkTo->nsBr[ uIx ].host.uPort = pPrimeFrom->nsBr[ uIx ].host.uPort;
         pBrBlkTo->nsBr[ uIx ].host.pcAddr = pPrimeFrom->nsBr[ uIx ].host.pcAddr;
         pPrimeFrom->nsBr[ uIx ].host.pcAddr = NULL;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   NsBr::nsBrXtlLstUpdate
**
**  PURPOSE:    Populates a backup and recovery block content using a configured
**              external list of target addresses.
**
**
**  PARAMETERS: uGuid - the  user resource GUID to look up the list for.
**
**  RETURNS:    Nothing.
**
**  NOTES:      Doesn't configure an associated port from the external list to
**              allow this feature to work with negotiated security associations.
***********************************************************************************/
void NsBr::nsBrXtlLstUpdate( BOS_UINT32 uGuid )
{
   const CFGUSR *pCfgUsr;
   NSBRBLK *pBrBlk = nsBrGetBlk ( uGuid );

   if ( pBrBlk )
   {
      if ( ( pCfgUsr = CFG_USR( uGuid ) ) != NULL )
      {
         /* If a new list is present, just populate the backup and recovery
         ** block with the external list.
         */
         if ( pCfgUsr->xtl.usr.net.brCfg.extList.uNum )
         {
            if ( !pBrBlk->uItemCnt )
            {
               BOS_UINT8 uAddress[ CCTK_STR_LEN ];
               BOS_STATUS status;
               BOS_UINT32 uIx;
               BOS_UINT32 uNumXtl = pCfgUsr->xtl.usr.net.brCfg.extList.uNum;

               for ( uIx = 0 ;
                     ( uIx < uNumXtl ) &&
                     ( uIx < CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS ) ; uIx++ )
               {
                  status = bosIpAddrGetStr(
                              &pCfgUsr->xtl.usr.net.brCfg.extList.ipport[uIx].addr,
                               reinterpret_cast<char *> ( uAddress ),
                               CCTK_STR_LEN );

                  if ( status == BOS_STATUS_OK )
                  {
                     pBrBlk->nsBr[ uIx ].host.pcAddr =
                        utilStrDup ( reinterpret_cast<const BOS_UINT8 *> (uAddress) );
                     pBrBlk->nsBr[ uIx ].host.uPort = 
                        pCfgUsr->xtl.usr.net.brCfg.extList.ipport[uIx].uPort;

                     pBrBlk->nsBr[ uIx ].eState = eNSBRSTATE_IDLE;
                     pBrBlk->uItemCnt++;

                     CCTK_TRACEI4( "nsBrXtlLstUpdate - inserted %s:%d for 0x%lX, num %u",
                           reinterpret_cast<const char *> (
                           pBrBlk->nsBr[ uIx ].host.pcAddr),
                           static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort),
                           pBrBlk->uGuid,
                           pBrBlk->uItemCnt );
                  }
               }
            }
            /* TODO - create an intersection list if one already exists.
            */
            
            /* The external list is an ordered list, so set the
            ** very first target as the primary.
            */
            if ( pBrBlk->uItemCnt && !pBrBlk->nsBr[0].bPrimary)
            {
               pBrBlk->nsBr[0].bPrimary = BOS_TRUE;
            }
         }
      }
   }
}

/***********************************************************************************
**  FUNCTION:   NsBr::nsBrXtlLstSetFailedTgt
**
**  PURPOSE:    Sets the specified target address as failed in the backup
**              and recovery block of the user
**
**  PARAMETERS: uGuid - the resource GUID to update the target for.
**              rActSockAddr - socket address which failed.
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:      This to only be used when the backup and recovery feature
**              in external service mode for the user.
***********************************************************************************/
BOS_BOOL NsBr::nsBrXtlLstSetFailedTgt( BOS_UINT32 uGuid,
                                       MXD_GNS::CSocketAddr& rActSockAddr )
{
   NSBRBLK *pBrBlk;
   BOS_BOOL bRet = BOS_FALSE;
   BOS_UINT32 uIx;

   /* Update the target list
   */
   nsBrXtlLstUpdate( uGuid );

   /* Find the specified target and mark as failed
   */
   pBrBlk = nsBrGetBlk ( uGuid );
   if ( pBrBlk )
   {
      for ( uIx = 0 ; uIx < pBrBlk->uItemCnt ; uIx++ )
      {
         if ( nsBrEqual( pBrBlk, uIx, rActSockAddr ) )
         {
            pBrBlk->nsBr[ uIx ].eState = eNSBRSTATE_FAILED;
            bRet = BOS_TRUE;

            CCTK_TRACEI3( "nsBrXtlLstSetFailedTgt - failed %s:%d for 0x%lX",
                  reinterpret_cast<const char *> (
                  pBrBlk->nsBr[ uIx ].host.pcAddr),
                  static_cast<uint16_t> (pBrBlk->nsBr[ uIx ].host.uPort),
                  pBrBlk->uGuid );

            break;
         }
      }
   }

   return bRet;
}
#endif /* CCTKCFG_PROXY_FAILOVER */
