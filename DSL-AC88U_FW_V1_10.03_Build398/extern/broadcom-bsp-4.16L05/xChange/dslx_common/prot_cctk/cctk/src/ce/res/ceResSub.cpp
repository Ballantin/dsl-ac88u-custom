/**********************************************************************************
** Copyright © 2008 - 2010 Broadcom Corporation
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
**      This file implements the resource subscriber class.
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <ceMain.h>
#include <ceResSub.h>
#include <ceDlg.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceCreate
**
**  PURPOSE:    Creates the necessary network resource support at the UASSP layer.
**
**  PARAMETERS:
**              uCfgGuid - the GUID to locate the configuration associated with
**                         this resource.
**              eType - the type of subscription to create.
**              pNot - pointer to the network notifier resource if already
**                     created.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeResSub::resourceCreate( BOS_UINT32 uCfgGuid,
                                   CCTKSUBEVT eType,
                                   MXD_GNS::ISceNotifier *pNot )
{
   BOS_BOOL bRet = BOS_TRUE;
   const CFGEVT *pCfgEvt;
   const CFGUSR *pCfgUsr;
   MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl;

   if ( pNot == NULL )
   {
      switch ( eType )
      {
         case eCCTKSUBEVT_MAX:
         case eCCTKSUBEVT_DLG:
         case eCCTKSUBEVT_UAP:
         {
            /* Create the resource instance at the UA layer.
            */
            mxt_result res = CreateEComInstance(
                                MXD_GNS::CLSID_CSceSubscriber,
                                NULL,
                                MXD_GNS::IID_ISceSubscriber,
                                reinterpret_cast<void**>( &mpSceSub ));
            if ( MX_RIS_S( res ))
            {
               /* Set the manager to receive event information for this service.
               */
               meSubType = eType;
               mpSceSub->SetManager( gpCeMain );

               res = mpSceSub->QueryIf( &pSceBasicExtCtl );
               if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
               {
                  pSceBasicExtCtl->SetBasicExtensionManager( gpCeMain );
                  pSceBasicExtCtl->SetSipPacketObserver(
                     reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>(
                                                               pmGetPktObs() ) );
                  pSceBasicExtCtl->ReleaseIfRef();
                  pSceBasicExtCtl = NULL;
               }
               else
               {
                  CCTK_TRACEI1(
                     "CeResSub (%p) - unable to set basic extension manager.",
                     this );
               }

               pCfgEvt = CFG_EVT( uCfgGuid );
               if ( pCfgEvt != NULL )
               {
                  pCfgUsr = CFG_USR ( pCfgEvt->in.uUsrId );
                  if ( pCfgUsr != NULL )
                  {
                     mpSceSub->SetConfiguration( pCfgUsr->in.pSceUsrCfg );
                     /* Set the retry delay on failed subscription attempt for this
                     ** subscription.
                     */
                     mpSceSub->SetSubscriptionRetryDelay(
                                    pCfgUsr->xtl.dft.evt.gen.tmr.uRetryTmr );
                     /* Set whether to map a 500 subscription response to a 408 one
                     ** for re-attempt behavior.
                     */
                     pCfgUsr->in.pSceUsrCfg->SetSubFail500(
                            pCfgUsr->xtl.dft.evt.gen.tmr.bMap500To408 == BOS_TRUE );
                  }
                  else
                  {
                     bRet = BOS_FALSE;
                  }
               }
               else
               {
                  bRet = BOS_FALSE;
               }
            }
            else
            {
               CCTK_TRACEI1( "CeResSub (%p) - unable to create SCE subscriber.",
                             this );
               bRet = BOS_FALSE;
            }
         }
         break;

         case eCCTKSUBEVT_MWI:
         {
            memset( static_cast<void *> (&mMsgSum),
                    0,
                    sizeof( CCTKMWIINFO ));
            
            /* Create the resource instance at the UA layer.
            */
            mxt_result res = CreateEComInstance(
                                MXD_GNS::CLSID_CUaSspMwi,
                                NULL,
                                MXD_GNS::IID_IUaSspMwi,
                                reinterpret_cast<void**>( &mpUaSspMwi ));
            if ( MX_RIS_S( res ))
            {
               /* Set the manager to receive event information for this service.
               */
               meSubType = eCCTKSUBEVT_MWI;
               mpUaSspMwi->SetManager( gpCeMain );

               res = mpUaSspMwi->QueryIf( &pSceBasicExtCtl );
               if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
               {
                  pSceBasicExtCtl->SetBasicExtensionManager( gpCeMain );
                  pSceBasicExtCtl->SetSipPacketObserver(
                     reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>(
                                                               pmGetPktObs() ) );

                  pSceBasicExtCtl->ReleaseIfRef();
                  pSceBasicExtCtl = NULL;
               }
               else
               {
                  CCTK_TRACEI1(
                     "CeResSub (%p) - unable to set basic extension manager.",
                     this );
               }

               pCfgUsr = CFG_USR( uCfgGuid );
               if ( pCfgUsr != NULL )
               {
                  mpUaSspMwi->SetConfiguration( pCfgUsr->in.pSceUsrCfg );
               }
               else
               {
                  bRet = BOS_FALSE;
               }
            }
            else
            {
               CCTK_TRACEI1( "CeResSub (%p) - unable to create UASSP MWI.",
                             this );
               bRet = BOS_FALSE;
            }
         }
         break;

         case eCCTKSUBEVT_BMWI:
         {
            memset( static_cast<void *> (&mMsgSum),
                    0,
                    sizeof( CCTKMWIINFO ));

            /* Create the resource instance at the UA layer.
            */
            mxt_result res = CreateEComInstance(
                                MXD_GNS::CLSID_CUaSspBlindMwi,
                                NULL,
                                MXD_GNS::IID_IUaSspMwi,
                                reinterpret_cast<void**>( &mpUaSspBlindMwi ));

            if ( MX_RIS_S( res ))
            {
               /* Set the manager to receive event information for this service.
               */
               meSubType = eCCTKSUBEVT_BMWI;
               mpUaSspBlindMwi->SetManager( gpCeMain );

               res = mpUaSspBlindMwi->QueryIf( &pSceBasicExtCtl );
               if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
               {
                  pSceBasicExtCtl->SetBasicExtensionManager( gpCeMain );
                  pSceBasicExtCtl->SetSipPacketObserver(
                     reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>(
                                                               pmGetPktObs() ) );

                  pSceBasicExtCtl->ReleaseIfRef();
                  pSceBasicExtCtl = NULL;
               }
               else
               {
                  CCTK_TRACEI1(
                     "CeResSub (%p) - unable to set basic extension manager.",
                     this );
               }

               pCfgUsr = CFG_USR( uCfgGuid );
               if ( pCfgUsr != NULL )
               {
                  mpUaSspBlindMwi->SetConfiguration( pCfgUsr->in.pSceUsrCfg );
               }
               else
               {
                  CCTK_TRACEI1(
                     "CeResSub (%p) - unable to set config for blind mwi.",
                     this );
               }
            }
         }
         break;

         case eCCTKSUBEVT_REG:
         default:
         {
            CCTK_TRACEI2( "CeResSub (%p) - Unknown subsription type: %d.",
                          this, eType );
            bRet = BOS_FALSE;
         }
         break;
      }
   }
   else
   {
      if ( eType == eCCTKSUBEVT_MAX )
      {
         mpSceNot = pNot;

         /* Increment the reference count for this resource such that it is not
         ** freed up on us.
         */
         mpSceNot->AddIfRef();

         /* Set the manager to receive event information for this service.
         */
         mpSceNot->SetManager( gpCeMain );

         mxt_result res = mpSceNot->QueryIf( &pSceBasicExtCtl );
         if ( MX_RIS_S( res ) && ( pSceBasicExtCtl != NULL ) )
         {
            pSceBasicExtCtl->SetBasicExtensionManager( gpCeMain );
            pSceBasicExtCtl->SetSipPacketObserver(
               reinterpret_cast<MXD_GNS::ISceSipPacketObserver *>(
                                                         pmGetPktObs() ) );

            pSceBasicExtCtl->ReleaseIfRef();
            pSceBasicExtCtl = NULL;
         }
         else
         {
            CCTK_TRACEI1(
               "CeResSub (%p) - unable to set basic extension manager.",
               this );
         }
      }
      else
      {
         bRet = BOS_FALSE;
      }
   }


   /* Create additional resources associated with the type of subscription
   ** that are necessary for this to work fine.
   */
   switch ( eType )
   {
      case eCCTKSUBEVT_DLG:
      {
         mpDlg = BRCM_NEW( CeDlg );

         if ( mpDlg == NULL )
         {
            bRet = BOS_FALSE;
         }
      }
      break;

      default:
      break;
   }

   CCTK_TRACED2( "CeResSub (%p) - Resource creation: %s.",
                 this,
                 ( bRet ? "Success" : "Fail" ));
   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourcePushData
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
void CeResSub::resourcePushData( CEEVT eEvt, void *pData )
{
   if ( pData == NULL )
   {
      return;
   }

   switch ( eEvt )
   {
      case eCEEVT_SUB_USR_CREATE:
      {
         /* We need to keep the address of the subed party as well as the
         ** subscription timeout.
         */
         const BOS_UINT8 *pSubedParty =
                        (static_cast<const TKMSUBINFO *> ( pData ))->pcSubAddr;

         mpSubedParty = utilStrDup( pSubedParty );
         muTimeOut = (static_cast<const TKMSUBINFO *> ( pData ))->uSubTimeout;
         mbOriginator = BOS_TRUE;
      }
      break;

      case eCEEVT_SUB_USR_NOTIFY:
      {
         switch ( meSubType )
         {
            case eCCTKSUBEVT_DLG:
            {
               /* For dialog information, we only pass the interested data which is
               ** whether we have a new dialog information or not.
               */
               if ( mpDlg )
               {
                  mpDlg->ceDlgSet( *(static_cast<BOS_BOOL *> ( pData )) );
               }
            }
            break;

            default:
            break;
         }
      }
      break;

      case eCEEVT_SUB_NET_REQ:
      {
         mbOriginator = BOS_FALSE;

         switch ( (static_cast<CeMain::CEMAINSUB *> ( pData ))->uType )
         {
            case MXD_GNS::eEVENTTYPE_DIALOG:
            {
               meSubType = eCCTKSUBEVT_DLG;
               muTimeOut = (static_cast<CeMain::CEMAINSUB *> ( pData ))->uTimeOut;
               mpAdditionalSubInfo =
                  (static_cast<CeMain::CEMAINSUB *> ( pData ))->pAdditionalSubInfo;
            }
            break;

            /* No other events can be subscribed to at this time.
            */
            default:
            {
               meSubType = eCCTKSUBEVT_MAX;
            }
            break;
         }
      }
      break;

      case eCEEVT_SUB_NET_NOTIFY:
      {
         switch ( meSubType )
         {
            case eCCTKSUBEVT_MWI:
            case eCCTKSUBEVT_BMWI:
            {
               if ( mpMsgSum == NULL )
               {
                  mpMsgSum = BRCM_NEW( CCTKMWIINFO );
               }

               *mpMsgSum = *(static_cast<CCTKMWIINFO *> ( pData ));
            }
            break;

            case eCCTKSUBEVT_DLG:
            case eCCTKSUBEVT_UAP:
            {
               mpNotBody = (static_cast<CeMain::CEMAINDATA *> ( pData ))->puData;
               mNotBody = (static_cast<CeMain::CEMAINDATA *> ( pData ))->uSize;
               mpAdditionalNotInfo =
                  (static_cast<CeMain::CEMAINDATA *> ( pData ))->pAdditionalNotInfo;
            }
            break;

            case eCCTKSUBEVT_REG:
            case eCCTKSUBEVT_MAX:
            default:
            break;
         }
      }
      break;

      default:
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceBasicCtlEqual
**
**  PURPOSE:    Checks whether a resource is actually associated with a given
**              basic extension controller.
**
**  PARAMETERS:
**              pBasicExtCtl - the basic extension controller to lookup.
**
**
**  RETURNS:    BOS_TRUE if this resource owns this controller, BOS_FALSE
**              otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeResSub::resourceBasicCtlEqual(
                        IN MXD_GNS::ISceBasicExtensionControl *pBasicExtCtl )
{
   MXD_GNS::ISceBasicExtensionControl *pMyBasicExtCtl = NULL;
   BOS_BOOL bRet = BOS_FALSE;
   mxt_result res = resFE_FAIL;

   switch ( meSubType )
   {
      case eCCTKSUBEVT_DLG:
      case eCCTKSUBEVT_UAP:
      case eCCTKSUBEVT_MAX:
      {
         if ( mpSceSub )
         {
            res = mpSceSub->QueryIf( &pMyBasicExtCtl );
         }
      }
      break;

      case eCCTKSUBEVT_MWI:
      {
         if ( mpUaSspMwi )
         {
            res = mpUaSspMwi->QueryIf( &pMyBasicExtCtl );
         }
      }
      break;

      case eCCTKSUBEVT_BMWI:
      {
         if ( mpUaSspBlindMwi )
         {
            res = mpUaSspBlindMwi->QueryIf( &pMyBasicExtCtl );
         }
      }
      break;

      default:
      break;
   }

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

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceBasicCtl
**
**  PURPOSE:    Retrieves the basic controller interface for a given CeResSub
**              component.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    The corresponding basic extension control interface is one is
**              available, NULL otherwise.
**
**  NOTES:      The caller is responsible for releasing the interface allocated
**              if such is valid.
***********************************************************************************/
BOS_BOOL CeResSub::resourceBasicCtl( MXD_GNS::ISceBasicExtensionControl **pExtCtl )
{
   MXD_GNS::ISceBasicExtensionControl *pMyBasicExtCtl = NULL;
   mxt_result res = resFE_FAIL;

   switch ( meSubType )
   {
      case eCCTKSUBEVT_DLG:
      case eCCTKSUBEVT_UAP:
      case eCCTKSUBEVT_MAX:
      {
         if ( mpSceSub )
         {
            res = mpSceSub->QueryIf( &pMyBasicExtCtl );
         }
      }
      break;

      case eCCTKSUBEVT_MWI:
      {
         if ( mpUaSspMwi )
         {
            res = mpUaSspMwi->QueryIf( &pMyBasicExtCtl );
         }
      }
      break;

      case eCCTKSUBEVT_BMWI:
      {
         if ( mpUaSspBlindMwi )
         {
            res = mpUaSspBlindMwi->QueryIf( &pMyBasicExtCtl );
         }
      }
      break;

      default:
      break;
   }

   if ( MX_RIS_S( res ) &&
        ( pMyBasicExtCtl != NULL ) )
   {
      /* Note that the caller is responsible for releasing the interface.
      */
      *pExtCtl = pMyBasicExtCtl;
      return BOS_TRUE; 
   }

   return BOS_FALSE;
}


/***********************************************************************************
**  FUNCTION:   CeResSub::resourceShow
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
void CeResSub::resourceShow( void )
{
#if CCTKCFG_APPLICATION_STATUS
   /* Invoke base class first to display the generic information about
   ** this resource.
   */
   CeResBase::resourceShow();

   /* Display the class specific data associated with this resource.
   */
   CCTK_TRACEI4( "SceSub [%p], UaSspMwi [%p], UaSspBMwi [%p], SceNot [%p]",
                 mpSceSub,
                 mpUaSspMwi,
                 mpUaSspBlindMwi,
                 mpSceNot );

   CCTK_TRACEI3( "Type [%d], Orig [%c], RelImm [%c]",
                 meSubType,
                 CERESBASE_BOOL_VALUE( mbOriginator ),
                 CERESBASE_BOOL_VALUE( mbReleaseImmediate ) );

   CCTK_TRACEI2( "Party [%s], TimeOut [%d]",
                 mpSubedParty,
                 muTimeOut );

   CCTK_TRACEI1( "Resource [0x%lX] -- END.",
                 resourceGuid() );
#endif /* CCTKCFG_APPLICATION_STATUS */
}
