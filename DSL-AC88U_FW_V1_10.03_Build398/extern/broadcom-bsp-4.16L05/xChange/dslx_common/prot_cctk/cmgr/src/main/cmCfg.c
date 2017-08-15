/**********************************************************************************
** Copyright (c) 2009-2013 Broadcom Corporation
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
**      related processings.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

#define CMGR__USER_IDENTITY__USERNAME           6
#define CMGR__USER_IDENTITY__PUBLIC_ID          3

#define CMGR_AOR_DELIMITER                      '@'

#define CMGR_T38_MAX_DATAGRAM_DEFAULT           160
#define CMGR_MAXPTIME_DEFAULT                   20
#define CMGR_MAXPTIME_AMR                       240
#define CMGR_MAXRED_AMR                         220

/* Note constraints on pool size depending on value of CMGR_RTCP_PORT_OFFSET_START */
#define CMGR_RTP_PORT_POOL_SIZE                 64

/* ---- Private Variables ------------------------------------------------------- */
static BOS_BOOL gbCmCfgCctkConfig = BOS_FALSE;

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmCfgInit
**
** PURPOSE:    Initializes the configuration interface and configuration elements.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if we have successfully initialized our configuration.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgInit ( void )
{
   CFGPROVDEVICECFG *pDevCfg = NULL;
   CFGSIPPCSCF *pPcscfUsr = NULL;
   CFGSIPUSERCFG *pUsrCfg = NULL;
   CCTKCFG cctkCfg;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT8 *pMacAdd = NULL;
   BOS_UINT8 seed[ CMGR_COLLECTED_DIGIT_LEN ];
   BOS_UINT8 uaInstanceId[ CMGR_URI_STRING_LEN ];
   CCTKTRANSTMRCFG cctkTmrCfg;
   CCTKLOCPORT cctkLocPortCfg;
   CCTKSIGTRANS cctkSigTransCfg;
#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_IMS) || (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_RFC3261)
   CCTKUAIDHDR cctkUaCfg;
#endif
   CCTKPANICFG paniCfg;
#if CCTKCFG_ICE_LITE_SUPPORT
   CCTKIPADDRLST cctkIceIpAddrLstCfg;
#endif
   BOS_UINT8 *puTmp = NULL;
   BOS_UINT32 uLen;

   memset ( (void *) seed, 0, CMGR_IP_ADDRESS_BYTES_SIZE );
   memset ( (void *) uaInstanceId, 0, CMGR_URI_STRING_LEN );
   memset( &paniCfg, 0, sizeof(CCTKPANICFG) );


   /* This should be the first configuration item we try to retrieve at this
   ** time.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                              &cfgIdx,
                              (void *) &pDevCfg,
                              CFG_ITEM_GET ) != BOS_STATUS_OK )
   {
      /* Note that we do not need to apply 'deletion' on the device
      ** concept configuration in this case because we failed to
      ** retrieve it properly in the first place.
      */
      return BOS_FALSE;
   }

#if CMGRCFG_TLS
   /* Configure TLS (must be done prior to calling cctkConfig().
   */
   if ( cmCfgTLS() == BOS_FALSE )
   {
      CMGRLOG_WARNING (( "cmCfgInit: Failed TLS setup." ));
   }
#endif /* CMGRCFG_TLS */

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = NULL;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) &pUsrCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pUsrCfg )
      {
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsrCfg->pDomainList->pcDomain;
         cfgIdx.uIndex = pUsrCfg->pDomainList->uDomainID;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                    &cfgIdx,
                                    (void *) &pPcscfUsr,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            /* Initialize all timers to zero to use default.
            */
            memset( (void *) &cctkTmrCfg,
                    0,
                    sizeof( cctkTmrCfg ) );

            /* Initialize signaling transport information.
            */
            cctkSigTransCfg.uSigTrans = CCTK_TRANSPORT_UDP | CCTK_TRANSPORT_TCP;
#if CMGRCFG_TLS
            cctkSigTransCfg.uSigTrans |= CCTK_TRANSPORT_TLS;
#endif
            cctkSigTransCfg.uTcpFallbackThresh = CCTK_TRANSPORT_TCP_FALLBACK_MTU;

            /* The decision to determine whether keep alive is enabled or not
            ** is controlled via the keep alive interval.  An interval of 0
            ** indicates that keep alive is to be disabled.
            */
            cctkSigTransCfg.bKeepAlive = ( pDevCfg->uKeepAliveMaxInterval ) ?
                                                BOS_TRUE : BOS_FALSE;

            /* overwrite the signaling transport setup.
            */
            if ( pPcscfUsr )
            {
               cctkTmrCfg.uT1 = pPcscfUsr->uTimerT1;
               cctkTmrCfg.uT2 = pPcscfUsr->uTimerT2;
               cctkTmrCfg.uT4 = pPcscfUsr->uTimerT4;
               cctkTmrCfg.uD  = pPcscfUsr->uTimerTD;
               cctkTmrCfg.uB  = pPcscfUsr->uTimerTB;
               cctkTmrCfg.uH  = pPcscfUsr->uTimerTH;
               cctkTmrCfg.uF  = pPcscfUsr->uTimerTF;

               cctkSigTransCfg.uSigTrans = pPcscfUsr->uSigTrans;
               if ( pPcscfUsr->bIgnoreTcpFallBack )
               {
                  cctkSigTransCfg.uTcpFallbackThresh = 0;
               }
            }

            cctkSetParm ( CMGR_INVALID_HANDLE,
                          eCCTKPARM_EC_TRANSTMR,
                          (void *) &cctkTmrCfg );

            /* If no valid signaling transport was configured, ignore the
            ** setting as it would render the device useless.
            */
            if ( cctkSigTransCfg.uSigTrans )
            {
               cctkSetParm ( CMGR_INVALID_HANDLE,
                             eCCTKPARM_EC_TRANSPORT,
                             (void *) &cctkSigTransCfg );
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                  &cfgIdx,
                                  (void *) pPcscfUsr,
                                  CFG_ITEM_DEL );
            pPcscfUsr = NULL;
         }
         else
         {
            CMGRLOG_WARNING (( "cmCfgInit: Failed to retrieve PCSCF object." ));
         }
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                            &cfgIdx,
                            (void *) pUsrCfg,
                            CFG_ITEM_DEL );
      pUsrCfg = NULL;
   }

   /* Initialize the device configuration.
   */
   memset ( &cctkCfg, 0, sizeof ( CCTKCFG ) );
   memcpy ( &cctkCfg.net.gen.host.addr[ cctkCfg.net.gen.host.uNum ].ipAddr,
            &pDevCfg->IpAddress,
            sizeof ( BOS_IP_ADDRESS ) );
   cctkCfg.net.gen.host.addr[ cctkCfg.net.gen.host.uNum ].pcFqdn = pDevCfg->pcFQDN;
#if CMGRCFG_IPv6
   cctkCfg.net.gen.host.addr[ cctkCfg.net.gen.host.uNum ].uScopeId =
                                                               pDevCfg->uInterface;
#endif
   ++cctkCfg.net.gen.host.uNum;

#if CCTKCFG_ICE_LITE_SUPPORT
   /* Copy over the primary address */
   memset ( (void *) &cctkIceIpAddrLstCfg,
            0,
            sizeof ( cctkIceIpAddrLstCfg ) );

   memcpy ( &cctkIceIpAddrLstCfg.addr[ cctkIceIpAddrLstCfg.uNum ],
               &pDevCfg->IpAddress,
               sizeof ( BOS_IP_ADDRESS ) );
   ++cctkIceIpAddrLstCfg.uNum;

   /* Check if there is a Secondary Address */
   if (bosIpAddrIsZero(&pDevCfg->SecIpAddress) == BOS_FALSE)
   {
      memcpy ( &cctkIceIpAddrLstCfg.addr[ cctkIceIpAddrLstCfg.uNum ],
               &pDevCfg->SecIpAddress,
               sizeof ( BOS_IP_ADDRESS ) );
      ++cctkIceIpAddrLstCfg.uNum;
   }
   cctkSetParm ( CMGR_INVALID_HANDLE,
                 eCCTKPARM_EC_ICEIPLSTCFG,
                 (void *) &cctkIceIpAddrLstCfg );
#endif

   pMacAdd = pDevCfg->pMAC;
   if ( pMacAdd )
   {
      sprintf ( (char *) seed,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                *pMacAdd, *(pMacAdd + 1),
                *(pMacAdd + 2), *(pMacAdd + 3),
                *(pMacAdd + 4), *(pMacAdd + 5) );
      /* Add the URN:UUID (per RFC 4122) */
      sprintf ( (char *) uaInstanceId,
                "urn:uuid:00000000-0000-1000-8000-%02X%02X%02X%02X%02X%02X",
                *pMacAdd, *(pMacAdd + 1),
                *(pMacAdd + 2), *(pMacAdd + 3),
                *(pMacAdd + 4), *(pMacAdd + 5) );
   }
   else
   {
      sprintf ( (char *) seed,
                "PlantRandomSeed" );
      /* Add the URN:UUID (per RFC 4122) */
      sprintf ( (char *) uaInstanceId,
                "urn:uuid:00000000-0000-1000-8000-010203040506" );
   }
   cctkCfg.loc.gen.pcSeed = (BOS_UINT8 *)seed;

   /* Populate the emergency information.
   */
   /*The structure tells us whether to use civic address or geo location*/
   if (pDevCfg->locType != CFG_PROV_LOC_INFO_PREF_GEO)
   {
      /*Populate fields for civic address format*/
      cctkCfg.net.prot.gen.emgAddr.bUseGeoLoc = BOS_FALSE;
      cctkCfg.net.prot.gen.emgAddr.pcHouseNum = pDevCfg->locInfo.civicInfo.HNO;
      cctkCfg.net.prot.gen.emgAddr.pcStreet = pDevCfg->locInfo.civicInfo.A6;
      cctkCfg.net.prot.gen.emgAddr.pcCity = pDevCfg->locInfo.civicInfo.A3;
      cctkCfg.net.prot.gen.emgAddr.pcSubDiv = pDevCfg->locInfo.civicInfo.A1;
      cctkCfg.net.prot.gen.emgAddr.pcCountry = pDevCfg->locInfo.civicInfo.country;
      cctkCfg.net.prot.gen.emgAddr.pcPostal = pDevCfg->locInfo.civicInfo.PC;
      cctkCfg.net.prot.gen.emgAddr.pcLocInfo = pDevCfg->locInfo.civicInfo.LOC;
      cctkCfg.net.prot.gen.emgAddr.pcTimeStamp = pDevCfg->locInfo.civicInfo.civicTimeStamp;
   }
   else
   {
     /*Populate fields for geo location info*/
     cctkCfg.net.prot.gen.emgAddr.bUseGeoLoc = BOS_TRUE;
     cctkCfg.net.prot.gen.emgAddr.pcFLR = NULL;
     if (pDevCfg->locInfo.geoInfo.FLR)
     {
         if (strlen( (char *)pDevCfg->locInfo.geoInfo.FLR ))
         {
           cctkCfg.net.prot.gen.emgAddr.pcFLR = pDevCfg->locInfo.geoInfo.FLR;
         }
     }
     cctkCfg.net.prot.gen.emgAddr.pcPos = pDevCfg->locInfo.geoInfo.pos;
     cctkCfg.net.prot.gen.emgAddr.pcSrsName = pDevCfg->locInfo.geoInfo.srsName;
     cctkCfg.net.prot.gen.emgAddr.pcGeoTimeStamp = pDevCfg->locInfo.geoInfo.geoTimeStamp;
   }

#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_IMS) || (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_RFC3261)
   memset ( (void *) &cctkUaCfg,
            0,
            sizeof ( cctkUaCfg ) );
   cctkGetParm ( CMGR_INVALID_HANDLE,
                 eCCTKPARM_EC_UAIDHDR,
                 (void *) &cctkUaCfg );
   cctkUaCfg.pcSipInstId = uaInstanceId;
   cctkSetParm ( CMGR_INVALID_HANDLE,
                 eCCTKPARM_EC_UAIDHDR,
                 (void *) &cctkUaCfg );
#endif

   memset ( (void *) &cctkLocPortCfg,
            0,
            sizeof ( cctkLocPortCfg ) );

   cctkLocPortCfg.uSipPort = (BOS_UINT16) pDevCfg->uSIPRecvPort;
   cctkSetParm ( CMGR_INVALID_HANDLE,
                 eCCTKPARM_EC_LOCPORT,
                 (void *) &cctkLocPortCfg );

   /* Setup P-Access-Network-info header
   */
   if ( pDevCfg->pcNetworkInfo )
   {
      uLen = strlen ( (const char *)pDevCfg->pcNetworkInfo ) + 1;
      puTmp = (BOS_UINT8 *) malloc ( sizeof ( BOS_UINT8 ) * uLen );
      if ( puTmp )
      {
         memset ( puTmp,
                  0,
                  sizeof ( BOS_UINT8 ) * uLen );

         strcpy ( (char *)puTmp,
                  (char *)pDevCfg->pcNetworkInfo );
      }

      paniCfg.bIncludeNetworkInfo = pDevCfg->bIncludeNetworkInfo;
      paniCfg.pcNetworkInfo = puTmp;

      cctkSetParm ( CMGR_INVALID_HANDLE,
                    eCCTKPARM_EC_PANICFG,
                    (const void *) &paniCfg );

      if ( puTmp )
      {
         free ( puTmp );
         puTmp = NULL;
      }
   }

   if ( pDevCfg->bDevEnabled )
   {
      /* Note: this must be called prior to any other CCTK configuration activity
      **       and after this call is placed, one MUST wait for the callback from
      **       CCTK to decide how to behave next depending on whether this is going
      **       to be successful (then continue with processing) or not (then we are
      **       doomed).
      */
      gbCmCfgCctkConfig = BOS_TRUE;
      cctkConfig ( &cctkCfg );
      gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                            &cfgIdx,
                            (void *) pDevCfg,
                            CFG_ITEM_DEL );

      pDevCfg = NULL;
   }
   else
   {
      CMGRLOG_CRITICAL (( "cmCfgInit: device not enabled." ));
      gbCmCfgCctkConfig = BOS_FALSE;
      gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                            &cfgIdx,
                            (void *) pDevCfg,
                            CFG_ITEM_DEL );
      pDevCfg = NULL;
      return BOS_FALSE;
   }

   return BOS_TRUE;
}

/***********************************************************************************
** FUNCTION:   cmCfgDeinit
**
** PURPOSE:    Deinitializes cmCfg
**
** PARAMETERS: None.
**
** RETURNS:    Nothing
**
***********************************************************************************/
void cmCfgDeinit ( void )
{
   gbCmCfgCctkConfig = BOS_FALSE;
}


/***********************************************************************************
** FUNCTION:   cmCfgIsCctkInit
**
** PURPOSE:    Checks whether the CCTK (network layer) was configured and
**             initialized at any point in time.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if CCTK had been initialized.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgIsCctkInit ( void )
{
   return gbCmCfgCctkConfig;
}


/***********************************************************************************
** FUNCTION:   cmCfgUserAdd
**
** PURPOSE:    Configure a specific CM user.
**
** PARAMETERS: pRes - the user resource to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the user has been allocated its user
**             identification already, which is necessary to configure a user
**             completely.
***********************************************************************************/
void cmCfgUserAdd ( void *pRes )
{
   /* Invoke the user dependent application profile configuration.
   */

#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_IMS)

   /* IMS application.
   */
   cmProfImsUserAdd ( pRes );

#elif (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_STANDALONE)

   /* Standalone application (mostly used for testing).
   */
   cmProfStandUserAdd ( pRes );

#elif (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_CUSTOM)

   /* Custom application (must have defined the actual use).
   */
   cmProfCustUserAdd ( pRes );

#else /* (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_RFC3261) */

   /* RFC 3261 application.  Also the 'default' usage.
   */
   cmProfRfc3261UserAdd ( pRes );

#endif

   /* Default initial feature state of interest.
   */
   {
      CMUSR *pUsr = (CMUSR *) pRes;

      pUsr->bPerCallCW = BOS_TRUE;
      pUsr->bRejectAnonymous = BOS_FALSE;
      pUsr->bDnd = BOS_FALSE;
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
      pUsr->bWarmLineActive = BOS_FALSE;
#endif
   }
}

/***********************************************************************************
** FUNCTION:   cmCctkAccntAdd
**
** PURPOSE:    Configure a specific CCTK account
**
** PARAMETERS: pRes - the CCTK account to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the account has been allocated its user
**             identification already, which is necessary to configure the account
**             completely.
***********************************************************************************/
void cmCctkAccntAdd ( void *pRes )
{
   /* Invoke the user dependent application profile configuration.
   */

#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_IMS)

   /* IMS application.
   */
   cmProfImsCctkAccntAdd ( pRes );

#elif (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_STANDALONE)

   /* Not supported at this time */

#elif (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_CUSTOM)

   /* Not supported at this time */

#else /* (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_RFC3261) */

   /* RFC 3261 application.  Also the 'default' usage.
   */
   cmProfRfc3261CctkAccntAdd ( pRes );

#endif

   /* After all the CCTK account configurations are done, register this account
   ** to the CCTK Account mapping table.
   */
   cmCctkAccntInit( pRes );
}


/***********************************************************************************
** FUNCTION:   cmCfgUser
**
** PURPOSE:    Configure the actual system users.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if we have successfully initialized our users.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgUser ( void )
{
   CFGSIPUSERCFG *pCctkAccntCfg = NULL;
   CFGSIPUSERCFG *pCctkIndAccntCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   CMUSR         *pCmUsr;

#if CMGRCFG_MULTILINE
   CFGEPTSTATUSCFG  *pEptStatusCfg = NULL;
   BOS_UINT32        uIx;
   CMCCTKACCNT      *pCctkAccnt;
#endif

   /* Initialize the user(s) configuration.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = NULL;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) &pCctkAccntCfg,
                              CFG_ITEM_GET ) != BOS_STATUS_OK )
   {
      /* Note that we do not need to apply 'deletion' on the user
      ** concept configuration in this case because we failed to
      ** retrieve it properly in the first place.
      */
      return BOS_FALSE;
   }

   pCctkIndAccntCfg = pCctkAccntCfg;

#if CMGRCFG_MULTILINE
   /* Initialize the CCTK account first */
   while ( pCctkIndAccntCfg != NULL )
   {
      if ( ( pCctkAccnt = cmCctkAccntAlloc() ) != NULL )
      {
         /* Associate this account with a valid identifier, this is the way to tell
         ** that this user is in fact used.
         */
         strncpy ( (char *)pCctkAccnt->uUid, (char *)pCctkIndAccntCfg->pcUserId, CMGR_USER_IDENTIFIER_LEN );
         pCctkAccnt->uHdl = pCctkIndAccntCfg->uIMPUIndex;

         cmCctkAccntAdd( pCctkAccnt );

         /* The actual CCTK Account registration will be done after the account has been
         ** initialized properly.
         */

         CMGRLOG_INFORMATION (( "cmCfgUser: CctkAccnt uUid %s - uHdl 0x%X",
                                pCctkAccnt->uUid, pCctkAccnt->uHdl ));
      }
      else
      {
         CMGRLOG_CRITICAL (( "cmCfgUser: Failed to allocate cctkAccnt block for pcUserId %s",
                              pCctkIndAccntCfg->pcUserId ));
         break;
      }

      /* Increment the user.
      */
      pCctkIndAccntCfg = pCctkIndAccntCfg->pNext;
   }

   /* Initialize the CctkAccnt routing table.  This needs to be done after all
   ** the accounts have been initialized so that the proper mapping can
   ** be created.
   */
   cmCctkUpdateRouting();

   /* Next, initialize the cmgr user */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   for ( uIx = 0; uIx < CMGRCFG_USER_MAX; uIx++ )
   {
      cfgIdx.uIndex = uIx;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, StatusConcept ),
                                 &cfgIdx,
                                 (void *) &pEptStatusCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pEptStatusCfg && pEptStatusCfg->bEndpointEnabled )
         {
            if ( ( pCmUsr = cmUsrAlloc() ) != NULL )
            {
               /* No need to update the uUid for the CM use here.  This will
               ** be done on an assignment basis.
               */

               /* Since there will be a 1-1 relationship between a CM user
               ** and the uPhy, establish this mapping now.
               */
               pCmUsr->uPhys = uIx;

               /* Add the user according to the configuration profile selected.
               */
               cmCfgUserAdd ( (void *) pCmUsr );

               CMGRLOG_INFORMATION (( "cmCfgUser: cmUsrUuid %s - cmUsrUhdl 0x%X, uNet 0x%X",
                                      pCmUsr->uUid, pCmUsr->uHdl,
                                      pCmUsr->uNet ));
            }
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, StatusConcept ),
                               &cfgIdx,
                               (void *) pEptStatusCfg,
                               CFG_ITEM_DEL );
         pEptStatusCfg = NULL;
      }
   }
#else
   while ( pCctkIndAccntCfg != NULL )
   {
      if ( ( pCmUsr = cmUsrAlloc() ) != NULL )
      {
         /* Associate this user with a valid identifier, this is the way to tell
         ** that this user is in fact used.
         */
         strncpy ( (char *)pCmUsr->uUid, (char *)pCctkIndAccntCfg->pcUserId, CMGR_USER_IDENTIFIER_LEN );
         pCmUsr->uUindex = pCctkIndAccntCfg->uIMPUIndex;

         /* Add the user according to the configuration profile selected.
         */
         cmCfgUserAdd ( (void *) pCmUsr );

         CMGRLOG_INFORMATION (( "cmCfgUser: cmUsrUuid %s - cmUsrUhdl 0x%X, uNet 0x%X",
                                pCmUsr->uUid, pCmUsr->uHdl,
                                pCmUsr->uNet ));
      }
      else
      {
         CMGRLOG_CRITICAL (( "cmCfgUser: Failed to allocate cmUsr block for pUsr->pcUserId %s!", pCctkIndAccntCfg->pcUserId ));
      }

      /* Increment the user.
      */
      pCctkIndAccntCfg = pCctkIndAccntCfg->pNext;
   }
#endif

   /* Free the resources allocated.
   */
   gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                         &cfgIdx,
                         (void *) pCctkAccntCfg,
                         CFG_ITEM_DEL );
   pCctkAccntCfg = NULL;

#if CMGRCFG_REMOTE_USER_CONTROL
   {
      /* Add the pre-Config SIP remote user */
      CFGSIPREMOTEUSERCFG *pRemUsrCfg = NULL;
      CFGSIPREMOTEUSERCFG *pRemUsr = NULL;
      CCTKUSRCFG cctkUsrCfg;

      /* Initialize the SIP remote user(s) configuration.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = NULL;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                                 &cfgIdx,
                                 (void *) &pRemUsrCfg,
                                 CFG_ITEM_GET ) != BOS_STATUS_OK )
      {
         /* There is no pre-config SIP remote user, exit and continues. */
         return BOS_TRUE;
      }

      pRemUsr = pRemUsrCfg;
      while ( pRemUsr != NULL )
      {
         CMUSR *pCmUsr;
         BOS_UINT32 strLen;

         /* Grab a user block.
         */
         if ( ( pCmUsr = cmUsrAlloc() ) != NULL )
         {
            /* Associate this user with a valid identifier, this is the way to tell
            ** that this user is in fact used.
            */
            strLen =
               ( ( strlen ( (char *)pRemUsr->pcUserId ) > CMGR_USER_IDENTIFIER_LEN ) ?
                    CMGR_USER_IDENTIFIER_LEN : strlen ( (char *)pRemUsr->pcUserId ) );
            strncpy ( (char *)pCmUsr->uUid, (char *)pRemUsr->pcUserId, strLen );
            pCmUsr->uUindex = pRemUsr->uAssociatedIMPUIndexRef-1;

            pCmUsr->eType = eCMUSRTYPE_REMOTE;


            /* Create the CCTK user.
            */
            memset ( &cctkUsrCfg,
                     0,
                     sizeof ( CCTKUSRCFG ) );

            /* Save the user name of the registered user.
            */
            cctkUsrCfg.usr.net.sipName.pcUsrName = pRemUsr->pcUserId;
            cctkUsrCfg.usr.net.sipName.pcDisplay = pRemUsr->pcDisplayInfo;
            cctkUsrCfg.remUsr.bRemUsr = BOS_TRUE;
            /* Save the address of record of the registered user in order
            ** to be able to target requests to it directly.
            */
            if ( cctkUserAdd ( &pCmUsr->uNet, &cctkUsrCfg ) == eCCTKSTATUS_SUCCESS )
            {
               CCTKREMUSRCFG cfg;
               CMGRLOG_INFORMATION (( "cmCfgUser: remote user '%s' - usr 0x%X, net 0x%X",
                                      pCmUsr->uUid, pCmUsr->uHdl,
                                      pCmUsr->uNet ));

               /* set user specific credential configuration */
               cfg.uIMPIIndexRef = pRemUsr->uIMPIIndexRef;
               cfg.pcIMPIUserId = pRemUsr->pcIMPIUserUserId;
               cfg.pcIMPIPassword = pRemUsr->credentials.pCredVal;

               cctkSetParm ( pCmUsr->uNet,
                             eCCTKPARM_EC_REMUSRCFG,
                             (void *) &cfg );
            }
         }

         /* Increment the user.
         */
         pRemUsr = pRemUsr->pNext;
      }

      /* Free the resources allocated.
      */
      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                            &cfgIdx,
                            (void *) pRemUsrCfg,
                            CFG_ITEM_DEL );
      pRemUsrCfg = NULL;
   }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

   return BOS_TRUE;
}


/***********************************************************************************
** FUNCTION:   cmCfgAudio
**
** PURPOSE:    Configure the audio settings for a given user to be used by the
**             CCTK layer.
**
** PARAMETERS: pRes - the resource to be configured.
**             pSes - session on which to configure audio
**             pCfgAudio - where to store the configuration information.
**
** RETURNS:    BOS_TRUE if we have successfully configured.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgAudio ( void *pRes, CMSES* pSes, CCTKMEDAUD *pCfgAudio )
{
   BOS_BOOL bRet = BOS_TRUE;
   CMUSR *pUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   CFGEPTCODECCFG *pCodecCfg = NULL;
   CFGEPTCODECLIST *pCodecLstCfg = NULL;
   CFGPROVDEVICECFG *pDevCfg = NULL;
   CFGSIPUSERCFG *pUsrCfg = NULL;
   CMCCTKACCNT* pCctkAccnt = NULL;

   if ( pUsr && pCfgAudio )
   {
      memset ( pCfgAudio,
               0,
               sizeof( CCTKMEDAUD ) );

      pCfgAudio->qos.bRtcpXrSupport = BOS_TRUE;

      pCfgAudio->gen.bTelEvt = BOS_TRUE;
      pCfgAudio->gen.uTelEvtTypes = CCTKNTE_DTMF;

      pCfgAudio->gen.rtpPort.uMinPort =
          (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                         pUsr->uHdl * ( CMGR_RTP_PORT_POOL_SIZE * CMGRCFG_MEDSES_MAX ) );
      pCfgAudio->gen.rtpPort.uMaxPort =
          (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                         ( pUsr->uHdl + 1 ) * ( CMGR_RTP_PORT_POOL_SIZE * CMGRCFG_MEDSES_MAX ) - 1 );
      pCfgAudio->gen.rtcpPort.uMinPort =
          (BOS_UINT16) ( CMGR_RTCP_AUD__PORT_OFFSET_START  +
                         pUsr->uHdl * ( CMGR_RTP_PORT_POOL_SIZE * CMGRCFG_MEDSES_MAX ) );
      pCfgAudio->gen.rtcpPort.uMaxPort =
          (BOS_UINT16) ( CMGR_RTCP_AUD__PORT_OFFSET_START +
                         ( pUsr->uHdl + 1 ) * ( CMGR_RTP_PORT_POOL_SIZE * CMGRCFG_MEDSES_MAX ) - 1 );

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      pCctkAccnt = cmCctkAccntFindByNet( pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE );
      if (pCctkAccnt)
      {
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                    &cfgIdx,
                                    (void *) &pUsrCfg,
                                    CFG_ITEM_GET) == BOS_STATUS_OK )
         {
            if ( pUsrCfg )
            {
               switch(pUsrCfg->eSilSupp)
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
               
               switch(pUsrCfg->eSrtpPref)
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
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                  &cfgIdx,
                                  (void *) pUsrCfg,
                                  CFG_ITEM_DEL );
            pUsrCfg = NULL;
         }
      }

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
      cfgIdx.pcCharIdx = pUsr->uUid;
      cfgIdx.uIndex = pUsr->uPhys; /* May be invalid, in such case the configuration
                                   ** should return the default set.
                                   */
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
                     case eCCTKCODEC_G729:
                     {
                        /* For the time being always set annex-b usage, this will
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

                     case eCCTKCODEC_GSMAMR:
                     case eCCTKCODEC_AMRWB:
                     {
                        /* If AMR is part of the capabilities, ensure we set
                        ** the 'maxptime' attribute because it is necessary for
                        ** AMR support.
                        */
                        pCfgAudio->gen.uMaxPtime = CMGR_MAXPTIME_AMR;
                        pCfgAudio->gen.codec[ pCfgAudio->gen.uNumCodec ].parm.amrCfg.uMaxRed = CMGR_MAXRED_AMR;
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

      if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
      {
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.uIndex = pUsr->uPhys;
         cfgIdx.pcCharIdx = pUsr->uUid;
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
                  switch( pCfgAudio->gen.codec[ uIx ].eType )
                  {
                     case eCCTKCODEC_G7231:
                     {
                        pCfgAudio->gen.codec[ uIx ].uPtime = CODEC_PTIME_30;
                     }
                     break;

                     case eCCTKCODEC_AMRWB:
                     case eCCTKCODEC_GSMAMR:
                     {
                        CFGAMRIN *amrCfg = ((pCfgAudio->gen.codec[ uIx ].eType == eCCTKCODEC_AMRWB)
                              ? &(pCodecCfg->amrWbCfg)
                              : &(pCodecCfg->amrNbCfg));
                        pCfgAudio->gen.codec[ uIx ].uPtime = (amrCfg->uPtime ? amrCfg->uPtime : CODEC_PTIME_20);
                        pCfgAudio->gen.codec[ uIx ].parm.amrCfg.bOctetAlign = amrCfg->bOctetAlign;
                        pCfgAudio->gen.codec[ uIx ].parm.amrCfg.uModeSet = amrCfg->uModeSet;
                        pCfgAudio->gen.codec[ uIx ].parm.amrCfg.bModeChangeNeighbor = amrCfg->bModeChangeNeighbor;
                        pCfgAudio->gen.codec[ uIx ].parm.amrCfg.bModeChangePeriod = amrCfg->bModeChangePeriod;
                        pCfgAudio->gen.codec[ uIx ].parm.amrCfg.uMaxRed = amrCfg->uMaxRed;
                     }
                     break;

                     default:
                     {
                        pCfgAudio->gen.codec[ uIx ].uPtime =
                                       (BOS_UINT32) pCodecCfg->sG711PktPeriod;
                     }
                     break;
                  }
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
      }

      bRet = BOS_TRUE;
   }

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmCfgFax
**
** PURPOSE:    Configure the fax settings for a given user to be used by the
**             CCTK layer.
**
** PARAMETERS: pRes - the resource to be configured.
**             pCfgFax - where to store the configuration information.
**             pbEnable - returned BOS_TRUE if fax configuration is supported,
**                        BOS_FALSE otherwise.
**
** RETURNS:    BOS_TRUE if we have successfully configured.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgFax ( void *pRes, CCTKMEDFAX *pCfgFax, BOS_BOOL *pbEnable )
{
   BOS_BOOL bRet = BOS_FALSE;
   CMUSR *pUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   CFGEPTCODECCFG *pCodecCfg = NULL;

   *pbEnable = BOS_TRUE;

   if ( pUsr && pCfgFax )
   {
      memset ( pCfgFax,
               0,
               sizeof( CCTKMEDFAX ) );

      /* Always set the max datagram size to be default, eventually when
      ** the physical interface will support higher fax rate (SG3 @ 33.3 kbps
      ** for example) we would adjust this accordingly.
      */
      pCfgFax->uMaxDatagram = CMGR_T38_MAX_DATAGRAM_DEFAULT;

      if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
      {
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.uIndex = pUsr->uPhys;
         cfgIdx.pcCharIdx = pUsr->uUid;
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
                                     pUsr->uHdl * ( CMGR_RTP_PORT_POOL_SIZE * CMGRCFG_MEDSES_MAX ) );
                  pCfgFax->t38Port.uMaxPort =
                      (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                                     ( pUsr->uHdl + 1 ) * ( CMGR_RTP_PORT_POOL_SIZE * CMGRCFG_MEDSES_MAX ) - 1 );
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
      }
      else
      {
         pCfgFax->t38Port.uMinPort =
             (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                            pUsr->uHdl * ( CMGR_RTP_PORT_POOL_SIZE * CMGRCFG_MEDSES_MAX ) );
         pCfgFax->t38Port.uMaxPort =
             (BOS_UINT16) ( CMGR_RTP_AUD__PORT_OFFSET_START +
                            ( pUsr->uHdl + 1 ) * ( CMGR_RTP_PORT_POOL_SIZE * CMGRCFG_MEDSES_MAX ) - 1 );
      }

      bRet = BOS_TRUE;
   }

   return bRet;
}


#if CMGRCFG_VIDEO_MEDIA
/***********************************************************************************
** FUNCTION:   cmCfgVideo
**
** PURPOSE:    Configure the video settings for a given user to be used by the
**             CCTK layer.
**
** PARAMETERS: pRes - the resource to be configured.
**             pCfgVideo - where to store the configuration information.
**
** RETURNS:    BOS_TRUE if we have successfully configured.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmCfgVideo ( void *pRes, CCTKMEDVID *pCfgVideo )
{
   BOS_BOOL bRet = BOS_FALSE;
   CMUSR *pUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   CFGEPTCODECVIDLIST *pCodecLstCfg = NULL;
   CFGEPTCODECVIDCFG *pCodecCfg = NULL;

   if ( pUsr && pCfgVideo )
   {
      memset ( pCfgVideo,
               0,
               sizeof( CCTKMEDVID ) );

      pCfgVideo->gen.rtpPort.uMinPort =
          (BOS_UINT16) ( CMGR_RTP_VID__PORT_OFFSET_START +
                         pUsr->uHdl * ( 2 * CMGRCFG_MEDSES_MAX ) +
                         pUsr->uHdl );
      pCfgVideo->gen.rtpPort.uMaxPort =
          (BOS_UINT16) ( CMGR_RTP_VID__PORT_OFFSET_START +
                         ( pUsr->uHdl + 1 ) * ( 2 * CMGRCFG_MEDSES_MAX ) +
                         pUsr->uHdl );
      pCfgVideo->gen.rtcpPort.uMinPort =
          (BOS_UINT16) ( CMGR_RTCP_VID__PORT_OFFSET_START  +
                         pUsr->uHdl * ( 2 * CMGRCFG_MEDSES_MAX ) +
                         pUsr->uHdl );
      pCfgVideo->gen.rtcpPort.uMaxPort =
          (BOS_UINT16) ( CMGR_RTCP_VID__PORT_OFFSET_START +
                         ( pUsr->uHdl + 1 ) * ( 2 * CMGRCFG_MEDSES_MAX ) +
                         pUsr->uHdl );

      /* Setup the supported codec.
      */
      pCfgVideo->gen.uNumCodec = 0;
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      cfgIdx.uIndex = pUsr->uPhys; /* May be invalid, in such case the configuration
                                   ** should return the default set.
                                   */
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecVidList ),
                                 &cfgIdx,
                                 (void *) &pCodecLstCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pCodecLstCfg )
         {
            CFGEPTCODECVIDLIST *pCodecLstCfgTmp = pCodecLstCfg;

            /* Loop through the codec and allocate those we
            ** can negotiate (i.e. know about and understand).
            */
            do
            {
               pCfgVideo->gen.codec[ pCfgVideo->gen.uNumCodec ].eType =
                  cmMapGetId ( cmMapCfgCodec2CctkCodec,
                               pCodecLstCfgTmp->codecType );

               /* If the codec retrieved is valid, increment, otherwise
               ** ignore it.
               */
               if ( pCfgVideo->gen.codec[ pCfgVideo->gen.uNumCodec ].eType !=
                                                                  eCCTKCODEC_MAX )
               {
                  /* Check whether there are per-codec specific data to be
                  ** set.
                  */
                  switch ( pCfgVideo->gen.codec[ pCfgVideo->gen.uNumCodec ].eType )
                  {
                     case eCCTKCODEC_H263:
                     {
                        pCfgVideo->gen.codec[
                              pCfgVideo->gen.uNumCodec ].parm.pH263 = NULL;

                        if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
                        {
                           memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                           cfgIdx.uIndex = pUsr->uPhys;
                           if ( gCmCfgCb.cmgrProvCb (
                                    CFGITEM( EPT, CodecVidConcept ),
                                    &cfgIdx,
                                    (void *) &pCodecCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
                           {
                              if ( pCodecCfg )
                              {
                                 pCfgVideo->gen.codec[
                                      pCfgVideo->gen.uNumCodec ].parm.pH263 =
                                         (CCTKH263PARM *) malloc (
                                                  sizeof ( CCTKH263PARM ) );
                                 if ( pCfgVideo->gen.codec[
                                             pCfgVideo->gen.uNumCodec ].parm.pH263 )
                                 {
                                    CCTKH263PARM *pH263 = pCfgVideo->gen.codec[
                                             pCfgVideo->gen.uNumCodec ].parm.pH263;

                                    memset ( pH263, 0, sizeof ( CCTKH263PARM ) );

                                    pH263->bIsSpecified = BOS_TRUE;
                                    pH263->uProfile = pCodecCfg->uH263Profile;
                                    pH263->uLevel = pCodecCfg->uH263Level;
                                 }

                                 gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                                       &cfgIdx,
                                                       (void *) pCodecCfg,
                                                       CFG_ITEM_DEL );
                                 pCodecCfg = NULL;
                              }
                           }
                        }
                     }
                     break;

                     case eCCTKCODEC_H264:
                     {
                        pCfgVideo->gen.codec[
                             pCfgVideo->gen.uNumCodec ].parm.pH264 = NULL;

                        if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
                        {
                           memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                           cfgIdx.uIndex = pUsr->uPhys;
                           if ( gCmCfgCb.cmgrProvCb (
                                    CFGITEM( EPT, CodecVidConcept ),
                                    &cfgIdx,
                                    (void *) &pCodecCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
                           {
                              if ( pCodecCfg )
                              {
                                 pCfgVideo->gen.codec[
                                      pCfgVideo->gen.uNumCodec ].parm.pH264 =
                                         (CCTKH264PARM *) malloc (
                                                  sizeof ( CCTKH264PARM ) );
                                 if ( pCfgVideo->gen.codec[
                                             pCfgVideo->gen.uNumCodec ].parm.pH264 )
                                 {
                                    CCTKH264PARM *pH264 = pCfgVideo->gen.codec[
                                             pCfgVideo->gen.uNumCodec ].parm.pH264;

                                    /* Note: we do not support/allow any prop
                                    **       parameters at this time.
                                    */
                                    memset ( pH264, 0, sizeof ( CCTKH264PARM ) );

                                    pH264->bIsSpecified = BOS_TRUE;
                                    pH264->eProfile =
                                       cmMapGetId ( cmMapCfgH264Prof2CctkH264Prof,
                                                    pCodecCfg->uH264Profile );
                                    switch ( pH264->eProfile )
                                    {
                                       case eCCTKH264PROFILE_BASELINE:
                                       {
                                          pH264->bConstraintSet0 = BOS_TRUE;
                                       }
                                       break;

                                       case eCCTKH264PROFILE_MAIN:
                                       {
                                          pH264->bConstraintSet1 = BOS_TRUE;
                                       }
                                       break;

                                       case eCCTKH264PROFILE_EXTENDED:
                                       {
                                          pH264->bConstraintSet2 = BOS_TRUE;
                                       }
                                       break;

                                       default:
                                       break;
                                    }
                                    pH264->ePacketizationMode =
                                       cmMapGetId ( cmMapCfgH264PktMode2CctkH264PktMode,
                                                    pCodecCfg->uH264PktMode );
                                    pH264->eLevel =
                                       cmMapGetId ( cmMapCfgH264Lvl2CctkH264Lvl,
                                                    pCodecCfg->uH264Level );
                                    pH264->uMaxBr = pCodecCfg->uH264MaxBr;
                                    pH264->uMaxMbps = pCodecCfg->uH264MaxMbps;
                                 }

                                 gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                                       &cfgIdx,
                                                       (void *) pCodecCfg,
                                                       CFG_ITEM_DEL );
                                 pCodecCfg = NULL;
                              }
                           }
                        }
                     }
                     break;

                     default:
                     break;
                  }

                  ++pCfgVideo->gen.uNumCodec;
               }

               pCodecLstCfgTmp = pCodecLstCfgTmp->pNext;
            }
            while ( pCodecLstCfgTmp != NULL );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecVidList ),
                               &cfgIdx,
                               (void *) pCodecLstCfg,
                               CFG_ITEM_DEL );
         pCodecLstCfg = NULL;
      }

      bRet = BOS_TRUE;
   }

   return bRet;
}
#endif


#if CMGRCFG_TLS

/***********************************************************************************
** FUNCTION:   cmCfgTLS
**
** PURPOSE:    Configure the TLS Configuration.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if we have successfully configured TLS.
**             BOS_FALSE otherwise.
**
** NOTES:      - This function only configures the CCTK and need to call cctkConfig()
**             - to propagate the configuration into the SIP stack.
**
***********************************************************************************/
BOS_BOOL cmCfgTLS ( void )
{
   CFGSECTLSCFG *pTLSCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   CCTKTLSCFG cctkTLSCfg;

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );

   /* Initialize the TLS configuration.
   */
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, SecConcept ),
                              &cfgIdx,
                              (void *) &pTLSCfg,
                              CFG_ITEM_GET ) != BOS_STATUS_OK )
   {
      /* Note that we do not need to apply 'deletion' on the security
      ** concept configuration in this case because we failed to
      ** retrieve it properly in the first place.
      */
      return BOS_FALSE;
   }

   cctkTLSCfg.ppcCertChain          = pTLSCfg->ppcCertChain;
   cctkTLSCfg.ppcTrustedCerts       = pTLSCfg->ppcTrustedCerts;
   cctkTLSCfg.pcPrivKey             = pTLSCfg->pcPrivKey;
   cctkTLSCfg.pcCipher              = pTLSCfg->pcCipher;
   cctkTLSCfg.bClientAuth           = pTLSCfg->bClientAuth;
   cctkTLSCfg.bServerAuth           = pTLSCfg->bServerAuth;
   cctkTLSCfg.preGenDhPrime.puPrime = pTLSCfg->preGenDhPrime.puPrime;
   cctkTLSCfg.preGenDhPrime.uLen    = pTLSCfg->preGenDhPrime.uLen;

   cctkSetParm ( CMGR_INVALID_HANDLE,
                 eCCTKPARM_EC_TLSCFG,
                 (void *) &cctkTLSCfg );

   gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, SecConcept ),
                         &cfgIdx,
                         (void *) pTLSCfg,
                         CFG_ITEM_DEL );
   pTLSCfg = NULL;

   return BOS_TRUE;
}

#endif /* CMGRCFG_TLS */


/***********************************************************************************
** FUNCTION:   cmCfgInitCallMedia
**
** PURPOSE:    Configure the media information to be used by the CCTK during
**             call negotiation.  This is done before any outgoing call as well
**             as before accepting any incoming call.  This gives the most up to
**             date and accurate view of the media capabilities.
**
** PARAMETERS: pRes - the resource to be configured.
**             pSes - the session to be configured.
**
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmCfgInitCallMedia ( void *pRes, CMSES *pSes )
{
   CCTKMEDCFG cctkMedCfg;
   CCTKMEDAUD cctkMedAud;
   CCTKMEDFAX cctkMedFax;
#if CMGRCFG_VIDEO_MEDIA
   CCTKMEDVID cctkMedVid;
#endif
   CCTKMEDLST cctkMedLst;
   CCTKHOLDCFG cctkHoldCfg;
   CCTKMEDNEGCFG cctkMedNegCfg;
   BOS_BOOL bEnable = BOS_TRUE;
   BOS_BOOL bHeldMedia = BOS_FALSE;
   PROV_CFG_IDX cfgIdx;
   CFGPROVDEVICECFG *pDevCfg = NULL;
   CFGEPTCODECCFG *pCodecCfg = NULL;
   CFGSIPHELDMEDIA *pHeldCfg = NULL;
   BOS_UINT32 uIx;
   CMUSR *pUsr = (CMUSR *) pRes;

   memset ( (void *) &cctkMedCfg,
            0,
            sizeof ( cctkMedCfg ) );

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = NULL;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                              &cfgIdx,
                              (void *) &pDevCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pDevCfg )
      {
         memcpy ( &cctkMedCfg.ipaddr,
                  &pDevCfg->IpAddress,
                  sizeof ( BOS_IP_ADDRESS ) );

         cctkMedCfg.pcSesName = pDevCfg->pcDevSesIdent;
         cctkMedCfg.sNtpTimeAdj = pDevCfg->sNtpTimeAdj;
      }
   }

   cmCfgAudio ( pRes, pSes, &cctkMedAud );
   cmCfgFax ( pRes, &cctkMedFax, &bEnable );
#if CMGRCFG_VIDEO_MEDIA
   cmCfgVideo ( pRes, &cctkMedVid );
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
      cctkMedLst.audCodeLst[ uIx ] =
                        cctkMedAud.gen.codec[ uIx ].eType;
   }
#if CMGRCFG_VIDEO_MEDIA
   cctkMedLst.mMediaTypes |= CCTK_MEDIA_VIDEO_MASK;
   cctkMedLst.uNumVidCodec = cctkMedVid.gen.uNumCodec;
   for ( uIx = 0 ; uIx < cctkMedLst.uNumVidCodec ; uIx++ )
   {
      cctkMedLst.vidCodeLst[ uIx ] = cctkMedVid.gen.codec[ uIx ].eType;
   }
#endif

   cctkMedCfg.pAudio = &cctkMedAud;
   cctkMedCfg.pFax = ( bEnable ? &cctkMedFax : NULL );
#if CMGRCFG_VIDEO_MEDIA
   cctkMedCfg.pVideo = &cctkMedVid;
#endif
   cctkMedCfg.pMediaLst = &cctkMedLst;

   cctkSetParm ( pSes ? pSes->uNet : CMGR_INVALID_HANDLE,
                 eCCTKPARM_MC_MEDIACFG,
                 (void *) &cctkMedCfg );

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

   if ( pDevCfg )
   {
      gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                            &cfgIdx,
                            (void *) pDevCfg,
                            CFG_ITEM_DEL );
      pDevCfg = NULL;
   }

   /* There may be a need to overwrite some of the default settings
   ** used for negotiation during this call.
   */
   if ( cctkGetParm ( pSes ? pSes->uNet : CMGR_INVALID_HANDLE,
                      eCCTKPARM_EC_MEDNEGCFG,
                      (void *) &cctkMedNegCfg ) == eCCTKSTATUS_SUCCESS )
   {
      BOS_BOOL bUpdate = BOS_FALSE;

      if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
      {
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.uIndex = pUsr->uPhys;
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                    &cfgIdx,
                                    (void *) &pCodecCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pCodecCfg &&
                 pCodecCfg->uTelEvtPayloadNum &&
                 ( pCodecCfg->uTelEvtPayloadNum != cctkMedNegCfg.uTelEvtRtpCode ) )
            {
               cctkMedNegCfg.uTelEvtRtpCode = pCodecCfg->uTelEvtPayloadNum;
               bUpdate = BOS_TRUE;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                  &cfgIdx,
                                  (void *) pCodecCfg,
                                  CFG_ITEM_DEL );
            pCodecCfg = NULL;
         }
      }

      if ( bUpdate )
      {
         cctkSetParm ( pSes ? pSes->uNet : CMGR_INVALID_HANDLE,
                       eCCTKPARM_EC_MEDNEGCFG,
                       (void *) &cctkMedNegCfg );
      }
   }

   /* Setup the held media configuration (local vs. network) for this
   ** session according to our knowledge.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HeldMediaConcept ),
                              &cfgIdx,
                              (void *) &pHeldCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pHeldCfg && pHeldCfg->Act.bCfgStatus )
      {
         bHeldMedia = pHeldCfg->bHeldMediaEnabled;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HeldMediaConcept ),
                            &cfgIdx,
                            (void *) pHeldCfg,
                            CFG_ITEM_DEL );
      pHeldCfg = NULL;
   }

   if ( cctkGetParm ( pSes ? pSes->uNet : CMGR_INVALID_HANDLE,
                      eCCTKPARM_EC_CALLHOLD,
                      (void *) &cctkHoldCfg ) == eCCTKSTATUS_SUCCESS )
   {
      if ( bHeldMedia != cctkHoldCfg.bLocalHold )
      {
         cctkHoldCfg.bLocalHold = bHeldMedia;
         cctkSetParm ( pSes ? pSes->uNet : CMGR_INVALID_HANDLE,
                       eCCTKPARM_EC_CALLHOLD,
                       (void *) &cctkHoldCfg );
      }
   }
}

#if CMGRCFG_REMOTE_USER_CONTROL
/***********************************************************************************
** FUNCTION:   cmCfgRemUsrSys
**
** PURPOSE:    Configure the SIP remote user system configuration.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if we have successfully configured.
**             BOS_FALSE otherwise.
**
** NOTES:      - This function only configures the CCTK and need to call cctkSetParam()
**             - to propagate the configuration into the SIP stack.
**
***********************************************************************************/
BOS_BOOL cmCfgRemUsrSys ( void )
{
   CFGSIPREMOTEUSERSYSCFG *pCfg = NULL;
   CCTKREMUSRSYS cctkRemUsrSys;
   /* Get current configuration.
   */
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                              NULL,
                              (void *) &pCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      /* update the CCTK - SIP remote user related parameters */
      cctkRemUsrSys.bRemUsrSupport = pCfg->bEnable;
      cctkRemUsrSys.eAdmMode = (CCTKREMUSRADMMODE) pCfg->sAdmissionMode;
      cctkRemUsrSys.uDftAssIMPUIndexRef = pCfg->uDftAssociatedIMPUIndexRef;
      cctkRemUsrSys.pcDftAssIMPUUserId = pCfg->pcDftAssociatedUserId;
      cctkRemUsrSys.uDftIMPIIndexRef = pCfg->uDftIMPIIndexRef;
      cctkRemUsrSys.pcDftIMPIUserId = pCfg->pcDftIMPIUserUserId;
      cctkRemUsrSys.pcDftIMPIUserPassword = pCfg->dftCredentials.pCredVal;

      cctkSetParm ( CMGR_INVALID_HANDLE,
                    eCCTKPARM_EC_REMUSRSYS,
                    (void *) &cctkRemUsrSys );

      gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                            NULL,
                            (void *) pCfg,
                            CFG_ITEM_DEL );
      pCfg = NULL;
   }

   return BOS_TRUE;
}
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

