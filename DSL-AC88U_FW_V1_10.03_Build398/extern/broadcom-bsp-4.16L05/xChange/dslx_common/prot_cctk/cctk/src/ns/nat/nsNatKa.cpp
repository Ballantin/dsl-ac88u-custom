/**********************************************************************************
** Copyright (c) 2008-2012 Broadcom Corporation
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
**      This file defines the Keep Alive module behavior.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <nsNatKa.h>
#include <nsNatStun.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define NS_NAT_KA__SINGLE_CR          0x0D
#define NS_NAT_KA__SINGLE_LF          0x0A
#define NS_NAT_KA__DOUBLE_CRLF        "\x0D\x0A\x0D\x0A"
#define NS_NAT_KA__DOUBLE_CRLF_SIZE   4

/* Default keep alive backoff retry timer value in seconds.
*/
static const BOS_UINT32 guNatKaBackoffTmr = 30;

/* this is taken from CAsyncSocketFactory.cpp, the details of the socket type
 * is specified using strings. Note that if that string changes at some point
 * this too needs to be updated.
 */
static const char cOptionClientStr[9] = "m=client";


/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   NsNatKa::NsNatKa
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
NsNatKa::NsNatKa()
{
   /* Insert this class to the transport service chain.
   ** NOTE: We access the CSipTransportMgr synchronously as we assume the transport
   **       thread has not been started yet (i.e. stack not started yet). Therefore,
   **       this constructor is expected to be running in the context of app's
   **       thread.
   */
   CCTK_ASSERT( MXD_GNS::CSipTransportMgr::GetBrcmTransportSvc() == NULL );
   MXD_GNS::CSipTransportMgr::SetBrcmTransportSvc( this );

#if CCTKCFG_KEEP_ALIVE
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      mpNsNatUsr[uIx] = BRCM_NEW( NSNATKABLK );
      if ( mpNsNatUsr[uIx] )
      {
         memset( mpNsNatUsr[uIx], 0, sizeof( NSNATKABLK ) );
         mpNsNatUsr[uIx]->uResGuid = guCeInvalidGuid;
         mpNsNatUsr[uIx]->uKaGuid = guCeInvalidGuid;
         mpNsNatUsr[uIx]->uTmrId = guCeInvalidGuid;
      }
   }
#endif /* CCTKCFG_KEEP_ALIVE */
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::~NsNatKa
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
NsNatKa::~NsNatKa()
{
   /* Remove this class from the transport service chain.
   ** NOTE: We access the CSipTransportMgr synchronously as we assume the transport
   **       thread has not been started yet (i.e. stack not started yet or it has
   **       already been shut down). Therefore, this destructor is expected to be
   **       running in the context of the application's thread.
   */
   if ( MXD_GNS::CSipTransportMgr::GetBrcmTransportSvc() != NULL )
   {
      /* The custom transport service should have been released during stack
      ** shutdown. This is executed only if the stack has never been started up.
      */
      MXD_GNS::CSipTransportMgr::SetBrcmTransportSvc( NULL );
   }

#if CCTKCFG_KEEP_ALIVE
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      if ( mpNsNatUsr[uIx] )
      {
         if ( mpNsNatUsr[uIx]->puAddr )
         {
            BRCM_DELETE_ARRAY( mpNsNatUsr[uIx]->puAddr );
            mpNsNatUsr[uIx]->puAddr = NULL;
         }
         BRCM_DELETE( mpNsNatUsr[uIx] );
         mpNsNatUsr[uIx] = NULL;
      }
   }
#endif /* CCTKCFG_KEEP_ALIVE */
}

#if CCTKCFG_KEEP_ALIVE
/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaBlk
**
**  PURPOSE:    Locates a keep alive information block based on its keep alive
**              index reference.
**
**  PARAMETERS:
**              uKaGuid - the keep alive index reference.
**
**  RETURNS:    Pointer to the located block or NULL if none is applicable.
**
**  NOTES:
***********************************************************************************/
NSNATKABLK *NsNatKa::nsNatKaBlk( BOS_UINT32 uKaGuid )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      if ( mpNsNatUsr[uIx] &&
           (mpNsNatUsr[uIx]->uResGuid != guCeInvalidGuid) &&
           (mpNsNatUsr[uIx]->uKaGuid == uKaGuid) )
      {
         return mpNsNatUsr[uIx];
      }
   }

   return NULL;
}

/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaStartA
**
**  PURPOSE:    Start a keep alive for a resource identified by the GUID.
**
**  PARAMETERS:
**              uResGuid  - the GUID to start the keep alive on.
**              pPeerAddr - peer address that the keep alive should be sent to
**
**
**  RETURNS:    BOS_TRUE - if successfully started the keep alive.
**              BOS_FALSE - otherwise.
**
**  NOTES:      - Currently the only resource that can control a keep alive are the
**                user resources.  Keep alive is used for checking connectivity to
**                the network element providing service to the user.
**              - This function is asynchronous.
***********************************************************************************/
BOS_BOOL NsNatKa::nsNatKaStartA( BOS_UINT32 uResGuid, MXD_GNS::CSocketAddr *pPeerAddr )
{
   BOS_BOOL bRet = BOS_FALSE;

   CCTK_TRACEI1( "nsNatKaStartA - Guid: 0x%lX", uResGuid );

   if ( cfgGetType( uResGuid ) == eCFGTYPE_USR )
   {
      const CFGUSR *pCfgUsr = CFG_USR( uResGuid );

      if ( pCfgUsr &&
           pCfgUsr->xtl.usr.sec.nat.kaCfg.server.hostPort.pcAddr &&
           pCfgUsr->xtl.usr.sec.nat.kaCfg.uTmr )
      {
         MXD_GNS::CMarshaler *pParms = MXD_GNS::CPool<MXD_GNS::CMarshaler>::New();
         BOS_UINT8 *puAddr = utilStrDup( 
               reinterpret_cast<const BOS_UINT8 *>(pPeerAddr->GetAddress().CStr()) );
         BOS_UINT16 uPort = pPeerAddr->GetPort();

         *pParms << uResGuid
                 << puAddr
                 << uPort
                 << pCfgUsr->xtl.usr.sec.nat.kaCfg.uFailureThreshold;

         mxt_result res = PostMessage( false, eNSNATKA_MSGID_START, pParms );
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI1( "nsNatKaStartA: unable to asynchronously post "
                          "eNSNATKA_MSGID_START message (%d)",
                          res );

            MXD_GNS::CPool<MXD_GNS::CMarshaler>::Delete( pParms );
            BRCM_DELETE_ARRAY( puAddr );
         }
         else
         {
            bRet = BOS_TRUE;
         }
      }
      else
      {
         /* Cannot locate user, or keep alive not configured,
         ** just assume success such that the rest of the application
         ** can work still.
         */
         bRet = BOS_TRUE;
      }
   }

   CCTK_TRACEI2( "nsNatKaStartA - Attempt for 0x%lX returns %s",
                 uResGuid, (bRet ? "success" : "failure") );
   return bRet;
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::internalNsNatKaStartA
**
**  PURPOSE:    Internal function of nsNatKaStartA().
**
**  PARAMETERS:
**              pParms - Parameters from nsNatKaStartA().
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
void NsNatKa::internalNsNatKaStartA( MXD_GNS::CMarshaler *pParms )
{
   CCTK_TRACED2( "NsNatKa(%p) - internalNsNatKaStartA(%p)", this, pParms );

   BOS_UINT32 uResGuid;
   BOS_UINT32 uIx;
   BOS_UINT32 uJx = guCeInvalidGuid;
   BOS_UINT16 uPort;
   BOS_UINT8 *puAddr;
   mxt_result res;
   MXD_GNS::CList<MXD_GNS::IAsyncSocket *> listOfSocket;
   MXD_GNS::IAsyncSocket *pSocket = NULL;
   BOS_UINT32 uSize;
   BOS_UINT32  uMaxTransmission;

   *pParms >> uResGuid
           >> puAddr
           >> uPort
           >> uMaxTransmission;

   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      if ( mpNsNatUsr[uIx] )
      {
         if ( (mpNsNatUsr[uIx]->uResGuid == guCeInvalidGuid) &&
              (uJx == guCeInvalidGuid) )
         {
            uJx = uIx;
         }

         if ( mpNsNatUsr[uIx]->uResGuid == uResGuid )
         {
            uJx = uIx;
            break;
         }
      }
   }

   if ( uJx != guCeInvalidGuid )
   {
      CCTK_TRACEI2( "internalNsNatKaStartA - Keep alive for %s:%i",
            puAddr, uPort );

      /* This is a new keep alive request.
       */
      if ( mpNsNatUsr[uJx]->uResGuid == guCeInvalidGuid )
      {
         mpNsNatUsr[uJx]->uResGuid = uResGuid;
         mpNsNatUsr[uJx]->uKaGuid = uResGuid;
         mpNsNatUsr[uJx]->bKaSent = BOS_FALSE;
         mpNsNatUsr[uJx]->uPort = uPort;
         mpNsNatUsr[uJx]->puAddr = utilStrDup( puAddr );
         mpNsNatUsr[uJx]->uMaxTx = uMaxTransmission;

         /* uTmrId will be initialized when the timer is started.
          * This happens in internalNsNatKaSuccessA */
         mpNsNatUsr[uJx]->uTmrId = guCeInvalidGuid;

         /* Add the user opaque to the socket list */
         res = MXD_GNS::CAsyncSocketFactory::GetSocketList( &listOfSocket );
         if ( MX_RIS_S( res ) )
         {
            uSize = listOfSocket.GetSize();
            for ( uIx = 0 ; uIx < uSize ; uIx++ )
            {
               if ( (pSocket = listOfSocket[uIx]) != NULL )
               {
                  TKMTRANSPORT eType = eTKMTRANSPORT_MAX;

                  eType = nsNatKaTransportType( pSocket );

                  /* For TCP, only add user opaque to the sockets that match the 
                   * destination address.
                   */
                  if ( eType == eTKMTRANSPORT_STREAM )
                  {
                     MXD_GNS::IAsyncIoSocket* pAsyncIoSocket = NULL;

                     res = pSocket->QueryIf(
                           MXD_GNS::IID_IAsyncIoSocket,
                           reinterpret_cast<void **> (&pAsyncIoSocket) );

                     if ( MX_RIS_S( res ) && pAsyncIoSocket )
                     {
                        MXD_GNS::CSocketAddr sockAddr;

                        res = pAsyncIoSocket->GetPeerAddress( &sockAddr );
                        if ( MX_RIS_S( res ) )
                        {
                           if ( !strcmp( sockAddr.GetAddress().CStr(), (char *) mpNsNatUsr[uJx]->puAddr ) )
                           {
                              pSocket->AddUserOpaque( reinterpret_cast< mxt_opaque >(mpNsNatUsr[uJx]->uResGuid));
                           }
                        }

                        pAsyncIoSocket->ReleaseIfRef();
                        pAsyncIoSocket = NULL;
                     }
                  }
                  else
                  {
                     pSocket->AddUserOpaque( reinterpret_cast< mxt_opaque >(mpNsNatUsr[uJx]->uResGuid));
                  }
               }
            }

            /* Release the data.
             */
            for( uIx = 0 ; uIx < uSize ; uIx++ )
            {
               listOfSocket[uIx]->ReleaseIfRef();
            }
            listOfSocket.EraseAll();
         }

         /* Trigger the initial keep alive immediately.
          */
         nsNatKaTick( uResGuid );

      }
      /* This is a 'reset' of an existing keep alive, or a new attempt
       ** to start a previously failed keep alive.
       */
      else
      {
         mpNsNatUsr[uJx]->bKaSent = BOS_FALSE;
         mpNsNatUsr[uJx]->uPort = uPort;
         if ( mpNsNatUsr[uJx]->puAddr )
         {
            BRCM_DELETE_ARRAY( mpNsNatUsr[uJx]->puAddr );
            mpNsNatUsr[uJx]->puAddr = NULL;
         }
         mpNsNatUsr[uJx]->puAddr = utilStrDup( puAddr );

         CEventDriven::StopTimer( mpNsNatUsr[uJx]->uTmrId );
      }
   }

   BRCM_DELETE_ARRAY( puAddr );
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaStopA
**
**  PURPOSE:    Stops a previously started keep alive for a resource identified
**              by the GUID.
**
**  PARAMETERS:
**              uResGuid - the GUID to start the keep alive on.
**
**
**  RETURNS:    BOS_TRUE - if successfully started the keep alive.
**              BOS_FALSE - otherwise.
**
**  NOTES:      - Currently the only resource that can control a keep alive are the
**                user resources.
**              - This function is asynchronous.
***********************************************************************************/
BOS_BOOL NsNatKa::nsNatKaStopA( BOS_UINT32 uResGuid )
{
   BOS_BOOL bRet = BOS_FALSE;

   CCTK_TRACEI1( "nsNatKaStopA - Guid: 0x%lX", uResGuid );

   if ( cfgGetType( uResGuid ) == eCFGTYPE_USR )
   {
      MXD_GNS::CMarshaler *pParms = MXD_GNS::CPool<MXD_GNS::CMarshaler>::New();

      *pParms << uResGuid;

      mxt_result res = PostMessage( false, eNSNATKA_MSGID_STOP, pParms );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "nsNatKaStopA: unable to asynchronously post "
                       "eNSNATKA_MSGID_STOP message (%d)",
                       res );

         MXD_GNS::CPool<MXD_GNS::CMarshaler>::Delete( pParms );
      }
      else
      {
         bRet = BOS_TRUE;
      }
   }

   CCTK_TRACEI2( "nsNatKaStopA - Attempt for 0x%lX returns %s",
                 uResGuid, (bRet ? "success" : "failure") );
   return bRet;
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::internalNsNatKaStopA
**
**  PURPOSE:    Internal function of nsNatKaStopA().
**
**  PARAMETERS:
**              pParms - Parameters from nsNatKaStopA().
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
void NsNatKa::internalNsNatKaStopA( MXD_GNS::CMarshaler *pParms )
{
   CCTK_TRACED2( "NsNatKa(%p) - internalNsNatKaStopA(%p)", this, pParms );

   BOS_UINT32 uResGuid;

   *pParms >> uResGuid;

   BOS_UINT32 uIx;
   BOS_UINT32 uJx = guCeInvalidGuid;

   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      if ( mpNsNatUsr[uIx] )
      {
         if ( (mpNsNatUsr[uIx]->uResGuid == uResGuid) &&
              (uJx == guCeInvalidGuid) )
         {
            uJx = uIx;
            break;
         }
      }
   }

   if ( uJx != guCeInvalidGuid )
   {
      CCTK_TRACEI1( "internalNsNatKaStopA - 0x%lX",
                    mpNsNatUsr[uJx]->uResGuid );

      /* Stop the timer, and release the keep alive block.
      */
      if ( mpNsNatUsr[uJx]->uTmrId != guCeInvalidGuid )
      {
         CEventDriven::StopTimer( mpNsNatUsr[uJx]->uTmrId );
      }

      /* Remove the user opaque from the table */
      {
         mxt_result res;
         MXD_GNS::CList<MXD_GNS::IAsyncSocket *> listOfSocket;
         MXD_GNS::IAsyncSocket *pSocket = NULL;
         BOS_UINT32 uSize;

         res = MXD_GNS::CAsyncSocketFactory::GetSocketList( &listOfSocket );
         if ( MX_RIS_S( res ) )
         {
            uSize = listOfSocket.GetSize();

            /* Remove the user opaque from the socket list */
            for ( uIx = 0 ; uIx < uSize ; uIx++ )
            {
               if ( (pSocket = listOfSocket[uIx]) != NULL )
               {
                  pSocket->RemoveUserOpaque( reinterpret_cast< mxt_opaque >(mpNsNatUsr[uJx]->uResGuid));
               }
            }

            /* Release the data.
            */
            for( uIx = 0 ; uIx < uSize ; uIx++ )
            {
               listOfSocket[uIx]->ReleaseIfRef();
            }

            listOfSocket.EraseAll();
         }
      }

      mpNsNatUsr[uJx]->uTmrId = guCeInvalidGuid;
      mpNsNatUsr[uJx]->uResGuid = guCeInvalidGuid;
      mpNsNatUsr[uJx]->uKaGuid = guCeInvalidGuid;
      mpNsNatUsr[uJx]->bKaSent = BOS_FALSE;
      mpNsNatUsr[uJx]->uPort = 0;
      if ( mpNsNatUsr[uJx]->puAddr )
      {
         BRCM_DELETE_ARRAY( mpNsNatUsr[uJx]->puAddr );
         mpNsNatUsr[uJx]->puAddr = NULL;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaSuccessA
**
**  PURPOSE:    Received confirmation of a successful keep alive for an outstanding
**              keep alive processing.
**
**  PARAMETERS:
**              uResGuid - the resource GUID for which keep alive was successful.
**
**
**  RETURNS:    None.
**
**  NOTES:      - This function is asynchronous.
***********************************************************************************/
void NsNatKa::nsNatKaSuccessA( BOS_UINT32 uResGuid )
{
   CCTK_TRACEI1( "nsNatKaSuccessA - Guid: 0x%lX", uResGuid );

   MXD_GNS::CMarshaler *pParms = MXD_GNS::CPool<MXD_GNS::CMarshaler>::New();

   *pParms << uResGuid;

   mxt_result res = PostMessage( false, eNSNATKA_MSGID_SUCCESS, pParms );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "nsNatKaSuccessA: unable to asynchronously post "
                    "eNSNATKA_MSGID_SUCCESS message (%d)",
                    res );

      MXD_GNS::CPool<MXD_GNS::CMarshaler>::Delete( pParms );
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::internalNsNatKaSuccessA
**
**  PURPOSE:    Internal function of nsNatKaSuccessA().
**
**  PARAMETERS:
**              pParms - Parameters from nsNatKaSuccessA().
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
void NsNatKa::internalNsNatKaSuccessA( MXD_GNS::CMarshaler *pParms )
{
   CCTK_TRACED2( "NsNatKa(%p) - internalNsNatKaSuccessA(%p)", this, pParms );

   BOS_UINT32 uResGuid;
   BOS_UINT32 uIx;
   BOS_UINT32 uJx = guCeInvalidGuid;

   *pParms >> uResGuid;

   const CFGUSR *pCfgUsr = CFG_USR( uResGuid );
   if ( pCfgUsr &&
        pCfgUsr->xtl.usr.sec.nat.kaCfg.uTmr )
   {
      for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
      {
         if ( mpNsNatUsr[uIx]->uResGuid == uResGuid )
         {
            uJx = uIx;
            break;
         }
      }

      if (uJx != guCeInvalidGuid)
      {
         CCTK_TRACEI2( "internalNsNatKaSuccessA - 0x%lX - refresh @ %lu msecs",
                     mpNsNatUsr[uJx]->uResGuid, pCfgUsr->xtl.usr.sec.nat.kaCfg.uTmr );

         /* Initialize the timer ID */
         mpNsNatUsr[uJx]->uTmrId = uResGuid;

         /* Start a keepalive timer */
         CEventDriven::StartTimer(
                        mpNsNatUsr[uJx]->uTmrId,
                        pCfgUsr->xtl.usr.sec.nat.kaCfg.uTmr,
                        static_cast<mxt_opaque>( 0 ),
                        MXD_GNS::ITimerService::
                        ePERIODICITY_NOT_PERIODIC );
      }
   }

   nsNatKaReceived( uResGuid, guCeInvalidGuid );
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaFailureA
**
**  PURPOSE:    Received confirmation of a failed keep alive for an outstanding
**              keep alive processing.
**
**  PARAMETERS:
**              uResGuid - the resource GUID for which keep alive was successful.
**
**
**  RETURNS:    None.
**
**  NOTES:      - This function is asynchronous.
**              - There may be more than one resource associated with this keep
**                alive resource.
***********************************************************************************/
void NsNatKa::nsNatKaFailureA( BOS_UINT32 uResGuid )
{
   CCTK_TRACEI1( "nsNatKaFailureA - Guid: 0x%lX", uResGuid );

   MXD_GNS::CMarshaler *pParms = MXD_GNS::CPool<MXD_GNS::CMarshaler>::New();

   *pParms << uResGuid;

   mxt_result res = PostMessage( false, eNSNATKA_MSGID_FAILURE, pParms );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "nsNatKaFailureA: unable to asynchronously post "
                    "eNSNATKA_MSGID_FAILURE message (%d)",
                    res );

      MXD_GNS::CPool<MXD_GNS::CMarshaler>::Delete( pParms );
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::internalNsNatKaFailureA
**
**  PURPOSE:    Internal function of nsNatKaFailureA().
**
**  PARAMETERS:
**              pParms - Parameters from nsNatKaFailureA().
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
void NsNatKa::internalNsNatKaFailureA( MXD_GNS::CMarshaler *pParms )
{
   CCTK_TRACED2( "NsNatKa(%p) - internalNsNatKaFailureA(%p)", this, pParms );

   BOS_UINT32 uResGuid;
   BOS_UINT32 uIx;

   *pParms >> uResGuid;

   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      if ( mpNsNatUsr[uIx] &&
           (mpNsNatUsr[uIx]->uResGuid != guCeInvalidGuid) &&
           (mpNsNatUsr[uIx]->uResGuid == uResGuid) )
      {
         mpNsNatUsr[uIx]->bKaSent = BOS_FALSE;

         nsNatKaFailure( mpNsNatUsr[uIx]->uResGuid );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaSendA
**
**  PURPOSE:    Keep alive data needs to be sent over the network.
**
**  PARAMETERS:
**              uSockGuid - the GUID referencing how to send this data.
**              puData - the data to be sent.
**              uLen - the size of the data to be sent.
**
**
**  RETURNS:    None.
**
**  NOTES:      - Currently the only resource that can control a keep alive are the
**                user resources.
**              - This function is asynchronous.
***********************************************************************************/
void NsNatKa::nsNatKaSendA( const BOS_UINT32 uSockGuid, const BOS_UINT8 *puData,
                            const BOS_UINT32 uLen )
{
   CCTK_TRACEI1( "nsNatKaSendA - Guid: 0x%lX", uSockGuid );

   MXD_GNS::CMarshaler *pParms = MXD_GNS::CPool<MXD_GNS::CMarshaler>::New();
   BOS_UINT8 *puCopy = utilDataDup( puData, uLen );

   *pParms << uSockGuid
           << puCopy
           << uLen;

   mxt_result res = PostMessage( false, eNSNATKA_MSGID_SEND, pParms );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "nsNatKaSendA: unable to asynchronously post "
                    "eNSNATKA_MSGID_SEND message (%d)",
                    res );

      MXD_GNS::CPool<MXD_GNS::CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( puCopy );
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::internalNsNatKaSendA
**
**  PURPOSE:    Internal function of nsNatKaSendA().
**
**  PARAMETERS:
**              pParms - Parameters from nsNatKaSendA().
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
void NsNatKa::internalNsNatKaSendA( MXD_GNS::CMarshaler *pParms )
{
   CCTK_TRACED2( "NsNatKa(%p) - internalNsNatKaSendA(%p)", this, pParms );

   BOS_UINT32 uSockGuid;
   BOS_UINT32 uLen;
   BOS_UINT8 *puData;
   BOS_UINT32 uIx;
   BOS_UINT32 uJx = guCeInvalidGuid;

   *pParms >> uSockGuid
           >> puData
           >> uLen;


   /* Note that there may be more than one resource user associated with the same
   ** keep alive (or socket) GUID in the case of shared connection.  This does not
   ** really matter in this case however because we are just interested in finding
   ** at least one valid resource that would use that socket.  If none exist, the
   ** message we are trying to send is invalid and should be ignored.  If at least
   ** one exists, use it to send the message, the actual outcome of this operation
   ** will dictate anyways the proper behavior.
   */
   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      if ( mpNsNatUsr[uIx] &&
           (mpNsNatUsr[uIx]->uResGuid != guCeInvalidGuid) &&
           (mpNsNatUsr[uIx]->uKaGuid == uSockGuid) )
      {
         uJx = uIx;
         break;
      }
   }

   /* If we fail to send, not so much of a problem since we would time out
   ** internally anyways.
   **
   ** Because this is a STUN message sending, we need to make sure we send
   ** over the UDP socket.  Only STUN message is coming though this interface.
   */
   if ( uJx != guCeInvalidGuid )
   {
      MXD_GNS::IAsyncSocket *pSocket =
                     nsNatKaSocket( mpNsNatUsr[uJx]->uResGuid,
                                    eTKMTRANSPORT_DATAGRAM );
      if ( pSocket )
      {
         /* Forward the STUN message to the destination that was previously
         ** saved in the internal table during Keep Alive start */
         MXD_GNS::CSocketAddr sockAddr(
            reinterpret_cast<const char *> ( mpNsNatUsr[uJx]->puAddr ),
            static_cast<uint16_t> ( mpNsNatUsr[uJx]->uPort ) );
         MXD_GNS::IAsyncUnconnectedIoSocket* pAsyncUnConnectedIoSocket = NULL;

         mxt_result res = pSocket->QueryIf(
            MXD_GNS::IID_IAsyncUnconnectedIoSocket,
            reinterpret_cast<void **> (&pAsyncUnConnectedIoSocket) );

         if ( MX_RIS_S( res ) && pAsyncUnConnectedIoSocket )
         {
            unsigned int bytesSent;
            pAsyncUnConnectedIoSocket->SendTo(
                        reinterpret_cast<const uint8_t *> (puData),
                        static_cast<unsigned int> (uLen),
                        static_cast<unsigned int*> (&bytesSent),
                        &sockAddr );

            pAsyncUnConnectedIoSocket->ReleaseIfRef();
            pAsyncUnConnectedIoSocket = NULL;
         }
      }
   }

   BRCM_DELETE_ARRAY( puData );
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaTick
**
**  PURPOSE:    A timer started by the keep alive module has timed out.  Need to
**              send the keep alive essentially.
**
**  PARAMETERS:
**              uId - the GUID associated with this timer.
**
**  RETURNS:    Nothing.
**
**  NOTES:      The keep alive timer is recurring, so we do not need to restart
**              a new keep alive timer upon expiration of the previous one.
***********************************************************************************/
void NsNatKa::nsNatKaTick( BOS_UINT32 uId )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uJx = guCeInvalidGuid;

   CCTK_TRACEI1( "nsNatKaTick - Timer 0x%lX", uId );

   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      if ( mpNsNatUsr[uIx] &&
           (mpNsNatUsr[uIx]->uResGuid != guCeInvalidGuid) )
      {
         uJx = uIx;
         break;
      }
   }

   if ( uJx != guCeInvalidGuid )
   {
      /* Verify our keep alive status. If we have not
       * sent a keep alive, we need to send one. */
      if ( !mpNsNatUsr[uJx]->bKaSent )
      {
         /* Send the keep alive to the destination.  Keep alive format is:
         **
         **    - A double CRLF (CRLFCRLF) for a TCP/TLS connection (expects
         **      a single CRLF in response).
         **    - A STUN binding request for a UDP connection (expects a STUN binding
         **      response).
         **
         ** Mark the keep alive as 'sent', this tracks an outstanding keep alive
         ** it will be removed (set to BOS_FALSE) upon confirmation of the keep
         ** alive response.  Otherwise, it will stay set and the next attempt to
         ** keep alive will result in a failure of the connection.
         */
         BOS_BOOL kaSent = BOS_FALSE;
         mxt_result res;
         MXD_GNS::IAsyncSocket *pSocket =
                     nsNatKaSocket( mpNsNatUsr[uJx]->uResGuid );

         if ( pSocket )
         {
            MXD_GNS::CVector<mxt_opaque> vectOpq;
            vectOpq.EraseAll();
            pSocket->GetUserOpaque( &vectOpq );

            switch( nsNatKaTransportType( pSocket ) )
            {
               case eTKMTRANSPORT_STREAM:
               {
                  if ( vectOpq.GetSize() )
                  {
                     /* Always use the first index (should always be present) as the
                     ** keep alive resource for that user.
                     */
                     mpNsNatUsr[uJx]->uKaGuid =
                           reinterpret_cast<BOS_UINT32> ( vectOpq[0] );
                  }

                  MXD_GNS::IAsyncIoSocket* pAsyncIoSocket = NULL;
                  res = pSocket->QueryIf(
                     MXD_GNS::IID_IAsyncIoSocket,
                     reinterpret_cast<void **> (&pAsyncIoSocket) );

                  if ( MX_RIS_S( res ) && pAsyncIoSocket )
                  {
                     BOS_UINT32 bytesSent;

                     res = pAsyncIoSocket->Send(
                        reinterpret_cast<const uint8_t *> (NS_NAT_KA__DOUBLE_CRLF),
                        NS_NAT_KA__DOUBLE_CRLF_SIZE,
                        reinterpret_cast<unsigned int *> (&bytesSent) );
                     if ( MX_RIS_S( res ) &&
                          (bytesSent == NS_NAT_KA__DOUBLE_CRLF_SIZE) )
                     {
                        /* Successfuly sent a keep alive over connected connection.
                        */
                        kaSent = BOS_TRUE;
                     }

                     pAsyncIoSocket->ReleaseIfRef();
                     pAsyncIoSocket = NULL;
                  }
               }
               break;

               case eTKMTRANSPORT_DATAGRAM:
               /* If we could not determine the socket type, assume UDP.
               */
               default:
               {
                  if ( vectOpq.GetSize() )
                  {
                     /* The keep alive index is the GUID that initiated the keep
                     ** alive.  We must set this here such we can retrieve the
                     ** destination of the STUN messages later on.
                     */
                     mpNsNatUsr[uJx]->uKaGuid = mpNsNatUsr[uJx]->uResGuid;
                  }

                  /* Assume success, the STUN module may say otherwise.
                  */
                  kaSent = BOS_TRUE;
                  NsNatStun::nsNatStunBind( mpNsNatUsr[uJx]->uResGuid,
                                            mpNsNatUsr[uJx]->uKaGuid,
                                            eNSNATSTUN_KEEPALIVE,
                                            mpNsNatUsr[uJx]->uMaxTx );
               }
               break;
            }
         }

         /* If keep alive was sent, mark the block as waiting.  Otherwise, declare
         ** a failure.
         */
         if ( kaSent )
         {
            CCTK_TRACED1( "nsNatKaTick - Keep Alive for 0x%lX",
                          mpNsNatUsr[uJx]->uResGuid );
            mpNsNatUsr[uJx]->bKaSent = BOS_TRUE;
         }
         else
         {
            CCTK_TRACEI1( "nsNatKaTick - Failure to Keep Alive 0x%lX",
                          mpNsNatUsr[uJx]->uResGuid );

            nsNatKaFailure( mpNsNatUsr[uJx]->uResGuid );
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaTransportType
**
**  PURPOSE:    Determine the transport type associated with this socket.
**
**  PARAMETERS:
**              pSocket - pointer to the socket to analyze.
**
**
**  RETURNS:    The transport type associated with the socket.
**
**  NOTES:      .
***********************************************************************************/
TKMTRANSPORT NsNatKa::nsNatKaTransportType( MXD_GNS::IAsyncSocket *pSocket )
{
   TKMTRANSPORT eType = eTKMTRANSPORT_MAX;
   MXD_GNS::ISocket::ESocketType eSockType;
   mxt_result res;

   res = pSocket->GetSocketType( &eSockType );
   if ( MX_RIS_S( res ) )
   {
      /* Only look at need for overwritting the default.
      */
      switch ( eSockType )
      {
         case MXD_GNS::ISocket::eSTREAM:
         {
            eType = eTKMTRANSPORT_STREAM;
         }
         break;

         case MXD_GNS::ISocket::eDATAGRAM:
         {
            eType = eTKMTRANSPORT_DATAGRAM;
         }
         break;

         default:
         break;
      }
   }

   return eType;
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaSocket
**
**  PURPOSE:    Determine the transport type associated with this socket.
**
**  PARAMETERS:
**              uResGuid - the resource GUID to get the socket associated with.
**              eTransport - the transport type we want the socket to be.  Can
**                           be left unspecified if we do not care about a
**                           particular transport.
**
**
**  RETURNS:    The socket associated with the resource GUID.
**
**  NOTES:      - A single socket may be associated with multiple user resources
**                in the case where several users are mapped to the same
**                destination.
**              - The socket selection process made here is simple.  It chooses
**                the first available socket for keep alive.  Since the sockets
**                are created in this order (UDP, TCP, TLS) when applicable, it
**                means that the UDP socket will always be chosen if available.
**                If the transport limits to TCP or TLS alone, then that socket
**                will be used instead.
***********************************************************************************/
MXD_GNS::IAsyncSocket* NsNatKa::nsNatKaSocket( BOS_UINT32 uResGuid,
                                               TKMTRANSPORT eTransport )
{
   mxt_result res;
   MXD_GNS::CList<MXD_GNS::IAsyncSocket *> listOfSocket;
   MXD_GNS::IAsyncSocket *pSocket = NULL;
   MXD_GNS::IAsyncSocket *pMatchSocket = NULL;
   MXD_GNS::CVector<mxt_opaque> vectOpq;
   BOS_UINT32 uIx, uJx;
   BOS_UINT32 uSize;
   TKMTRANSPORT eTrans;


   res = MXD_GNS::CAsyncSocketFactory::GetSocketList( &listOfSocket );
   if ( MX_RIS_S( res ) )
   {
      uSize = listOfSocket.GetSize();

      for ( uIx = 0 ; uIx < uSize ; uIx++ )
      {
         if ( (pSocket = listOfSocket.GetAt(uIx)) != NULL )
         {
            vectOpq.EraseAll();

            pSocket->GetUserOpaque( &vectOpq );
            for ( uJx = 0 ; uJx < vectOpq.GetSize() ; uJx++ )
            {
               if ( (pMatchSocket == NULL) &&
                     reinterpret_cast<BOS_UINT32> (
                        vectOpq[ uJx ] ) == uResGuid )
               {
                  eTrans = nsNatKaTransportType( pSocket );
                  if ( ((eTrans != eTKMTRANSPORT_MAX) &&
                           (eTransport != eTKMTRANSPORT_MAX) &&
                           (eTrans == eTransport)) ||
                        (eTransport == eTKMTRANSPORT_MAX) ||
                        (eTrans == eTKMTRANSPORT_MAX) )
                  {
                     if ( eTrans == eTKMTRANSPORT_STREAM )
                     {
                        const char* const* apszType = NULL;
                        unsigned int uTypeSize = 0;

                        apszType = pSocket->GetSocketType(&uTypeSize);
                        for ( int i = 0 ; i < uTypeSize ; i++ )
                        {
                           if (strstr(apszType[i], cOptionClientStr) != NULL)
                           {
                              pMatchSocket = pSocket;
                              break;
                           }
                        }
                     }
                     else
                     {
                        /* assume udp and return socket */
                        pMatchSocket = pSocket;
                     }
                  }
               }
            }
         }
      }

      /* Release the data.
       */
      for( uIx = 0 ; uIx < uSize ; uIx++ )
      {
         listOfSocket[uIx]->ReleaseIfRef();
      }

      listOfSocket.EraseAll();
   }

   return pMatchSocket;
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaReceived
**
**  PURPOSE:    This is a callback from the network layer (either the transport
**              service or the STUN service) indicating that a valid keep-alive
**              has been received for the identifier keep alive.
**
**  PARAMETERS:
**              uResGuid - the resource GUID for which keep alive was successful.
**              uKaGuid - the keep alive GUID to be associated with the correct
**                        resource(s).
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      The keep alive may have been received either for a targetted user
**              in which case we are looking up the resource GUID for that user,
**              or for a keep alive (i.e. a specific socket) in which case we
**              may have to lookup all possible users of that socket.
**              User vs keep alive is dictated by the processing.  Always try to
**              match the user first since it is more precise.
***********************************************************************************/
void NsNatKa::nsNatKaReceived( BOS_UINT32 uResGuid, BOS_UINT32 uKaGuid )
{
   BOS_UINT32 uIx;

   CCTK_TRACEI2( "nsNatKaReceived - Res 0x%lX, Ka 0x%lX", uResGuid, uKaGuid );

   if ( uResGuid != guCeInvalidGuid )
   {
      for ( uIx = 0 ;
            uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ;
            uIx++ )
      {
         if ( mpNsNatUsr[uIx] &&
              (mpNsNatUsr[uIx]->uResGuid != guCeInvalidGuid) &&
              (mpNsNatUsr[uIx]->uResGuid == uResGuid) )
         {
            /* Only care if we were waiting for the keep alive response, which is now
            ** received.
            */
            if ( mpNsNatUsr[uIx]->bKaSent )
            {
               mpNsNatUsr[uIx]->bKaSent = BOS_FALSE;

               nsNatKaSuccess( mpNsNatUsr[uIx]->uResGuid );
            }
         }
      }
   }
   else if ( uKaGuid != guCeInvalidGuid )
   {
      /* Note: there may be more than one internal resource associated with a single
      **       keep alive resource.  This is the case when different internal 
      **       resources are sharing the same keep alive socket (for example).
      **
      **       As such, we cannot distinguish between the actual different known
      **       user resources of that socket (due to type of keep alive we are using
      **       which does not provide this depth of information) for which the keep
      **       alive has succeeded.  However, since all the internal resources are
      **       sharing the same socket ultimately, it is a valid assumption to make
      **       that a success on a shared keep alive resource will affect all
      **       internal users for that resource in a similar manner.
      */
      for ( uIx = 0 ;
            uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ;
            uIx++ )
      {
         if ( mpNsNatUsr[uIx] &&
              (mpNsNatUsr[uIx]->uKaGuid != guCeInvalidGuid) &&
              (mpNsNatUsr[uIx]->uKaGuid == uKaGuid) )
         {
            /* Only care if we were waiting for the keep alive response, which
            ** is now received.
            */
            if ( mpNsNatUsr[uIx]->bKaSent &&
                 (mpNsNatUsr[uIx]->uResGuid != guCeInvalidGuid) )
            {
               mpNsNatUsr[uIx]->bKaSent = BOS_FALSE;

               nsNatKaSuccess( mpNsNatUsr[uIx]->uResGuid );
            }
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaFailure
**
**  PURPOSE:    This is an indication that the keep alive sent for the
**              identified resource has failed to be acknowledged by the
**              remote end.
**
**  PARAMETERS:
**              uResGuid - the resource GUID associated with this resource.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      The GUID will represent the user identifier that issued the keep
**              alive in the first place.
***********************************************************************************/
void NsNatKa::nsNatKaFailure( BOS_UINT32 uResGuid )
{
   CCTK_TRACEI1( "nsNatKaFailure - Guid 0x%lX", uResGuid );

   /* Since keep alive has failed, assign the backoff timer for keep alive
   ** recovery */
   nsNatKaSetTmr ( uResGuid );

   tkmKaFailure( uResGuid );
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaSuccess
**
**  PURPOSE:    This is a confirmation that the keep alive sent for the
**              identified resource has been successfully acknowledged by the
**              remote end.
**
**  PARAMETERS:
**              uResGuid - the GUID associated with this resource.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void NsNatKa::nsNatKaSuccess( BOS_UINT32 uResGuid )
{
   CCTK_TRACEI1( "nsNatKaSuccess - Guid 0x%lX", uResGuid );
   TKMTRANSPORT eType = eTKMTRANSPORT_DATAGRAM;
   BOS_UINT8 uAddress[CCTK_STR_LEN];
   BOS_UINT32 uCapacity = CCTK_STR_LEN;
   MXD_GNS::IAsyncSocket *pSocket;
   MXD_GNS::IAsyncIoSocket* pAsyncIoSocket = NULL;
   mxt_result res;

   memset( uAddress, 0, CCTK_STR_LEN );

   /* Retrieve some information associated with this keep alive socket
   ** for user consumption.
   */
   pSocket = nsNatKaSocket( uResGuid );

   if ( pSocket )
   {
      eType = nsNatKaTransportType( pSocket );

      /* Look up the peer address to be relayed back to the application */
      switch( eType )
      {
         case eTKMTRANSPORT_STREAM:
         {
            res = pSocket->QueryIf(
               MXD_GNS::IID_IAsyncIoSocket,
               reinterpret_cast<void **> (&pAsyncIoSocket) );

            if ( MX_RIS_S( res ) && pAsyncIoSocket )
            {
               MXD_GNS::CSocketAddr sockAddr;

               res = pAsyncIoSocket->GetPeerAddress( &sockAddr );
               if ( MX_RIS_S( res ) )
               {
                  sockAddr.GetAddress( static_cast<unsigned int> (uCapacity),
                                    reinterpret_cast<char *> (uAddress) );
               }

               pAsyncIoSocket->ReleaseIfRef();
               pAsyncIoSocket = NULL;
            }
         }
         break;

         case eTKMTRANSPORT_DATAGRAM:
         /* If we could not determine the socket type, assume UDP.
         */
         default:
         {
            NSNATKABLK *pKaBlk = nsNatKaBlk ( uResGuid );
            if ( pKaBlk )
            {

               /* For UDP, since the socket is not connected, we cannot look up
               ** the peer address from the socket interface.  We will instead
               ** use the saved peer address in the internal table */
               strncpy( (char *)uAddress, (char *)pKaBlk->puAddr, uCapacity );
            }
         }
         break;
      }
   }

   tkmKaSuccess( uResGuid, eType, uAddress );
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaEndTimer
**
**  PURPOSE:    Terminate all timers that may be running under the control of the
**              keep alive module.  This is a hard (and final) termination, so this
**              function should only be called with care and for example during
**              final shutdown of the system.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    CCTKSTATUS defining the final response on the action.
**
**  NOTES:      - This function is synchronous.
***********************************************************************************/
CCTKSTATUS NsNatKa::nsNatKaEndTimer( void )
{
   CCTK_TRACEI0( "nsNatKaEndTimer" );

   mxt_result res = PostMessage( true, eNSNATKA_MSGID_ENDTIMER, NULL );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "nsNatKaEndTimer: unable to asynchronously post "
                    "eNSNATKA_MSGID_ENDTIMER message (%d)",
                    res );

      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::internalNsNatKaEndTimer
**
**  PURPOSE:    Internal function of nsNatKaEndTimer().
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
void NsNatKa::internalNsNatKaEndTimer( void )
{
   CCTK_TRACED1( "NsNatKa(%p) - internalNsNatKaEndTimer", this );

   BOS_UINT32 uIx;

   /* Terminate all the timers that may be running on any of the possible keep
   ** alive blocks.
   */
   for ( uIx = 0 ; uIx < ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ; uIx++ )
   {
      if ( mpNsNatUsr[uIx]->uTmrId != guCeInvalidGuid )
      {
         CEventDriven::StopTimer( mpNsNatUsr[uIx]->uTmrId );
         mpNsNatUsr[uIx]->uTmrId = guCeInvalidGuid;
      }
   }
}

/***********************************************************************************
**  FUNCTION:   NsNatKa::nsNatKaBackOff
**
**  PURPOSE:    Gets the value of the backoff timer for the keep alive recovery
**              process.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    The timer, or 0 if none applicable (error scenario).
**
**  NOTES:
***********************************************************************************/
BOS_UINT32 NsNatKa::nsNatKaBackOff( BOS_UINT32 uGuid )
{
   NSNATKABLK *pKaBlk = nsNatKaBlk ( uGuid );
   BOS_UINT32 uTmr = 0;

   if ( pKaBlk )
   {
      uTmr = pKaBlk->uBackOffTmr;
   }

   return uTmr;
}

/***********************************************************************************
**  FUNCTION:   nsNatKaSetTmr
**
**  PURPOSE:    Assign the next value for the backoff retry timer for keep
**              alive recovery
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    Nothing, but the timer value in the keep alive block
**              is updated accordingly.
**
**  NOTES:
***********************************************************************************/
void NsNatKa::nsNatKaSetTmr( BOS_UINT32 uGuid )
{
   NSNATKABLK *pKaBlk = nsNatKaBlk ( uGuid );
   const CFGUSR *pCfgUsr;
   BOS_UINT32  uBackoffTmrS = guNatKaBackoffTmr;

   if ( pKaBlk )
   {
      if ( ( pCfgUsr = CFG_USR( uGuid ) ) != NULL )
      {
         /* The upper bound of the backoff retry timer (or the flow recovery time) 
         ** is determined via the following formula:
         ** 
         **    W = min( max-time, ( base-time * ( 2 ^ consecutive-failures) )
         ** 
         ** where:
         **    max-time with a default of 1800 sec
         **    base-time (if all failed) with a default of 30 sec
         **    base-time (if all have not failed) with a default of 90 sec
         */

         BOS_UINT32  uFlowMaxTmrS = pCfgUsr->xtl.reg.gen.tmr.uFlowMaxTmr;
         BOS_UINT32  uFlowBaseNokTmrS = pCfgUsr->xtl.reg.gen.tmr.uFlowBaseNokTmr;
         BOS_UINT32  uFlowBaseOkTmrS = pCfgUsr->xtl.reg.gen.tmr.uFlowBaseOkTmr;

         /* Since we only support 1 flow per destination, a flow failure essentially
         ** translates to all flow failure */
         BOS_BOOL    bAllFlowsFailed = TRUE;

         /* Track the number of consecutive failure counts.  We currently assume
         ** no consecutive failure counts */
         BOS_UINT32  uFailureCount = 0;

         BOS_UINT32  uBaseTmrS;

         uBackoffTmrS = uFlowMaxTmrS;

         /* Determine the base timer first.  The decision is based on whether
         ** or not all flows have failed */
         uBaseTmrS = bAllFlowsFailed ? uFlowBaseNokTmrS : uFlowBaseOkTmrS;

         /* If failure count is bigger than 31, the multiplication factor will be 0,
         ** so no need to do a costly operation in that case */
         if ( uFailureCount < 31 )
         {
             BOS_UINT32 uTempS = uBaseTmrS * (1 << uFailureCount);

             /* Use the minimum of the max-time or base-time calculation */
             if ( uTempS < uFlowMaxTmrS )
             {
                 uBackoffTmrS = uTempS;
             }
         }

         /* Compute the actual backoff time by selecting a uniform random time
         ** between 50% and 100% of the upper-bound wait time.  It is logical 
         ** to only apply this random range if the selected timer is at least 
         ** larger than a certain value (i.e. 2) */
         if ( uBackoffTmrS > 2 )
         {
            BOS_UINT32 uBackoffTmrHalf = uBackoffTmrS / 2;
            uBackoffTmrS = uBackoffTmrHalf + ( rand() % uBackoffTmrHalf );
         }
      }

      /* Assign the new timer value.
      */
      pKaBlk->uBackOffTmr = uBackoffTmrS;
   }
}

#endif /* CCTKCFG_KEEP_ALIVE */



/* ------------------------------------------------------------------------------ */
/* ---- BELOW THIS POINT, THE CALLBACK INTERFACE FROM THE STACK IS IMPLEMENTED -- */
/* ---- THIS IS THE ONLY PORTION OF THE KEEP ALIVE MODULE THAT MAY -------------- */
/* ---- (SHOULD) BE IN DIRECT CONTACT WITH THIRD PARTY PROCESSING. -------------- */
/* ------------------------------------------------------------------------------ */

/***********************************************************************************
**  FUNCTION:   NsNatKa::ReceivedFromNetwork
**
**  PURPOSE:    Callback from the custom transport service when data is received
**              from the network before passing calling other stack's transport
**              services. This event callback can be used to filtering the data
**              content received from the network by validating/modifying the
**              stack's principal buffer, i.e. g_pPrincipalBuffer. This event
**              callback can also be used to manage outgoing keep-alive through
**              the CBrcmTransportSvc timer API (inherited from CEventDriven).
**
**  PARAMETERS:
**              pPacket - The parsed packet received from the network. This must be
**                        NULL as this is the first transport service being called
**                        from the network.
**              pSocket - The local socket that has received the data.
**                        This parameter must NOT be NULL as this is the first
**                        transport service being called from the network.
**
**  RETURNS:    None
**
**  NOTES:      - This event is called within the context of the transport threads.
**                Always queue up for core thread processing. Data exchange between
**                CCTK and this requires synchronization.
**              - This event MUST call CSipTransportSvc::ReceivedFromNetwork() at
**                the end to pass the control to the other stack's transport
**                services in the receive chain. Otherwise, the other stack's
**                transport services will not be notified of the incoming data.
**                This is particularly useful when one wishes to filter out the
**                incoming data from the stack's receive chain. Refer to
**                CSipParserSvc for examples.
***********************************************************************************/
void NsNatKa::ReceivedFromNetwork( MXD_GNS::CSipPacket *pPacket,
                                   MXD_GNS::CSipClientSocket *pSocket )
{
   BOS_BOOL bTerminate = BOS_FALSE;
   CCTK_TRACED3(
      "NsNatKa(%p) - ReceivedFromNetwork(%p, %p)", this, pPacket, pSocket );

   /* This custom transport service is placed before the stack's parser service.
   ** Thus, the data should not have been parsed yet.
   */
   CCTK_ASSERT( pPacket == NULL );

   /* pSocket is always valid since this is the first transport service to be
   ** called from the network.
   */
   CCTK_ASSERT( pSocket != NULL );

#if CCTKCFG_KEEP_ALIVE
   /* Ensure there is valid data in the principal buffer.  Then check whether
   ** we think this data is for keep alive processing or not.  If it is, we will
   ** terminate it at this layer, otherwise we will pass it on.
   */
   if ( (MXD_GNS::g_pPrincipalBuffer->GetEndIndexPtr() -
            MXD_GNS::g_pPrincipalBuffer->GetFirstIndexPtr()) != 0 )
   {
      MXD_GNS::CVector<mxt_opaque> vectOpq;

      vectOpq.EraseAll();
      pSocket->GetUserOpaque( &vectOpq );

      /* If this is a stream oriented transport, we care about whether the
      ** content if only 'CRLF'.
      */
      if ( MXD_GNS::CSipTransportTools::IsStreaming( pSocket->GetTransport() ))
      {
        BOS_UINT8 *pData =
            (static_cast<BOS_UINT8 *>
                (MXD_GNS::g_pPrincipalBuffer->GetFirstIndexPtr() ));

         if( ( *pData == NS_NAT_KA__SINGLE_CR ) &&
             ( *(pData + 1) == NS_NAT_KA__SINGLE_LF ) )
         {
            if ( vectOpq.GetSize() )
            {
               /* Always use the first entry in the opaque vector (there should be
               ** at least one always) to index the keep alive sent.
               */
               nsNatKaReceived(
                  guCeInvalidGuid,
                  reinterpret_cast<BOS_UINT32> ( vectOpq[0] ) );
               /* For stream oriented protocol, we are allowed to receive a packet
               ** with leading CRLF, therefore we should not 'terminate' this data
               ** in such case as the next transport chain entry may see something
               ** more into it.  If this was only a keep alive, then no big deal,
               ** it will be ignored.  If this was not a keep alive, then the keep
               ** alive process would have ignored it as well.
               */
               /* bTerminate = BOS_TRUE; */
            }
         }
      }
      /* If this is a datagram oriented transport, we care about the information
      ** being STUN.
      */
      else
      {
         /* This call is synchronous, it should not hold the processing of the
         ** transport thread.
         */
         if ( NsNatStun::nsNatStunIsStun(
                static_cast<BOS_UINT8 *> (
                     MXD_GNS::g_pPrincipalBuffer->GetFirstIndexPtr() ),
                static_cast<BOS_UINT32> (
                     MXD_GNS::g_pPrincipalBuffer->GetEndIndexPtr() -
                           MXD_GNS::g_pPrincipalBuffer->GetFirstIndexPtr() )) )
         {
            bTerminate = BOS_TRUE;
            /* This call MUST be queued to the thread dealing with STUN.
            */
            if ( vectOpq.GetSize() )
            {
               /* Always use the first entry in the opaque vector (there should be
               ** at least one always) to index the keep alive sent.
               */
               NsNatStun::nsNatStunProcess(
                  reinterpret_cast<BOS_UINT32> ( vectOpq[0] ),
                  static_cast<BOS_UINT8 *> (
                       MXD_GNS::g_pPrincipalBuffer->GetFirstIndexPtr() ),
                  static_cast<BOS_UINT32> (
                       MXD_GNS::g_pPrincipalBuffer->GetEndIndexPtr() -
                             MXD_GNS::g_pPrincipalBuffer->GetFirstIndexPtr() ) );
            }
         }
      }
   }
#endif /* CCTKCFG_KEEP_ALIVE */

   if ( bTerminate )
   {
      /* Erase the buffer and do not forward.
      */
      MXD_GNS::g_pPrincipalBuffer->EraseAll();
   }
   else
   {
      /* Forward the packet to the next entry in the chain.
      */
      CSipTransportSvc::ReceivedFromNetwork( pPacket, pSocket );
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::SendToNetwork
**
**  PURPOSE:    Callback from the custom transport service when data is received
**              from the core side of the transport service chain, i.e.
**              CSipParserSvc. Since this service is the last in the chain,
**              it can be used to monitoring all outgoing traffic when keep-alive
**              is enabled. It is expected that when this event callback is called,
**              the stack's principle buffer should be filled with the outgoing
**              packet content.
**
**  PARAMETERS:
**              rPacketOut - The actual SIP packet to send on the network.
**              pSource    - The socket on which to send the packet.
**              pUser      - The instance to which report the transmission progress.
**              opq        - Application's opaque data related to this operation.
**
**  RETURNS:    Status from CSipTransportSvc::SendToNetwork().
**
**  NOTES:      - This event is called within the context of the transport threads.
**                Always queue up for core thread processing. Data exchange between
**                CCTK and this requires synchronization.
**              - This event MUST call CSipTransportSvc::SendToNetwork() at
**                the end to send the data, which is generated from the stack's
**                transport services, to the network. Refer to CSipParserSvc for
**                examples.
**              - It is recommended that the stack's principle buffer should not be
**                touched in this function.
***********************************************************************************/
mxt_result NsNatKa::SendToNetwork( MXD_GNS::CSipPacket &rPacketOut,
                                   MXD_GNS::CSipClientSocket *pSource,
                                   MXD_GNS::ISipTransportUser *pUser,
                                   mxt_opaque opq )
{
   CCTK_TRACED4(
      "NsNatKa(%p) - SendToNetwork(%p, %p, %p)", this, pSource, pUser, opq );

   CCTK_ASSERT( pSource != NULL );
   if ( pSource != NULL )
   {
      /* Update the socket's TOS option.
      */
      rPacketOut.SetDscp( reinterpret_cast<uint32_t> ( rPacketOut.GetTosOpaque() ) );
   }

   /* Make sure the packet is destined to be send out to the network.
   */
   if ( rPacketOut.GetSendOption() )
   {
      return CSipTransportSvc::SendToNetwork( rPacketOut, pSource, pUser, opq );
   }
   /* If not, report a failure to send this packet.
   */
   else
   {
      return resFE_ABORT;
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::EvMessageServiceMgrAwaken
**
**  PURPOSE:    Notifies the manager that a new message must be processed.
**
**  PARAMETERS:
**              bWaitingCompletion  - True if the message is being processed
**                                    synchronously, false if processed
**                                    asynchronously.
**              uMessage            - The identifier of the message that must be
**                                    posted.
**              pParameter          - An optional CMarshaler parameter.
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void NsNatKa::EvMessageServiceMgrAwaken( bool bWaitingCompletion,
                                         unsigned int uMessage,
                                         MXD_GNS::CMarshaler *pParameter )
{
   NSNATKA_MSGID eMsg = static_cast< NSNATKA_MSGID >( uMessage );
   switch( eMsg )
   {
#if CCTKCFG_KEEP_ALIVE
      case eNSNATKA_MSGID_START:
      {
         internalNsNatKaStartA( pParameter );
      }
      break;

      case eNSNATKA_MSGID_STOP:
      {
         internalNsNatKaStopA( pParameter );
      }
      break;

      case eNSNATKA_MSGID_SUCCESS:
      {
         internalNsNatKaSuccessA( pParameter );
      }
      break;

      case eNSNATKA_MSGID_FAILURE:
      {
         internalNsNatKaFailureA( pParameter );
      }
      break;

      case eNSNATKA_MSGID_SEND:
      {
         internalNsNatKaSendA( pParameter );
      }
      break;

      case eNSNATKA_MSGID_ENDTIMER:
      {
         internalNsNatKaEndTimer();
      }
      break;

#else
      case eNSNATKA_MSGID_START:
      case eNSNATKA_MSGID_STOP:
      case eNSNATKA_MSGID_SUCCESS:
      case eNSNATKA_MSGID_FAILURE:
      case eNSNATKA_MSGID_SEND:
      case eNSNATKA_MSGID_ENDTIMER:
#endif /* CCTKCFG_KEEP_ALIVE */
      default:
      {
         CBrcmTransportSvc::EvMessageServiceMgrAwaken( bWaitingCompletion,
                                                       uMessage,
                                                       pParameter );
      }
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   NsNatKa::EvTimerServiceMgrAwaken
**
**  PURPOSE:    Notifies the manager that a new timer elapsed or has been stopped.
**
**  PARAMETERS:
**              bStopped   - True if the timer has been explicitly stopped, false
**                           if it normally elapsed. For an exponential timer,
**                           true is also sent if the ceiling has been reached
**                           and bStopAtCeiling is true.
**              uTimer     - The identifier of the timer.
**              opq        - An optional opaque supplied by the caller.
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void NsNatKa::EvTimerServiceMgrAwaken( bool bStopped,
                                       unsigned int uTimer,
                                       mxt_opaque opq )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( opq );

#if CCTKCFG_KEEP_ALIVE
   /* Invoke the keep alive tick on timer expiry, if the timer was
   ** stopped forcefully, do not invoke the keep alive tick as it is
   ** assumed the forcefull stop happened for a reason.
   */
   if ( !bStopped )
   {
      nsNatKaTick( static_cast<BOS_UINT32> (uTimer) );
   }
   else
   {
      CCTK_TRACEI1( "NsNatKa - Timer 0x%lX explicitely stopped",
                    static_cast<BOS_UINT32> (uTimer) );
   }
#else
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( bStopped );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uTimer );
#endif /* CCTKCFG_KEEP_ALIVE */
}
