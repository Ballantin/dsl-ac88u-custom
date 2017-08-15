/**********************************************************************************
** Copyright © 2008 Broadcom
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
**      This module defines the basic API necessary to run the call client
**      application which is the user application of the GUI.
**
***********************************************************************************/

#ifndef __CALLCLIENT__H__INCLUDED__
#define __CALLCLIENT__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */

#include "gui.h"


/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Function Prototypes -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   appInit
**
**  PURPOSE:    Initialize the application.
**
**  PARAMETERS: None
**
**  RETURNS:    Nothing
***********************************************************************************/
void appInit ( void );


/***********************************************************************************
**  FUNCTION:   appExit
**
**  PURPOSE:    Terminates the application.
**
**  PARAMETERS: None
**
**  RETURNS:    Nothing
***********************************************************************************/
void appExit ( void );


/***********************************************************************************
**  FUNCTION:   appReceive
**
**  PURPOSE:    Receives packet for the application.
**
**  PARAMETERS: None
**
**  RETURNS:    Nothing
***********************************************************************************/
void appReceive ( void );


/***********************************************************************************
**  FUNCTION:   appButton
**
**  PURPOSE:    Manages button press for the application.
**
**  PARAMETERS: id - button identifier
**              msg - message content
**
**  RETURNS:    Nothing
***********************************************************************************/
BOS_BOOL appButton( int id, int msg );


/***********************************************************************************
**  FUNCTION:   appMenu
**
**  PURPOSE:    Manages menu information for the application.
**
**  PARAMETERS: id - button identifier
**
**  RETURNS:    Nothing
***********************************************************************************/
BOS_BOOL appMenu( int id );


/***********************************************************************************
**  FUNCTION:   appSetup
**
**  PURPOSE:    Manages setup information for the application.
**
**  PARAMETERS: None
**
**  RETURNS:    Nothing
***********************************************************************************/
void appSetup( void );


/***********************************************************************************
** FUNCTION:   appCmgrDeInit
**
** PURPOSE:    Terminate cleanup of the Call Manager application.
**
** PARAMETERS: None
**
** RETURNS:    None.
***********************************************************************************/
void appCmgrDeInit( void );

/***********************************************************************************
** FUNCTION:   appPicCtrlNotify
**
** PURPOSE:    Picture Control Notification Handler
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
BOS_BOOL appPicCtrlNotify(int id, int msg);


#endif /* __CALLCLIENT__H__INCLUDED__ */
