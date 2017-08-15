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
**      This file implements the core functionality of the DNS Lookup Manipulation
**      module.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <nsDlm.h>
#include <nsBr.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
MXD_GNS::mxt_PFNServerLocationListModifier NsDlm::mpfnLocLstCb = NULL;
NsBr * NsDlm::mpNsBr = NULL;

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   NsDlm::NsDlm
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              pNsBr - pointer to the associated 'backup and recovery' module.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
NsDlm::NsDlm( NsBr *pNsBr )
{
   mpNsBr = pNsBr;
}


/***********************************************************************************
**  FUNCTION:   NsDlm::~NsDlm
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
NsDlm::~NsDlm()
{
}


/***********************************************************************************
**  FUNCTION:   NsDlm::nsDlmAttach
**
**  PURPOSE:    Attach the DNS Lookup Manipulation callback for this resource.
**
**  PARAMETERS:
**              uGuid - the GUID for this resource.
**              pContext - the SCE context associated with this resource, this
**                         is the context which is used to attach the service.
**
**
**  RETURNS:    BOS_TRUE if success, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL NsDlm::nsDlmAttach( BOS_UINT32 uGuid,
                             void *pContext )
{
   BOS_BOOL bRet = BOS_FALSE;

   if ( ( uGuid != guCeInvalidGuid ) &&
        ( pContext != NULL ) )
   {
      MXD_GNS::ISipContext* pSipContext =
         reinterpret_cast<MXD_GNS::ISipContext *> ( pContext );

      MXD_GNS::ISipServerLocationSvc* pServerLocation = NULL;

      pSipContext->QueryIf( MXD_GNS::IID_ISipServerLocationSvc,
                            reinterpret_cast<void**> ( &pServerLocation ) );

      if ( pServerLocation != NULL )
      {
         MXD_GNS::mxt_PFNServerLocationListModifier pfn = NULL;
         mxt_opaque opq;

         pServerLocation->SetServerLocationListModifier(
                                          NsDlm::nsDlmCb,
                                          reinterpret_cast<mxt_opaque> ( uGuid ),
                                          pfn,
                                          opq );

         /* If a callback was already set for this service, save it such that we
         ** can invoke it when appropriate.
         **
         ** Make sure it is not our own callback (do not invoke the same service
         ** twice).
         */
         if ( pfn != NsDlm::nsDlmCb )
         {
            /* There is currently one other service in the SCE core that would
            ** use this callback functionality, that is the black-list service.
            ** The callback would always be the same.
            */
            mpfnLocLstCb = pfn;

#if CCTKCFG_PROXY_FAILOVER
            /* Save the mapping of our 'opaque' to the other service opaque.
            */
            mpNsBr->nsBrSetOpq ( uGuid,
                                 opq );
#endif
         }

         pServerLocation->ReleaseIfRef();
         pServerLocation = NULL;

         /* Success
         */
         bRet = BOS_TRUE;
      }
   }

   CCTK_TRACED3( "NsDlm::nsDlmAttach - 0x%lX, %p - %s",
                 uGuid, pContext, bRet ? "Success" : "Fail" );

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   NsDlm::nsDlmCb
**
**  PURPOSE:    Callback invoked from the SCE layer when the DNS Lookup
**              Manipulation service was installed for this resource (the
**              resource identifier is passed in as 'opaque' information).
**
**              The callback interacts with the Backup and REcovery module to
**              determine what sort of manipulation is necessary on the records
**              information.  Following such interaction, the records are updated
**              to reflect the intention.
**
**  PARAMETERS:
**              rFqdn - the FQDN that this callback is being invoked for.
**              rlstNaptrRecord - the set of resolved records which correspond to
**                                the DNS resolution of the 'rFqdn' information.
**              opq - the opaque information set when the callback is attached to
**                    the service.  In our implementation, this is the GUID of the
**                    resource for which this callback is invoked.
**
**
**  RETURNS:    Nothing - however the rlstNaptrRecord may be changed accordingly.
**
**  NOTES:      The execution of this callback must be within the callback context
**              there can be no context switch happening at this time.
***********************************************************************************/
void NsDlm::nsDlmCb( const MXD_GNS::CHostPort& rFqdn,
                     MXD_GNS::CList<MXD_GNS::SNaptrRecord> &rlstNaptrRecord,
                     mxt_opaque opq )
{
   BOS_UINT32 uResGuid = guCeInvalidGuid;
   BOS_UINT32 uNaptrCurIx;
   BOS_UINT32 uSrvCurIx;
   BOS_UINT32 uSocketCurIx;
   BOS_UINT8 uAddress[ CCTK_STR_LEN ];
   BOS_UINT32 uCapacity = CCTK_STR_LEN;
   BOS_BOOL bSkipBl = BOS_FALSE;   

#if !CCTKCFG_PROXY_FAILOVER
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rlstNaptrRecord );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rFqdn );
#endif

   /* Properly associate the opaque with the proper user GUID for this
   ** resource.
   */
   uResGuid = ceGetUsrGuid( reinterpret_cast<BOS_UINT32> ( opq ) );

   /* Retrieve the backup and recovery block for the user and check
   ** if we should skip the callback modifier that was overriden
   */
   if ( uResGuid != guCeInvalidGuid )
   {
#if CCTKCFG_PROXY_FAILOVER
      NSBRBLK *pBrBlk = mpNsBr->nsBrGetBlk ( uResGuid );
      BOS_BOOL bPrefPrimFound = BOS_FALSE;
      
      if ( pBrBlk == NULL )
      {
         CCTK_TRACEI1( "nsDlmCb - no backup-recovery info for 0x%X",
                       uResGuid);
         return;
      }
#endif
      
      CCTK_TRACED2( "nsDlmCb - %d NAPTR record(s) for 0x%lX",
                 rlstNaptrRecord.GetSize(),
                 uResGuid);
      for ( uNaptrCurIx = 0 ;
            uNaptrCurIx < rlstNaptrRecord.GetSize() ;
            uNaptrCurIx++ )
      {
         MXD_GNS::CList<MXD_GNS::SSrvRecord>& rlstSrv =
                        rlstNaptrRecord[ uNaptrCurIx ].lstSrvRecord;
         CCTK_TRACED3( "nsDlmCb - %d SRV record(s) for NAPTR-%d on 0x%lX",
                       rlstSrv.GetSize(),
                       uNaptrCurIx,
                       uResGuid );
         for ( uSrvCurIx = 0 ;
               uSrvCurIx < rlstSrv.GetSize() ;
               uSrvCurIx++ )
         {
            MXD_GNS::CList<MXD_GNS::CSocketAddr>& rlstAddress =
                                          rlstSrv[ uSrvCurIx ].lstSocketAddr;
            CCTK_TRACED4( "nsDlmCb - %d A record(s) for NAPTR-%d/SRV-%d on 0x%lX",
                          rlstAddress.GetSize(),
                          uNaptrCurIx,
                          uSrvCurIx,
                          uResGuid);
            for ( uSocketCurIx = 0 ;
                  uSocketCurIx < rlstAddress.GetSize() ;
                  uSocketCurIx++ )
            {
               MXD_GNS::CSocketAddr& rCurrentSocket =
                                          rlstAddress[ uSocketCurIx ];
               /*If a valid port is not set, set it now */
               if ( rCurrentSocket.GetPort() == 0 )
               {
                  rCurrentSocket.SetPort( rlstSrv[ uSrvCurIx ].uPort );
               }
               rCurrentSocket.GetAddress( static_cast<unsigned int> (uCapacity),
                                             reinterpret_cast<char *> (uAddress) );
               CCTK_TRACED3( "nsDlmCb - target %s:%d for 0x%lX",
                             reinterpret_cast<const char *> (uAddress),
                             static_cast<uint16_t> (rCurrentSocket.GetPort()),
                             uResGuid);
#if CCTKCFG_PROXY_FAILOVER                             
               /* If this is the very first address in the list, then it's the most preferred
               ** target (taking into account priority and weight). Save this target info
               ** as the new preferred primary for the user's backup and recovery block.
               */
               if ( !bPrefPrimFound && ( uSocketCurIx == 0 ) )
               {
                  bPrefPrimFound = BOS_TRUE;
                  /* Clean up any prior preferred primary knowledge and copy over
                   ** the latest.
                   */
                  pBrBlk->prefPrimSockAddr.Reset();
                  pBrBlk->prefPrimSockAddr = rCurrentSocket;
               }
#endif               
            }
         }
      }
   
      
#if CCTKCFG_PROXY_FAILOVER
      
      /* Don't run the resolved records through the black list service
      ** if the backup and recovery block doesn't have any prior info
      ** for this user - no need to filter records for this user if
      ** this is the first attempt. 
      */ 
      if (pBrBlk->uItemCnt == 0)
      {
         bSkipBl = BOS_TRUE;
         CCTK_TRACED1( "nsDlmCb - skip blacklist for 0x%lX",
                       uResGuid);
      }
#else
      bSkipBl = BOS_TRUE;
#endif
   }
   
   /* First step is to apply the callback modifier that may have been
   ** installed by another service that this callback has overriden.
   */
   if ( !bSkipBl &&
           mpfnLocLstCb != NULL)
   {
#if CCTKCFG_PROXY_FAILOVER
      mxt_opaque cbOpq;

      if ( mpNsBr->nsBrGetOpq ( reinterpret_cast<BOS_UINT32> ( opq ),
                                cbOpq ) )
      {
         mpfnLocLstCb ( rFqdn,
                        rlstNaptrRecord,
                        cbOpq );
      }
#endif
   }

   /* Second step is to check from the backup and recovery module which
   ** target should be enforced for this callback.
   **
   */
   if ( uResGuid != guCeInvalidGuid )
   {
#if CCTKCFG_PROXY_FAILOVER
      mpNsBr->nsBrSelectDest( uResGuid,
                              reinterpret_cast<BOS_UINT32> ( opq ),
                              rFqdn,
                              rlstNaptrRecord );
#endif
   }
}
