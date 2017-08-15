/**********************************************************************************
** Copyright (c) 2015 Broadcom
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
**      This file contains the Call Manager CCTK Account mapping resource.
**
***********************************************************************************/

#ifndef __CM_NET__H__INCLUDED__
#define __CM_NET__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>
#include <cmSes.h>

/* ---- Constants and Types ----------------------------------------------------- */
/* Unique net identifier for Application controlled calls.
*/
#define CMGR_APPNET_HANDLE               0xFFFFFFFF

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/* Return status */
typedef enum CMAPPNETSTATUS
{
   /* Successful return status */
   eCMAPPNETSTATUS_FAIL,
   eCMAPPNETSTATUS_SUCCESS
   
} CMAPPNETSTATUS;

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
CMGRSTATUS cmAppNetStartup ( CMGRNETEVT netEvtCb );


/***********************************************************************************
**  FUNCTION:   cmAppNetCallCreate
**
**  PURPOSE:    Requests that the controlling application create a call on the
**              network
**
**  PARAMETERS:
**              pCid - Call id (created on return).
**              uUid - User id for the call.
**              pcTo - Call destination address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CMAPPNETSTATUS cmAppNetCallCreate( CMSES *pSes, const BOS_UINT8 *pcTo );

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
CMAPPNETSTATUS cmAppNetCallAnswer( BOS_UINT32 uNetCid );

/***********************************************************************************
**  FUNCTION:   cmAppNetCallCreate
**
**  PURPOSE:    Requests that the controlling application terminates a call.
**
**  PARAMETERS:
**              uNetCid - the network handle for this call.
**
**
**  RETURNS:    Return status.
***********************************************************************************/
CMAPPNETSTATUS cmAppNetCallRelease( BOS_UINT32 uNetCid );


#if defined(__cplusplus)
}
#endif

#endif /* __CM_NET__H__INCLUDED__ */

