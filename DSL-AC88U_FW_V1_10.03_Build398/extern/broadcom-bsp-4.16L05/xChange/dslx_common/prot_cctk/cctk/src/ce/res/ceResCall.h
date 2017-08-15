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
**      This file defines the call class of the resource object that is managed
**      by the Call Engine.
**
***********************************************************************************/
#ifndef __CE_RES_CALL__H__INCLUDED__
#define __CE_RES_CALL__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceResBase.h>

/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */


/*
** This class defines the user resource.
*/
class CeResCall: virtual public CeResBase
{
   public:

      CeResCall( void );
      ~CeResCall( void );

      BOS_BOOL               resourceCreate( BOS_UINT32 uCfgGuid,
                                             MXD_GNS::IUaSspCall *pCall = NULL );
      BOS_BOOL               resourceTerminate( void );

      BOS_BOOL               resourceEqual( MXD_GNS::IUaSspCall *pCall );
      BOS_BOOL               resourceEqualFork( MXD_GNS::IUaSspCall *pCall );
      BOS_BOOL               resourceOwner( MXD_GNS::ISceReferrer *pRefer );
      void                   resourceEnd( void );

      MXD_GNS::IUaSspCall    *resourceNetHdl( void );
      MXD_GNS::ISceReferrer  *resourceNetReferrer( void );

      inline void            resourceSetStatus( BOS_UINT16 uStatus )
                                             { muOutCallStatus = uStatus; };
      inline BOS_UINT16      resourceGetStatus( void )
                                             { return muOutCallStatus; };

      inline void            resourceSetCall( const MXD_GNS::CNameAddr &rCall )
                                             { mCall = rCall; };
      inline void            resourceGetCall( MXD_GNS::CNameAddr &rCall )
                                             { rCall = mCall; };

      inline void            resourceSetSvc( const MXD_GNS::CNameAddr &rCallSvc )
                                             { mCallSvc = rCallSvc; };
      inline void            resourceGetSvc( MXD_GNS::CNameAddr &rCallSvc )
                                             { rCallSvc = mCallSvc; };

      inline void            resourceSetReferrer( MXD_GNS::ISceReferrer *pReferrer )
                                             { mpSceReferrer = pReferrer; };

      inline void            resourceSetLink( MXD_GNS::IUaSspCall *pCall )
                                             { mpUaSspLinkedCall = pCall; };
      inline void            resourceGetLink( MXD_GNS::IUaSspCall *&prCall )
                                             { prCall = mpUaSspLinkedCall; };

      inline void            resourceSetMse( MXD_GNS::MseSdpSession *pMse )
                                             { mpMse = pMse; };
      inline void            resourceGetMse( MXD_GNS::MseSdpSession *&prMse )
                                             { prMse = mpMse; };

      inline void            resourceSetCallInfo( CCTKCALLINFO *pCallInfo );
      
      inline void            resourceSetPrivCfgInfo ( CCTKPRIVCFG *pPrivCfg );
      inline CCTKPRIVCFG *   resourceGetPrivCfgInfo( void ) { return &mPrivCfgInfo; }; 
      
      void                   resourcePushData( CEEVT eEvt, void *pData );

      inline BOS_UINT8 *     resourceCalledParty( void )
                                                    { return mpCalledParty; };
      inline BOS_UINT8 *     resourceForwardParty( void )
                                                    { return mpForwardToParty; };
      inline CCTKCALLINFO *  resourceCallingParty( void ) { return &mCallInfo; };
      inline CCTKCALLDELRSN  resourceCallEndReason( void )
                                                    { return mCallDelReason; };
      inline CCTKMUPDRSN     resourceCallMediaReason( void )
                                                    { return mCallMediaReason; };
      inline void *          resourceAddtionalCallData( void )
                                                 { return mpAdditionalCallData; };
      inline BOS_UINT32      resourceXferGuid( void ) { return muXferGuid; };
      inline void            resourceSetXferGuid( BOS_UINT32 uResGuid )
                                                { muXferGuid = uResGuid; };
      inline BOS_UINT32      resourceXferTimerGuid( void )
                                                { return muXferTimerGuid; };
      inline void            resourceSetXferTimerGuid( BOS_UINT32 uResGuid )
                                                { muXferTimerGuid = uResGuid; };
      inline BOS_BOOL        resourceIsXferer( void ) { return mbXferer; };
      inline BOS_BOOL        resourceNoXferTerm( void ) { return mbNoXferTerm; };
      inline void            resourceSetLocalTerm( void )
                                                { mbLocalTerm = BOS_TRUE; };
      inline BOS_BOOL        resourceLocalTerm( void ) { return mbLocalTerm; };
      inline BOS_BOOL        resourceIsConnected( void ) { return mbConnect; };
      inline void            resourceSetEmergRingbck( BOS_BOOL bEmergRingbck )
                                             { mbEmergRingbck = bEmergRingbck; };
      inline BOS_BOOL        resourceIsEmergRingbck( void )
                                                      { return mbEmergRingbck; };

      inline BOS_UINT32      resourceProxyGuid( void ) { return muProxyGuid; };
      inline void            resourceProxyGuid( BOS_UINT32 uProxyGuid )
                                                { muProxyGuid = uProxyGuid; };
      inline BOS_UINT32      resourceUsrProxyEventData( void ) 
                                            { return muProxyGuidDataHolder; };


      inline MXD_GNS::CBlob* resourceSdpOffer( void ) { return mpSdpOffer; };
      inline void            resourceSdpOffer( MXD_GNS::CBlob *pSdpOffer )
                                                { mpSdpOffer = pSdpOffer; };
      inline MXD_GNS::CBlob* resourceSdpAnswer( void ) { return mpSdpAnswer; };
      inline void            resourceSdpAnswer( MXD_GNS::CBlob *pSdpAnswer )
                                                { mpSdpAnswer = pSdpAnswer; };

      void                   resourceSetHold( CEEVT eHold,
                                              BOS_BOOL bCurrent,
                                              BOS_BOOL bPending );
      void                   resourceGetHold( CEEVT eHold,
                                              BOS_BOOL &rbCurrent,
                                              BOS_BOOL &rbPending );
      BOS_BOOL               resourceBasicCtlEqual( 
                                 IN MXD_GNS::ISceBasicExtensionControl
                                                               *pBasicExtCtl );

      BOS_BOOL               resourceForkAdd( MXD_GNS::IUaSspCall *pCall );
      void                   resourceForkRem( MXD_GNS::IUaSspCall *pCall );
      void                   resourceForkTerm( MXD_GNS::IUaSspCall *pCall = NULL );
      void                   resourceForkTermImmediately( MXD_GNS::IUaSspCall *pCall = NULL );
      void                   resourceReferrer( void );
      void                   resourceReferEnd( void );

      inline BOS_UINT32      resourceInBandDtmf( void ) { return muInBandDtmf; };
      inline BOS_UINT32      resourceIBDtmfDur( void ) { return muIbdtmfDur; };
      inline BOS_UINT8 *     resourceHfMimeType( void ) { return mpHfMimeType; };
      inline BOS_UINT8 *     resourceHfContent( void ) { return mpHfContent; };
      inline BOS_BOOL        resourceLocalError( void ) { return mbLocalError; };
      inline void            resourceLocalError( BOS_BOOL bLclErr )
                                                   { mbLocalError = bLclErr; };
      
      void resourceSetSigRestrictedToAddr( BOS_BOOL bSigRestricted, 
                                           MXD_GNS::CSocketAddr *pRestrictAddr );
      MXD_GNS::CSocketAddr* resourceGetSigRestrictedToAddr( void );
      inline BOS_BOOL        resourceIsSigRestrictedToAddr( void ) { return mbSigRestricted; };

      /* Base class virtual method overrides.
      */
      virtual void           resourcePushState( CeFsmBase::CEFSMSTATE eNewState );
      virtual void           resourceShow( void );

   private:

      MXD_GNS::IUaSspCall    *mpUaSspCall;
      MXD_GNS::MseSdpSession *mpMse;
      void                   *mpAdditionalCallData;

      BOS_UINT16             muOutCallStatus;
      MXD_GNS::CNameAddr     mCall;
      MXD_GNS::CNameAddr     mCallSvc;
      MXD_GNS::IUaSspCall    *mpUaSspLinkedCall;
      MXD_GNS::IUaSspCall    *mpUaSspForkedCall[guCeResMaxForkedCall];
      MXD_GNS::ISceReferrer  *mpSceReferrer;

      BOS_UINT8              *mpCalledParty;
      BOS_UINT8              *mpForwardToParty;

      BOS_BOOL               mbLocalHold;
      BOS_BOOL               mbLocalHoldPending;
      BOS_BOOL               mbLocalUnHoldPending;
      BOS_BOOL               mbRemoteHold;

      BOS_BOOL               mbLocalTerm;

      CCTKCALLINFO           mCallInfo;
      CCTKPRIVCFG            mPrivCfgInfo;
      CCTKCALLDELRSN         mCallDelReason;
      CCTKMUPDRSN            mCallMediaReason;

      BOS_UINT32             muXferGuid;
      BOS_UINT32             muXferTimerGuid;
      BOS_BOOL               mbXferer;
      BOS_BOOL               mbNoXferTerm;

      BOS_BOOL               mbConnect;
      BOS_BOOL               mbEmergRingbck;

      /* data for processing the eCEEVT_CALL_USR_PROXY event */
      BOS_UINT32             muProxyGuidDataHolder;  
      BOS_UINT32             muProxyGuid;

      MXD_GNS::CBlob         *mpSdpOffer;
      MXD_GNS::CBlob         *mpSdpAnswer;

      BOS_UINT32             muInBandDtmf;
      BOS_UINT32             muIbdtmfDur;

      BOS_UINT8              *mpHfMimeType;
      BOS_UINT8              *mpHfContent;

      BOS_BOOL               mbLocalError;
      BOS_BOOL               mbSigRestricted;
      MXD_GNS::CSocketAddr   mSigRestrictedAddr;
      void                   resourceFreeNetHdl( void );
};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeResCall::CeResCall
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
inline CeResCall::CeResCall( void ) :
   CeResBase             ( CeResBase::eCERESTYPE_CALL ),
   mpUaSspCall           ( NULL ),
   mpMse                 ( NULL ),
   mpAdditionalCallData  ( NULL ),
   mpUaSspLinkedCall     ( NULL ),
   mpSceReferrer         ( NULL ),
   mpCalledParty         ( NULL ),
   mpForwardToParty      ( NULL ),
   mbLocalHold           ( BOS_FALSE ),
   mbLocalHoldPending    ( BOS_FALSE ),
   mbLocalUnHoldPending  ( BOS_FALSE ),
   mbRemoteHold          ( BOS_FALSE ),
   mbLocalTerm           ( BOS_FALSE ),
   mCallDelReason        ( eCCTKCALLDELRSN_NORMAL ),
   muXferGuid            ( guCeInvalidGuid ),
   muXferTimerGuid       ( guCeInvalidGuid ),
   mbXferer              ( BOS_FALSE ),
   mbNoXferTerm          ( BOS_FALSE ),
   mbConnect             ( BOS_FALSE ),
   mbEmergRingbck        ( BOS_FALSE ),
   muProxyGuid           ( guCeInvalidGuid ),
   mpSdpOffer            ( NULL ),
   mpSdpAnswer           ( NULL ),
   muInBandDtmf          ( guCeInvalidGuid ),
   muIbdtmfDur           ( 0 ),
   mpHfMimeType          ( NULL ),
   mpHfContent           ( NULL ),
   mbLocalError          ( BOS_FALSE ),
   mbSigRestricted       ( BOS_FALSE )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < guCeResMaxForkedCall ; uIx++ )
   {
      mpUaSspForkedCall[ uIx ] = NULL;     
   }

   memset( &mPrivCfgInfo, 0, sizeof( mPrivCfgInfo ) );
   
   resourceSetAddress( (void *) this );
}


/***********************************************************************************
**  FUNCTION:   CeResCall::~CeResCall
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
inline CeResCall::~CeResCall( void )
{
   resourceTerminate();
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceTerminate
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
inline BOS_BOOL CeResCall::resourceTerminate( void )
{
   MXD_GNS::MseSdpSession *pMse = NULL;

   if ( mpCalledParty )
   {
      BRCM_DELETE_ARRAY( mpCalledParty );
      mpCalledParty = NULL;
   }

   if ( mpForwardToParty )
   {
      BRCM_DELETE_ARRAY( mpForwardToParty );
      mpForwardToParty = NULL;
   }

   mbLocalHold = BOS_FALSE;
   mbLocalHoldPending = BOS_FALSE;
   mbLocalUnHoldPending = BOS_FALSE;
   mbRemoteHold = BOS_FALSE;
   mpAdditionalCallData = NULL;
   mbXferer = BOS_FALSE;
   mbNoXferTerm = BOS_FALSE;
   muXferGuid = guCeInvalidGuid;
   muXferTimerGuid = guCeInvalidGuid;
   mbLocalTerm = BOS_FALSE;
   mbConnect = BOS_FALSE;
   mbEmergRingbck = BOS_FALSE;
   muProxyGuid = guCeInvalidGuid;
   muInBandDtmf = guCeInvalidGuid;
   muIbdtmfDur = 0;
   mbLocalError = BOS_FALSE;
   mbSigRestricted = BOS_FALSE;

   mCall.Reset();
   mCallSvc.Reset();
   mSigRestrictedAddr.Reset();

   if ( mpSceReferrer )
   {
      mpSceReferrer->ReleaseIfRef();
      mpSceReferrer = NULL;
   }

   if ( resourceNetHdl() )
   {
      resourceFreeNetHdl();
   }

   /* This should not happen because the MSE should be released
   ** as part of the proper FSM processing for that resource.  So, log
   ** an error here but do it anyways to prevent resource leakage.
   */
   resourceGetMse( pMse );
   if ( pMse )
   {
      CCTK_TRACEI2( "resCallTerm (0x%lX) - unexpected need to end MSE (%p).",
                    resourceGuid(), pMse );
      BRCM_DELETE( pMse );
      resourceSetMse( NULL );
   }

   if ( mpSdpOffer )
   {
      BRCM_DELETE( mpSdpOffer );
      mpSdpOffer = NULL;
   }

   if ( mpSdpAnswer )
   {
      BRCM_DELETE( mpSdpAnswer );
      mpSdpAnswer = NULL;
   }

   if ( mpHfMimeType )
   {
      BRCM_DELETE_ARRAY( mpHfMimeType );
      mpHfMimeType = NULL;
   }

   if ( mpHfContent )
   {
      BRCM_DELETE_ARRAY( mpHfContent );
      mpHfContent = NULL;
   }

   resourceLiberate();

   return BOS_TRUE;
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceEqual
**
**  PURPOSE:    Checks whether this is the resource we are looking for based on
**              the UA handle (unique for a given resource).
**
**  PARAMETERS:
**              pCall   - the call handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResCall::resourceEqual( MXD_GNS::IUaSspCall *pCall )
{
   return (( mpUaSspCall == pCall ) ? BOS_TRUE : BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceOwner
**
**  PURPOSE:    Checks whether this resource owns this referrer.
**
**  PARAMETERS:
**              pRefer   - the refer handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResCall::resourceOwner( MXD_GNS::ISceReferrer *pRefer )
{
   return (( mpSceReferrer == pRefer ) ? BOS_TRUE : BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceEqualFork
**
**  PURPOSE:    Checks whether this is the forked resource we are looking for based
**              on the UA handle (unique for a given resource).
**
**  PARAMETERS:
**              pCall   - the call handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResCall::resourceEqualFork( MXD_GNS::IUaSspCall *pCall )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < guCeResMaxForkedCall ; uIx++ )
   {
      if ( mpUaSspForkedCall[ uIx ] == pCall )
      {
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceNetHdl
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
inline MXD_GNS::IUaSspCall *CeResCall::resourceNetHdl( void )
{
   return mpUaSspCall;
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceFreeNetHdl
**
**  PURPOSE:    Frees up the reference to the network resource such that this
**              latter can be cleaned up within UASSP.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResCall::resourceFreeNetHdl( void )
{
   if ( mpUaSspCall )
   {
      mpUaSspCall->ReleaseIfRef();
      mpUaSspCall = NULL;
   }

   if ( mpUaSspLinkedCall )
   {
      /* It is not my responsibility to release this resource, but
      ** I set the pointer to it to NULL to be safe here.
      */
      mpUaSspLinkedCall = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceSetCallInfo
**
**  PURPOSE:    Sets the call information into the call resource.
**
**  PARAMETERS:
**              pCallInfo - the call information to set.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResCall::resourceSetCallInfo( CCTKCALLINFO *pCallInfo )
{
   memcpy( reinterpret_cast<void *> ( &mCallInfo ),
           reinterpret_cast<void *> ( pCallInfo ),
           sizeof ( CCTKCALLINFO ) );
}

/***********************************************************************************
**  FUNCTION:   CeResCall::resourceSetPrivCfgInfo
**
**  PURPOSE:    Sets the call information into the call resource.
**
**  PARAMETERS:
**              pCallInfo - the call information to set.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResCall::resourceSetPrivCfgInfo( CCTKPRIVCFG *pPrivCfg )
{
  memcpy( reinterpret_cast<void *> ( &mPrivCfgInfo ),
           reinterpret_cast<void *> ( pPrivCfg ),
           sizeof ( CCTKPRIVCFG ) );
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceNetReferrer
**
**  PURPOSE:    Returns the handle to the associated network referrer resource.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The handle of interest (warning: may be NULL).
**
**  NOTES:
***********************************************************************************/
inline MXD_GNS::ISceReferrer *CeResCall::resourceNetReferrer( void )
{
   return mpSceReferrer;
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceReferEnd
**
**  PURPOSE:    Terminate the knowledge of the referrer associated with this call.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    .
**
**  NOTES:
***********************************************************************************/
inline void CeResCall::resourceReferEnd( void )
{
   if ( mpSceReferrer )
   {
      mpSceReferrer->ReleaseIfRef();
      mpSceReferrer = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   CeResCall::resourceSetSigRestrictedToAddr
**
**  PURPOSE:    Apply or lift a signalling restriction for this call resource.
**
**  PARAMETERS:
**              bSigRestricted - BOS_TRUE to apply a restriction, BOS_FALSE to
**                               lift one.
**
**              pRestrictAddr - the restricted address info.
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
inline void CeResCall::resourceSetSigRestrictedToAddr( BOS_BOOL bSigRestricted, 
                                                MXD_GNS::CSocketAddr *pRestrictAddr )
{
   /* Regardless of whether we're applying a restriction or removing one
   ** clear all prior restricted address knowledge.
   */
   mbSigRestricted = BOS_FALSE;
   mSigRestrictedAddr.Reset();
   
   /* Only apply the restriction if both the action and address are valid.
   */
   if ( pRestrictAddr && bSigRestricted )
   {
      mbSigRestricted = BOS_TRUE;
      mSigRestrictedAddr = *pRestrictAddr;
   }
}

/***********************************************************************************
**  FUNCTION:   CeResCall::resourceGetSigRestrictedToAddr
**
**  PURPOSE:    Returns a pointer the restricted address info.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    The handle of interest. May be NULL.
**
**  NOTES:
***********************************************************************************/
inline MXD_GNS::CSocketAddr* CeResCall::resourceGetSigRestrictedToAddr( void )
{
   if ( mbSigRestricted )
   {
      return &mSigRestrictedAddr;
   }
   return NULL;
}

#endif /* __CE_RES_CALL__H__INCLUDED__ */
