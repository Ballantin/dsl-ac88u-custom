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
**      This file defines the Telephony Client Application Manager Resources
**      management.
**
***********************************************************************************/

#ifndef __TCAM_RES__H__INCLUDED__
#define __TCAM_RES__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */

/* ---- Constants and Types ----------------------------------------------------- */

typedef struct
{
   BOS_UINT32 uAppIx;
   BOS_UINT32 uAppIdent;
   BOS_UINT8  uAppId [ TCAM_COM_PARM_LEN + 1 ];

} TCAMAPPMAP;


/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   tcamResInit
**
** PURPOSE:    Initializes the TCAM resource layer.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamResInit ( void );


/***********************************************************************************
** FUNCTION:   tcamResSetAppIx
**
** PURPOSE:    Assign an application index based on the external (pAppId) unique
**             identifier for this resource.
**
** PARAMETERS: pAppId - the (external to TCAM) string identifier for the resource.
**             uAppId - the (external to TCAM) GUID identifier for the resource.
**
** RETURNS:    The TCAM internal application index.
**
** NOTE:       The TCAM internal application index is used for interface with the
**             CMGR actions/events.
**
**             The combination of pAppId and uAppId create a unique identifier
**             for the resource, uAppId in particular is guaranteed to be unique
**             by the CMGR.
***********************************************************************************/
BOS_UINT32 tcamResSetAppIx ( BOS_UINT8 *pAppId, BOS_UINT32 uAppId );


/***********************************************************************************
** FUNCTION:   tcamResFreeAppIx
**
** PURPOSE:    Release an application index identifier from usage.
**
** PARAMETERS: uAppIx - the TCAM identifier for the resource.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamResFreeAppIx ( BOS_UINT32 uAppIx );


/***********************************************************************************
** FUNCTION:   tcamResFindAppId
**
** PURPOSE:    Locates the external (to TCAM) application identifier to be used
**             for communication about a given call.
**
** PARAMETERS: uAppIx - the TCAM identifier for the resource.
**
** RETURNS:    The external application identifier.
**
** NOTE:       .
***********************************************************************************/
BOS_UINT8 *tcamResFindAppId ( BOS_UINT32 uAppIx );


/***********************************************************************************
** FUNCTION:   tcamResFindAppIx
**
** PURPOSE:    Locates the internal (to TCAM) application identifier to be used
**             for communication with CMGR.
**
** PARAMETERS: uAppId - the external identifier of the resource.
**
** RETURNS:    The TCAM internal application identifier.
**
** NOTE:       .
***********************************************************************************/
BOS_UINT32 tcamResFindAppIx ( BOS_UINT8 *uAppId );


/***********************************************************************************
** FUNCTION:   tcamResReplaceAppIx
**
** PURPOSE:    Replace an assigned application identifier with the new one
**             provided.
**
** PARAMETERS: uAppIx - the identifier for the resource to update.
**             pAppId - the application identifier to use as replacement
**
** RETURNS:    Nothing.
**
** NOTE:       This function is used to replace a temporary (or no longer valid)
**             application identifier with a one provided by a third party.  A
**             typical usage example of this is when following an incoming call
**             the user application has been made aware of the call and it is
**             assigning its own call handle to track it.
***********************************************************************************/
void tcamResReplaceAppIx ( BOS_UINT32 uAppIx, BOS_UINT8 *pAppId );


/***********************************************************************************
** FUNCTION:   tcamResRemapAppHdl
**
** PURPOSE:    Remap a selected application handle if it is not the proper one.
**
** PARAMETERS: uAppIdent - the internal handle to check for.
**             puAppId - the application identifier to use as replacement.
**
** RETURNS:    Nothing.
**
** NOTE:       .
***********************************************************************************/
void tcamResRemapAppHdl ( BOS_UINT32 uAppIdent, BOS_UINT32 *puAppId );


#if defined(__cplusplus)
}
#endif

#endif /* __TCAM_RES__H__INCLUDED__ */

