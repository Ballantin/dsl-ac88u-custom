/**********************************************************************************
** Copyright (c) 2010-2013 Broadcom
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
**      This file defines the subscriber class of the resource object that is
**      managed by the Call Engine.
**
***********************************************************************************/
#ifndef __CE_RES_SUB__H__INCLUDED__
#define __CE_RES_SUB__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceResBase.h>
#include <ceDlg.h>


/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

/*
** This class defines the user resource.
*/
class CeResSub: virtual public CeResBase
{
   public:

      CeResSub( void );
      ~CeResSub( void );

      BOS_BOOL  resourceCreate( BOS_UINT32 uCfgGuid,
                                CCTKSUBEVT eType,
                                MXD_GNS::ISceNotifier *pNot = NULL );
      BOS_BOOL  resourceTerminate( void );

      BOS_BOOL  resourceSubEqual( MXD_GNS::ISceSubscriber *pSub );
      BOS_BOOL  resourceMwiEqual( MXD_GNS::IUaSspMwi *pMwi );
      BOS_BOOL  resourceNotEqual( MXD_GNS::ISceNotifier *pNot );

      void                             resourcePushData( CEEVT eEvt,
                                                         void *pData );

      CCTKSUBEVT                       resourceType( void );
      void                             resourceEvtType( CCTKSUBEVT eEvt );

      BOS_BOOL                         resourceInform( void );

      MXD_GNS::ISceSubscriber          *resourceSubHdl( void );
      MXD_GNS::ISceNotifier            *resourceNotHdl( void );
      MXD_GNS::IUaSspMwi               *resourceMwiHdl( void );
      MXD_GNS::IUaSspMwi               *resourceBlindMwiHdl( void );

      inline CCTKMWIINFO               *resourceGetNewMwiMsgSummary( void )
                                                            { return mpMsgSum; }
      inline CCTKMWIINFO               *resourceGetCurMwiMsgSummary( void )
                                                            { return &mMsgSum; }

      inline BOS_BOOL                  resourceOriginator( void )
                                                        { return mbOriginator; };

      inline BOS_UINT32                resourceSubTimeOut( void )
                                                         { return muTimeOut; };
      inline BOS_UINT8                 *resourceSubedParty( void )
                                                         { return mpSubedParty; };
      inline BOS_UINT8                 *resourceNotBodyData( BOS_BOOL bTake );
      inline BOS_UINT32                resourceNotBodySize( void )
                                                         { return mNotBody; };
      inline CeDlg                     *resourceNotDlg( void ) { return mpDlg; };
      inline BOS_BOOL                  resourceDlgFullGet( void ) 
                                                         { return mbFull; };
      inline void                      resourceDlgFullSet( BOS_BOOL bFull )
                                                         { mbFull = bFull; };
      inline void *                    resourceAddtionalSubInfo( void )
                                                 { return mpAdditionalSubInfo; };
      inline void *                    resourceAddtionalNotInfo( void )
                                                 { return mpAdditionalNotInfo; };
      inline BOS_UINT32                resourceProfGet( void ) { return muProfIx; };
      inline void                      resourceProfSet( BOS_UINT32 uProfIx )
                                                         { muProfIx = uProfIx; };

      BOS_BOOL  resourceBasicCtlEqual(
                                 IN MXD_GNS::ISceBasicExtensionControl
                                                               *pBasicExtCtl );
      BOS_BOOL resourceBasicCtl( MXD_GNS::ISceBasicExtensionControl **pExtCtl );
      inline void resourceSetFeatInfo( CCTKFEATINFO &featInfo );
      inline CCTKFEATINFO *resourceGetFeatInfo( void ) { return &mFeatInfo; };

      inline BOS_BOOL                  resourceRelease( void )
                                                { return mbReleaseImmediate; }; 
      inline void                      resourceReleaseImmediate( void )
                                                { mbReleaseImmediate = BOS_TRUE; };

      inline BOS_BOOL                  resourceSubPending( void )
                                                { return mbSubPending; };
      inline void                      resourceSubPending( BOS_BOOL bSubPending )
                                                { mbSubPending = bSubPending; };

      /* Base class virtual method overrides.
      */
      virtual void                     resourceShow( void );

   private:

      CCTKSUBEVT                       meSubType;
      BOS_BOOL                         mbOriginator;

      MXD_GNS::ISceSubscriber          *mpSceSub;
      MXD_GNS::IUaSspMwi               *mpUaSspMwi;
      MXD_GNS::IUaSspMwi               *mpUaSspBlindMwi;
      MXD_GNS::ISceNotifier            *mpSceNot;

      CCTKMWIINFO                      mMsgSum;
      CCTKMWIINFO                      *mpMsgSum;

      BOS_UINT8                        *mpSubedParty;
      BOS_UINT32                       muTimeOut;
      void                             *mpAdditionalSubInfo;

      BOS_UINT8                        *mpNotBody;
      BOS_UINT32                       mNotBody;
      void                             *mpAdditionalNotInfo;

      CeDlg                            *mpDlg;
      BOS_BOOL                         mbFull;

      BOS_UINT32                       muProfIx;
      CCTKFEATINFO                     mFeatInfo;

      BOS_BOOL                         mbReleaseImmediate;
      BOS_BOOL                         mbSubPending;
};


/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeResSub::CeResSub
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
inline CeResSub::CeResSub( void ) :
   CeResBase             ( CeResBase::eCERESTYPE_SUB ),
   meSubType             ( eCCTKSUBEVT_MAX ),
   mbOriginator          ( BOS_FALSE ),
   mpSceSub              ( NULL ),
   mpUaSspMwi            ( NULL ),
   mpUaSspBlindMwi       ( NULL ),
   mpSceNot              ( NULL ),
   mpMsgSum              ( NULL ),
   mpSubedParty          ( NULL ),
   muTimeOut             ( 0 ),
   mpAdditionalSubInfo   ( NULL ),
   mpNotBody             ( NULL ),
   mNotBody              ( 0 ),
   mpAdditionalNotInfo   ( NULL ),
   mpDlg                 ( NULL ),
   mbFull                ( BOS_FALSE ),
   muProfIx              ( guCeInvalidGuid ),
   mbReleaseImmediate    ( BOS_FALSE )
{
   memset( &mFeatInfo, 0 , sizeof( CCTKFEATINFO ) );

   resourceSetAddress( (void *) this );
}


/***********************************************************************************
**  FUNCTION:   CeResSub::~CeResSub
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
inline CeResSub::~CeResSub( void )
{
   resourceTerminate();
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceTerminate
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
inline BOS_BOOL CeResSub::resourceTerminate( void )
{
   if ( mpMsgSum )
   {
      BRCM_DELETE( mpMsgSum );
      mpMsgSum = NULL;
   }

   if ( mpSubedParty )
   {
      BRCM_DELETE_ARRAY( mpSubedParty );
      mpSubedParty = NULL;
   }

   if ( mpNotBody )
   {
      BRCM_DELETE_ARRAY( mpNotBody );
      mpNotBody = NULL;
   }

   if ( resourceSubHdl() )
   {
      mpSceSub->ReleaseIfRef();
      mpSceSub = NULL;
   }

   if ( resourceNotHdl() )
   {
      mpSceNot->ReleaseIfRef();
      mpSceNot = NULL;
   }

   if ( resourceMwiHdl() )
   {
      mpUaSspMwi->ReleaseIfRef();
      mpUaSspMwi = NULL;
   }

   if ( resourceBlindMwiHdl() )
   {
      mpUaSspBlindMwi->ReleaseIfRef();
      mpUaSspBlindMwi = NULL;
   }

   if ( mpDlg )
   {
      BRCM_DELETE( mpDlg );
      mpDlg = NULL;
   }

   /* If we allocated a configuration resource for this event support,
   ** remove it now.
   */
   if ( cfgGetType( resourceGuid() ) == eCFGTYPE_EVT )
   {
      cfgRemoveEvt( resourceGuid() );
   }

   mpAdditionalNotInfo = NULL;
   mpAdditionalSubInfo = NULL;
   mbReleaseImmediate = BOS_FALSE;
   meSubType = eCCTKSUBEVT_MAX;

   resourceLiberate();

   return BOS_TRUE;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceType
**
**  PURPOSE:    Gets the resource type associated with this subscription.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    The subscription type.
**
**  NOTES:
***********************************************************************************/
inline CCTKSUBEVT CeResSub::resourceType( void )
{
   return meSubType;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceEvtType
**
**  PURPOSE:    Sets the resource event type associated with this subscription.
**
**  PARAMETERS:
**              eEvt - the subscription event type.
**
**
**  RETURNS:    None.
**
**  NOTES:      This method must be used with extreme caution as it will set the
**              subscription type that this resource shall be interpreted in.
**              The resource subscription type is normally automatically assigned
**              by the Call Engine framework based on the context of the resource
**              creation, however there is one special case which requires the
**              Call Engine to create a subscription that is not associated with
**              any context, that case is when the subscription is temporarily
**              assigned to handle out-of-dialog notification requests.  In such
**              case, the Call Engine resource subscription type can only be
**              assigned using this method.
***********************************************************************************/
inline void CeResSub::resourceEvtType( CCTKSUBEVT eEvt )
{
   meSubType = eEvt;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceSubEqual
**
**  PURPOSE:    Checks whether this is the resource we are looking for based on
**              the UA handle (unique for a given resource).
**
**  PARAMETERS:
**              pSub   - the subscriber resource handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResSub::resourceSubEqual( MXD_GNS::ISceSubscriber *pSub )
{
   return (( mpSceSub == pSub ) ? BOS_TRUE : BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceMwiEqual
**
**  PURPOSE:    Checks whether this is the resource we are looking for based on
**              the UA handle (unique for a given resource).
**
**  PARAMETERS:
**              pMwi   - the message waiting resource handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:      Check for either subscribed MWI or blind MWI, either is treated
**              the same.
***********************************************************************************/
inline BOS_BOOL CeResSub::resourceMwiEqual( MXD_GNS::IUaSspMwi *pMwi )
{
   return (( ( mpUaSspMwi == pMwi ) || ( mpUaSspBlindMwi == pMwi ) ) ?
                                                            BOS_TRUE : BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceNotEqual
**
**  PURPOSE:    Checks whether this is the resource we are looking for based on
**              the UA handle (unique for a given resource).
**
**  PARAMETERS:
**              pNot   - the notifier resource handle to match.
**
**
**  RETURNS:    BOS_TRUE if this is the resource, BOS_FALSE otherwise.
**
**  NOTES:
***********************************************************************************/
inline BOS_BOOL CeResSub::resourceNotEqual( MXD_GNS::ISceNotifier *pNot )
{
   return (( mpSceNot == pNot ) ? BOS_TRUE : BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceSubHdl
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
inline MXD_GNS::ISceSubscriber *CeResSub::resourceSubHdl( void )
{
   return mpSceSub;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceNotHdl
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
inline MXD_GNS::ISceNotifier *CeResSub::resourceNotHdl( void )
{
   return mpSceNot;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceMwiHdl
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
inline MXD_GNS::IUaSspMwi *CeResSub::resourceMwiHdl( void )
{
   return mpUaSspMwi;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceBlindMwiHdl
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
inline MXD_GNS::IUaSspMwi *CeResSub::resourceBlindMwiHdl( void )
{
   return mpUaSspBlindMwi;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceInform
**
**  PURPOSE:    Determines whether an event for a given resource type should be
**              reported or not.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    BOS_TRUE if such report should happen.
**              BOS_FALSE otherwise.
**
**  NOTES:      Some events are entirely dealt within Call Engine (and CCTK) and
**              actions on those do not need to be reported up to the application
**              since this latter will have no notion of it.
**
**              Only those types which are controlled by the application being
**              reported.
***********************************************************************************/
inline BOS_BOOL CeResSub::resourceInform( void )
{
   BOS_BOOL bRet = BOS_TRUE;

   switch ( meSubType )
   {
      case eCCTKSUBEVT_MWI:
      case eCCTKSUBEVT_BMWI:
      case eCCTKSUBEVT_REG:
      case eCCTKSUBEVT_UAP:
      case eCCTKSUBEVT_MAX:
      {
         bRet = BOS_FALSE;
      }
      break;

      case eCCTKSUBEVT_DLG:
      default:
      break;
   }

   return bRet;
}

/***********************************************************************************
**  FUNCTION:   CeResSub::resourceSetFeatInfo
**
**  PURPOSE:    Sets the feature information for this user.
**
**  PARAMETERS:
**              featInfo - the feature information to set.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
inline void CeResSub::resourceSetFeatInfo( CCTKFEATINFO &featInfo )
{
   memcpy( &mFeatInfo,
           &featInfo,
           sizeof( CCTKFEATINFO ) );
}

/***********************************************************************************
**  FUNCTION:   CeResSub::resourceNotBodyData
**
**  PURPOSE:    Retrieve the stored NOTIFY message body data.
**
**  PARAMETERS:
**              bTake - True if the caller of the function wants to take the
**                      ownership of the body data (i.e. the caller is responsible
**                      for deleting the body data after use).
**
**
**  RETURNS:    NOTIFY message body data pointer.
**              NULL if no NOTIFY message body has been stored.
**
**  NOTES:
***********************************************************************************/
inline BOS_UINT8 *CeResSub::resourceNotBodyData( BOS_BOOL bTake )
{
   BOS_UINT8 *puRes = mpNotBody;

   if ( bTake )
   {
      mpNotBody = NULL;
   }
   
   return puRes;
}

#endif /* __CE_RES_SUB__H__INCLUDED__ */
