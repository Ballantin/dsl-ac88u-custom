/**********************************************************************************
** Copyright © 2009 Broadcom
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
**      This file defines the Telephony Client Application Manager callbacks.
**
***********************************************************************************/

#ifndef __TCAM_CB__H__INCLUDED__
#define __TCAM_CB__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */

/* ---- Constants and Types ----------------------------------------------------- */


/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

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
void tcamCbCmgrEvtCb ( CMGREVT eEvt, BOS_UINT32 uUsr, void *pData );


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
void tcamCbCmgrEptCtlCb ( CMGREPCMD *pCmd );


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
                              CFG_ACCESS_ACT eCfgAct );


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
                                BOS_UINT32 uHdl, void *pData );


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
void tcamCbCmgrStatusCb ( BOS_UINT32 uRid, CMGRSTATUS eStatus );


#if defined(__cplusplus)
}
#endif

#endif /* __TCAM_CB__H__INCLUDED__ */

