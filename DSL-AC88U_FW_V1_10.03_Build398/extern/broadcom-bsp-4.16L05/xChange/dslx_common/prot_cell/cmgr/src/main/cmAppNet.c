/**********************************************************************************
** Copyright (c) 2015 Broadcom Corporation
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
**      This file contains the Call Manager Network interface implementation.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmAppNetStartup
**
** PURPOSE:    Registers the callback for network events.
**
** PARAMETERS: epEvtCb - the callback used by the application to
**                       notify CMGR of network events.
**
** RETURNS:    eCMGRSTATUS_STARTUP_SUCCESS on success
**             eCMGRSTATUS_STARTUP_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmAppNetStartup ( CMGRNETEVT netEvtCb )
{
   
   /* Post this message to the application.
   */
   if ( gCmCfgCb.cmgrNetReqCb )
   {
      ( gCmCfgCb.cmgrNetReqCb ) ( CMGRNETREQ_REGISTER,
                                  CMGR_INVALID_HANDLE,
                                  &netEvtCb,
                                  NULL );
                                  
   }
   else
   {
      return eCMGRSTATUS_STARTUP_FAILED; 
   }
   
   return eCMGRSTATUS_STARTUP_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cmAppNetCallCreate
**
**  PURPOSE:    Requests that the controlling application create a call on the
**              network.
**
**  PARAMETERS:
**              pCid - Call id (created on return).
**              uUid - User id for the call.
**              pcTo - Call destination address (dial-string or URI).
**
**
**  RETURNS:    Return status.
***********************************************************************************/
CMAPPNETSTATUS cmAppNetCallCreate( CMSES *pSes, const BOS_UINT8 *pcTo )
{
   
   CMGRLOG_INFORMATION (( "cmAppNetCallCreate(%p, %p)", pcTo ));
   if ( pSes == NULL )
   {
      CMGRLOG_CRITICAL (("cmAppNetCallCreate: Bad parameters." ));
      return eCMAPPNETSTATUS_FAIL;
   }
                          
   /* Post this message to the application.
   */
   if ( gCmCfgCb.cmgrNetReqCb )
   {
      CMGRLOG_DEBUG (("cmAppNetCallCreate: Call to %s | %s", pSes->ses.med.uDigCol, pcTo ));

      ( gCmCfgCb.cmgrNetReqCb ) ( CMGRNETREQ_CALL_MAKE,
                                  CMGR_INVALID_HANDLE,
                                  pSes->ses.med.uDigCnt ? pSes->ses.med.uDigCol : NULL,
                                  &pSes->uNet );
      CMGRLOG_DEBUG (( "cmAppNetCallCreate: assigned net 0x%X for ses (0x%X) user 0x%X",
                          pSes->uNet, pSes->uHdl, pSes->uUsr ));
   }
   else
   {
      return eCMAPPNETSTATUS_FAIL;
   }
   
   return eCMAPPNETSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cmAppNetCallAnswer
**
**  PURPOSE:    Requests that the controlling application answer an incoming call.
**
**  PARAMETERS:
**              uNetCid - the network handle for this call.
**
**
**  RETURNS:    Return status.
***********************************************************************************/
CMAPPNETSTATUS cmAppNetCallAnswer( BOS_UINT32 uNetCid )
{
   
   CMGRLOG_INFORMATION (( "cmAppNetCallAnswer(0x%X)", uNetCid ));
   if ( uNetCid == CMGR_INVALID_HANDLE )
   {
      CMGRLOG_CRITICAL (("cmAppNetCallAnswer: Bad parameters." ));
      return eCMAPPNETSTATUS_FAIL;
   }
                          
   /* Post this message to the application.
   */
   if ( gCmCfgCb.cmgrNetReqCb )
   {
      ( gCmCfgCb.cmgrNetReqCb ) ( CMGRNETREQ_CALL_ANSWER,
                                  uNetCid,
                                  NULL,
                                  NULL );
   }
   else
   {
      return eCMAPPNETSTATUS_FAIL;
   }
   
   return eCMAPPNETSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cmAppNetCallRelease
**
**  PURPOSE:    Requests that the controlling application terminate a call.
**
**  PARAMETERS:
**              uNetCid - the network handle for this call.
**
**
**  RETURNS:    Return status.
***********************************************************************************/
CMAPPNETSTATUS cmAppNetCallRelease( BOS_UINT32 uNetCid )
{
   
   CMGRLOG_INFORMATION (( "cmAppNetCallRelease(0x%X)", uNetCid ));
   if ( uNetCid == CMGR_INVALID_HANDLE )
   {
      CMGRLOG_CRITICAL (("cmAppNetCallRelease: Bad parameters." ));
      return eCMAPPNETSTATUS_FAIL;
   }
                          
   /* Post this message to the application.
   */
   if ( gCmCfgCb.cmgrNetReqCb )
   {
      ( gCmCfgCb.cmgrNetReqCb ) ( CMGRNETREQ_CALL_RELEASE,
                                  uNetCid,
                                  NULL,
                                  NULL );
   }
   else
   {
      return eCMAPPNETSTATUS_FAIL;
   }
   
   return eCMAPPNETSTATUS_SUCCESS;
}