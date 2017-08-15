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
**      This module implements the Call Manager 'show' interface which is used to
**      provide information on the run time status of the Call Manager.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

#define CM_SHOW_STATUS_BOOL( a )             (a) ? 'Y' : 'N'


/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmShow
**
** PURPOSE:    Main 'show' function used to display (via logging system) information
**             about the current Call Manager managed data.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void cmShow( void )
{
#if CMGRCFG_STATUS_SHOW
   BOS_UINT32 uIx;

   CMGRLOG_DEBUG (( "\nCall Manager - User Information.\n" ));
   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
   {
      if ( cmUsrValid ( uIx ) != NULL )
      {
         cmShowUsr ( (void *) cmUsrValid ( uIx ) );
      }
   }

   CMGRLOG_DEBUG (( "\nCall Manager - Media Session Information.\n" ));
   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX * CMGRCFG_MEDSES_MAX ; uIx++ )
   {
      if ( cmSesMedValid ( uIx | CMSES_MEDIA_OFFSET ) != NULL )
      {
         cmShowSes ( (void *) cmSesMedValid ( uIx | CMSES_MEDIA_OFFSET ) );
      }
   }

   CMGRLOG_DEBUG (( "\nCall Manager - Service Session Information.\n" ));
   for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX * CMGRCFG_SRVSES_MAX ; uIx++ )
   {
      if ( cmSesSrvValid ( uIx | CMSES_SERVICE_OFFSET ) != NULL )
      {
         cmShowSes ( (void *) cmSesSrvValid ( uIx | CMSES_SERVICE_OFFSET ) );
      }
   }
#endif /* CMGRCFG_STATUS_SHOW */
}


/***********************************************************************************
** FUNCTION:   cmShowUsr
**
** PURPOSE:    Show information about a given user.
**
** PARAMETERS: pRes - the resource user pointed.
**
** RETURNS:    None.
***********************************************************************************/
void cmShowUsr( void *pRes )
{
#if CMGRCFG_STATUS_SHOW
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_UINT32 uIx, uJx;

   /* Generic information.
   */
   CMGRLOG_DEBUG (( "User [0x%X, %s] - Hdl [0x%X], Net [0x%X], Type [%d]",
                    pUsr->uUindex,
                    strlen( pUsr->uUid ) ? pUsr->uUid : "NULL",
                    pUsr->uHdl,
                    cmUsrGetCurrNtwkHdl( pUsr ),
                    pUsr->eType ));

   /* Boolean statuses.
   */
   CMGRLOG_DEBUG (( "Srv [%c], OffHook [%c],PcCw [%c], Lnk [%c], Delay [%c]",
                    CM_SHOW_STATUS_BOOL( pUsr->bInSrv ),
                    CM_SHOW_STATUS_BOOL( pUsr->bOffHook ),
                    CM_SHOW_STATUS_BOOL( pUsr->bPerCallCW ),
                    CM_SHOW_STATUS_BOOL( pUsr->bDtLnk ),
                    CM_SHOW_STATUS_BOOL( pUsr->bRegNoDelay )));

   /* Other information (non status).
   */
   CMGRLOG_DEBUG (( "CfgAct [%d], Phy [0x%X], Cpt [0x%X],  Dig [%d], DigCol [%s] (%d)",
                    pUsr->eCfgAction,
                    pUsr->uPhys,
                    pUsr->uCpt,
                    pUsr->eDigDetState,
                    pUsr->uDigCol,
                    pUsr->uDigCnt ));

   /* FSM information.
   */
   for ( uIx = 0 ; uIx < CMGRCFG_STATE_MAX ; uIx++ )
   {
      CMGRLOG_DEBUG (( "State (%d): [%s] (0x%X)",
                       uIx,
                       (char *) cmMapGetStr ( cmMapCmState, pUsr->eSta[ uIx ] ),
                       pUsr->eSta[ uIx ] ));

      for ( uJx = 0 ; uJx < CMGRCFG_EVENT_MAX ; uJx++ )
      {
         CMGRLOG_DEBUG (( "      Evt (%d): [%s] (0x%X)",
                          uJx,
                          (char *) cmMapGetStr ( cmMapCmEvent,
                                                 pUsr->eEvt[ uIx ][ uJx ] ),
                          pUsr->eEvt[ uIx ][ uJx ] ));
      }
   }

   /* Associated session(s) information.
   */
   CMGRLOG_DEBUG (( "CwSes [0x%X]", pUsr->pSesWait ));
   CMGRLOG_DEBUG (( "HeldSes [0x%X]", pUsr->pSesHeld ));
 
   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      CMGRLOG_DEBUG (( "MedSes (%d) [0x%X]", uIx, pUsr->pMedSes[ uIx ] ));
   }

   for ( uIx = 0 ; uIx < CMGRCFG_SRVSES_MAX + 1 ; uIx++ )
   {
      CMGRLOG_DEBUG (( "SrvSes (%d) [0x%X]", uIx, pUsr->pSrvSes[ uIx ] ));
   }

   /* End of it all...
   */
   CMGRLOG_DEBUG (( "User [0x%X] -- END --",
                    pUsr->uHdl ));
#else
   CMGR_REMOVE_UNUSED_PARAM_WARNING( pRes );
#endif /* CMGRCFG_STATUS_SHOW */
}


/***********************************************************************************
** FUNCTION:   cmShowSes
**
** PURPOSE:    Show information about a given session.
**
** PARAMETERS: pRes - the resource session pointed.
**
** RETURNS:    None.
***********************************************************************************/
void cmShowSes( void *pRes )
{
#if CMGRCFG_STATUS_SHOW
   CMSES *pSes = (CMSES *) pRes;

   /* Generic information.
   */
   CMGRLOG_DEBUG (( "Ses - Hdl [0x%X], Net [0x%X], Usr [0x%X], App [0x%X]",
                    pSes->uHdl,
                    pSes->uNet,
                    pSes->uUsr,
                    pSes->uApp ));

   /* Common information.
   */
   CMGRLOG_DEBUG (( "Type [%d], Estab [%c]",
                    pSes->eType,
                    CM_SHOW_STATUS_BOOL( pSes->bEstab ) ));


   /* Session type specific information.
   */
   switch ( pSes->eType )
   {
      case eCMSESTYPE_MEDIA:
      {
         /* Generic information.
         */
         CMGRLOG_DEBUG (( "PhyHdl [0x%X], CnfHdl [0x%X], DigCol [%s] (%d)",
                          pSes->ses.med.uPhy,
                          pSes->ses.med.uConf,
                          pSes->ses.med.uDigCol,
                          pSes->ses.med.uDigCnt ));

         /* Session information as received from the network signaling side
         ** of things.
         */
         CMGRLOG_DEBUG (( "CidName [%s], CidNum [%s]",
                          pSes->ses.med.callInfo.clid.cName,
                          pSes->ses.med.callInfo.clid.cNumber ));
         CMGRLOG_DEBUG (( "Lpbck [%c], Emg [%c], KAM [%c]",
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.callInfo.bLoopBack ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.callInfo.bEmergency ),
                          CM_SHOW_STATUS_BOOL(
                                    pSes->ses.med.callInfo.bMedKeepAlive ) ));
         CMGRLOG_DEBUG (( "AlrtNfo [%s], ErrNfo [%s], Join [0x%X]",
                          pSes->ses.med.callInfo.cAlertInfo[ 0 ],
                          pSes->ses.med.callInfo.cErrorInfo,
                          pSes->ses.med.callInfo.uJoin ));

         /* Associated user action.
         */
         CMGRLOG_DEBUG (( "Action [%s] (0x%X)",
                          (char *) cmMapGetStr ( cmMapDmAction,
                                                 pSes->ses.med.eAction ),
                          pSes->ses.med.eAction ));

         /* Boolean statuses.
         */
         CMGRLOG_DEBUG (( "Foc [%c], Emg [%c], Med [%c], Cfv [%c]",
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bFocus ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bEmergency ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bMediaRx ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bCfv ) ));
         CMGRLOG_DEBUG (( "Held [%c], RemInit [%c], KAM [%c], BgFoc [%c], FT [%c]",
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bHeld ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bRemInit ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bMediaKAM ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bBgFocus ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.bFastTrac ) ));

         /* Fax session specific information.
         */
         CMGRLOG_DEBUG (( "Fax: Fail [%c], Att [%c], Ong [%c], End [%c]",
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.faxRelay.bFailed ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.faxRelay.bAttempt ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.faxRelay.bOngoing ),
                          CM_SHOW_STATUS_BOOL( pSes->ses.med.faxRelay.bDeferEnd )
                       ));
      }
      break;

      case eCMSESTYPE_SRV:
      {
         CMGRLOG_DEBUG (( "SrvHdl [0x%X]",
                          pSes->ses.srv.uSrv ));
      }
      break;

      case eCMSESTYPE_NONE:
      default:
      {
         CMGRLOG_DEBUG (( "No further information" ));
      }
      break;
   }

   CMGRLOG_DEBUG (( "Ses [0x%X] -- END --",
                    pSes->uHdl ));
#else
   CMGR_REMOVE_UNUSED_PARAM_WARNING( pRes );
#endif /* CMGRCFG_STATUS_SHOW */
}
