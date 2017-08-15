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

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CMGR_T38_MAX_DATAGRAM_DEFAULT           160
#define CMGR_MAXPTIME_DEFAULT                   20
#define CMGR_AOR_DELIMITER                      '@'
#define CMGR_SIP_URI                            "sip:"
#define CMGR_TEL_URI                            "tel:"

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */
// #if CMGRCFG_MULTILINE
//static void cmProfRfc3261MultilineUserAdd ( void *pRes );
// #else
static void cmProfRfc3261DefaultUserAdd ( void *pRes );
// #endif /* CMGRCFG_MULTILINE */

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
// #if CMGRCFG_MULTILINE
   // cmProfRfc3261MultilineUserAdd( pRes );
// #else
   cmProfRfc3261DefaultUserAdd( pRes );
// #endif /* CMGRCFG_MULTILINE */
}


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
   CFGSIPUSERCFG *pUsr = NULL;
   PROV_CFG_IDX cfgIdx;
   CMDMSTATUS dmStatus = eCMDM_ERROR;
   CFGSIPDIGITMAP *pUsrDigMap = NULL;
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
         /* At this time we are only creating 'line' users, which are users
         ** associated with an FXS interface on our VoIP gateway.
         */
         pCmUsr->eType = eCMUSRTYPE_LINE;

         /* Set this user to in-service by default
         ** FIXME: We should only allow one cmusr/terminal for 2G/3G
         */
         pCmUsr->bInSrv = BOS_TRUE;

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
                                
         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
         cfgIdx.pcCharIdx = pCmUsr->uUid;
         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                               &cfgIdx,
                               (void *) pUsr,
                               CFG_ITEM_DEL );

         pUsr = NULL;
      }
   }
}

