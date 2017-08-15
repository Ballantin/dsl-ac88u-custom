/**********************************************************************************
** Copyright (c) 2008-2012 Broadcom
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
**      This file contains the Call Manager main.
**
***********************************************************************************/

#ifndef __CM_MAIN__H__INCLUDED__
#define __CM_MAIN__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>

/* ---- Constants and Types ----------------------------------------------------- */

extern CMGRSTATUSCB gCmStatusCb;
extern CMGRCFGBLK   gCmCfgCb;

/* Event structure definition which is used for communication into the
** Call Manager thread.
*/
typedef struct
{
   BOS_UINT32 uEvt;
   BOS_UINT32 uDat1;
   BOS_UINT32 uDat2;
   void       *pDat3;
   BOS_UINT32 uDat4;
   BOS_BOOL   bLast;

} CMEVT;

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmMainRemoveUnusedParamWarning
**
** PURPOSE:    Utility function used to remove warnings for unused parameters
**             within function call.
**
** PARAMETERS: pParam - the parameter not used.
**
** RETURNS:    None.
***********************************************************************************/
void *cmMainRemoveUnusedParamWarning( void *pParam );


/***********************************************************************************
** FUNCTION:   cmMainInit
**
** PURPOSE:    Creates the Call Manager support.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing, however the status callback will be invoked when this
**             action is terminated.  The controlling application must wait until
**             that callback has been invoked prior to using the Call Manager for
**             any functionality.
***********************************************************************************/
void cmMainInit ( void );


/***********************************************************************************
** FUNCTION:   cmMainDeinit
**
** PURPOSE:    Destroy the Call Manager support.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing, however the status callback will be invoked when this
**             action is terminated.  
***********************************************************************************/
void cmMainDeinit ( void );


/***********************************************************************************
** FUNCTION:   cmPostEvt
**
** PURPOSE:    Post an event to the Call Manager queue.
**
** PARAMETERS: pEvt - pointer to the event to queue.
**
** RETURNS:    BOS_STATUS_OK in case of success, an error code otherwise.
***********************************************************************************/
BOS_STATUS cmPostEvt( CMEVT *pEvt );


/***********************************************************************************
** FUNCTION:   cmSignalEvt
**
** PURPOSE:    Signals an event to the Call Manager.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void cmSignalEvt( void );

/***********************************************************************************
** FUNCTION:   cmUpdateState
**
** PURPOSE:    sets the state of the Call Manager.
**
** PARAMETERS: uRid - ID, not used ny Call Manager, just pass up to the app
**             eStatus - state of the Call Manager
**
** RETURNS:    None.
***********************************************************************************/
void cmUpdateState( BOS_UINT32 uRid,
                    CMGRSTATUS eStatus );

/***********************************************************************************
** FUNCTION:   cmGetState
**
** PURPOSE:    returns the state of the Call Manager.
**
** PARAMETERS:  uRid - ID, not used currently
**
** RETURNS:    The state of Call Manager
***********************************************************************************/
CMGRSTATUS cmGetState( BOS_UINT32 uRid );

#if defined(__cplusplus)
}
#endif


/* Special macro to be used to remove warnings on parameter not used within
** a function.
*/
#define CMGR_REMOVE_UNUSED_PARAM_WARNING( X )                                     \
   cmMainRemoveUnusedParamWarning( (void *)&(X) );


#endif /* __CM_MAIN__H__INCLUDED__ */

