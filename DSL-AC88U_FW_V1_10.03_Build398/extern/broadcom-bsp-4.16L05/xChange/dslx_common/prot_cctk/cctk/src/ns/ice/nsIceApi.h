/**********************************************************************************
** Copyright (c) 2012 Broadcom
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
**      This file defines the ICE control component module implementation.
**
***********************************************************************************/

#ifndef __ICE_CTL__H__INCLUDED__
#define __ICE_CTL__H__INCLUDED__

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Include Files ----------------------------------------------------------- */
#include <ice.h>
/* ---- Public Variables -------------------------------------------------------- */

/* ---- Public Constants and Types --------------------------------------------- */
MX_NAMESPACE_START( MXD_GNS )
class MseSdpSession;
MX_NAMESPACE_END( MXD_GNS )

enum MSEMEDIATYPE
{
   eMSEMEDIATYPE_AUDIO,
   eMSEMEDIATYPE_VIDEO,
   eMSEMEDIATYPE_T38,
};
enum MSEICEMODE
{
   eMSEICEMODE_UNKNOWN,
   eMSEICEMODE_NONE,
   eMSEICEMODE_LITE,
   eMSEICEMODE_FULL,
};

enum MSEICESTATUS
{
   eMSEICESTATUS_CONCLUDED,        /* done */
   eMSEICESTATUS_IN_PROGRESS,      /* waiting for stun check */
   eMSEICESTATUS_FAIL,
};

/* This is the ICE agent handle */
typedef void* MSEICEAGENTHANDLE;

typedef struct MSEICEMEDIATRANSPORT
{
   ICE_MEDIA_TRANSPORT *pMediaTransport;
   class MX_NS MseSdpSession *pMse;
   MSEMEDIATYPE eMediaType;
   BOS_BOOL bEarlySession;
} MSEICEMEDIATRANSPORT;

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   nsIceInit
**
**  PURPOSE:    ICE Control module initialization.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    None
**
**  NOTES:      This function initializes the ICE control module.
***********************************************************************************/
void nsIceInit( void );

/***********************************************************************************
**  FUNCTION:   nsIceDeInit
**
**  PURPOSE:    ICE Control module deinitialization.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    None
**
**  NOTES:      This function deinitializes the ICE control module.
***********************************************************************************/
void nsIceDeInit( void );

/**********************************************************************************
**  FUNCTION:   nsIceStart
**
**  PURPOSE:    Get the configuration from the configuration and start the internal
**              ICE stack.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    None
**
**  NOTES:      It will stop and restart the internal ICE stack if the ICE stack
**              is currently running.
***********************************************************************************/
void nsIceRun( void );

/**********************************************************************************
**  FUNCTION:   nsIceMseAddAgent
**
**  PURPOSE:    Create and add a new agent for the given stream of a MSE.
**
**  PARAMETERS:
**              pMse - [IN] MSE session.
**              uMediaStreamIndex - [IN] stream index. (unique)
**              bEarlySession - [IN] flag indicates if this is early media.
**              eMediaType - [IN] media stream type.
**
**  RETURNS:    ICE agent handle
**
**  NOTES:      If an agent for the given stream for this MSE exists, the exist
**              agent is returned.
***********************************************************************************/
MSEICEAGENTHANDLE nsIceMseAddAgent( MX_NS MseSdpSession* pMse, 
   BOS_UINT32 uMediaStreamIndex,
   BOS_BOOL bEarlySession,
   MSEMEDIATYPE eMediaType
);

/**********************************************************************************
**  FUNCTION:   nsIceMseFreeAgents
**
**  PURPOSE:    Free all the ICE agents associated with the given MSE.
**
**  PARAMETERS:
**              pMse - MSE session.
**
**  RETURNS:    None
**
**  NOTES:      
***********************************************************************************/
void nsIceMseFreeAgents( MX_NS MseSdpSession* pMse );

/**********************************************************************************
**  FUNCTION:   nsIceEncodeLocalIceParams
**
**  PURPOSE:    ICE agent gathers local information and set all the local 
**              parameters into the SDP media stream
**
**  PARAMETERS:
**              bIsCaller - [IN] flag indicates if ICE agent's MSE is the caller.
**              rStream - [IN/OUT] local SDP media stream.
**              iceAgentHdl - [IN] ICE agent to perform the action.
**              rIceUfrag - [OUT] ICE agent's Ufrag for STUN
**              rIcePwd - [OUT] ICE agent's Password for STUN
**
**  RETURNS:    None
**
**  NOTES:      If both local and remote information are available, ICE
**              connectivity check is initiated.
***********************************************************************************/
void nsIceEncodeLocalIceParams( BOS_BOOL bIsCaller,
   MX_NS CSdpLevelMedia &rStream, 
   MSEICEAGENTHANDLE iceAgentHdl, 
   MX_NS CString &rIceUfrag, 
   MX_NS CString &rIcePwd );
   
/**********************************************************************************
**  FUNCTION:   nsIceDecodeRemoteIceParams
**
**  PURPOSE:    ICE agent gathers remote information from the given SDP media stream.
**
**  PARAMETERS:
**              bIsCaller - [IN] flag indicates if ICE agent's MSE is the caller.
**              rStream - [IN] remote SDP media stream.
**              iceAgentHdl - [IN] ICE agent to perform the action.
**
**  RETURNS:    Peer's ICE mode according to the SDP media stream.
**
**  NOTES:      If both local and remote information are available, ICE
**              connectivity check is initiated.
***********************************************************************************/
MSEICEMODE nsIceDecodeRemoteIceParams( BOS_BOOL bIsCaller,
   MX_NS CSdpLevelMedia &rStream,
   MSEICEAGENTHANDLE iceAgentHdl);

/**********************************************************************************
**  FUNCTION:   nsIceGetResult
**
**  PURPOSE:    Ask ICE agent if the ICE process is completed.
**
**  PARAMETERS:
**              iceAgentHdl - [IN] ICE agent to perform the action.
**              pLclDftCnxAddr - [OUT] Connection address to be used in local SDP.
**
**  RETURNS:    
**              eMSEICESTATUS_CONCLUDED if ICE process is completed.
**              eMSEICESTATUS_IN_PROGRESS if ICE process is in progress (waiting for STUN check ).
**              eMSEICESTATUS_FAIL if ICE concluded peer is unreachable.
**
***********************************************************************************/
MSEICESTATUS nsIceGetResult( MSEICEAGENTHANDLE iceAgentHdl, 
   BOS_IP_ADDRESS *pLclDftCnxAddr );
   
/* debug function */   
void dumpMseIceAgent( MSEICEAGENTHANDLE iceAgentHdl );

#ifdef __cplusplus
}
#endif

#endif /* __ICE_CTL__H__INCLUDED__ */
