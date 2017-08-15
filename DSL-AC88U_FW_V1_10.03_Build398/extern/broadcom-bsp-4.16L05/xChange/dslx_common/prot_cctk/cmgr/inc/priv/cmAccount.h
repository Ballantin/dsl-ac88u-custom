/**********************************************************************************
** Copyright (c) 2012-2013 Broadcom
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

#ifndef __CM_ACCOUNT__H__INCLUDED__
#define __CM_ACCOUNT__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */

/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmAccntEventHandler
**
** PURPOSE:    Main entry for managing the account events.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntEventHandler ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmAccntInSrv
**
** PURPOSE:    Account in-service handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntInSrv ( void *pRes );


/***********************************************************************************
** FUNCTION:   cmAccntOutOfSrv
**
** PURPOSE:    Account out-of-service handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntOutOfSrv ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmAccntTargetsUpdate
**
** PURPOSE:    Account information that the target has been updated.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       None.
***********************************************************************************/
void cmAccntTargetsUpdate ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmAccntPublishTarget
**
** PURPOSE:    Account publish target handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmAccntPublishTarget ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmAccntUpdate
**
** PURPOSE:    Account information update handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntUpdate ( void *pRes );

/***********************************************************************************
** FUNCTION:   cmAccntPauUpdate
**
** PURPOSE:    Account P-Associated-URI update handler.
**
** PARAMETERS: pRes - the pointer to the account resource to manage the event on.
**
** RETURNS:    None.
***********************************************************************************/
void cmAccntPauUpdate ( void *pRes );

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
void cmAccntSubInfo ( void *pRes );

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
void cmAccntCdrInfo ( void *pRes );

#if defined(__cplusplus)
}
#endif

#endif /* __CM_FEAT__H__INCLUDED__ */

