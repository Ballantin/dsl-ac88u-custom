/**********************************************************************************
** Copyright (c) 2007-2012 Broadcom Corporation
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
**      This file implements the resource call class.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <ceMain.h>
#include <ceResCall.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceCreate
**
**  PURPOSE:    Creates the necessary network resource support at the UASSP layer.
**
**  PARAMETERS:
**              uCfgGuid - the GUID to locate the configuration associated with
**                         this resource.
**              pCall - pointer to associated network resource (may be NULL).
**
**
**  RETURNS:    Nothing
**
**  NOTES:      If the pCall is not NULL, then this is due to the creation of an
**              incoming call resource by the stack and one simply needs to set
**              the correct manager.
**
**              If pCall is NULL, then this is due to the creation of an outgoing
**              call resource by the Call Engine and one should also create a
**              network call resource first.
***********************************************************************************/
BOS_BOOL CeResCall::resourceCreate( BOS_UINT32 uCfgGuid,
                                    MXD_GNS::IUaSspCall *pCall )
{
   BOS_BOOL bRet = BOS_TRUE;
   MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl = NULL;
   mxt_result res;
   const CFGCALL *pCfgCall;
   const CFGUSR *pCfgUsr;
   BOS_UINT32 uIx;

   /* Safeguard in case some previously associated forked call with
   ** this resource have not been cleaned up properly.
   */
   for ( uIx = 0 ; uIx < guCeResMaxForkedCall ; uIx++ )
   {
      if ( mpUaSspForkedCall[ uIx ] != NULL )
      {
         mpUaSspForkedCall[ uIx ]->ReleaseIfRef();
         mpUaSspForkedCall[ uIx ] = NULL;
      }
   }

   if ( pCall == NULL )
   {
      /* Create the resource instance at the UA layer.
      */
      mxt_result res = CreateEComInstance(
                          MXD_GNS::CLSID_CUaSspCall,
                          NULL,
                          MXD_GNS::IID_IUaSspCall,
                          reinterpret_cast<void**>( &mpUaSspCall ));
      if ( MX_RIS_S( res ))
      {
         /* Set the manager to receive event information for this service.
         */
         mpUaSspCall->SetManager( gpCeMain );

         /* Configure the call.  Note that at the moment there is no per-call
         ** configuration, so the configuration for the call is equivalent to
         ** the user configuration for the user owner of this call.
         */
         res = mpUaSspCall->QueryIf( &pSceBasicExtCtl );
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
               "CeResCall (%p) - unable to set basic extension manager.",
               this );
         }

         pCfgCall = CFG_CALL( uCfgGuid );
         if ( pCfgCall != NULL )
         {
            pCfgUsr = CFG_USR ( pCfgCall->in.uUsrId );
            if ( pCfgUsr != NULL )
            {
               mpUaSspCall->SetConfiguration( pCfgUsr->in.pSceUsrCfg );
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
         CCTK_TRACEI1( "CeResCall (%p) - unable to create UASSP call.",
                       this );
         bRet = BOS_FALSE;
      }
   }
   else
   {
      mpUaSspCall = pCall;

      /* Increment the reference count for this resource such that it is not
      ** freed up on us.
      */
      mpUaSspCall->AddIfRef();

      /* Set the manager to receive event information for this service.
      */
      mpUaSspCall->SetManager( gpCeMain );

      res = mpUaSspCall->QueryIf( &pSceBasicExtCtl );
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
            "CeResCall (%p) - unable to set basic extension manager.",
            this );
      }
   }

   CCTK_TRACED2( "CeResCall (%p) - Resource creation: %s.",
                 this,
                 ( bRet ? "Success" : "Fail" ));
   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceEnd
**
**  PURPOSE:    Terminates the life of an allocated resource by cleaning all the
**              necessary aspects of it.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    Nothing
**
**  NOTES:      Once invoked for this resource, the resource is basically freed
**              up to the resource pool, no recovery is possible.
**
**              Use with extreme caution.
***********************************************************************************/
void CeResCall::resourceEnd( void )
{
   resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_IDLE );
   resourceTerminate();
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourcePushData
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
void CeResCall::resourcePushData( CEEVT eEvt, void *pData )
{
   if ( pData == NULL )
   {
      return;
   }

   switch ( eEvt )
   {
      case eCEEVT_CALL_NET_CALLED:
      case eCEEVT_CALL_NET_ANSWER:
      case eCEEVT_CALL_NET_INFO:
      {
         if ( pData )
         {
            mpAdditionalCallData = pData;
         }
      }
      break;

      case eCEEVT_CALL_USR_CREATE:
      {
         /* We need to keep the address of the called party.
         */
         const BOS_UINT8 *pCalledParty = static_cast<const BOS_UINT8 *> ( pData );

         mpCalledParty = utilStrDup( pCalledParty );
      }
      break;

      case eCEEVT_CALL_USR_FORWARD:
      {
         /* We need to keep the address of the 'forward to' party.
         */
         const BOS_UINT8 *pForwardToParty =
                                static_cast<const BOS_UINT8 *> ( pData );

         mpForwardToParty = utilStrDup( pForwardToParty );
      }
      break;

      case eCEEVT_CALL_USR_XFER_NOTERM:
      {
         mbNoXferTerm = BOS_TRUE;
      }
	  /* Fall-through from special transfer case without call termination */
      case eCEEVT_HOLD_USR_XFER:
      case eCEEVT_CALL_USR_XFER:
      {
         /* We need to keep the GUID of the call being transfered to.  We
         ** also need to mark that we are originating the transfer.
         */
         muXferGuid = *(static_cast<BOS_UINT32 *> ( pData ));
         mbXferer = BOS_TRUE;
      }
      break;

      case eCEEVT_HOLD_USR_BXFER:
      case eCEEVT_CALL_USR_BXFER:
      {
         /* We need to keep the party being transfered to (use the forward to
         ** party for this).  We also need to mark that we are originating the
         ** transfer.
         */
         const BOS_UINT8 *pXferParty = static_cast<const BOS_UINT8 *> ( pData );

         mpForwardToParty = utilStrDup( pXferParty );
         muXferGuid = guCeInvalidGuid;
         mbXferer = BOS_TRUE;
      }
      break;

      case eCEEVT_CALL_USR_ENDED:
      {
         /* We need to keep the reason for the call termination by user.
         */
         mCallDelReason = *(static_cast<CCTKCALLDELRSN *> ( pData ));
      }
      break;

      case eCEEVT_CALL_USR_PROXY:
      {
         /* We need to keep the reference user that this call is proxy'ed
         ** through.  For the local user call session, the Proxy Guid is set
         ** when the remote user answers.  For the remote user call session,
         ** the Proxy Guid when this event is processed.
         */
         muProxyGuidDataHolder = *(static_cast<BOS_UINT32 *> ( pData ));
      }
      break;

      case eCEEVT_CALL_USR_DTMF:
      {
         muInBandDtmf = (static_cast<const TKMDTMFINFO *> ( pData ))->uDtmf;
         muIbdtmfDur = (static_cast<const TKMDTMFINFO *> ( pData ))->uDuration;
      }
      break;

      case eCEEVT_CALL_USR_FLASH:
      {
         const BOS_UINT8 *pHfMimeType = (static_cast<const TKMFLASHINFO *> ( pData ))->pcHfMimeType;
         const BOS_UINT8 *pHfContent = (static_cast<const TKMFLASHINFO *> ( pData ))->pcHfContent;

         mpHfMimeType = utilStrDup( pHfMimeType );
         mpHfContent = utilStrDup( pHfContent );
      }
      break;

      case eCEEVT_CALL_NET_PEND_ANSWER_SDP:
      {
#if CCTKCFG_REMOTE_USER_CONTROL
         MXD_GNS::CBlob *pSrc = reinterpret_cast<MXD_GNS::CBlob *>( pData );
         if ( mpSdpOffer )
         {
            BRCM_DELETE( mpSdpOffer );
         }
         mpSdpOffer = BRCM_NEW( MXD_GNS::CBlob ) (*pSrc);
#endif
      }
      break;

      case eCEEVT_CALL_NET_ANSWER_SDP:
      {
#if CCTKCFG_REMOTE_USER_CONTROL
         MXD_GNS::CBlob *pSrc = reinterpret_cast<MXD_GNS::CBlob *>( pData );
         if ( mpSdpAnswer )
         {
            BRCM_DELETE( mpSdpAnswer );
         }
         mpSdpAnswer = BRCM_NEW( MXD_GNS::CBlob ) (*pSrc);
#endif
      }
      break;
      case eCEEVT_CALL_USR_MEDIA:
      {
         mCallMediaReason = *(static_cast<CCTKMUPDRSN *> ( pData ));
      }
      break;
      default:
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceSetHold
**
**  PURPOSE:    Sets the resource hold information to keep track of what status
**              the resource is in.
**
**  PARAMETERS:
**              eHold    - the event associated with the data.
**              bCurrent - the value to set for the current status.
**              bPending - the value to set for the 'pending' status.
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeResCall::resourceSetHold( CEEVT eHold, BOS_BOOL bCurrent, BOS_BOOL bPending )
{
   switch ( eHold )
   {
      case eCEEVT_HOLD_USR_HOLD:
      {
         mbLocalHold = bCurrent;
         mbLocalHoldPending = bPending;
      }
      break;

      case eCEEVT_HOLD_USR_UNHOLD:
      {
         mbLocalHold = bCurrent;
         mbLocalUnHoldPending = bPending;
      }
      break;

      case eCEEVT_HOLD_NET_HOLD:
      case eCEEVT_HOLD_NET_UNHOLD:
      {
         mbRemoteHold = bCurrent;
      }
      break;

      default:
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceGetHold
**
**  PURPOSE:    Gets the resource hold information which keeps track of what status
**              the resource is in.
**
**  PARAMETERS:
**              eHold    - the event associated with the data.
**              bCurrent - the value to set for the current status.
**              bPending - the value to set for the 'pending' status.
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeResCall::resourceGetHold( CEEVT eHold, BOS_BOOL &rbCurrent,
                                 BOS_BOOL &rbPending )
{
   switch ( eHold )
   {
      case eCEEVT_HOLD_USR_HOLD:
      {
         rbCurrent = mbLocalHold;
         rbPending = mbLocalHoldPending;
      }
      break;

      case eCEEVT_HOLD_USR_UNHOLD:
      {
         rbCurrent = mbLocalHold;
         rbPending = mbLocalUnHoldPending;
      }
      break;

      case eCEEVT_HOLD_NET_HOLD:
      case eCEEVT_HOLD_NET_UNHOLD:
      {
         rbCurrent = mbRemoteHold;
         rbPending = BOS_FALSE;
      }
      break;

      default:
      {
         rbCurrent = BOS_FALSE;
         rbPending = BOS_FALSE;
      }
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceBasicCtlEqual
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
BOS_BOOL CeResCall::resourceBasicCtlEqual(
                        IN MXD_GNS::ISceBasicExtensionControl *pBasicExtCtl )
{
   MXD_GNS::ISceBasicExtensionControl *pMyBasicExtCtl = NULL;
   BOS_BOOL bRet = BOS_FALSE;

   if ( mpUaSspCall )
   {
      mxt_result res = mpUaSspCall->QueryIf( &pMyBasicExtCtl );

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
   }

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceForkAdd
**
**  PURPOSE:    Associate the newly received forked call with the current CE
**              call.
**
**  PARAMETERS:
**              pCall - the call to associate.
**
**
**  RETURNS:    BOS_TRUE - if the association was successful.
**              BOS_FALSE - otherwise.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeResCall::resourceForkAdd( MXD_GNS::IUaSspCall *pCall )
{
   BOS_UINT32 uIx;
   BOS_BOOL bRet = BOS_FALSE;

   for ( uIx = 0 ; uIx < guCeResMaxForkedCall ; uIx++ )
   {
      if ( mpUaSspForkedCall[ uIx ] == NULL )
      {
         bRet = BOS_TRUE;
         mpUaSspForkedCall[ uIx ] = pCall;
         break;
      }
   }

   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceForkRem
**
**  PURPOSE:    Remove and associated forked call from the current CE
**              call knowledge.
**
**  PARAMETERS:
**              pCall - the call to remove.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CeResCall::resourceForkRem( MXD_GNS::IUaSspCall *pCall )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < guCeResMaxForkedCall ; uIx++ )
   {
      if ( pCall && ( mpUaSspForkedCall[ uIx ] == pCall ) )
      {
         mpUaSspForkedCall[ uIx ]->ReleaseIfRef();
         mpUaSspForkedCall[ uIx ] = NULL;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceForkTerm
**
**  PURPOSE:    Terminates the forked calls associated with the CE call.
**
**  PARAMETERS:
**              pCall - the call to terminate.  If NULL (default), terminate
**                      all the associated calls.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CeResCall::resourceForkTerm( MXD_GNS::IUaSspCall *pCall )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < guCeResMaxForkedCall ; uIx++ )
   {
      if ( (( pCall == NULL ) && ( mpUaSspForkedCall[ uIx ] != NULL )) ||
           (( pCall != NULL ) && ( mpUaSspForkedCall[ uIx ] == pCall )) )
      {
         mpUaSspForkedCall[ uIx ]->TerminateCall();   
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceForkTermImmediately
**
**  PURPOSE:    Terminates the forked calls associated with the CE call 
**              immediately.
**
**  PARAMETERS:
**              pCall - the call to terminate.  If NULL (default), terminate
**                      all the associated calls.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CeResCall::resourceForkTermImmediately( MXD_GNS::IUaSspCall *pCall )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < guCeResMaxForkedCall ; uIx++ )
   {
      if ( (( pCall == NULL ) && ( mpUaSspForkedCall[ uIx ] != NULL )) ||
           (( pCall != NULL ) && ( mpUaSspForkedCall[ uIx ] == pCall )) )
      {
         mpUaSspForkedCall[ uIx ]->TerminateImmediately();   
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceReferrer
**
**  PURPOSE:    Creates a referrer to handle out of dialog transfer scenario
**              originated by the local user.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CeResCall::resourceReferrer( void )
{
   MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl = NULL;
   const CFGCALL *pCfgCall;
   const CFGUSR *pCfgUsr;

   mxt_result res = CreateEComInstance(
                       MXD_GNS::CLSID_CSceReferrer,
                       NULL,
                       MXD_GNS::IID_ISceReferrer,
                       reinterpret_cast<void**>( &mpSceReferrer ));
   if ( MX_RIS_S( res ))
   {
      /* Set the manager to receive event information for this service.
      */
      mpSceReferrer->SetManager( gpCeMain );

      /* Configure the referrer.  Note that at the moment there is no per-call
      ** configuration, so the configuration for the referrer is equivalent to
      ** the user configuration for the user owner of this referrer.
      */
      res = mpSceReferrer->QueryIf( &pSceBasicExtCtl );
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
            "CeResCall (%p) - unable to set basic extension manager for referrer.",
            this );
      }

      pCfgCall = CFG_CALL( resourceGuid() );
      if ( pCfgCall != NULL )
      {
         pCfgUsr = CFG_USR ( pCfgCall->in.uUsrId );
         if ( pCfgUsr != NULL )
         {
            mpSceReferrer->SetConfiguration( pCfgUsr->in.pSceUsrCfg );
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourcePushState
**
**  PURPOSE:    Associates a new state to this resource.
**
**  PARAMETERS:
**              newState - the new state to be associated with the resource.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeResCall::resourcePushState( CeFsmBase::CEFSMSTATE eNewState )
{
   /* If this resource is now connected, mark it as so such that we may
   ** trigger specific services on it which are only applicable prior or
   ** after such has happened.
   */
   if ( eNewState == CeFsmBase::eCEFSMSTATE_CALL_CONNECT )
   {
      mbConnect = BOS_TRUE;
   }

   /* Invoke base class for further processing.
   */
   CeResBase::resourcePushState ( eNewState );
}


/***********************************************************************************
**  FUNCTION:   CeResCall::resourceShow
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
void CeResCall::resourceShow( void )
{
#if CCTKCFG_APPLICATION_STATUS
   /* Invoke base class first to display the generic information about
   ** this resource.
   */
   CeResBase::resourceShow();

   /* Display the class specific data associated with this resource.
   */
   CCTK_TRACEI2( "UaSsp [%p], Mse [%p]",
                 mpUaSspCall,
                 mpMse );
   CCTK_TRACEI2( "LinkUaSsp [%p], Ref [%p]",
                 mpUaSspLinkedCall,
                 mpSceReferrer );
   CCTK_TRACEI1( "CalledParty [%s]",
                 mpCalledParty );
   CCTK_TRACEI1( "ProxyGuid [0x%lX]",
                 muProxyGuid );
   
   CCTK_TRACEI4( "Hold: LclHld [%c], LclHldPen [%c], LclUnHldPen [%c], RemHld [%c]",
                 CERESBASE_BOOL_VALUE( mbLocalHold ),
                 CERESBASE_BOOL_VALUE( mbLocalHoldPending ),
                 CERESBASE_BOOL_VALUE( mbLocalUnHoldPending ),
                 CERESBASE_BOOL_VALUE( mbRemoteHold ) );
   CCTK_TRACEI4( "LclTerm [%c], Cnct [%c], EmgRbk [%c], DelRsn [%d]",
                 CERESBASE_BOOL_VALUE( mbLocalTerm ),
                 CERESBASE_BOOL_VALUE( mbConnect ),
                 CERESBASE_BOOL_VALUE( mbEmergRingbck ),
                 mCallDelReason );
   CCTK_TRACEI1( "MedRsn [%d]", mCallMediaReason);

   CCTK_TRACEI1( "Resource [0x%lX] -- END.",
                 resourceGuid() );
#endif /* CCTKCFG_APPLICATION_STATUS */
}
