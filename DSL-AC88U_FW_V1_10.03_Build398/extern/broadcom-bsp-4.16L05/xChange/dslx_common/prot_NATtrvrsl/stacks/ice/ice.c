/****************************************************************************
*
* Copyright © 2006-2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
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
****************************************************************************
*
*    Description:
*
*      ICE (Interactive Connectivity Establishment) Module per 
*      draft-ietf-mmusic-ice-15
*      
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <bosTypes.h>
#include <bosSocket.h>
#include <stunc.h>
#if NTCFG_TURN_SUPPORT
   #include <turnc.h>
#endif
#include "iceagnt.h"
#include "icesdp.h"
#include "icei.h"
/* ---- Public Variables ------------------------------------- */
ICE_EVTCB iceEvtCB = NULL;      
ICE_LOGCB iceLogCB = NULL;

/* ---- Private Constants and Types -------------------------- */

/* ---- Private Variables ------------------------------------ */
static BOS_BOOL bIceConfigured = BOS_FALSE;
static BOS_BOOL bStartedStunc = BOS_FALSE;
#if NTCFG_TURN_SUPPORT
static BOS_BOOL bStartedTurnc = BOS_FALSE;
#endif
static ICE_LOCAL_IF_LIST localIfList;
/* ---- Private Function Prototypes -------------------------- */
static BOS_BOOL bindingReqHdlr( const STUN_REQ_DATA *pData );
/* ---- Functions -------------------------------------------- */

/*****************************************************************************
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
ICE_STATUS iceStartup( const ICE_CFG *pIceCfg )
{
   BOS_BOOL rc;
   BOS_ASSERT( ( NULL != pIceCfg ) && 
               ( NULL != pIceCfg->pLocalIfList ) &&
               ( pIceCfg->pLocalIfList->num > 0 ) &&
               ( pIceCfg->pLocalIfList->num <= ICE_LOCAL_INTERFACE_MAX ) &&
               ( NULL != pIceCfg->evtCB ) );

   if ( bIceConfigured )
   {
      LOG(( "ICE already configured" ));
      return( ICE_SUCCESS );
   }

   iceEvtCB = pIceCfg->evtCB ;
   iceLogCB = pIceCfg->logCB ;

   if ( NULL != pIceCfg->pStuncCfg )
   {
      if ( STUNC_SUCCESS == stuncStartup( pIceCfg->pStuncCfg ))
      {
         LOG(( "STUNC Ready" ));
         bStartedStunc = BOS_TRUE;
      }
      else
      {
         BOS_ASSERT( BOS_FALSE );
         return( ICE_FAILURE );
      }
   }

#if NTCFG_TURN_SUPPORT
   if ( NULL != pIceCfg->pTurncCfg )
   {
      if ( TURNC_SUCCESS == turncStartup( pIceCfg->pTurncCfg ) )
      {
         LOG(( "TURNC Ready" ));
         bStartedTurnc = BOS_TRUE;
      }
      else
      {
         BOS_ASSERT( BOS_FALSE );
         return( ICE_FAILURE );
      }
   }
#endif

   /* Set STUN Binding Request callback for handling Peer Connectivity
    * Checks 
    */
   rc = stunSetRequestCallback( STUN_MSG_BINDING_REQUEST, bindingReqHdlr );
   BOS_ASSERT( BOS_TRUE == rc );
   /* initialize host interface list */
   memcpy( &localIfList, pIceCfg->pLocalIfList, sizeof(ICE_LOCAL_IF_LIST) );

   if ( !(iceagnt_Init()) )
   {
      BOS_ASSERT( BOS_FALSE );
      return( ICE_FAILURE );
   }

   LOG(( "ICE configuration successful" ));

   bIceConfigured = BOS_TRUE;

   return( ICE_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   iceShutdown
*
*  PURPOSE:    Shutdown ICE module and free all resources.
*
*  PARAMETERS:
*              none
*
*
*  RETURNS:    ICE_SUCESSS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceShutdown( void )
{
   if ( !bIceConfigured )
   {
      LOG(( "iceShutdown: not configured" ));
      return( ICE_FAILURE );
   }

   if ( bStartedStunc )
   {
      (void)stuncShutdown( );
   }

#if NTCFG_TURN_SUPPORT
   if ( bStartedTurnc )
   {
      (void)turncShutdown( );
   }
#endif

   bIceConfigured = BOS_FALSE;
   bStartedStunc = BOS_FALSE;
#if NTCFG_TURN_SUPPORT
   bStartedTurnc = BOS_FALSE;
#endif

   memset( &localIfList, 0, sizeof(ICE_LOCAL_IF_LIST) );

   if ( BOS_TRUE != iceagnt_Deinit() )
   {
      LOG(( "iceShutdown: iceagnt_Deinit failed" ));
   }

   LOG(( "iceShutdown Successful" ));
   
   return( ICE_SUCCESS );
}

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
                           BOS_UINT32                 *pAgentId )
{
   BOS_ASSERT( NULL != pServerList );
   BOS_ASSERT( NULL != pOptions );
   BOS_ASSERT( (ICE_FULL == pOptions->mode) || (ICE_LITE == pOptions->mode) );
   BOS_ASSERT( (ICE_REGULAR == pOptions->nom) || (ICE_AGGRESSIVE == pOptions->nom) );
   BOS_ASSERT( NULL != pAgentId );

   if ( BOS_TRUE != iceagnt_Create( pServerList, pOptions, pAgentId ) )
   {
      return( ICE_FAILURE );
   }

   return( ICE_SUCCESS );
}

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
ICE_STATUS iceDeleteAgent( const BOS_UINT32 agentId )
{
   if ( BOS_TRUE != iceagnt_Delete( agentId ) )
   {
      return( ICE_FAILURE );
   }
   
   return( ICE_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   iceSetPortParms
*
*  PURPOSE:    Set media port parameters.
*
*  PARAMETERS:
*              agentId        [IN] - Agent Identifier
*              pPortparms     [IN] - Media port parameters
*
*  RETURNS:    ICE_SUCESSS if OK. ICE_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
ICE_STATUS iceSetPortParms( const BOS_UINT32          agentId,
                            const ICE_PORT_PARMS      *pPortParms[] )
{
   BOS_ASSERT( ICE_UNKNOWN != agentId );
   BOS_ASSERT( NULL != pPortParms );

   if ( BOS_TRUE != iceagnt_SetPortParms( agentId, pPortParms ) )
   {
      return( ICE_FAILURE );
   }

   return( ICE_SUCCESS );
}

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
ICE_STATUS iceGatherCandidates( const BOS_UINT32            agentId,
                                const BOS_UINT16            maxWait )
{
   BOS_ASSERT( ICE_UNKNOWN != agentId );

   if ( BOS_TRUE != iceagnt_GatherCndts( agentId, maxWait ) )
   {
      return( ICE_FAILURE );
   }

   return( ICE_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   iceProcessLocalSdp
*
*  PURPOSE:    This function generates SDP data into <pSdpParms> for
*              inclusion into the SDP and sets the agent internal state
*              in preparation for connectivity checks.
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
                               ICE_SDP_PARMS            *pSdpParms )
{
   BOS_ASSERT( ICE_UNKNOWN != agentId );
   BOS_ASSERT( (ICE_SDP_OFFERER == sdpAgent) || 
               (ICE_SDP_ANSWERER == sdpAgent) );
   BOS_ASSERT( NULL != pSdpParms );

   memset( pSdpParms, 0, sizeof(ICE_SDP_PARMS) );

   if ( BOS_TRUE != iceagnt_EncodeSdp( agentId, sdpAgent, pSdpParms ) )
   {
      return( ICE_FAILURE );
   }

   return( ICE_SUCCESS );
}

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
                                const ICE_SDP_PARMS      *pSdpParms )
{

   BOS_ASSERT( ICE_UNKNOWN != agentId );
   BOS_ASSERT( (ICE_SDP_OFFERER == sdpAgent) || 
               (ICE_SDP_ANSWERER == sdpAgent) );
   BOS_ASSERT( NULL != pSdpParms );

   if ( BOS_TRUE != iceagnt_ParseSdp( agentId, sdpAgent, pSdpParms ) )
   {
      return( ICE_FAILURE );
   }

   return( ICE_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   ice_GetLocalIfFromId
*
*  PURPOSE:    Return host interface from the host interface id.
*
*  PARAMETERS:
*              hIfId [IN] - host interface id
*
*  RETURNS:    pointer to host interface structure. NULL if not found.
*
*  NOTES:
*
*****************************************************************************/
ICE_LOCAL_IF *ice_GetLocalIfFromId( const BOS_UINT16 localIfId )
{
   int i;
   ICE_LOCAL_IF *pLIf = NULL;
   for ( i = 0; i < ICE_LOCAL_INTERFACE_MAX && i < localIfList.num; i++ )
   {
      if ( localIfId == localIfList.localIf[i].id )
      {
         pLIf = &localIfList.localIf[i];
         break;
      }
   }
   return( pLIf );
}

/*****************************************************************************
*
*  FUNCTION:   bindingReqHdlr
*
*  PURPOSE:    Peer STUN Binding Request Handler.
*
*  PARAMETERS:
*              request [IN] - request
*              pMsg    [IN] - parsed STUN message
*
*  RETURNS:    BOS_FALSE if error.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL bindingReqHdlr( const STUN_REQ_DATA *pData )
{
   BOS_BOOL rc;
   BOS_UINT16 msgType;

   BOS_ASSERT( NULL != pData );

   msgType = stunGetMsgType( pData->pMsg );
   BOS_ASSERT( STUN_MSG_BINDING_REQUEST == msgType );

   rc = iceagnt_Notify( ICEAGNT_CTTYCHECK_REQUEST,
                        (BOS_UINT32)msgType,
                        0,
                        (void *)pData );
   return( rc );
}

