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
**      This module implements the feature processing for the Call Manager.
**
**      This is the only location in the Call Manager code where there is a need
**      to make the support of certain features compile time configurable.  Any
**      feature that is to be attempted will go through the same check to verify
**      that the feature is enabled for the application.  If not, the feature
**      will fail.  In order to determine if a feature is enabled or not, the
**      application configuration MUST define the following macros (for each
**      feature of interest): '#define CMGR__FEATURE__XXX' where XXX is the
**      name of the feature of interest.
**
**      For example 'CMGR__FEATURE__DO_NOT_DISTURB' must be defined in the
**      application configuration file (the cmgrCfgCustom.h module) for the DND
**      (do-not-disturb) feature to be enabled.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* The following constants are used to format the URI used when invoking
** a feature (service).
*/
#define CMGR_SIP_URI                      "sip:"
#define CMGR_SIPS_URI                     "sips:"
#define CMGR_USER_DIALSTRING_URI          ";user=dialstring"
#define CMGR_AT_URI                       "@"
#define CMGR_SEPARATOR_URI                "."

#define CMGR_PRIVATE_DCS_TRACE_PARTY_ID   "P-DCS-TRACE-PARTY-ID"
#define CMGR_ANONYMOUS_URI                "anonymous@anonymous.invalid"
#define CMGR_USER_PHONE_URI               ";user=phone"
#define CMGR_USER_CALLID_URI              ";callid="
#define CMGR_USER_TIMESTAMP_URI           ";timestamp="

#define CMGR_CALL_INFO_PURPOSE            ";purpose="
#define CMGR_CALL_INFO_PURPOSE_ANSNOBUSY  "answer_if_not_busy"

#define CMGR_STAR_INTERNALCALL            "*"



/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmFeatEnabled
**
** PURPOSE:    Utility function to check whether a given feature is enabled
**             or not.
**
** PARAMETERS: pRes - the resource to apply this check on.
**             eAction - the action for which we need to check the enable status.
**
** RETURNS:    BOS_TRUE if the feature is enabled and therefore should be
**             processed by the FSM.
**             BOS_FALSE otherwise.  In such case, some error handling is signaled
**             to the user.
***********************************************************************************/
BOS_BOOL cmFeatEnabled ( void *pRes, CMDMACTIONTYPE eAction )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_BOOL bRet = BOS_FALSE;
   PROV_CFG_IDX cfgIdx;

   CMGRLOG_DEBUG (( "cmFeatEnabled: %s (0x%X) on 0x%X (0x%X)",
                    (char *) cmMapGetStr ( cmMapDmAction, eAction ),
                    eAction, pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   if ( pUsr )
   {
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;

      switch ( eAction )
      {
         /* Automatic call rejection.
         */
#ifdef CMGR__FEATURE__AUTOMATIC_CALL_REJECTION
         case eCMDM_ACRACTIVATE:
         case eCMDM_ACRDEACTIVATE:
         {
            CFGSIPACR *pAcrCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ACRConcept ),
                                       &cfgIdx,
                                       (void *) &pAcrCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pAcrCfg && pAcrCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ACRConcept ),
                                     &cfgIdx,
                                     (void *) pAcrCfg,
                                     CFG_ITEM_DEL );
               pAcrCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__AUTOMATIC_CALL_REJECTION */

         /* Auto-callback.
         */
#ifdef CMGR__FEATURE__AUTOMATIC_CALLBACK
         case eCMDM_ACACTIVATE:
         case eCMDM_ACDEACTIVATE:
         {
            CFGSIPAUTOCALL *pAcCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                       &cfgIdx,
                                       (void *) &pAcCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pAcCfg && pAcCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                     &cfgIdx,
                                     (void *) pAcCfg,
                                     CFG_ITEM_DEL );
               pAcCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__AUTOMATIC_CALLBACK */

         /* Auto-recall.
         */
#ifdef CMGR__FEATURE__AUTOMATIC_RECALL
         case eCMDM_ARACTIVATE:
         case eCMDM_ARDEACTIVATE:
         {
            CFGSIPAUTOCALL *pArCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                       &cfgIdx,
                                       (void *) &pArCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pArCfg && pArCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                     &cfgIdx,
                                     (void *) pArCfg,
                                     CFG_ITEM_DEL );
               pArCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__AUTOMATIC_RECALL */

         /* Caller identification - per call blocking.
         */
#ifdef CMGR__FEATURE__CLID_SUPPRESSION
         case eCMDM_CIDSUPPRESS:
         {
            CFGSIPCIDBLOCK *pCidBlkCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDBlkConcept ),
                                       &cfgIdx,
                                       (void *) &pCidBlkCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCidBlkCfg && pCidBlkCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDBlkConcept ),
                                     &cfgIdx,
                                     (void *) pCidBlkCfg,
                                     CFG_ITEM_DEL );
               pCidBlkCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__CLID_SUPPRESSION */

         /* Caller identification - per call delivery.
         */
#ifdef CMGR__FEATURE__CLID_DELIVERY
         case eCMDM_CIDDELIVER:
         {
            CFGSIPCIDDEL *pCidDelCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDelConcept ),
                                       &cfgIdx,
                                       (void *) &pCidDelCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCidDelCfg && pCidDelCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDelConcept ),
                                     &cfgIdx,
                                     (void *) pCidDelCfg,
                                     CFG_ITEM_DEL );
               pCidDelCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__CLID_DELIVERY */

         /* Caller identification - per call toggling of presentation status.
         */
#ifdef CMGR__FEATURE__CLID_SUPPRESSION
         case eCMDM_CNDBTOGGLE:
         {
            /* CNDB toggle feature is enabled, if CIDS suppression feature is
            ** enabled as well.
            */
            CFGSIPCIDBLOCK *pCidBlkCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDBlkConcept ),
                                       &cfgIdx,
                                       (void *) &pCidBlkCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCidBlkCfg && pCidBlkCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDBlkConcept ),
                                     &cfgIdx,
                                     (void *) pCidBlkCfg,
                                     CFG_ITEM_DEL );
               pCidBlkCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__CLID_SUPPRESSION */

         /* Call forward variable.
         */
#ifdef CMGR__FEATURE__CALL_FORWARD_VARIABLE
         case eCMDM_CFPROGRAM:
         case eCMDM_CFACTIVATE:
         case eCMDM_CFDEACTIVATE:
         {
            CFGSIPCALLFWD *pCallFwdCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                       &cfgIdx,
                                       (void *) &pCallFwdCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCallFwdCfg )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                     &cfgIdx,
                                     (void *) pCallFwdCfg,
                                     CFG_ITEM_DEL );
               pCallFwdCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__CALL_FORWARD_VARIABLE */

         /* Call barring.
         */
#ifdef CMGR__FEATURE__CALL_BARRING
         case eCMDM_CBACTIVATEALL:
         case eCMDM_CBACTIVATEDM:
         case eCMDM_CBDEACTIVATE:
         {
            CFGSIPCALLBARR *pCallBarr = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallBarrConcept ),
                                       &cfgIdx,
                                       (void *) &pCallBarr,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if( pCallBarr && pCallBarr->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallBarrConcept ),
                                     &cfgIdx,
                                     (void *) pCallBarr,
                                     CFG_ITEM_DEL );
               pCallBarr = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__CALL_BARRING */

         /* Customer originated call trace.
         */
#ifdef CMGR__FEATURE__CUSTOMER_ORIGINATED_TRACE
         case eCMDM_COTACTIVATE:
         {
            CFGSIPCOT *pCotCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, COTConcept ),
                                       &cfgIdx,
                                       (void *) &pCotCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCotCfg && pCotCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, COTConcept ),
                                     &cfgIdx,
                                     (void *) pCotCfg,
                                     CFG_ITEM_DEL );
               pCotCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__CUSTOMER_ORIGINATED_TRACE */

         /* Do not disturb client (local use only).
         */
#ifdef CMGR__FEATURE__DO_NOT_DISTURB
         case eCMDM_DNDACTIVATE:
         case eCMDM_DNDDEACTIVATE:
         {
            /* Always available if received as a command from the
            ** digit map.
            */
            bRet = BOS_TRUE;
         }
         break;
#endif /* CMGR__FEATURE__DO_NOT_DISTURB */

         /* Hold activation.
         */
#ifdef CMGR__FEATURE__HOLD_ACTIVATE
         case eCMDM_HOLDACTIVATE:
         {
            CFGSIPCALLHOLD *pHoldCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallHoldConcept ),
                                       &cfgIdx,
                                       (void *) &pHoldCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pHoldCfg && pHoldCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallHoldConcept ),
                                     &cfgIdx,
                                     (void *) pHoldCfg,
                                     CFG_ITEM_DEL );
               pHoldCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__HOLD_ACTIVATE */

         /* Hook flash override behavior control.
         */
#ifdef CMGR__FEATURE__HOOKFLASH_CONTROL
         case eCMDM_HOOKFLASHACTIVATE:
         case eCMDM_HOOKFLASHDEACTIVATE:
         {
            /* Always available if received as a command from the
            ** digit map.
            */
            bRet = BOS_TRUE;
         }
         break;
#endif /* CMGR__FEATURE__HOOKFLASH_CONTROL */

         /* Hotline.
         */
#ifdef CMGR__FEATURE__HOTLINE
         case eCMDM_HOTLINEINT:
         {
            CFGSIPHOTLINE *pHotLineCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                       &cfgIdx,
                                       (void *) &pHotLineCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               /* Hot line is configured when offhook timer is set to 0, otherwise
               ** it is a warmline.
               */
               if ( pHotLineCfg && pHotLineCfg->Act.bCfgStatus && !pHotLineCfg->uOffHookTimer )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                     &cfgIdx,
                                     (void *) pHotLineCfg,
                                     CFG_ITEM_DEL );
               pHotLineCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__HOTLINE */

         /* Call waiting - toggle.
         */
#ifdef CMGR__FEATURE__CALL_WAITING
         case eCMDM_CWTOGGLE:
         {
            CFGSIPCALLWAIT *pCwCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                                       &cfgIdx,
                                       (void *) &pCwCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCwCfg && pCwCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                                     &cfgIdx,
                                     (void *) pCwCfg,
                                     CFG_ITEM_DEL );
               pCwCfg = NULL;
            }

            /* There is a special case to handle with the call waiting toggle
            ** action which is when the call waiting feature is either not
            ** configured or configured but not enabled and we already have a
            ** valid active call for this user.
            ** In this case, the action is used to control the 'cancel call
            ** waiting' feature and therefore should be applied no matter
            ** what.  Distinction of process will happen within the feature
            ** application handler (cmFeatAccept).
            */
            if ( !bRet && cmFeatCcw ( pRes ) )
            {
               bRet = BOS_TRUE;
            }
         }
         break;
#endif /* CMGR__FEATURE__CALL_WAITING */

         /* Emergency calling.
         */
#ifdef CMGR__FEATURE__EMERGENCY_CALL
         case eCMDM_EMERGENCYCALL:
         {
            CFGSIPEMERGE *pEmergCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                       &cfgIdx,
                                       (void *) &pEmergCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pEmergCfg && pEmergCfg->Act.bCfgStatus )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                     &cfgIdx,
                                     (void *) pEmergCfg,
                                     CFG_ITEM_DEL );
               pEmergCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__EMERGENCY_CALL */

         /* Warmline - 'warmline' is actually an extension of 'hotline'
         **            feature for which there is a non NULL timer
         **            set.  Externally 'warmline' does not exist, it
         **            is 'hotline' alone.
         */
#ifdef CMGR__FEATURE__HOTLINE
         case eCMDM_WARMLINEINT:
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
         case eCMDM_WARMLINE_DM_ACTIVATE:
         case eCMDM_WARMLINE_DM_DEACTIVATE:
#endif
         {
            CFGSIPHOTLINE *pHotLineCfg = NULL;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                       &cfgIdx,
                                       (void *) &pHotLineCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pHotLineCfg &&
                    pHotLineCfg->Act.bCfgStatus &&
                    /* Timer must be set and non NULL, then it is a
                    ** warmline feature.
                    */
                    pHotLineCfg->uOffHookTimer )
               {
                  bRet = BOS_TRUE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                     &cfgIdx,
                                     (void *) pHotLineCfg,
                                     CFG_ITEM_DEL );
               pHotLineCfg = NULL;
            }
         }
         break;
#endif /* CMGR__FEATURE__HOTLINE */

         default:
         break;
      }
   }

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmFeatAccept
**
** PURPOSE:    Feature accepted function handler.
**
** PARAMETERS: pRes - the resource to apply this feature on.
**             eAction - the action to be taken.
**
** RETURNS:    BOS_TRUE if the action applied due to the feature allows for further
**             pending action(s) to be processed as follow up to this action.
**             BOS_FALSE otherwise.  This means that following the end of this
**             action processing there is no need to process the rest of the
**             pending action(s).
***********************************************************************************/
BOS_BOOL cmFeatAccept ( void *pRes, CMDMACTIONTYPE eAction )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMGREPSIG sig;
   BOS_BOOL bRet = BOS_TRUE;
   PROV_CFG_IDX cfgIdx;

   CMGRLOG_INFORMATION (( "cmFeatAccept: %s (0x%X) on 0x%X (0x%X)",
                          (char *) cmMapGetStr ( cmMapDmAction, eAction ),
                          eAction, pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   switch ( eAction )
   {
      case eCMDM_CIDSUPPRESS:
      {
         /* Mark the pending action.
         */
         pSes->ses.med.eAction = eCMDM_CIDSUPPRESS;

         /* Apply recall dial tone to continue with the feature process.
         */
         cmFeatRecall ( pRes );
      }
      break;

      case eCMDM_CIDDELIVER:
      {
         /* Mark the pending action.
         */
         pSes->ses.med.eAction = eCMDM_CIDDELIVER;

         /* Apply recall dial tone to continue with the feature process.
         */
         cmFeatRecall ( pRes );
      }
      break;

      case eCMDM_CNDBTOGGLE:
      {
         /* Mark the pending action.  Caller identification toggle changes
         ** the current status of the presentation status for the next call,
         ** therefore it is equivalent to map this to either caller identification
         ** delivery or caller identification suppression, which ever applies
         ** depending on the current presentation status.
         */
         CFGSIPCID *pCfgCid = NULL;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDConcept ),
                                    &cfgIdx,
                                    (void *) &pCfgCid,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pCfgCid )
            {
               switch ( pCfgCid->sPPS )
               {
                  /* If current presentation status is anonymous, toggle is
                  ** equivalent to invoking call identification delivery.
                  */
                  case CFG_SIP_CID_PPS_ANONYMOUS:
                  {
                     pSes->ses.med.eAction = eCMDM_CIDDELIVER;
                  }
                  break;

                  /* If current presentation status is public, toggle is
                  ** equivalent to invoking call identification suppression.
                  */
                  case CFG_SIP_CID_PPS_PUBLIC:
                  {
                     pSes->ses.med.eAction = eCMDM_CIDSUPPRESS;
                  }
                  break;

                  default:
                  break;
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDConcept ),
                   &cfgIdx,
                   (void *) pCfgCid,
                   CFG_ITEM_DEL );
            pCfgCid = NULL;
         }

         /* Apply recall dial tone to continue with the feature process.
         */
         cmFeatRecall ( pRes );
      }
      break;

      case eCMDM_CWTOGGLE:
      {
         pUsr->bPerCallCW = !pUsr->bPerCallCW;

         if ( cmFeatCcw ( pRes ) )
         {
            CMSES *pActiveSes = cmUsrActiveSes ( pUsr );

            /* Play confirmation or deny tone, terminate the transient session
            ** we used for dialing and restore the initial active session
            ** automatically.
            */
            CFGSIPCALLWAIT *pCwCfg = NULL;

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                                       &cfgIdx,
                                       (void *) &pCwCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCwCfg && pCwCfg->Act.bCfgStatus )
               {
                  cmFeatConf ( pRes );
               }
               else
               {
                  cmFeatDeny ( pRes );
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                                     &cfgIdx,
                                     (void *) pCwCfg,
                                     CFG_ITEM_DEL );
               pCwCfg = NULL;
            }
            else
            {
               cmFeatDeny ( pRes );
            }

            cmUsrUnLinkSes ( pUsr, pSes );
            cmSesFree ( pSes, pUsr->uHdl );

            pActiveSes->ses.med.bFocus = BOS_TRUE;
            cctkCallUnhold ( pActiveSes->uNet );
            cmUsrNewState ( pUsr, eCMST_TALK );

            /* Terminate any further action.
            */
            bRet = BOS_FALSE;
         }
         else
         {
            /* Technically, we should play a confirmation tone followed by a
            ** recall dial tone in this scenario, however due to our physical
            ** layer current limitation we cannot do so, so we need to play
            ** an already combined tone instead.  Hence the special case.
            */

            /* -- DO NOT -- */
            /* cmFeatConf ( pRes );     */
            /* cmFeatRecall ( pRes );   */

            /* -- DO INSTEAD -- */
            cmFeatConfRecall ( pRes );
         }
      }
      break;

      case eCMDM_HOLDACTIVATE:
      {
         CMSES *pOtherSes = NULL;
         BOS_UINT32 uIx;

         /* Locate hold active session (if set already).
         */
         for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
         {
            if ( pUsr->pMedSes [ uIx ] &&
                 ( pUsr->pMedSes [ uIx ]->ses.med.eAction == eCMDM_HOLDACTIVATE ) )
            {
               pSes = pUsr->pMedSes [ uIx ];
            }
         }

         /* Locate other active session (if any).
         */
         for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
         {
            if ( pUsr->pMedSes [ uIx ] )
            {
               if ( pUsr->pMedSes [ uIx ] != pSes )
               {
                  pOtherSes = pUsr->pMedSes [ uIx ];
               }
            }
         }

         /* If there is no already a hold-active session and there is a
         ** valid session, this is the one we want to apply hold active on
         ** now.
         */
         if ( ( pSes == NULL ) && pOtherSes )
         {
            pSes = pOtherSes;
            pOtherSes = NULL;
         }

         /* Only one session.  Toggle hold status.
         */
         if ( pSes && !pOtherSes )
         {
            /* Unhold session.
            */
            if ( pSes->ses.med.eAction == eCMDM_HOLDACTIVATE )
            {
               pSes->ses.med.bFocus = BOS_TRUE;
               pSes->ses.med.eAction = eCMDM_UNKNOWNACTION;
               pUsr->pSesHeld = NULL;
               pUsr->bPresrvHeld = BOS_FALSE;

               cctkCallUnhold ( pSes->uNet );

               cmUsrNewState ( pUsr, eCMST_TALK );

               /* Terminate any further action.
               */
               bRet = BOS_FALSE;
            }
            /* Ready to create new call.
            */
            else
            {
               pSes->ses.med.eAction = eCMDM_HOLDACTIVATE;
               pUsr->pSesHeld = pSes;

               pUsr->bShortDT = BOS_TRUE;

               cmFsmOutgoingCall ( pRes );
            }
         }
         /* More than one session.
         */
         else if ( pSes && pOtherSes )
         {
            if ( pSes == pUsr->pSesHeld )
            {
               pSes->ses.med.bFocus = BOS_TRUE;
               pOtherSes->ses.med.bFocus = BOS_FALSE;

               pSes->ses.med.eAction = eCMDM_UNKNOWNACTION;
               /* Swap the session.
               */
               if ( pOtherSes->bEstab )
               {
                  pOtherSes->ses.med.eAction = eCMDM_HOLDACTIVATE;
                  pUsr->pSesHeld = pOtherSes;
               }
               /* The other session was just used to de-activate the hold
               ** feature, terminate it now.
               */
               else
               {
                  cmSesTerminate ( pOtherSes );
                  pUsr->pSesHeld = NULL;
                  pUsr->bPresrvHeld = BOS_FALSE;
                  pOtherSes = NULL;
               }

               cctkCallUnhold ( pSes->uNet );

               cmUsrNewState ( pUsr, eCMST_TALK );

               /* Terminate any further action.
               */
               bRet = BOS_FALSE;
            }
         }
      }
      break;

      case eCMDM_ACDEACTIVATE:
      case eCMDM_ARDEACTIVATE:
      {
         CMAUTOCALL *pAutoCall = cmUsrMostRecentAc( pUsr, eAction );

         if ( pAutoCall )
         {
            /* Keep track of the auto-call information for this session.
            */
            pSes->ses.med.pAutoCall = (void *) pAutoCall;

            if ( pAutoCall->bAnonymous )
            {
               /* Anonymous auto-callback is handled through the network.
               */
               cmFeatSend ( pRes, eAction );
            }
            else
            {
               /* Non-anonymous auto-call is handled through the local
               ** processing.
               */
               CMSES *pSes = cmSesValid ( pAutoCall->uSes );
               if ( pSes != NULL )
               {
                  cctkEventUnsubscribe ( pSes->uNet );
                  cctkEventDelete ( pSes->uNet );
               }
               else
               {
                  cmFeatReject ( pRes, eAction );
               }
            }

            pAutoCall->bUsed = BOS_FALSE;
         }
         /* If the feature was not enabled, ignore its de-activation.
         */
         else
         {
            cmFeatReject ( pRes, eAction );
         }
      }
      break;

      case eCMDM_ACACTIVATE:
      case eCMDM_ARACTIVATE:
      {
         /* Check whether this is a reactivation of the same feature.
         */
         if ( cmUsrReactivateAc ( pUsr, eAction ) )
         {
            cmFeatConf ( pRes );
         }
         else
         {
            BOS_BOOL bDenied = BOS_FALSE;
            CCTKCDR *pCdr = cmCdrGet();
            CCTKPARM eParm =
               ( ( eAction == eCMDM_ARACTIVATE ) ? eCCTKPARM_LASTCALLIN :
                                                      eCCTKPARM_LASTCALLOUT );

            if ( pCdr && ( cctkGetParm ( cmUsrGetNtwkHdl( pUsr ),
                                         eParm,
                                         (void *) pCdr )
                                                == eCCTKSTATUS_SUCCESS ) )
            {
               /* Deny any auto-call type of feature to be activated on an
               ** emergency call (whether incoming or outgoing).
               */
               if ( pCdr->bEmergency )
               {
                  bDenied = BOS_TRUE;
               }
            }

            if ( !bDenied )
            {
               CMAUTOCALL *pAutoCall = cmUsrFreeAc( pUsr, eAction );

               if ( pAutoCall )
               {
                  /* Keep track of the auto-call information for this session.
                  */
                  pSes->ses.med.pAutoCall = (void *) pAutoCall;
                  pSes->ses.med.eAction = eAction;

                  cmFeatSend ( pRes, eAction );
               }
               else
               {
                  bDenied = BOS_TRUE;
               }
            }

            /* Feature can be denied either due to lack of resource or by local
            ** policy.
            */
            if ( bDenied )
            {
               cmFeatReject ( pRes, eAction );
            }
         }
      }
      break;

      case eCMDM_EMERGENCYCALL:
      {
         pSes->ses.med.bEmergency = BOS_TRUE;

         cmFeatSend ( pRes, eAction );
      }
      break;

      case eCMDM_ACRACTIVATE:
      case eCMDM_ACRDEACTIVATE:
      {
         CFGSIPACR *pAcrCfg = NULL;
         BOS_BOOL bActLocalAcr = BOS_FALSE;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;

         /* Determine whether ACR is to be activated locally or on the network.
         */
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ACRConcept ),
                                    &cfgIdx,
                                    (void *) &pAcrCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pAcrCfg && pAcrCfg->Act.bCfgStatus )
            {
               if ( pAcrCfg->bLocalAcrEnabled )
               {
                  bActLocalAcr = BOS_TRUE;
                  pUsr->bRejectAnonymous = ( eAction == eCMDM_ACRACTIVATE ) ?
                                             BOS_TRUE : BOS_FALSE;
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ACRConcept ),
                                  &cfgIdx,
                                  (void *) pAcrCfg,
                                  CFG_ITEM_DEL );
            pAcrCfg = NULL;
         }

         /* ACR is handled locally.
         */
         if ( bActLocalAcr )
         {
            /* Technically, we should play a confirmation tone followed by a
            ** recall dial tone in this scenario, however due to our physical
            ** layer current limitation we cannot do so, so we need to play
            ** an already combined tone instead.  Hence the special case.
            */

            /* -- DO NOT -- */
            /* cmFeatConf ( pRes );     */
            /* cmFeatRecall ( pRes );   */

            /* -- DO INSTEAD -- */
            cmFeatConfRecall ( pRes );
         }
         /* ACR is handled by the network.
         */
         else
         {
            cmFeatSend ( pRes, eAction );
         }
      }
      break;

#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
      case eCMDM_WARMLINE_DM_ACTIVATE:
      case eCMDM_WARMLINE_DM_DEACTIVATE:
      {
         /* Set our local warmline based on VSC entry via dialplan.
         */

         pUsr->bWarmLineActive = ( eAction == eCMDM_WARMLINE_DM_ACTIVATE ) ? BOS_TRUE : BOS_FALSE;

         /* Technically, we should play a confirmation tone followed by a
         ** recall dial tone in this scenario, however due to our physical
         ** layer current limitation we cannot do so, so we need to play
         ** an already combined tone instead.  Hence the special case.
         */

         /* -- DO NOT -- */
         /* cmFeatConf ( pRes );     */
         /* cmFeatRecall ( pRes );   */

         /* -- DO INSTEAD -- */
         cmFeatConfRecall ( pRes );
      }
      break;
#endif

      case eCMDM_COTACTIVATE:
      {
         cmFeatSend ( pRes, eAction );
      }
      break;

      case eCMDM_HOTLINEINT:
      {
         pSes->ses.med.bHotline = BOS_TRUE;
         cmFeatSend ( pRes, eAction );
      }
      break;

      case eCMDM_DNDACTIVATE:
      case eCMDM_DNDDEACTIVATE:
      {
         /* Set our local DND knowledge properly.
         */
         pUsr->bDnd = ( eAction == eCMDM_DNDACTIVATE ) ? BOS_TRUE : BOS_FALSE;

         /* Technically, we should play a confirmation tone followed by a
         ** recall dial tone in this scenario, however due to our physical
         ** layer current limitation we cannot do so, so we need to play
         ** an already combined tone instead.  Hence the special case.
         */

         /* -- DO NOT -- */
         /* cmFeatConf ( pRes );     */
         /* cmFeatRecall ( pRes );   */

         /* -- DO INSTEAD -- */
         cmFeatConfRecall ( pRes );
      }
      break;

      case eCMDM_HOOKFLASHACTIVATE:
      case eCMDM_HOOKFLASHDEACTIVATE:
      {
         /* Set our local hook flash control knowledge properly.
         */
         CFGSIPBASICCALL *pBasicCall = NULL;
         PROV_CFG_IDX cfgIdx;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                    &cfgIdx,
                                    (void *) &pBasicCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pBasicCall )
            {
               /* Set the hook flash disabled status based on the feature detected,
               ** this is a local user override of the ocnfiguration that will apply
               ** to a certain feature set requiring hook flash process to be used.
               */
               pBasicCall->bHfDisabled =
                 ( eAction == eCMDM_HOOKFLASHACTIVATE ) ? BOS_FALSE : BOS_TRUE;

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_SET );
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                  &cfgIdx,
                                  (void *) pBasicCall,
                                  CFG_ITEM_DEL );
            pBasicCall = NULL;
         }
      }
      break;

      case eCMDM_CFPROGRAM:
      {
         BOS_UINT8 *pActParm = NULL;
         CMDMSTATUS eDmStatus;

         /* There shoudl be a parameter for this action and it should be the
         ** actual URI generated by the digit map rules that corresponds to the
         ** 'forward to uri' that the user wants to program.
         */
         eDmStatus = cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                        &pActParm );
         if ( ( eDmStatus == eCMDM_SUCCESS ) &&
              ( pActParm != NULL ) )
         {
            CFGSIPCALLFWD *pCallFwd = NULL;
            PROV_CFG_IDX cfgIdx;

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                       &cfgIdx,
                                       (void *) &pCallFwd,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCallFwd )
               {
                  /* Make sure the pcFwdToDstring or pcFwdToUri buffer is not empty
                  ** or we can't pass the 'forward-to' string as parameter
                  */
                  if( pCallFwd->bUseFwdDString && pCallFwd->pcFwdToDString )
                  {
                     /* Save the 'forward-to' dialstring programmed by digit map into
                     ** the call forward concept for further usage.  The configuration
                     ** module MUST make a copy of this dialstring and store it, it cannot
                     ** keep this pointer.
                     **
                     ** be careful the size of pcFwdToDstring (512 Bytes ) which is
                     ** allocate from CFG_ITEM_GET call
                     */
                     strncpy( (char *)pCallFwd->pcFwdToDString, (const char *)pActParm, strlen((const char *)pActParm));
                  }
                  else if( pCallFwd->pcFwdToUri )
                  {
                     /* Save the 'forward-to' URI programmed by digit map into
                     ** the call forward concept for further usage.  The configuration
                     ** module MUST make a copy of this URI and store it, it cannot
                     ** keep this pointer.
                     **
                     ** be careful the size of pcFwdToUri (512 Bytes ) which is
                     ** allocate from CFG_ITEM_GET call
                     */
                     strncpy((char *)pCallFwd->pcFwdToUri, (const char *)pActParm, strlen((const char *)pActParm));
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                        &cfgIdx,
                                        (void *) pCallFwd,
                                        CFG_ITEM_SET );
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                     &cfgIdx,
                                     (void *) pCallFwd,
                                     CFG_ITEM_DEL );
               pCallFwd = NULL;
            }

            /* -- DO NOT -- */
            /* cmFeatConf ( pRes );     */
            /* cmFeatRecall ( pRes );   */

            /* -- DO INSTEAD -- */
            cmFeatConfRecall ( pRes );
         }
         else
         {
            /* Missing mandatory parameter, reject the action.
            */
            cmFeatReject ( pRes, eAction );
         }
      }
      break;

      case eCMDM_CFACTIVATE:
      case eCMDM_CFDEACTIVATE:
      {
         CFGSIPCALLFWD *pCallFwd = NULL;
         PROV_CFG_IDX cfgIdx;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                    &cfgIdx,
                                    (void *) &pCallFwd,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pCallFwd )
            {
               /* Save the user setting for the feature such that we know
               ** whether the user want to apply call forward or not.
               */
               pCallFwd->bUsrSet =
                 ( eAction == eCMDM_CFACTIVATE ) ? BOS_TRUE : BOS_FALSE;

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                     &cfgIdx,
                                     (void *) pCallFwd,
                                     CFG_ITEM_SET );
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                  &cfgIdx,
                                  (void *) pCallFwd,
                                  CFG_ITEM_DEL );
            pCallFwd = NULL;
         }

         /* -- DO NOT -- */
         /* cmFeatConf ( pRes );     */
         /* cmFeatRecall ( pRes );   */

         /* -- DO INSTEAD -- */
         cmFeatConfRecall ( pRes );
      }
      break;

      case eCMDM_CBACTIVATEALL:
      case eCMDM_CBACTIVATEDM:
      case eCMDM_CBDEACTIVATE:
      {
         CFGSIPCALLBARR *pCallBarr = NULL;
         PROV_CFG_IDX cfgIdx;
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( cmUsrGetNtwkHdl( pUsr ) );
         if( !pCctkAccnt )
         {
            CMGRLOG_CRITICAL(( "%s: Unable to find CCTK Account associated with Usr [%X]", __FUNCTION__, pUsr->uHdl ));
            cmFeatReject( pRes, eAction );
            break;
         }

         memset( &cfgIdx, 0, sizeof( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCctkAccnt->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallBarrConcept ),
                                    &cfgIdx,
                                    (void *) &pCallBarr,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if( pCallBarr )
            {
               BOS_UINT8 *pActParm;
               cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                  &pActParm );

               if( strcmp( (char *)pActParm,
                           (char *)pCallBarr->pcBarringPIN ) == 0 )
               {
                  CFGSIPDIGITMAP *pUsrDigMap = NULL;
                  /* Play a confirmation tone for the user */
                  cmFeatConf ( pRes );

                  /* Activate or deactivate */
                  pCallBarr->bBarrDm = ( eAction == eCMDM_CBACTIVATEDM ) ? BOS_TRUE : BOS_FALSE;
                  pCallBarr->bBarrAll = ( eAction == eCMDM_CBACTIVATEALL ) ? BOS_TRUE : BOS_FALSE;

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallBarrConcept ),
                                        &cfgIdx,
                                        (void *) pCallBarr,
                                        CFG_ITEM_SET );

                  /* Re-create the line-specific DM for this user */
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                             &cfgIdx,
                                             (void *) &pUsrDigMap,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     /* Wipe the old digitmap */
                     memset( pCctkAccnt->pDmDigMap, 0, CMDM_MAP_BUFFER_SIZE * sizeof(BOS_UINT8) );

                     cmDmCompile ( pCctkAccnt->uNet,
                                   pUsrDigMap->pABNF,
                                   (CMDMDIGITMAP *) pCctkAccnt->pDmDigMap,
                                   CMDM_MAP_BUFFER_SIZE );

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapConcept ),
                                           &cfgIdx,
                                           (void *) pUsrDigMap,
                                           CFG_ITEM_DEL );
                  }
               }
               else
               {
                  CMGRLOG_INFORMATION(( "%s: pUsr [%X] - Invalid CallBarring PIN",
                                        __FUNCTION__,
                                        pUsr->uHdl ));
                  cmFeatReject ( pRes, eAction );
               }
            }
            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallBarrConcept ),
                                  &cfgIdx,
                                  (void *) pCallBarr,
                                  CFG_ITEM_DEL );
            pCallBarr = NULL;
         }
      }
      break;

      default:
      break;
   }

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmFeatReject
**
** PURPOSE:    Feature rejected function handler.
**
** PARAMETERS: pRes - the resource to apply this feature on.
**             eAction - the action being rejected.
**
** RETURNS:    Nothing.  Depending on the actual action taken, the function
**             exercizes the correct processing.
***********************************************************************************/
void cmFeatReject ( void *pRes, CMDMACTIONTYPE eAction )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMGREPSIG sig;
   CMTMR tmr;
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;

   CMGRLOG_INFORMATION (( "cmFeatReject: %s (0x%X) on 0x%X (0x%X)",
                          (char *) cmMapGetStr ( cmMapDmAction, eAction ),
                          eAction, pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   switch ( eAction )
   {
      case eCMDM_ACRACTIVATE:
      case eCMDM_ACRDEACTIVATE:
      case eCMDM_DNDACTIVATE:
      case eCMDM_DNDDEACTIVATE:
      {
         /* Ignore the feature and simply play recall dial tone.
         */
         cmFeatRecall ( pRes );
      }
      break;

      case eCMDM_ACACTIVATE:
      case eCMDM_ACDEACTIVATE:
      case eCMDM_ARACTIVATE:
      case eCMDM_ARDEACTIVATE:
      case eCMDM_CIDSUPPRESS:
      case eCMDM_CIDDELIVER:
      case eCMDM_CFPROGRAM:
      case eCMDM_CFACTIVATE:
      case eCMDM_CFDEACTIVATE:
      case eCMDM_CNDBTOGGLE:
      case eCMDM_COTACTIVATE:
      case eCMDM_CWTOGGLE:
      case eCMDM_HOLDACTIVATE:
      {
         /* Check whether a 'feature failure URI' was provided for this
         ** feature, if so, we need to redirect the call to it, otherwise
         ** we will handle the clean up locally.
         */
         if ( cmFeatError ( pRes, eAction ) )
         {
            /* If the case was handled, exit now.
            */
            break;
         }
      }
      /*
      ** WARNING: Fall through.  This is expected, see comment above for
      **          procedure.
      */
      default:
      {
         /* If we are configured to do so, issue a network disconnect
         ** signal.
         */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                    &cfgIdx,
                                    (void *) &pBasicCall,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pBasicCall && pBasicCall->bIssueNetDisc )
            {
               cmFsmOsi ( pRes );
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                  &cfgIdx,
                                  (void *) pBasicCall,
                                  CFG_ITEM_DEL );
            pBasicCall = NULL;
         }

         /* Play an error tone (re-order) to signify the end of the service
         ** to the user.
         */
         sig.eAct = eCMGREPSIGACT_START;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
         sig.uLine = CMGR_INVALID_HANDLE;
#endif
         sig.uSig = eEPTSIG_REORDER;
         pUsr->uCpt = sig.uSig;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_OHWARN;
         tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;

         cmTmrStart ( &tmr );

         /* Update current state.
         */
         cmUsrNewState ( pUsr, eCMST_RELEASING );
      }
      break;
   }
}


/***********************************************************************************
** FUNCTION:   cmFeatRecall
**
** PURPOSE:    Generic handler to play 'recall' dial tone when a feature is
**             activated.  Used by many features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatRecall ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMTMR tmr;
   CMGREPSIG sig;
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT32 tmrShort, tmrLong;
   BOS_BOOL bInterDigTmr = BOS_FALSE;

   CMGRLOG_INFORMATION (( "cmFeatRecall: 0x%X (0x%X) on 0x%X (0x%X)",
                          pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   /* If we already are applying the correct behavior, do not repeat it.
   */
   if ( ! ( ( pUsr->uCpt == eEPTSIG_DIAL ) ||
            ( pUsr->uCpt == eEPTSIG_SPECIAL_DIAL ) ) )
   {
      pUsr->eDigDetState = CMDIGITDETSTA_NONE;
      pUsr->bIntCallDet = BOS_FALSE;

      tmrLong = CMGR_INTERDIGIT_LONG_TONE_TIMEOUT;

      sig.eAct = eCMGREPSIGACT_START;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif
      sig.uSig = eEPTSIG_SPECIAL_DIAL;
      pUsr->uCpt = sig.uSig;

      cmPhySignal ( pUsr->uPhys, &sig );

      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_DIALTONE;

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pBasicCall )
         {
            tmr.uTimeOut = pBasicCall->uOrigDTTimer;
            tmrLong = pBasicCall->uOrigLITimer;
            bInterDigTmr = pBasicCall->bInterDigTmrOnRecall;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }
      else
      {
         tmr.uTimeOut = CMGR_DIAL_TONE_TIMEOUT;
      }

      cmTmrStart ( &tmr );

      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmDigMap,
                                CMDM_INTERDIGIT_TMR_SHORT,
                                &tmrShort ) != eCMDM_SUCCESS )
      {
         tmrShort = CMGR_INTERDIGIT_SHORT_TONE_TIMEOUT;
      }

      /* If the interdigit timer is to be started, do it now.
      */
      if ( bInterDigTmr )
      {
         tmr.eType = eCMEVT_TMR_INTERDIGIT_SHORT;
         tmr.uTimeOut = tmrShort;
         cmTmrStart ( &tmr );

         tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
         tmr.uTimeOut = tmrLong;
         cmTmrStart ( &tmr );
      }

      cmUsrNewState ( pUsr, eCMST_DIALING );
   }
}


/***********************************************************************************
** FUNCTION:   cmFeatConfRecall
**
** PURPOSE:    Special handler to play 'confirmation' tone followed by a 'recall'
**             dial tone when a is activated.  May be used by any features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatConfRecall ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMTMR tmr;
   CMGREPSIG sig;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE   */
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT32 tmrShort, tmrLong;
   BOS_BOOL bInterDigTmr = BOS_FALSE;

   CMGRLOG_INFORMATION (( "cmFeatConfRecall: 0x%X (0x%X) on 0x%X (0x%X)",
                          pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   sig.eAct = eCMGREPSIGACT_START;
   sig.uSes = CMGR_INVALID_HANDLE;
   sig.uApp = CMGR_INVALID_HANDLE;
   sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
   sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE   */
   sig.uSig = eEPTSIG_CONF_STUTTER;
   pUsr->uCpt = sig.uSig;

   cmPhySignal ( pUsr->uPhys, &sig );

   tmr.uSes = pSes->uHdl;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_DIALTONE;
   tmrLong = CMGR_INTERDIGIT_LONG_TONE_TIMEOUT;

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                              &cfgIdx,
                              (void *) &pBasicCall,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pBasicCall )
      {
         tmr.uTimeOut = pBasicCall->uOrigDTTimer;
         tmrLong = pBasicCall->uOrigLITimer;
         bInterDigTmr = pBasicCall->bInterDigTmrOnRecall;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                            &cfgIdx,
                            (void *) pBasicCall,
                            CFG_ITEM_DEL );
      pBasicCall = NULL;
   }
   else
   {
      tmr.uTimeOut = CMGR_DIAL_TONE_TIMEOUT;
   }

   cmTmrStart ( &tmr );

   if ( cmDmGetTimerValue ( (CMDMDIGITMAP *) pUsr->pDmDigMap,
                             CMDM_INTERDIGIT_TMR_SHORT,
                             &tmrShort ) != eCMDM_SUCCESS )
   {
      tmrShort = CMGR_INTERDIGIT_SHORT_TONE_TIMEOUT;
   }

   /* If the interdigit timer is to be started, do it now.
   */
   if ( bInterDigTmr )
   {
      tmr.eType = eCMEVT_TMR_INTERDIGIT_SHORT;
      tmr.uTimeOut = tmrShort;
      cmTmrStart ( &tmr );

      tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
      tmr.uTimeOut = tmrLong;
      cmTmrStart ( &tmr );
   }

   pUsr->eDigDetState = CMDIGITDETSTA_NONE;
   pUsr->bIntCallDet = BOS_FALSE;


}


/***********************************************************************************
** FUNCTION:   cmFeatConf
**
** PURPOSE:    Generic handler to play 'confirmation' tone when a feature is
**             activated.  May be used by any features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatConf ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMGREPSIG sig;

   CMGRLOG_INFORMATION (( "cmFeatConf: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   sig.eAct = eCMGREPSIGACT_START;
   sig.uSes = CMGR_INVALID_HANDLE;
   sig.uApp = CMGR_INVALID_HANDLE;
   sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
   sig.uLine = CMGR_INVALID_HANDLE;
#endif
   sig.uSig = eEPTSIG_CONF;
   pUsr->uCpt = sig.uSig;

   cmPhySignal ( pUsr->uPhys, &sig );
}


/***********************************************************************************
** FUNCTION:   cmFeatDeny
**
** PURPOSE:    Generic handler to play 'deny' tone when a feature is
**             not activated or cannot be applied but the processing is not
**             dependent on a specific user action to continue successfully.
**             May be used by any features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatDeny ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMGREPSIG sig;

   CMGRLOG_INFORMATION (( "cmFeatDeny: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   sig.eAct = eCMGREPSIGACT_START;
   sig.uSes = CMGR_INVALID_HANDLE;
   sig.uApp = CMGR_INVALID_HANDLE;
   sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
   sig.uLine = CMGR_INVALID_HANDLE;
#endif
   sig.uSig = eEPTSIG_DENY;

   pUsr->uCpt = sig.uSig;

   cmPhySignal ( pUsr->uPhys, &sig );
}


/***********************************************************************************
** FUNCTION:   cmFeatSend
**
** PURPOSE:    Generic handler to send a feature request to the network when the
**             corresponding feature is activated.  Used by many features.
**
** PARAMETERS: pRes - the resource to apply this action on.
**             eAction - the action from the digit map that is currently processed.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatSend ( void *pRes, CMDMACTIONTYPE eAction )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   BOS_UINT8 *pActParm = NULL;
   CMDMSTATUS eDmStatus;
   BOS_UINT8 featUri[ CMGR_URI_STRING_LEN + 1 ];
   CFGSIPUSERCFG *pUsrCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   CCTKCDR *pCdr = cmCdrGet();
   BOS_UINT32 uAccount = ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) ? pSes->uNetUsr : cmUsrGetNtwkHdl( pUsr );

   CMGRLOG_INFORMATION (( "cmFeatSend: 0x%X (0x%X) on 0x%X (0x%X)",
                          pUsr->uHdl, uAccount, pSes->uHdl, pSes->uNet ));

   memset ( pCdr,
            0,
            sizeof ( CCTKCDR ) );

   memset ( featUri,
            0,
            sizeof ( featUri ) );

   if( uAccount == CMGR_INVALID_HANDLE )
   {
      CMGREPSIG sig;
      CMTMR tmr;

      memset ( &sig, 0, sizeof ( CMGREPSIG ) );
      memset ( &tmr, 0, sizeof ( CMTMR ) );

      CMGRLOG_DEBUG (( "%s: No CCTK user available for 0x%X", __FUNCTION__, pUsr->uHdl ));

      sig.eAct = eCMGREPSIGACT_START;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif
      sig.uSig = eEPTSIG_REORDER;
      pUsr->uCpt = sig.uSig;

      cmPhySignal ( pUsr->uPhys, &sig );

      /* Don't start offhook warning timer if there are
      ** other sessions for this user.
      */
      if( ( pUsr->pSesWait == NULL ) && ( pUsr->pSesHeld == NULL ) )
      {
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_OHWARN;
         tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;

         cmTmrStart ( &tmr );
      }

      cmUsrUnLinkSes ( pUsr, pSes );
      cmSesFree ( pSes, pUsr->uHdl );
      return;
   }

   /* Retrieve or format the service URI for the feature depending on
   ** the type of feature we are dealing with.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                              &cfgIdx,
                              (void *) &pUsrCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( ( eAction == eCMDM_ACDEACTIVATE ) ||
           ( eAction == eCMDM_ARDEACTIVATE ) )
      {
         /* Auto-recall and auto-callback deactivation actions must use the
         ** same information that was sent during activation except for the
         ** initial VSC.
         */
         BOS_UINT8 *pPeer = ((CMAUTOCALL *) pSes->ses.med.pAutoCall)->uPeer;
         BOS_UINT8 *pColumn = NULL;
         BOS_UINT8 *pSep = NULL;
         BOS_UINT8 uSaved;

         pColumn = (BOS_UINT8 *) strchr ( (const char *) pPeer, ':' );
         pSep = (BOS_UINT8 *) strchr ( (const char *) pPeer, '.' );

         if ( pColumn && pSep )
         {
            ++pColumn;
            uSaved = *pColumn;
            *pColumn = '\0';

            strMaxCat ( (char *) featUri,
                        (const char *) pPeer,
                        CMGR_URI_STRING_LEN );

            *pColumn = uSaved;

            /* Add new Vertical Service Code.
            */
            eDmStatus = cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                           &pActParm );
            if ( ( eDmStatus == eCMDM_SUCCESS ) &&
                 ( pActParm != NULL ) )
            {
               strMaxCat ( (char *) featUri,
                           (const char *) pActParm,
                           CMGR_URI_STRING_LEN );
            }

            strMaxCat ( (char *) featUri,
                        (const char *) pSep,
                        CMGR_URI_STRING_LEN );
         }
         else
         {
            /* Something is wrong.  Fail the feature.
            */
            cmFsmCallEnd ( pRes );
         }
      }
      else
      {
         BOS_BOOL bCommon = BOS_TRUE;

         switch ( eAction )
         {
            case eCMDM_ACACTIVATE:
            case eCMDM_ARACTIVATE:
            {
               BOS_UINT8 callId[ CMGR_URI_STRING_LEN / 2 ];
               BOS_UINT8 *pAt;
               CCTKPARM eParm =
                  ( ( eAction == eCMDM_ARACTIVATE ) ? eCCTKPARM_LASTCALLIN :
                                                         eCCTKPARM_LASTCALLOUT );

               if ( pCdr && ( cctkGetParm ( uAccount,
                                            eParm,
                                            (void *) pCdr )
                                                   == eCCTKSTATUS_SUCCESS ) )
               {
                  if ( pCdr->bAnonymous &&
                       !strlen ( (const char *) pCdr->uPeer ) )
                  {
                     ((CMAUTOCALL *)pSes->ses.med.pAutoCall)->bAnonymous =
                                                                       BOS_TRUE;

                     /* Add header.
                     */
                     strMaxCat ( (char *) featUri,
                                 ( pUsrCfg->bSigSecurity ?
                                                CMGR_SIPS_URI : CMGR_SIP_URI ),
                                 CMGR_URI_STRING_LEN );

                     /* Add Vertical Service Code.
                     */
                     eDmStatus = cmDmGetParameter (
                                       (CMDMACTIONSTATE *)pUsr->pDmState,
                                       &pActParm );
                     if ( ( eDmStatus == eCMDM_SUCCESS ) &&
                          ( pActParm != NULL ) )
                     {
                        strMaxCat ( (char *) featUri,
                                    (const char *) pActParm,
                                    CMGR_URI_STRING_LEN );
                     }

                     /* Add separator.
                     */
                     strMaxCat ( (char *) featUri,
                                 CMGR_SEPARATOR_URI,
                                 CMGR_URI_STRING_LEN );

                     if ( strchr ( (const char *) pCdr->uCallId, '@' ) != NULL )
                     {
                        /* Add 'edited' call identification of the last
                        ** recorded caller.
                        */
                        memset ( (void *) callId,
                                 0,
                                 sizeof ( callId ) );

                        strMaxCat ( (char *) callId,
                                    (const char *) pCdr->uCallId,
                                    CMGR_URI_STRING_LEN );

                        pAt = (BOS_UINT8 *) strchr ( (const char *) callId, '@' );
                        *pAt = '\0';

                        strMaxCat ( (char *) featUri,
                                    (const char *) callId,
                                    CMGR_URI_STRING_LEN );

                        strMaxCat ( (char *) featUri,
                                    "%40",
                                    CMGR_URI_STRING_LEN );

                        *pAt = '@';
                        ++pAt;
                        strMaxCat ( (char *) featUri,
                                    (const char *) pAt,
                                    CMGR_URI_STRING_LEN );
                     }
                     else
                     {
                        /* Add call identification of the last recorded caller.
                        */
                        strMaxCat ( (char *) featUri,
                                    (const char *) pCdr->uCallId,
                                    CMGR_URI_STRING_LEN );
                     }
                  }
                  else
                  {
                     strMaxCat ( (char *) featUri,
                                 (const char *) pCdr->uPeer,
                                 CMGR_URI_STRING_LEN );
                     /* The URI is complete, no need to add extra information
                     ** beyond this.
                     */
                     bCommon = BOS_FALSE;
                  }
               }
            }
            break;

            case eCMDM_EMERGENCYCALL:
            {
               /* Add Emergency Service URI.
               */
               eDmStatus = cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                              &pActParm );
               if ( ( eDmStatus == eCMDM_SUCCESS ) &&
                    ( pActParm != NULL ) )
               {
                  strMaxCat ( (char *) featUri,
                              (const char *) pActParm,
                              CMGR_URI_STRING_LEN );
               }

               /* Nothing else to add, the emergency URI is supposed to be entirely
               ** formatted by the digit map.
               */
               bCommon = BOS_FALSE;
            }
            break;

            case eCMDM_HOTLINEINT:
            {
               CFGSIPHOTLINE *pHotLineCfg = NULL;

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                          &cfgIdx,
                                          (void *) &pHotLineCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pHotLineCfg && pHotLineCfg->pHotLineUri )
                  {

                     eDmStatus = cmDmCheckDialString (
                                       pUsr->uHdl,
                                       (CMDMDIGITMAP *) pUsr->pDmInUse,
                                       pHotLineCfg->pHotLineUri,
                                       (CMDMACTIONSTATE *) pUsr->pDmState );
                     if( eDmStatus == eCMDM_SUCCESS )
                     {
                        CMDMACTIONTYPE eAction;
                        /* Process the actions associated with this user.
                        */
                        cmDmGetAction ( (CMDMACTIONSTATE *)pUsr->pDmState, &eAction );

                        eDmStatus = cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState, &pActParm );
                        if ( ( eDmStatus == eCMDM_SUCCESS ) && ( pActParm != NULL ) )
                        {
                           strMaxCat ( (char *) featUri,
                                       (const char *) pActParm,
                                       CMGR_URI_STRING_LEN );
                        }
                     }
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                        &cfgIdx,
                                        (void *) pHotLineCfg,
                                        CFG_ITEM_DEL );
                  pHotLineCfg = NULL;
               }

               /* Nothing else to add, the hotline URI is coming entirely from the
               ** hotline feature configuration.
               */
               bCommon = BOS_FALSE;
            }
            break;

            default:
            {
               /* Get the URI from the digit map.
               */
               eDmStatus = cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                              &pActParm );
               if ( ( eDmStatus == eCMDM_SUCCESS ) &&
                    ( pActParm != NULL ) )
               {
                  strMaxCat ( (char *) featUri,
                              (const char *) pActParm,
                              CMGR_URI_STRING_LEN );
               }

               /* Nothing else to add for this URI.
               */
               bCommon = BOS_FALSE;
            }
            break;
         }

         /* Add the remainder 'common' information, unless this was overruled by
         ** a specific feature.
         */
         if ( bCommon )
         {
            /* Add domain.
            */
            strMaxCat ( (char *) featUri,
                        CMGR_AT_URI,
                        CMGR_URI_STRING_LEN );
            strMaxCat ( (char *) featUri,
                        (const char *) pUsrCfg->pcActiveDomain,
                        CMGR_URI_STRING_LEN );

            /* Add parameters.
            */
            strMaxCat ( (char *) featUri,
                        CMGR_USER_DIALSTRING_URI,
                        CMGR_URI_STRING_LEN );
         }
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                            &cfgIdx,
                            (void *) pUsrCfg,
                            CFG_ITEM_DEL );
      pUsrCfg = NULL;
   }

   /* Setup parameters that need to be set prior to creating the call in order
   ** to take effect.
   */
   switch ( eAction )
   {
      case eCMDM_EMERGENCYCALL:
      {
         CCTKEMGCALLCFG emCfg;
         CFGSIPEMERGE *pEmergCfg = NULL;

         memset ( &emCfg, 0, sizeof ( CCTKEMGCALLCFG ) );
         emCfg.bEmergency = BOS_TRUE;
         emCfg.bAddLocInfo = BOS_TRUE;

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                    &cfgIdx,
                                    (void *) &pEmergCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            /* Possible overwrite of the default behavior per configuration.
            */
            if ( pEmergCfg )
            {
               emCfg.bAddLocInfo = !pEmergCfg->bNoLocInfo;
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, EServicesConcept ),
                                  &cfgIdx,
                                  (void *) pEmergCfg,
                                  CFG_ITEM_DEL );
            pEmergCfg = NULL;
         }

         /* Note: use the user configuration index because per-call
         **       configuration is not supported at the CCTK/UASSP
         **       interface yet.
         */
         cctkSetParm ( uAccount,
                       eCCTKPARM_EC_EMGCALLCFG,
                       (void *) &emCfg );
      }
      break;

      default:
      break;
   }

   /* Create the network resource to manage this feature.
   */


   /* Associate the CCTK user with the session before making any outgoing calls.
   */
   pSes->uNetUsr = uAccount;


   if ( strlen( (const char *) featUri ) &&
        ( cctkCallCreate ( &pSes->uNet,
                           uAccount,
                           featUri ) == eCCTKSTATUS_SUCCESS ) )
   {
      CMGRLOG_DEBUG (( "cmFeatSend: \'%s\' on 0x%X (0x%X)",
                       featUri, pUsr->uHdl, uAccount ));

#if CMGRCFG_MULTILINE
      /* For multiline implementation we need to establish sessions and add calls
      ** to our cctkAccnt right away.
      */
      if( !pSes->bEstab )
      {
         cmCctkAccntCallAdd( pSes->uNetUsr );
         pSes->bEstab = BOS_TRUE;
      }
#endif /* CMGRCFG_MULTILINE */


      /* Add any feature specific information that must apply for this
      ** particular session.
      */
      switch ( eAction )
      {
         case eCMDM_ACACTIVATE:
         case eCMDM_ARACTIVATE:
         {
            CCTKGENHDRCFG hdrCfg;
            BOS_UINT8 hdrValue[ 2 * CMGR_URI_STRING_LEN + 1 ];

            memset ( (void *) ((CMAUTOCALL *) pSes->ses.med.pAutoCall)->uPeer,
                     0,
                     sizeof ( ((CMAUTOCALL *) pSes->ses.med.pAutoCall)->uPeer ) );
            /* Remember the information for de-activation.
            */
            memcpy ( (void *) ((CMAUTOCALL *) pSes->ses.med.pAutoCall)->uPeer,
                     (const void *) featUri,
                     sizeof ( ((CMAUTOCALL *) pSes->ses.med.pAutoCall)->uPeer ) );

            ((CMAUTOCALL *) pSes->ses.med.pAutoCall)->bUsed = BOS_TRUE;
            ((CMAUTOCALL *) pSes->ses.med.pAutoCall)->uSes = pSes->uHdl;
            bosTimeGetMs( &((CMAUTOCALL *) pSes->ses.med.pAutoCall)->time );

            /* Adding the header: Call-Info for this feature
            ** to be executed properly.
            */
            memset ( (void *) hdrValue,
                     0,
                     sizeof ( hdrValue ) );

            hdrCfg.uNum = 1;
            hdrCfg.hdr[0].uMethod = CCTK_METHOD_INVITE;
            hdrCfg.hdr[0].eMap = eCCTKHDRMAP_REQ;
            hdrCfg.hdr[0].eMode = eCCTKHDRMODE_OVRWRITE;

            strMaxCat ( (char *) hdrValue,
                        "<",
                        2 * CMGR_URI_STRING_LEN );
            /* Use the feature URI as the absolute URI for the header content.
            */
            strMaxCat ( (char *) hdrValue,
                        (const char *) featUri,
                        2 * CMGR_URI_STRING_LEN );
            strMaxCat ( (char *) hdrValue,
                        ">",
                        2 * CMGR_URI_STRING_LEN );
            /* Set the purpose of the Call-Info to be the predefined string:
            ** "answer_if_not_busy" (CMGR_CALL_INFO_PURPOSE_ANSNOBUSY).
            */
            strMaxCat ( (char *) hdrValue,
                        CMGR_CALL_INFO_PURPOSE,
                        2 * CMGR_URI_STRING_LEN );
            strMaxCat ( (char *) hdrValue,
                        CMGR_CALL_INFO_PURPOSE_ANSNOBUSY,
                        2 * CMGR_URI_STRING_LEN );

            hdrCfg.hdr[0].hdr.eType = eCCTKHDRTYPE_CALL_INFO;
            hdrCfg.hdr[0].hdr.pcName = NULL;
            hdrCfg.hdr[0].hdr.pcValue = hdrValue;

            cctkSetParm ( pSes->uNet,
                          eCCTKPARM_EC_GENHDRCFG,
                          (void *) &hdrCfg );
         }
         break;

         case eCMDM_COTACTIVATE:
         {
            /* Adding the private header: P-DCS-TRACE-PARTY-ID for this feature
            ** to be executed properly.
            */
            CCTKGENHDRCFG hdrCfg;
            BOS_UINT8 hdrValue[ 2 * CMGR_URI_STRING_LEN + 1 ];
            BOS_UINT8 timeStamp[ 12 ];

            memset ( (void *) hdrValue,
                     0,
                     sizeof ( hdrValue ) );

            if ( pCdr && ( cctkGetParm ( uAccount,
                                         eCCTKPARM_LASTCALLIN,
                                         (void *) pCdr ) == eCCTKSTATUS_SUCCESS ) )
            {
               hdrCfg.uNum = 1;
               hdrCfg.hdr[0].uMethod = CCTK_METHOD_INVITE;
               hdrCfg.hdr[0].eMap = eCCTKHDRMAP_REQ;
               hdrCfg.hdr[0].eMode = eCCTKHDRMODE_OVRWRITE;

               strMaxCat ( (char *) hdrValue,
                           "<",
                           2 * CMGR_URI_STRING_LEN );
               if ( pCdr->bAnonymous || !strlen ( (const char *) pCdr->uPeer ) )
               {
                  strMaxCat ( (char *) hdrValue,
                              CMGR_SIP_URI,
                              2 * CMGR_URI_STRING_LEN );
                  strMaxCat ( (char *) hdrValue,
                              CMGR_ANONYMOUS_URI,
                              2 * CMGR_URI_STRING_LEN );
               }
               else
               {
                  strMaxCat ( (char *) hdrValue,
                              (const char *) pCdr->uPeer,
                              2 * CMGR_URI_STRING_LEN );
               }
               strMaxCat ( (char *) hdrValue,
                           CMGR_USER_PHONE_URI,
                           2 * CMGR_URI_STRING_LEN );
               strMaxCat ( (char *) hdrValue,
                           CMGR_USER_CALLID_URI,
                           2 * CMGR_URI_STRING_LEN );
               strMaxCat ( (char *) hdrValue,
                           (const char *) pCdr->uCallId,
                           2 * CMGR_URI_STRING_LEN );
               strMaxCat ( (char *) hdrValue,
                           CMGR_USER_TIMESTAMP_URI,
                           2 * CMGR_URI_STRING_LEN );
               strPrintf( (char *) timeStamp, 12, "%u", pCdr->timeStart );
               strMaxCat ( (char *) hdrValue,
                           (const char *) timeStamp,
                           2 * CMGR_URI_STRING_LEN );
               strMaxCat ( (char *) hdrValue,
                           ">",
                           2 * CMGR_URI_STRING_LEN );

               hdrCfg.hdr[0].hdr.eType = eCCTKHDRTYPE_PRIVATE;
               hdrCfg.hdr[0].hdr.pcName =
                              (BOS_UINT8 *)CMGR_PRIVATE_DCS_TRACE_PARTY_ID;
               hdrCfg.hdr[0].hdr.pcValue = hdrValue;

               cctkSetParm ( pSes->uNet,
                             eCCTKPARM_EC_GENHDRCFG,
                             (void *) &hdrCfg );
            }
         }
         break;

         case eCMDM_CIDSUPPRESS:
         case eCMDM_CIDDELIVER:
         {
            CCTKPRIVCFG privCfg;
            CFGSIPCID *pCfgCid = NULL;

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDConcept ),
                                       &cfgIdx,
                                       (void *) &pCfgCid,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               privCfg.bCritical = BOS_TRUE;
               if ( pCfgCid )
               {
                  privCfg.bCritical = !pCfgCid->bNotCritical;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDConcept ),
                                     &cfgIdx,
                                     (void *) pCfgCid,
                                     CFG_ITEM_DEL );
               pCfgCid = NULL;
            }

            privCfg.bRetry = BOS_TRUE;
            privCfg.bRejectAnon = BOS_FALSE;

            switch ( pSes->ses.med.eAction )
            {
               /* Caller Identification Delivery per Call.
               */
               case eCMDM_CIDDELIVER:
               {
                  privCfg.eLevel = eCCTKPRIVLVL_NONE;
               }
               break;

               /* Caller Identification Blocking per Call.
               */
               case eCMDM_CIDSUPPRESS:
               {
                  privCfg.eLevel = eCCTKPRIVLVL_NETHDR;
               }
               break;

               default:
               break;
            }

            cctkSetParm ( pSes->uNet,
                          eCCTKPARM_EC_CALLPRIV,
                          (void *) &privCfg );
         }
         break;

         default:
         break;
      }

      cmCfgInitCallMedia ( pRes, pSes );

      cctkCallMake ( pSes->uNet );
#if CMGRCFG_MULTILINE
      cmCctkAccntUnforce( pSes->uNetUsr, pSes->uUsr );
#endif /* CMGRCFG_MULTILINE */

      cmFeatFaxOnly ( pRes );

      /* Restore default configuration *AFTER* calling the cctkCallMake API.  This
      ** ensures that the configuration is applied to the call of interest.  This is
      ** necessary due to the current limitation of some of the process which do not
      ** allow per-call configuration but only per-user configuration.  This will
      ** be changed later on.
      */
      switch ( eAction )
      {
         case eCMDM_EMERGENCYCALL:
         {
            CCTKEMGCALLCFG emCfg;

            memset ( &emCfg, 0, sizeof ( CCTKEMGCALLCFG ) );
            cctkSetParm ( uAccount,
                          eCCTKPARM_EC_EMGCALLCFG,
                          (void *) &emCfg );
         }
         break;

         default:
         break;
      }
   }
   else
   {
      pUsr->evt.uDat2 = pSes->uHdl;

      cmFsmCallEnd ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFeatError
**
** PURPOSE:    Generic handler to manage an error reported against a feature when
**             this latter is not activated.  This is a specific handler which
**             assumes there is way to handle such error as configured by the digit
**             map.  If no such handling is possible, the error handling is left up
**             to the application.
**
** PARAMETERS: pRes - the resource to apply this action on.
**             eAction - the action from the digit map that is currently processed.
**
** RETURNS:    BOS_TRUE if the error on the action was handled according to the
**             digit map rules.
**             BOS_FALSE otherwise, the error on the action is to be handled
**             locally.
***********************************************************************************/
BOS_BOOL cmFeatError ( void *pRes, CMDMACTIONTYPE eAction )
{
   BOS_BOOL bRet = BOS_FALSE;
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   BOS_UINT8 *pActParm = NULL;
   CMDMSTATUS eDmStatus;
   BOS_UINT8 featUri[ CMGR_URI_STRING_LEN + 1 ];
   BOS_UINT32 uAccount = ( pSes && ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) ) ?
                           pSes->uNetUsr : cmUsrGetNtwkHdl( pUsr ) ;

   CMGRLOG_INFORMATION (( "cmFeatError: 0x%X (0x%X) on 0x%X (0x%X)",
                          pUsr->uHdl, uAccount,
                          pSes ? pSes->uHdl : CMGR_INVALID_HANDLE,
                          pSes ? pSes->uNet : CMGR_INVALID_HANDLE));

   memset ( featUri,
            0,
            sizeof ( featUri ) );

   switch ( eAction )
   {
      case eCMDM_ACACTIVATE:
      case eCMDM_ACDEACTIVATE:
      case eCMDM_ARACTIVATE:
      case eCMDM_ARDEACTIVATE:
      case eCMDM_COTACTIVATE:
      {
         /* If an error handler is associated with this action, it will be the
         ** second parameter associated with the action, so trim off one parameter
         ** first.
         */
         eDmStatus = cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                        &pActParm );
         if ( ! (( eDmStatus == eCMDM_SUCCESS ) &&
                 ( pActParm != NULL )) )
         {
            break;
         }
      }
      /* WARNING: Fall through.  Intentional to continue processing the
      **          action.
      */
      case eCMDM_CIDSUPPRESS:
      case eCMDM_CIDDELIVER:
      case eCMDM_CFPROGRAM:
      case eCMDM_CFACTIVATE:
      case eCMDM_CFDEACTIVATE:
      case eCMDM_CNDBTOGGLE:
      case eCMDM_CWTOGGLE:
      case eCMDM_HOLDACTIVATE:
      /* Those actions may also carry a default error handler.
      */
      case eCMDM_ACTIVATEDCHECK:
      case eCMDM_DEACTIVATEDCHECK:
      case eCMDM_FEATURECHECK:
      {
         /* Determine whether there is a failure action associated with this
         ** feature.  If so, it must be the URI of the failure handler.
         */
         eDmStatus = cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                        &pActParm );
         if ( ( eDmStatus == eCMDM_SUCCESS ) &&
              ( pActParm != NULL ) )
         {
            strMaxCat ( (char *) featUri,
                        (const char *) pActParm,
                        CMGR_URI_STRING_LEN );

            /* The failure action URI may simply be a local tone of announcement
            ** file to be played in this particular scenario, check for that
            ** condition.
            */
            if ( ( pUsr->uCfgTone =
                     cmPhyToneFromString ( featUri ) ) == CMGR_INVALID_HANDLE )
            {
               bRet = BOS_TRUE;
            }
         }
      }
      break;

      default:
      break;
   }

   if ( bRet )
   {
      /* There is a valid remotely specified error handler, place a call
      ** to it now.
      */
      if ( pSes && strlen( (const char *) featUri ))
      {
         /* In case it hasn't already hasn't been set, assosciate the
         ** the network user with this session, before making any
         ** outgoing calls.
         */
         pSes->uNetUsr = uAccount;

         if ( cctkCallCreate ( &pSes->uNet,
                              uAccount,
                              featUri ) == eCCTKSTATUS_SUCCESS )
         {
            CMGRLOG_DEBUG (( "cmFeatError: \'%s\' on 0x%X (0x%X)",
                             featUri, pUsr->uHdl, pSes->uNetUsr ));

            cmCfgInitCallMedia ( pRes, pSes );

            cctkCallMake ( pSes->uNet );
#if CMGRCFG_MULTILINE
            cmCctkAccntUnforce( pSes->uNetUsr, pSes->uUsr );
#endif /* CMGRCFG_MULTILINE */
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

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmFeatApplyAction
**
** PURPOSE:    Feature handler to apply an action from the digit manipulation
**             output.
**
** PARAMETERS: pRes - the resource to apply this action on.
**             eAction - the action to execute.
**
** RETURNS:    BOS_TRUE if the action is successfully processed and one can
**             continue with other successive actions following.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatApplyAction ( void *pRes, CMDMACTIONTYPE eAction )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   PROV_CFG_IDX cfgIdx;
   CMTMR tmr;
   BOS_BOOL bRet = BOS_TRUE;
   CCTKPRIVCFG privCfg;

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   switch ( eAction )
   {
      case eCMDM_MAKECALL:
      {
         /* The long interdigit timer may still be running if we came here
         ** because of a short interdigit timer expiry.
         */
         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = pSes->uHdl;
         tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
         cmTmrStop ( &tmr );

         /* If there was a pending delayed action, apply it now.
         */
         if ( ( pSes->ses.med.eAction == eCMDM_CIDSUPPRESS ) ||
              ( pSes->ses.med.eAction == eCMDM_CIDDELIVER ) )
         {
            cmFeatSend ( pRes, pSes->ses.med.eAction );
         }
         else
         {
            BOS_UINT8 *pActParm;
            BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

            cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                               &pActParm );

            pSes->uNetUsr = uAccount;

            /* If we couldn't find a valid network handle for this call,
            ** we should play a re-order tone and wait for on-hook or hookflash.
            */
            if ( uAccount == CMGR_INVALID_HANDLE )
            {
               CMGREPSIG sig;

               memset ( &sig, 0, sizeof ( CMGREPSIG ) );

               sig.eAct = eCMGREPSIGACT_START;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
               sig.uLine = CMGR_INVALID_HANDLE;
#endif
               sig.uSig = eEPTSIG_REORDER;
               pUsr->uCpt = sig.uSig;

               cmPhySignal ( pUsr->uPhys, &sig );

               /* Don't start offhook warning timer if there are
               ** other sessions for this user.
               */
               if( ( pUsr->pSesWait == NULL ) && ( pUsr->pSesHeld == NULL ) )
               {
                  tmr.uSes = CMGR_INVALID_HANDLE;
                  tmr.uUsr = pUsr->uHdl;
                  tmr.eType = eCMEVT_TMR_OHWARN;
                  tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;

                  cmTmrStart ( &tmr );
               }

               pUsr->evt.uDat2 = pSes->uHdl;
               cmFsmCallEnded( pUsr );
               break;
            }

            if ( cctkCallCreate ( &pSes->uNet,
                                  uAccount,
                                  pActParm ) == eCCTKSTATUS_SUCCESS )
            {
#if CMGRCFG_MULTILINE
               if( !pSes->bEstab )
               {
                  cmCctkAccntCallAdd( pSes->uNetUsr );
                  pSes->bEstab = BOS_TRUE;
               }
#endif /* CMGRCFG_MULTILINE */

               if ( gCmCfgCb.cmgrEvtCb )
               {
                  CMGRCALLDATA tData;
                  CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );

                  tData.eType = eCMGRCALLTYPE_OUTGOING;
                  tData.uSrc = CMGR_INVALID_HANDLE;

                  ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_NEW,
                                           pSes->uNet,
#if CMGRCFG_MULTILINE
                                           pUsr->uHdl,
                                           (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                           (void *) &tData );
               }

               if ( cmFeatAssignPrivacy ( pRes, &privCfg ) )
               {
                  cctkSetParm ( pSes->uNet,
                                eCCTKPARM_EC_CALLPRIV,
                                (void *) &privCfg );
               }

               cmCfgInitCallMedia ( pRes, pSes );

               cctkCallMake ( pSes->uNet );
#if CMGRCFG_MULTILINE
               cmCctkAccntUnforce( pSes->uNetUsr, pSes->uUsr );
#endif /* CMGRCFG_MULTILINE */

               cmFeatFaxOnly ( pRes );
            }
            else
            {
               cmFsmCallEnd ( pRes );
            }
         }
      }
      break;

      case eCMDM_MAKECALLINT:
      {
         BOS_UINT8 *pActParm;
         BOS_UINT32 uIx;
         CMUSRLIST usrList;
         CMEVT evt;
         BOS_BOOL bGeneralCall = BOS_FALSE;

         memset( &usrList, 0, sizeof( CMUSRLIST ) );

         /* The long interdigit timer may still be running if we came here
         ** because of a short interdigit timer expiry.
         */
         tmr.uUsr = pUsr->uHdl;
         tmr.uSes = pSes->uHdl;
         tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
         cmTmrStop ( &tmr );

         cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                            &pActParm );

         pSes->uNetUsr = CMGR_INVALID_HANDLE; /* No CCTK user */
         pSes->uNet = cctkGenerateId(); /* Generate a unique call id */

         /* Get the internal call concept for this originating cmUsr */
         CFGSIPINTERNALCALL * pCfgSipInternalCall = NULL;
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pUsr->uUid;
         cfgIdx.uIndex = pUsr->uPhys;

         /* Reject outgoing internal call if cannot retrieve concept */
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, InternalCallConcept ),
                                    &cfgIdx,
                                    (void *) &pCfgSipInternalCall,
                                    CFG_ITEM_GET ) != BOS_STATUS_OK )
         {
            /* reject call if internal call feature not present */
            cmFsmCallReject(pRes);
            break;
         }

         if( !pSes->bEstab )
         {
            pSes->bEstab = BOS_TRUE;
         }

         /* If the user doesn't dial "*" as the target terminal, then
         ** this was a call to specific terminals.
         */
         if( ( pActParm[0] != '*' ) )
         {
            char* pNumber = (char*)pActParm;
            char* pDelim = NULL;
            char* pCol = NULL;

            pCol = strchr( (char*)pActParm, ':' );
            if( pCol == NULL )
            {
               CMGRLOG_CRITICAL(( "%s: No \':\' in dialstring! Failing call.", __FUNCTION__ ));
               break;
            }

            *pCol = '\0';
            pCol++;

            /* Set the sessions' collected digits to the extra dialstring */
            memset( pSes->ses.med.uDigCol, 0, (CMGR_COLLECTED_DIGIT_LEN + 1) * sizeof( BOS_UINT8 ) );
            snprintf( (char*)&( pSes->ses.med.uDigCol[ 0 ] ), CMGR_COLLECTED_DIGIT_LEN, "%s", pCol );

            while( pNumber != NULL )
            {
               CMUSR* pOtherUsr = NULL;

               pDelim = strchr( pNumber, '*' );
               if( pDelim != NULL )
               {
                  *pDelim = '\0';
               }

               /* Attempt to find the other user */
               pOtherUsr = cmUsrFindPhyHdl ( atoi(pNumber) );
               /* Make sure this user is not the calling user */
               if( pOtherUsr && pOtherUsr->uHdl != pSes->uUsr )
               {
                  /* Ensure we aren't adding a user more than once to the list */
                  for( uIx = 0; uIx < usrList.uLength; uIx++ )
                  {
                     if( usrList.uUsr[ uIx ] == pOtherUsr->uHdl )
                     {
                        break;
                     }
                  }
                  if( uIx >= usrList.uLength )
                  {
                     /* If the user didn't exist in the list, add it */
                     usrList.uUsr[ usrList.uLength ] = pOtherUsr->uHdl;
                     usrList.uLength++;
                  }
               }

               /* If there were no more delimiters, end the loop */
               if( pDelim == NULL )
               {
                  pNumber = NULL;
               }
               else
               {
                  pNumber = (char*)(pDelim+1);
               }
            }
         }
         /* Otherwise, we need to make an internal call to all terminals.
         */
         else
         {
            /* Get a list of all users */
            cmUsrGetAll( &usrList, pUsr );
            bGeneralCall = BOS_TRUE;
         }

         if( usrList.uLength == 0 )
         {
            /* If there are no users to ring, we should play a busy tone */
            cmFsmCallBusy( pRes );
            break;
         }

         /* Notify the application of the new outgoing call */
         if ( gCmCfgCb.cmgrEvtCb )
         {
            CMGRCALLDATA tData;

            tData.eType = eCMGRCALLTYPE_OUTGOING;
            tData.uSrc = CMGR_INVALID_HANDLE;

            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_NEW,
                                     pSes->uNet,
#if CMGRCFG_MULTILINE
                                     pUsr->uHdl,
                                     CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_CATIQ_2_0 */
                                     (void *) &tData );
         }

         /* Iterate through our user list and call each terminal in the list */
         for( uIx = 0 ; uIx < usrList.uLength ; uIx++ )
         {
            BOS_BOOL* pbEmergency;
            CCTKCALLINFO *pCallInfo;
            CMUSR* pOtherUsr = NULL;

            /* Don't send an incoming call request to ourselves */
            if( usrList.uUsr[ uIx ] == pUsr->uHdl )
            {
               continue;
            }

            pOtherUsr = cmUsrValid( usrList.uUsr[ uIx ] );
            if( !pOtherUsr )
            {
               CMGRLOG_CRITICAL(( "%s: No user found with id [%X]", __FUNCTION__, (unsigned int)usrList.uUsr[ uIx ] ));
               continue;
            }
            else if( pOtherUsr->eType == eCMUSRTYPE_PSTN && bGeneralCall )
            {
               /* We don't want to route to the PSTN terminal in a general internal call. */
               CMGRLOG_DEBUG(( "%s: Skip routing to FXO terminal [%X]", __FUNCTION__, (unsigned int)usrList.uUsr[ uIx ] ));
               continue;
            }
            else if( pOtherUsr->bInSrv == BOS_FALSE )
            {
               /* We don't want to route to the users/terminals which are not in service */
               CMGRLOG_DEBUG(( "%s: Skip routing to out-of-service terminal [%X]", __FUNCTION__, (unsigned int)usrList.uUsr[ uIx ] ));
               continue;
            }

            pbEmergency = malloc( sizeof( BOS_BOOL ) );
            pCallInfo = malloc ( sizeof ( CCTKCALLINFO ) );

            memset ( &evt, 0, sizeof( CMEVT ) );
            memset( pbEmergency, 0, sizeof( BOS_BOOL ) );
            memset( pCallInfo, 0, sizeof( CCTKCALLINFO ) );

            *pbEmergency = BOS_FALSE;

            snprintf( (char*)pCallInfo->clid.cName,
                      CCTK_CLID_LEN,
                      "%s",
                      (char*)pCfgSipInternalCall->pcIntCallClidName );
            snprintf( (char*)pCallInfo->clid.cNumber,
                      CCTK_CLID_LEN,
                      "%s",
                      (char*)pCfgSipInternalCall->pcIntCallClidNumber );
            pCallInfo->bEmergency = BOS_FALSE;
            pCallInfo->bLoopBack = BOS_FALSE;

            /* Send two "dummy" CCTK events. INCOMING_CALL will
            ** trigger session creation for the terminal and
            ** ALERT_CALL will cause the terminal to start
            ** ringing/play CW.
            */
            evt.uEvt = ( eCMEVT_INCOMING_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
            evt.uDat1 = pOtherUsr->uHdl;     /* User handle */
            evt.uDat2 = pSes->uNet;          /* Call ID */
            evt.pDat3 = pbEmergency;         /* pData */
            evt.uDat4 = CMGR_INVALID_HANDLE; /* CCTK user id */
            evt.bLast = BOS_TRUE;            /* Last event */

            if( cmPostEvt ( &evt ) == BOS_STATUS_OK )
            {
               cmSignalEvt ();
            }

            memset ( &evt, 0, sizeof( CMEVT ) );

            evt.uEvt = ( eCMEVT_ALERT_CALL | CMGR_EVT_CLASS__CCTK );
            evt.uDat1 = pOtherUsr->uHdl;
            evt.uDat2 = pSes->uNet;
            evt.pDat3 = pCallInfo;
            evt.uDat4 = CMGR_INVALID_HANDLE;
            evt.bLast = BOS_TRUE;

            if( cmPostEvt ( &evt ) == BOS_STATUS_OK )
            {
               cmSignalEvt ();
            }
         }

         /* Send a eCMEVT_RINGING_CALL event to this user to
         ** start ringback and place the user in the correct
         ** state.
         */
         memset ( &evt, 0, sizeof( CMEVT ) );

         evt.uEvt = ( eCMEVT_RINGING_CALL | CMGR_EVT_CLASS__CCTK );
         evt.uDat1 = pUsr->uHdl;
         evt.uDat2 = pSes->uHdl;
         evt.uDat4 = CMGR_INVALID_HANDLE;
         evt.bLast = BOS_TRUE;

         if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, InternalCallConcept ),
                               &cfgIdx,
                               (void *) pCfgSipInternalCall,
                               CFG_ITEM_DEL );
         pCfgSipInternalCall = NULL;
      }
      break;

      case eCMDM_REORDER:
      {
         cmFsmCallEnd ( pRes );

         /* Do not process any further action.
         */
         bRet = BOS_FALSE;
      }
      break;

      case eCMDM_RECALL:
      {
         CMUSR *pUsr = (CMUSR *) pRes;

         /* Ensure that we have a valid resource to apply this
         ** action.  This is to eliminate potential 'random' action
         ** from the digit map that may be out of place due to a
         ** misconfiguration of the latter.
         */
         if ( cmUsrFocusSes ( pUsr ) )
         {
            cmFeatRecall ( pRes );
         }
         else
         {
            CMGRLOG_WARNING ((
               "cmFeatApplyAction: %s (0x%X) on 0x%X (0x%X) - erroneous, ignoring",
               (char *) cmMapGetStr ( cmMapDmAction, eAction ),
               eAction, pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));
         }
      }
      break;

      /* Check feature activation state prior to doing
      ** anything about the feature.
      */
      case eCMDM_ACACTIVATE:
      case eCMDM_ACDEACTIVATE:
      case eCMDM_ACRACTIVATE:
      case eCMDM_ACRDEACTIVATE:
      case eCMDM_ARACTIVATE:
      case eCMDM_ARDEACTIVATE:
      case eCMDM_CIDSUPPRESS:
      case eCMDM_CIDDELIVER:
      case eCMDM_CFPROGRAM:
      case eCMDM_CFACTIVATE:
      case eCMDM_CFDEACTIVATE:
      case eCMDM_CBACTIVATEALL:
      case eCMDM_CBACTIVATEDM:
      case eCMDM_CBDEACTIVATE:
      case eCMDM_CNDBTOGGLE:
      case eCMDM_COTACTIVATE:
      case eCMDM_CWTOGGLE:
      case eCMDM_DNDACTIVATE:
      case eCMDM_DNDDEACTIVATE:
      case eCMDM_EMERGENCYCALL:
      case eCMDM_HOLDACTIVATE:
      case eCMDM_HOOKFLASHACTIVATE:
      case eCMDM_HOOKFLASHDEACTIVATE:
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
      case eCMDM_WARMLINE_DM_ACTIVATE:
      case eCMDM_WARMLINE_DM_DEACTIVATE:
#endif
      {
         CMUSR *pUsr = (CMUSR *) pRes;

         /* Ensure that we have a valid resource to apply this
         ** action.  This is to eliminate potential 'random' action
         ** from the digit map that may be out of place due to a
         ** misconfiguration of the latter.
         **
         ** Check that the feature is enabled in order to be
         ** applied.
         */
         if ( cmUsrFocusSes ( pUsr ) &&
              cmFeatEnabled ( pRes, eAction ) )
         {
            /* Some action may trigger an end to the processing of
            ** further digit map action, if so let it be known.
            */
            bRet = cmFeatAccept ( pRes, eAction );
         }
         else
         {
            cmFeatReject ( pRes, eAction );
            /* Stop processing following actions (if any).
            */
            bRet = BOS_FALSE;
         }
      }
      break;

      /* Actions which are triggering a lookup on a specific feature.
      **
      ** Those actions are always processed, the following lookup on the
      ** given feature via the feature index information will dictate what
      ** to do next.
      */
      case eCMDM_FEATURECHECK:
      case eCMDM_ACTIVATEDCHECK:
      case eCMDM_DEACTIVATEDCHECK:
      {
         /* Retrieve the information about the feature being dealt with.
         */
         BOS_UINT8 *pActParm = NULL;
         CMDMSTATUS eDmStatus;
         PROV_CFG_ITEM *pCfgItem = NULL;
         CFGSIPACTSTATUS *pCfgActStatus = NULL;
         BOS_BOOL bCfv = BOS_FALSE;

         /* By default, assume the check on activation is going to fail, in
         ** such case we will stop processing any further action.
         */
         bRet = BOS_FALSE;

         /* First parameter must be the feature identifier.
         */
         eDmStatus = cmDmGetParameter ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                        &pActParm );
         if ( ( eDmStatus == eCMDM_SUCCESS ) &&
              ( pActParm != NULL ) )
         {
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            cfgIdx.uIndex = atoi ( (const char *) pActParm );
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, FeatureCfg ),
                                       &cfgIdx,
                                       (void *) &pCfgItem,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pCfgItem &&
                    (*pCfgItem != CFGITEM( CFG, Unknown )) )
               {
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = pUsr->uUid;
                  /* The feature index returned by the previous query.
                  */
                  cfgIdx.uIndex = *pCfgItem;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, FeatureAct ),
                                             &cfgIdx,
                                             (void *) &pCfgActStatus,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pCfgActStatus &&
                       /* Activated or Feature Check Action.
                       ** Look for the features configured availability.
                       */
                        ( ( ( eAction == eCMDM_ACTIVATEDCHECK || eAction == eCMDM_FEATURECHECK ) &&
                              pCfgActStatus->bCfgStatus ) ||
                             /* Deactivated Check Action.  Look for the feature
                             ** being configured and inactive.
                             */
                            ( ( eAction == eCMDM_DEACTIVATEDCHECK ) &&
                              !pCfgActStatus->bCfgStatus ) ) )
                     {
                        /* The checked action is successful.
                        */
                        bRet = BOS_TRUE;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, FeatureAct ),
                                           &cfgIdx,
                                           (void *) pCfgActStatus,
                                           CFG_ITEM_DEL );
                     pCfgActStatus = NULL;
                  }

                  /* Keep track whether this feature was a call forward as we
                  ** need to associated the session with that knowledge.
                  */
                  if ( *pCfgItem == CFGITEM( SIP, CallFwdConcept ) )
                  {
                     bCfv = BOS_TRUE;
                  }
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, FeatureCfg ),
                                     &cfgIdx,
                                     (void *) pCfgItem,
                                     CFG_ITEM_DEL );
               pCfgItem = NULL;
            }
         }

         /* If successful, we will continue processing the next action from the
         ** digit map.
         */
         if ( bRet )
         {
            CMGRLOG_DEBUG (( "cmFeatApplyAction: %s (0x%X) on 0x%X (0x%X) - pass",
                             (char *) cmMapGetStr ( cmMapDmAction, eAction ),
                             eAction, pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

            if ( bCfv )
            {
               pSes->ses.med.bCfv = BOS_TRUE;
            }
         }
         /* If failed, check whether there is an error handler for this
         ** action.  If so, we will apply it, otherwise we will apply the
         ** default local error handling (end the call locally by applying
         ** reorder tone).
         */
         else
         {
            if ( !cmFeatError ( pRes, eAction ) )
            {
               cmFsmCallEnd ( pRes );
            }
         }
      }
      break;

      /* Invalid or not handled in this state.  Continue to process
      ** other actions nevertheless if any.
      */
      case eCMDM_UNKNOWNACTION:
      default:
      break;
   }

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmFeatIgnoreFlash
**
** PURPOSE:    Check whether a recorded flash event can be applied to any of the
**             configured features for this user.
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if the flash event can be ignored, which means there is
**             no valid feature at this time for the user that would require the
**             use of flash, hence it should be ignored.
**             BOS_FALSE otherwise.
**
** NOTE:       This function does not make assumption on the current status of
**             the user, i.e. if the flash occurs for the first time on the first
**             active call or if it happens for a subsequent call with the initial
**             call already on hold.  It does not seem like we need to make such
**             fine decision right now, but perhaps in the future we will if the
**             user application requires this kind of smarts.
***********************************************************************************/
BOS_BOOL cmFeatIgnoreFlash ( void *pRes )
{
   BOS_BOOL bRet = BOS_TRUE;
   BOS_BOOL bApply = BOS_FALSE;
   BOS_BOOL bDisHotlineHf = BOS_FALSE;
   CMUSR *pUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGSIPHOTLINE *pHotLineCfg = NULL;
   CFGSIPCALLWAIT *pCwCfg = NULL;
   CMSES *pSes = cmUsrFocusSes ( pUsr );

   /* The features that are enabled for a user that would lead to using flash
   ** are:
   **          - Call Waiting.
   **          - Call Hold.
   **          - Call Transfer.
   **          - Call Conference (3-way).
   **          - Cancel Call Waiting (special case).
   **          - CAT_IQ Parallel call (internal/external create, etc.)
   **
   ** If any of the above feature is enabled, then flash is a valid action to
   ** process.  If none of the above is enabled, then flash could be ignored as
   ** it would lead to no further action for this user. Additionally, if the
   ** current session is a hotline session and the Hotline feature is
   ** configured to not allow hookflash features, then the flash must be ignored.
   **
   ** The above process is only applied if configured to do so.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;

   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                              &cfgIdx,
                              (void *) &pBasicCall,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pBasicCall &&
           pBasicCall->bIgnoreHfNoFeat &&
           !pBasicCall->bHfReported)
      {
         bApply = BOS_TRUE;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                            &cfgIdx,
                            (void *) pBasicCall,
                            CFG_ITEM_DEL );
      pBasicCall = NULL;
   }

#if CMGRCFG_MULTILINE
   if( pUsr->bDisableAutoCallSwitch == BOS_TRUE )
   {
      /* If we are pending a parallel call operation, allow the flash to continue */
      return BOS_FALSE;
   }
#endif /* CMGRCFG_MULTILINE */

   /* Determine if the Hotline feature disables hookflash.
   */
   if ( pSes &&
        pSes->ses.med.bHotline )
   {
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                                 &cfgIdx,
                                 (void *) &pHotLineCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pHotLineCfg && pHotLineCfg->Act.bCfgStatus )
         {
            bDisHotlineHf = pHotLineCfg->bDisableHfFeat;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, HotLineConcept ),
                               &cfgIdx,
                               (void *) pHotLineCfg,
                               CFG_ITEM_DEL );
         pHotLineCfg = NULL;
      }
   }

   /* If Hotline is active and configured to not allow hookflash, no further
   ** processing is needed.
   */
   if( bDisHotlineHf )
   {
      return BOS_TRUE;
   }

   /* Hotline is not enabled or does allow hookflash, check the actual hookflash
   ** features now.
   */
   if ( bApply )
   {
      /* Is any non 'call-waiting' feature enabled?
      */
      if ( cmFeatFlashNonCw ( pRes ) )
      {
         bRet = BOS_FALSE;
      }
      /* Is there a valid 'call-waiting' situation?
      */
      if ( bRet &&
           cmFeatEnabled ( pRes, eCMDM_CWTOGGLE ) )
      {
         /* Already in a 'call-waiting' scenario?
         */
         if ( pUsr->pSesWait != NULL)
         {
            bRet = BOS_FALSE;
         }

         /* No current 'call waiting' scenario, but allowed to have one
         ** and 'cancel call waiting' can be applied.
         */
         else if ( pUsr->bPerCallCW )
         {
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                                       &cfgIdx,
                                       (void *) &pCwCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               /* Allowed to apply 'cancel call waiting'. */
               if ( pCwCfg && pCwCfg->bCallWaitCancel )
               {
                  bRet = BOS_FALSE;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                                     &cfgIdx,
                                     (void *) pCwCfg,
                                     CFG_ITEM_DEL );
               pCwCfg = NULL;
            }
         }
         /* A 'call waiting' scenario is not allowed but the feature is
         ** still available.
         */
         else if ( !pUsr->bPerCallCW )
         {
            bRet = BOS_FALSE;
         }
      }
   }
   else
   {
      /* If the feature is not applicable, never ignore hook flash.
      */
      bRet = BOS_FALSE;
   }

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmFeatReportFlash
**
** PURPOSE:    Check whether a recorded flash event should be reported via SIP INFO
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if the flash event is notified via SIP INFO instead of being
**             processed
**             BOS_FALSE otherwise.
**
** NOTE:
***********************************************************************************/
BOS_BOOL cmFeatReportFlash ( void *pRes )
{
   BOS_BOOL bReport = BOS_FALSE;
   CMUSR *pUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   CFGSIPBASICCALL *pBasicCall = NULL;
   BOS_UINT8 hfMimeType[ CCTK_STR_LEN ];
   BOS_UINT8 hfContent[ CCTK_STR_LEN ];

   CMSES *pSes = cmUsrFocusSes ( pUsr );

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;

   memset ( hfMimeType, 0, sizeof(hfMimeType) );
   memset ( hfContent, 0, sizeof(hfContent) );

   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                              &cfgIdx,
                              (void *) &pBasicCall,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pBasicCall &&
           pBasicCall->bHfReported )
      {
         bReport = BOS_TRUE;
         strcpy(hfMimeType, pBasicCall->pcHFMIMEType);
         strcpy(hfContent, pBasicCall->pcHFInfoContent);
     }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                            &cfgIdx,
                            (void *) pBasicCall,
                            CFG_ITEM_DEL );
      pBasicCall = NULL;
   }

#if CMGRCFG_MULTILINE
   if( pUsr->bDisableAutoCallSwitch == BOS_TRUE )
   {
      /* If we are pending a parallel call operation, allow the flash to continue */
      return BOS_FALSE;
   }
#endif /* CMGRCFG_MULTILINE */

   /* check if the hook flash event will result in an INFO message instead of being
   ** processed
   */
   if (bReport)
   {
      cctkCallFlashInfo ( pSes->uNet, hfMimeType, hfContent);
   }

   return bReport;
}


/***********************************************************************************
** FUNCTION:   cmFeatCcw
**
** PURPOSE:    Check whether we are in a special situation for applying the
**             'cancel call waiting' feature.  The 'cancel call waiting'
**             feature is applicable when the user is already engaged in an
**             existing stable two-ways call and it attempts to toggle the
**             call waiting indication.
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if we are in the case where 'cancel call waiting' should
**             apply.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatCcw ( void *pRes )
{
   BOS_BOOL bCcw = BOS_FALSE;

   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pActiveSes = cmUsrActiveSes ( pUsr );

   if ( pActiveSes &&
        pActiveSes->bEstab &&
        ( pActiveSes != pUsr->pSesHeld ) &&
        ( pActiveSes != pUsr->pSesWait ) )
   {
      bCcw = BOS_TRUE;
   }

   return bCcw;
}


/***********************************************************************************
** FUNCTION:   cmFeatFaxOnly
**
** PURPOSE:    Check whether we are dealing with a physical resource that is setup
**             for fax only purpose.  In such case, when a call is either placed
**             or received using this resource, we only expect to handle fax call
**             as such, we would start a timer waiting for valid fax event on the
**             resource.  If none is detected following expiration of the timer, we
**             would terminate the call to prevent fraudulent audio call.
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatFaxOnly ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes( pUsr );
   CMTMR tmr;
   CFGEPTLINECFG *pEptLineCfg = NULL;
   PROV_CFG_IDX cfgIdx;

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   if ( pSes )
   {
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.uIndex = pUsr->uPhys;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, LineConcept ),
                                 &cfgIdx,
                                 (void *) &pEptLineCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         /* The line associated with this call is a fax only line, start the
         ** timer awaiting for fax event on that line.
         */
         if ( pEptLineCfg && pEptLineCfg->bFaxOnly )
         {
            tmr.eType = eCMEVT_TMR_FAX_EVENT;
            tmr.uUsr = pUsr->uHdl;
            tmr.uSes = pSes->uHdl;
            tmr.uTimeOut = pEptLineCfg->uFaxWaitTime;

            cmTmrStart ( &tmr );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, LineConcept ),
                               &cfgIdx,
                               (void *) pEptLineCfg,
                               CFG_ITEM_DEL );
         pEptLineCfg = NULL;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFeatDialStrManip
**
** PURPOSE:    Apply feature specific digit string manipulation.  This function
**             is meant to be used to see whether a given dial string needs to
**             be morphed to a different one prior to digit map processing.
**
** PARAMETERS: pUsrRes - the user resource to apply this on.
**             pSesRes - the session resource associated to the user (can be
**                       NULL) to apply this on.  Only relevant when the dialing
**                       is associated with a session rather than with the user.
**
** RETURNS:    BOS_TRUE if the dial string was manipulated (i.e. changed in any
**             ways within this function call), BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatDialStrManip ( void *pUsrRes, void *pSesRes )
{
   BOS_BOOL bRet = BOS_FALSE;
   CMUSR *pUsr = (CMUSR *) pUsrRes;
   PROV_CFG_IDX cfgIdx;
   CFGSIPLOCALSPEEDDIALING *pLclSpeedDial = NULL;

   if ( pUsr != NULL )
   {
      BOS_UINT8 *pCurDig =
         (pSesRes != NULL) ?
            ((CMSES *)pSesRes)->ses.med.uDigCol : pUsr->uDigCol;
      BOS_UINT32 *pCurCnt =
         (pSesRes != NULL) ?
            &(((CMSES *)pSesRes)->ses.med.uDigCnt) : &(pUsr->uDigCnt);

      /* The only feature that we currently implement that is able to
      ** swap the current collected digits with a new set is the local
      ** speed dial feature.
      */
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, LocalSpeedDialingConcept ),
                                 &cfgIdx,
                                 (void *) &pLclSpeedDial,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pLclSpeedDial )
         {
            CFGSIPLOCALSPEEDDIALENTRY *pSpDialCur = pLclSpeedDial->pSpeedDialList;
            BOS_UINT8 uDigCol[CMGR_COLLECTED_DIGIT_LEN + 1];
            BOS_UINT8 uTerm[2];
            BOS_UINT8 uPound =
               cmMapGetEvt ( cmMapCmEvt2DigCol, eCMEVT_DTMF_HASH );

            while ( pSpDialCur && !bRet )
            {
               /* Try to match the current string against the local speed
               ** dial code appended with an inter-digit timeout termination.
               */
               memset ( (void *) uDigCol, 0, sizeof ( uDigCol ) );
               sprintf ( (char *) uTerm, "%c",
                         (char) CMDM_INTERDIGIT_TMR_SHORT );
               strMaxCat ( (char *) uDigCol,
                           (const char *) pSpDialCur->pcSpeedDialLocalMapCode,
                           CMGR_COLLECTED_DIGIT_LEN + 1 );
               strMaxCat ( (char *) uDigCol,
                           (const char *) uTerm,
                           CMGR_COLLECTED_DIGIT_LEN + 1 );
               if ( !strcmp ( (const char *) pCurDig,
                              (const char *) uDigCol ) )
               {
                  /* Match found.  Replace the current string and
                  ** exit processing.
                  */
                  CMGRLOG_INFORMATION ((
                     "cmFeatDialStrManip: 0x%X (0x%X) - set \'%s\' to \'%s\'",
                     pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ),
                     pCurDig, pSpDialCur->pcSpeedDialLocalMapDigitString ));

                  memset ( (void *) pCurDig, 0, CMGR_COLLECTED_DIGIT_LEN + 1 );
                  *pCurCnt =
                     (BOS_UINT32) strPrintf ( (char *) pCurDig,
                                              CMGR_COLLECTED_DIGIT_LEN,
                                              "%s",
                                              (const char *) pSpDialCur->
                                                   pcSpeedDialLocalMapDigitString );
                  bRet = BOS_TRUE;
               }

               /* Further try to match the current string against the local speed
               ** dial code appended with a pound termination.
               */
               if ( !bRet )
               {
                  memset ( (void *) uDigCol, 0, sizeof ( uDigCol ) );
                  sprintf ( (char *) uTerm, "%c",
                            (char) uPound );
                  strMaxCat ( (char *) uDigCol,
                              (const char *) pSpDialCur->pcSpeedDialLocalMapCode,
                              CMGR_COLLECTED_DIGIT_LEN + 1 );
                  strMaxCat ( (char *) uDigCol,
                              (const char *) uTerm,
                              CMGR_COLLECTED_DIGIT_LEN + 1 );
                  if ( !strcmp ( (const char *) pCurDig,
                                 (const char *) uDigCol ) )
                  {
                     /* Match found.  Replace the current string and
                     ** exit processing.
                     */
                     CMGRLOG_INFORMATION ((
                        "cmFeatDialStrManip: 0x%X (0x%X) - set \'%s\' to \'%s\'",
                        pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ),
                        pCurDig, pSpDialCur->pcSpeedDialLocalMapDigitString ));

                     memset ( (void *) pCurDig, 0, CMGR_COLLECTED_DIGIT_LEN + 1 );
                     *pCurCnt =
                        (BOS_UINT32) strPrintf ( (char *) pCurDig,
                                                 CMGR_COLLECTED_DIGIT_LEN,
                                                 "%s",
                                                 (const char *) pSpDialCur->
                                                   pcSpeedDialLocalMapDigitString );
                     bRet = BOS_TRUE;
                  }
               }

               pSpDialCur = pSpDialCur->pNext;
            };
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, LocalSpeedDialingConcept ),
                               &cfgIdx,
                               (void *) pLclSpeedDial,
                               CFG_ITEM_DEL );
         pLclSpeedDial = NULL;
      }
   }

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmFeatCtrl
**
** PURPOSE:    Utility function to change the state of a specific feature and
**             apply corresponding service change.
**
** PARAMETERS: pRes - the resource to apply this on.
**             uFeatIx - the feature reference to apply this on.
**             bEnable - whether to enable or disable the feature
**
** RETURNS:    Nothing, however the status of the identified feature may be
**             changed following this action.
***********************************************************************************/
void cmFeatCtrl ( void *pRes, BOS_UINT32 uFeatIx, BOS_BOOL bEnable )
{
#if CMGRCFG_MULTILINE
   CMCCTKACCNT *pUsr = (CMCCTKACCNT *) pRes;
#else
   CMUSR *pUsr = (CMUSR *) pRes;
#endif
   PROV_CFG_ITEM cfgItem = (PROV_CFG_ITEM) uFeatIx;
   PROV_CFG_IDX cfgIdx;

   if ( (pUsr == NULL) ||
        (cfgItem == CFGITEM( CFG, Unknown )) )
   {
      /* Nothing to do...
      */
      return;
   }

   /* Apply feature specific processing now.
   */
   switch ( cfgItem )
   {
      case CFGITEM( SIP, MWIConcept ):
      {
         CFGPROVDEVICECFG *pDevCfg = NULL;
         BOS_BOOL bNoSubscribe;
         BOS_UINT32 uAccount = pUsr->uNet;

         /* First check if subscription is allowed */
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                    &cfgIdx,
                                    (void *) &pDevCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            bNoSubscribe = pDevCfg->bDisableSubscribe;

            gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                  &cfgIdx,
                                  (void *) pDevCfg,
                                  CFG_ITEM_DEL );
            pDevCfg = NULL;
         }

         if ( bNoSubscribe )
         {
            CMGRLOG_INFORMATION (( "cmFeatCtrl: 0x%X (0x%X) - MWI Sub not allowed!",
                                pUsr->uHdl, uAccount ));
            break;
         }


         /* If we are enabling MWI support, make sure we are also using
         ** the latest configuration for it.
         */
         if ( bEnable )
         {
            CCTKMWICFG mwiCfg;
            CCTKMWITMR mwiTmr;
            CCTKSIPUSRHOST mwiSrv;
            CFGSIPMWI *pUsrMwi = NULL;
            CFGSIPUSERCFG *pUsrCfg = NULL;
            CFGSIPPCSCF *pPcscfUsr = NULL;

            BOS_BOOL bFromDomain = BOS_FALSE;
            BOS_UINT8 uMwiIpAddr[ CMGR_IP_ADDRESS_STRING_LEN + 1 ];

            memset ( (void *) &mwiSrv, 0, sizeof ( CCTKSIPUSRHOST ));
            memset ( (void *) &mwiTmr, 0, sizeof ( CCTKMWITMR ));
            memset ( (void *) &mwiCfg, 0, sizeof ( CCTKMWICFG ));

            /* Default configuration.
            */
            mwiCfg.mode.eEvtPolicy = eCCTKMWIEVT_NOTIFY_UPDATE;
            mwiTmr.uExpTmr = 0;
            mwiTmr.uRetryTmr = CM_CFG__SUB_RETRY_TIMER;

            /*
            **  Subscribe retry timer.
            **  Taken from the PCSCF config concept
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                       &cfgIdx,
                                       (void *) &pUsrCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               PROV_CFG_IDX cfgDomainIdx;
               memset ( &cfgDomainIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgDomainIdx.pcCharIdx = pUsrCfg->pcActiveDomain;
               cfgDomainIdx.uIndex = pUsrCfg->uActiveDomain;

               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                          &cfgDomainIdx,
                                          (void *) &pPcscfUsr,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {

                  mwiTmr.uRetryTmr = pPcscfUsr->uSubscribeRetry/CMGR_MSEC_2_SEC_DIVIDER;

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                                          &cfgDomainIdx,
                                          (void *) pPcscfUsr,
                                          CFG_ITEM_DEL );
                  pPcscfUsr = NULL;
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                     &cfgIdx,
                                     (void *) pUsrCfg,
                                     CFG_ITEM_DEL );
               pUsrCfg = NULL;
            }

            /*
            ** MWI configuration.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
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
                        mwiSrv.sipHost.hostPort.pcAddr =
                                              pUsrMwi->pcMwiSrvDomain;
                     }
                     else
                     {
                        bosIpAddrGetStr ( &pUsrMwi->mwiSrvAddr,
                                          (char *) uMwiIpAddr,
                                          CMGR_IP_ADDRESS_STRING_LEN );

                        mwiSrv.sipHost.hostPort.pcAddr = &uMwiIpAddr[0];
                     }
                     mwiSrv.sipHost.hostPort.uPort =
                                         (BOS_UINT16) pUsrMwi->uSIPPort;
                  }
                  else
                  {
                     bFromDomain = BOS_TRUE;
                  }

                  mwiTmr.uExpTmr = pUsrMwi->uSubDuration / CMGR_MSEC_2_SEC_DIVIDER;
               }
            }
            /*
            ** No specific message waiting server defined, assign the same as the
            ** active domain selected during user creation.
            */
            if ( bFromDomain )
            {
               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                          &cfgIdx,
                                          (void *) &pUsrCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  mwiSrv.sipHost.hostPort.pcAddr = pUsrCfg->pcActiveDomain;
                  mwiSrv.sipHost.hostPort.uPort = 0;

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                        &cfgIdx,
                                        (void *) pUsrCfg,
                                        CFG_ITEM_DEL );
                  pUsrCfg = NULL;
               }
            }
            /* Issue configuration to CCTK.
            */
            cctkSetParm ( uAccount,
                          eCCTKPARM_EC_MWICFG,
                          (const void *) &mwiCfg );
            cctkSetParm ( uAccount,
                          eCCTKPARM_EC_MWITMR,
                          (const void *) &mwiTmr );
            cctkSetParm ( uAccount,
                          eCCTKPARM_MC_MWISVRADDR,
                          (const void *) &mwiSrv );
         }
         /*
         ** Configuration aspects are dealt with, now deal with the feature
         ** status itself.
         **
         ** Because MWI is handled internally within the CCTK layer, we do
         ** not have visibility nor own the GUID handle for the service, hence
         ** we need to send the command via the user registration service that
         ** will take care of the rest of the processing.
         */
         cctkServiceControl ( uAccount,
                              cmMapGetId ( cmMapCfgSvc2CctkSvc,
                                           cfgItem ),
                              bEnable );
      }
      break;

      default:
      break;
   }
}

/***********************************************************************************
** FUNCTION:   cmFeatFlashCwOnly
**
** PURPOSE:    Check whether 'call waiting' (and its associated features such as
**             'cancel call waiting') is the only feature available following a
**             hook flash or not.
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if call waiting is the only feature associated with the
**             hook flash, BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatFlashCwOnly ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_BOOL bRet = BOS_FALSE;

   /* 1) Is 'call waiting' enabled?
   ** 2) Any other hook flash dependent features enabled?
   */
   if ( cmFeatEnabled ( pRes, eCMDM_CWTOGGLE ) &&
        !cmFeatFlashNonCw( pRes ) )
   {
      bRet = BOS_TRUE;
   }

   CMGRLOG_INFORMATION (( "cmFeatFlashCwOnly: 0x%X (0x%X) - %s",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ),
                          bRet ? "cw-only" : "unrestricted" ));
   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmFeatFlashNonCw
**
** PURPOSE:    Check for enabled status of any 'hook flash' dependent feature that
**             is not call waiting (or its derivative associate such as 'cancel call
**             waiting').
**
** PARAMETERS: pRes - the resource to apply this check on.
**
** RETURNS:    BOS_TRUE if some feature other than call waiting can be associated
**             with the hook flash.  BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatFlashNonCw ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   CFGSIPCALLXFER *pXferCfg = NULL;
   CFGSIPTHREEWAYCALL *p3WayCall = NULL;
   BOS_BOOL bRet = BOS_FALSE;

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;

   /* Is hold activate enabled?
   */
   if ( cmFeatEnabled ( pRes, eCMDM_HOLDACTIVATE ) )
   {
      bRet = BOS_TRUE;
   }
   /* Is call transfer enabled?
   */
   if ( !bRet &&
        gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                              &cfgIdx,
                              (void *) &pXferCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pXferCfg && pXferCfg->Act.bCfgStatus )
      {
         bRet = BOS_TRUE;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                            &cfgIdx,
                            (void *) pXferCfg,
                            CFG_ITEM_DEL );
      pXferCfg = NULL;
   }
   /* Is call conference (3-way) enabled?
   */
   if ( !bRet &&
        gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                              &cfgIdx,
                              (void *) &p3WayCall,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( p3WayCall && p3WayCall->Act.bCfgStatus )
      {
         bRet = BOS_TRUE;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept ),
                            &cfgIdx,
                            (void *) p3WayCall,
                            CFG_ITEM_DEL );
      p3WayCall = NULL;
   }

   CMGRLOG_INFORMATION (( "cmFeatFlashNonCw: 0x%X (0x%X) - %s",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ),
                          bRet ? "applicable" : "unavailable" ));
   return bRet;
}

/***********************************************************************************
** FUNCTION:   cmFeatFlashEscAct
**
** PURPOSE:    Escape any action processing while in 'hook flash' mode.  Escaping
**             the process will trigger a feature deny tone to be applied to the
**             local user as well as will resume the current active call.
**
** PARAMETERS: pRes - the resource to apply this action on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatFlashEscAct ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMTMR tmr;

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   CMGRLOG_INFORMATION (( "cmFeatFlashEscAct: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   /* The long interdigit timer may still be running, cancel it now.
   */
   tmr.uUsr = pUsr->uHdl;
   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
   cmTmrStop ( &tmr );

   /* Apply a 'deny service' tone.
   */
   cmFeatDeny ( pRes );

   /* Resume the current active session automatically.
   */
   if ( pSes == NULL )
   {
      pSes = cmUsrActiveSes ( pUsr );
      if ( pSes )
      {
         pSes->ses.med.bFocus = BOS_TRUE;
      }
   }

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFeatFlashEscAct: resuming 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet ));

      cctkCallUnhold ( pSes->uNet );
      cmUsrNewState ( pUsr, eCMST_TALK );
   }
}

/***********************************************************************************
** FUNCTION:   cmFeatAssignPrivacy
**
** PURPOSE:    Gets the privacy information associated with a user.
**
** PARAMETERS: pRes - the resource to apply this check on.
**             pPrivCfg - the privacy configuration updated to the latest
**                        user specific information.
**
** RETURNS:    BOS_TRUE on success, BOS_FALSE otherwise - if so pPrivCfg must
**             be ignored.
***********************************************************************************/
BOS_BOOL cmFeatAssignPrivacy ( void *pRes, CCTKPRIVCFG *pPrivCfg )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_BOOL bRet = BOS_FALSE;
   CFGSIPCID *pCfgCid = NULL;
   PROV_CFG_IDX cfgIdx;

   /* Setup the privacy settings based on the permanent presentation
   ** status.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDConcept ),
                              &cfgIdx,
                              (void *) &pCfgCid,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      pPrivCfg->bCritical = BOS_FALSE;

      if ( pCfgCid )
      {
         bRet = BOS_TRUE;
         pPrivCfg->bCritical = !pCfgCid->bNotCritical;

         switch ( pCfgCid->sPPS )
         {
            case CFG_SIP_CID_PPS_ANONYMOUS:
            {
               pPrivCfg->eLevel = eCCTKPRIVLVL_NETHDR;
            }
            break;

           case CFG_SIP_CID_PPS_ID:
            {
               pPrivCfg->eLevel = eCCTKPRIVLVL_NETID;
            }
            break;

            case CFG_SIP_CID_PPS_LOCAL:
            {
               pPrivCfg->eLevel = eCCTKPRIVLVL_LOCAL;
            }
            break;

            case CFG_SIP_CID_PPS_PUBLIC:
            default:
            {
               pPrivCfg->eLevel = eCCTKPRIVLVL_NONE;
            }
            break;
         }

         pPrivCfg->bRetry = BOS_TRUE;
         pPrivCfg->bRejectAnon = BOS_FALSE;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDConcept ),
                            &cfgIdx,
                            (void *) pCfgCid,
                            CFG_ITEM_DEL );
      pCfgCid = NULL;
   }

   return bRet;
}

/***********************************************************************************
** FUNCTION:   cmFeatCallInt
**
** PURPOSE:    Initiates an internal call to the specified users
**
** PARAMETERS: pRes - the resource to apply this check on.
**             pUsrList - list of users to call
**             bGeneralCall - indicates whether the call is for all users on the
**                            system.
**
** RETURNS:    Nothing,
***********************************************************************************/
void cmFeatCallInt ( void* pRes, CMUSRLIST* pUsrList, BOS_BOOL bGeneralCall )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   BOS_UINT32 uIx;
   CMEVT evt;
   CCTKCALLINFO *pCallInfo2;

   if ( pSes )
   {
      if( ( pUsrList == NULL ) || pUsrList->uLength == 0 )
      {
         /* If there are no users to ring, we should play a busy tone */
         cmFsmCallBusy( pRes );
         return;
      }


      /* Iterate through our user list and call each terminal in the list */
      for( uIx = 0 ; uIx < pUsrList->uLength ; uIx++ )
      {
         CCTKCALLINFO *pCallInfo;
         CMUSR* pOtherUsr = NULL;

         /* We shouldn't be in this list, but to be safe
         ** don't send an incoming call request to ourselves
         */
         if( pUsrList->uUsr[ uIx ] == pUsr->uHdl )
         {
            continue;
         }

         pOtherUsr = cmUsrValid( pUsrList->uUsr[ uIx ] );
         if( !pOtherUsr )
         {
            CMGRLOG_CRITICAL(( "%s: No user found with id [%X]", __FUNCTION__, (unsigned int)pUsrList->uUsr[ uIx ] ));
            continue;
         }
         else if( pOtherUsr->eType == eCMUSRTYPE_PSTN && bGeneralCall )
         {
            /* We don't want to route to the PSTN terminal in a general internal call. */
            CMGRLOG_DEBUG(( "%s: Skip routing to FXO terminal [%X]", __FUNCTION__, (unsigned int)pUsrList->uUsr[ uIx ] ));
            continue;
         }

         pCallInfo = malloc ( sizeof ( CCTKCALLINFO ) );

         memset ( &evt, 0, sizeof( CMEVT ) );
         memset( pCallInfo, 0, sizeof( CCTKCALLINFO ) );

         strPrintf( (char*)pCallInfo->clid.cName,
                   CCTK_CLID_LEN,
                   "%s",
                   (char*)pUsr->uUid );
         strPrintf( (char*)pCallInfo->clid.cNumber,
                   CCTK_CLID_LEN,
                   "%s",
                   (char*)pUsr->uUid );
         pCallInfo->bEmergency = BOS_FALSE;
         pCallInfo->bLoopBack = BOS_FALSE;

         /* Send two "dummy" CCTK events. INCOMING_CALL will
         ** trigger session creation for the terminal and
         ** ALERT_CALL will cause the terminal to start
         ** ringing/play CW.
         */
         evt.uEvt = ( eCMEVT_INCOMING_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
         evt.uDat1 = pOtherUsr->uHdl;     /* User handle */
         evt.uDat2 = pSes->uNet;          /* Call ID */
         evt.pDat3 = pCallInfo;         /* pData */
         evt.uDat4 = CMGR_INVALID_HANDLE; /* CCTK user id */
         evt.bLast = ( ( uIx + 1 ) == pUsrList->uLength ) ? BOS_TRUE : BOS_FALSE;

         if( cmPostEvt ( &evt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }

         pCallInfo2 = malloc ( sizeof ( CCTKCALLINFO ) );

         memset ( &evt, 0, sizeof( CMEVT ) );
         memset( pCallInfo2, 0, sizeof( CCTKCALLINFO ) );

         strPrintf( (char*)pCallInfo2->clid.cName,
                   CCTK_CLID_LEN,
                   "%s",
                   (char*)pUsr->uUid );
         strPrintf( (char*)pCallInfo2->clid.cNumber,
                   CCTK_CLID_LEN,
                   "%s",
                   (char*)pUsr->uUid );
         pCallInfo2->bEmergency = BOS_FALSE;
         pCallInfo2->bLoopBack = BOS_FALSE;

         evt.uEvt = ( eCMEVT_ALERT_CALL | CMGR_EVT_CLASS__CCTK );
         evt.uDat1 = pOtherUsr->uHdl;
         evt.uDat2 = pSes->uNet;
         evt.pDat3 = pCallInfo2;
         evt.uDat4 = CMGR_INVALID_HANDLE;
         evt.bLast = ( ( uIx + 1 ) == pUsrList->uLength ) ? BOS_TRUE : BOS_FALSE;;

         if( cmPostEvt ( &evt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }
      }

      /* Send a eCMEVT_RINGING_CALL event to this user to
      ** start ringback and place the user in the correct
      ** state.
      */
      memset ( &evt, 0, sizeof( CMEVT ) );

      evt.uEvt = ( eCMEVT_RINGING_CALL | CMGR_EVT_CLASS__CCTK );
      evt.uDat1 = pUsr->uHdl;
      evt.uDat2 = pSes->uHdl;
      evt.uDat4 = CMGR_INVALID_HANDLE;
      evt.bLast = BOS_TRUE;

      if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
      {
         cmSignalEvt ();
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmFeatFormatCallerID
**
** PURPOSE:    Format the caller-id date and number according to configuration
**
** PARAMETERS: puUid - the user id to apply this check on.
**             pClidInfo - the caller-id info to be reformated.
**             pTime - the time to be formated into a string.
**             puStr - the string to hold the formatted time info.
**
** RETURNS:    Nothing,
***********************************************************************************/
void cmFeatFormatCallerID ( BOS_UINT8 *puUid, CCTKCLIDINFO *pClidInfo, time_t *pTime,
                            BOS_UINT8 *puStr )
{
   struct tm locTime;
   CCTKCLIDINFO clidInfoFmtd;
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   PROV_CFG_IDX cfgIdx;


   if ( puUid )
   {
      /* Format the date and time.
      */
      if ( puStr && pTime )
      {
         bosTimeGetGmTime ( pTime, &locTime );

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = puUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                    &cfgIdx,
                                    (void *) &pUsrClidDsp,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pUsrClidDsp )
            {
               /* Check whether we need to adjust the local time based on the
               ** time adjustement offset.
               */
               if ( pUsrClidDsp->sTimeAdj )
               {
                  cmDateOffsetMin ( &locTime, pUsrClidDsp->sTimeAdj );
               }

               /* Check whether we need to adjust the local time based on the
               ** DST (daylight saving time) offset.
               */
               if ( pUsrClidDsp->sDSTTimeAdj )
               {
                  cmDateOffsetMin ( &locTime, pUsrClidDsp->sDSTTimeAdj );
               }
            }


            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                  &cfgIdx,
                                  (void *) pUsrClidDsp,
                                  CFG_ITEM_DEL );
            pUsrClidDsp = NULL;
         }

         /* Set caller identification time.
         */
         sprintf ( (char *) puStr,
                   "%02d/%02d/%02d/%02d, ",
                   locTime.tm_mon + 1,
                   locTime.tm_mday,
                   locTime.tm_hour,
                   locTime.tm_min );
      }

      if ( pClidInfo )
      {
         BOS_UINT32 uStrLen = strlen ( (const char *) pClidInfo->cNumber );

         if ( uStrLen )
         {
            /* Format caller identification number
            */
            CFGPROVDEVICECFG *pDevCfg = NULL;
            PROV_CFG_IDX cfgIdx;
            BOS_UINT32 uStrip = 0;

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                       &cfgIdx,
                                       (void *) &pDevCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               /* If a country code was configured.
               */
               if ( pDevCfg &&
                    pDevCfg->pcCountryCode &&
                    strlen ( (const char *) pDevCfg->pcCountryCode ) )
               {
                  /* Check whether we should remove the country code from the
                  ** leading number information.
                  */
                  if ( !strCmpNoCaseSize(
                           (const char *) pDevCfg->pcCountryCode,
                           (const char *) pClidInfo->cNumber,
                           strlen ( (const char *) pDevCfg->pcCountryCode ) ) )
                  {
                     uStrip = strlen ( (const char *) pDevCfg->pcCountryCode );
                  }
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                     &cfgIdx,
                                     (void *) pDevCfg,
                                     CFG_ITEM_DEL );
               pDevCfg = NULL;
            }

            if ( uStrip )
            {
               BOS_UINT8 cTemp[ CMGR_CLID_LEN + 1];
               memset ( cTemp, 0, sizeof ( cTemp ) );

               strncpy ( (char *) cTemp,
                           (char *)&pClidInfo->cNumber[ uStrip ],
                              ( ( uStrLen - uStrip ) > CCTK_CLID_LEN ) ?
                                    CCTK_CLID_LEN :( uStrLen - uStrip ));

               /* Update to the formatted number
               */
               strncpy ( (char *) pClidInfo->cNumber, cTemp,
                           strlen ( (const char *) cTemp ) );
            }
         }

         CMGRLOG_DEBUG (( "cmFeatFormatCallerID: \'%s\' (\'%s\', \'%s\') for %s",
                             puStr, pClidInfo->cName,
                             pClidInfo->cNumber, puUid ));
      }
   }
}


#if CMGRCFG_REMOTE_USER_CONTROL
/***********************************************************************************
** FUNCTION:   cmFeatProxyCaller
**
** PURPOSE:    Proxy an incoming call on the remote user as a new outgoing
**             call to the first associate local user.
**
** PARAMETERS: pRes - the resource user to apply this feature on.
**             pResSes - the resource session associated to apply this feature on.
**
** RETURNS:    Nothing.
***********************************************************************************/
void cmFeatProxyCaller ( void *pResUsr, void *pResSes )
{
   CMUSR *pUsr = (CMUSR *) pResUsr;
   CMSES *pSes = (CMSES *) pResSes;
   CMUSR *pLocalUsr;
   CMSES *pNewSes = NULL;
   BOS_BOOL bFailed = BOS_TRUE;
   BOS_UINT8 tgtUri[ CMGR_URI_STRING_LEN + 1 ];
   CFGSIPUSERCFG *pUsrCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   CCTKPRIVCFG privCfg;

   BOS_BOOL bFeatEnable = BOS_FALSE;
   CFGSIPREMOTEUSERSYSCFG *pCfg = NULL;

   /* Get current configuration.
   */
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                              NULL,
                              (void *) &pCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      bFeatEnable = pCfg->bEnable;
      gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                            NULL,
                            (void *) pCfg,
                            CFG_ITEM_DEL );
      pCfg = NULL;
   }

   if ( bFeatEnable == BOS_FALSE )
   {
      CMGRLOG_WARNING ((
         "cmFeatProxyCaller: remote user feature is currently disabled.  Release call" ));
      if ( pSes )
      {
         /* Release the network call.
         */
         cctkCallDelete ( pSes->uNet,
                          eCCTKCALLDELRSN_BUSY );
         cmUsrNewState ( pUsr, eCMST_RELEASING );
      }
      return;
   }



   if ( pUsr->eType != eCMUSRTYPE_REMOTE )
   {
      CMGRLOG_WARNING ((
         "cmFeatProxyCaller: invalid non-local user 0x%X (0x%X)",
         pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));
      return;
   }
   pLocalUsr = cmUsrValid( pUsr->uLinkedLocalUserHdl );

   memset ( tgtUri,
            0,
            sizeof ( tgtUri ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION ((
         "cmFeatProxyCaller: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if ( pLocalUsr )
      {
         /*
         ** Construct the address for the outgoing proxy'ed call.
         */
         strMaxCat ( (char *) tgtUri,
                     (const char *) CMGR_SIP_URI,
                     CMGR_URI_STRING_LEN );

         /*
         ** Note that the target user portion of the address is received
         ** via the CLID data for this particular call.
         */
         strMaxCat ( (char *) tgtUri,
                     (const char *) pSes->ses.med.callInfo.cTarget,
                     CMGR_URI_STRING_LEN );

         strMaxCat ( (char *) tgtUri,
                     CMGR_AT_URI,
                     CMGR_URI_STRING_LEN );

         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pLocalUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                    &cfgIdx,
                                    (void *) &pUsrCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            strMaxCat ( (char *) tgtUri,
                        (const char *) pUsrCfg->pcActiveDomain,
                        CMGR_URI_STRING_LEN );

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                                  &cfgIdx,
                                  (void *) pUsrCfg,
                                  CFG_ITEM_DEL );
            pUsrCfg = NULL;
         }

         /* Create the call.
         */
         pNewSes = cmSesAlloc ( eCMSESTYPE_MEDIA,
                                pLocalUsr->uHdl );
         if ( pNewSes != NULL )
         {
            cmUsrLinkSes ( pLocalUsr, pNewSes );
            pNewSes->uNetUsr = cmUsrGetNtwkHdl( pLocalUsr );

            /* This session is a 'background' focus session, meaning
            ** it is associated and monitored by the user, the user can
            ** even apply services on that session but it is not the primary
            ** focus session nor any other active session which are managed
            ** directly by this user since it is essentially used for
            ** tracking remote resource usage.
            */
            pNewSes->ses.med.bBgFocus = BOS_TRUE;

            if ( cctkCallCreate ( &pNewSes->uNet,
                                  pNewSes->uNetUsr,
                                  tgtUri ) == eCCTKSTATUS_SUCCESS )
            {
               CMGRLOG_DEBUG (( "cmFeatProxyCaller: target \'%s\' via 0x%X (0x%X)",
                                tgtUri, pNewSes->uHdl, pNewSes->uNet ));

               if ( cmFeatAssignPrivacy ( (void *)pLocalUsr, &privCfg ) )
               {
                  cctkSetParm ( pNewSes->uNet,
                                eCCTKPARM_EC_CALLPRIV,
                                (void *) &privCfg );
               }

               /* Proxy the incoming call from the remote user using this
               ** newly created call via the associated local user.
               */
               cctkCallProxy ( pSes->uNet, pNewSes->uNet );

               cmUsrNewState ( pUsr, eCMST_RINGBACK );
               bFailed = BOS_FALSE;
            }
         }
      }

      /* Failed the proxy action...  Terminate the incoming call from
      ** the remote user and clean all things up.
      */
      if ( bFailed )
      {
         CMGRLOG_DEBUG (( "cmFeatProxyCaller: failed 0x%X (0x%X)",
                          pSes->uHdl, pSes->uNet ));

         if ( pLocalUsr && pNewSes )
         {
            cmUsrUnLinkSes ( pLocalUsr, pNewSes );
            cmSesFree ( pNewSes, pLocalUsr->uHdl );
         }

         if( cmNumSesWithCid( pSes->uNet ) == 1 )
         {
            /* Release the network call.
            */
            cctkCallDelete ( pSes->uNet,
                             eCCTKCALLDELRSN_BUSY );
         }

         cmUsrUnLinkSes ( pUsr, pSes );
         cmSesFree ( pSes, pUsr->uHdl );

         cmUsrNewState ( pUsr, eCMST_RELEASING );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFeatProxyCallee
**
** PURPOSE:    Proxy an incoming call on the local user as a new outgoing
**             call to any of the associated remote user(s).
**
** PARAMETERS: pRes - the resource session to apply this feature on.
**
** RETURNS:    BOS_TRUE if we successfully proxy this call to at least one remote
**             user, BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmFeatProxyCallee ( void *pRes )
{
   CMUSR *pUsr = NULL;
   CMSES *pSes = (CMSES *) pRes;
   CMSES *pNewSes = NULL;
   CMUSR *pLocalUsr = cmUsrValid( pSes->uUsr );
   BOS_UINT32 uIx;
   BOS_BOOL bFailed = BOS_TRUE;
   BOS_UINT8 tgtUri[ CMGR_URI_STRING_LEN + 1 ];
   CFGSIPREMOTEUSERCFG *pRemUsrCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT8 uIpAddr[ CMGR_IP_ADDRESS_STRING_LEN + 1 ];
   BOS_BOOL bRet = BOS_FALSE;
   BOS_BOOL bFeatEnable = BOS_FALSE;
   CFGSIPREMOTEUSERSYSCFG *pCfg = NULL;

   /* Get current configuration.
   */
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                              NULL,
                              (void *) &pCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      bFeatEnable = pCfg->bEnable;
      gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                            NULL,
                            (void *) pCfg,
                            CFG_ITEM_DEL );
      pCfg = NULL;
   }

   if ( bFeatEnable && pSes && pLocalUsr && pLocalUsr->eType != eCMUSRTYPE_REMOTE &&
        pLocalUsr->uLinkedRemoteUserCnt )
   {
      CMGRLOG_INFORMATION ((
         "cmFeatProxyCallee: 0x%X (0x%X) from 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pLocalUsr->uHdl, pLocalUsr->uNet ));

      /* Loop through all the associated remote user(s) and apply
      ** the same behavior - trigger a call to it.
      */
      for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX; uIx++ )
      {
         pUsr = NULL;
         memset ( tgtUri, 0, sizeof ( tgtUri ) );
         if ( pLocalUsr->uLinkedRemoteUserHdl[uIx] != CMGR_INVALID_HANDLE )
         {
            pUsr = cmUsrValid ( pLocalUsr->uLinkedRemoteUserHdl[uIx] );
         }

         /* If the remote user is the caller, and it is calling the local user, do not
         ** notify this remote user.
         */
         if ( pUsr && strncmp ( (const char*)pSes->ses.med.callInfo.cFromUserName,
                                (const char*)pUsr->uUid, sizeof (pUsr->uUid)) )
         {
            /* Construct the address for the outgoing proxy'ed call.
            */
            strMaxCat ( (char *) tgtUri,
                        (const char *) CMGR_SIP_URI,
                        CMGR_URI_STRING_LEN );

            /* The target user for this call is the actual remote user
            ** itself therefore the URI created is based off the user
            ** identity (user part) and the user host (host part).  Both
            ** of those information were learned during initial registration
            ** of the remote user.
            */
            strMaxCat ( (char *) tgtUri,
                        (const char *) pUsr->uUid,
                        CMGR_URI_STRING_LEN );

            strMaxCat ( (char *) tgtUri,
                        CMGR_AT_URI,
                        CMGR_URI_STRING_LEN );

            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                                       &cfgIdx,
                                       (void *) &pRemUsrCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               strMaxCat ( (char *) tgtUri,
                           (const char *) pRemUsrCfg->pcAddr,
                           CMGR_URI_STRING_LEN );

               if ( pRemUsrCfg->uSipPort )
               {
                  sprintf ( (char *) uIpAddr,
                            ":%d",
                            pRemUsrCfg->uSipPort );
                  strMaxCat ( (char *) tgtUri,
                              (const char *) uIpAddr,
                              CMGR_URI_STRING_LEN );
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                                     &cfgIdx,
                                     (void *) pRemUsrCfg,
                                     CFG_ITEM_DEL );
               pRemUsrCfg = NULL;
            }

            /* Create the call.
            */
            pNewSes = cmSesAlloc ( eCMSESTYPE_MEDIA,
                                   pUsr->uHdl );
            if ( pNewSes != NULL )
            {
               cmUsrLinkSes ( pUsr, pNewSes );
               pNewSes->uNetUsr = cmUsrGetNtwkHdl( pUsr );

               /* This session is a 'background' focus session, meaning
               ** it is associated and monitored by the user, the user can
               ** even apply services on that session but it is not the primary
               ** focus session nor any other active session which are managed
               ** directly by this user since it is essentially used for
               ** tracking remote resource usage.
               */
               pNewSes->ses.med.bBgFocus = BOS_TRUE;

               if ( cctkCallCreate ( &pNewSes->uNet,
                                     pNewSes->uNetUsr,
                                     tgtUri ) == eCCTKSTATUS_SUCCESS )
               {
                  CMGRLOG_DEBUG ((
                     "cmFeatProxyCallee: target \'%s\' via 0x%X (0x%X)",
                     tgtUri, pNewSes->uHdl, pNewSes->uNet ));

                  /* Proxy the incoming call from the local user using this
                  ** newly created call to the associated remote user.
                  */
                  cctkCallProxy ( pSes->uNet, pNewSes->uNet );

                  pNewSes->ses.med.pLinkedSes = (void *) pSes;

                  cmUsrNewState ( pUsr, eCMST_RINGBACK );
                  bFailed = BOS_FALSE;

                  /* At least one remote user target is attempted, marked it as such
                  ** so that further processing can take place accordingly.
                  */
                  bRet = BOS_TRUE;
               }
            }

            /* Failed the proxy action...  Clean up.
            */
            if ( bFailed )
            {
               CMGRLOG_DEBUG (( "cmFeatProxyCallee: failed 0x%X (0x%X)",
                                pSes->uHdl, pSes->uNet ));

               if ( pNewSes )
               {
                  cmUsrUnLinkSes ( pUsr, pNewSes );
                  cmSesFree ( pNewSes, pUsr->uHdl );
               }
            }
         }
      }
   }

   return bRet;
}


/***********************************************************************************
** FUNCTION:   cmFeatProxyCalleeEnd
**
** PURPOSE:    End a call that was being proxy'ed on remote users.
**
** PARAMETERS: pRes - the resource user associated with this call.
**
** RETURNS:    If pRes is NULL or is the 'first-user', cancel the associated call
**             on all remote user(s).  If pRes is a remote user identification,
**             cancel the call on all but the identified remote user(s).
***********************************************************************************/
void cmFeatProxyCalleeEnd ( void *pRes )
{
   BOS_UINT32 uIx;
   CMSES *pSes = NULL;
   CMUSR *pLocalUsr = NULL;
   CMUSR *pRemoteUsr = NULL;
   BOS_UINT32 uCurrentRemoteUsrHdl = CMGR_INVALID_HANDLE;
   CMUSR *pUsr = (CMUSR *) pRes;

   if ( pUsr->eType == eCMUSRTYPE_REMOTE )
   {
      /* The remote user is stopping other remote users from ringing */
      uCurrentRemoteUsrHdl = pUsr->uHdl;
      pLocalUsr = cmUsrValid ( pUsr->uLinkedLocalUserHdl );
   }
   else
   {
      /* The local user is stopping remote users from ringing */
      pLocalUsr = pUsr;
   }

   if ( pLocalUsr && pLocalUsr->eType != eCMUSRTYPE_REMOTE && pLocalUsr->uLinkedRemoteUserCnt )
   {
      for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
      {
         pRemoteUsr = NULL;

         /* End call for remote user that is not my current remote user */
         if ( pLocalUsr->uLinkedRemoteUserHdl[uIx] != CMGR_INVALID_HANDLE &&
            pLocalUsr->uLinkedRemoteUserHdl[uIx] != uCurrentRemoteUsrHdl )
         {
            pRemoteUsr = cmUsrValid ( pLocalUsr->uLinkedRemoteUserHdl[uIx] );
         }
         if ( pRemoteUsr )
         {
            /* End the call on this user.
            */
            if ( (pSes = cmUsrBgSes ( pRemoteUsr )) != NULL )
            {
               CMGRLOG_DEBUG ((
                  "cmFeatProxyCalleeEnd: ending 0x%X (0x%X) on 0x%X (0x%X)",
                  pSes->uHdl, pSes->uNet, pRemoteUsr->uHdl, pSes->uNetUsr ));

               if( cmNumSesWithCid( pSes->uNet ) == 1 )
               {
                  cctkCallDelete ( pSes->uNet,
                                eCCTKCALLDELRSN_NORMAL );
               }

               cmUsrNewState ( pRemoteUsr, eCMST_RELEASING );
            }
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFeatProxyMove2Bg
**
** PURPOSE:    Move the current session in progress to be a background one on
**             the identified user resource.
**
** PARAMETERS: pResUsr - the resource user associated with this call.
**             pResSes - the resource session associated with this user.
**
** RETURNS:    .
***********************************************************************************/
void cmFeatProxyMove2Bg ( void *pResUsr, void *pResSes )
{
   CMUSR *pUsr = (CMUSR *) pResUsr;
   CMSES *pSes = (CMSES *) pResSes;

   /* Make sure we apply this action on the local user always.
   */
   if ( pUsr && pSes && (pUsr->eType != eCMUSRTYPE_REMOTE ) )
   {
      CMGRLOG_DEBUG ((
         "cmFeatProxyMove2Bg: moving 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if ( pSes->uNet != CMGR_INVALID_HANDLE )
      {
         /* Push the session in the background for the local
         ** user.
         */
         pSes->ses.med.bBgFocus = BOS_TRUE;
         pSes->ses.med.bFocus = BOS_FALSE;

         /* Terminate local user activity on the local user, which
         ** essentially means terminating user feedback.
         */
         switch ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] )
         {
            /* Will end the ringing.
            */
            case eCMST_RINGING:
            {
               CMGREPSIG sig;
#if CMGRCFG_MULTILINE
               CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE               */
               CMTMR tmr;

               memset ( &sig, 0, sizeof ( CMGREPSIG ) );
               memset ( &tmr, 0, sizeof ( CMTMR ) );

               tmr.uSes = pSes->uHdl;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_RINGING;

               cmTmrStop ( &tmr );

               sig.eAct = eCMGREPSIGACT_STOP;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
               sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE               */
               sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGING */
               pUsr->uCpt = eEPTSIG_NULL;

               cmPhySignal ( pUsr->uPhys, &sig );

               if ( pSes && ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE ) )
               {
                  /* We would be playing ringback to the peer. Terminate it.
                  */
                  sig.eAct = eCMGREPSIGACT_STOP;
                  sig.uSes = pSes->ses.med.uPhy;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
                  sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE                  */
                  sig.uSig = eEPTSIG_RINGBACK;

                  cmPhySignal ( pUsr->uPhys, &sig );
               }

               cmUsrNewState ( pUsr, eCMST_IDLE );

               /* Publish a 'line inactive' event.
               */
               if ( gCmCfgCb.cmgrEvtCb &&
                    (pUsr->eType != eCMUSRTYPE_REMOTE) )
               {
                  gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_INACTIVE,
                                       pUsr->uPhys,
#if CMGRCFG_MULTILINE
                                       pUsr->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                       NULL );
               }

               /* If we are pending on an action, don't release
               ** the resource.
               */

               /* Keep the Phy associated until the Proxy Call Ended */
               /*
               if ( pUsr->bPendingActPhy == BOS_FALSE )
               {
                  pUsr->uPhys = CMGR_INVALID_HANDLE;
               }
               */
            }
            break;

            /* Will end the call waiting.
            */
            case eCMST_TALK:
            {
               CMTMR tmr;

               memset ( &tmr, 0, sizeof ( CMTMR ) );

               tmr.uSes = pSes->uHdl;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_CALLWAIT;

               cmTmrStop ( &tmr );
            }
            break;

            default:
            break;
         }
      }
   }
}
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
