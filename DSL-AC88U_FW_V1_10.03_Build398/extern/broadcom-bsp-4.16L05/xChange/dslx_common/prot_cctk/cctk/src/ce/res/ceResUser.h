/**********************************************************************************
** Copyright (c) 2007-2012 Broadcom
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
**      This file defines the user class of the resource object that is managed
**      by the Call Engine.
**
***********************************************************************************/
#ifndef __CE_RES_USER__H__INCLUDED__
#define __CE_RES_USER__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceResBase.h>
#include <ceCdr.h>
#include <ceResSub.h>

/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

/*
** This class defines the user resource.
*/
class CeResUser: virtual public CeResBase
{
   public:

      CeResUser( void );
      ~CeResUser( void );

      BOS_BOOL                            resourceCreate( BOS_UINT32 uCfgGuid );
      BOS_BOOL                            resourceTerminate( void );

      BOS_BOOL  resourceEqual( MXD_GNS::IUaSspRegistration *pRegistration );
      BOS_BOOL  resourceOwner( MXD_GNS::ISceReferrer *pReferrer );

      MXD_GNS::IUaSspRegistration         *resourceNetHdl( void );

      inline void resourceSetRealm( const MXD_GNS::CString &rRealm )
                                                    { mRealm = rRealm; };
      inline void resourceGetRealm( MXD_GNS::CString &rRealm )
                                                    { rRealm = mRealm; };
      inline BOS_UINT32 resourceRegId( void ) { return muRegId; };
      void resourceNewRegId( void );

      inline MXD_GNS::ISipServerEventControl *resourceGetSrvCtl( void )
                                                    { return mpServerEvtCtl; };

      BOS_BOOL resourceBasicCtlEqual( 
                  IN MXD_GNS::ISceBasicExtensionControl *pBasicExtCtl,
                  BOS_BOOL &bAssRes );

      void resourcePushData( CEEVT eEvt, void *pData );

      inline CeResBase *resourceGetMwiSub( void ) { return mpMwiSub; };
      inline CeResBase *resourceGetBlindMwi( void ) { return mpBlindMwi; };
      inline void resourceSetMwiSub( CeResBase *pRes ) { mpMwiSub = pRes; };
      inline void resourceSetBlindMwi( CeResBase *pRes ) { mpBlindMwi = pRes; };

      inline CeResBase *resourceGetUapSub( BOS_UINT32 uIx )
                                                { return mpUapSub[uIx]; };
      inline void resourceSetUapSub( BOS_UINT32 uIx,
                                     CeResBase *pRes )
                                                { mpUapSub[uIx] = pRes; };

      inline void resourceSetLastCall( CeCdr *pCdr, CeCdr::CECDRTYPE eType );
      inline CeCdr *resourceGetLastCall( CeCdr::CECDRTYPE eType );

      inline void resourceResetKaInfo( void ) { mbKaNew = BOS_FALSE; };
      inline BOS_BOOL resourceNewKaInfo( void ) { return mbKaNew; };
      inline CCTKKAINFO *resourceKaInfo( void ) { return &mKaInfo; };

      inline BOS_BOOL resourcePublishMetrics( void ) { return mbPublishMetrics; };
      inline BOS_BOOL resourceGetAuthChg( void ) { return mbAuthChg; };
      inline BOS_BOOL resourceGetNoSub( void ) { return mbNoSub; };
      inline BOS_UINT8 *resourcePubParty( void ) { return mpPubedParty; };
      inline BOS_UINT32 resourcePubGuid( void ) { return muPubedGuid; };
      inline void resourceSetPublishMetrics( BOS_BOOL bMetrics )
                                               { mbPublishMetrics = bMetrics; };
      inline void resourceSetAuthChg( BOS_BOOL bAuthChg )
                                               { mbAuthChg = bAuthChg; };
      inline void resourceSetNoSub( BOS_BOOL bNoSub )
                                               { mbNoSub = bNoSub; };
      BOS_BOOL resourceSetReferrer( MXD_GNS::ISceReferrer *pReferrer );
      void resourceUnsetReferrer( MXD_GNS::ISceReferrer *pReferrer );
      void resourceSetReferTo( const MXD_GNS::CNameAddr& rReferTo );
      void resourceSetTgtDlg( MXD_GNS::IEComUnknown* pTargetDialog );
      void resourceReferEnd( void );
      void resourceReferClean( void );
      inline MXD_GNS::ISceReferrer *resourceGetReferrer( void )
                                                      { return mpCurReferrer; };
      inline MXD_GNS::IEComUnknown *resourceGetTgtDlg( void )
                                                      { return mpTargetDialog; };
      inline MXD_GNS::CNameAddr *resourceGetReferTo( void ) { return &mReferTo; };
      void resourceSetAssumedId( const MXD_GNS::CSipHeader* pPAURI );
      void resourceGetAssumedId( BOS_UINT32 uIx, MXD_GNS::CNameAddr& rIdentity );
      BOS_BOOL resourceIsAssumedId( const MXD_GNS::CNameAddr& rIdentity );
      inline BOS_UINT32 resourceSubRegGuid( void ) { return muSubRegGuid; };
      inline void resourceSetSubRegGuid( BOS_UINT32 uGuid )
                                             { muSubRegGuid = uGuid; };
      inline BOS_UINT32 resourceTmrBkupReco( void ) { return muTmrBkupReco; };
      inline void resourceSetTmrBkupReco( BOS_UINT32 uGuid )
                                             { muTmrBkupReco = uGuid; };
      inline BOS_BOOL resourceFailbkPrim( void ) { return mbFailbkPrim; };
      inline void resourceSetFailbkPrim( BOS_BOOL bFailbkPrim )
                                             { mbFailbkPrim = bFailbkPrim; };
      inline BOS_UINT32  resourceTmrFailbkPrimReco( void ) { return muTmrFailbkPrim; };
      inline void resourceSetTmrFailbkPrimReco( BOS_UINT32 uGuid )
                                             { muTmrFailbkPrim = uGuid; };

      inline BOS_UINT32 resourceTmrKeepAliveReco( void ) { return muTmrKeepAlive; };
      inline void resourceSetTmrKeepAliveReco( BOS_UINT32 uGuid )
                                             { muTmrKeepAlive = uGuid; };

      inline MXD_GNS::CNameAddr *resourceGetPauId( void )
                                                      { return &mPauId; };
      void resourceSetPauId( const MXD_GNS::CNameAddr& rPauId );

      inline MXD_GNS::CNameAddr *resourceGetRegPath( void )
                                                      { return &mRegPath; };
      inline void resourceSetRegPath( const MXD_GNS::CNameAddr& rPath )
                                             { mRegPath = rPath; };

      inline BOS_UINT32 resourceBkupParent( void ) { return muBkupParent; };
      inline void resourceSetBkupParent( BOS_UINT32 uGuid )
                                             { muBkupParent = uGuid; };
      inline BOS_UINT32 resourceBkupConfig( void ) { return muBkupConfig; };
      inline void resourceSetBkupConfig( BOS_UINT32 uGuid )
                                             { muBkupConfig = uGuid; };
      inline void resourceUserUnreg( BOS_BOOL bUnreg ) { mbUsrUnreg = bUnreg; };
      inline BOS_BOOL resourceUserUnreg( void ) { return mbUsrUnreg; };
      inline mxt_opaque resourceChallengeId( void ) { return mChallengeId; };
      void resourceReleaseChallenge( void );
      void resourceResetAllChallengeInfo( void );
      inline BOS_UINT32 resourceNonceCount( void ) { return muNonceCount; };
      void resourceSetNounceCount( BOS_UINT32 uNonceCount )
                                                { muNonceCount = uNonceCount; };
      inline void resourceSetNonce( const MXD_GNS::CString &rNonce )
                                                    { mNonce = rNonce; };
      inline void resourceGetNonce( MXD_GNS::CString &rNonce )
                                                    { rNonce = mNonce; };
      inline void resourceSwap4PauId( BOS_BOOL bSwap ) { mbSwap4PauId = bSwap; };
      inline BOS_BOOL resourceSwap4PauId( void ) { return mbSwap4PauId; };
      inline void resourceLifeEnding( BOS_BOOL bLifeEnd )
                                                { mbLifeEnding = bLifeEnd; };
      inline BOS_BOOL resourceLifeEnding( void ) { return mbLifeEnding; };
      inline void resourceReplacingGuid( BOS_UINT32 uGuid )
                                                { muReplacingGuid = uGuid; };
      inline BOS_UINT32 resourceReplacingGuid( void ) { return muReplacingGuid; };
      inline CCTKSUBEVT resourceSvcCtrl( void ) { return mSvcCtrl; };
      inline BOS_BOOL resourceResetRoute( void ) { return mResetRoute; };
      inline void resourceTargetFailed( BOS_BOOL bTargetFailed ) { mbTargetFailed = bTargetFailed; };
      inline BOS_BOOL resourceTargetFailed( void ) { return mbTargetFailed; };
      inline void resourceSuccessfulRegCycle( BOS_BOOL bSuccessRegCycle ) { mbSuccessRegCycle = bSuccessRegCycle; };
      inline BOS_BOOL resourceSuccessfulRegCycle( void ) { return mbSuccessRegCycle; };
      inline void resourceApplyBkoffDelay( BOS_BOOL bApplyBkoffDelay ) { mbApplyBkoffDelay = bApplyBkoffDelay; };
      inline BOS_BOOL resourceApplyBkoffDelay( void ) { return mbApplyBkoffDelay; };

      BOS_UINT32 resourceReferrerLookupBasicCtl( IN MXD_GNS::ISceBasicExtensionControl *pBasicExtCtl );
      inline void resourceSetRemUsr( BOS_BOOL bRemote ) { mbRemote = bRemote; };
      inline BOS_BOOL resourceIsRemUsr( void ) { return mbRemote; };

      inline BOS_UINT8 *resourceMsgParty( void ) { return mpMsgParty; };
      inline BOS_UINT8 *resourceMsgData( void ) { return mpMsgData; };
      inline BOS_UINT32 resourceMsgSize( void ) { return muMsgSize; };
      inline BOS_UINT32 resourceMsgNotGuid( void ) { return muMsgEvtGuid; };

      void resourceResetRegExp( void );
      void resourceSetRegExp( CCTKEXPIREINFO &rExpInfo );
      inline CCTKEXPIREINFO *resourceGetRegExp( void ){ return &mExpInfo; };

      inline MXD_GNS::CSocketAddr *resourceGetInbdTgtAddr( void ) { return mpInbdTgtSockAddr; };
      inline void resourceSetInbdTgtAddr( MXD_GNS::CSocketAddr *pInbdAddr ) 
                                                { mpInbdTgtSockAddr = pInbdAddr; };
      BOS_BOOL resourceSelectNewObProxy( void );
      inline void resourceIncrReRegCount( BOS_BOOL bIncrement ) { bIncrement ? muReRegCount++ : (muReRegCount = 0); };
      inline BOS_UINT32 resourceGetReRegCount ( void ) { return muReRegCount; };
      inline void resourceUserReplace( BOS_BOOL bReplace ) { mbReplace = bReplace; };
      inline BOS_BOOL resourceUserReplace( void ) { return mbReplace; };

#if CCTKCFG_REMOTE_USER_CONTROL
      inline void resourceSetRemUsrAOR( const CCTKHOSTPORT aor )
                                                    { mRemUsrAOR = aor; };
      inline void resourceGetRemUsrAOR( CCTKHOSTPORT &rAOR )
                                                    { rAOR = mRemUsrAOR; };
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
      /* Base class virtual method overrides.
      */
      virtual void resourceShow( void );

   private:

      MXD_GNS::IUaSspRegistration         *mpUaSspReg;

      MXD_GNS::ISipServerEventControl     *mpServerEvtCtl;

      MXD_GNS::CString                    mRealm;

      BOS_UINT32                          muRegId;
      CeResBase                           *mpMwiSub;
      CeResBase                           *mpBlindMwi;
      CeResBase                           *mpUapSub[CCTKCFG_MAX_PROFCFGLST];
      CeCdr                               *mpLastCallIn;
      CeCdr                               *mpLastCallOut;
      MXD_GNS::ISceReferrer               *mpReferrer[CCTKCFG_MAX_CALL];

      CCTKKAINFO                          mKaInfo;
      BOS_BOOL                            mbKaNew;

      BOS_BOOL                            mbPublishMetrics;
      BOS_BOOL                            mbAuthChg;
      BOS_BOOL                            mbUsrUnreg;
      BOS_BOOL                            mbFailbkPrim;
      BOS_UINT8                           *mpPubedParty;
      BOS_UINT32                          muPubedGuid;

      BOS_UINT32                          muSubRegGuid;
      BOS_UINT32                          muTmrBkupReco;
      BOS_UINT32                          muBkupParent;
      BOS_UINT32                          muBkupConfig;
      BOS_UINT32                          muReRegCount;
      BOS_BOOL                            mbReplace;

      BOS_UINT32                          muTmrKeepAlive;
      BOS_UINT32                          muTmrFailbkPrim;

      MXD_GNS::ISceReferrer               *mpCurReferrer;
      MXD_GNS::IEComUnknown               *mpTargetDialog;
      MXD_GNS::CNameAddr                  mReferTo;

      MXD_GNS::CNameAddr                  mAssumedId[CCTKCFG_MAX_ASSUMED_ID];
      MXD_GNS::CNameAddr                  mPauId;
      MXD_GNS::CNameAddr                  mRegPath;
      BOS_BOOL                            mbSwap4PauId;
      BOS_BOOL                            mbLifeEnding;
      BOS_UINT32                          muReplacingGuid;

      mxt_opaque                          mChallengeId;
      BOS_UINT32                          muNonceCount;
      MXD_GNS::CString                    mNonce;

      CCTKSUBEVT                          mSvcCtrl;

      CCTKEXPIREINFO                      mExpInfo;

      BOS_BOOL                            mResetRoute;
      BOS_BOOL                            mbTargetFailed;
      BOS_BOOL                            mbSuccessRegCycle;
      BOS_BOOL                            mbApplyBkoffDelay;
      BOS_BOOL                            mbNoSub;

      BOS_BOOL                            mbRemote;

      BOS_UINT8                           *mpMsgParty;
      BOS_UINT8                           *mpMsgData;
      BOS_UINT32                          muMsgSize;
      BOS_UINT32                          muMsgEvtGuid;
      
      MXD_GNS::CSocketAddr                *mpInbdTgtSockAddr;

#if CCTKCFG_REMOTE_USER_CONTROL
      CCTKHOSTPORT                        mRemUsrAOR;
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeResUser::CeResUser
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline CeResUser::CeResUser( void ) :
   CeResBase         ( CeResBase::eCERESTYPE_USR ),
   mpUaSspReg        ( NULL ),
   mpServerEvtCtl    ( NULL ),
   muRegId           ( 1 ),          /* start from 1 as 0 is forbidden by rfc 5626 */
   mpMwiSub          ( NULL ),
   mpBlindMwi        ( NULL ),
   mpLastCallIn      ( NULL ),
   mpLastCallOut     ( NULL ),
   mbKaNew           ( BOS_FALSE ),
   mbPublishMetrics  ( BOS_FALSE ),
   mbAuthChg         ( BOS_FALSE ),
   mbUsrUnreg        ( BOS_FALSE ),
   mbFailbkPrim      ( BOS_FALSE ),
   mpPubedParty      ( NULL ),
   muPubedGuid       ( guCeInvalidGuid ),
   muSubRegGuid      ( guCeInvalidGuid ),
   muTmrBkupReco     ( guCeInvalidGuid ),
   muTmrFailbkPrim   ( guCeInvalidGuid ),
   muTmrKeepAlive    ( guCeInvalidGuid ),
   muBkupParent      ( guCeInvalidGuid ),
   muBkupConfig      ( guCeInvalidGuid ),
   muReRegCount      ( 0 ),
   mbReplace         ( BOS_FALSE ),
   mpCurReferrer     ( NULL ),
   mbSwap4PauId      ( BOS_FALSE ),
   mbLifeEnding      ( BOS_FALSE ),
   muReplacingGuid   ( guCeInvalidGuid ),
   mChallengeId      ( 0 ),
   muNonceCount      ( 1 ),
   mSvcCtrl          ( eCCTKSUBEVT_MAX ),
   mResetRoute       ( BOS_FALSE ),
   mbTargetFailed    ( BOS_FALSE ),
   mbSuccessRegCycle ( BOS_FALSE ),
   mbApplyBkoffDelay ( BOS_FALSE ),
   mbNoSub           ( BOS_FALSE ),
   mbRemote          ( BOS_FALSE ),
   mpMsgParty        ( NULL ),
   mpMsgData         ( NULL ),
   muMsgSize         ( 0 ),
   muMsgEvtGuid      ( guCeInvalidGuid ),
   mpInbdTgtSockAddr ( NULL )
{
   BOS_UINT32 uIx;
   for ( uIx = 0 ; uIx < CCTKCFG_MAX_PROFCFGLST ; uIx++ )
   {
      mpUapSub[uIx] = NULL;
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      mpReferrer[uIx] = NULL;
   }

   memset( &mKaInfo, 0, sizeof( mKaInfo ) );
   memset( &mExpInfo, 0, sizeof( mExpInfo) );

   resourceSetAddress( (void *) this );
}


/***********************************************************************************
**  FUNCTION:   CeResUser::~CeResUser
**
**  PURPOSE:    Class destructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline CeResUser::~CeResUser( void )
{
   resourceTerminate();
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceTerminate
**
**  PURPOSE:    Terminates a resouce usage.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    BOS_TRUE (always...)
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResUser::resourceTerminate( void )
{
   BOS_UINT32 uIx;

   resourceReleaseChallenge();

   if ( mpUaSspReg )
   {
      mpUaSspReg->ReleaseIfRef();
      mpUaSspReg = NULL;
   }
   
   if ( mpMwiSub )
   {
      if ( mpMwiSub->resourceGender() == CeResBase::eCERESTYPE_SUB )
      {
         CeResSub *pSub = reinterpret_cast<CeResSub *>
            ( mpMwiSub->resourceAddress() );
         if ( pSub->resourceMwiHdl() )
         {
            pSub->resourceMwiHdl()->Terminate();
         }
         else
         {
            pSub->resourceTerminate();
         }
      }
      mpMwiSub = NULL;
   }

   if ( mpBlindMwi )
   {
      if ( mpBlindMwi->resourceGender() == CeResBase::eCERESTYPE_SUB )
      {
         CeResSub *pSub = reinterpret_cast<CeResSub *>
            ( mpBlindMwi->resourceAddress() );
         if ( pSub->resourceBlindMwiHdl() )
         {
            pSub->resourceBlindMwiHdl()->Terminate();
         }
         else
         {
            pSub->resourceTerminate();
         }
      }
      mpBlindMwi = NULL;
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_PROFCFGLST ; uIx++ )
   {
      if ( mpUapSub[uIx] )
      {
         if ( mpUapSub[uIx]->resourceGender() == CeResBase::eCERESTYPE_SUB )
         {
            CeResSub *pSub = reinterpret_cast<CeResSub *>
               ( mpUapSub[uIx]->resourceAddress() );
            if ( pSub->resourceNotHdl() )
            {
               pSub->resourceNotHdl()->TerminateImmediately();
            }
            else
            {
               pSub->resourceTerminate();
            }
         }
         mpUapSub[uIx] = NULL;
      }
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( mpReferrer[uIx] )
      {
         mpReferrer[uIx]->ReleaseIfRef();
         mpReferrer[uIx] = NULL;
      }
   }

   /* Nothing to release here.
   */
   mpServerEvtCtl = NULL;
   mpLastCallIn = NULL;
   mpLastCallOut = NULL;
   mbPublishMetrics = BOS_FALSE;
   mbAuthChg = BOS_FALSE;
   mbUsrUnreg = BOS_FALSE;
   muReRegCount = 0;
   mbReplace    = BOS_FALSE;
   mbFailbkPrim = BOS_FALSE;
   mpPubedParty = NULL;
   muPubedGuid = guCeInvalidGuid;
   muSubRegGuid = guCeInvalidGuid;
   muTmrBkupReco = guCeInvalidGuid;
   muTmrKeepAlive = guCeInvalidGuid;
   muTmrFailbkPrim = guCeInvalidGuid;
   muBkupParent = guCeInvalidGuid;
   mpCurReferrer = NULL;
   mbRemote = BOS_FALSE;
   mReferTo.Reset();
   mPauId.Reset();
   mRegPath.Reset();
   mbSwap4PauId = BOS_FALSE;
   mbLifeEnding = BOS_FALSE;
   muReplacingGuid = guCeInvalidGuid;
   mbTargetFailed = BOS_FALSE;
   mbSuccessRegCycle = BOS_FALSE;
   mbApplyBkoffDelay = BOS_FALSE;
   mbNoSub = BOS_FALSE;
   mpMsgParty = NULL;
   mpMsgData = NULL;
   muMsgSize = 0;
   muMsgEvtGuid = guCeInvalidGuid;
   mpInbdTgtSockAddr = NULL;

   if ( muBkupConfig != guCeInvalidGuid )
   {
      cfgRemoveUsr ( muBkupConfig );
      muBkupConfig = guCeInvalidGuid;
   }

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_ASSUMED_ID ; uIx++ )
   {
      mAssumedId[ uIx ].Reset();
   }

   /* Clean up backup and recovery associated data in case any
   ** have been created for this user.
   */
   nsBrDetach( muSubRegGuid );
   nsBrDeRegister ( resourceGuid() );

   resourceLiberate();

   return BOS_TRUE;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceEqual
**
**  PURPOSE:    Checks whether this is the resource we are looking for based on
**              the UA handle (unique for a given resource).
**
**  PARAMETERS:
**              pRegistration   - the registration handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResUser::resourceEqual(
                              MXD_GNS::IUaSspRegistration *pRegistration )
{
   return (( mpUaSspReg == pRegistration ) ? BOS_TRUE : BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceOwner
**
**  PURPOSE:    Checks whether this resource owns this referrer
**
**  PARAMETERS:
**              pReferrer   - the referrer handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResUser::resourceOwner(
                              MXD_GNS::ISceReferrer *pReferrer )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CCTKCFG_MAX_CALL ; uIx++ )
   {
      if ( ( mpReferrer[uIx] != NULL ) &&
           ( mpReferrer[uIx] == pReferrer ) )
      {
         return BOS_TRUE;
      }
   }
   
   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceNetHdl
**
**  PURPOSE:    Returns the handle to the associated network resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The handle of interest (warning: may be NULL).
**
**  NOTES:
***********************************************************************************/
inline MXD_GNS::IUaSspRegistration *CeResUser::resourceNetHdl( void )
{
   return mpUaSspReg;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceSetLastCall
**
**  PURPOSE:    Associates the last call information to this user.
**
**  PARAMETERS:
**              pCdr - the pointer to the CDR of the call.
**              eType - whether this is for an incoming or an outgoing call.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResUser::resourceSetLastCall( CeCdr *pCdr, CeCdr::CECDRTYPE eType )
{
   switch ( eType )
   {
      case CeCdr::eCECDRTYPE_IN:
      {
         mpLastCallIn = pCdr;
      }
      break;

      case CeCdr::eCECDRTYPE_OUT:
      {
         mpLastCallOut = pCdr;
      }
      break;
   
      default:
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceGetLastCall
**
**  PURPOSE:    Retrieves the the last call information from this user.
**
**  PARAMETERS:
**              eType - whether this is the last incoming or outgoing call.
**
**
**  RETURNS:    The pointer to the last call recorded, note that this can be
**              NULL.
**
**  NOTES:
***********************************************************************************/
inline CeCdr *CeResUser::resourceGetLastCall( CeCdr::CECDRTYPE eType )
{
   CeCdr *pCdr = NULL;
   
   switch ( eType )
   {
      case CeCdr::eCECDRTYPE_IN:
      {
         pCdr = mpLastCallIn;
      }
      break;

      case CeCdr::eCECDRTYPE_OUT:
      {
         pCdr = mpLastCallOut;
      }
      break;
   
      default:
      break;
   }

   return pCdr;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceSetReferTo
**
**  PURPOSE:    Saves the refer to information to be kept for transfer service
**              activation.
**
**  PARAMETERS:
**              rReferTo - the refer to address.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResUser::resourceSetReferTo( const MXD_GNS::CNameAddr& rReferTo )
{
   mReferTo = rReferTo;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceSetTgtDlg
**
**  PURPOSE:    Saves the target dialog information to be kept for transfer service
**              activation.
**
**  PARAMETERS:
**              pTargetDialog - the target dialog.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResUser::resourceSetTgtDlg( MXD_GNS::IEComUnknown* pTargetDialog )
{
   mpTargetDialog = pTargetDialog;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceReferClean
**
**  PURPOSE:    Cleans up the temporary information used for transfer service.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResUser::resourceReferClean( void )
{
   mpTargetDialog = NULL;
   mReferTo.Reset();
   mpCurReferrer = NULL;
}


/***********************************************************************************
**  FUNCTION:   CeResUser::resourceSetPauId
**
**  PURPOSE:    Saves the P-Associated-URI information to be kept for user
**              identification update.
**
**  PARAMETERS:
**              rPauId - the P-Associated-URI information.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResUser::resourceSetPauId( const MXD_GNS::CNameAddr& rPauId )
{
   mPauId = rPauId;
}

/***********************************************************************************
**  FUNCTION:   CeResUser::resourceSetRegExp
**
**  PURPOSE:    Saves the expiration time and re-registration time information
**
**  PARAMETERS:
**              rExpInfo - the expiration information.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResUser::resourceSetRegExp( CCTKEXPIREINFO &rExpInfo )
{
   memcpy( &mExpInfo,
           &rExpInfo,
           sizeof( CCTKEXPIREINFO ) );
}

/***********************************************************************************
**  FUNCTION:   CeResUser::resourceResetRegExp
**
**  PURPOSE:    Reset the mExpInfo
**
**  PARAMETERS:
**
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResUser::resourceResetRegExp ()
{
   memset ( static_cast<void *> (&mExpInfo),
            0,
            sizeof ( CCTKEXPIREINFO ) );
}

#endif /* __CE_RES_USER__H__INCLUDED__ */
