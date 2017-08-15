/****************************************************************************
*
* Copyright © 2006-2008 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    ice.h
*
*  @brief   ICE (Interactive Connectivity Establishment) header file
*
*  This file contains software interface information for the ICE Module.
*/
/****************************************************************************/

#ifndef ICE_H    /* support nested includes */
#define ICE_H

/* ---- Include Files ---------------------------------------- */
#include <ntcfg.h>
#include <bosTypes.h>
#include <bosIpAddr.h>
#include <stunc.h>
#include <stun.h>
#include <bosIpAddr.h>

#if NTCFG_TURN_SUPPORT
   #include <turnc.h>
#endif

/* ---- Constants and Types ---------------------------------- */
#define ICE_VERSION "BRCM ICE 1.0"

#define ICE_UNKNOWN                       (BOS_UINT32)(-1)

/* Maximum number of agents allowed */
#define ICE_AGENT_NUM_MAX                 10

/* Maximum number of network interfaces */
#define ICE_LOCAL_INTERFACE_MAX           1

/* Maximum number of STUN servers */
#define ICE_STUN_SERVER_MAX               1

/* Maximum number of characters in an a-line */
#define ICE_CNDT_ALINE_LEN_MAX            127

/* Total number of a-lines for RTP and RTCP */
#define ICE_CNDT_ALINE_NUM_MAX            6  

#define ICE_MODE_LEN_MAX                  15

/*
 * Gramma for short term credential
 *    ufrag    = 4*ice-char
 *    password = 22*ice-char
 */
#define ICE_SDP_UFRAG_LEN_MAX             4
#define ICE_SDP_PWD_LEN_MAX               22 

/* STUN Client status */
typedef enum
{
   ICE_SUCCESS = 0,
   ICE_FAILURE
} ICE_STATUS;

/* ICE Events */
typedef enum
{
   ICE_EVT_CANDIDATES_GATHER_COMPLETE,    /* All candidates gathered. 
                                             pData = NULL */

   ICE_EVT_CANDIDATES_GATHER_TIMEOUT,     /* Some of the candidates gathered,
                                             pData = NULL */

   ICE_EVT_CONCLUDED_SUCCESS,             /* Transport addresses obtained
                                             for the media line,
                                             pData = ICE_MEDIA_TRANSPORT */

   ICE_EVT_CONCLUDED_FAILURE,             /* No Transport addresses obtained
                                             for the media line,
                                             pData = NULL */
   ICE_EVT_NUM
} ICE_EVT;

/* ICE Agent mode of operation */
typedef enum
{
   ICE_FULL,
   ICE_LITE
} ICE_MODE;

/* Transport type */
typedef enum
{
   ICE_TRANS_UDP,
   ICE_TRANS_TCP
} ICE_TRANS_TYPE;

/* Media Component */
#define ICE_COMP_NUM_MAX                        2
typedef enum
{
   ICE_COMP_MIN = 1,
   ICE_COMP_RTP = 1,
   ICE_COMP_RTCP = 2,
} ICE_COMP_ID;

/* Type and Local Preference priority settings */
#define ICE_TYPEPREF_HOST_DEFAULT               126
#define ICE_TYPEPREF_SERVER_REFLEXIVE_DEFAULT   100
#define ICE_TYPEPREF_PEER_REFLEXIVE_DEFAULT     110
#define ICE_TYPEPREF_RELAYED_DEFAULT            0
#define ICE_LOCALPREF_DEFAULT                   65535
#define ICE_TYPEPREF_MAX                        126

typedef struct
{
   BOS_UINT8   host;       /* host candidate */
   BOS_UINT8   srflx;      /* server reflexive */
   BOS_UINT8   prflx;      /* peer reflexive */
   BOS_UINT8   relayed;    /* relayed */
} ICE_TYPEPREF;

typedef struct
{
   ICE_TYPEPREF   type;    /* type preference */
   BOS_UINT16     local;   /* local preference */
} ICE_PREF;

/* STUN server type */
typedef enum
{
   ICE_SRV_STUN,
   ICE_SRV_TURN
} ICE_SRV_TYPE;

/* Local Interface */
typedef struct
{
   BOS_UINT16           id;            /* Interface Identifier */
   BOS_IP_ADDRESS       address;       /* Interface IP address */
} ICE_LOCAL_IF;

/* Local interface list */
typedef struct
{
   BOS_UINT16           num;
   ICE_LOCAL_IF         localIf[ICE_LOCAL_INTERFACE_MAX];
} ICE_LOCAL_IF_LIST;

/* STUN server */
typedef struct
{
   BOS_UINT16        id;            /* STUN Server id */
   ICE_SRV_TYPE      type;          /* STUN server type */
   STUN_TRANSPORT    socAddr;       /* Server address and port */
} ICE_STUN_SERVER;

/* STUN server list */
typedef struct
{
   BOS_UINT16           num;
   ICE_STUN_SERVER      server[ICE_STUN_SERVER_MAX];
} ICE_STUN_SERVER_LIST;

/* Parameters used in gathering Candidates */
typedef struct
{
   ICE_COMP_ID          component;     /* Component type */
   BOS_UINT16           localIfId;     /* Local interface id */
   BOS_UINT16           stunServerId;  /* STUN server id */
   ICE_TRANS_TYPE       transType;     /* Transport type */
   BOS_UINT32           sid;           /* Socket id */
   BOS_UINT16           port;          /* Port number */
   STUNC_SHARED_SECRET  *pSecret;      /* STUN Shared secret,
                                        *  NULL if not used
                                        */
} ICE_PORT_PARMS;

/* Credential parameters used in SDP */
typedef struct
{
   char                 ufrag[ICE_SDP_UFRAG_LEN_MAX+1];
   char                 pwd[ICE_SDP_PWD_LEN_MAX+1];
} ICE_CREDENTIAL;

/* SDP Parameters */
typedef struct
{
   char                 iceMode[ICE_MODE_LEN_MAX+1];
   STUN_TRANSPORT       addr;             /* RTP transport */
   BOS_UINT16           rtcpPort;         /* 0 if not used */
   ICE_CREDENTIAL       credential;
   char                 aline[ICE_CNDT_ALINE_NUM_MAX][ICE_CNDT_ALINE_LEN_MAX+1];
} ICE_SDP_PARMS;

/* SDP role */
typedef enum
{
   ICE_SDP_OFFERER,
   ICE_SDP_ANSWERER,
} ICE_SDP_AGENT;

/* ICE Agent candidate nomination type */
typedef enum
{
   ICE_REGULAR,
   ICE_AGGRESSIVE
} ICE_NOM;

/* ICE Agent options */
typedef struct
{
   ICE_MODE       mode;             /* ICE Agent mode */
   ICE_NOM        nom;              /* ICE Agent nomination */
   ICE_PREF       *pPrefOverride;   /* Priority preference override */
} ICE_AGENT_OPTIONS;

/* Candidate pair */
typedef struct
{
   ICE_COMP_ID       compId;        /* Component type */
   BOS_UINT32        sid;           /* Socket id */
   ICE_TRANS_TYPE    transType;     /* Transport type */
   STUN_TRANSPORT    local;         /* Local transport */
   STUN_TRANSPORT    remote;        /* Remote transport */
   char              lCndtAline[ICE_CNDT_ALINE_LEN_MAX+1]; /* Local Candidate 
                                                            * a-line 
                                                            */
   char              rCndtAline[ICE_CNDT_ALINE_LEN_MAX+1]; /* Remote Candidate 
                                                            * a-line 
                                                            */
} ICE_CNDT_PAIR;

/* Media transport data block returned 
 * in ICE_EVT_CONCLUDED_SUCCESS 
 */
typedef struct
{
   BOS_UINT8         num;
   ICE_CNDT_PAIR     cndtPair[ICE_COMP_NUM_MAX];
} ICE_MEDIA_TRANSPORT;

/*****************************************************************************
*
*  FUNCTION:   ICE_EVTCB - ICE event Callback
*
*  PURPOSE:    A callback function for receiving ICE events.
*
*  PARAMETERS:
*              evt     [IN] - evt type
*              pData   [IN] - event data
*              reqHdl  [IN] - request handle of the original request
*
*  RETURNS:    ICE_STATUS
*
*  NOTES:
*
*****************************************************************************/
typedef void (*ICE_EVTCB)( const ICE_EVT        evt,
                           const void           *pData,
                           const BOS_UINT32     agentId );

/*****************************************************************************
*
*  FUNCTION:   ICE_LOGCB - log handler
*
*  PURPOSE:    A callback function for displaying log data.
*              If not set, printf() is used.
*
*  PARAMETERS:
*              format [IN] - format string
*              ...    [IN] - data

*  RETURNS:    
*           none
*
*  NOTES:

*****************************************************************************/
typedef void (*ICE_LOGCB)( const char *format, ... );

/* ICE configuration */
typedef struct
{
   ICE_LOCAL_IF_LIST    *pLocalIfList; /* Local interface list */
   ICE_EVTCB            evtCB;         /* event callback */
   ICE_LOGCB            logCB;         /* LOG callback */
   STUNC_CFG            *pStuncCfg;    /* STUNC configuration */
#if NTCFG_TURN_SUPPORT
   TURNC_CFG            *pTurncCfg;    /* TURNC configuration */
#endif
} ICE_CFG;

/* ---- Variable Externs ------------------------------------- */
/* ---- Function Prototypes ---------------------------------- */

/*
** Exports
*/
#if defined(__cplusplus)
extern "C"
{
#endif

/*****************************************************************************
*
*  FUNCTION:   iceStartup
*
*  PURPOSE:    Configure and startup ICE Client software module
*
*  PARAMETERS:
*          pIceCfg - pointer to configuration parameters
*
*  RETURNS:    ICE_SUCCESS if configuration successful. ICE_FAILURE
*              otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceStartup( const ICE_CFG *pIceCfg );

/*****************************************************************************
*
*  FUNCTION:   iceShutdown
*
*  PURPOSE:    Shutdown ICE software module and free resources.
*
*  PARAMETERS:
*              none.
*
*
*  RETURNS:    ICE_SUCESSS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceShutdown( void );

/*****************************************************************************
*
*  FUNCTION:   iceCreateAgent
*
*  PURPOSE:    Start an ICE agent to manage candidate gathering, 
*              SDP processing and connectivity checks.
*
*  PARAMETERS:
*              pServerList    [IN]  - STUN Server List
*              pOptions       [IN]  - Agent options
*              agentId        [OUT] - Agent Identifier
*
*  RETURNS:    ICE_SUCESSS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceCreateAgent( const ICE_STUN_SERVER_LIST *pServerList,
                           const ICE_AGENT_OPTIONS    *pOptions,
                           BOS_UINT32                 *agentId );

/*****************************************************************************
*
*  FUNCTION:   iceDeleteAgent
*
*  PURPOSE:    Delete a previously created agent and free associated 
*              resources. 
*
*  PARAMETERS:
*              agentId [IN] - Agent Identifier
*
*  RETURNS:    ICE_SUCESSS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceDeleteAgent( const BOS_UINT32 agentId );

/*****************************************************************************
*
*  FUNCTION:   iceSetPortParms
*
*  PURPOSE:    Set media port parameters.
*
*  PARAMETERS:
*              agentId        [IN] - Agent Identifier
*              pPortparms     [IN] - Media port parameters list,
*                                    NULL terminated
*
*  RETURNS:    ICE_SUCESSS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceSetPortParms( const BOS_UINT32          agentId,
                            const ICE_PORT_PARMS      *pPortParms[] );

/*****************************************************************************
*
*  FUNCTION:   iceGatherCandidates
*
*  PURPOSE:    Gather candidates for agent <agentId>,
*              and prioritize them per setting of the agent. 
*
*              Depending on the mode and server configuration, 
*              three possible types of candidates can be obtained:
*              i) HOST-CANDIDATE,
*              ii) SERVER-REFLEXIVE
*              iii) RELAYED
*
*              For full mode operation, if only STUN server is configured,
*              reflexive candidates will be obtained by sending 
*              BINDING request to the server. If a TURN server is
*              configured, both the reflexive candidate and the
*              relayed candidate will be obtained by
*              sending the ALLOCATE request to the server.
*
*              For ice-lite mode operation, only the HOST-CANDIDATE 
*              is obtained.
*              
*              This function can be called to populate the candidate
*              list prior to session initiation (e.g. OFFER/ANSWER)
*              to help speed up the ICE process.
*
*              This function may be called by iceSdpOffer() to gather 
*              candidates for including into the SDP offer. 
*              iceSdpOffer() can bypass this step if it's already been
*              initiated beforehand. See iceSdpOffer for details.
*              
*  PARAMETERS:
*              pTypePref      [IN] - Type preference, NULL if use default
*              maxWait        [IN] - candidate gathering time allowance in ms
*
*  RETURNS:    ICE_SUCCESS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceGatherCandidates( const BOS_UINT32         agentId, 
                                const BOS_UINT16         maxWait );

/*****************************************************************************
*
*  FUNCTION:   iceProcessLocalSdp
*
*  PURPOSE:    This function generates SDP data into <pSdpParms> for
*              inclusion into the SDP.
*
*              If it's a full mode agent and the offerer, this 
*              function readies the connectivity check enginee to 
*              respond to connectivity checks, if any, from its peer. 
*
*              If it's a full mode agent and the answerer, this
*              function initiates connectivity checks when called.
*
*              If the agent is a lite-mode agent, it will only respond
*              to connectivity check, but not initiate any.
*              
*  PARAMETERS:
*              agentId     [IN] - agent Id
*              sdpAgent    [IN] - SDP offerer/answerer
*              pSdpParms   [OUT]- SDP data
*
*  RETURNS:    ICE_SUCCESS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceProcessLocalSdp( const BOS_UINT32         agentId,
                               const ICE_SDP_AGENT      sdpAgent,
                               ICE_SDP_PARMS            *pSdpParms );

/*****************************************************************************
*
*  FUNCTION:   iceProcessRemoteSdp
*
*  PURPOSE:    This function parses SDP data contained in <pSdpParms> and
*              generate candidates list for connectivity checks.
*
*              If it's a full mode agent and the offerer, this 
*              function initiates connectivity check when called.
*
*              If it's a full mode agent and the answerer, this
*              function readies the connectivity check engine to respond
*              to connectivity checks.
*
*              If the agent is a lite-mode agent, it will only respond
*              to connectivity check, but not initiate any.
*              
*  PARAMETERS:
*              agentId     [IN] - agent Id
*              sdpAgent    [IN] - SDP offerer/answerer
*              pSdpParms   [IN] - SDP data
*
*  RETURNS:    ICE_SUCCESS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceProcessRemoteSdp( const BOS_UINT32         agentId,
                                const ICE_SDP_AGENT      sdpAgent,
                                const ICE_SDP_PARMS      *pSdpParms );
#if defined(__cplusplus)
}
#endif
#endif
