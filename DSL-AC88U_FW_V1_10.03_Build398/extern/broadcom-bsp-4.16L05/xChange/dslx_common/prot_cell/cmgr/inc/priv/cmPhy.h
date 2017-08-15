/**********************************************************************************
** Copyright © 2008 - 2010 Broadcom
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
**      This file contains the Call Manager physical interface definition.
**
***********************************************************************************/

#ifndef __CM_PHY__H__INCLUDED__
#define __CM_PHY__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>
#include <cmgrMainCfg.h>
#include <cmUsr.h>

/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Public variables -------------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
** FUNCTION:   cmPhyStartup
**
** PURPOSE:    Launches the physical (endpoint) interface.
**
** PARAMETERS: epEvtCb - the callback used by the external physical layer to
**                       notify CMGR of events.
**
** RETURNS:    eCMGRSTATUS_STARTUP_SUCCESS on success
**             eCMGRSTATUS_STARTUP_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyStartup ( CMGREPEVT epEvtCb );


/***********************************************************************************
** FUNCTION:   cmPhyIsStarted
**
** PURPOSE:    Checks whether the physical (endpoint) interface is started.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_TRUE if the physical interface had been started at some point.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmPhyIsStarted ( void );


/***********************************************************************************
** FUNCTION:   cmPhyShutdown
**
** PURPOSE:    Shutsdown the physical (endpoint) interface.
**
** PARAMETERS: None.
**
** RETURNS:    eCMGRSTATUS_SHUTDOWN_COMPLETE on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyShutdown ( void );


/***********************************************************************************
** FUNCTION:   cmPhyTerminate
**
** PURPOSE:    Terminates (destroy) all the created physical (endpoint)
**             interface(s).
**
** PARAMETERS: None.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyTerminate ( void );


/***********************************************************************************
** FUNCTION:   cmPhyCreate
**
** PURPOSE:    Creates a specific physical (endpoint) interface.
**
** PARAMETERS: uPhy - the interface identifier to be created.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyCreate ( BOS_UINT32 uPhy );


/***********************************************************************************
** FUNCTION:   cmPhyCreate
**
** PURPOSE:    Destroys a specific physical (endpoint) interface.
**
** PARAMETERS: uPhy - the interface identifier to be destroyed.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyDestroy ( BOS_UINT32 uPhy );


/***********************************************************************************
** FUNCTION:   cmPhySignal
**
** PURPOSE:    Signals something to a physical (endpoint) interface.
**
** PARAMETERS: uPhy - physical interface to signal to.
**             pSig - what to signal.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhySignal ( BOS_UINT32 uPhy, CMGREPSIG *pSig );


/***********************************************************************************
** FUNCTION:   cmPhyAllocate
**
** PURPOSE:    Allocates a physical interface with a given user if none is
**             associated with it at this time.
**
** PARAMETERS: pUsr - the user to associate an interface with.
**
** RETURNS:    eCMGRSTATUS_COMMAND_SUCCESS on success
**             eCMGRSTATUS_COMMAND_FAILED otherwise.
***********************************************************************************/
CMGRSTATUS cmPhyAllocate ( CMUSR *pUsr );


/***********************************************************************************
** FUNCTION:   cmPhyToneFromString
**
** PURPOSE:    Attemps to lookup a tone information to be applied on the physical
**             interface from its string representation value.
**
** PARAMETERS: puStr - the tone string information.
**
** RETURNS:    The tone identification if found, CMGR_INVALID_HANDLE otherwise.
***********************************************************************************/
BOS_UINT32 cmPhyToneFromString ( BOS_UINT8 *puStr );


/***********************************************************************************
** FUNCTION:   cmPhyToneWithClid
**
** PURPOSE:    Maps a tone type to its equivalent tone when CLID is enabled with
**             this tone.
**
** PARAMETERS: uSig - the tone to map.
**             bClid - whether CLID is enabled for this tone.
**
** RETURNS:    uSig if CLID is not enabled, the mapped equivalent of uSig if CLID
**             is enabled.
***********************************************************************************/
BOS_UINT32 cmPhyToneWithClid ( BOS_UINT32 uSig, BOS_BOOL bClid );


/***********************************************************************************
** FUNCTION:   cmPhyToneValidateRing
**
** PURPOSE:    Validates that the tone passed as input corresponds to the a ring
**             tone (alerting tone).
**
** PARAMETERS: uSig - the tone to check.
**
** RETURNS:    Identity if valid, CMGR_INVALID_HANDLE otherwise.
***********************************************************************************/
BOS_UINT32 cmPhyToneValidateRing ( BOS_UINT32 uSig );


/***********************************************************************************
** FUNCTION:   cmPhyToneValidateCallWait
**
** PURPOSE:    Validates that the tone passed as input corresponds to the a call
**             waiting tone.
**
** PARAMETERS: uSig - the tone to check.
**
** RETURNS:    Identity if valid, CMGR_INVALID_HANDLE otherwise.
***********************************************************************************/
BOS_UINT32 cmPhyToneValidateCallWait ( BOS_UINT32 uSig );


/***********************************************************************************
** FUNCTION:   cmPhyToneRing2CallWait
**
** PURPOSE:    Checks whether the tone passed is a valid ring tone and if so change
**             it to a corresponding valid call waiting tone.
**
** PARAMETERS: uSig - the tone to check.
**
** RETURNS:    Tone mapped if valid, CMGR_INVALID_HANDLE otherwise.
***********************************************************************************/
BOS_UINT32 cmPhyToneRing2CallWait ( BOS_UINT32 uSig );


#if defined(__cplusplus)
}
#endif

#endif /* __CM_PHY__H__INCLUDED__ */

