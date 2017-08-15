/**********************************************************************************
** Copyright (c) 2012 Broadcom Corporation
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
**      This file implements the socket control core module.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include "cctkInc.h"
#include <cfgApi.h>
#include <ice.h>

#if CCTKCFG_ICE_LITE_SUPPORT

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define ICE_LITE_STR                   "ice-lite"
#define IP_ADDR_LEN_MAX                40
#define TYP_LEN_MAX                    5
#define TRANSPORT_TYPE_LEN_MAX         3
#define ICECNDT_FOUNDATION_LEN_MAX     31

typedef enum {
   eMSEICEAGENTSTATE_INITIAL,
   eMSEICEAGENTSTATE_IN_PROGRESS,
   eMSEICEAGENTSTATE_DONE,
} MSEICEAGENTSTATE;

/* internal type, keep all parameters for an ICE agent */
typedef struct MSEICEAGENT
{
   BOS_UINT32 agentId;
   BOS_UINT32 index;                /* media stream index in SDP */
   class MX_NS MseSdpSession *pMse; /* Mse pointer */
   MSEMEDIATYPE eMediaType;
   BOS_BOOL bEarlySession;          /* indicate this agent is for early session */
   MX_NS CVector <void*> vecSocketId;
   BOS_BOOL bPortSet;               /* flag, so port is added once only */
   BOS_UINT32 rtpPort;              /* RTP port of the media stream */
   BOS_UINT32 rtcpPort;             /* RTCP port of the media stream */
   char lclCandidates[ICE_CNDT_ALINE_NUM_MAX][ICE_CNDT_ALINE_LEN_MAX+1];
   char rmtCandidates[ICE_CNDT_ALINE_LEN_MAX+1];
   
   MSEICEMODE lclIceMode;
   MSEICEMODE rmtIceMode;
   BOS_IP_ADDRESS   lclDftCnxAddr;       /* Session default connect address */
   BOS_BOOL bRmtUnreachable;
   MSEICEAGENTSTATE state;
} MSEICEAGENT;

/* ---- Private Variables ------------------------------------------------------- */
static MX_NS CVector <MSEICEAGENT*> *gp_vecIceAgents = NULL;
static ICE_LOCAL_IF_LIST gIceIfList;
static BOS_BOOL gbIceInitialized = BOS_FALSE;

static ICE_LOGCB gIceLogCb = NULL;      /* ICE Log callback function */
#define ICELOG(...) if (gIceLogCb) gIceLogCb(__VA_ARGS__);

/* ---- Private Function Prototypes --------------------------------------------- */
void nsIceEventCB( const ICE_EVT        evt,
   const void           *pData,
   const BOS_UINT32     agentId );
/* ---- Functions --------------------------------------------------------------- */
/***********************************************************************************
**  FUNCTION:   nsIceInit
**
**  PURPOSE:    ICE Control module initialization.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    None
**
**  NOTES:      This function initializes the ICE control module.
***********************************************************************************/
void nsIceInit( void )
{
   gp_vecIceAgents = BRCM_NEW( MX_NS CVector <MSEICEAGENT*> );
   gbIceInitialized = BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   nsIceDeInit
**
**  PURPOSE:    ICE Control module deinitialization.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    None
**
**  NOTES:      This function deinitializes the ICE control module.
***********************************************************************************/
void nsIceDeInit( void )
{
   if ( gbIceInitialized )
   {
      iceShutdown();
      gbIceInitialized == BOS_FALSE;
   }
   BRCM_DELETE( gp_vecIceAgents );
}


/***********************************************************************************
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
void nsIceRun( void )
{
   ICE_CFG iceCfg;
   STUNC_CFG stuncCfg;
   STUN_CFG stunCfg;
   int i;
   
   gIceLogCb = CFG_SYS->xtl.net.prot.gen.ice.pIceLogFunc;
    
   /* Fill interface list from System config */
   gIceIfList.num = CFG_SYS->xtl.net.prot.gen.iceAddrLst.uNum;
   ICELOG( "gIceIfList num=%d", gIceIfList.num );
   for ( i = 0; i < gIceIfList.num && i < ICE_LOCAL_INTERFACE_MAX; i++ )
   {
      char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
      bosIpAddrGetStr(&(CFG_SYS->xtl.net.prot.gen.iceAddrLst.addr[i]), ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);   
      ICELOG( "gIceIfList (%d) (%s)", i, ipAddrStr ); 

      gIceIfList.localIf[i].id = i;
      bosIpAddrCreateCopy( &(CFG_SYS->xtl.net.prot.gen.iceAddrLst.addr[i]), 
         &(gIceIfList.localIf[i].address) );
   }

   /* local host interface list is updated */
   if ( gbIceInitialized )
   {  
      iceShutdown();
      gbIceInitialized = BOS_FALSE;
   }
   


   
   if ( gIceIfList.num > 0 )
   {
      memset( &iceCfg, 0, sizeof(ICE_CFG) );
      iceCfg.pLocalIfList = &gIceIfList;
      iceCfg.evtCB = &nsIceEventCB;
      iceCfg.logCB = CFG_SYS->xtl.net.prot.gen.ice.pIceLogFunc; /* just use printf for now */
      iceCfg.sendPktCB = CFG_SYS->xtl.net.prot.gen.ice.pIceStunSendFunc;
      stuncCfg.pStunCfg = &stunCfg;
      iceCfg.pStuncCfg = &stuncCfg;  
      
      /* Init STUN module */
      stunCfg.sendPktCB = CFG_SYS->xtl.net.prot.gen.ice.pIceStunSendFunc;
      stunCfg.rand32CB = NULL; /* use the internal rand() function */
      stunCfg.hmacCB = CFG_SYS->xtl.net.prot.gen.ice.pIceHmacFunc;
      stunCfg.crc32CB = CFG_SYS->xtl.net.prot.gen.ice.pIceCrc32Func;
      stunCfg.logCB = CFG_SYS->xtl.net.prot.gen.ice.pIceLogFunc;
            
#if NTCFG_TURN_SUPPORT
#   error CCTK is not supporting TURN for its ICE implementation
      iceCfg.pTurncCfg = NULL; /* Not supporting TURNC */
#endif
      /* start ICE module */
      iceStartup( &iceCfg );
      gbIceInitialized = BOS_TRUE;
      
      gIceLogCb = iceCfg.logCB;
   }
}

/**********************************************************************************
**  FUNCTION:   nsIceMseAddAgent
**
**  PURPOSE:    Create and add a new agent for the given stream of a MSE.
**
**  PARAMETERS:
**              pMse - [IN] MSE session.
**              uMediaStreamIndex - [IN] stream index. (unique)
**              bEarlySession - [IN] flag indicate if this is for early media.
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
   MSEMEDIATYPE eMediaType )
{
   ICE_STATUS iceRes;
   BOS_UINT32 agentId;
   ICE_STUN_SERVER_LIST stunServerList;
   ICE_AGENT_OPTIONS agtOpt;
   ICE_PREF agntPriPref;
   ICE_PREF agntDftCndtPref;
   MSEICEAGENT* pAgent;
   
   /* search if the agent exist */
   int i = 0;
   while (i < gp_vecIceAgents->GetSize())
   {
      pAgent = gp_vecIceAgents->GetAt(i);
      if ( pAgent && pAgent->pMse == pMse && pAgent->index == uMediaStreamIndex 
           && pAgent->eMediaType == eMediaType )
      {
         ICELOG( "iceCreateAgent -- existing agnt pAgntHdl[%x] agntId=%d, "
            "streamIndex=%d type=%d",
            (int) pAgent, pAgent->agentId, uMediaStreamIndex, eMediaType );      
         return ( MSEICEAGENTHANDLE ) pAgent;
      }
      i++;
   }
   
   /* not found */
   
   /* empty STUN Server List as this is ICE-LITE */
   memset( &stunServerList, 0, sizeof (stunServerList) );
   
   agtOpt.mode = ICE_LITE;
   agtOpt.nom = ICE_REGULAR;
   
   /* Since this is ICE-lite, for simply precedent sorting, set all to 0 */
   agntPriPref.type.host = 0;
   agntPriPref.type.srflx = 0;
   agntPriPref.type.prflx = 0;
   agntPriPref.type.relayed = 0;
   
   /* Set different Precedent value for IPv4 and IPv6 */
   if ( CFG_SYS->xtl.net.prot.gen.ice.bIPv6PrefMedia )
   {
      agntPriPref.local.IPv4 = 0;
      agntPriPref.local.IPv6 = 1;
   }
   else
   {
      agntPriPref.local.IPv4 = 1;
      agntPriPref.local.IPv6 = 0;   
   }
   agtOpt.pPrefPriorityOverride = &agntPriPref; 

   agtOpt.pPrefDftCndtOverride = NULL;

   /* create an ICE-LITE agent */
   iceRes = iceCreateAgent( &stunServerList, &agtOpt, &agentId );
   if ( iceRes != ICE_SUCCESS )
   {
      ICELOG( "iceCreateAgent Failed" );
      return ( NULL );
   }
   
   pAgent = BRCM_NEW( MSEICEAGENT );
   pAgent->index = uMediaStreamIndex;
   pAgent->agentId = agentId;
   pAgent->pMse = pMse;
   pAgent->bEarlySession = bEarlySession;
   pAgent->eMediaType = eMediaType;
   pAgent->bPortSet = BOS_FALSE;
   memset( pAgent->lclCandidates, 0, sizeof (pAgent->lclCandidates) );
   memset( pAgent->rmtCandidates, 0, sizeof (pAgent->rmtCandidates) );
   
   pAgent->lclIceMode = eMSEICEMODE_LITE;  /* always ICE-lite */
   pAgent->rmtIceMode = eMSEICEMODE_UNKNOWN;
   bosIpAddrCreateZero( BOS_IPADDRESS_TYPE_V4, &pAgent->lclDftCnxAddr );
   pAgent->bRmtUnreachable = BOS_FALSE;
   pAgent->state = eMSEICEAGENTSTATE_INITIAL;   

   gp_vecIceAgents->Append( pAgent );
   ICELOG( "iceCreateAgent -- newly created pAgntHdl[%x] agntId=%d, streamIndex=%d type=%d",
      (int) pAgent, pAgent->agentId, uMediaStreamIndex, eMediaType );    
   return ( MSEICEAGENTHANDLE ) pAgent;
}

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
void nsIceMseFreeAgents( MX_NS MseSdpSession* pMse )
{
   int i;
   int j;
   for ( i = 0; i < gp_vecIceAgents->GetSize(); i++ )
   {
      MSEICEAGENT *pAgent = gp_vecIceAgents->GetAt(i);
      if ( pAgent->pMse == pMse )
      {
         for ( j = 0; j < pAgent->vecSocketId.GetSize(); j++ )
         {
            ICELOG( "IceCloseCnx -- remove pHdl[%x] index=%d", (int) pAgent->vecSocketId[j], j );      
            CFG_SYS->xtl.net.prot.gen.ice.pIceCloseCnxFunc( (void*)pAgent->vecSocketId[j] );
         }
         iceDeleteAgent(pAgent->agentId);
         BRCM_DELETE ( pAgent );
         gp_vecIceAgents->Erase(i);

         /* Because the element in the current index is deleted, the next element
            have the current index. Decrement the index by one, so the next element
            is fetched on the next loop. */
         i--;
      }
   }
}

/**********************************************************************************
**  FUNCTION:   nsIceEncodeLocalIceParams
**
**  PURPOSE:    ICE agent gathers local information and set all the local 
**              parameters into the SDP media stream
**
**  PARAMETERS:
**              bIsCaller - [IN] flag indicates if ICE agent's MSE is the caller.
**              rStream - [IN/OUT] SDP media stream.
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
   MX_NS CString &rIcePwd )
{      
   MX_NS CVector<MX_NS CSdpFieldAttributeIceCandidate> candidateList;
   MX_NS CSdpFieldAttributeIceCandidate candidate;
   MX_NS CVector<MX_NS CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates*> rmtCandidateList;
   MX_NS CSdpFieldAttributeIceRemoteCandidates rmtCandidate;
   int j = 0;
   int i;
   MX_NS CSdpParser::EAddressType eIPType;
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
   MSEICEAGENT *pAgent = (MSEICEAGENT *) iceAgentHdl;
   BOS_BOOL bRestart;
   BOS_BOOL bHasRtcp;

   ICELOG( "nsIceEncodeLocalIceParams -- agnt pAgntHdl[%x] agntId=%d",
      (int) pAgent, pAgent->agentId );  
   
   if ( pAgent->bPortSet == BOS_FALSE )
   {
      /* Create ICE Candidates */
      
      int32_t rtpport = rStream.GetMediaAnnouncement().GetTransportPort();
      int32_t rtcpport = (-1);
      MX_NS CSdpParser::ETransportProtocol protocol = 
         rStream.GetMediaAnnouncement().GetTransportProtocolId();
       
      bHasRtcp = BOS_TRUE;
      /* Check if RTP is deactivated */
      if ( !( (protocol == MX_NS CSdpParser::eRTPAVP) ||
              (protocol == MX_NS CSdpParser::eRTPSAVP) 
            ) ||
           rStream.IsRtcpDeactivated() )
      {
         bHasRtcp = BOS_FALSE;
      }

      if ( bHasRtcp )
      {
         rtcpport = rStream.GetSdpFieldAttributeRtcp().GetPort();
         if ( rtcpport == (-1) )
         {
            rtcpport = rtpport + 1;
         }
      }

      pAgent->rtpPort = rtpport;
      pAgent->rtcpPort = rtcpport;

      ICE_PORT_PARMS portParms[10];
      ICE_PORT_PARMS *pPortParms[10];
      memset( portParms, 0, sizeof(portParms) );

      void* pHdl = NULL;
      BOS_STATUS bosStatus;   
      /* Setup the RTP Port */
      for ( i = 0; i < gIceIfList.num; i++ )
      {
         /* Create ICE listening socket */
         bosStatus = CFG_SYS->xtl.net.prot.gen.ice.pIceOpenCnxFunc( &gIceIfList.localIf[i].address,
                                 rtpport,
                                 &pHdl );
         ICELOG( "IceOpenCnx -- Add RTP pHdl[%x] index=%d", (int) pHdl, pAgent->vecSocketId.GetSize() );
         pAgent->vecSocketId.Append( (void*) pHdl );
                                 
         portParms[j].component = (ICE_COMP_ID)MX_NS gs_uRTP_COMPONENT_ID;
         portParms[j].localIfId = gIceIfList.localIf[i].id;
         portParms[j].stunServerId = 0; /* not used since this is ICE lite*/
         portParms[j].transType = ICE_TRANS_UDP; /* always UDP */
         portParms[j].sid = (BOS_UINT32) pHdl;
         portParms[j].port = rtpport;
         portParms[j].pSecret = NULL; /* STUN shared secret is not used */
         pPortParms[j] = &portParms[j];
         j++;
      }
      
      /* Setup the RTCP Port if there is RTCP */
      for ( i = 0; i < gIceIfList.num && bHasRtcp; i++ )
      {
         /* Create ICE listening socket */
         bosStatus = CFG_SYS->xtl.net.prot.gen.ice.pIceOpenCnxFunc( &gIceIfList.localIf[i].address,
                                 rtcpport,
                                 &pHdl );
         ICELOG( "IceOpenCnx -- Add RTCP pHdl[%x] index=%d", (int) pHdl, pAgent->vecSocketId.GetSize() );                              
         pAgent->vecSocketId.Append( (void*) pHdl );
         
         portParms[j].component = (ICE_COMP_ID)MX_NS gs_uRTCP_COMPONENT_ID;
         portParms[j].localIfId = gIceIfList.localIf[i].id;
         portParms[j].stunServerId = 0; /* not used since this is ICE lite*/
         portParms[j].transType = ICE_TRANS_UDP; /* always UDP */
         portParms[j].sid = (BOS_UINT32) pHdl;
         portParms[j].port = rtcpport;
         portParms[j].pSecret = NULL; /* STUN shared secret is not used */
         pPortParms[j] = &portParms[j];
         j++;
      }
      pPortParms[j] = NULL;
      /* Set the ICE Port from the SDP Media line */
      iceSetPortParms( pAgent->agentId, (const ICE_PORT_PARMS **)pPortParms );
      
      /* This is ICE-LITE, so a dummy gather candidate process */
      iceGatherCandidates( pAgent->agentId, 0 );

      pAgent->bPortSet = BOS_TRUE;
   }

   /* get the candidate lines */
   ICE_SDP_PARMS sdpParms;
   memset( &sdpParms, 0, sizeof(ICE_SDP_PARMS) );
   iceProcessLocalSdp( pAgent->agentId,
                       bIsCaller? ICE_SDP_OFFERER: ICE_SDP_ANSWERER,
                       &sdpParms );

   rIceUfrag = sdpParms.credential.ufrag;
   rIcePwd = sdpParms.credential.pwd;                          
   
   /* Copy the local candidates */
   memcpy( pAgent->lclCandidates, sdpParms.cndtAline, sizeof (pAgent->lclCandidates) );
   memcpy( pAgent->rmtCandidates, sdpParms.rCndtAline, sizeof (pAgent->rmtCandidates) );

   /* set the Agent current local default connection address */
   if ( pAgent->rmtIceMode != eMSEICEMODE_NONE )
   {
      /* Only update the local default connection address if 
      ** peer is not non-ICE.  For non-ICE peer, it 
      ** is updated in nsIceDecodeRemoteIceParams */
      bosIpAddrCreateCopy( &sdpParms.addr.ipAddr, &pAgent->lclDftCnxAddr );
   }
 
   /* Set connection address */
   bosIpAddrGetStr( &pAgent->lclDftCnxAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN );
   if ( bosIpAddrIsV4( &pAgent->lclDftCnxAddr ) )
   {
      eIPType = MX_NS CSdpParser::eIP4;
   }
   else if ( bosIpAddrIsV6( &pAgent->lclDftCnxAddr ) )
   {
      eIPType = MX_NS CSdpParser::eIP6;
   }

   /* c= address is default to "0.0.0.0" by CSdpCapabilitiesMgr */
   if ( strcmp( rStream.GetSession()->GetConnectionData().GetAddress(), "0.0.0.0" ) == 0 )
   {
      /* This is the first media in the session */
      /* Set session level connection data */
      rStream.GetSession()->GetConnectionData().SetAddress( ipAddrStr );
      rStream.GetSession()->GetConnectionData().SetAddressTypeId( eIPType );         
      ICELOG( "nsIceEncodeLocalIceParams session level c= to %s:%d", 
         ipAddrStr, sdpParms.addr.port );    
   }
   else if ( strcmp( rStream.GetSession()->GetConnectionData().GetAddress(), ipAddrStr ) != 0 )
   {
      /* media uses a different address, override the session default by writing
      ** its own c= line for this media. */
      /* Set media level connection data */
      rStream.GetExplicitConnectionData(0) = 
         rStream.GetSession()->GetConnectionData();
      rStream.GetExplicitConnectionData(0).SetAddress( ipAddrStr );       
      rStream.GetExplicitConnectionData(0).SetAddressTypeId( eIPType );   
      ICELOG( "nsIceEncodeLocalIceParams media level c= to %s:%d", 
         ipAddrStr, sdpParms.addr.port );          
   }
   /* Set media port */
   rStream.GetMediaAnnouncement().SetTransportPort(sdpParms.addr.port);
   
   /* a=rtcp attribute is mandatory in ICE if RTCP is enabled */
   /*
   ** RFC 5245 4.3. Encoding the SDP.
   ** If the agent is utilizing RTCP, it MUST encode the 
   ** RTCP candidate using the a=rtcp attribute as defined 
   ** in RFC 3605 [RFC3605]. If RTCP is not in use, the agent
   ** MUST signal that using b=RS:0 and b=RR:0 as defined 
   ** in RFC 3556 [RFC3556].    
   */
   if ( sdpParms.rtcpPort != 0 )
   {
      MX_NS CSdpFieldAttributeRtcp rtcpAttrib;
      rtcpAttrib = rStream.GetSdpFieldAttributeRtcp();
      rtcpAttrib.SetPort( sdpParms.rtcpPort );
      rStream.SetSdpFieldAttributeRtcp( rtcpAttrib );
   }
   
   /* Set ice-ufrag and ice-pwd attributes */
   rStream.GetIceUserFragment().SetUserFrag( sdpParms.credential.ufrag );
   rStream.GetIcePassword().SetPassword( sdpParms.credential.pwd );
   
   /* Set candidate attributes */
   for (i = 0; i < ICE_CNDT_ALINE_NUM_MAX; i++ )
   {
      if ( pAgent->lclCandidates[i][0] == '\0' )
      {
         break;
      }
      {
         const char fmtStr[]="candidate:%s %u %s %u %s %u typ %s";
         char typ[TYP_LEN_MAX+1];
         char cnxAddr[IP_ADDR_LEN_MAX+1];   
         char transportType[TRANSPORT_TYPE_LEN_MAX+1];
         char foundation[ICECNDT_FOUNDATION_LEN_MAX+1];
         BOS_UINT32 priority;
         BOS_UINT32 compId;
         BOS_UINT32 cnxPort;
         sscanf( pAgent->lclCandidates[i], fmtStr, foundation, &compId, transportType,
            &priority, cnxAddr, &cnxPort, typ );
         MX_NS CSocketAddr sockAddr;
         sockAddr.SetAddress( cnxAddr,
                                   cnxPort,
                                   eBASE_AUTO,
                                   BOS_FALSE);
         candidate.SetComponentId( compId );
         candidate.SetPriority( priority );
         candidate.SetConnectionAddr( sockAddr );
         candidate.GetCandidate() = typ;
         candidate.GetTransport() = transportType;
         candidate.GetFoundation() = foundation;

         candidateList.Append(candidate);
      }
   }
   rStream.GetIceCandidate() = candidateList;

   /* Set remote-candidates attributes */
   if ( pAgent->rmtCandidates[0] != '\0' )
   {
      MX_NS CSocketAddr raddr;
      char *ptr; 
      ptr = strchr (pAgent->rmtCandidates, ':');
      ptr++;
      
      char cnxAddr[IP_ADDR_LEN_MAX+1];   
      BOS_UINT32 cnxPort;
      BOS_UINT32 compId;
      int nVarFilled = 0;
      char *pch;
      pch = strtok( ptr, " " );
      while (pch != NULL)
      {
         nVarFilled++;
         switch (nVarFilled)
         {
            case 1:
               sscanf (pch, "%u", &compId );
               break;
            case 2:
               sscanf (pch, "%s", cnxAddr );
               break;
            case 3:
               sscanf (pch, "%u", &cnxPort );
               break;
            default:
               break;
         }
         
         if ( nVarFilled == 3)
         {
            MX_NS CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates* pRmtCan = 
               MX_NEW( MX_NS CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates );
               
            pRmtCan->SetComponentId( compId );
            raddr.SetAddress( cnxAddr, cnxPort, eBASE_AUTO, false );
            pRmtCan->SetConnectionAddr( raddr );
            rmtCandidateList.Append( pRmtCan );
            nVarFilled = 0;
         }
         pch = strtok( NULL, " " );
      }
   }
   rmtCandidate.GetIceRemoteCandidates() = rmtCandidateList;
   rStream.GetIceRemoteCandidatesAttribute() = rmtCandidate;
}

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
   MSEICEAGENTHANDLE iceAgentHdl)
{
   MSEICEAGENT *pAgent = (MSEICEAGENT *) iceAgentHdl;
   ICE_SDP_PARMS iceRmtSdp;
   BOS_BOOL bDoIce = BOS_FALSE;
   BOS_BOOL bIceLite = BOS_FALSE;
   memset( &iceRmtSdp, 0, sizeof(iceRmtSdp));

   /* Session Level ICE setting */
   MX_NS CSdpLevelSession* pSession = rStream.GetSession();
   if ( pSession->IsIceAttributePresent() )
   {
      if ( pSession->IsIceLite() )
      {
         bIceLite = BOS_TRUE;
      }
      /* For ice-ufrag and ice-pwd, session level value may get overwriten 
      ** by media level */
      MX_NS CSdpFieldAttributeIcePwd& icePwd = pSession->GetIcePassword();
      if ( icePwd.Validate() )
      {
         strncpy( iceRmtSdp.credential.pwd, icePwd.GetPassword(), sizeof(iceRmtSdp.credential.pwd));
      }
      MX_NS CSdpFieldAttributeIceUserFrag& iceUFrag = pSession->GetIceUserFragment();
      if ( iceUFrag.Validate() )
      {
         strncpy( iceRmtSdp.credential.ufrag, iceUFrag.GetUserFrag(), sizeof(iceRmtSdp.credential.ufrag));
      }
   }
   
   /* Media Level ICE setting */
   MX_NS CVector<MX_NS CSdpFieldAttributeIceCandidate> candidateList;
   MX_NS CSdpFieldAttributeIceCandidate candidate;
   MX_NS CVector<MX_NS CSdpFieldAttributeIceRemoteCandidates::CIceRemoteCandidates*> rmtCandidateList;
   MX_NS CSdpFieldAttributeIceRemoteCandidates rmtCandidate;
   if ( rStream.IsIceAttributePresent() )
   {
      MX_NS CBlob rCndtBlob;
      
      /* media level value ice-ufrag and ice-pwd value */
      MX_NS CSdpFieldAttributeIcePwd& icePwd = rStream.GetIcePassword();
      if ( icePwd.Validate() )
      {
         strncpy( iceRmtSdp.credential.pwd, icePwd.GetPassword(), sizeof(iceRmtSdp.credential.pwd));
      }
      MX_NS CSdpFieldAttributeIceUserFrag& iceUFrag = rStream.GetIceUserFragment();
      if ( iceUFrag.Validate() )
      {
         strncpy( iceRmtSdp.credential.ufrag, iceUFrag.GetUserFrag(), sizeof(iceRmtSdp.credential.ufrag));
      }
      
      /* setup the ice candidate line */
      candidateList = rStream.GetIceCandidate();
      
      unsigned int i;
      unsigned int uNumCndts;
      uNumCndts = candidateList.GetSize();
      for ( i = 0; i < uNumCndts && i < ICE_CNDT_ALINE_NUM_MAX; i++ )
      {
         candidate = candidateList[i];

         rCndtBlob.EraseAll();
         candidate.Serialize( rCndtBlob );
         rCndtBlob.Append((uint8_t)'\0'); /* append a NULL terminator */
         const char* pCndtLine = reinterpret_cast<const char*> ( rCndtBlob.GetFirstIndexPtr() );
         const char* ptr = strstr( pCndtLine, "candidate:" );
         strncpy(iceRmtSdp.cndtAline[i], ptr, ICE_CNDT_ALINE_LEN_MAX+1 );
      }
      /* setup the ice remote-candidates line */
      rmtCandidate = rStream.GetIceRemoteCandidatesAttribute();
      if ( rmtCandidate.Validate() )
      {
         rCndtBlob.EraseAll();
         rmtCandidate.Serialize( rCndtBlob );
         rCndtBlob.Append((uint8_t)'\0'); /* append a NULL terminator */
         const char* pRmtCndtLine = reinterpret_cast<const char*> ( rCndtBlob.GetFirstIndexPtr() );
         const char* ptr = strstr( pRmtCndtLine, "remote-candidates:" );
         strncpy(iceRmtSdp.rCndtAline, ptr, ICE_CNDT_ALINE_LEN_MAX+1 );
      }
      
      /* Do ICE processing unless 
      ** there is an "mis-match" attribute, or 
      ** there is a mismatch between ICE candidates and the connection address, or
      ** there is no ICE candidates */
      if ( !rStream.ValidateIceCandidates() ||
           rStream.IsIceMismatch() || 
           uNumCndts == 0 )
      {
         bDoIce = BOS_FALSE;
      }
      else
      {
         bDoIce = BOS_TRUE;
      }
   }
   else
   {
      ICELOG( "ICE attribute is missing or incorrect!  No ICE processing." );
      bDoIce = BOS_FALSE;
   }

   if ( bDoIce )
   {
      BOS_UINT16 rtcpPort;
      if ( bIceLite )
      {
         strncpy( iceRmtSdp.iceMode, ICE_LITE_STR, sizeof(iceRmtSdp.iceMode) );
      }
      /* else iceRmtSdp.iceMode == "", which is ICE full */
         
      bosIpAddrCreateFromStr( rStream.GetConnectionData(0).GetAddress(),
         &iceRmtSdp.addr.ipAddr );
      iceRmtSdp.addr.port = rStream.GetMediaAnnouncement().GetTransportPort();
      
      rtcpPort= rStream.GetSdpFieldAttributeRtcp().GetPort();
      if ( rtcpPort == (-1) )
      {
         rtcpPort = iceRmtSdp.addr.port + 1;
      }
      iceRmtSdp.rtcpPort = rStream.IsRtcpDeactivated()? 0 : rtcpPort;
         
      pAgent->rmtIceMode = bIceLite? eMSEICEMODE_LITE : eMSEICEMODE_FULL;
      if ( ICE_SUCCESS != iceProcessRemoteSdp( pAgent->agentId,
                          bIsCaller? ICE_SDP_OFFERER: ICE_SDP_ANSWERER,
                          &iceRmtSdp ) )
                          
      {
         ICELOG( "iceProcessRemoteSdp - FAILED unreachable!!!!!" );
         pAgent->bRmtUnreachable = TRUE;
      }
   }
   else
   {
      BOS_BOOL bRmtIPv4CnxAddr;
      BOS_BOOL bLclIPv4CnxAddr;
      BOS_UINT16 uIdx;
      pAgent->rmtIceMode = eMSEICEMODE_NONE;
      /* Check if peer address is reachable */
      
      /* Check if peer connection address
      ** matches with the one of the interface address type */
      bRmtIPv4CnxAddr = ( pSession->GetConnectionData().GetAddressTypeId() 
         == MX_NS CSdpParser::eIP4 );
      for ( uIdx = 0; uIdx < gIceIfList.num; uIdx++ )
      {
         bLclIPv4CnxAddr = bosIpAddrIsV4( &gIceIfList.localIf[uIdx].address);
         if ( bLclIPv4CnxAddr == bRmtIPv4CnxAddr )
         {
            /* same address type */
            break;
         }
      }
      
      if ( uIdx < gIceIfList.num )
      {
         bosIpAddrCreateCopy( &gIceIfList.localIf[uIdx].address, &pAgent->lclDftCnxAddr );
      }
      else
      {
         ICELOG( "iceProcessRemoteSdp - FAILED unreachable!!!!!" );         
         pAgent->bRmtUnreachable = TRUE;
      }
   }
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
   bosIpAddrGetStr(&(pAgent->lclDftCnxAddr), ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);      
   ICELOG( "nsIceDecodeRemoteIceParams: remote ice mode=%d lclDftCnxAddr=%s", pAgent->rmtIceMode, ipAddrStr);
   return pAgent->rmtIceMode;
}

/* static callback function */
void nsIceEventCB( const ICE_EVT        evt,
   const void           *pData,
   const BOS_UINT32     agentId )
{
   MSEICEAGENT* pAgent = NULL;
   ICE_MEDIA_TRANSPORT *pTransport = (ICE_MEDIA_TRANSPORT *) pData;
   /* search if the agent exist */
   int i = 0;
   int j;
   while (i < gp_vecIceAgents->GetSize())
   {
      pAgent = gp_vecIceAgents->GetAt(i);
      if ( pAgent && pAgent->agentId == agentId )
      {
         break;
      }
      i++;
   }
   if ( pAgent == NULL )
   {
      return;
   }
   
   /* make copy before posting - NotifyIceConcludedParameter will free it */
   MSEICEMEDIATRANSPORT *pIceMediaTransport = BRCM_NEW (MSEICEMEDIATRANSPORT);

   pIceMediaTransport->pMse = pAgent->pMse;
   pIceMediaTransport->eMediaType = pAgent->eMediaType;
   pIceMediaTransport->bEarlySession = pAgent->bEarlySession;
   switch (evt)
   {
      case ICE_EVT_CONCLUDED_FAILURE:
      {
         ICELOG( "ICE_EVT_CONCLUDED_FAILURE! No candidate pair can be found." );
         pIceMediaTransport->pMediaTransport = NULL;
         /* Queue a message the work queue to be handled ICE result.
         ** It is very important to be using the message queue, as MSE expects
         ** to finish it current fsmProcess()before running 
         ** NotifyIceConcludedParameter() */
         if ( tkmMsgPost( BOS_FALSE, (BOS_UINT32) evt, (void*) pIceMediaTransport,
            &MX_NS MseSdpSession::NotifyIceConcludedParameter )  != eCCTKSTATUS_SUCCESS )
         {
            BRCM_DELETE( pIceMediaTransport );
         }   
         pAgent->state = eMSEICEAGENTSTATE_DONE;
      }
      break;
      case ICE_EVT_CONCLUDED_SUCCESS:
      case ICE_EVT_REOFFER:
      {
         ICELOG( "ICE_EVT_CONCLUDED_SUCCESS/REOFFER." );         
         /* We have results */
         ICE_MEDIA_TRANSPORT *pMediaTransport = BRCM_NEW (ICE_MEDIA_TRANSPORT);  

         /* make copy before posting - NotifyIceConcludedParameter will free it */
         memcpy( pMediaTransport, pData, sizeof (ICE_MEDIA_TRANSPORT) );
         pIceMediaTransport->pMediaTransport = pMediaTransport;

         /* Queue a message the work queue to be handled ICE result.
         ** It is very important to be using the message queue, as MSE expects
         ** to finish it current fsmProcess()before running 
         ** NotifyIceConcludedParameter() */            
         if ( tkmMsgPost( BOS_FALSE, (BOS_UINT32) evt, (void*) pIceMediaTransport,
            &MX_NS MseSdpSession::NotifyIceConcludedParameter )  != eCCTKSTATUS_SUCCESS )
         {
            BRCM_DELETE( pMediaTransport );
            BRCM_DELETE( pIceMediaTransport );
         }
         pAgent->state = eMSEICEAGENTSTATE_DONE;         
      }
      break;
      case ICE_EVT_CANDIDATES_GATHER_COMPLETE:
      case ICE_EVT_CANDIDATES_GATHER_TIMEOUT:
      {
         ICELOG( "ICE_EVT_CANDIDATES_GATHER_COMPLETE/TIMEOUT." );              
         pAgent->state = eMSEICEAGENTSTATE_IN_PROGRESS;
      }
      break;
      default: 
      {
         /* no action needed */
      }
      break;
   }
}

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
   BOS_IP_ADDRESS *pLclDftCnxAddr )
{
   MSEICEAGENT *pAgent = (MSEICEAGENT *) iceAgentHdl;
   if ( pAgent->bRmtUnreachable )
   {
      return eMSEICESTATUS_FAIL;
   }
   
   if ( pLclDftCnxAddr ) 
   {
      bosIpAddrCreateCopy( &pAgent->lclDftCnxAddr, pLclDftCnxAddr );
   }
   
   switch ( pAgent->state )
   {
      case eMSEICEAGENTSTATE_INITIAL:
      case eMSEICEAGENTSTATE_IN_PROGRESS:
      {
         return eMSEICESTATUS_IN_PROGRESS;
      }
      case eMSEICEAGENTSTATE_DONE:
      default:
      {
         return eMSEICESTATUS_CONCLUDED;
      }
   }
}

void dumpMseIceAgent( MSEICEAGENTHANDLE iceAgentHdl )
{
   MSEICEAGENT *pAgent = (MSEICEAGENT *) iceAgentHdl;
   int i;
   
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
   bosIpAddrGetStr(&(pAgent->lclDftCnxAddr), ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);     
   
   if ( pAgent )
   {
      char tempStr[256];
      char *ptr = tempStr;
      ICELOG( "MseIceAgent [0x%x]: Id=0x%x, uMedIndex=0x%x, Mse=0x%x, type=%d, bEarly=%d, bPortSet=%d, bRmtUnreachable=%d, lclIceMode=%d, rmtIceMode=%d lclDftCnxAddr=%s",
         pAgent, 
         pAgent->agentId,
         pAgent->index,
         pAgent->pMse,
         pAgent->eMediaType,
         pAgent->bEarlySession, 
         pAgent->bPortSet,
         pAgent->bRmtUnreachable,
         pAgent->lclIceMode,
         pAgent->rmtIceMode, 
         ipAddrStr );
         
      ptr += sprintf( ptr, "   sockHdl:" );
      for ( i = 0; i < pAgent->vecSocketId.GetSize(); i++ )
      {
         ptr += sprintf( ptr, " 0x%x", pAgent->vecSocketId[i] );
      }
      ICELOG( "%s", tempStr );
      
      for ( i = 0; i < ICE_CNDT_ALINE_NUM_MAX && pAgent->lclCandidates[i][0] != '\0'; i++ )
      {
         ICELOG( "   aline: [%s]", pAgent->lclCandidates[i] );
      }      
      if ( pAgent->rmtCandidates[0] != '\0' )
      {
         ICELOG( "   rline: [%s]", pAgent->rmtCandidates );
      }
   }
}
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
