/**********************************************************************************
** Copyright © 2009 - 2010 Broadcom Corporation
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
**      This file contains the Telephony Client Application Manager callbacks.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <tcamInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/***********************************************************************************
** FUNCTION:   tcamCbCmgrEvtCb
**
** PURPOSE:    Callback functionality for event publication outside of
**             the Call Manager.
**
** PARAMETERS: eEvt  - the event of interest published.
**             uUsr  - the user for which this event is being published.
**             pData - additional data (may be NULL) associated to the
**                     published event.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
void tcamCbCmgrEvtCb ( CMGREVT eEvt, BOS_UINT32 uUsr, void *pData )
{
   /* Map some of the events sent by the CMGR to the corresponding
   ** external world events.
   */
   TCAMEVT eTcamEvt = (TCAMEVT) tcamMapGetId ( tcamMapCmgrEvt2OutEvt,
                                               (BOS_UINT32) eEvt );

   TCAMLOG_DEBUG (( "tcamCbCmgrEvtCb: (0x%lX -> 0x%lX) for 0x%lX",
                    eEvt, eTcamEvt, uUsr ));

   if ( eTcamEvt != eTCAM_INVALID )
   {
      switch ( eTcamEvt )
      {
         case eTCAM_SVCRDY:
         case eTCAM_SVCNRDY:
         {
            /* Pass directly to the com-cfg interface which will publish
            ** the event to the outside world.  Since for the time being we
            ** always assume support for a single user at the time, we do not
            ** need to worry about the actual CMGR user that reported this
            ** event.
            */
            tcamComCfg ( eTcamEvt );
         }
         break;

         case eTCAM_ACTIVE:
         case eTCAM_DISCONNECT:
         {
            BOS_UINT32 uAppIx = uUsr;

            /* Pass the event to the outside world.
            */
            tcamComWorld ( eTcamEvt, uAppIx, NULL );

            if ( eTcamEvt == eTCAM_DISCONNECT )
            {
               TCAMCMGREVT cmgrEvt;
               /* Clean up the CMGR resources as well.
               */
               TCAMLOG_INFORMATION (( "tcamCbCmgrEvtCb: posting 0x%lX",
                                      uAppIx ));

               memset ( (void *) &cmgrEvt, 0, sizeof ( TCAMCMGREVT ) );
               cmgrEvt.uEvt = eCMCMD_ENDCALL;
               cmgrEvt.uDat1 = uAppIx;

               tcamAsyncCmgrCbPost ( &cmgrEvt );

               if ( uAppIx != TCAM_INVALID_HANDLE )
               {
                  tcamResFreeAppIx ( uAppIx );
                  tcamComWorld ( eTCAM_DISC, uAppIx, NULL );
               }
            }
         }
         break;

         case eTCAM_CALL_XFER:
         {
            BOS_UINT32 uAppIx = uUsr;

            /* Existing call has been transferred. Hence, Remote User
            ** information has changed.
            ** Forward Event to the Outside World.
            */
            /* Enable Remote Handle available
            TCAMLOG_INFORMATION (( "tcamCbCmgrEvtCb: Remote Handle %d",
                                   *((BOS_UINT32 *) pData ) ));
            */
            tcamComWorld ( eTcamEvt, uAppIx, pData );
         }
         break;

         case eTCAM_CONFEST:
         {
            if ( pData != NULL )
            {
               BOS_UINT32 uAppIx = uUsr;
               BOS_UINT32 uAppConfIx = *((BOS_UINT32 *) pData);

               TCAMLOG_INFORMATION ((
                  "tcamCbCmgrEvtCb: conference established 0x%X and 0x%X",
                  uAppIx, uAppConfIx ));

               tcamComWorld ( eTcamEvt,
                              uAppIx,
                              (void *) &uAppConfIx );
            }
         }
         break;

#if CMGRCFG_PASS_THRU_MSG
         case eTCAM_MESSAGE:
         {
            if ( pData != NULL )
            {
               CCTKMSGPASSTHRU *pMsg = (CCTKMSGPASSTHRU *) pData;
               TCAMCMGREVT cmgrEvt;

               TCAMLOG_INFORMATION (( "tcamCbCmgrEvtCb: msg-recv on 0x%X (%p, %u)",
                                      uUsr, pMsg->puData, pMsg->uSize ));

               /* Send a response.
               */
               memset ( (void *) &cmgrEvt, 0, sizeof ( TCAMCMGREVT ) );
               cmgrEvt.uEvt = eCMCMD_MSG_RESP;
               cmgrEvt.uDat1 = 200;     /* Send a 200-OK. */
               cmgrEvt.uDat2 = uUsr;    /* Handle to use for the response. */

               tcamAsyncCmgrCbPost ( &cmgrEvt );
            }
         }
         break;

         case eTCAM_MSG_RSP:
         {
            if ( pData != NULL )
            {
               CMGRAPPCALLHDL *pRsp = (CMGRAPPCALLHDL *) pData;

               TCAMLOG_INFORMATION ((
                  "tcamCbCmgrEvtCb: resp %u to msg recv on 0x%X (user 0x%X)",
                  pRsp->uAppIx, pRsp->uSesHdl, uUsr ));
            }
         }
         break;

         case eTCAM_MSG_HDL:
         {
            if ( pData != NULL )
            {
               CMGRAPPCALLHDL *pHdl = (CMGRAPPCALLHDL *) pData;

               TCAMLOG_INFORMATION ((
                  "tcamCbCmgrEvtCb: assigned 0x%X for msg 0x%X (user 0x%X)",
                  pHdl->uSesHdl, pHdl->uAppIx, uUsr ));
            }
         }
         break;
#endif /* CMGRCFG_PASS_THRU_MSG */

         case eTCAM_SVCOK:
         case eTCAM_SVCFAIL:
         {
            if ( pData != NULL )
            {
               /* Remap the generic service event to a particular service
               ** identification that is then issued to the outside world.
               */
               TCAMEVT eTcamSvc = (TCAMEVT) tcamMapGetId ( tcamMapCmgrSvc2OutEvt,
                                                           *((BOS_UINT32 *) pData) );
               BOS_UINT32 uAppIx = uUsr;

               /* In case this is a service failure indication, make sure we 
               ** pass along the proper information.
               */
               if ( eTcamEvt == eTCAM_SVCFAIL )
               {
                  switch ( eTcamSvc )
                  {
                     case eTCAM_HOLD:
                     {
                        /* Hold has failed, inform of 'unhold'.
                        */
                        eTcamSvc = eTCAM_UNHOLD;
                     }
                     break;

                     case eTCAM_UNHOLD:
                     {
                        /* Unhold has failed, inform of 'hold'.
                        */
                        eTcamSvc = eTCAM_HOLD;
                     }
                     break;

                     case eTCAM_CONF:
                     {
                        /* Conference has failed, inform of 'end of conference'.
                        */
                        eTcamSvc = eTCAM_CONFEND;
                     }
                     break;

                     case eTCAM_BXFER:
                     case eTCAM_CXFER:
                     {
                        /* Transfer has failed, inform of 'end of transfer'.
                        */
                        eTcamSvc = eTCAM_XFEREND;
                     }
                     break;

                     default:
                     break;
                  }
               }

               /* Pass the event to the outside world.
               */
               tcamComWorld ( eTcamSvc, uAppIx, NULL );
            }
         }
         break;

         case eTCAM_MWION:
         case eTCAM_MWIOFF:
         {
            /* Pass the event to the outside world.
            */
            tcamComWorld ( eTcamEvt, TCAM_INVALID_HANDLE, NULL );
         }
         break;

         default:
            /* Ignore those messages.
            */
         break;
      }
   }
}


/***********************************************************************************
** FUNCTION:   tcamCbCmgrEptCtlCb
**
** PURPOSE:    Callback functionality for endpoint action control outside of
**             the Call Manager.
**
** PARAMETERS: pCmd - the command to be passed to the endpoint.  This is a
**                    generic command interface which can be used for many
**                    purposes.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
void tcamCbCmgrEptCtlCb ( CMGREPCMD *pCmd )
{
   switch ( pCmd->eAction )
   {
      /* Those actions are being controlled outside of the TCAM
      ** scope.
      */
      case eCMGRPHYCMD_INIT:
      case eCMGRPHYCMD_DEINIT:
      case eCMGRPHYCMD_CREATE:
      case eCMGRPHYCMD_DESTROY:
      break;

      case eCMGRPHYCMD_SIGNAL:
      {
         /* Signal events from the CMGR destined to the endpoint layer are
         ** map to event action that are dealt with at the call application
         ** layer.
         */
         switch ( pCmd->cmd.sig.eAct )
         {
            case eCMGREPSIGACT_START:
            case eCMGREPSIGACT_REPEAT:
            {
               TCAMEVT eTcamEvt = (TCAMEVT) tcamMapGetId ( tcamMapCmgrEptSig2OutEvt,
                                                           pCmd->cmd.sig.uSig );

               /* Special case if this is a ringgback signal on a connection, do not
               ** actually pass it along as an alerting signal since it is in fact an
               ** action to force the local end to play ringback to the remote but we
               ** do not handle it there yet.
               */
               if ( ( pCmd->cmd.sig.uSig == eEPTSIG_RINGBACK ) &&
                    ( pCmd->cmd.sig.uSes != CMGR_INVALID_HANDLE ) )
               {
                  eTcamEvt = eTCAM_INVALID;
               }

               if ( ( eTcamEvt != eTCAM_INVALID ) &&
                    ( pCmd->cmd.sig.uApp != CMGR_INVALID_HANDLE ) )
               {
                  if( eTcamEvt == eTCAM_NEWCALL )
                  {
                     BOS_UINT32 uAppIx;

                     if ( pCmd->cmd.sig.pData &&
                          strlen ( (const char *) pCmd->cmd.sig.pData ) )
                     {
                        /* Assign the application identifier.
                        */
                        uAppIx =
                           tcamResSetAppIx ( (BOS_UINT8 *) pCmd->cmd.sig.pData,
                                             pCmd->cmd.sig.uApp );
                     }
                     else
                     {
                        /* Assign a default application identifier.
                        */
                        uAppIx = tcamResSetAppIx ( (BOS_UINT8 *) "Private",
                                                   pCmd->cmd.sig.uApp );
                     }

                     if ( uAppIx != TCAM_INVALID_HANDLE )
                     {
                        TCAMCMGREVT cmgrEvt;

                        /* Pass the new call to the outside world.
                        */
                        tcamComWorld ( eTCAM_NEWCALL, uAppIx, NULL );

                        /* Inform the upper layer that this call is an incoming
                        ** call.
                        */
                        tcamComWorld ( eTCAM_DIRINC, uAppIx, NULL );

                        TCAMLOG_INFORMATION ((
                           "tcamCbCmgrEptCtlCb: posting handle 0x%lX for 0x%lX",
                           uAppIx, pCmd->cmd.sig.uApp ));

                        /* Setup the application handle that is going to be used
                        ** later on when other events are passed for this call.
                        */
                        memset ( (void *) &cmgrEvt, 0, sizeof ( TCAMCMGREVT ) );
                        cmgrEvt.uEvt = eCMCMD_APPCALLHDL;
                        cmgrEvt.uDat1 = pCmd->cmd.sig.uApp;
                        cmgrEvt.uDat2 = uAppIx;

                        tcamAsyncCmgrCbPost ( &cmgrEvt );
                     }
                     else
                     {
                        BOS_UINT32 uAppIx = pCmd->cmd.sig.uApp;
                        TCAMCMGREVT cmgrEvt;

                        /* Send the command to the Call Manager for call termination.
                        */
                        TCAMLOG_INFORMATION ((
                           "tcamCbCmgrEptCtlCb: posting 0x%lX (no valid handle)",
                           uAppIx ));

                        memset ( (void *) &cmgrEvt, 0, sizeof ( TCAMCMGREVT ) );
                        cmgrEvt.uEvt = eCMCMD_ENDCALL;
                        cmgrEvt.uDat1 = uAppIx;

                        tcamAsyncCmgrCbPost ( &cmgrEvt );
                     }
                  }
                  else
                  {
                     BOS_UINT32 uAppIx = pCmd->cmd.sig.uApp;
                     TCAMCMGREVT cmgrEvt;

                     /* Pass the event to the outside world.
                     */
                     tcamComWorld ( eTcamEvt, uAppIx, NULL );

                     if ( eTcamEvt == eTCAM_DISCONNECT )
                     {
                        /* Clean up the CMGR resources as well.
                        */
                        TCAMLOG_INFORMATION ((
                           "tcamCbCmgrEptCtlCb: posting 0x%lX (disconnected)",
                           uAppIx ));

                        memset ( (void *) &cmgrEvt, 0, sizeof ( TCAMCMGREVT ) );
                        cmgrEvt.uEvt = eCMCMD_ENDCALL;
                        cmgrEvt.uDat1 = uAppIx;

                        tcamAsyncCmgrCbPost ( &cmgrEvt );
                        tcamResFreeAppIx ( uAppIx );
                     }
                  }
               }
            }
            break;

            case eCMGREPSIGACT_STOP:
            default:
            break;
         }
      }
      break;

      default:
      break;
   }
}


/****************************************************************************
** FUNCTION:   tcamCbCmgrProvCb
**
** PURPOSE:    Callback functionality for provisioning control outside of
**             the Call Manager.
**
** PARAMETERS: cfgItem  - the provisioning item of interest.
**             pCfgIx   - the index associated with the provisioning item.
**             pData    - pointer to the placeholder for the provisioning
**                        information.
**             eCfgAct  - the provisioning action to take.
**
** RETURNS:    BOS_STATUS_OK on success, BOS_STATUS_ERROR otherwise.
**
** NOTE:
*****************************************************************************/
BOS_STATUS tcamCbCmgrProvCb ( PROV_CFG_ITEM cfgItem,
                              PROV_CFG_IDX *pCfgIx,
                              void *pData,
                              CFG_ACCESS_ACT eCfgAct )
{
   BOS_STATUS eRet = BOS_STATUS_ERR;

   TCAMLOG_DEBUG (( "tcamCbCmgrProvCb: item (%d) - id (%d, %s) - action (%d)",
                    cfgItem,
                    (pCfgIx != NULL) ? pCfgIx->uIndex : 0,
                    ((pCfgIx != NULL) && (pCfgIx->pcCharIdx != NULL)) ?
                        pCfgIx->pcCharIdx : (BOS_UINT8*) "(null)",
                    eCfgAct ));

   /* At this time, we simply invoke the interface callback which is the
   ** location where the TCAM can integrate processing with others third
   ** party modules.
   */
   eRet = tcamIfcCfg ( cfgItem,
                       pCfgIx,
                       pData,
                       eCfgAct );

   return eRet;
}


/****************************************************************************
** FUNCTION:   tcamCbCmgrSesCtlCb
**
** PURPOSE:    Callback functionality for session control outside of
**             the Call Manager.
**
** PARAMETERS: eCmd  - the session command to apply.
**             uPhy  - the physical handle associated with this session.
**             uHdl  - the handle for which this command applies.
**             pData - additional data (may be NULL) necessary to complete
**                     the action.
**
** RETURNS:    The reference index for the session of interest as allocated
**             by the outside world.
**
** NOTE:
*****************************************************************************/
BOS_UINT32 tcamCbCmgrSesCtlCb ( CMGRSESCMD eCmd, BOS_UINT32 uPhy,
                                BOS_UINT32 uHdl, void *pData )
{
   /* At this time, we simply invoke the interface callback which is the
   ** location where the TCAM can integrate processing with others third
   ** party modules.
   */
   BOS_UINT32 uRet = TCAM_INVALID_HANDLE;

   TCAMLOG_DEBUG (( "tcamCbCmgrSesCtlCb: cmd (%d) - phy (0x%lX) - hdl (0x%lX)",
                    eCmd, uPhy, uHdl ));

   uRet = tcamIfcEpt ( eCmd,
                       uPhy,
                       uHdl,
                       pData );

   return uRet;
}


/***********************************************************************************
**  FUNCTION:   tcamCbCmgrStatusCb
**
**  PURPOSE:    Callback from Call Manager to deliver status information to the
**              outer application.
**
**  PARAMETERS: uRid - resource identifier, may be used in some specific status
**                     information when necessary.
**              eStatus - the status reported.
**
**  RETURNS:    Nothing.
**
***********************************************************************************/
void tcamCbCmgrStatusCb ( BOS_UINT32 uRid, CMGRSTATUS eStatus )
{
   /* At this time, there is no need to worry about any of those callbacks.
   */
   (void) uRid;
   (void) eStatus;

   TCAMLOG_DEBUG (( "tcamCbCmgrStatusCb: rid (0x%lX) - status (%d)",
                    uRid, eStatus ));
}
