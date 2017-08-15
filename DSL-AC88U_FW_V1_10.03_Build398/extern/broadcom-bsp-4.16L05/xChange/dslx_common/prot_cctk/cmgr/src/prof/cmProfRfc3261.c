/**********************************************************************************
** Copyright (c) 2010-2013 Broadcom Corporation
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
**      This module implements the configuration interface and all configuration
**      related processings for the specific RFC 3261 profile.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_RFC3261)

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CMGR_T38_MAX_DATAGRAM_DEFAULT           160
#define CMGR_MAXPTIME_DEFAULT                   20

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */
#if CMGRCFG_MULTILINE
static void cmProfRfc3261MultilineUserAdd ( void *pRes );
#else
static void cmProfRfc3261DefaultUserAdd ( void *pRes );
#endif /* CMGRCFG_MULTILINE */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmProfRfc3261UserAdd
**
** PURPOSE:    Configure a specific user for a RFC 3261 application.
**
** PARAMETERS: pRes - the user resource to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the user has been allocated its user
**             identification already, which is necessary to configure a user
**             completely.
***********************************************************************************/
void cmProfRfc3261UserAdd ( void *pRes )
{
#if CMGRCFG_MULTILINE
   cmProfRfc3261MultilineUserAdd( pRes );
#else
   cmProfRfc3261DefaultUserAdd( pRes );
#endif /* CMGRCFG_MULTILINE */
}

#if !CMGRCFG_MULTILINE
/***********************************************************************************
** FUNCTION:   cmProfRfc3261DefaultUserAdd
**
** PURPOSE:    Configure a specific user for the default RFC 3261 application.
**
** PARAMETERS: pRes - the user resource to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the user has been allocated its user
**             identification already, which is necessary to configure a user
**             completely.
***********************************************************************************/
static void cmProfRfc3261DefaultUserAdd ( void *pRes )
{
   CFGPROVDEVICECFG *pDevCfg = NULL;
   CFGSIPUSERCFG *pUsr = NULL;
   PROV_CFG_IDX cfgIdx;
   CCTKMEDAUD cctkMedAud;
   CCTKMEDFAX cctkMedFax;
#if CMGRCFG_VIDEO_MEDIA
   CCTKMEDVID cctkMedVid;
#endif
   CCTKMEDLST cctkMedLst;
   BOS_BOOL bEnable = BOS_TRUE;
   CMDMSTATUS dmStatus = eCMDM_ERROR;
   BOS_UINT32 uIx;
   CCTKUSRCFG cctkUsr;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGSIPEMERGE *pEmergSvc = NULL;
   CFGSIPPCSCF *pPcscfUsr = NULL;
   CFGSIPDIGITMAP *pUsrDigMap = NULL;
   BOS_UINT8 *pcAt = NULL;
   BOS_UINT8 *puTmp = NULL;
   CFGSIPMWI *pUsrMwi = NULL;
   CFGSIPBLV *pUsrBlv = NULL;
   CFGSIPAUTOCALL *pUsrACall = NULL;
   CFGSIPSCF *pUsrScf = NULL;
   CFGSIPCALLFWD *pUsrCfv = NULL;
   CFGSIPDND *pUsrDnd = NULL;
   CFGSIPCALLXFER *pUsrXfer = NULL;
   CFGSIPACTSTAT *pActSta = NULL;
   CFGSIPUNSOLICITEDNOTIFY *pUsrUnSolNtfy = NULL;
   CCTKMWICFG mwiCfg;
   CCTKMWITMR mwiTmr;
   CCTKDIAGEVTCFG dlgCfg;
   CCTKUAPEVTCFG uapCfg;
   CCTKXFERCFG xferCfg;
   CCTKUSRPORTCFG portCfg;
   CCTKREGTMR regTmrCfg;
   CCTKREGEVTCFG regCfg;
   CCTKUSRBRCFG usrBrCfg;
   CCTKSESTMR usrSesTmrCfg;
   CCTKEVTTMR evtTmrCfg;
   CCTKTRANSTMRCFG cctkTmrCfg;
   BOS_BOOL bExist;
   BOS_BOOL bPcscfQueried = BOS_FALSE;
   BOS_UINT32 uKeepAliveMaxInterval = CM_CFG__KEEP_ALIVE_TIMER__FIXED;
   BOS_UINT32 uKeepAliveFailureThreshold = 0;
   BOS_BOOL bKeepAlive = BOS_TRUE;
   BOS_UINT16 uLocalPort = CM_CFG__KEEP_ALIVE_DEFAULT_PORT;
   BOS_UINT8 uIpAddr[ CMGR_IP_ADDRESS_STRING_LEN + 1 ];
   BOS_UINT8 uMwiIpAddr[ CMGR_IP_ADDRESS_STRING_LEN + 1 ];
   BOS_BOOL bSetSessionTmr = BOS_FALSE;
   BOS_UINT32 uSessionTmrExpires = 0;
   BOS_UINT32 uSessionTmrMin = 0;
   BOS_BOOL bNoSubscribe = BOS_FALSE;
   CMUSR *pCmUsr = (CMUSR *) pRes;

   if ( !pCmUsr )
   {
      return;
   }

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pCmUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) &pUsr,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pUsr )
      {
         /* Default settings setup.
         */
         memset ( &cctkUsr, 0, sizeof ( CCTKUSRCFG ) );

         mwiTmr.uExpTmr = 0;
         mwiTmr.uRetryTmr = CM_CFG__SUB_RETRY_TIMER;

         memset( &dlgCfg, 0, sizeof( CCTKDIAGEVTCFG ) );
         memset( &uapCfg, 0, sizeof( CCTKUAPEVTCFG ) );
         memset( &portCfg, 0, sizeof( CCTKUSRPORTCFG ) );
         memset( &xferCfg, 0, sizeof( CCTKXFERCFG ) );
         memset( &regCfg, 0, sizeof( CCTKREGEVTCFG ) );
         memset( &regTmrCfg, 0, sizeof( CCTKREGTMR ) );
         memset( &usrBrCfg, 0, sizeof( CCTKUSRBRCFG ) );
         memset( &usrSesTmrCfg, 0, sizeof( CCTKSESTMR ) );
         memset( &evtTmrCfg, 0, sizeof( CCTKEVTTMR ) );
         memset( &mwiCfg, 0, sizeof( CCTKMWICFG) );

         /* Default MWI event policy is to only notify the application
         ** if the message-summary has changed.
         */
         mwiCfg.mode.eEvtPolicy = eCCTKMWIEVT_NOTIFY_UPDATE;

         /* reg-event subscription is by default on unless configured
          * otherwise using the BasicCallConcept */
         regCfg.bRegEvtSupport = BOS_TRUE;

         /* At this time we are only creating 'line' users, which are users
         ** associated with an FXS interface on our VoIP gateway.
         */
         pCmUsr->eType = eCMUSRTYPE_LINE;

         /* Create the CCTK configuration for this user.
         */
         switch ( pUsr->sUserType )
         {
            case CFG_SIP_USER_TYPE_PUBLICIDENTITY:
            {
               BOS_UINT32 uLen = strlen ( (const char *)pUsr->pcUserId ) + 1;
               puTmp = (BOS_UINT8 *) malloc ( sizeof ( BOS_UINT8 ) * uLen );
               if ( puTmp )
               {
                  memset ( puTmp,
                           0,
                           sizeof ( BOS_UINT8 ) * uLen );

                  if ( !strCmpNoCaseSize( (const char *) pUsr->pcUserId,
                                          CMGR_SIP_URI,
                                          strlen( CMGR_SIP_URI ) ) )
                  {
                     memcpy ( puTmp,
                              pUsr->pcUserId + strlen( CMGR_SIP_URI ),
                              sizeof ( BOS_UINT8 ) * uLen );

                     if ( ( pcAt =
                              (BOS_UINT8 *) strchr ( (const char *)puTmp,
                                               CMGR_AOR_DELIMITER ) ) != NULL )
                     {
                        *pcAt = '\0';
                        cctkUsr.reg.gen.aor.pcAddr = pcAt + 1;
                        cctkUsr.reg.gen.aor.uPort = 0;
                     }
                  }
                  else if ( !strCmpNoCaseSize( (const char *) puTmp,
                                               CMGR_TEL_URI,
                                               strlen( CMGR_TEL_URI ) ) )
                  {
                     memcpy ( puTmp,
                              pUsr->pcUserId + strlen( CMGR_TEL_URI ),
                              sizeof ( BOS_UINT8 ) * uLen );
                  }
                  else
                  {
                     /* This is an invalid type format for the configured type
                     ** in such case, we just copy over the user data as is and
                     ** see what will happen.
                     */
                     memcpy ( puTmp,
                              pUsr->pcUserId,
                              sizeof ( BOS_UINT8 ) * uLen );
                  }

                  cctkUsr.usr.net.sipName.pcUsrName = puTmp;
               }
            }
            break;

            case CFG_SIP_USER_TYPE_USERNAME:
            {
               BOS_UINT32 uLen = strlen ( (const char *)pUsr->pcUserId ) + 1;
               puTmp = (BOS_UINT8 *) malloc ( sizeof ( BOS_UINT8 ) * uLen );
               if ( puTmp )
               {
                  memset ( puTmp,
                           0,
                           sizeof ( BOS_UINT8 ) * uLen );
                  memcpy ( puTmp,
                           pUsr->pcUserId,
                           sizeof ( BOS_UINT8 ) * uLen );

                  if ( ( pcAt = (BOS_UINT8 *) strchr ( (const char *)puTmp,
                                                       CMGR_AOR_DELIMITER ) ) != NULL )
                  {
                     *pcAt = '\0';
                     cctkUsr.reg.gen.aor.pcAddr = pcAt + 1;
                     cctkUsr.reg.gen.aor.uPort = 0;
                  }

                  cctkUsr.usr.net.sipName.pcUsrName = puTmp;
               }
            }
            break;

            default:
            {
               cctkUsr.usr.net.sipName.pcUsrName = pUsr->pcUserId;
            }
            break;
         }
         cctkUsr.usr.net.sipName.pcDisplay = pUsr->pcDisplayInfo;
         cctkUsr.usr.net.sipName.bUsrNameFromTempId = pUsr->bUserIdFromTempInfo;

         cctkUsr.usr.sec.auth.client.pcAuthName = pUsr->pcPrivId;
         cctkUsr.usr.sec.auth.client.pcPwd = pUsr->credentials.pCredVal;

         /* Enable security as required.
         */
         if ( pUsr->bSigSecurity )
         {
            cctkUsr.usr.net.bSecOnly = BOS_TRUE;
#if !CMGRCFG_TLS
            if ( gCmCfgCb.cmgrEvtCb )
            {
               ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_NO_SEC,
                                        pCmUsr->uHdl,
#if CMGRCFG_MULTILINE
                                        CMGR_INVALID_HANDLE,
                                        pCmUsr->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                        (void *) pCmUsr->uUid );
            }
#endif
         }

         cctkUsr.reg.gen.registrar.hostPort.pcAddr =
                                            pUsr->pDomainList->pcDomain;
         cctkUsr.reg.gen.registrar.hostPort.uPort = 0;
         if ( pUsr->bSigSecurity )
         {
            cctkUsr.reg.gen.registrar.bSec = BOS_TRUE;
         }

         /* Select the domain to use for this user.
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->pDomainList->pcDomain;
         cfgIdx.uIndex = pUsr->pDomainList->uDomainID;

         cctkUsr.usr.net.uDnsId = pUsr->pDomainList->uDomainID;

         /* Initialize all timers to zero to use default.
         */
         memset( (void *) &cctkTmrCfg,
                 0,
                 sizeof( cctkTmrCfg ) );

         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                    &cfgIdx,
                                    (void *) &pPcscfUsr,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            bPcscfQueried = BOS_TRUE;

            cctkUsr.usr.net.obProxyLst.eType = eCCTKROUTETYPE_PRELOADED;

            cctkUsr.usr.net.obProxyLst.route[0].bLr = BOS_TRUE;
            cctkUsr.usr.net.obProxyLst.route[0].addr.hostPort.uPort =
                                              (BOS_UINT16) pPcscfUsr->uSIPPort;
            if ( pUsr->bSigSecurity )
            {
               cctkUsr.usr.net.obProxyLst.route[0].addr.bSec = BOS_TRUE;
            }

            if ( pPcscfUsr->obProxyLst.obProxy[0].pcAddr && 
                 strlen( (char*)pPcscfUsr->obProxyLst.obProxy[0].pcAddr ) )
            {
               cctkUsr.usr.net.obProxyLst.route[0].addr.hostPort.pcAddr =
                                              pPcscfUsr->obProxyLst.obProxy[0].pcAddr;
               cctkUsr.usr.net.obProxyLst.route[0].addr.hostPort.uPort =
                                              (BOS_UINT16) pPcscfUsr->obProxyLst.obProxy[0].uPort;
            }

            /* Configure SIP transaction timers.  Note that those can be associated
            ** per domain (so per user potentially) but we can only support on set of
            ** timer value, so we in fact take the set for the first user/domain
            ** configured on the application.
            */
            cctkTmrCfg.uT1 = pPcscfUsr->uTimerT1;
            cctkTmrCfg.uT2 = pPcscfUsr->uTimerT2;
            cctkTmrCfg.uT4 = pPcscfUsr->uTimerT4;
            cctkTmrCfg.uD  = pPcscfUsr->uTimerTD;
            cctkTmrCfg.uB  = pPcscfUsr->uTimerTB;
            cctkTmrCfg.uH  = pPcscfUsr->uTimerTH;
            cctkTmrCfg.uF  = pPcscfUsr->uTimerTF;

            cctkSetParm ( CMGR_INVALID_HANDLE,
                          eCCTKPARM_EC_TRANSTMR,
                          (void *) &cctkTmrCfg );

         }


         /* Set the domain in use back to the configuration module such
         ** that further 'domain' dependent features can be properly identified.
         */
         /* Remember original active domain, so it can be later restored before pUser
         ** is deleted to avoid memory leak. */
         BOS_UINT8 *pcOrgActiveDomain = pUsr->pcActiveDomain;

         pUsr->pcActiveDomain = pUsr->pDomainList->pcDomain;
         pUsr->uActiveDomain = pUsr->pDomainList->uDomainID;

         /* Set the Registration Status back to UNKNOWN if previously set to
         ** FORBIDDEN state*/
         if (pUsr->sRegStatus == CFG_SIP_USER_REG_FORBIDDEN)
         {
            pUsr->sRegStatus = CFG_SIP_USER_REG_UNKNOWN;
         }

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCmUsr->uUid;
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pUsr,
                               CFG_ITEM_SET );
         /* Restored to original active domain, so it is deleted to avoid memory leak. */
         pUsr->pcActiveDomain = pcOrgActiveDomain;

         /* Default device configuration for this user.
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                    &cfgIdx,
                                    (void *) &pDevCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            memcpy ( &cctkUsr.dft.call.media.ipaddr,
                     &pDevCfg->IpAddress,
                     sizeof ( BOS_IP_ADDRESS ) );

            uLocalPort = (BOS_UINT16) pDevCfg->uSIPRecvPort;
            uKeepAliveMaxInterval = pDevCfg->uKeepAliveMaxInterval;
            uKeepAliveFailureThreshold = pDevCfg->uKeepAliveFailureThreshold;
            bKeepAlive = (pDevCfg->bDisableKeepAlive == BOS_TRUE) ? BOS_FALSE : BOS_TRUE;

            bSetSessionTmr = BOS_TRUE;
            uSessionTmrExpires = pDevCfg->uSessionExpiry;
            uSessionTmrMin = pDevCfg->uSessionMinimum;

            bNoSubscribe = pDevCfg->bDisableSubscribe;

            gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                  &cfgIdx,
                                  (void *) pDevCfg,
                                  CFG_ITEM_DEL );
            pDevCfg = NULL;
         }

         /* configure MWI subscription only if bNoSubscribe is not set */
         if ( !bNoSubscribe )
         {
            /* Setup Message Waiting Indication configuration if enabled.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCmUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, MWIConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrMwi,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrMwi && pUsrMwi->Act.bCfgStatus )
               {
                  if ( pUsrMwi->bMwiSrvSet )
                  {
                     if ( pUsrMwi->pcMwiSrvDomain )
                     {
                        cctkUsr.mwi.gen.server.sipHost.hostPort.pcAddr =
                                                       pUsrMwi->pcMwiSrvDomain;
                     }
                     else
                     {
                        bosIpAddrGetStr ( &pUsrMwi->mwiSrvAddr,
                                          (char *) uMwiIpAddr,
                                          CMGR_IP_ADDRESS_STRING_LEN );

                        cctkUsr.mwi.gen.server.sipHost.hostPort.pcAddr =
                                                               &uMwiIpAddr[0];
                     }
                     cctkUsr.mwi.gen.server.sipHost.hostPort.uPort =
                                                (BOS_UINT16) pUsrMwi->uSIPPort;
                  }
                  else
                  {
                     cctkUsr.mwi.gen.server.sipHost.hostPort.pcAddr =
                        cctkUsr.reg.gen.registrar.hostPort.pcAddr;
                     cctkUsr.mwi.gen.server.sipHost.hostPort.uPort = 0;
                  }
                  if ( pUsr->bSigSecurity )
                  {
                     cctkUsr.mwi.gen.server.sipHost.bSec = BOS_TRUE;
                  }

                  mwiTmr.uExpTmr = pUsrMwi->uSubDuration / CMGR_MSEC_2_SEC_DIVIDER;
               }
            }
         }

         /* Setup unsolicited notify configuration.
         ** Default is to accept unsolicited MWI notifications.
         */
         cctkUsr.mwi.gen.bUnsolNtfy = BOS_TRUE;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCmUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UnsolicitedMWIConcept ),
                                    &cfgIdx,
                                    (void *) &pUsrUnSolNtfy,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            /* Enable support for blind message waiting.
            */
            if ( pUsrUnSolNtfy && pUsrUnSolNtfy->Act.bCfgStatus )
            {
               cctkUsr.mwi.gen.bUnsolNtfy = BOS_TRUE;
            }
            /* Disable support for blind message waiting.
            */
            else
            {
               cctkUsr.mwi.gen.bUnsolNtfy = BOS_FALSE;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UnsolicitedMWIConcept ),
                                  &cfgIdx,
                                  (void *) pUsrUnSolNtfy,
                                  CFG_ITEM_DEL );
            pUsrUnSolNtfy = NULL;
         }

         cmCfgAudio ( (void *) pCmUsr, NULL, &cctkMedAud );
         cmCfgFax ( (void *) pCmUsr, &cctkMedFax, &bEnable );
#if CMGRCFG_VIDEO_MEDIA
         cmCfgVideo ( (void *) pCmUsr, &cctkMedVid );
#endif

         /* Setup ourselves to always use the audio media only (or with
         ** video media if supported) at first.  If a fax media (image)
         ** is needed, it would happen through a reconfiguration.
         */
         memset ( &cctkMedLst,
                  0,
                  sizeof( CCTKMEDLST ) );

         cctkMedLst.mMediaTypes = CCTK_MEDIA_AUDIO_MASK;
         cctkMedLst.uNumAudCodec = cctkMedAud.gen.uNumCodec;
         for ( uIx = 0 ; uIx < cctkMedLst.uNumAudCodec ; uIx++ )
         {
            cctkMedLst.audCodeLst[ uIx ] = cctkMedAud.gen.codec[ uIx ].eType;
         }
#if CMGRCFG_VIDEO_MEDIA
         cctkMedLst.mMediaTypes |= CCTK_MEDIA_VIDEO_MASK;
         cctkMedLst.uNumVidCodec = cctkMedVid.gen.uNumCodec;
         for ( uIx = 0 ; uIx < cctkMedLst.uNumVidCodec ; uIx++ )
         {
            cctkMedLst.vidCodeLst[ uIx ] = cctkMedVid.gen.codec[ uIx ].eType;
         }
#endif
         /* Associate the media configuration with this user.
         */
         cctkUsr.dft.call.media.pAudio = &cctkMedAud;
         cctkUsr.dft.call.media.pFax = ( bEnable ? &cctkMedFax : NULL );
#if CMGRCFG_VIDEO_MEDIA
         cctkUsr.dft.call.media.pVideo = &cctkMedVid;
#endif
         cctkUsr.dft.call.media.pMediaLst = &cctkMedLst;

         if ( pPcscfUsr && pPcscfUsr->bBackupRecovery )
         {
            /* Using backup and recovery feature.  This is an internal feature of
            ** the CCTK layer which will take care of selecting the target (or an
            ** alternate) for a given request based on internal knowledge.  It also
            ** offers recovery process which allows to return to a previously known
            ** 'good' target following a loss of connectivity to that target.
            */
            CMGRLOG_INFORMATION ((
                        "cmProfRfc3261DefaultUserAdd: %s - usr 0x%X, uses backup-recovery",
                        pCmUsr->uUid, pCmUsr->uHdl ));
         }

#if CCTKCFG_REMOTE_USER_CONTROL
         /* When supporting remote user, one cannot have an outbound keep alive
         ** configuration because it would prevent any remote user to target the
         ** device for signaling hence would render the intended feature useless.
         */
         CMGRLOG_INFORMATION ((
            "cmProfRfc3261DefaultUserAdd: %s - usr 0x%X, no outbound - remote user support",
            pCmUsr->uUid, pCmUsr->uHdl ));
#else
         /* Setup outbound keep alive configuration.  The outbound keep alive
         ** configuration ensures that we are always accessing the same target
         ** as long as it is alive, this is a requirement of IMS.  Note that it
         ** does not necessarely mean that we are running keep alive signaling
         ** on the link.
         */

         /* If keepalive is enabled */
         if (bKeepAlive )
         {
            /* If the selected keep alive interval was set to an unreasonably
            ** low value,  we will ignore this value and revert to using the
            ** default */
            if ( uKeepAliveMaxInterval < CM_CFG__KEEP_ALIVE_TIMER__VARMOD )
            {
               cctkUsr.usr.sec.nat.kaCfg.uTmr = CM_CFG__KEEP_ALIVE_TIMER__FIXED +
                           1000 * (rand() % CM_CFG__KEEP_ALIVE_TIMER__VARMOD);
            }
            else
            {
               cctkUsr.usr.sec.nat.kaCfg.uTmr = uKeepAliveMaxInterval -
                           1000 * (rand() % CM_CFG__KEEP_ALIVE_TIMER__VARMOD);
            }

            /* Set the maximum number of transmissions before keep alive
            ** is considered failed */
            cctkUsr.usr.sec.nat.kaCfg.uFailureThreshold =
                        uKeepAliveFailureThreshold;

            cctkUsr.usr.sec.nat.kaCfg.server.hostPort.pcAddr =
                        cctkUsr.usr.net.obProxyLst.route[0].addr.hostPort.pcAddr;
            cctkUsr.usr.sec.nat.kaCfg.server.hostPort.uPort =
                        cctkUsr.usr.net.obProxyLst.route[0].addr.hostPort.uPort;
            cctkUsr.usr.sec.nat.kaCfg.server.bSec = cctkUsr.reg.gen.registrar.bSec;
            cctkUsr.usr.sec.nat.kaCfg.uSrcPort = uLocalPort;
            cctkUsr.usr.sec.nat.kaCfg.bUsePersistCnx = BOS_FALSE;
         }
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

         /* Create the CCTK user.
         */
         if ( cctkUserAdd ( &pCmUsr->uNet, &cctkUsr ) == eCCTKSTATUS_SUCCESS )
         {
#if CMGRCFG_VIDEO_MEDIA
            /* Clean up following configuration assignement.
            */
            for ( uIx = 0 ; uIx < cctkMedVid.gen.uNumCodec ; uIx++ )
            {
               if ( (cctkMedVid.gen.codec[ uIx ].eType == eCCTKCODEC_H264) &&
                    (cctkMedVid.gen.codec[ uIx ].parm.pH264 != NULL) )
               {
                  free ( cctkMedVid.gen.codec[ uIx ].parm.pH264 );
                  cctkMedVid.gen.codec[ uIx ].parm.pH264 = NULL;
               }
            }
#endif
            /* Setup registration timeout configuration for that user.
            */
            if ( cctkGetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_REGTMR,
                          (void *) &regTmrCfg ) == eCCTKSTATUS_SUCCESS)
            {

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCmUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, StatusChangeConcept ),
                                       &cfgIdx,
                                       (void *) &pActSta,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pActSta && pActSta->Act.bCfgStatus )
               {
                  regTmrCfg.uRegExpTmr =
                        (BOS_UINT32) pActSta->uChgRegExp / CMGR_MSEC_2_SEC_DIVIDER;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, StatusChangeConcept ),
                                     &cfgIdx,
                                     (void *) pActSta,
                                     CFG_ITEM_DEL );
               pActSta = NULL;
            }

            /* Setup subscription and registration recovery timeouts
            **/
            if ( bPcscfQueried && pPcscfUsr )
            {
               regTmrCfg.uSubRetryTmr =
                        pPcscfUsr->uSubscribeRetry / CMGR_MSEC_2_SEC_DIVIDER;

               if ( bKeepAlive )
               {
                  regTmrCfg.uFlowMaxTmr =
                           pPcscfUsr->uMaxTime / CMGR_MSEC_2_SEC_DIVIDER;
                  regTmrCfg.uFlowBaseOkTmr =
                           pPcscfUsr->uBaseTimeAllNotFailed / CMGR_MSEC_2_SEC_DIVIDER;
                  regTmrCfg.uFlowBaseNokTmr =
                           pPcscfUsr->uBaseTimeAllFailed / CMGR_MSEC_2_SEC_DIVIDER;
               }

               regTmrCfg.uRegRetryTmrOn403 = pPcscfUsr->uRegRetryTimerOn403;
            }

            cctkSetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_REGTMR,
                          (const void *) &regTmrCfg );
            }

            /* Setup registration event package configuration.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCmUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pBasicCall && pBasicCall->Act.bCfgStatus )
               {
                  regCfg.bMapRejToDeact = pBasicCall->bMapRegEvtRej2Deact;
                  regCfg.bRegEvtSupport = !pBasicCall->bDisableRegEvtSub;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_DEL );
               pBasicCall = NULL;
            }

            cctkSetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_REGEVTCFG,
                          (const void *) &regCfg );

            /* Setup subscription configuration for other events
            */
            if ( cctkGetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_EVTTMR,
                          (void *) &evtTmrCfg ) == eCCTKSTATUS_SUCCESS)
            {

               if ( bPcscfQueried && pPcscfUsr )
               {
                  evtTmrCfg.uRetryTmr = pPcscfUsr->uSubscribeRetry / CMGR_MSEC_2_SEC_DIVIDER;
               }

               cctkSetParm ( pCmUsr->uNet,
                             eCCTKPARM_EC_EVTTMR,
                             (const void *) &evtTmrCfg );
            }

            /* Setup TOS configuration for that user.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCmUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pBasicCall && pBasicCall->Act.bCfgStatus )
               {
                  portCfg.tosCfg.uDefault =
                                 (BOS_UINT8) pBasicCall->uNEMDSCPValueSig;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_DEL );
               pBasicCall = NULL;
            }

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCmUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                       &cfgIdx,
                                       (void *) &pEmergSvc,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pEmergSvc && pEmergSvc->Act.bCfgStatus )
               {
                  portCfg.tosCfg.uEmgCall =
                                    (BOS_UINT8) pEmergSvc->uSigDSCPVal;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                     &cfgIdx,
                                     (void *) pEmergSvc,
                                     CFG_ITEM_DEL );
               pEmergSvc = NULL;
            }

            cctkSetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_USRPORTCFG,
                          (const void *) &portCfg );

            /* Setup MWI configuration.
            */
            if ( bPcscfQueried && pPcscfUsr )
            {
               /* Apply the subscribe retry timeout for MWI subscriptions
               */
               mwiTmr.uRetryTmr = pPcscfUsr->uSubscribeRetry / CMGR_MSEC_2_SEC_DIVIDER;
            }

            cctkSetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_MWICFG,
                          (const void *) &mwiCfg );

            cctkSetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_MWITMR,
                          (const void *) &mwiTmr );

            /* Setup transfer configuration.
            */
            if (cctkGetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_CALLXFER,
                          (void *) &xferCfg ) != eCCTKSTATUS_SUCCESS)
            {
               xferCfg.tfererCfg.bEndOnNotify = BOS_FALSE;
               xferCfg.tfererCfg.bUseOodRefer = BOS_TRUE;
               xferCfg.tfererCfg.bOodRefFallBack = BOS_FALSE;
               xferCfg.tfererCfg.bCleanOnFail = BOS_TRUE;
            }
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCmUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrXfer,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrXfer && pUsrXfer->Act.bCfgStatus )
               {
                  xferCfg.tfererCfg.uNotifyWaitTime = pUsrXfer->uNotifyTO;
                  xferCfg.tfererCfg.bUseOodRefer = (pUsrXfer->bInDialogRefer == BOS_TRUE) ? BOS_FALSE : BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                     &cfgIdx,
                                     (void *) pUsrXfer,
                                     CFG_ITEM_DEL );
               pUsrXfer = NULL;
            }

            cctkSetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_CALLXFER,
                          (const void *) &xferCfg );

            /* Setup dialog event configuration.
            */
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BLVConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrBlv,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrBlv &&
                    pUsrBlv->Act.bCfgStatus &&
                    pUsrBlv->pcOperId )
               {
                  dlgCfg.pcSubscriber = pUsrBlv->pcOperId;
                  /* Allow at least one subscriber to the dialog event
                  ** package, this is for the operator.
                  */
                  dlgCfg.uSubeeMax = 1;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BLVConcept ),
                                     &cfgIdx,
                                     (void *) pUsrBlv,
                                     CFG_ITEM_DEL );
               pUsrBlv = NULL;
            }

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrACall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrACall && pUsrACall->Act.bCfgStatus )
               {
                  dlgCfg.uSuberMax += pUsrACall->uMaxSubSend;
                  dlgCfg.uSubeeMax += pUsrACall->uMaxSubRec;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                     &cfgIdx,
                                     (void *) pUsrACall,
                                     CFG_ITEM_DEL );
               pUsrACall = NULL;
            }

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrACall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrACall && pUsrACall->Act.bCfgStatus )
               {
                  dlgCfg.uSuberMax += pUsrACall->uMaxSubSend;
                  dlgCfg.uSubeeMax += pUsrACall->uMaxSubRec;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                     &cfgIdx,
                                     (void *) pUsrACall,
                                     CFG_ITEM_DEL );
               pUsrACall = NULL;
            }

            cctkSetParm ( pCmUsr->uNet,
                          eCCTKPARM_EC_DIAGEVTCFG,
                          (const void *) &dlgCfg );

            /* Only configure the ua-profile subscription if bNoSubscribe is not set */
            if (!bNoSubscribe)
            {
               /* Setup User-Agent Profile event configuration.
               **
               ** Note on this configuration.  We need to get the information from
               ** the user-agent profile for those features which status is kept
               ** outside of our internal knowledge and for which we need to act
               ** upon accordingly.  There are three features that are in this
               ** category: do-not-disturb, selective-call-forwarding and
               ** call-forwarding-variable.
               **
               ** Each of these features may be managed by a different application
               ** server, which means we may have to be ready to subscribe to up to
               ** three different servers to get the full picture of the feature
               ** status.  To how many application server(s) we will subscribe in the
               ** end will be determined by the configuration, which we are now
               ** analyzing.
               */

               /* Get the default configuration and update as appropriate */
               if ( cctkGetParm ( pCmUsr->uNet,
                                  eCCTKPARM_EC_UAPEVTCFG,
                                  (void *) &uapCfg ) == eCCTKSTATUS_SUCCESS )
               {
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DNDConcept ),
                                             &cfgIdx,
                                             (void *) &pUsrDnd,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pUsrDnd &&
                          pUsrDnd->Act.bCfgStatus &&
                          pUsrDnd->pcAUID &&
                          strlen( (const char *)pUsrDnd->pcAUID ) )
                     {
                        uapCfg.pcProfile[ uapCfg.uNum ] = pUsrDnd->pcAUID;
                        uapCfg.uNum++;
                     }
                  }

                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, SCFConcept ),
                                             &cfgIdx,
                                             (void *) &pUsrScf,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pUsrScf &&
                          pUsrScf->Act.bCfgStatus &&
                          pUsrScf->pcAUID &&
                          strlen( (const char *)pUsrScf->pcAUID ) )
                     {
                        /* Only add this configuration if we have not set any yet or
                        ** if it is different value than the one we may have already
                        ** setup.
                        */
                        bExist = BOS_FALSE;
                        for ( uIx = 0 ; uIx < uapCfg.uNum ; uIx++ )
                        {
                           if ( uapCfg.pcProfile[ uIx ] &&
                                !strCmpNoCase( (const char *)uapCfg.pcProfile[ uIx ],
                                               (const char *)pUsrScf->pcAUID ) )
                           {
                              bExist = BOS_TRUE;
                           }
                        }

                        if ( !uapCfg.uNum || !bExist )
                        {
                           uapCfg.pcProfile[ uapCfg.uNum ] = pUsrScf->pcAUID;
                           uapCfg.uNum++;
                        }
                     }
                  }

                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                             &cfgIdx,
                                             (void *) &pUsrCfv,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pUsrCfv &&
                          pUsrCfv->Act.bCfgStatus &&
                          pUsrCfv->pcAUID &&
                          strlen( (const char *)pUsrCfv->pcAUID ) )
                     {
                        /* Only add this configuration if we have not set any yet or
                        ** if it is different value than the one we may have already
                        ** setup.
                        */
                        bExist = BOS_FALSE;
                        for ( uIx = 0 ; uIx < uapCfg.uNum ; uIx++ )
                        {
                           if ( uapCfg.pcProfile[ uIx ] &&
                                !strCmpNoCase( (const char *)uapCfg.pcProfile[ uIx ],
                                               (const char *)pUsrCfv->pcAUID ) )
                           {
                              bExist = BOS_TRUE;
                           }
                        }

                        if ( !uapCfg.uNum || !bExist )
                        {
                           uapCfg.pcProfile[ uapCfg.uNum ] = pUsrCfv->pcAUID;
                           uapCfg.uNum++;
                        }
                     }
                  }
               }  /* end of !bNoSubscribe */

               /* Set up unsolicited UA-Profile Notify configuration */
               pUsrUnSolNtfy = NULL;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UnsolicitedUAProfileConcept ),
                                          &cfgIdx,
                                          (void *) &pUsrUnSolNtfy,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  /* Enable support for unsolicited ua-profile notifications.
                  */
                  if ( pUsrUnSolNtfy && pUsrUnSolNtfy->Act.bCfgStatus )
                  {
                     uapCfg.bUnsolNtfy = BOS_TRUE;
                  }
                  /* Disable support for unsolicited ua-profile notifications.
                  */
                  else
                  {
                     uapCfg.bUnsolNtfy = BOS_FALSE;
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UnsolicitedUAProfileConcept ),
                                        &cfgIdx,
                                        (void *) pUsrUnSolNtfy,
                                        CFG_ITEM_DEL );
                  pUsrUnSolNtfy = NULL;
               }

               cctkSetParm ( pCmUsr->uNet,
                             eCCTKPARM_EC_UAPEVTCFG,
                            (const void *) &uapCfg );
            }

            /* Setup backup and recovery configuration.
            */
            if ( cctkGetParm ( pCmUsr->uNet,
                               eCCTKPARM_EC_USRBRCFG,
                               (void *) &usrBrCfg ) == eCCTKSTATUS_SUCCESS )
            {
               /* Update backup and recovery timer value as apporpriate.
               */
               usrBrCfg.uBackoffMinTmr = pUsr->uRegRetryTimerMin / 1000;
               usrBrCfg.uBackoffMaxTmr = pUsr->uRegRetryTimerMax / 1000;

               /* Update BrCfg flags as appropriate.
               ** bFailover is used to control the failover feature in cctk.
               */
               if ( bPcscfQueried && pPcscfUsr )
               {
                  /* The bFailover flag should only be enabled if
                  ** the failover feature itself is not disabled.
                  */
                  usrBrCfg.bFailover = ( usrBrCfg.eMode != eCCTKBRSVCMODE_DISABLED ) ?
                                          pPcscfUsr->bBackupRecovery : BOS_FALSE;

                  /* Update the backup and recovery timer as per the backoff mode.
                  ** If in exponential outbound mode the backoff timers must
                  ** be set to the flow recovery timer values.
                  */
                  if( usrBrCfg.bOutboundBackoff )
                  {
                     usrBrCfg.uBackoffMinTmr = pPcscfUsr->uBaseTimeAllFailed / CMGR_MSEC_2_SEC_DIVIDER;
                     usrBrCfg.uBackoffMaxTmr = pPcscfUsr->uMaxTime / CMGR_MSEC_2_SEC_DIVIDER;
                  }

                  usrBrCfg.uBackToPrimMinTmr = pPcscfUsr->uPrimaryRecoveryTimerMin;
                  usrBrCfg.uBackToPrimMaxTmr = pPcscfUsr->uPrimaryRecoveryTimerMax;

                  if ( !pPcscfUsr->bBackupRecovery )
                  {
                     usrBrCfg.eBackToPrimMode = eCCTKBRBACKTOPRIMMODE_DISABLED;
                     usrBrCfg.bEnableInbdReg = BOS_FALSE;
                  }
                  /* If failover is enabled and is in external IP-list mode
                  ** build the target list from the outbound proxy list.
                  */
                  else if ( usrBrCfg.eMode == eCCTKBRSVCMODE_EXTLIST )
                  {
                     BOS_IP_ADDRESS obIpAddr;
                     BOS_UINT32 uJx = 0;
                     
                     memset( &usrBrCfg.extList, 0, sizeof( CCTKIPPORTLST ) );

                     for ( uIx = 0; uIx < pPcscfUsr->obProxyLst.uNum; uIx++ )
                     {
                        /* If there is a outbound proxy ip-address configured
                        ** then it can be added to the external list.
                        */
                        if ( ( pPcscfUsr->obProxyLst.obProxy[uIx].pcAddr &&
                                 strlen( (char*)pPcscfUsr->obProxyLst.obProxy[uIx].pcAddr ) ) &&
                             ( bosIpAddrCreateFromStr(
                                 (char*)pPcscfUsr->obProxyLst.obProxy[uIx].pcAddr, &obIpAddr ) == BOS_STATUS_OK )  &&
                             ( !bosIpAddrIsZero(&obIpAddr) ) )
                        {
                           usrBrCfg.extList.ipport[usrBrCfg.extList.uNum].addr = obIpAddr;
                           usrBrCfg.extList.ipport[usrBrCfg.extList.uNum].uPort =
                                                   pPcscfUsr->obProxyLst.obProxy[uIx].uPort;
                           usrBrCfg.extList.uNum++;
                        }
                        else
                        {
                           CMGRLOG_CRITICAL (( "cmProfImsDefaultUserAdd: Failed to configure recovery ip list!" ));
                        }

                        if ( usrBrCfg.extList.uNum == CCTKCFG_MAX_ADDRLST )
                        {
                           /* Backup and recovery external list is full.
                           */
                           break;
                        }
                     }
                  }
               }

               cctkSetParm ( pCmUsr->uNet,
                             eCCTKPARM_EC_USRBRCFG,
                             (const void *) &usrBrCfg );
            }

            /* Setup session timer configuration.
            */
            if ( bSetSessionTmr &&
                 ( cctkGetParm ( pCmUsr->uNet,
                               eCCTKPARM_EC_SESTMR,
                               (void *) &usrSesTmrCfg ) == eCCTKSTATUS_SUCCESS ) )
            {
               usrSesTmrCfg.uTmr = uSessionTmrExpires;
               usrSesTmrCfg.uMinSe = uSessionTmrMin;

               cctkSetParm ( pCmUsr->uNet,
                             eCCTKPARM_EC_SESTMR,
                             (const void *) &usrSesTmrCfg );
            }

            /* Clean up configuration.
            */
            if ( pUsrDnd )
            {
               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DNDConcept ),
                                     &cfgIdx,
                                     (void *) pUsrDnd,
                                     CFG_ITEM_DEL );
               pUsrDnd = NULL;
            }

            if ( pUsrScf )
            {
               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, SCFConcept ),
                                     &cfgIdx,
                                     (void *) pUsrScf,
                                     CFG_ITEM_DEL );
               pUsrScf = NULL;
            }

            if ( pUsrCfv )
            {
               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                     &cfgIdx,
                                     (void *) pUsrCfv,
                                     CFG_ITEM_DEL );
               pUsrCfv = NULL;
            }
         }

         if ( puTmp )
         {
            free ( puTmp );
            puTmp = NULL;
         }

         /* Continue to setup the user with the local information used only
         ** by Call Manager processing such as: digit collection and manipulation,
         ** feature setup, etc...
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCmUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                    &cfgIdx,
                                    (void *) &pUsrDigMap,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            dmStatus = cmDmCompile ( pCmUsr->uHdl,
                                     pUsrDigMap->pABNF,
                                     (CMDMDIGITMAP *) pCmUsr->pDmDigMap,
                                     CMDM_MAP_BUFFER_SIZE );

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                  &cfgIdx,
                                  (void *) pUsrDigMap,
                                  CFG_ITEM_DEL );
            pUsrDigMap = NULL;
         }
         else
         {
            CMGRLOG_CRITICAL (( "cmProfRfc3261DefaultUserAdd: %s - failed to get digit map!",
                                pCmUsr->uUid ));
         }

         CMGRLOG_INFORMATION (( "cmProfRfc3261DefaultUserAdd: %s - usr 0x%X, net 0x%X, dm %d",
                                pCmUsr->uUid, pCmUsr->uHdl,
                                pCmUsr->uNet, dmStatus ));

         if ( bPcscfQueried )
         {
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->pDomainList->pcDomain;
            cfgIdx.uIndex = pUsr->pDomainList->uDomainID;

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                  &cfgIdx,
                                  (void *) pPcscfUsr,
                                  CFG_ITEM_DEL );
            pPcscfUsr = NULL;
         }

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCmUsr->uUid;

         if ( pUsrMwi != NULL )
         {
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, MWIConcept ),
                         &cfgIdx,
                         (void *) pUsrMwi,
                         CFG_ITEM_DEL );
            pUsrMwi = NULL;
         }
      }

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pCmUsr->uUid;

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                            &cfgIdx,
                            (void *) pUsr,
                            CFG_ITEM_DEL );
      pUsr = NULL;
   }
}
#endif /* !CMGRCFG_MULTILINE */

#if CMGRCFG_MULTILINE
/***********************************************************************************
** FUNCTION:   cmProfRfc3261MultilineUserAdd
**
** PURPOSE:    Configure a specific user for a multiline enabled RFC 3261 application.
**
** PARAMETERS: pRes - the user resource to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the user has been allocated its user
**             identification already, which is necessary to configure a user
**             completely.
***********************************************************************************/
static void cmProfRfc3261MultilineUserAdd ( void *pRes )
{
   CMUSR *pCmUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   CFGEPTLINECFG *pEptLineCfg = NULL;

   if ( !pCmUsr )
   {
      CMGRLOG_CRITICAL (( "cmProfRfc3261MultilineUserAdd: pRes is NULL!" ));
      return;
   }

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.uIndex = pCmUsr->uPhys;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, LineConcept ),
                              &cfgIdx,
                              (void *) &pEptLineCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      pCmUsr->eType = ( (pEptLineCfg->eTermType == CFG_SIP_USER_LINE) ?
                         eCMUSRTYPE_PSTN : eCMUSRTYPE_LINE );
      pCmUsr->eAlarmCfgState = pEptLineCfg->sAnalogAlarmCfgState;

      gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, LineConcept ),
                                    &cfgIdx,
                            (void *) pEptLineCfg,
                                  CFG_ITEM_DEL );
      pEptLineCfg = NULL;
         }
         else
         {
      /* if LineConcept failed to be retrived, provide default for eAlarmCfgState */
      pCmUsr->eAlarmCfgState = CMALGALRMCFGSTATE_NONE;
         }

   /* Mark the line as in service for now */
   pCmUsr->bInSrv = BOS_TRUE;

   /* TODO - need to add digitmap support for the cmgr user */

}
#endif /* CMGRCFG_MULTILINE */


/***********************************************************************************
** FUNCTION:   cmProfRfc3261CctkAccntAdd
**
** PURPOSE:    Configure a specific CCTK account for a multiline and RFC3261
**             application.
**
** PARAMETERS: pRes - the user account to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the account has been allocated its user
**             identification already, which is necessary to configure the account
**             completely.
***********************************************************************************/
BOS_BOOL cmProfRfc3261CctkAccntAdd ( void *pRes )
{
   CFGPROVDEVICECFG *pDevCfg = NULL;
   CFGSIPUSERCFG *pUsr = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT32 uIx;
   CCTKMEDAUD cctkMedAud;
   CCTKMEDLST cctkMedLst;
   CCTKMEDFAX cctkMedFax;
   CFGEPTCODECCFG *pCodecCfg = NULL;
   CFGEPTCODECLIST *pCodecLstCfg = NULL;
   BOS_BOOL bEnable = BOS_TRUE;
   CCTKUSRCFG cctkUsr;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGSIPEMERGE *pEmergSvc = NULL;
   CFGSIPPCSCF *pPcscfUsr = NULL;
   CFGSIPMWI *pUsrMwi = NULL;
   CFGSIPBLV *pUsrBlv = NULL;
   CFGSIPAUTOCALL *pUsrACall = NULL;
   CFGSIPSCF *pUsrScf = NULL;
   CFGSIPCALLFWD *pUsrCfv = NULL;
   CFGSIPDND *pUsrDnd = NULL;
   CFGSIPCALLXFER *pUsrXfer = NULL;
   CFGSIPACTSTAT *pActSta = NULL;
   CFGSIPUNSOLICITEDNOTIFY *pUsrUnSolNtfy = NULL;
   CCTKMWICFG mwiCfg;
   CCTKMWITMR mwiTmr;
   CCTKDIAGEVTCFG dlgCfg;
   CCTKUAPEVTCFG uapCfg;
   CCTKXFERCFG xferCfg;
   CCTKUSRPORTCFG portCfg;
   CCTKREGTMR regTmrCfg;
   CCTKUSRBRCFG usrBrCfg;
   CCTKSESTMR usrSesTmrCfg;
   BOS_BOOL bExist;
   BOS_BOOL bPcscfQueried = BOS_FALSE;
   BOS_BOOL bKeepAliveDisabled = BOS_FALSE;
   BOS_UINT16 uLocalPort = CM_CFG__KEEP_ALIVE_DEFAULT_PORT;
   BOS_UINT8 uIpAddr[ CMGR_IP_ADDRESS_STRING_LEN + 1];
   BOS_UINT8 uMwiIpAddr[ CMGR_IP_ADDRESS_STRING_LEN + 1];
   BOS_BOOL bSetSessionTmr = BOS_FALSE;
   BOS_UINT32 uSessionTmrExpires = 0;
   BOS_UINT32 uSessionTmrMin = 0;
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   char* puTemp = NULL;
   char* pAt = NULL;
   BOS_UINT8 localIp[ CMGR_IP_ADDRESS_STRING_LEN + 1];
   CFGSIPDIGITMAP *pUsrDigMap = NULL;
   BOS_BOOL bNoSubscribe = BOS_FALSE;

   if ( !pCctkAccnt )
   {
      return BOS_FALSE;
   }

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pCctkAccnt->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) &pUsr,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pUsr )
      {
         /* Default settings setup.
         */
         memset ( &cctkUsr, 0, sizeof ( CCTKUSRCFG ) );

         mwiTmr.uExpTmr = 0;
         mwiTmr.uRetryTmr = CM_CFG__SUB_RETRY_TIMER;

         memset( &dlgCfg, 0, sizeof( CCTKDIAGEVTCFG ) );
         memset( &uapCfg, 0, sizeof( CCTKUAPEVTCFG ) );
         memset( &portCfg, 0, sizeof( CCTKUSRPORTCFG ) );
         memset( &xferCfg, 0, sizeof( CCTKXFERCFG ) );
         memset( &regTmrCfg, 0, sizeof( CCTKREGTMR ) );
         memset( &usrBrCfg, 0, sizeof( CCTKUSRBRCFG ) );
         memset( &usrSesTmrCfg, 0, sizeof( CCTKSESTMR ) );

         xferCfg.tfererCfg.bEndOnNotify = BOS_TRUE;
         xferCfg.tfererCfg.bUseOodRefer = BOS_FALSE;
         xferCfg.tfererCfg.bOodRefFallBack = BOS_FALSE;
         xferCfg.tfererCfg.bCleanOnFail = BOS_TRUE;

         /* Default MWI event policy is to only notify the application
         ** if the message-summary has changed.
         */
         mwiCfg.mode.eEvtPolicy = eCCTKMWIEVT_NOTIFY_UPDATE;

         /* Create the CCTK configuration for this user.
         */
         puTemp = strdup( (char*)pUsr->pcUserId );
         if( strchr( puTemp, '@' ) != NULL )
         {
            pAt = strchr( puTemp, '@' );
            *pAt = '\0';
            pAt++; /* pAt points to the domain portion */
         }

         /* (step 0) Set username, displayname, authname and password */
         cctkUsr.usr.net.sipName.pcUsrName = (BOS_UINT8*)puTemp;
         cctkUsr.usr.net.sipName.pcDisplay = pUsr->pcDisplayInfo;
         cctkUsr.usr.sec.auth.client.pcAuthName = pUsr->pcPrivId;
         cctkUsr.usr.sec.auth.client.pcPwd = pUsr->credentials.pCredVal;

         /* Enable security as required.
         */
         if ( pUsr->bSigSecurity )
         {
            cctkUsr.usr.net.bSecOnly = BOS_TRUE;
#if !CMGRCFG_TLS
            if ( gCmCfgCb.cmgrEvtCb )
            {
               ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_NO_SEC,
                                        pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                                        CMGR_INVALID_HANDLE,
                                        pCctkAccnt->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                        (void *) pCctkAccnt->uUid );
            }
#endif
         }

         /* Default media configuration for this user.
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                    &cfgIdx,
                                    (void *) &pDevCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            memcpy ( &cctkUsr.dft.call.media.ipaddr,
                     &pDevCfg->IpAddress,
                     sizeof ( BOS_IP_ADDRESS ) );

            bosIpAddrGetStr( &pDevCfg->IpAddress,
                             (char*)localIp,
                             CMGR_IP_ADDRESS_STRING_LEN );

            uLocalPort = (BOS_UINT16) pDevCfg->uSIPRecvPort;
            bKeepAliveDisabled = pDevCfg->bDisableKeepAlive;

            bSetSessionTmr = BOS_TRUE;
            uSessionTmrExpires = pDevCfg->uSessionExpiry;
            uSessionTmrMin = pDevCfg->uSessionMinimum;

            bNoSubscribe = pDevCfg->bDisableSubscribe;

            gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                  &cfgIdx,
                                  (void *) pDevCfg,
                                  CFG_ITEM_DEL );
            pDevCfg = NULL;
         }

         /* Select the domain to use for this user.
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );

         if ( pUsr->pDomainList )
         {
            cfgIdx.pcCharIdx = pUsr->pDomainList->pcDomain;
            cfgIdx.uIndex = pUsr->pDomainList->uDomainID;
            cctkUsr.usr.net.uDnsId = pUsr->pDomainList->uDomainID;
         }
         else if (pAt)
         {
            cfgIdx.pcCharIdx = (BOS_UINT8*)pAt;
            /* TODO? Set the DNS id to the cctkAccount handle */
            cctkUsr.usr.net.uDnsId = pCctkAccnt->uHdl;
         }


         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                    &cfgIdx,
                                    (void *) &pPcscfUsr,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            bPcscfQueried = BOS_TRUE;

            /* (step 1) Set registrar: Registrar address is either the pcscfDomain or pcscfAddr */
            if ( pPcscfUsr->pcPCSCFDomain && strlen( (char*)pPcscfUsr->pcPCSCFDomain ) )
            {
               cctkUsr.reg.gen.registrar.hostPort.pcAddr = pPcscfUsr->pcPCSCFDomain;
            }
            else
            {
               if( bosIpAddrIsZero(&pPcscfUsr->PCSCFAddr) )
               {
                  cctkUsr.reg.gen.registrar.hostPort.pcAddr = NULL;
               }
               else
               {
                  bosIpAddrGetStr ( &pPcscfUsr->PCSCFAddr,
                                 (char *) uIpAddr,
                                 CMGR_IP_ADDRESS_STRING_LEN );

                  cctkUsr.reg.gen.registrar.hostPort.pcAddr = &uIpAddr[0];
               }
            }

            /* (step 2) Set AOR: The domain part of the pcUserId is our address-of-record. If empty then use local IP */
            if( pAt )
            {
               cctkUsr.reg.gen.aor.pcAddr = (BOS_UINT8*)pAt;
            }
            else
            {
               cctkUsr.reg.gen.aor.pcAddr = &localIp[0];
            }

            cctkUsr.reg.gen.registrar.hostPort.uPort =
                                       (BOS_UINT16) pPcscfUsr->uSIPPort;
            if ( pUsr->bSigSecurity )
            {
               cctkUsr.reg.gen.registrar.bSec = BOS_TRUE;
            }

            /* (step 3) Setup Outbound Proxy */
            cctkUsr.usr.net.obProxyLst.eType = eCCTKROUTETYPE_PRELOADED;

            cctkUsr.usr.net.obProxyLst.route[0].bLr = BOS_TRUE;
            cctkUsr.usr.net.obProxyLst.route[0].addr.hostPort.uPort =
                                              (BOS_UINT16) pPcscfUsr->uSIPPort;
            if ( pUsr->bSigSecurity )
            {
               cctkUsr.usr.net.obProxyLst.route[0].addr.bSec = BOS_TRUE;
            }

            if ( pPcscfUsr->obProxyLst.obProxy[0].pcAddr && 
                 strlen( (char*)pPcscfUsr->obProxyLst.obProxy[0].pcAddr ) )
            {
               cctkUsr.usr.net.obProxyLst.route[0].addr.hostPort.pcAddr =
                                              pPcscfUsr->obProxyLst.obProxy[0].pcAddr;
               cctkUsr.usr.net.obProxyLst.route[0].addr.hostPort.uPort =
                                              (BOS_UINT16) pPcscfUsr->obProxyLst.obProxy[0].uPort;
            }
         }

         /* Set the domain in use back to the configuration module such
         ** that further 'domain' dependent features can be properly identified.
         */
         BOS_UINT8 *pcOrgActiveDomain = pUsr->pcActiveDomain;
         if ( pUsr->pDomainList )
         {
            pUsr->pcActiveDomain = pUsr->pDomainList->pcDomain;
            pUsr->uActiveDomain = pUsr->pDomainList->uDomainID;
         }
         else if (pAt)
         {
            pUsr->pcActiveDomain = (BOS_UINT8*)pAt;
         }

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pUsr,
                               CFG_ITEM_SET );
         pUsr->pcActiveDomain = pcOrgActiveDomain;


         /* configure MWI subscription only if bNoSubscribe is not set */
         if ( !bNoSubscribe )
         {
         /* Setup Message Waiting Indication configuration if enabled.
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, MWIConcept ),
                                    &cfgIdx,
                                       (void *) &pUsrMwi,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
               if ( pUsrMwi && pUsrMwi->Act.bCfgStatus )
            {
                  if ( pUsrMwi->bMwiSrvSet )
               {
                     if ( pUsrMwi->pcMwiSrvDomain )
                  {
                     cctkUsr.mwi.gen.server.sipHost.hostPort.pcAddr =
                                                       pUsrMwi->pcMwiSrvDomain;
                  }
                  else
                  {
                        bosIpAddrGetStr ( &pUsrMwi->mwiSrvAddr,
                                       (char *) uMwiIpAddr,
                                       CMGR_IP_ADDRESS_STRING_LEN );

                     cctkUsr.mwi.gen.server.sipHost.hostPort.pcAddr =
                                                            &uMwiIpAddr[0];
                  }
                  cctkUsr.mwi.gen.server.sipHost.hostPort.uPort =
                                                   (BOS_UINT16) pUsrMwi->uSIPPort;
               }
               else
               {
                  cctkUsr.mwi.gen.server.sipHost.hostPort.pcAddr =
                     cctkUsr.reg.gen.registrar.hostPort.pcAddr;
                  cctkUsr.mwi.gen.server.sipHost.hostPort.uPort = 0;
               }
                  if ( pUsr->bSigSecurity )
               {
                  cctkUsr.mwi.gen.server.sipHost.bSec = BOS_TRUE;
               }

                  mwiTmr.uExpTmr = pUsrMwi->uSubDuration / CMGR_MSEC_2_SEC_DIVIDER;
               }
            }
         } /* !bNoSubscribe */

         /* Setup unsolicited notify configuration.
         ** Default is to not accept unsolicited MWI notification (as per RFC 3265).
         */
         cctkUsr.mwi.gen.bUnsolNtfy = BOS_FALSE;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UnsolicitedMWIConcept ),
                                    &cfgIdx,
                                    (void *) &pUsrUnSolNtfy,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            /* Enable support for blind message waiting.
            */
            if ( pUsrUnSolNtfy && pUsrUnSolNtfy->Act.bCfgStatus )
            {
               cctkUsr.mwi.gen.bUnsolNtfy = BOS_TRUE;
            }
            /* Disable support for blind message waiting.
            */
            else
            {
               cctkUsr.mwi.gen.bUnsolNtfy = BOS_FALSE;
         }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UnsolicitedMWIConcept ),
                                  &cfgIdx,
                                  (void *) pUsrUnSolNtfy,
                                  CFG_ITEM_DEL );
            pUsrUnSolNtfy = NULL;
         }

         /* Configure codecs/fax info by default using the endpoint id */
         CCTKMEDAUD *pCfgAudio = &cctkMedAud;
         memset( pCfgAudio, 0, sizeof( CCTKMEDAUD ) );
         pCfgAudio->qos.bRtcpXrSupport = BOS_TRUE;

         pCfgAudio->gen.bTelEvt = BOS_TRUE;
         pCfgAudio->gen.uTelEvtTypes = CCTKNTE_DTMF;
         pCfgAudio->gen.bCn = BOS_TRUE;

         pCfgAudio->gen.rtpPort.uMinPort =
            (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                  pCctkAccnt->uHdl * ( 2 * CMGRCFG_MEDSES_MAX ) +
                  pCctkAccnt->uHdl );
         pCfgAudio->gen.rtpPort.uMaxPort =
            (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                  ( pCctkAccnt->uHdl + 1 ) * ( 2 * CMGRCFG_MEDSES_MAX ) +
                  pCctkAccnt->uHdl );
         pCfgAudio->gen.rtcpPort.uMinPort =
            (BOS_UINT16) ( CMGR_RTCP_AUD__PORT_OFFSET_START  +
                  pCctkAccnt->uHdl * ( 2 * CMGRCFG_MEDSES_MAX ) +
                  pCctkAccnt->uHdl );
         pCfgAudio->gen.rtcpPort.uMaxPort =
            (BOS_UINT16) ( CMGR_RTCP_AUD__PORT_OFFSET_START +
                  ( pCctkAccnt->uHdl + 1 ) * ( 2 * CMGRCFG_MEDSES_MAX ) +
                  pCctkAccnt->uHdl );

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = NULL;

         switch(pUsr->eSilSupp)
         {
            case CFG_SILENCE_SUPP_ON:
            {
               pCfgAudio->gen.eSilSupp = eCCTKSILSUPP_ON;
               pCfgAudio->gen.bCn = BOS_TRUE;
            }
            break;
            case CFG_SILENCE_SUPP_OFF:
            {
               pCfgAudio->gen.eSilSupp = eCCTKSILSUPP_OFF;
               pCfgAudio->gen.bCn = BOS_FALSE;
            }
            break;
            case CFG_SILENCE_SUPP_NO_SDP:
            {
               pCfgAudio->gen.eSilSupp = eCCTKSILSUPP_NOTUSED;
               pCfgAudio->gen.bCn = BOS_FALSE;
            }
            break;
            case CFG_SILENCE_SUPP_ON_NO_RTP_MAP:
            {
               pCfgAudio->gen.eSilSupp = eCCTKSILSUPP_ON;
               pCfgAudio->gen.bCn = BOS_FALSE;
            }
            break;
            default:
            break;
         }

         switch(pUsr->eSrtpPref)
         {
#if CMGRCFG_SRTP
            case CFG_SIP_SRTP_OPTIONAL:
            {
               pCfgAudio->sec.eSrtpPref = eCCTKSRTPPREF_PREFERRED;
			   pCfgAudio->sec.cryptoLst.uNumCrypto = 1;
			   pCfgAudio->sec.cryptoLst.crypto[0].eCryptoSuite = eCCTKCSUITE_AES_CM_128_HMAC_SHA1_80;
			   pCfgAudio->sec.cryptoLst.crypto[0].uLifeTime = 0; /* will use default */
            }
            break;
            case CFG_SIP_SRTP_MANDATORY:
            {
               pCfgAudio->sec.eSrtpPref = eCCTKSRTPPREF_MANDATORY;
			   pCfgAudio->sec.cryptoLst.uNumCrypto = 1;
			   pCfgAudio->sec.cryptoLst.crypto[0].eCryptoSuite = eCCTKCSUITE_AES_CM_128_HMAC_SHA1_80;
			   pCfgAudio->sec.cryptoLst.crypto[0].uLifeTime = 0; /* will use default */
            }
            break;
#endif
            case CFG_SIP_SRTP_DISABLED:
			default:
            {
               pCfgAudio->sec.eSrtpPref = eCCTKSRTPPREF_DISABLED;
			   pCfgAudio->sec.cryptoLst.uNumCrypto = 0;
            }
            break;
         }

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = NULL;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                    &cfgIdx,
                                    (void *) &pDevCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pDevCfg )
            {
               if( pDevCfg->uRtpMinPort && pDevCfg->uRtpMaxPort &&
                     pDevCfg->uRtcpMinPort && pDevCfg->uRtcpMaxPort )
               {
                  pCfgAudio->gen.rtpPort.uMinPort = pDevCfg->uRtpMinPort;
                  pCfgAudio->gen.rtpPort.uMaxPort = pDevCfg->uRtpMaxPort;
                  pCfgAudio->gen.rtcpPort.uMinPort = pDevCfg->uRtcpMinPort;
                  pCfgAudio->gen.rtcpPort.uMaxPort = pDevCfg->uRtcpMaxPort;
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                  &cfgIdx,
                                  (void *) pDevCfg,
                                  CFG_ITEM_DEL );
            pDevCfg = NULL;
         }

         /* Setup the supported codec.
         */
         pCfgAudio->gen.uNumCodec = 0;
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         cfgIdx.uIndex = 0;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecList ),
                                    &cfgIdx,
                                    (void *) &pCodecLstCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pCodecLstCfg )
            {
               CFGEPTCODECLIST *pCodecLstCfgTmp = pCodecLstCfg;

               /* Loop through the codec and allocate those we
               ** can negotiate (i.e. know about and understand).
               */
               do
               {
                  pCfgAudio->gen.codec[ pCfgAudio->gen.uNumCodec ].eType =
                     cmMapGetId ( cmMapCfgCodec2CctkCodec,
                           pCodecLstCfgTmp->codecType );

                  /* If the codec retrieved is valid, increment, otherwise
                  ** ignore it.
                  */
                  if ( pCfgAudio->gen.codec[ pCfgAudio->gen.uNumCodec ].eType !=
                        eCCTKCODEC_MAX )
                  {
                     /* Check whether there are per-codec specific data to be
                     ** set.
                     */
                     switch ( pCfgAudio->gen.codec[ pCfgAudio->gen.uNumCodec ].eType )
                     {
                        case eCCTKCODEC_G729E:
                        {
                           /* For the time being always set non annex-b usage, this will
                           ** be negotiated.
                           */
                           pCfgAudio->gen.codec[
                              pCfgAudio->gen.uNumCodec ].parm.eG729Vad =
                                                               eCCTKG729VAD_YES;
                        }
                        break;

                        case eCCTKCODEC_G7231:
                        {
                           /* For the time being always set annex-a usage, this will
                           ** be negotiated.
                           */
                           pCfgAudio->gen.codec[
                              pCfgAudio->gen.uNumCodec ].parm.eG7231Vad =
                              eCCTKG7231VAD_YES;
                        }
                        break;

                        case eCCTKCODEC_AMRWB:
                        {
                           /* If AMR-WB is part of the capabilities, ensure we set
                           ** the 'maxptime' attribute because it is necessary for
                           ** AMR-WB support.
                           */
                           pCfgAudio->gen.uMaxPtime = CMGR_MAXPTIME_DEFAULT;
                        }
                        break;

                        default:
                        break;
                     }

                     ++pCfgAudio->gen.uNumCodec;
                  }

                  pCodecLstCfgTmp = pCodecLstCfgTmp->pNext;
               }
               while ( pCodecLstCfgTmp != NULL );
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecList ),
                                  &cfgIdx,
                                  (void *) pCodecLstCfg,
                                  CFG_ITEM_DEL );
            pCodecLstCfg = NULL;
         }
         /* If for some reason we were not able to get the codec supported
         ** information or we have found no valid entries in our codec list that
         ** are acceptable to use, we are still forcing usage of PCMU/PCMA since
         ** those are the minimum mandatory set that we know we can support.
         */
         if ( !pCfgAudio->gen.uNumCodec )
         {
            pCfgAudio->gen.uNumCodec = 2;
            pCfgAudio->gen.codec[ 0 ].eType = eCCTKCODEC_PCMU;
            pCfgAudio->gen.codec[ 1 ].eType = eCCTKCODEC_PCMA;
         }

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         cfgIdx.uIndex = 0;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                    &cfgIdx,
                                    (void *) &pCodecCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pCodecCfg )
            {
               BOS_UINT32 uIx;

#if CCTKCFG_RED
               /* If pCodecCfg has redundancy enabled, set the first codec of
                * the codec list as eCCTKCODEC_RED. */
               if(pCodecCfg->bREDEnable)
               {
                  memmove( &pCfgAudio->gen.codec[1], &pCfgAudio->gen.codec[0], sizeof(pCfgAudio->gen.codec) - sizeof(pCfgAudio->gen.codec[0]) );

                  pCfgAudio->gen.codec[0].eType = eCCTKCODEC_RED;
                  if( pCfgAudio->gen.uNumCodec < CCTKCFG_MAX_AUDIO_CODEC )
                  {
                     pCfgAudio->gen.uNumCodec++;
                  }

                  /* TODO: Support recieving any configuration of RED. */
                  pCfgAudio->gen.codec[0].parm.redCfg.uNumCodec = 1;
                  pCfgAudio->gen.codec[0].parm.redCfg.eCodec[0] = pCfgAudio->gen.codec[1].eType;
               }
#endif

               /* For now, use the default value for all configured codec.
               */
               for ( uIx = 0 ; uIx < pCfgAudio->gen.uNumCodec ; uIx++ )
               {
                  pCfgAudio->gen.codec[ uIx ].uPtime =
                     (BOS_UINT32) pCodecCfg->sG711PktPeriod;
               }

               /* Possible over-write of default value.
               */
               pCfgAudio->gen.bTelEvt = pCodecCfg->bDtmfRelayEnabled;
               pCfgAudio->qos.bRtcpXrSupport = pCodecCfg->bRTCPXRepEnable;

               /* Reset the RTCP port setting if we want to use RTP port + 1 for
               ** RTCP.
               */
               if ( pCodecCfg->bRTCPUseNextPort )
               {
                  pCfgAudio->gen.rtcpPort.uMinPort = 0;
                  pCfgAudio->gen.rtcpPort.uMaxPort = 0;
               }

               /* Override the device level silence suppression setting if perUsr setting is differnt */
               if ( pCodecCfg->bSilenceDisable )
               {
                  pCfgAudio->gen.eSilSupp = eCCTKSILSUPP_OFF;
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                  &cfgIdx,
                                  (void *) pCodecCfg,
                                  CFG_ITEM_DEL );
            pCodecCfg = NULL;
         }

         CCTKMEDFAX* pCfgFax = &cctkMedFax;
         BOS_BOOL* pbEnable = &bEnable;
         memset ( pCfgFax, 0, sizeof( CCTKMEDFAX ) );

         /* Always set the max datagram size to be default, eventually when
         ** the physical interface will support higher fax rate (SG3 @ 33.3 kbps
         ** for example) we would adjust this accordingly.
         */
         pCfgFax->uMaxDatagram = CMGR_T38_MAX_DATAGRAM_DEFAULT;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.uIndex = 0;
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                    &cfgIdx,
                                    (void *) &pCodecCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pCodecCfg )
            {
               if ( pCodecCfg->bT38Enable )
               {
                  pCfgFax->t38Port.uMinPort =
                      (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                                     pCctkAccnt->uHdl * ( 2 * CMGRCFG_MEDSES_MAX ) );
                  pCfgFax->t38Port.uMaxPort =
                      (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                             ( pCctkAccnt->uHdl + 1 ) * ( 2 * CMGRCFG_MEDSES_MAX ) );

                  /* If max and min ports are specificed in device
                  ** configuration (ie, if CCTKCFG_RANDOM_PORTS is specified),
                  ** we should override the T.38 ports with these.
                  */
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = NULL;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                             &cfgIdx,
                                             (void *) &pDevCfg,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pDevCfg && pDevCfg->uRtpMinPort && pDevCfg->uRtpMaxPort &&
                           pDevCfg->uRtcpMinPort && pDevCfg->uRtcpMaxPort )
                     {
                        pCfgFax->t38Port.uMinPort = pDevCfg->uT38MinPort;
                        pCfgFax->t38Port.uMaxPort = pDevCfg->uT38MaxPort;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                           &cfgIdx,
                                           (void *) pDevCfg,
                                           CFG_ITEM_DEL );
                     pDevCfg = NULL;
                  }
               }
               else
               {
                  *pbEnable = BOS_FALSE;
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                  &cfgIdx,
                                  (void *) pCodecCfg,
                                  CFG_ITEM_DEL );
            pCodecCfg = NULL;
         }

         /* Setup ourselves to always use the audio media only (or with
         ** video media if supported) at first.  If a fax media (image)
         ** is needed, it would happen through a reconfiguration.
         */
         memset ( &cctkMedLst,
                  0,
                  sizeof( CCTKMEDLST ) );

         cctkMedLst.mMediaTypes = CCTK_MEDIA_AUDIO_MASK;
         cctkMedLst.uNumAudCodec = cctkMedAud.gen.uNumCodec;
         for ( uIx = 0 ; uIx < cctkMedLst.uNumAudCodec ; uIx++ )
         {
            cctkMedLst.audCodeLst[ uIx ] = cctkMedAud.gen.codec[ uIx ].eType;
         }

         /* Associate the media configuration with this user.
         */
         cctkUsr.dft.call.media.pAudio = &cctkMedAud;
         cctkUsr.dft.call.media.pFax = ( bEnable ? &cctkMedFax : NULL );
         cctkUsr.dft.call.media.pMediaLst = &cctkMedLst;

         /* Note that the following two features:
         **
         **      - Outbound Keep Alive (i.e. use of Persistent Connection).
         **      - Backup and Recovery
         **
         ** are mutually exclusives in the current architecture, only one of those
         ** can be used at any one time.
         */
         if ( pPcscfUsr && pPcscfUsr->bBackupRecovery )
         {
            /* Using backup and recovery feature.  This is an internal feature of
            ** the CCTK layer which will take care of selecting the target (or an
            ** alternate) for a given request based on internal knowledge.  It also
            ** offers recovery process which allows to return to a previously known
            ** 'good' target following a loss of connectivity to that target.
            */
            CMGRLOG_INFORMATION ((
                        "cmProfRfc3261CctkAccntAdd: cctkUsrUuid %s - cctkUsrHdl 0x%X, uses backup-recovery",
                        pCctkAccnt->uUid, pCctkAccnt->uHdl ));
         }
#if defined(CMGRCFG_OUTBOUND_KEEP_ALIVE) && !defined(CCTKCFG_REMOTE_USER_CONTROL)
         else if ( !bKeepAliveDisabled )
         {
            /* Setup outbound keep alive configuration.  The outbound keep alive
            ** configuration ensures that we are always accessing the same target
            ** as long as it is alive, this is a requirement of IMS.  Note that it
            ** does not necessarely mean that we are running keep alive signaling
            ** on the link.
            */
            cctkUsr.usr.sec.nat.kaCfg.uTmr = CM_CFG__KEEP_ALIVE_TIMER__FIXED +
                                 1000 * (rand() % CM_CFG__KEEP_ALIVE_TIMER__VARMOD);
            cctkUsr.usr.sec.nat.kaCfg.server.hostPort.pcAddr =
                                 cctkUsr.reg.gen.registrar.hostPort.pcAddr;
            cctkUsr.usr.sec.nat.kaCfg.server.hostPort.uPort =
                                 cctkUsr.reg.gen.registrar.hostPort.uPort;
            cctkUsr.usr.sec.nat.kaCfg.server.bSec = cctkUsr.reg.gen.registrar.bSec;
            cctkUsr.usr.sec.nat.kaCfg.uSrcPort = uLocalPort;
         }
#endif /* defined(CMGRCFG_OUTBOUND_KEEP_ALIVE) && !defined(CCTKCFG_REMOTE_USER_CONTROL) */

         /* Create the CCTK user.
         */
         if ( cctkUserAdd ( &pCctkAccnt->uNet, &cctkUsr ) == eCCTKSTATUS_SUCCESS )
         {
#if CMGRCFG_VIDEO_MEDIA
            /* Clean up following configuration assignement.
            */
            for ( uIx = 0 ; uIx < cctkMedVid.gen.uNumCodec ; uIx++ )
            {
               if ( (cctkMedVid.gen.codec[ uIx ].eType == eCCTKCODEC_H264) &&
                    (cctkMedVid.gen.codec[ uIx ].parm.pH264 != NULL) )
               {
                  free ( cctkMedVid.gen.codec[ uIx ].parm.pH264 );
                  cctkMedVid.gen.codec[ uIx ].parm.pH264 = NULL;
               }
            }
#endif

            /* Setup registration timeout configuration for that user.
            */
            cctkGetParm ( pCctkAccnt->uNet,
                          eCCTKPARM_EC_REGTMR,
                          (void *) &regTmrCfg );

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCctkAccnt->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, StatusChangeConcept ),
                                       &cfgIdx,
                                       (void *) &pActSta,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pActSta && pActSta->Act.bCfgStatus )
               {
                  regTmrCfg.uRegExpTmr =
                        (BOS_UINT32) pActSta->uChgRegExp / 1000;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, StatusChangeConcept ),
                                     &cfgIdx,
                                     (void *) pActSta,
                                     CFG_ITEM_DEL );
               pActSta = NULL;
            }

            cctkSetParm ( pCctkAccnt->uNet,
                          eCCTKPARM_EC_REGTMR,
                          (const void *) &regTmrCfg );

            /* Setup TOS configuration for that user.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCctkAccnt->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pBasicCall && pBasicCall->Act.bCfgStatus )
               {
                  portCfg.tosCfg.uDefault =
                        (BOS_UINT8) pBasicCall->uNEMDSCPValueSig;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_DEL );
               pBasicCall = NULL;
            }

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                       &cfgIdx,
                                       (void *) &pEmergSvc,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pEmergSvc && pEmergSvc->Act.bCfgStatus )
               {
                  portCfg.tosCfg.uEmgCall =
                                 (BOS_UINT8 )pEmergSvc->uSigDSCPVal;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                     &cfgIdx,
                                     (void *) pEmergSvc,
                                     CFG_ITEM_DEL );
               pEmergSvc = NULL;
            }

            cctkSetParm ( pCctkAccnt->uNet,
                          eCCTKPARM_EC_USRPORTCFG,
                          (const void *) &portCfg );

            /* Setup MWI configuration.
            */
            cctkSetParm ( pCctkAccnt->uNet,
                          eCCTKPARM_EC_MWICFG,
                          (const void *) &mwiCfg );

            cctkSetParm ( pCctkAccnt->uNet,
                          eCCTKPARM_EC_MWITMR,
                          (const void *) &mwiTmr );

            /* Setup transfer configuration.
            */
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrXfer,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrXfer && pUsrXfer->Act.bCfgStatus )
               {
                  xferCfg.tfererCfg.uNotifyWaitTime = pUsrXfer->uNotifyTO;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                     &cfgIdx,
                                     (void *) pUsrXfer,
                                     CFG_ITEM_DEL );
               pUsrXfer = NULL;
            }

            cctkSetParm ( pCctkAccnt->uNet,
                          eCCTKPARM_EC_CALLXFER,
                          (const void *) &xferCfg );

            /* Setup dialog event configuration.
            */
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BLVConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrBlv,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrBlv &&
                    pUsrBlv->Act.bCfgStatus &&
                    pUsrBlv->pcOperId )
               {
                  dlgCfg.pcSubscriber = pUsrBlv->pcOperId;
                  /* Allow at least one subscriber to the dialog event
                  ** package, this is for the operator.
                  */
                  dlgCfg.uSubeeMax = 1;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BLVConcept ),
                                     &cfgIdx,
                                     (void *) pUsrBlv,
                                     CFG_ITEM_DEL );
               pUsrBlv = NULL;
            }

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrACall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrACall && pUsrACall->Act.bCfgStatus )
               {
                  dlgCfg.uSuberMax += pUsrACall->uMaxSubSend;
                  dlgCfg.uSubeeMax += pUsrACall->uMaxSubRec;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                     &cfgIdx,
                                     (void *) pUsrACall,
                                     CFG_ITEM_DEL );
               pUsrACall = NULL;
            }

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrACall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pUsrACall && pUsrACall->Act.bCfgStatus )
               {
                  dlgCfg.uSuberMax += pUsrACall->uMaxSubSend;
                  dlgCfg.uSubeeMax += pUsrACall->uMaxSubRec;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                     &cfgIdx,
                                     (void *) pUsrACall,
                                     CFG_ITEM_DEL );
               pUsrACall = NULL;
            }

            cctkSetParm ( pCctkAccnt->uNet,
                          eCCTKPARM_EC_DIAGEVTCFG,
                          (const void *) &dlgCfg );

            /* configure ua-profile subscription only if bNoSubscribe is not set */
            if ( !bNoSubscribe )
            {
            /* Setup User-Agent Profile event configuration.
            **
            ** Note on this configuration.  We need to get the information from
            ** the user-agent profile for those features which status is kept
            ** outside of our internal knowledge and for which we need to act
            ** upon accordingly.  There are three features that are in this
            ** category: do-not-disturb, selective-call-forwarding and
            ** call-forwarding-variable.
            **
            ** Each of these features may be managed by a different application
            ** server, which means we may have to be ready to subscribe to up to
            ** three different servers to get the full picture of the feature
            ** status.  To how many application server(s) we will subscribe in the
            ** end will be determined by the configuration, which we are now
            ** analyzing.
            */

               /* Get the default configuration and update as appropriate */
               if ( cctkGetParm ( pCctkAccnt->uNet,
                             eCCTKPARM_EC_UAPEVTCFG,
                             (const void *) &uapCfg ) == eCCTKSTATUS_SUCCESS )
               {
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DNDConcept ),
                                       &cfgIdx,
                                             (void *) &pUsrDnd,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
                     if ( pUsrDnd &&
                          pUsrDnd->Act.bCfgStatus &&
                          pUsrDnd->pcAUID &&
                          strlen( (const char *)pUsrDnd->pcAUID ) )
               {
                        uapCfg.pcProfile[ uapCfg.uNum ] = pUsrDnd->pcAUID;
                  uapCfg.uNum++;
               }
            }

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, SCFConcept ),
                                       &cfgIdx,
                                             (void *) &pUsrScf,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
                     if ( pUsrScf &&
                          pUsrScf->Act.bCfgStatus &&
                          pUsrScf->pcAUID &&
                          strlen( (const char *)pUsrScf->pcAUID ) )
               {
                  /* Only add this configuration if we have not set any yet or
                  ** if it is different value than the one we may have already
                  ** setup.
                  */
                  bExist = BOS_FALSE;
                  for ( uIx = 0 ; uIx < uapCfg.uNum ; uIx++ )
                  {
                     if ( uapCfg.pcProfile[ uIx ] &&
                          !strCmpNoCase( (const char *)uapCfg.pcProfile[ uIx ],
                                               (const char *)pUsrScf->pcAUID ) )
                     {
                        bExist = BOS_TRUE;
                     }
                  }

                  if ( !uapCfg.uNum || !bExist )
                  {
                           uapCfg.pcProfile[ uapCfg.uNum ] = pUsrScf->pcAUID;
                     uapCfg.uNum++;
                  }
               }
            }

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                       &cfgIdx,
                                             (void *) &pUsrCfv,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
                     if ( pUsrCfv &&
                          pUsrCfv->Act.bCfgStatus &&
                          pUsrCfv->pcAUID &&
                          strlen( (const char *)pUsrCfv->pcAUID ) )
               {
                  /* Only add this configuration if we have not set any yet or
                  ** if it is different value than the one we may have already
                  ** setup.
                  */
                  bExist = BOS_FALSE;
                  for ( uIx = 0 ; uIx < uapCfg.uNum ; uIx++ )
                  {
                     if ( uapCfg.pcProfile[ uIx ] &&
                          !strCmpNoCase( (const char *)uapCfg.pcProfile[ uIx ],
                                               (const char *)pUsrCfv->pcAUID ) )
                     {
                        bExist = BOS_TRUE;
                     }
                  }

                  if ( !uapCfg.uNum || !bExist )
                  {
                           uapCfg.pcProfile[ uapCfg.uNum ] = pUsrCfv->pcAUID;
                     uapCfg.uNum++;
                  }
               }
            }
               } /* end of !bNoSubscribe */

               /* Set up unsolicited UA-Profile Notify configuration */
               pUsrUnSolNtfy = NULL;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UnsolicitedUAProfileConcept ),
                                          &cfgIdx,
                                          (void *) &pUsrUnSolNtfy,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  /* Enable support for unsolicited ua-profile notifications.
                  */
                  if ( pUsrUnSolNtfy && pUsrUnSolNtfy->Act.bCfgStatus )
                  {
                     uapCfg.bUnsolNtfy = BOS_TRUE;
                  }
                  /* Disable support for unsolicited ua-profile notifications.
                  */
                  else
                  {
                     uapCfg.bUnsolNtfy = BOS_FALSE;
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UnsolicitedUAProfileConcept ),
                                        &cfgIdx,
                                        (void *) pUsrUnSolNtfy,
                                        CFG_ITEM_DEL );
                  pUsrUnSolNtfy = NULL;
               }

               cctkSetParm ( pCctkAccnt->uNet,
                          eCCTKPARM_EC_UAPEVTCFG,
                          (const void *) &uapCfg );
            }

            /* Setup backup and recovery configuration.
            */
            if ( cctkGetParm ( pCctkAccnt->uNet,
                               eCCTKPARM_EC_USRBRCFG,
                               (void *) &usrBrCfg ) == eCCTKSTATUS_SUCCESS )
            {
               /* Update backup and recovery timer value as apporpriate.
               */
               usrBrCfg.uBackoffMinTmr = pUsr->uRegRetryTimerMin / 1000;
               usrBrCfg.uBackoffMaxTmr = pUsr->uRegRetryTimerMax / 1000;

               /* Update BrCfg flags as appropriate.
               ** bFailover is used to control the failover feature in cctk.
               */
               if ( bPcscfQueried && pPcscfUsr )
               {
                  /* The bFailover flag should only be enabled if
                  ** the failover feature itself is not disabled.
                  */
                  usrBrCfg.bFailover = ( usrBrCfg.eMode != eCCTKBRSVCMODE_DISABLED ) ?
                                          pPcscfUsr->bBackupRecovery : BOS_FALSE;

                  /* Update the backup and recovery timer as per the backoff mode.
                  ** If in exponential outbound mode the backoff timers must
                  ** be set to the flow recovery timer values.
                  */
                  if( usrBrCfg.bOutboundBackoff )
                  {
                     usrBrCfg.uBackoffMinTmr = pPcscfUsr->uBaseTimeAllFailed / CMGR_MSEC_2_SEC_DIVIDER;
                     usrBrCfg.uBackoffMaxTmr = pPcscfUsr->uMaxTime / CMGR_MSEC_2_SEC_DIVIDER;
                  }

                  if ( !pPcscfUsr->bBackupRecovery )
                  {
                     usrBrCfg.eBackToPrimMode = eCCTKBRBACKTOPRIMMODE_DISABLED;
                     usrBrCfg.bEnableInbdReg = BOS_FALSE;
                  }
                  /* If failover is enabled and is in external IP-list mode
                  ** build the target list from the outbound proxy list.
                  */
                  else if ( usrBrCfg.eMode == eCCTKBRSVCMODE_EXTLIST )
                  {
                     BOS_IP_ADDRESS obIpAddr;
                     BOS_UINT32 uJx = 0;
                     
                     memset( &usrBrCfg.extList, 0, sizeof( CCTKIPPORTLST ) );

                     for ( uIx = 0; uIx < pPcscfUsr->obProxyLst.uNum; uIx++ )
                     {
                        /* If there is a outbound proxy ip-address configured
                        ** then it can be added to the external list.
                        */
                        if ( ( pPcscfUsr->obProxyLst.obProxy[uIx].pcAddr &&
                                 strlen( (char*)pPcscfUsr->obProxyLst.obProxy[uIx].pcAddr ) ) &&
                             ( bosIpAddrCreateFromStr(
                                 (char*)pPcscfUsr->obProxyLst.obProxy[uIx].pcAddr, &obIpAddr ) == BOS_STATUS_OK ) &&
                             ( !bosIpAddrIsZero(&obIpAddr) ) )
                        {
                           usrBrCfg.extList.ipport[usrBrCfg.extList.uNum].addr = obIpAddr;
                           usrBrCfg.extList.ipport[usrBrCfg.extList.uNum].uPort =
                                                   pPcscfUsr->obProxyLst.obProxy[uIx].uPort;
                           usrBrCfg.extList.uNum++;
                        }
                        else
                        {
                           CMGRLOG_CRITICAL (( "cmProfImsDefaultUserAdd: Failed to configure recovery ip list!" ));
                        }

                        if ( usrBrCfg.extList.uNum == CCTKCFG_MAX_ADDRLST )
                        {
                           /* Backup and recovery external list is full.
                           */
                           break;
                        }
                     }
                  }

                  usrBrCfg.uBackToPrimMinTmr = pPcscfUsr->uPrimaryRecoveryTimerMin;
                  usrBrCfg.uBackToPrimMaxTmr = pPcscfUsr->uPrimaryRecoveryTimerMax;
               }

               cctkSetParm ( pCctkAccnt->uNet,
                             eCCTKPARM_EC_USRBRCFG,
                             (const void *) &usrBrCfg );
            }

            /* Setup session timer configuration.
            */
            if ( bSetSessionTmr &&
                 ( cctkGetParm ( pCctkAccnt->uNet,
                               eCCTKPARM_EC_SESTMR,
                               (void *) &usrSesTmrCfg ) == eCCTKSTATUS_SUCCESS ) )
            {
               usrSesTmrCfg.uTmr = uSessionTmrExpires;
               usrSesTmrCfg.uMinSe = uSessionTmrMin;

               cctkSetParm ( pCctkAccnt->uNet,
                             eCCTKPARM_EC_SESTMR,
                             (const void *) &usrSesTmrCfg );
            }

            /* Create a line-specific DM for this user */
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrDigMap,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               cmDmCompile ( pCctkAccnt->uNet,
                             pUsrDigMap->pABNF,
                             (CMDMDIGITMAP *) pCctkAccnt->pDmDigMap,
                             CMDM_MAP_BUFFER_SIZE );

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                     &cfgIdx,
                                     (void *) pUsrDigMap,
                                     CFG_ITEM_DEL );
               pUsrDigMap = NULL;
            }

            /* Clean up configuration.
            */
            if ( pUsrDnd )
            {
               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DNDConcept ),
                                     &cfgIdx,
                                     (void *) pUsrDnd,
                                     CFG_ITEM_DEL );
               pUsrDnd = NULL;
            }

            if ( pUsrScf )
            {
               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, SCFConcept ),
                                     &cfgIdx,
                                     (void *) pUsrScf,
                                     CFG_ITEM_DEL );
               pUsrScf = NULL;
            }

            if ( pUsrCfv )
            {
               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                     &cfgIdx,
                                     (void *) pUsrCfv,
                                     CFG_ITEM_DEL );
               pUsrCfv = NULL;
            }
         }

         CMGRLOG_INFORMATION ((
              "cctkUsrUuid %s - cctkUsrHdl 0x%X, net 0x%X",
              pCctkAccnt->uUid,
              pCctkAccnt->uHdl,
              pCctkAccnt->uNet ));

         if ( bPcscfQueried )
         {
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCctkAccnt->uUid;

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                  &cfgIdx,
                                  (void *) pPcscfUsr,
                                  CFG_ITEM_DEL );
            pPcscfUsr = NULL;
         }

         if ( pUsrMwi != NULL )
         {
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCctkAccnt->uUid;

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, MWIConcept ),
                                  &cfgIdx,
                                  (void *) pUsrMwi,
                                  CFG_ITEM_DEL );
            pUsrMwi = NULL;
         }
      }

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pCctkAccnt->uUid;

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                            &cfgIdx,
                            (void *) pUsr,
                            CFG_ITEM_DEL );
      pUsr = NULL;

      /* Cleanup allocated resources */
      if( puTemp )
      {
         free(puTemp);
         puTemp = NULL;
      }
   }

   return BOS_TRUE;
}

#endif /* (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_RFC3261) */
