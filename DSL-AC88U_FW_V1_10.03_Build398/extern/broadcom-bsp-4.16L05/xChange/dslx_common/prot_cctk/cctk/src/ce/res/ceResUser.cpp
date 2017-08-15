/**********************************************************************************
** Copyright (c) 2007-2011 Broadcom Corporation
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
**      This file implements the resource user class.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <ceMain.h>
#include <ceResUser.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeResUser::resourceCreate
**
**  PURPOSE:    Creates the necessary network resource to handle this user
**              resource.
**
**  PARAMETERS:
**              uCfgGuid - the GUID to locate the configuration associated with
**                         this resource.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeResUser::resourceCreate( BOS_UINT32 uCfgGuid )
{
   BOS_BOOL bRet = BOS_TRUE;
   const CFGUSR *pCfgUsr;
   MXD_GNS::ISceUserAuthentication *pSceUsrAuth;
   MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl;

   /* Allocate the GUID up front for this resource because following process may
   ** invoke callbacks from the UASSP/SCE core for which we would need to know the
   ** actual GUID allocation for matching purposes.
   */
   resourceAllocated( uCfgGuid, guCeGuidSeed );

   /* Create the resource instance at the UA layer.
   */
   mxt_result res = CreateEComInstance( MXD_GNS::CLSID_CUaSspRegistration,
                                        NULL,
                                        MXD_GNS::IID_IUaSspRegistration,
                                        reinterpret_cast<void**>( &mpUaSspReg ));
   if ( MX_RIS_S( res ))
   {
      /* Set the manager to receive event information for this service.
      */
      mpUaSspReg->SetManager( gpCeMain );

      res = mpUaSspReg->QueryIf( &pSceBasicExtCtl );
      if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
      {
         pSceBasicExtCtl->SetBasicExtensionManager( gpCeMain );
         pSceBasicExtCtl->SetSipPacketObserver(
            reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>( pmGetPktObs() ) );

         pSceBasicExtCtl->ReleaseIfRef();
         pSceBasicExtCtl = NULL;
      }
      else
      {
         CCTK_TRACEI1(
            "CeResUser (%p) - unable to set basic extension manager.",
            this );
      }
   }
   else
   {
      CCTK_TRACEI1( "CeResUser (%p) - unable to create UASSP registration.",
                    this );
      bRet = BOS_FALSE;
   }

   pCfgUsr = CFG_USR( uCfgGuid );
   if ( bRet && (pCfgUsr != NULL) )
   {
      if ( pCfgUsr->xtl.usr.net.brCfg.eMode != eCCTKBRSVCMODE_DISABLED )
      {
         if ( !nsBrRegister ( uCfgGuid ) )
         {
            CCTK_TRACEI1( "CeResUser (%p) - unable to register backup/recovery.",
                          this );
         }

         /* If the 'back-to-primary' feature is enabled, make a copy of the
         ** initial user configuration that we will be using possibly at some
         ** later time during the failover process.
         */
         if ( pCfgUsr->xtl.usr.net.brCfg.eBackToPrimMode == eCCTKBRBACKTOPRIMMODE_SILENT )
         {
            BOS_UINT32 uBkupGuid = ceGuid();
            CFGSTATUS eCfgStatus = cfgDupUsr ( uBkupGuid, uCfgGuid );
            
            if ( CFGSTATUS_IS_SUCCESS( eCfgStatus ) )
            {
               resourceSetBkupConfig( uBkupGuid );
            }
         }
      }

      res = pCfgUsr->in.pSceUsrCfg->QueryIf( &pSceUsrAuth );
      if ( MX_RIS_S( res ) && ( pSceUsrAuth != NULL ) )
      {
         pSceUsrAuth->SetManager( gpCeMain );
         pSceUsrAuth->ReleaseIfRef();
         pSceUsrAuth = NULL;
      }
      else
      {
         CCTK_TRACEI1( "CeResUser (%p) - unable to set user auth manager.",
                       this );
      }

      /* The configuration for this resource will only be set once confirmation
      ** from the CCTK framework that the user has been properly added, this is to
      ** ensure that all the necessary pieces of the user configuration are in place
      ** prior to invoking the configuration interface on the UASSP.
      **
      ** Failure to do so may result in an incompletely configured user within the
      ** UASSP layer which in turn may result in the actual user being unable to
      ** offer the intended services.
      */
   }
   else
   {
      bRet = BOS_FALSE;
   }

   CCTK_TRACED2( "CeResUser (%p) - Resource creation: %s.",
                 this,
                 ( bRet ? "Success" : "Fail" ));
   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceBasicCtlEqual
**
**  PURPOSE:    Checks whether a resource is actually associated with a given
**              basic extension controller.
**
**  PARAMETERS:
**              pBasicExtCtl - the basic extension controller to lookup.
**              bAssRes - set to BOS_TRUE if this is the associated resource
**                        that we are dealing with, BOS_FALSE otherwise.
**
**
**  RETURNS:    BOS_TRUE if this resource owns this controller, BOS_FALSE
**              otherwise.
**
**  NOTES:      The associated resource in the case of a user resource is the
**              resource allocated for registration event package subscription
**              which is an internal resource of the SCE support which is to be
**              associated with the user (there is no CE resource allocated for
**              this subscription).
***********************************************************************************/
BOS_BOOL CeResUser::resourceBasicCtlEqual(
                        IN MXD_GNS::ISceBasicExtensionControl *pBasicExtCtl,
                        BOS_BOOL &bAssRes )
{
   MXD_GNS::ISceBasicExtensionControl *pMyBasicExtCtl = NULL;
   BOS_BOOL bRet = BOS_FALSE;

   bAssRes = BOS_FALSE;

   if ( mpUaSspReg )
   {
      mxt_result res = mpUaSspReg->QueryIf( &pMyBasicExtCtl );
      if ( MX_RIS_S( res ) &&
           ( pMyBasicExtCtl != NULL ) )
      {
         if ( pMyBasicExtCtl == pBasicExtCtl )
         {
            bRet = BOS_TRUE;
         }

         pMyBasicExtCtl->ReleaseIfRef();
         pMyBasicExtCtl = NULL;
      }

      /* Check also whether this is the interface for the internal
      ** subscriber.
      */
      if ( !bRet )
      {
         res =
            mpUaSspReg->QuerySubscriber(
                              MXD_GNS::IID_ISceBasicExtensionControl,
                              reinterpret_cast<void**>(&pMyBasicExtCtl) );
         if ( MX_RIS_S( res ) &&
              ( pMyBasicExtCtl != NULL ) )
         {
            if ( pMyBasicExtCtl == pBasicExtCtl )
            {
               bRet = BOS_TRUE;
               bAssRes = BOS_TRUE;
            }

            pMyBasicExtCtl->ReleaseIfRef();
            pMyBasicExtCtl = NULL;
         }
      }
   }

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourcePushData
**
**  PURPOSE:    Pushes some information to the resource, information which is to
**              be used in order to properly apply the intended behavior.
**
**  PARAMETERS:
**              eEvt - the event associated with the data.
**              pData - the data of interest.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeResUser::resourcePushData( CEEVT eEvt, void *pData )
{
   if ( pData == NULL )
   {
      return;
   }

   switch ( eEvt )
   {
      case eCEEVT_REG_NET_OPTIONS:
      {
         mpServerEvtCtl =
            reinterpret_cast<MXD_GNS::ISipServerEventControl *> ( pData );
      }
      break;

      case eCEEVT_REG_USR_KA_OK:
      {
         /* If the data has changed, update our internal knowledge of it.
         */
         if ( memcmp( &mKaInfo,
                      pData,
                      sizeof ( CCTKKAINFO ) ) )
         {
            memcpy( &mKaInfo,
                    pData,
                    sizeof ( CCTKKAINFO ) );
            mbKaNew = BOS_TRUE;
         }
         else
         {
            mbKaNew = BOS_FALSE;
         }
      }
      break;

      case eCEEVT_REG_USR_PUBLISH:
      {
         mbPublishMetrics =
            ((static_cast<TKMPUBINFO *> ( pData ))->ePubType
                                             == eCCTKPUBLISH_CALLMETRICS);
         muPubedGuid = (static_cast<const TKMPUBINFO *> ( pData ))->uGuid;
         mpPubedParty = (static_cast<const TKMPUBINFO *> ( pData ))->pcSubAddr;
      }
      break;

      case eCEEVT_REG_USR_MSG:
      {
         mpMsgParty = (static_cast<const TKMMSGINFO *> ( pData ))->pcMsgAddr;
         mpMsgData = (static_cast<const TKMMSGINFO *> ( pData ))->pcMsgBody;
         muMsgSize = (static_cast<const TKMMSGINFO *> ( pData ))->uSize;
         muMsgEvtGuid = (static_cast<const TKMMSGINFO *> ( pData ))->uGuid;
      }
      break;

      case eCEEVT_REG_NET_REFENDED:
      case eCEEVT_REG_NET_REFER:
      {
         mpCurReferrer = static_cast<MXD_GNS::ISceReferrer *> ( pData );
      }
      break;

      case eCEEVT_REG_NET_NEXT_NONCE:
      {
         mChallengeId = *(static_cast<mxt_opaque *> ( pData ));
      }
      break;

      case eCEEVT_REG_USR_SVC_ENA:
      case eCEEVT_REG_USR_SVC_DIS:
      {
         mSvcCtrl = *(static_cast<CCTKSUBEVT *> ( pData ));
      }
      break;

      case eCEEVT_REG_USR_ADD_OK:
      {
         mResetRoute =
             (pData != NULL) ? *(static_cast<BOS_BOOL *> ( pData )) : BOS_FALSE;
      }
      break;

      default:
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceNewRegId
**
**  PURPOSE:    Assigns a new registration identifier for this user, this is to
**              ensure uniqueness of the identifier accross multiple registration.
**
**  PARAMETERS:
**
**
**  RETURNS:    Nothing
**
**  NOTES:      Only invoke this API when the user is 'unregistered' since the
**              identifier must only be unique accross multiple registration, but
**              not accross refresh of the same registration.
***********************************************************************************/
void CeResUser::resourceNewRegId( void )
{
   if ( ++muRegId > guCeInvalidGuid )
   {
      /* Reset the registration identifier when we pass a threshold.
      */
      muRegId = 1;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceSetReferrer
**
**  PURPOSE:    Assigns a referrer to a user as a placeholder for transfer service.
**
**  PARAMETERS:
**              pReferrer - the referrer handler of interest.
**
**
**  RETURNS:    BOS_TRUE if we could assign the referrer to this user.
**              BOS_FALSE otherwise.
**
**  NOTES:      The referrer is used for out of dialog transfer, in order to start
**              such process, we need to keep knowledge of the referrer until we
**              are ready to pass it on to the appropriate call.  When this happens
**              the ownership of the referrer will go to the call rather than the
**              user.
***********************************************************************************/
BOS_BOOL CeResUser::resourceSetReferrer( MXD_GNS::ISceReferrer *pReferrer )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpReferrer[uIx] == NULL )
      {
         mpReferrer[uIx] = pReferrer;
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceUnsetReferrer
**
**  PURPOSE:    Deassigns a referrer from a user.  Ownership has been given to the
**              proper call resource.
**
**  PARAMETERS:
**              pReferrer - the referrer handler to take care of.
**
**
**  RETURNS:    BOS_TRUE if we could assign the referrer to this user.
**              BOS_FALSE otherwise.
**
**  NOTES:      The referrer is used for out of dialog transfer, in order to start
**              such process, we need to keep knowledge of the referrer until we
**              are ready to pass it on to the appropriate call.  When this happens
**              the ownership of the referrer will go to the call rather than the
**              user.
***********************************************************************************/
void CeResUser::resourceUnsetReferrer( MXD_GNS::ISceReferrer *pReferrer )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( ( mpReferrer[uIx] != NULL ) &&
           ( mpReferrer[uIx] == pReferrer ) )
      {
         mpReferrer[uIx] = NULL;
         if ( mpCurReferrer == pReferrer )
         {
            mpCurReferrer = NULL;
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceReferEnd
**
**  PURPOSE:    Terminates the knowledge of a referrer at the user resource
**              level.
**
**  PARAMETERS:
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void CeResUser::resourceReferEnd( void )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( ( mpReferrer[uIx] != NULL ) &&
           ( mpReferrer[uIx] == mpCurReferrer ) )
      {
         mpReferrer[uIx]->ReleaseIfRef();
         mpReferrer[uIx] = NULL;

         mpCurReferrer = NULL;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceSetAssumedId
**
**  PURPOSE:    Allocates the 'assumed identity' that are associated with a user
**              an assumed identity is an identity associated with this user that
**              is not the primary identity of the user but that can be used later
**              on by the user when sending or receiving requests.
**
**  PARAMETERS:
**              pPAURI - the 'P-Associated-URI' (RFC 3455) header which contains
**                       the assumed identity information that we want to preserve
**                       and associate with this user.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void CeResUser::resourceSetAssumedId( const MXD_GNS::CSipHeader* pPAURI )
{
   BOS_UINT32 uIx;
   BOS_BOOL bAssumedId = ( CCTKCFG_MAX_ASSUMED_ID != 0 );

   /* Reset all assumed identity first.
   */
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_ASSUMED_ID ; uIx++ )
   {
      mAssumedId[ uIx ].Reset();
   }

   /* Populate the new assumed identities that this user can assume.
   */
   if ( ( pPAURI != NULL ) && bAssumedId )
   {
      uIx = 0;

      do
      {
         mAssumedId[ uIx++ ] = pPAURI->GetPAssociatedUri();
         pPAURI = pPAURI->GetNextHeader();
      }
      while( ( pPAURI != NULL ) && ( uIx < CCTKCFG_MAX_ASSUMED_ID ) );
   }
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceGetAssumedId
**
**  PURPOSE:    Retrieves an allocated assumed identity for this user.
**
**  PARAMETERS:
**              uIx - the user assumed identity index
**              rIdentity - reference to the identity to retrieve
**
**
**  RETURNS:    Nothing, but the identity will be updated accordingly.
**
**  NOTES:      .
***********************************************************************************/
void CeResUser::resourceGetAssumedId( BOS_UINT32 uIx,
                                      MXD_GNS::CNameAddr& rIdentity )
{
   MXD_GNS::CNameAddr assumedId;
   BOS_BOOL bAssumedId = ( CCTKCFG_MAX_ASSUMED_ID != 0 );

   assumedId.Reset();

   if ( bAssumedId && ( uIx < CCTKCFG_MAX_ASSUMED_ID ) )
   {
      assumedId = mAssumedId[ uIx ];
   }

   rIdentity = assumedId;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceIsAssumedId
**
**  PURPOSE:    Terminates the knowledge of a referrer at the user resource
**              level.
**
**  PARAMETERS: 
**              rIdentity - the identity we want to check on.
**
**
**  RETURNS:    BOS_TRUE if the passed in identity is an assumed identity for the
**              user.  BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL CeResUser::resourceIsAssumedId( const MXD_GNS::CNameAddr& rIdentity )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_ASSUMED_ID ; uIx++ )
   {
      if ( /* Compare the full name-addr value.
           */
           ( mAssumedId[ uIx ] == rIdentity ) ||
           /* Compare also just the URI value within the name-addr just in case
           ** those match as well.
           */
           ( mAssumedId[ uIx ].GetUri() == rIdentity.GetUri() ) )
      {
         /* There is a match, this user is the one assuming this identity.
         */
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceReleaseChallenge
**
**  PURPOSE:    Release internal component (challenge information cache).
**
**  PARAMETERS:
**              None
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
void CeResUser::resourceReleaseChallenge( void )
{
   if ( resourceChallengeId() )
   {
      MXD_GNS::ISceUserAuthentication *pSceUsrAuth;
      MXD_GNS::ISceUserConfig *pSceUsrCfg = NULL;
      mxt_result res;

      resourceNetHdl()->GetConfiguration( pSceUsrCfg );
      if ( pSceUsrCfg != NULL )
      {
         res = pSceUsrCfg->QueryIf( &pSceUsrAuth );
         if ( MX_RIS_S( res ) && ( pSceUsrAuth != NULL ) )
         {
            pSceUsrAuth->DeleteChallengeInfo( resourceChallengeId() );
            mChallengeId = 0;

            pSceUsrAuth->ReleaseIfRef();
            pSceUsrAuth = NULL;
         }

         pSceUsrCfg->ReleaseIfRef();
         pSceUsrCfg = NULL;
      }
   }
}
/***********************************************************************************
**  FUNCTION:   CeResUser::resourceResetAllChallengeInfo
**
**  PURPOSE:    Clears out the challenge information and resets the
**              authentication state.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
void CeResUser::resourceResetAllChallengeInfo( void )
{
   MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl = NULL;
   mxt_result res;
   
   res = resourceNetHdl()->QueryIf( &pSceBasicExtCtl );
   if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
   {
      pSceBasicExtCtl->ResetChallengeInfo();
      
      pSceBasicExtCtl->ReleaseIfRef();
      pSceBasicExtCtl = NULL;
   }
   
   resourceReleaseChallenge();
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceShow
**
**  PURPOSE:    Shows (displays via log) information about the current resource.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
void CeResUser::resourceShow( void )
{
#if CCTKCFG_APPLICATION_STATUS
   BOS_UINT32 uIx;

   /* Invoke base class first to display the generic information about
   ** this resource.
   */
   CeResBase::resourceShow();

   /* Display the class specific data associated with this resource.
   */
#if CCTKCFG_REMOTE_USER_CONTROL
   CCTK_TRACEI1( "Remote [%d]",
                 mbRemote );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
   
   CCTK_TRACEI2( "UaSsp [%p], SceEvt [%p]",
                 mpUaSspReg,
                 mpServerEvtCtl );
   CCTK_TRACEI2( "Mwi [%p], BMwi [%p]",
                 mpMwiSub,
                 mpBlindMwi );
   CCTK_TRACEI1( "SubReg [0x%lX]",
                 muSubRegGuid );
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_PROFCFGLST ; uIx++ )
   {
      CCTK_TRACEI2( "Uap (%d) [%p]",
                    uIx,
                    mpUapSub[ uIx ] );
   }
   CCTK_TRACEI1( "TmrBkupReco [0x%lX]",
                 muTmrBkupReco);
   CCTK_TRACEI2( "BkupPar [0x%lX], BkupCfg [0x%lX]",
                 muBkupParent,
                 muBkupConfig );

   CCTK_TRACEI2( "CaIn [%p], CaOut [%p]",
                 mpLastCallIn,
                 mpLastCallOut );

   CCTK_TRACEI1( "Resource [0x%lX] -- END.",
                 resourceGuid() );
#endif /* CCTKCFG_APPLICATION_STATUS */
}


/***********************************************************************************
**  FUNCTION:   resourceReferrerLookupBasicCtl
**
**  PURPOSE:    Checks whether a resource is associated with a given basic
**              extention controller for a specific referrer
**
**  PARAMETERS:
**              pReferrerBasicExtCtl - the basic extension controller associated
**                                     with a specific referrer to lookup
**
**  RETURNS:    the GUID associated with the resources if found.  
**              guCeInvalidGuid otherwise
**
**  NOTES:
***********************************************************************************/
BOS_UINT32 CeResUser::resourceReferrerLookupBasicCtl( 
                        IN MXD_GNS::ISceBasicExtensionControl *pReferrerBasicExtCtl )
{
   BOS_UINT32 uIx;
   mxt_result res;
   BOS_UINT32  uGuid = guCeInvalidGuid;
   MXD_GNS::ISceBasicExtensionControl *pBasicExtCtl = NULL;

   /* Check against the current user referrer first.  The current user referrer
   ** may not have been setup if the associated resource has just been created
   */
   if ( mpCurReferrer != NULL )
   {
      res = mpCurReferrer->QueryIf( &pBasicExtCtl );      
      if ( MX_RIS_S( res ) && ( pBasicExtCtl != NULL ) )
      {
         if ( pBasicExtCtl == pReferrerBasicExtCtl )
         {
            uGuid = resourceGuid();
         }

         pBasicExtCtl->ReleaseIfRef();
         pBasicExtCtl = NULL;
      }
   }

   /* Current referrer not available.  Iterate through the complete referrer list
   ** for a match.
   */
   if ( uGuid == guCeInvalidGuid )
   {
      for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
      {
         if ( mpReferrer[uIx] != NULL )
         {
            pBasicExtCtl = NULL;
            res = mpReferrer[uIx]->QueryIf( &pBasicExtCtl );      
            if ( MX_RIS_S( res ) && ( pBasicExtCtl != NULL ) )
            {
               if ( pBasicExtCtl == pReferrerBasicExtCtl )
               {
                  uGuid = resourceGuid();
               }

               pBasicExtCtl->ReleaseIfRef();
               pBasicExtCtl = NULL;

               if ( uGuid != guCeInvalidGuid )
               {
                  break;
               }
            }
         }
      }
   }

   return ( uGuid );
}

/***********************************************************************************
**  FUNCTION:   CeResUser::resourceSelectNewObProxy
**
**  PURPOSE:    Consults the backup and recovery service and configures a new
**              outbound proxy (if available) for the user.
**
**  PARAMETERS:
**
**
**  RETURNS:    BOS_TRUE if a new outbound proxy was chosen, BOS_FALSE otherwise.
**
**  NOTES:      This is to be used when the backup and recovery service is in
**              external list mode. There are other mechanisms in place to direct
**              future signaling in the eCCTKBRSVCMODE_DLM.
***********************************************************************************/
BOS_BOOL CeResUser::resourceSelectNewObProxy( void )
{
   const CFGUSR *pUsrCfg = CFG_USR( resourceGuid() );
   
   /* Find the next available target, and reset the user's
   ** outbound proxy to it, so that all future signalling
   ** will be generated to it.
   */
   if ( pUsrCfg )
   {
      BOS_UINT8 newAddr[ CCTK_STR_LEN ];
      MXD_GNS::ISceUserConfig *pSceCfg = pUsrCfg->in.pSceUsrCfg;

      if ( pSceCfg != NULL )
      {
         CCTKROUTELST obProxyLst;
         memcpy( &obProxyLst,
                 &pUsrCfg->xtl.usr.net.obProxyLst,
                 sizeof( CCTKROUTELST ) );
         
         /* Ask the backup and recovery module to choose the
         ** next target based on it's previous knoweledge of
         ** attempted targets.
         */
         if ( nsBrGetAvailTgt( resourceGuid(),
                               obProxyLst.route[0].addr.hostPort ) )
         {
            /* Found a new candidate, update the outbound
            ** proxy for this user.
            */            
            CCTK_TRACEI3( "Reconfiguring outbound proxy 0x%X - %s:%u",
                           resourceGuid(), 
                           obProxyLst.route[0].addr.hostPort.pcAddr,
                           obProxyLst.route[0].addr.hostPort.uPort );

            tkmSetParm ( resourceGuid(),
                         eCCTKPARM_MC_OBPROXYLST,
                         reinterpret_cast<const void *> ( &obProxyLst ) );

            return BOS_TRUE;
         }
      }
   }

   return BOS_FALSE;
}