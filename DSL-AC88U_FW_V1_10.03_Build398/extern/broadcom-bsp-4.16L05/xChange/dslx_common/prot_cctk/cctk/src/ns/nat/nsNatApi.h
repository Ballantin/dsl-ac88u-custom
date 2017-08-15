/**********************************************************************************
** Copyright © 2008-2010 Broadcom
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
**      This file defines the API (application programming interface) to the
**      Call Control Toolkit Network Address Translation module.
**
***********************************************************************************/
#ifndef __NS_NAT_API__H__INCLUDED__
#define __NS_NAT_API__H__INCLUDED__

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Include Files ----------------------------------------------------------- */

/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   nsNatInit
**
**  PURPOSE:    NAT Control module initialization.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    None
**
**  NOTES:      This function initializes the NAT control module.
***********************************************************************************/
void nsNatInit( void );


/***********************************************************************************
**  FUNCTION:   nsNatDeInit
**
**  PURPOSE:    NAT Control module de-initialization.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    None
**
**  NOTES:      This function de-initializes the NAT control module.
***********************************************************************************/
void nsNatDeInit( void );


/***********************************************************************************
**  FUNCTION:   nsNatStartKeepAlive
**
**  PURPOSE:    Start a keep alive for a given resource (user).
**
**  PARAMETERS:
**              uResGuid - the GUID of the resource to apply this action on
**              pPeerAddr - peer address that the keep alive should be sent to
**
**  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
**
**  NOTES:      Will return BOS_TRUE if there is no keep alive to be started and
**              the action is still successful.
***********************************************************************************/
BOS_BOOL nsNatStartKeepAlive( BOS_UINT32 uResGuid, MXD_GNS::CSocketAddr *pPeerAddr );


/***********************************************************************************
**  FUNCTION:   nsNatStopKeepAlive
**
**  PURPOSE:    Stops a keep alive for a given resource (user).
**
**  PARAMETERS:
**              uResGuid - the GUID of the resource to apply this action on
**
**
**  RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
**
**  NOTES:      Will return BOS_TRUE if there is no keep alive to be stopped and
**              the action is still successful.
***********************************************************************************/
BOS_BOOL nsNatStopKeepAlive( BOS_UINT32 uResGuid );


/***********************************************************************************
**  FUNCTION:   nsNatEndTimer
**
**  PURPOSE:    Terminates all timer under control of the NAT module that may be
**              running at the time of the call.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    CCTKSTATUS based on result of action taken.
**
**  NOTES:      .
***********************************************************************************/
CCTKSTATUS nsNatEndTimer( void );

/***********************************************************************************
**  FUNCTION:   nsNatKaBackOff
**
**  PURPOSE:    Gets the value of the backoff timer for the keep alive recovery
**              process.
**
**  PARAMETERS:
**              uGuid - the resource GUID.
**
**  RETURNS:    The timer.  Can be 0 if configured by external means
**
**  NOTES:
***********************************************************************************/
BOS_UINT32 nsNatKaBackOff( BOS_UINT32 uResGuid );

#ifdef __cplusplus
}
#endif

#endif /* __NS_NAT_API__H__INCLUDED__ */
