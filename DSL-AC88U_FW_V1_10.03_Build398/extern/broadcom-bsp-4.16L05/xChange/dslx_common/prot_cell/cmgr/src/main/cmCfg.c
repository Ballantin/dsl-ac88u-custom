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
   PROV_CFG_IDX cfgIdx;

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

   if ( pDevCfg->bDevEnabled )
   {
      gbCmCfgCctkConfig = BOS_TRUE;
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
   /* RFC 3261 application.  Also the 'default' usage.
   */
   cmProfRfc3261UserAdd ( pRes );

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
   while ( pCctkIndAccntCfg != NULL )
   {
      if ( ( pCmUsr = cmUsrAlloc() ) != NULL )
      {
         /* Associate this user with a valid identifier, this is the way to tell
         ** that this user is in fact used.
         */
         strncpy ( (char *)pCmUsr->uUid, (char *)pCctkIndAccntCfg->pcUserId, CMGR_USER_IDENTIFIER_LEN );
         pCmUsr->uUindex = pCctkIndAccntCfg->uIMPUIndex;
         
         /* Application controlled network user.
         */
         pCmUsr->uNet = CMGR_APPNET_HANDLE;

         /* Add the user according to the configuration profile selected.
         */
         cmCfgUserAdd ( (void *) pCmUsr );

         CMGRLOG_INFORMATION (( "cmCfgUser: cmUsrUuid %s - cmUsrUhdl 0x%X, uNet 0x%X",
                                pCmUsr->uUid, pCmUsr->uHdl,
                                pCmUsr->uNet ));
                                
         /* Can only have one cellular user!
         */
         break;
      }
      else
      {
         CMGRLOG_CRITICAL (( "cmCfgUser: Failed to allocate cmUsr block for pUsr->pcUserId %s!", pCctkIndAccntCfg->pcUserId ));
      }

      /* Increment the user.
      */
      pCctkIndAccntCfg = pCctkIndAccntCfg->pNext;
   }

   /* Free the resources allocated.
   */
   gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, UserConcept ),
                         &cfgIdx,
                         (void *) pCctkAccntCfg,
                         CFG_ITEM_DEL );
   pCctkAccntCfg = NULL;

   return BOS_TRUE;
}

