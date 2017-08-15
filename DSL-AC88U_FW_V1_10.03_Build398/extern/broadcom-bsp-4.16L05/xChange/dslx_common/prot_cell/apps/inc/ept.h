/****************************************************************************
*  
*  Copyright © 2000-2008 Broadcom
*
*    This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.  
*
*    Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: ept.h
*
****************************************************************************
*    Description:
*
*      Endpoint abstraction layer.
*
****************************************************************************/

#ifndef EPT_H
#define EPT_H

#if defined(__cplusplus)
extern "C"
{
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <eptTypes.h>
#include <eptSig.h>
#include <eptProv.h>

/* ---- Constants and Types ---------------------------------------------- */

/* --------------------------------------------------------------------------
** Callback functions that needs to be registered with the endpoint.
*/

/*
*****************************************************************************
** FUNCTION:   eptEvtCB
**
** PURPOSE:    Endpoint event callback
**
** PARAMETERS: uIndex   - index containing both endpoint and cnx ID
**             eEvt     - the event
**             sData    - extra information (example: reason code).
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void (*eptEvtCB)
( 
   BOS_UINT32  uIndex, 
   EPTEVT      eEvt, 
   int         sData
);


/*
*****************************************************************************
** FUNCTION:   eptPktCB
**
** PURPOSE:    Endpoint connection packet callback
**
** PARAMETERS: uIndex      - index containing both endpoint and cnx ID
**             pPkt        - pointer the ingress packet to forward
**             uLength     - packet length including header
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void (*eptPktCB)
(
   BOS_UINT32     uIndex,
   EPTPACKET     *pPkt,
   BOS_UINT16     uLength
);


/*
*****************************************************************************
** FUNCTION:   eptProvGetCB
**
** PURPOSE:    Endpoint provisioned value callback
**
** PARAMETERS: uLineId        - line ID
**             eProvItemId    - the enumerated id of the provisionable item
**             pProvItemValue - pointer to the provisioned data value
**             uProvBufSize - size of the buffer to store provisioned value
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
typedef EPTSTATUS (*eptProvGetCB)
(
   BOS_UINT16     uLineId,
   EPTPROV        eProvItemId,
   void          *pProvItemValue,
   BOS_UINT32     uProvBufSize
);


/*
*****************************************************************************
** FUNCTION:   eptProvSetCB
**
** PURPOSE:    Endpoint provisioned value callback
**
** PARAMETERS: uLineId         - line ID
**             eProvItemId     - the enumerated id of the provisionable item
**             pProvItemValue  - pointer to the provisioned data value
**             pLength         - pointer to the length to set
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
typedef EPTSTATUS (*eptProvSetCB)
(
   BOS_UINT16     uLineId,
   EPTPROV        eProvItemId,
   void          *pProvItemValue,
   void          *pLength
);


/*
*****************************************************************************
** FUNCTION:   eptPktReleaseCB
**
** PURPOSE:    Callback handler invoked when the packet passed to endptRtpPacket
**             is released by the endpoint back to the caller.
**
** PARAMETERS: uCnxId      - connection identifier
**             uBufDesc    - buffer descriptor value passed to endptRtpPacket
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void ( *eptPktReleaseCB )
(
   BOS_UINT16     uCnxId,
   BOS_UINT32     uBufDesc
);


/*
*****************************************************************************
** FUNCTION:   eptInit
**
** PURPOSE:    Initialize the basic framework for endpoint processing.  This
**             also creates the resource manager which is used to track the
**             overall system.
**
** PARAMETERS: pEptInitCfg  - pointer to endpoint init config
**             evtCB        - Callback to invoke for notifying events
**             pktCB        - Callback to invoke for notifying packets
**             provGetCB    - Callback to get provisioned values
**             provSetCB    - Callback to set provisioned values
**             pktReleaseCB - Callback to invoke to release packets
**
** RETURNS:    EPTSTATUS
**
** NOTE:       If a callback does not apply, specify NULL.
**
*****************************************************************************
*/
EPTSTATUS eptInit
(
   EPTINITCFG          *pEptInitCfg,
   eptEvtCB             evtCB,
   eptPktCB             pktCB,
   eptProvGetCB         provGetCB,
   eptProvSetCB         provSetCB,
   eptPktReleaseCB      pktReleaseCB
);


/*
*****************************************************************************
** FUNCTION:   eptCreate
**
** PURPOSE:    Create a new endpoint instace on a particular physical/line 
**             interface.  An endpoint instance is required to establish
**             connections and to allow line processing on a particular ept.
**
** PARAMETERS: uPhysId  - physical ID
**             uLineId  - line ID
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
EPTSTATUS eptCreate
(
   BOS_UINT16     uPhysId, 
   BOS_UINT16     uLineId 
);


/*
*****************************************************************************
** FUNCTION:   eptDestroy
**
** PURPOSE:    De-initialize an endpoint instance.  Clean up any outstanding
**             resources on the particular line
**
** PARAMETERS: uLineId - line ID
**
** RETURNS:    EPTSTATUS
**
** NOTE:       This function simply cleans up any outstanding resources used
**             by the endpoint in question.  It does not free the resources to
**             be used by another endpoint since we currently do not support
**             per-endpoint shutdown yet.
**
*****************************************************************************
*/
EPTSTATUS eptDestroy
(
   BOS_UINT16     uLineId
);


/*
*****************************************************************************
** FUNCTION:   eptDeinit
**
** PURPOSE:    Shutdown endpoint processing including the Resource Manager.
**             This function should only be called after all the initialized
**             endpoints have been de-initialized via eptMgrDeinit.
**
** PARAMETERS: none
**
** RETURNS:    EPTSTATUS
**             This function should only return an error under catastrophic
**             circumstances. i.e. Something that cannot be fixed by re-invoking
**             the module initialization function.
**
** NOTE:       It is assumed that this function is only called after all endpoint
**             tasks have been notified of a pending application reset, and each
**             one has acknowledged the notification. This implies that each endpoint
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization functions.
**
*****************************************************************************
*/
EPTSTATUS eptDeinit( void );


/*
*****************************************************************************
** FUNCTION:   eptGetNumEpt
**
** PURPOSE:    Retrieve the maximum number of endpoints that are currently
**             configured to be supported
**
** PARAMETERS: None
**
** RETURNS:    Number of endpoints
**
*****************************************************************************
*/

BOS_UINT16 eptGetNumEpt( void );


/*****************************************************************************
*  FUNCTION:   eptGetCap
*
*  PURPOSE:    Retrieve the capabilities for an endpoint
*
*  PARAMETERS: uLineId  - line ID
*              pCap     - a pointer to the capabilities struct to populate
*
*  RETURNS:    EPTSTATUS
*
*****************************************************************************/
EPTSTATUS eptGetCap
(
   BOS_UINT16     uLineId,
   EPTCAP        *pCap
);


/*
*****************************************************************************
** FUNCTION:   eptProcessSignal
**
** PURPOSE:    Process a signal on the endpoint (or connection)
**
** PARAMETERS: uIndex      - index containing both endpoint and cnx ID
**             eSignal     - signal to generate / terminate
**             pSignalParm - pointer to signal paramters
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
EPTSTATUS eptProcessSignal
(
   BOS_UINT32     uIndex,
   EPTSIG         eSignal,
   EPTSIGPARM    *pSignalParm
);


/*
*****************************************************************************
** FUNCTION:   eptCreateCnx
**
** PURPOSE:    Function to create a connection
**
** PARAMETERS: uIndex      - index containing both endpoint and cnx ID
**             pCnxParm    - ptr to connection parameter block
**
** RETURNS:    EPTSTATUS
**
** NOTE:
*****************************************************************************
*/
EPTSTATUS eptCreateCnx
(
   BOS_UINT32     uIndex,
   EPTCNXPARM    *pCnxParm
);


/*
*****************************************************************************
** FUNCTION:   eptModifyCnx
**
** PURPOSE:    Function to modify a connection
**
** PARAMETERS: uIndex      - index containing both endpoint and cnx ID
**             pCnxParm    - ptr to connection parameter block
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
EPTSTATUS eptModifyCnx
(
   BOS_UINT32     uIndex,
   EPTCNXPARM    *pCnxParm
);


/*
*****************************************************************************
** FUNCTION:   eptDeleteCnx
**
** PURPOSE:    Function to delete a connection
**
** PARAMETERS: uIndex      - index containing both endpoint and cnx ID
**
** RETURNS:    EPTSTATUS - if success, fills in statistics block
**
*****************************************************************************
*/
EPTSTATUS eptDeleteCnx
(
   BOS_UINT32     uIndex
);


/*
*****************************************************************************
** FUNCTION:   eptMuteCnx
**
** PURPOSE:    Toggle the mute status of a connection on the endpoint.
**
** PARAMETERS: uIndex      - index containing both endpoint and cnx ID
**             bMuteAudio  - mute status for the connection
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
EPTSTATUS eptMuteCnx
(
   BOS_UINT32     uIndex,
   BOS_BOOL       bMuteAudio
);


/*
*****************************************************************************
** FUNCTION:   eptPkt
**
** PURPOSE:    Transmit a endpt packet to the endpoint. This function is asynchronous,
**             it does not wait for the data to be transmitted  before returning.
**             In addition, the buffer release mechanism is asynchronous.
**             The endpoint owns the packet passed to this function until a callback
**             is invoked releasing the packet back to the caller (eptPktReleaseCB).
**             The value of 'bufDesc' will be passed to the callback function so that
**             the caller can identify which packet is being released by the endpoint.
**
** PARAMETERS: uIndex      - index containing both endpoint and cnx ID
**             pEpPacket   - pointer to the EPPACKET packet.
**             uLength     - the length of the endpt packet including header
**             uBufDesc    - buffer descriptor value that uniquely identifies
**                          the packet. This value is returned in the
**                          callback invoked once the buffer has been completely
**                          transmitted. This value is only meaningful to the
**                          caller and is opaque to the endpoint.
**
** RETURNS:    EPTSTATUS
**
** NOTE:       If this function does not return eEPTSTATUS_SUCCESS, it is the caller's
**             responsibility to free the RTP packet.
**
*****************************************************************************
*/
EPTSTATUS eptPkt
(
   BOS_UINT32     uIndex,
   EPTPACKET     *pEpPacket,
   BOS_UINT16     uLength,
   BOS_UINT32     uBufDesc
);


/*
*****************************************************************************
** FUNCTION:   eptPowerSourceUpdate
**
** PURPOSE:    Event handler for modified power source
**
** PARAMETERS: bOnBatteryPower - flag indicating operating on battery power
**
** RETURNS:    nothing
**
*****************************************************************************
*/
void eptPowerSourceUpdate
(
   BOS_BOOL bOnBatteryPower
);


/*
*****************************************************************************
** FUNCTION:   eptGetInfo
**
** PURPOSE:    Generic information access function
**
** PARAMETERS: uIndex   - index containing both endpoint and cnx ID
**             eInfo    - information type code (see EPINFO)
**             pData    - Pointer to the structure to be filled in with the
**                        requested information
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
EPTSTATUS eptGetInfo
(
   BOS_UINT32     uIndex,
   EPTINFOTYPE    eInfo,
   void          *pData
); 


/*
*****************************************************************************
** FUNCTION:   eptVerifyCountry
**
** PURPOSE:    Look up the eCountry code and verify it is legal
**
** PARAMETERS: eCountry - eCountry code
**
**  RETURNS:   EPTSTATUS
**
**  NOTES:
**
*****************************************************************************
*/
EPTSTATUS eptVerifyCountry
(
   void
);


/*
*****************************************************************************
** FUNCTION:   eptCodecTableInit
**
** PURPOSE:    Initialize the codec table to reflect simultaneous support
**             of different codecs.
**
** PARAMETERS: none
**
** RETURNS:    EPTSTATUS
**
** NOTE:
*****************************************************************************
*/
EPTSTATUS eptCodecTableInit( void );


/*
*****************************************************************************
** FUNCTION:   eptCodecTableRetrieveRow
**
** PURPOSE:    Return a single row from the codec table
**
** PARAMETERS: uIndex       - row index to return (0-indexed)
**             pNumOfCodecs - pointer to the location for storing the
**                            retrieved row
**
** RETURNS:    none
**
*****************************************************************************
*/
void eptCodecTableRetrieveRow
(
   BOS_UINT16           uIndex, 
   EPTCODECTABLE_ROW   *pNumOfCodecs
);


/*
*****************************************************************************
** FUNCTION:   eptConsoleCmd
**
** PURPOSE:    Display endpoint info to console
**
** PARAMETERS: ulineId      - the line index
**             eCmd         - console command to process
**             sParm        - additional parameters.  May contain cnx ID or
**                            an address to additional parameters depending
**                            on the underlying console command.
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
EPTSTATUS eptConsoleCmd
(
   BOS_UINT16     uLineId,
   EPTCONSOLE     eCmd,
   int            sParm
);


/*
*****************************************************************************
** FUNCTION:   eptGetOperStatus
**
** PURPOSE:    Get endpt operation status.  An endpt becomes un-operational
**             if any of the vhds associated with the endpt is pending a reset
**
** PARAMETERS: uLineId - line Id
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
EPTSTATUS eptGetOperStatus
(
   BOS_UINT16     uLineId
);

/*
*****************************************************************************
** FUNCTION:   eptProcessCmd
**
** PURPOSE:    Process a command.  Note that command is currently defined
**             to be any commands that may change a line or DSP
**             characteristic.  To generate a tone, please use 
**             eptProcessSignal.  To create or modify a connection, please
**             use eptXXXXCnx
**
** PARAMETERS: uIndex      - index containing both endpoint and cnx ID
**             eCmd        - command to process
**             sValue      - associated values
**
** RETURNS:    EPTSTATUS
**
*****************************************************************************
*/
EPTSTATUS eptProcessCmd
(
   BOS_UINT32     uIndex,
   EPTCMD         eCmd,
   BOS_SINT32     sValue
);


#if defined(__cplusplus)
}
#endif


#endif  /* EPT_H  */
