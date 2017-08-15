/**********************************************************************************
** Copyright (c) 2011-2013 Broadcom Corporation
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
**      This file implements the resource notifier class.
**
**      The resource notifier class can be used for several purposes including:
**
**             - Handling incoming subscription and notifying subscribed users
**               accordingly.
**             - Handling out-of-dialog incoming notifications (received from
**               a peer).
**             - Handling out-of-dialog outgoing notifications (to be sent to a
**               peer).
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <ceMain.h>
#include <ceResNot.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceCreate
**
**  PURPOSE:    Creates the necessary network resource support at the UASSP layer.
**
**  PARAMETERS:
**              uCfgGuid - the GUID to locate the configuration associated with
**                         this resource.
**              eType    - the type of publication for this notifier resource.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
BOS_BOOL CeResNot::resourceCreate( BOS_UINT32 uCfgGuid, CCTKPUBLISH eType )
{
   BOS_BOOL bRet = BOS_TRUE;
   MXD_GNS::ISceBasicExtensionControl *pSceBasicExtCtl;

   /* Create the resource instance at the UA layer.
   */
   mxt_result res = CreateEComInstance(
                       MXD_GNS::CLSID_CSceOodRequestSender,
                       NULL,
                       MXD_GNS::IID_ISceOodRequestSender,
                       reinterpret_cast<void**>( &mpSceOodReqSnd ));
   if ( MX_RIS_S( res ))
   {
      const CFGUSR *pCfgUsr = CFG_USR ( uCfgGuid );

      mePubType = eType;

      /* Set the manager to receive event information for this service.
      */
      mpSceOodReqSnd->SetOodRequestSenderManager( gpCeMain );

      res = mpSceOodReqSnd->QueryIf( &pSceBasicExtCtl );
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
            "CeResNot (%p) - unable to set basic extension manager.",
            this );
      }

      if ( pCfgUsr != NULL )
      {
         mpSceOodReqSnd->SetConfig( pCfgUsr->in.pSceUsrCfg );
      }
      else
      {
         bRet = BOS_FALSE;
      }
   }
   else
   {
      CCTK_TRACEI1( "CeResNot (%p) - unable to create SCE out-of-dialog request.",
                    this );
      bRet = BOS_FALSE;
   }

   CCTK_TRACED2( "CeResNot (%p) - Resource creation: %s.",
                 this,
                 ( bRet ? "Success" : "Fail" ));
   return bRet;
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceBasicCtlEqual
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
BOS_BOOL CeResNot::resourceBasicCtlEqual(
                        IN MXD_GNS::ISceBasicExtensionControl *pBasicExtCtl )
{
   MXD_GNS::ISceBasicExtensionControl *pMyBasicExtCtl = NULL;
   BOS_BOOL bRet = BOS_FALSE;

   if ( mpSceOodReqSnd )
   {
      mxt_result res = mpSceOodReqSnd->QueryIf( &pMyBasicExtCtl );

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
**  FUNCTION:   CeResNot::resourcePushData
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
void CeResNot::resourcePushData( CEEVT eEvt, void *pData )
{
   if ( pData == NULL )
   {
      return;
   }

   switch ( eEvt )
   {
      case eCEEVT_NOT_USR_PUBLISH:
      case eCEEVT_NOT_USR_MSG:
      {
         const BOS_UINT8 *pPubParty =
                           static_cast<const BOS_UINT8 *> ( pData );

         mpPubParty = utilStrDup( pPubParty );
      }
      break;

      case eCEEVT_NOT_NET_MSG:
      {
         mpBody = (static_cast<CeMain::CEMAINDATA *> ( pData ))->puData;
         muSize = (static_cast<CeMain::CEMAINDATA *> ( pData ))->uSize;
         mpSrvEvtCtl = static_cast<MXD_GNS::ISipServerEventControl *> (
            (static_cast<CeMain::CEMAINDATA *> ( pData ))->pAdditionalNotInfo );
      }
      break;

      case eCEEVT_NOT_NET_OK:
      case eCEEVT_NOT_NET_FAIL:
      case eCEEVT_NOT_USR_RSP:
      {
         mFinalRsp =
            *(static_cast<BOS_UINT32 *> ( pData ));
      }
      break;

      default:
      break;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceMetrics
**
**  PURPOSE:    Format the metrics to be published in a correct output.
**
**  PARAMETERS:
**              puData - where to put the data formatted.
**              uSize - the size of the formatted data holder.
**
**
**  RETURNS:    The actual size of the formatted data written to the buffer.
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 CeResNot::resourceMetrics( BOS_UINT8 *puData, BOS_UINT32 uSize )
{
   BOS_UINT32 uLen = 0;
   CeCdr *pCdr;

   memset( reinterpret_cast<void *> (puData),
           0,
           uSize );

   /* Get the CDR for this call which contains all the information we need.
   */
   pCdr =
       CeMain::ceCdrMgr()->locatePrevCdr( resourceParent(),
                                          resourceGetCall() );
   if ( pCdr && pCdr->cdrData() && pCdr->cdrMetrics() && pCdr->cdrCnx() )
   {
      CCTKCDR *pData = pCdr->cdrData();
      CCTKCALLMETRICS *pMetrics = pCdr->cdrMetrics();
      CeCdr::CECDRCNX *pCnx = pCdr->cdrCnx();
      BOS_UINT8 ipAddr[CCTK_STR_LEN];
      struct tm start_tm, end_tm;

      snprintf( reinterpret_cast<char *> (puData), 
                uSize, "VQSessionReport\n" );
      uLen = strlen( reinterpret_cast<char *> (puData));

      /* Fill in local metrics.
      */
      snprintf( reinterpret_cast<char *> (&puData[uLen]), 
               (uSize - uLen),
               "LocalMetrics:\n" );
      uLen = strlen( reinterpret_cast<char *> (puData));
      bosTimeGetLocalTime( reinterpret_cast<const time_t *> (&pData->timeStart),
                           &start_tm );
      bosTimeGetLocalTime( reinterpret_cast<const time_t *> (&pData->timeStop),
                           &end_tm );
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "TimeStamps:START=%d%d%d.%d.%d.%d STOP=%d%d%d.%d.%d.%d\n",
               start_tm.tm_mday,
               start_tm.tm_mon + 1,
               start_tm.tm_year + 1900,
               start_tm.tm_hour,
               start_tm.tm_min,
               start_tm.tm_sec,
               end_tm.tm_mday,
               end_tm.tm_mon + 1,
               end_tm.tm_year + 1900,
               end_tm.tm_hour,
               end_tm.tm_min,
               end_tm.tm_sec );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "SessionDesc:PT=%u PD=%s PLC=%u\n", 
               pCnx->uRtpCodeLcl,
               MXD_GNS::CSdpParser::ms_apszRtpCompressionAlgorithmMap[pCnx->uPrsrCodeLcl],
               (( pMetrics->bExtCallStats ) ?
                     pMetrics->extCallStatsLoc.uPktLossConcealmentType : 0 ));
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "CallID:%s\n", pData->uCallId );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "FromID:%s\n", pCdr->cdrLocal() );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "ToID:%s\n", pCdr->cdrRemote() );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "OrigID:%s\n",
               (pCdr->cdrType() == CeCdr::eCECDRTYPE_IN) ?
                     pCdr->cdrRemote() : pCdr->cdrLocal() );
      uLen = strlen( reinterpret_cast<char *> (puData));
      bosIpAddrGetStr ( &pCnx->ipLcl,
                        reinterpret_cast<char *> (ipAddr),
                        CCTK_STR_LEN );
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "LocalAddr:IP=%s PORT=%u SSRC=%08X\n",
               ipAddr, pCnx->uPortLcl, (unsigned int)pMetrics->callStats.uLclSSRC );
      uLen = strlen( reinterpret_cast<char *> (puData));
      bosIpAddrGetStr ( &pCnx->ipRem,
                        reinterpret_cast<char *> (ipAddr),
                        CCTK_STR_LEN );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "RemoteAddr:IP=%s PORT=%u SSRC=%08X\n",
               ipAddr, pCnx->uPortRem, (unsigned int)pMetrics->callStats.uRemSSRC );
      uLen = strlen( reinterpret_cast<char *> (puData));
      if ( pMetrics->bExtCallStats )
      {
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "JitterBuffer:JBA=%u JBR=%u JBN=%u JBM=%u JBX=%u\n",
                  pMetrics->extCallStatsLoc.uJbAdaptive,
                  pMetrics->extCallStatsLoc.uJbRate,
                  pMetrics->extCallStatsLoc.uJbNominal,
                  pMetrics->extCallStatsLoc.uJbMax,
                  pMetrics->extCallStatsLoc.uJbAbsMax );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "PacketLoss:NLR=%u JDR=%u\n",
                  pMetrics->extCallStatsLoc.uNetPktLossRate,
                  pMetrics->extCallStatsLoc.uJbDiscardRate );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "BurstGapLoss:BLD=%u BD=%u GLD=%u GD=%u GMIN=%u\n",
                  pMetrics->extCallStatsLoc.uBurstLossDensity,
                  pMetrics->extCallStatsLoc.uBurstDuration,
                  pMetrics->extCallStatsLoc.uGapLossDensity,
                  pMetrics->extCallStatsLoc.uGapDuration,
                  pMetrics->extCallStatsLoc.uMinGapThreshold );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "Delay:RTD=%u ESD=%u IAJ=%u\n",
                  pMetrics->extCallStatsLoc.uRdTripNetDelay,
                  pMetrics->extCallStatsLoc.uEndSystemDelay,
                  pMetrics->extCallStatsLoc.uInterArrivalJitter );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "Signal:SL=%d NL=%d RERL=%u\n",
                  pMetrics->extCallStatsLoc.sSignalLevel,
                  pMetrics->extCallStatsLoc.sNoiseLevel,
                  pMetrics->extCallStatsLoc.uResidualEchoRetLoss );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "QualityEst:MOSLQ=%u MOSCQ=%u RCQ=%u EXTRI=%u\n",
                  pMetrics->extCallStatsLoc.uEstMOSLQ,
                  pMetrics->extCallStatsLoc.uEstMOSCQ,
                  pMetrics->extCallStatsLoc.uRFactor,
                  pMetrics->extCallStatsLoc.uExtRFactor );
         uLen = strlen( reinterpret_cast<char *> (puData));
      }

      /* Fill in remote metrics. */
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "RemoteMetrics:\n" );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "TimeStamps:START=%d%d%d.%d.%d.%d STOP=%d%d%d.%d.%d.%d\n",
               start_tm.tm_mday,
               start_tm.tm_mon + 1,
               start_tm.tm_year + 1900,
               start_tm.tm_hour,
               start_tm.tm_min,
               start_tm.tm_sec,
               end_tm.tm_mday,
               end_tm.tm_mon + 1,
               end_tm.tm_year + 1900,
               end_tm.tm_hour,
               end_tm.tm_min,
               end_tm.tm_sec );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "SessionDesc:PT=%u PD=%s PLC=%u\n",
               pCnx->uRtpCodeRem, 
               MXD_GNS::CSdpParser::ms_apszRtpCompressionAlgorithmMap[pCnx->uPrsrCodeRem],
               (( pMetrics->bExtCallStats ) ?
                     pMetrics->extCallStatsRem.uPktLossConcealmentType : 0 ));
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "CallID:%s\n", pData->uCallId );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "FromID:%s\n", pCdr->cdrLocal() );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "ToID:%s\n", pCdr->cdrRemote() );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "OrigID:%s\n",
               (pCdr->cdrType() == CeCdr::eCECDRTYPE_IN) ?
                     pCdr->cdrRemote() : pCdr->cdrLocal() );
      bosIpAddrGetStr ( &pCnx->ipLcl,
                        reinterpret_cast<char *> (ipAddr),
                        CCTK_STR_LEN );
      uLen = strlen( reinterpret_cast<char *> (puData));
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "LocalAddr:IP=%s PORT=%u SSRC=%08X\n",
               ipAddr, pCnx->uPortLcl, (unsigned int)pMetrics->callStats.uLclSSRC );
      uLen = strlen( reinterpret_cast<char *> (puData));
      bosIpAddrGetStr ( &pCnx->ipRem,
                        reinterpret_cast<char *> (ipAddr),
                        CCTK_STR_LEN );
      snprintf( reinterpret_cast<char *> (&puData[uLen]),
               (uSize - uLen),
               "RemoteAddr:IP=%s PORT=%u SSRC=%08X\n",
               ipAddr, pCnx->uPortRem, (unsigned int)pMetrics->callStats.uRemSSRC );
      uLen = strlen( reinterpret_cast<char *> (puData));
      if ( pMetrics->bExtCallStats )
      {
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "JitterBuffer:JBA=%u JBR=%u JBN=%u JBM=%u JBX=%u\n",
                  pMetrics->extCallStatsRem.uJbAdaptive,
                  pMetrics->extCallStatsRem.uJbRate,
                  pMetrics->extCallStatsRem.uJbNominal,
                  pMetrics->extCallStatsRem.uJbMax,
                  pMetrics->extCallStatsRem.uJbAbsMax );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "PacketLoss:NLR=%u JDR=%u\n",
                  pMetrics->extCallStatsRem.uNetPktLossRate,
                  pMetrics->extCallStatsRem.uJbDiscardRate );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "BurstGapLoss:BLD=%u BD=%u GLD=%u GD=%u GMIN=%u\n",
                  pMetrics->extCallStatsRem.uBurstLossDensity,
                  pMetrics->extCallStatsRem.uBurstDuration,
                  pMetrics->extCallStatsRem.uGapLossDensity,
                  pMetrics->extCallStatsRem.uGapDuration,
                  pMetrics->extCallStatsRem.uMinGapThreshold );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "Delay:RTD=%u ESD=%u IAJ=%u\n",
                  pMetrics->extCallStatsRem.uRdTripNetDelay,
                  pMetrics->extCallStatsRem.uEndSystemDelay,
                  pMetrics->extCallStatsRem.uInterArrivalJitter );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "Signal:SL=%d NL=%d RERL=%u\n",
                  pMetrics->extCallStatsRem.sSignalLevel,
                  pMetrics->extCallStatsRem.sNoiseLevel,
                  pMetrics->extCallStatsRem.uResidualEchoRetLoss );
         uLen = strlen( reinterpret_cast<char *> (puData));
         snprintf( reinterpret_cast<char *> (&puData[uLen]),
                  (uSize - uLen),
                  "QualityEst:MOSLQ=%u MOSCQ=%u RCQ=%u EXTRO=%u\n",
                  pMetrics->extCallStatsRem.uEstMOSLQ,
                  pMetrics->extCallStatsRem.uEstMOSCQ,
                  pMetrics->extCallStatsRem.uRFactor,
                  pMetrics->extCallStatsRem.uExtRFactor );
      }
   }

   return strlen( reinterpret_cast<char *> (puData));
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceSetBody
**
**  PURPOSE:    Assigns the body to be sent with this notification.
**
**  PARAMETERS:
**              pBody - pointer to the (formatted) message body.
**              uSize - size of the body.
**
**  RETURNS:    None.
**
**  NOTES:
***********************************************************************************/
void CeResNot::resourceSetBody( BOS_UINT8 *pBody, BOS_UINT32 uSize )
{
   mpBody = utilDataDup( pBody, uSize );

   if ( mpBody )
   {
      muSize = uSize;
   }
}


/***********************************************************************************
**  FUNCTION:   CeResNot::resourceShow
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
void CeResNot::resourceShow( void )
{
#if CCTKCFG_APPLICATION_STATUS
   /* Invoke base class first to display the generic information about
   ** this resource.
   */
   CeResBase::resourceShow();

   /* Display the class specific data associated with this resource.
   */
   CCTK_TRACEI2( "SceOod [%p], PubTgt [%s]",
                 mpSceOodReqSnd,
                 mpPubParty );

   CCTK_TRACEI2( "Type [%d], GUID [0x%lX]",
                 mePubType,
                 muCallGuid );

   CCTK_TRACEI1( "Resource [0x%lX] -- END.",
                 resourceGuid() );
#endif /* CCTKCFG_APPLICATION_STATUS */
}

