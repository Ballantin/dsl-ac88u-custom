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
**      This module implements the configuration interface and all configuration
**      related processings for the specific standalone profile.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_STANDALONE)

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CMGR_NULL_REGISTRAR          "0.0.0.0"

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/***********************************************************************************
** FUNCTION:   cmProfStandUserAdd
**
** PURPOSE:    Configure a specific user for a standalone application.
**
** PARAMETERS: pRes - the user resource to configure.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that the user has been allocated its user
**             identification already, which is necessary to configure a user
**             completely.
***********************************************************************************/
void cmProfStandUserAdd ( void *pRes )
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
   CMDMSTATUS eDmStatus = eCMDM_ERROR;
   CFGSIPDIGITMAP *pUsrDigMap = NULL;
   CCTKUSRCFG cctkUsr;
   BOS_UINT32 uIx;
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

         /* At this time we are only creating 'line' users, which are users
         ** associated with an FXS interface on our VoIP gateway.
         */
         pCmUsr->eType = eCMUSRTYPE_LINE;

         /* Create the CCTK configuration for this user.
         */
         cctkUsr.usr.net.sipName.pcUsrName = pUsr->pcUserId;
         cctkUsr.usr.net.sipName.pcDisplay = pUsr->pcDisplayInfo;

         cctkUsr.usr.sec.auth.client.pcAuthName = pUsr->pcPrivId;
         cctkUsr.usr.sec.auth.client.pcPwd = pUsr->credentials.pCredVal;

         /* Enable security as required.
         */
         if ( pUsr->bSigSecurity )
         {
            cctkUsr.usr.net.bSecOnly = BOS_TRUE;
         }

         cctkUsr.reg.gen.registrar.hostPort.pcAddr =
                                         (BOS_UINT8 *)CMGR_NULL_REGISTRAR;

         /* Select the domain to use for this user.
         */
         BOS_UINT8 *pcOrgActiveDomain = pUsr->pcActiveDomain;

         pUsr->pcActiveDomain = pUsr->pDomainList->pcDomain;
         pUsr->uActiveDomain = pUsr->pDomainList->uDomainID;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCmUsr->uUid;
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pUsr,
                               CFG_ITEM_SET );
         pUsr->pcActiveDomain = pcOrgActiveDomain;

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

            gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                  &cfgIdx,
                                  (void *) pDevCfg,
                                  CFG_ITEM_DEL );
            pDevCfg = NULL;
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


         /* Create the CCTK user.
         */
         cctkUserAdd ( &pCmUsr->uNet, &cctkUsr );

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
            eDmStatus = cmDmCompile ( pCmUsr->uHdl,
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
            CMGRLOG_CRITICAL (( "cmProfStandUserAdd: %s - failed to get digit map!",
                                pCmUsr->uUid ));
         }

         CMGRLOG_INFORMATION ((
              "cmProfStandUserAdd: %s - usr 0x%X, net 0x%X, dm %d",
              pCmUsr->uUid, pCmUsr->uHdl,
              pCmUsr->uNet, eDmStatus ));
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                            &cfgIdx,
                            (void *) pUsr,
                            CFG_ITEM_DEL );
      pUsr = NULL;
   }
}

#endif /* (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_STANDALONE) */
