/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom Corporation
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
**      This file contains the Call Manager account processing functionality.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
#if CMGRCFG_MULTILINE
static const CMFSMVOIDFUNCTIONPTR
   gpCmAccntEvtHdlr[eCMEVT_CCTK_ACCNT_END - eCMEVT_CCTK_ACCNT_START - 1] =
{
   /* ---- eCMEVT_ACCNT_OUT_OF_SERVICE ------------ */ &cmAccntOutOfSrv,
   /* ---- eCMEVT_ACCNT_IN_SERVICE ---------------- */ &cmAccntInSrv,
   /* ---- eCMEVT_ACCNT_TARGETS_UPDATE ------------ */ &cmAccntTargetsUpdate,
   /* ---- eCMEVT_ACCNT_PUBLISH_TARGET ------------ */ &cmAccntPublishTarget,
   /* ---- eCMEVT_ACCNT_SUB_STATUS ---------------- */ &cmAccntSubInfo,
   /* ---- eCMEVT_ACCNT_UPDATE -------------------- */ &cmAccntUpdate,
   /* ---- eCMEVT_ACCNT_PAU_UPDATE ---------------- */ &cmAccntPauUpdate,
   /* ---- eCMEVT_ACCNT_CDR_INFO ------------------ */ &cmAccntCdrInfo,
};
#endif

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmAccntEventHandler
**
** PURPOSE:    Main entry for managing the account events.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntEventHandler ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;

   if ( pCctkAccnt )
   {

      BOS_UINT32 uEvt = pCctkAccnt->evt.uEvt;
#if CMGRCFG_MULTILINE
      CMGRLOG_INFORMATION (( "cmAccntEventHandler: accnt hdl 0x%X , uNet 0x%X, evt [%s (0x%X)]",
                          pCctkAccnt->uHdl,
                          pCctkAccnt->uNet,
                          (char *) cmMapGetStr ( cmMapCmEvent, uEvt ), uEvt ));

      if ( uEvt != eCMEVT_INVALID )
      {
         (gpCmAccntEvtHdlr [ uEvt - (eCMEVT_CCTK_ACCNT_START + 1) ]) ( pRes );
      }
#else
      /* We shouldn't really be here if multiline is disabled but we'll keep this as a precaution.
      */
      CMGRLOG_WARNING (( "cmAccntEventHandler: Multiline disabled, ignore evt (0x%X), accnt hdl 0x%X, uNet 0x%X",
                             uEvt,
                             pCctkAccnt->uHdl,
                             pCctkAccnt->uNet));
#endif
   }
}

/***********************************************************************************
** FUNCTION:   cmAccntOutOfSrv
**
** PURPOSE:    Account out-of-service handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntOutOfSrv ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   CFGPROVDEVICECFG *pDevCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bReportOperNok = BOS_TRUE;
   CFGSIPUSERCFG *pCctkAccntCfg = NULL;
   CCTKEXPIREINFO expInfo;
   BOS_UINT32 uTime = 0;

   CMGRLOG_WARNING (( "cmAccntOutOfSrv: accnt hdl 0x%X, net 0x%X!!!",
                       pCctkAccnt->uHdl, pCctkAccnt->uNet ));

   /* User is out of service (lost registration).
   */
   pCctkAccnt->bInSrv = BOS_FALSE;

   /* Determine how the out-of-service event should be handled.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                              &cfgIdx,
                              (void *) &pDevCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pDevCfg )
      {
         /* Check to see whether the user can always be reported as
         ** non-operational when out of service. For special cases
         ** (such as 403 failures) this flag will be overwritten regardless.
         */
         if ( pDevCfg->bOperNokIffNoTgtsAvail )
         {
            bReportOperNok = BOS_FALSE;
         }
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                            &cfgIdx,
                            (void *) pDevCfg,
                            CFG_ITEM_DEL );
      pDevCfg = NULL;
   }

   cctkGetParm ( pCctkAccnt->uNet, eCCTKPARM_EXPINFO, (void *)&expInfo );

   /* Update the re-reg time as an absolute time
   ** instead of an interval from expInfo.tUpdateTime
   */
   if ( expInfo.tUpdateTime != (time_t)(-1))
   {
      uTime = (BOS_UINT32)expInfo.tUpdateTime;
   }

   if ( gCmCfgCb.cmgrEvtCb )
   {
      CMGREVT eEvent = eCMGREVT_OPER_NOK;

      /* Determine the cause of the out-of-service and handle it accordingly.
      */
      if(pCctkAccnt->evt.pDat3)
      {
         CMGRLOG_INFORMATION (( "cmAccntOutOfSrv: accnt hdl 0x%X, net 0x%X pCctkAccnt->evt.pDat3=0x%X",
                       pCctkAccnt->uHdl, pCctkAccnt->uNet, *(CCTKRESPCODE *)pCctkAccnt->evt.pDat3));

         if (*(CCTKRESPCODE *)pCctkAccnt->evt.pDat3 == eCCTKRESPCODE_FORBIDDEN)
         {
            /*
            **  Send event to indicate if out of service due to 403 forbidden
            */
             eEvent = eCMGREVT_OPER_NOK_FORBIDDEN;
             bReportOperNok = BOS_TRUE;
         }
         else if (*(CCTKRESPCODE *)pCctkAccnt->evt.pDat3 == eCCTKRESPCODE_ALLREGTARGETSFAILED)
         {
            bReportOperNok = BOS_TRUE;
         }
      }

      if ( pCctkAccnt->eCfgAction == eCMCFGACT_CFGUSRTERM )
      {
         /* We had a configuration action to terminate this user.
         ** Report the out of service*/
         bReportOperNok = BOS_TRUE;
      }

      /* Report the user as non operational if the event relates to any special
      ** cases or if configuration allows all reg failures to be reported.
      ** Otherwise the user will be out-of-service but still operational.
      */
      if ( bReportOperNok )
      {
         CMGROOSINFO oosInfo;
         oosInfo.uReRegTime = uTime + expInfo.uReReg;
         memcpy(oosInfo.uUid, pCctkAccnt->uUid, sizeof(pCctkAccnt->uUid));

         ( gCmCfgCb.cmgrEvtCb ) ( eEvent,
                                  pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                                  CMGR_INVALID_HANDLE,
                                  pCctkAccnt->uHdl,
#endif
                                  (void *) &oosInfo );
      }
   }

   /* Update registration info
   */
   if (!bReportOperNok)
   {
      /* update the registration info if it isn't already
      ** passed to the UserConcept
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pCctkAccnt->uUid;
      if( gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) &pCctkAccntCfg,
                               CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         pCctkAccntCfg->uExp = 0;
         pCctkAccntCfg->uReReg = uTime + expInfo.uReReg;
         gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) pCctkAccntCfg,
                              CFG_ITEM_SET );
         gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) pCctkAccntCfg,
                              CFG_ITEM_DEL );
         pCctkAccntCfg = NULL;
      }
   }

   /* TO-DO!!!!!
   ** Fix me when each non-idle CM user has an assigned account:
   ** 1) Transient session - as well as possibly active ones depending on the
   ** desired configured behavior - for this user should be terminated now.
   **
   ** 2) One exception - unless instructed to terminate all resources - is if
   ** we have received an incoming call for this user and we consider that
   ** it is valid scenario to stay in service.
   ** 3) Revert the out-of-service decision if needed (there is an incoming
   ** call from the network, we assume we are in service even if registration
   ** says otherwise).
   ** 4) Update the states of all CM users who released sessions due to this account
   ** going out of service.
   */

   /* Finally, if we were waiting for such event to finalize a reconfiguration
   ** of the account, apply such now.
   */
   if ( pCctkAccnt->eCfgAction != eCMCFGACT_NOACTION )
   {
      cmCctkAccntReConfig ( pCctkAccnt );
   }
}


/***********************************************************************************
** FUNCTION:   cmAccntInSrv
**
** PURPOSE:    Account in-service handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntInSrv ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   CCTKKAINFO *pKaInfo = NULL;
   PROV_CFG_IDX cfgIdx;
   CFGSIPUSERCFG *pCctkAccntCfg = NULL;

   CMGRLOG_INFORMATION (( "cmAccntInSrv: accnt hdl 0x%X, net 0x%X",
                          pCctkAccnt->uHdl, pCctkAccnt->uNet ));

   /* Validate that the network user handle is the one currently assigned
   ** to the account.  If it is not, ignore the event and warn the user.
   ** If it is, continue with processing.  This check is in place because
   ** an 'in-service' event may be triggered at the same time as an 'update'
   ** event from the network layer but in the end at this level we want to
   ** process only the event that is associated with the correct account owning
   ** the resource.
   */
   if ( pCctkAccnt->uNet != pCctkAccnt->evt.uDat2 )
   {
      CMGRLOG_INFORMATION ((
         "cmAccntInSrv [IGNORE]: accnt hdl 0x%X - (net 0x%X != evt-net 0x%X)",
         pCctkAccnt->uHdl, pCctkAccnt->uNet, pCctkAccnt->evt.uDat2 ));

      /* Exit now, the event does not need to be processed such that no
      ** false information is being reported.
      */
      return;
   }

   /* User is in service.
   */
   pCctkAccnt->bInSrv = BOS_TRUE;

   if ( gCmCfgCb.cmgrEvtCb )
   {
      ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_OK,
                               pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                               CMGR_INVALID_HANDLE,
                               pCctkAccnt->uHdl,
#endif
                               (void *) pCctkAccnt->uUid );
   }

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pCctkAccnt->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) &pCctkAccntCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      /* If we have received extra information with this event, this is
      ** some data we need to populate the configuration dynamic information.
      */
      if ( pCctkAccnt->evt.pDat3 &&
         ( pKaInfo = (CCTKKAINFO *) pCctkAccnt->evt.pDat3 ) != NULL )
      {
         if ( pCctkAccntCfg && pCctkAccntCfg->Act.bCfgStatus )
         {
            CFGSIPPCSCF *pPcscfCfg;
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pCctkAccntCfg->pcActiveDomain;
            cfgIdx.uIndex = pCctkAccntCfg->uActiveDomain;
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                  &cfgIdx,
                                  (void *) &pPcscfCfg,
                                  CFG_ITEM_GET );

            pPcscfCfg->sProtIdUsed = ( pKaInfo->bDatagram ? CFG_SIP_PROT_ID_UDP :
                                     CFG_SIP_PROT_ID_TCP );
            bosIpAddrCreateFromStr( (const char *)pKaInfo->uKaPeer,
                                    &pPcscfCfg->PCSCFAddrUsed );

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                  &cfgIdx,
                                  (void *) pPcscfCfg,
                                  CFG_ITEM_SET );
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                      &cfgIdx,
                      (void *) pPcscfCfg,
                      CFG_ITEM_DEL );

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );

         }
      }

      {
         /* Get the registration expiration information
         ** and pass it to user configuration */
         CCTKEXPIREINFO expInfo;
         cctkGetParm ( pCctkAccnt->uNet, eCCTKPARM_EXPINFO, (void *)&expInfo );

         /* Update the registration expiry and re-reg info as an absolute time
         ** instead of an interval from expInfo.tUpdateTime
         */
         BOS_UINT32 uTime = 0;
         if ( expInfo.tUpdateTime != (time_t)(-1))
         {
            uTime = ( BOS_UINT32 )expInfo.tUpdateTime;
         }
         pCctkAccntCfg->uExp = uTime + expInfo.uExp;
         pCctkAccntCfg->uReReg = uTime + expInfo.uReReg;
         pCctkAccntCfg->bPrimary = expInfo.bPrimary;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) pCctkAccntCfg,
                              CFG_ITEM_SET );
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pCctkAccntCfg,
                               CFG_ITEM_DEL );
      }
      pCctkAccntCfg = NULL;
   }


   /* TO-DO!!!!!
   ** Fix me when each non-idle CM user has an assigned account:
   ** Determine if any of the CM users that are using this account are off-hook
   ** when the in-service event is processed. If so, and if configured to play dialtone when
   ** offhook and in-service, apply dialtone and put the CM user in a state to make calls.
   */

}


/***********************************************************************************
** FUNCTION:   cmAccntTargetsUpdate
**
** PURPOSE:    Account information that the target has been updated.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
**
** NOTE:       None.
***********************************************************************************/
void cmAccntTargetsUpdate ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   CCTKTARGETINFO *pTargetInfo;
   CMGRTARGETINFO  cmgrTargetInfo;
   int             i;

   if ( ( pTargetInfo = (CCTKTARGETINFO *) pCctkAccnt->evt.pDat3 ) != NULL )
   {
      if ( gCmCfgCb.cmgrEvtCb )
      {
         /* Copy over targets and report to application */
         cmgrTargetInfo.uNumTargets = ( pTargetInfo->uNumTargets > CMGRCFG_TARGETS_MAX ) ?
                                       CMGRCFG_TARGETS_MAX : pTargetInfo->uNumTargets;

         for ( i = 0; i < cmgrTargetInfo.uNumTargets; i++ )
         {
            cmgrTargetInfo.uTargetPort[i] = pTargetInfo->uTargetPort[i];
            cmgrTargetInfo.targetipAddr[i] = pTargetInfo->targetipAddr[i];
         }

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_TARGETS_UPDATE,
                                  pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                                  CMGR_INVALID_HANDLE,
                                  pCctkAccnt->uHdl,
#endif
                                  &cmgrTargetInfo);
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmAccntPublishTarget
**
** PURPOSE:    Account publish target handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmAccntPublishTarget ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   BOS_IP_ADDRESS *pPublishTarget;

   /* Always inform the application of the recent publish target */
   if ( ( pPublishTarget = (BOS_IP_ADDRESS *) pCctkAccnt->evt.pDat3 ) != NULL )
   {
      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_PUBLISH_TARGET,
                                  pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                                  CMGR_INVALID_HANDLE,
                                  pCctkAccnt->uHdl,
#endif
                                  pPublishTarget );
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmAccntSubInfo
**
** PURPOSE:    Account subscription info handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmAccntSubInfo ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   BOS_BOOL bSubEnabled = *(BOS_BOOL *) pCctkAccnt->evt.pDat3;

   if ( gCmCfgCb.cmgrEvtCb )
   {
      ( gCmCfgCb.cmgrEvtCb ) ( bSubEnabled ? eCMGREVT_SUB_OK : eCMGREVT_SUB_NOK,
                               pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                               CMGR_INVALID_HANDLE,
                               pCctkAccnt->uHdl,
#endif
                              (void *) pCctkAccnt->uUid);
   }

}

/***********************************************************************************
** FUNCTION:   cmAccntUpdate
**
** PURPOSE:    Account information update handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntUpdate ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   BOS_UINT32 uCurrNetHdl = pCctkAccnt->uNet;
   BOS_UINT32 *pNewNetHdl;

   if ( pCctkAccnt->evt.pDat3 &&
        ( pNewNetHdl = (BOS_UINT32 *) pCctkAccnt->evt.pDat3 ) != NULL )
   {
      /* Re-assign the network (CCTK) handle, note that following this action,
      ** any event coming from CCTK refering to the previous network handle would
      ** be ignored, this action is to be executed with care and as directed
      ** by the CCTK layer only.
      */
      CMGRLOG_INFORMATION (( "cmAccntUpdate: Accnt hdl 0x%X, net 0x%X -> net 0x%X",
                             pCctkAccnt->uHdl,
                             uCurrNetHdl,
                             *pNewNetHdl ));

      /* If there are sessions that need to be updated with new network handle
      ** do so now.
      */
      if ( cmNumSesWithNet( uCurrNetHdl ) > 0 )
      {
            CMSESLIST sesList;
            BOS_UINT32 i;

            memset( &sesList, 0, sizeof( CMSESLIST ) );

            cmSesListWithNet( uCurrNetHdl, &sesList );
            for( i = 0 ; i < sesList.uLength ; i++ )
            {
               CMSES *pSes = cmSesValid( sesList.uSes[ i ] );

               if( pSes )
               {
                  pSes->uNetUsr = *pNewNetHdl;
               }
            }
      }

      /* Update the account
      */
      cmCctkAccntUpdateNet( pCctkAccnt, *pNewNetHdl );

      /* TO-DO!!!!!
      ** Fix me when each non-idle CM user has an assigned account:
      ** For each CM user currently using this account, update the account hdl if needed
      */

   }
}

/***********************************************************************************
** FUNCTION:   cmAccntPauUpdate
**
** PURPOSE:    Account P-Associated-URI update handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntPauUpdate ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   PROV_CFG_IDX cfgIdx;
   CCTKPAUINFO *pPAURIs = NULL;
   CFGSIPUSERCFG *pCctkAccntCfg = NULL;

   if ( pCctkAccnt->evt.pDat3 &&
        ( pPAURIs = (CCTKPAUINFO *) pCctkAccnt->evt.pDat3 ) != NULL )
   {
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pCctkAccnt->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                 &cfgIdx,
                                 (void *) &pCctkAccntCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pCctkAccntCfg && pCctkAccntCfg->pcPAURIs )
         {
            /* remember the already malloced buffer so we can delete it later */
            BOS_UINT8 *pPAUtmp = pCctkAccntCfg->pcPAURIs;
            /* this will have the effect of forcing the pau string
            ** to be copied to the mib */
            pCctkAccntCfg->pcPAURIs = pPAURIs->pau;
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                  &cfgIdx,
                                  (void *) pCctkAccntCfg,
                                  CFG_ITEM_SET );
            /* restore the original pointer so the original buffer gets deleted */
            pCctkAccntCfg->pcPAURIs = pPAUtmp;
         }
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pCctkAccntCfg,
                               CFG_ITEM_DEL );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmAccntCdrInfo
**
** PURPOSE:    Account CDR info handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmAccntCdrInfo ( void *pRes )
{
   CMCCTKACCNT *pCctkAccnt = (CMCCTKACCNT *) pRes;
   CCTKCDRINFO *pCdrInfo = NULL;
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bClidName = BOS_TRUE;
   BOS_BOOL bClidNumber = BOS_TRUE;

   if ( pCctkAccnt->evt.pDat3 &&
        ( pCdrInfo = (CCTKCDRINFO *) pCctkAccnt->evt.pDat3 ) != NULL )
   {
      CMGRLOG_INFORMATION ((
         "cmAccntCdrInfo: cdr information for accnt hdl 0x%X, net 0x%X",
         pCctkAccnt->uHdl, pCctkAccnt->uNet ));

      /* Caller-id info and time in the cdr needs to be reformatted according
      ** to configuration so that it aligns with how the caller-id displayed
      ** this info at the time of the call.
      */

      /* Determine the information that can be displayed.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pCctkAccnt->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                 &cfgIdx,
                                 (void *) &pUsrClidDsp,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         bClidNumber = pUsrClidDsp->bCNDActStat;
         bClidName = pUsrClidDsp->bCNAMDActStat;

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                               &cfgIdx,
                               (void *) pUsrClidDsp,
                               CFG_ITEM_DEL );
         pUsrClidDsp = NULL;
      }

      /* Clear the caller-id info if configuration doesn't allow it.
      */
      if ( !bClidNumber )
      {
         memset( pCdrInfo->cdr.clid.cNumber, 0,  sizeof ( pCdrInfo->cdr.clid.cNumber ) );
      }

      if ( !bClidName )
      {
         memset( pCdrInfo->cdr.clid.cName, 0,  sizeof ( pCdrInfo->cdr.clid.cName ) );
      }


      /* If a callback is registered, publish this event out, the data passed during
      ** the callback is only valid within the scope of the callback.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         gCmCfgCb.cmgrEvtCb ( eCMGREVT_CALL_STATS,
                               pCctkAccnt->uHdl,
#if CMGRCFG_MULTILINE
                               CMGR_INVALID_HANDLE,
                               pCctkAccnt->uHdl,
#endif
                              (void *) pCdrInfo );
      }
   }
}
