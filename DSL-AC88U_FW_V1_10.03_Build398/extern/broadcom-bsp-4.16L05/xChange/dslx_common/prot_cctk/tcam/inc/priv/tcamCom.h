/**********************************************************************************
** Copyright © 2009 - 2010 Broadcom
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
**      This file defines the Telephony Client Application Manager Communication
**      layer.
**
***********************************************************************************/

#ifndef __TCAM_COM__H__INCLUDED__
#define __TCAM_COM__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */

/* ---- Constants and Types ----------------------------------------------------- */

/* Maximum size of a TCAM action name.
*/
#define TCAM_COM_ACTION_LEN  32

/* Maximum size of each parameter with a TCAM message.
*/
#define TCAM_COM_PARM_LEN    127

/* Maximum number of parameters associated with a TCAM message.
*/
#define TCAM_COM_PARM_NUM    5

/* User and P-CSCF identifier holder.
*/
extern BOS_UINT8 guUsrIdentity[ TCAM_USR_IDENTIFIER_LEN + 1 ];
extern BOS_UINT8 guPcscfIdentity[ TCAM_PCSCF_IDENTIFIER_LEN + 1 ];

/* The holder for sending out an action (with parameter) to the outside
** world.  Format is "ACTION<comma>PARAM*(<comma>PARAM)".
*/
extern BOS_UINT8 guTcamMsg[ TCAM_COM_ACTION_LEN + 1 +  \
                              TCAM_COM_PARM_NUM * (TCAM_COM_PARM_LEN + 1) ];


/* This defines the events exchanged with the outside application to drive the
** application states.
**
** Each event can either be a command from the outside application to TCAM or an
** event from TCAM to the application as the result of an underlying action.
*/
typedef enum
{
   /* Call related.
   */
   eTCAM_DIAL,
   eTCAM_HANGUP,
   eTCAM_ANSWER,
   eTCAM_ALERT,
   eTCAM_NEWCALL,
   eTCAM_ACTIVE,
   eTCAM_DISCONNECT,
   eTCAM_HOLD,
   eTCAM_UNHOLD,
   eTCAM_REMOTE_HOLD,
   eTCAM_REMOTE_UNHOLD,
   eTCAM_CONF,
   eTCAM_CONFEND,
   eTCAM_BXFER,
   eTCAM_CXFER,
   eTCAM_XFEREND,
   eTCAM_HANDLE,
   eTCAM_DTMF,
   eTCAM_CALL_XFER,

   /* Configuration related.
   */
   eTCAM_CFG,
   eTCAM_USRCFG,
   eTCAM_USRDEL,
   eTCAM_SVCRDY,
   eTCAM_SVCNRDY,
   eTCAM_RESTART,

   /* Message Pass Throug related.
   */
   eTCAM_MESSAGE,
   eTCAM_MSG_RSP,
   eTCAM_MSG_HDL,

   /* Internal usage (misc information).
   */
   eTCAM_DIRINC,
   eTCAM_DIROUT,
   eTCAM_DISC,
   eTCAM_DTMFPLAY,
   eTCAM_ANSWERED,
   eTCAM_SVCOK,
   eTCAM_SVCFAIL,
   eTCAM_MWION,
   eTCAM_MWIOFF,
   eTCAM_CONFEST,

   eTCAM_INVALID,

} TCAMEVT;

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   tcamCom
**
** PURPOSE:    Entry point into the communication layer for receiving and processing
**             commands from the outside application.
**
** PARAMETERS: pCmdStr - the command string received.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
void tcamCom ( BOS_UINT8 *pCmdStr );


/***********************************************************************************
** FUNCTION:   tcamComParse
**
** PURPOSE:    Parse a command received.
**
** PARAMETERS: pCmdStr - the command string to be parsed.
**
** RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
**
** NOTE:       The result of the parsed command is saved in local variable(s)
**             to be used immediately following.  Since the process here is
**             synchronous, that works just fine.  Note that if the function
**             return an error (BOS_FALSE), the content of the local variable(s)
**             should be ignored.
***********************************************************************************/
BOS_BOOL tcamComParse ( BOS_UINT8 *pCmdStr );


/***********************************************************************************
** FUNCTION:   tcamComCmgr
**
** PURPOSE:    Invoke the proper CMGR command based on the last known received
**             and parsed command.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamComCmgr ( void );


/***********************************************************************************
** FUNCTION:   tcamComCfg
**
** PURPOSE:    Invoke the handler for configuration update to the GUI application
**             when such request is received.
**
** PARAMETERS: eEvt - the event to publish.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamComCfg ( TCAMEVT eEvt );


/***********************************************************************************
** FUNCTION:   tcamComWorld
**
** PURPOSE:    Invoke the callback into the outside world to issue an event to
**             the application.
**
** PARAMETERS: eTcamEvt - the TCAM event to notify.
**             uAppIx - the TCAM application handle used to match to the outside
**                      world application handle.
**             pData - extra information data associated with the command.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamComWorld ( TCAMEVT eTcamEvt, BOS_UINT32 uAppIx, void *pData );


/***********************************************************************************
** FUNCTION:   tcamComPcscf
**
** PURPOSE:    Retrieve the P-CSCF information for the default user account.
**
** PARAMETERS: Nothing.
**
** RETURNS:    Pointer to the P-CSCF information retrieved.
**
** NOTE:
***********************************************************************************/
BOS_UINT8 * tcamComPcscf ( void );


/***********************************************************************************
** FUNCTION:   tcamComUser
**
** PURPOSE:    Retrieve the user information for the default user account.
**
** PARAMETERS: Nothing.
**
** RETURNS:    Pointer to the user information retrieved.
**
** NOTE:       TCAM only supports one user at the time being, so the user
**             retrieved through this call is always the initial user configured.
***********************************************************************************/
BOS_UINT8 * tcamComUser ( void );


/***********************************************************************************
** FUNCTION:   tcamComValid
**
** PURPOSE:    Applies some validation on a parsed message content based on the
**             event type and the arguments given.
**
** PARAMETERS: eTcamEvt - the TCAM event.
**             uArgs - the number of arguments processed.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
BOS_BOOL tcamComValid ( TCAMEVT eTcamEvt, BOS_UINT32 uArgs );


#if defined(__cplusplus)
}
#endif

#endif /* __TCAM_COM__H__INCLUDED__ */

