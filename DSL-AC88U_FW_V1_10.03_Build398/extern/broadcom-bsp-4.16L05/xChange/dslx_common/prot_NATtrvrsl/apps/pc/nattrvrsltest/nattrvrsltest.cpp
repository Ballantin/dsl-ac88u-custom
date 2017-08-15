/****************************************************************************
*
* Copyright © 2002-2008 Broadcom Corporation
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
******************************************************************************
*
*    Description:
*
*      NAT Traversal Test Client
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include "WinMain.h"
#include "nattrvrsltest.h"
#include <bosInit.h>
#include <bosTask.h>
#include <bosSleep.h>
#include <bosTypes.h>
#include <stunc.h>
#include <turnc.h>
#include <ice.h>
#include "hmac.h"
#include "crc32.h"


/* ---- Public Variables ------------------------------------- */
char gLocalHost[NATTRVRSLTEST_ADDR_BUFFER_SIZE] = {NULL};
int gTlsPort = 0;
int gUdpPort[MAX_UDP_PORTS]={0};
int gStuncStart = 0;
int gTurncStart = 0;
int gIceStart = 0;
int gSyncEvt = 0;
int gTurnUdpPort = 0;
int gTurnTcpPort = 0;
int gTurnTlsPort = 0;
char gTurnHost[NATTRVRSLTEST_ADDR_BUFFER_SIZE] = {NULL};
int gTurnPort = 0;
char gStunHost[NATTRVRSLTEST_ADDR_BUFFER_SIZE] = {NULL};
int gStunPort = 0;
int gStunMaxWait = 0;
int gBandwidth = -1;
int gLifetime = -1;
int gReqdTransport = -1;
char gReqdIPAddr[NATTRVRSLTEST_ADDR_BUFFER_SIZE] = {NULL};
int gReqdPort = 0;
int gPortProps = -1;
char gActiveDestIPAddr[NATTRVRSLTEST_ADDR_BUFFER_SIZE] = {NULL};
int gActiveDestPort = 0;
char gCnxRemoteIPAddr[NATTRVRSLTEST_ADDR_BUFFER_SIZE] = {NULL};
int gCnxRemotePort = 0;
char gSendDataRemoteIPAddr[NATTRVRSLTEST_ADDR_BUFFER_SIZE] = {NULL};
int gSendDataRemotePort = 0;
int gDataLen = 0;
int gTurnOrgTransport = 0;
int gAllocateHdl = 0;

int gIceAgentId = 0;
int gIcePrefLocal = 0;
int gIcePrefHost = 0;
int gIcePrefSrflx = 0;
int gIcePrefPrflx = 0;
int gIcePrefRelay = 0;
int gIceRtpPort[3]={0};
int gIceRtcpPort[3]={0};
int gIceBRtp[3]={0};
int gIceBRtcp[3]={0};
int gIcePrefOverride = 0;
int gIceLiteMode = 0;
int gIceAggressiveNom = 0;
int gIceMaxWait = 0;
int gIceSdpAgentId = 0;
int gIceSdpRole = 0;
int gIceRSdpAgentId = 0;
int gIceRSdpRole = 0;

char gSdpLine1[NATTRVRSLTEST_SDP_BUFFER_SIZE]={NULL};
char gSdpLine2[NATTRVRSLTEST_SDP_BUFFER_SIZE]={NULL};
char gSdpLine3[NATTRVRSLTEST_SDP_BUFFER_SIZE]={NULL};
char gSdpLine4[NATTRVRSLTEST_SDP_BUFFER_SIZE]={NULL};

extern "C"
{
int gTurnctest_ConnectError = 0;
int gTurnctest_AllocateError = 0;
int gTurnctest_ActiveDestError = 0;
int gStuntest_BindingError = 0;
int gStuntest_SharedSecretError = 0;
extern void dumpCndtLstAll( BOS_UINT32 agentId );
extern void dumpChkLst( BOS_UINT32 agentId );
int gIcedbg_RuntimeDLog = 1;
}
/* For debugging purposes */
#include "../../../stacks/ice/icedbg.h"

/* ---- Private Constants and Types -------------------------- */
#define LOG( x )  Log x 
#define MAX_STR   100
#define MAX_BUF_SIZE 1500
#define MAX_SOCKET_NUM   20
#define TNRX_TASK_STACK       (6 *1024)
#define TNRX_TASK_PRIORITY    BOS_TASK_CLASS_MED

#define STR( arg) { arg, #arg }
/* generic mapped string table */
typedef struct
{
   BOS_UINT32 id;
   char  *str;
} MAPPED_STR;

/* Socket handle */
typedef struct
{
   BOS_SOCKET *pSoc;    /* BOS socket handle */
   BOS_UINT32  sid;     /* Socket id */
}SOCKET_HANDLE;
/* ---- Private Variables ------------------------------------ */
static BOS_BOOL bStarted = BOS_FALSE;
static BOS_TASK_ID rxTaskId = (BOS_TASK_ID)NULL;
static SOCKET_HANDLE socHdlList[MAX_SOCKET_NUM] = {0};
/*
 * local sockets
 */
static BOS_SOCKET stunUdpSocket[MAX_UDP_PORTS];
static BOS_SOCKET stunTlsSocket;
static BOS_SOCKET turnUdpSocket;
static BOS_SOCKET turnTcpSocket;
static BOS_SOCKET turnTlsSocket;

static BOS_SOCKET iceRtpSocket[3];
static BOS_SOCKET iceRtcpSocket[3];

/* cached username and password */
static STUNC_SHARED_SECRET secret;
static TURNC_SHARED_SECRET *pSecret = (TURNC_SHARED_SECRET *)&secret;
static char username[100] = { 0 };
static char password[100] = { 0 };

static BOS_UINT32 agentId[3] = {0};

static BOS_IP_ADDRESS turnServerIpAddr;

static char testPayload[] = "this is a test payload";

static BOS_UINT8 recvPayload[] = {
0x00, 0x15, /* data indication */
0x00, 0x30, /* message length */
/* transaction id */
0x21, 0x12, 0xA4, 0x42, 
0xab, 0xcd, 0xdc, 0xba, 
0x12, 0x34, 0x43, 0x21, 
0x11, 0x22, 0x33, 0x44, 
/* remote transport: ipv4, ip = 10.136.64.99, port = 54321*/
0x00, 0x12, 0x00, 0x08,
0x00, 0x01, 0xD4, 0x31, 0x0A, 0x88, 0x40, 0x63,
/* Data, len = 32 byes */
0x00, 0x13, 0x00, 0x20,
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 
0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37 
};

/* TODO: Connect Status Indication details incomplete in 
 * draft-ietf-behave-turn-02 incomplete 
 */
static BOS_UINT8 cnxStatus[] = {
0x01, 0x18, /* Connect Status Indication */
0x00, 0x00, /* message length */
/* transaction id */
0x21, 0x12, 0xA4, 0x42, 
0x11, 0x22, 0x33, 0x44, 
0x12, 0x34, 0x43, 0x21, 
0xab, 0xcd, 0xdc, 0xba
};

/* debug lookup tables */
static MAPPED_STR stuncEvtMap[]=
{
   STR(STUNC_EVT_MAPPED_ADDRESS),   /* mapped address */
   STR(STUNC_EVT_SHARED_SECRET),    /* shared-secret */
   STR(STUNC_EVT_TX_TIMEOUT),       /* transmission timeout */
   STR(STUNC_EVT_RES_ERROR),        /* error response */
   STR(STUNC_EVT_SYSTEM_ERR)        /* System error */
};

static MAPPED_STR turncEvtMap[]=
{
   STR(TURNC_EVT_ALLOCATE_SUCCESS),    /* ALLOCATE success */
   STR(TURNC_EVT_ALLOCATE_ERROR),      /* ALLOCATE error */
   STR(TURNC_EVT_ACTIVE_DEST_SET),     /* Active Destination set */
   STR(TURNC_EVT_ACTIVE_DEST_UNSET),  /* Active Destination not set*/
   STR(TURNC_EVT_ACTIVE_DEST_ERROR),   /* Active Destination set error */
   STR(TURNC_EVT_CONNECT_SUCCESS),     /* Connect success */
   STR(TURNC_EVT_CONNECT_ERROR),       /* Connect error */
   STR(TURNC_EVT_CONNECT_STATUS)       /* Connect status */
};

static MAPPED_STR iceEvtMapStr[]=
{
   STR(ICE_EVT_CANDIDATES_GATHER_COMPLETE),  /* Candidate event */
   STR(ICE_EVT_CANDIDATES_GATHER_TIMEOUT),   /* Candidate event */
   STR(ICE_EVT_CONCLUDED_SUCCESS),           /* Candidate event */
   STR(ICE_EVT_CONCLUDED_FAILURE)            /* Candidate event */
};

/* ---- Private Function Prototypes -------------------------- */
static void stuncPacketSender( const BOS_UINT32      sid,
                               const STUN_TRANSPORT  *pDest,
                               const BOS_UINT8       *pData,
                               const BOS_UINT16      len );
static void stuncEventHandler( const BOS_UINT32       reqHdl,
                               const STUNC_EVT_DATA   *pEvtData );
static void hasher( const STUN_HMAC_TYPE  alg, 
                    const STUN_DATA       *pKey,
                    const STUN_DATA       *pData,
                    STUN_DATA             *pHmac );

static BOS_UINT32 checksum32( const BOS_UINT8   *pData,
                              const BOS_UINT16  len );
static void rxThread( void );
static void turncEventHandler( const BOS_UINT32       reqHdl,
                               const TURNC_EVT_DATA   *pEvtData );

static void turncSendData( const BOS_UINT32        sid,
                           const STUN_TRANSPORT    *pDest,
                           const BOS_UINT8         *pData,
                           const BOS_UINT16        len);

static void turncRecvData( const BOS_UINT32        sid,
                           const STUN_TRANSPORT    *pSrc,
                           const BOS_UINT8         *pData,
                           const BOS_UINT16        len);

static BOS_SOCKET *getOrgSocket( int transport );
static void fillAllocateOpts( TURNC_ALLOCATE_OPTS *opts );
static BOS_BOOL getServer( BOS_SOCKET **ppSoc, BOS_IP_ADDRESS *pIpAddr, BOS_SOCKET_IP_PORT *pPort );
static void iceEventHandler( const ICE_EVT      evt,
                             const void         *pData,
                             const BOS_UINT32   reqHdl );
static BOS_UINT32 addSocToList( BOS_SOCKET *pSoc );
static BOS_SOCKET *pSocFromSid( BOS_UINT32 sid );
static BOS_UINT32 sidFromSoc( BOS_SOCKET *pSoc );

/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestInit
*
*  PURPOSE:
*     Initialize NATT test client 
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestInit( void )
{
   int i;
   BOS_STATUS status;

   status = bosInit();
   BOS_ASSERT( BOS_STATUS_OK == status );

   for ( i = 0; i < MAX_UDP_PORTS; i++ )
   {
      memset(&stunUdpSocket[i], 0, sizeof( BOS_SOCKET ));
   }
   memset(&stunTlsSocket, 0, sizeof( BOS_SOCKET ));

   memset(&turnUdpSocket, 0, sizeof( BOS_SOCKET ));
   memset(&turnTcpSocket, 0, sizeof( BOS_SOCKET ));
   memset(&turnTlsSocket, 0, sizeof( BOS_SOCKET ));

   for ( i = 0; i < 3; i++ )
   {
      memset(&iceRtpSocket[i], 0, sizeof( BOS_SOCKET ));
      memset(&iceRtcpSocket[i], 0, sizeof( BOS_SOCKET ));
   }
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestStart
*
*  PURPOSE:
*     Configure and Start TURNC client
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestStart( void )
{
   BOS_STATUS status;
   STUNC_CFG   stuncCfg;
   STUN_CFG    stunCfg;
   TURNC_CFG   turncCfg;
   ICE_CFG     iceCfg;
   char errStr[MAX_STR];
   BOS_SOCKET_IP_PORT turnServerPort;

   int i;
   BOS_IP_ADDRESS   localAddr;

   if ( BOS_TRUE != bStarted )
   {
      /* initialize the sharedsecret being use in the test */
      memset( &secret, 0, sizeof( STUNC_SHARED_SECRET ) );
      memset( &socHdlList, 0, MAX_SOCKET_NUM * sizeof(SOCKET_HANDLE));
      /* startup STUNC client */
      stunCfg.sendPktCB = stuncPacketSender;
      stunCfg.rand32CB = NULL;
      stunCfg.hmacCB = hasher;
      stunCfg.crc32CB = checksum32;
      stunCfg.logCB = Log;
      stuncCfg.pStunCfg = &stunCfg;

      if ( gStuncStart )
      {
         if ( STUNC_SUCCESS == stuncStartup( &stuncCfg ) )
         {
            LOG(( "nattrvrsltestStart: STUNC client Started"));
         }
         else
         {
            LOG(( "nattrvrsltestStart: STUNC client failed to start"));
         }
      }
    
      /* start TURNC */
      turncCfg.evtCB = turncEventHandler;
      turncCfg.sendCB = turncSendData;
      turncCfg.recvCB = turncRecvData;
      turncCfg.logCB = Log;
      if ( gTurncStart )
      {
         if ( TURNC_SUCCESS == turncStartup( &turncCfg ) )
         {
            LOG(( "nattrvrsltestStart: TURNC client Started"));
         }
         else
         {
            LOG(( "nattrvrsltestStart: TURNC client failed to start"));
         }
      }

      status = bosIpAddrCreateFromStr( gLocalHost, &localAddr );
      BOS_ASSERT( BOS_STATUS_OK == status );
      
      /* Open STUN UDP sockets */
      for ( i = 0; gUdpPort[i] != 0; i++ )
      {
         LOG(( "nattrvrsltestStart: Open STUN UDP socket %d", gUdpPort[i] ));

         /* create UDP socket */
         status = bosSocketCreateDatagram( &stunUdpSocket[i] );
         BOS_ASSERT( BOS_STATUS_OK == status );
         addSocToList( &stunUdpSocket[i] );

         /* set socket nonblocking */
         status = bosSocketSetNonBlocking( &stunUdpSocket[i], BOS_TRUE );
         BOS_ASSERT( BOS_STATUS_OK == status );

         /* bind socket to port */
         status = bosSocketBindIp( &stunUdpSocket[i], 0, &localAddr, (BOS_SOCKET_IP_PORT)gUdpPort[i] );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* Open STUN TLS socket */
      if ( gTlsPort != 0 )
      {
         LOG(( "nattrvrsltestStart: Open STUN TLS socket %d", gTlsPort ));
         status = bosSocketCreateStream( &stunTlsSocket );
         BOS_ASSERT( BOS_STATUS_OK == status );
         addSocToList( &stunTlsSocket );

         status = bosSocketBindIp( &stunTlsSocket, 0, &localAddr, (BOS_SOCKET_IP_PORT)gTlsPort );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* Open TURN UDP socket */
      if ( gTurnUdpPort != 0 )
      {
         LOG(( "nattrvrsltestStart: Open TURN UDP socket %d", gTurnUdpPort ));
         
         /* create UDP socket */
         status = bosSocketCreateDatagram( &turnUdpSocket );
         BOS_ASSERT( BOS_STATUS_OK == status );
         addSocToList( &turnUdpSocket );

         /* set socket nonblocking */
         status = bosSocketSetNonBlocking( &turnUdpSocket, BOS_TRUE );
         BOS_ASSERT( BOS_STATUS_OK == status );

         /* bind socket to port */
         status = bosSocketBindIp( &turnUdpSocket, 0, &localAddr, (BOS_SOCKET_IP_PORT)gTurnUdpPort );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* Open TURN TCP socket */
      if ( gTurnTcpPort != 0 )
      {
         LOG(( "nattrvrsltestStart: Open TURN TCP socket %d", gTurnTcpPort ));
         status = bosSocketCreateStream( &turnTcpSocket );
         BOS_ASSERT( BOS_STATUS_OK == status );
         addSocToList( &turnTcpSocket );

         status = bosSocketBindIp( &turnTcpSocket, 0, &localAddr, (BOS_SOCKET_IP_PORT)gTurnTcpPort );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* Open TURN TLS socket */
      if ( gTurnTlsPort != 0 )
      {
         LOG(( "nattrvrsltestStart: Open TURN TLS socket %d", gTurnTlsPort ));
         status = bosSocketCreateStream( &turnTlsSocket );
         BOS_ASSERT( BOS_STATUS_OK == status );
         addSocToList( &turnTlsSocket );

         status = bosSocketBindIp( &turnTlsSocket, 0, &localAddr, (BOS_SOCKET_IP_PORT)gTurnTlsPort );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* look up TURN server IP */
      if ( strlen( gTurnHost ) )
      {
         /* WARNING: this routine returns host byte ordering in ipAddr and 
          * ipPort 
          */
         status = bosSocketLookupServerAndPort( gTurnHost,
                                                &turnServerIpAddr,
                                                &turnServerPort,
                                                errStr,
                                                MAX_STR );
         if ( status != BOS_STATUS_OK )
         {
            LOG(("getServer: TURN Server lookup failed err %s, request not sent", errStr));
         }

      }
      else
      {
         LOG(("getServer: invalid TURN Server, request not sent"));
      }

      
      // Open ICE RTP/RTCP UDP Ports
      for ( i = 0; i < 3; i++ )
      {
         /* create UDP socket */
         status = bosSocketCreateDatagram( &iceRtpSocket[i] );
         BOS_ASSERT( BOS_STATUS_OK == status );
         addSocToList( &iceRtpSocket[i] );
         status = bosSocketCreateDatagram( &iceRtcpSocket[i] );
         BOS_ASSERT( BOS_STATUS_OK == status );
         addSocToList( &iceRtcpSocket[i] );

         /* set socket nonblocking */
         status = bosSocketSetNonBlocking( &iceRtpSocket[i], BOS_TRUE );
         BOS_ASSERT( BOS_STATUS_OK == status );
         status = bosSocketSetNonBlocking( &iceRtcpSocket[i], BOS_TRUE );
         BOS_ASSERT( BOS_STATUS_OK == status );

         // Set Binding of UDP Port
         if ( gIceBRtp[i] )
         {
            LOG(( "nattrvrsltestStart: Open ICE RTP UDP socket %d", gIceRtpPort[i] ));
            /* bind socket to port */
            status = bosSocketBindIp( &iceRtpSocket[i], 0, &localAddr, (BOS_SOCKET_IP_PORT)gIceRtpPort[i] );
            BOS_ASSERT( BOS_STATUS_OK == status );
         }

         // Set Binding of UDP Port
         if ( gIceBRtcp[i] && gIceRtcpPort[i] )
         {
            LOG(( "nattrvrsltestStart: Open ICE RTCP UDP socket %d", gIceRtcpPort[i] ));
            /* bind socket to port */
            status = bosSocketBindIp( &iceRtcpSocket[i], 0, &localAddr, (BOS_SOCKET_IP_PORT)gIceRtcpPort[i] );
            BOS_ASSERT( BOS_STATUS_OK == status );
         }

      }

      if ( gIceStart )
      {
         ICE_LOCAL_IF_LIST localIfList;
         STUNC_CFG *pStuncCfg = &stuncCfg;
         TURNC_CFG *pTurncCfg = &turncCfg;
         if ( gStuncStart )
         {
            pStuncCfg = NULL;
         }
         if ( gTurncStart )
         {
            pTurncCfg = NULL;
         }
         localIfList.num = 1;
         localIfList.localIf[0].id = 0;
         localIfList.localIf[0].address = localAddr;

         
         iceCfg.pLocalIfList = &localIfList;
         iceCfg.evtCB = iceEventHandler;
         iceCfg.logCB = Log;
         iceCfg.pStuncCfg = pStuncCfg;
         iceCfg.pTurncCfg = pTurncCfg;
         if ( ICE_SUCCESS == iceStartup( &iceCfg ) )
         {
            LOG(( "nattrvrsltestStart: ICE Started"));
         }
         else
         {
            LOG(( "nattrvrsltestStart: ICE failed to start"));
         }
      }

      /* Create a thread to listen for packets from the sockets */
      if ( BOS_STATUS_OK != bosTaskCreateEx( "TNRX",
                                             TNRX_TASK_STACK,
                                             TNRX_TASK_PRIORITY,
                                             NULL,
                                             (BOS_TASK_ENTRY)rxThread,
                                             NULL,
                                             (BOS_TASK_ARG *)NULL,
                                             &rxTaskId ))
      {
         LOG(( "nattrvrsltestStart: Failed to create Read Thread"));
      }
 
      bStarted = BOS_TRUE;
   }
   else
   {
      LOG(( "nattrvrsltestStart: NAT Traversal Test client already started."));
   }
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestEnd
*
*  PURPOSE:
*     Terminates NATT Test client
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestEnd( void )
{
   BOS_STATUS status;
   int i;
   if ( bStarted )
   {
      /* destroy socket listening thread */
      bosTaskDestroy( &rxTaskId );

      /* close STUN UDP sockets */
      for ( i = 0; i < MAX_UDP_PORTS; i++ )
      {
         if ( gUdpPort[i] )
         {
            LOG(( "nattrvrsltestEnd: Close STUN UDP socket %d", gUdpPort[i] ));
            status = bosSocketClose( &stunUdpSocket[i] );
            BOS_ASSERT( BOS_STATUS_OK == status );
         }
      }

      /* close STUN TLS socket */
      if ( gTlsPort )
      {
         LOG(( "nattrvrsltestEnd: Close STUN TLS socket %d", gTlsPort ));
         status = bosSocketClose( &stunTlsSocket );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* Close TURN UDP socket */
      if ( gTurnUdpPort )
      {
         LOG(( "nattrvrsltestEnd: Close TURN UDP socket %d", gTurnUdpPort ));
         status = bosSocketClose( &turnUdpSocket );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* Close TURN TCP socket */
      if ( gTurnTcpPort )
      {
         LOG(( "nattrvrsltestEnd: Close TURN TCP socket %d", gTurnTcpPort ));
         status = bosSocketClose( &turnTcpSocket );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* Close TURN TCP socket */
      if ( gTurnTlsPort )
      {
         LOG(( "nattrvrsltestEnd: Close TURN TLS socket %d", gTurnTlsPort ));
         status = bosSocketClose( &turnTlsSocket );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      /* Close ICE Sockets */
      for ( i = 0; i < 3; i++ )      
      {
         LOG(( "nattrvrsltestEnd: Close ICE RTP/RTCP" ));
         status = bosSocketClose( &iceRtpSocket[i] );
         BOS_ASSERT( BOS_STATUS_OK == status );
         status = bosSocketClose( &iceRtcpSocket[i] );
         BOS_ASSERT( BOS_STATUS_OK == status );
      }

      if ( gIceStart )
      {
         /* shutdown ICE Client */
         if ( STUNC_SUCCESS == iceShutdown() )
         {
            LOG(( "nattrvrsltestEnd: ICE client shuts down"));
         }
         else
         {
            LOG(( "nattrvrsltestEnd: ICE client failed to shut down"));
         }
      }

      if ( gTurncStart )
      {
         /* shutdown TURNC Client */
         if ( TURNC_SUCCESS == turncShutdown() )
         {
            LOG(( "nattrvrsltestEnd: TURNC client shuts down"));
         }
         else
         {
            LOG(( "nattrvrsltestEnd: TURNC client failed to shut down"));
         }
      }

      if ( gStuncStart )
      {
         /* shutdown STUNC Client */
         if ( STUNC_SUCCESS == stuncShutdown() )
         {
            LOG(( "nattrvrsltestEnd: STUNC client shuts down"));
         }
         else
         {
            LOG(( "nattrvrsltestEnd: STUNC client failed to shut down"));
         }
      }

      bStarted = BOS_FALSE;
   }
   else
   {
      LOG(( "nattrvrsltestEnd: NAT Traversal Test client not started yet"));
   }
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestStuncSharedSecretReq
*
*  PURPOSE:
*     Send Shared Secret Request to TURN server over TLS port.
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void  nattrvrsltestStuncSharedSecretReq( void )
{
   BOS_STATUS status;
   static BOS_IP_ADDRESS      ipAddr;
   static BOS_SOCKET_IP_PORT  ipPort;
   STUN_TRANSPORT dest;
   char errStr[MAX_STR];
   BOS_UINT32 reqHdl;
   STUN_OPTS options = 0;
   static BOS_BOOL bConnected = BOS_FALSE;
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
   STUNC_EVT_DATA evt;
   STUNC_EVT_DATA *pEvt = NULL;

   if ( !bStarted )
   {
      LOG(( " nattrvrsltestStuncSharedSecret: TURNC client not started."));
      return;
   }

   if ( !bConnected )
   {
      /* look up server IP */
      if ( strlen( gStunHost ) )
      {
         /* WARNING: this routine returns host byte ordering in ipAddr 
          * and port 
          */
         status = bosSocketLookupServerAndPort( gStunHost,
                                                &ipAddr,
                                                &ipPort,
                                                errStr,
                                                MAX_STR );
         if ( status != BOS_STATUS_OK )
         {
            LOG((" nattrvrsltestStuncSharedSecretReq: TLS Server lookup failed err %s, request not sent", errStr));
            return;
         }
      }
      else
      {
         LOG((" nattrvrsltestStuncSharedSecretReq: invalid TLS Server, request not sent"));
         return;
      }

      bosIpAddrGetStr(&ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
      
      LOG((" nattrvrsltestStuncSharedSecretReq: IP lookup OK. connecting to Server IP %s port %d", ipAddrStr, gStunPort ));

      status = bosSocketConnectIp( &stunTlsSocket, &ipAddr, (BOS_SOCKET_IP_PORT)gStunPort);
      if ( status != BOS_STATUS_OK )
      {
         LOG((" nattrvrsltestStuncSharedSecretReq: Failed to connect to server"));
         return;
      }

      /* connected to server */
      bConnected = BOS_TRUE;
   }
   else
   {
      LOG((" nattrvrsltestStuncSharedSecretReq: Server already connected"));
   }

   /* change address back to host byte ordering */
   dest.ipAddr = ipAddr;
   dest.port = (BOS_SOCKET_IP_PORT)gStunPort;
   if ( gSyncEvt == 1 )
   {
      pEvt = &evt;
      options |= STUN_OPT_SYNCD_RES;
   }
   /* issue request */
   if (STUNC_SUCCESS != stuncSharedSecret( sidFromSoc(&stunTlsSocket), 
                                           &dest, 
                                           0, 
                                           &reqHdl, 
                                           stuncEventHandler,
                                           gStunMaxWait
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                                           ,pEvt
#endif
                                           ))
   {
      LOG((" nattrvrsltestStuncSharedSecretReq: Failed to issue req to stack"));
   }
   else
   {
      LOG((" nattrvrsltestStuncSharedSecretReq: req passed to stack"));
      if ( NULL != pEvt )
      {
         if ( pEvt->evt == STUNC_EVT_SHARED_SECRET )
         {
            STUNC_SHARED_SECRET *pSs = (STUNC_SHARED_SECRET *)&pEvt->data.sharedSecret;
            /* extract username and password */
            LOG((" nattrvrsltestStuncSharedSecretReq: Username = [%.*s], Password = [%.*s]", pSs->username.size, pSs->username.buf, pSs->password.size, pSs->password.buf ));
            /* cache the username and password */
            memcpy( secret.username.buf, pSs->username.buf, pSs->username.size );
            secret.username.size = pSs->username.size;
            memcpy( secret.password.buf, pSs->password.buf, pSs->password.size );
            secret.password.size = pSs->password.size;
         }
         else
         {
            LOG((" nattrvrsltestStuncSharedSecretReq: Failed to get Shared Secret. evt %d", pEvt->evt));
         }
      }
   }
}

/*****************************************************************************
*  FUNCTION:
*      nattrvrsltestStuncClearUsrPwdCache
*
*  PURPOSE:
*     clear username and password cache
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void  nattrvrsltestStuncClearUsrPwdCache( void )
{
   LOG((" nattrvrsltestStuncClearUsrPwdCache:"));
   LOG(("--- current Username = [%.*s], Password = [%.*s]", secret.username.size, secret.username.buf, secret.password.size, secret.password.buf ));
   username[0] = '\0';
   memset( &secret, 0, sizeof( STUNC_SHARED_SECRET ) );
}

/*****************************************************************************
*  FUNCTION:
*      nattrvrsltestStuncShowUsrPwdCache
*
*  PURPOSE:
*     display username and password cache
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void  nattrvrsltestShowUsrPwdCache( void )
{
   LOG((" nattrvrsltestStuncShowUsrPwdCache:"));
   LOG(("--- current Username = [%.*s], Password = [%.*s]", secret.username.size, secret.username.buf, secret.password.size, secret.password.buf ));
}

/*****************************************************************************
*  FUNCTION:
*      nattrvrsltestStuncBindingReq
*
*  PURPOSE:
*     Sending Binding Request
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void  nattrvrsltestStuncBindingReq( void )
{
   int i;
   BOS_STATUS status;
   BOS_IP_ADDRESS      ipAddr;
   BOS_SOCKET_IP_PORT  ipPort;
   STUN_TRANSPORT dest;
   BOS_UINT32 reqHdl;
   STUN_OPTS options = 0;
   char errStr[MAX_STR];
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
   STUNC_EVT_DATA evt;
   STUNC_EVT_DATA *pEvt = NULL;

   if ( !bStarted )
   {
      LOG(( " nattrvrsltestStuncBindingReq: TURNC client not started."));
      return;
   }

   /* look up server IP */
   if ( strlen( gStunHost ) )
   {
      /* WARNING: this routine returns host byte ordering in ipAddr and 
       * ipPort 
       */
      status = bosSocketLookupServerAndPort( gStunHost,
                                             &ipAddr,
                                             &ipPort,
                                             errStr,
                                             MAX_STR );
      if ( status != BOS_STATUS_OK )
      {
         LOG((" nattrvrsltestStuncBindingReq: STUN Server lookup failed err %s, request not sent", errStr));
         return;
      }
   }
   else
   {
      LOG((" nattrvrsltestStuncBindingReq: invalid STUN Server, request not sent"));
      return;
   }

   bosIpAddrGetStr(&ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
   LOG((" nattrvrsltestStuncBindingReq: IP lookup OK. Sending request to Server IP %s port %d", ipAddrStr, gStunPort ));

   dest.ipAddr = ipAddr;
   dest.port = (BOS_SOCKET_IP_PORT)gStunPort;
   if ( gSyncEvt == 1 )
   {
      pEvt = &evt; 
      options |= STUN_OPT_SYNCD_RES;
   }
   /* send request per each udp port */
   for ( i = 0; gUdpPort[i] != 0; i++ )
   {
      LOG((" nattrvrsltestStuncBindingReq: Sending Binding Request for port %d", gUdpPort[i]));
      if (STUNC_SUCCESS != stuncPublicAddr( sidFromSoc(&stunUdpSocket[i]), 
                                            &dest, 
                                            &secret,
                                            0,
                                            &reqHdl,
                                            stuncEventHandler,
                                            gStunMaxWait 
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                                            ,pEvt
#endif
                                            ))
      {
         LOG((" nattrvrsltestStuncBindingReq: Failed to issue req to stack"));
      }
      else
      {
         LOG((" nattrvrsltestStuncBindingReq: req passed to stack"));

         if ( NULL != pEvt )
         {
            if ( pEvt->evt == STUNC_EVT_MAPPED_ADDRESS )
            {
               char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];

               STUN_TRANSPORT *mappedAddrp = (STUN_TRANSPORT *)&pEvt->data.socAddr;
               bosIpAddrGetStr(&mappedAddrp->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);

               LOG((" nattrvrsltestStuncBindingReq: Mapped IP Address %s, port %d", ipAddrStr, mappedAddrp->port));
            }
            else
            {
               LOG((" nattrvrsltestStuncBindingReq: Failed to get Mapped IP Address. evt %d", pEvt->evt));
            }
         }
      }
   }
}

/*****************************************************************************
*  FUNCTION:
*     stuncPacketSender
*
*  PURPOSE:
*     send packet to network
*
*  PARAMETERS:
*     sid     [IN] - socket id
*     pDest   [IN] - destination IP and port
*     pData   [IN] - point to data to be sent
*     len     [IN] - length of data in number of bytes
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static void stuncPacketSender( const BOS_UINT32      sid,
                               const STUN_TRANSPORT  *pDest,
                               const BOS_UINT8       *pData,
                               const BOS_UINT16      len )
{
   BOS_STATUS status;
   unsigned int sLen = 0; /* actual sent length */
   BOS_SOCKET *pSid = pSocFromSid( sid );

   LOG(("stuncPacketSender: "));

   BOS_ASSERT( (NULL != pSid) && 
               (NULL != pDest) &&
               (NULL != pData) && 
               len > 0);

   /* send the request to network */
   if ( pSid == &stunTlsSocket )
   {
      /* send to TLS socket */
      status = bosSocketSend( pSid,
                              pData,
                              len,
                              0,
                              &sLen );
      if ( BOS_STATUS_OK == status && ((BOS_UINT32)len == sLen))
      {
         LOG(("stuncPacketSender: sending Successful"));
      }
      else
      {
         LOG(("stuncPacketSender: sending Failed"));
      }
   }
   else
   {
      /* send to UDP socket */
      status = bosSocketSendToIp( pSid,
                                  pData,
                                  len,
                                  0,
                                  (BOS_IP_ADDRESS *)&pDest->ipAddr,
                                  pDest->port,
                                  0,
                                  &sLen );
      if ( BOS_STATUS_OK == status && ((BOS_UINT32)len == sLen))
      {
         LOG(("stuncPacketSender: sending Successful"));
      }
      else
      {
         LOG(("stuncPacketSender: sending Failed"));
      }

   }

}

/*****************************************************************************
*  FUNCTION:
*     stuncEventHandler
*
*  PURPOSE:
*     Handler STUNC events
*
*  PARAMETERS:
*     evt     [IN] - evt type
*     pData   [IN] - event data
*     reqHdl  [IN] - request handle
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static void stuncEventHandler( const BOS_UINT32       reqHdl,
                               const STUNC_EVT_DATA   *pEvtData )
{
   BOS_ASSERT( NULL != pEvtData );
   BOS_ASSERT( pEvtData->evt < STUNC_EVT_NUM );
   LOG(("stuncEventHandler: evt [%s] reqHdl [%d]", stuncEvtMap[pEvtData->evt].str, reqHdl));

   switch( pEvtData->evt )
   {
      case STUNC_EVT_MAPPED_ADDRESS:
      {
         char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
         STUN_TRANSPORT *mappedAddrp = (STUN_TRANSPORT *)&pEvtData->data.socAddr;
         (void)bosIpAddrGetStr(&mappedAddrp->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);

         LOG(("Mapped IP Address %s, port %d", ipAddrStr, mappedAddrp->port));
      }
      break;
      case STUNC_EVT_SHARED_SECRET:
      {
         STUNC_SHARED_SECRET *pSs = (STUNC_SHARED_SECRET *)&pEvtData->data.sharedSecret;
         /* extract username and password */
         LOG(("Username = [%.*s], Password = [%.*s]", pSs->username.size, pSs->username.buf, pSs->password.size, pSs->password.buf ));
         /* cache the username and password */
         memcpy( secret.username.buf, pSs->username.buf, pSs->username.size );
         secret.username.size = pSs->username.size;
         memcpy( secret.password.buf, pSs->password.buf, pSs->password.size );
         secret.password.size = pSs->password.size;
      }
      break;
      case STUNC_EVT_RES_ERROR:
      {
         STUNC_ERR *errp = (STUNC_ERR *)&pEvtData->data.err;
         LOG(("Error number [%d],Phrase [%.*s]", errp->errnoa, errp->reason.size, errp->reason.buf ));
      }
      break;
      case STUNC_EVT_TX_TIMEOUT:
      {
      }
      break;
      case STUNC_EVT_SYSTEM_ERR:
      {
         /* no further action */
      }
      break;
      default:
      {
         /* unexpected event */
         BOS_ASSERT( BOS_FALSE );
      }
   }
}

/*****************************************************************************
*  FUNCTION:
*     hasher
*
*  PURPOSE:
*     HMAC hashing routine
*
*  PARAMETERS:
*              alg    [IN] - hmac algorithm
*              pKey   [IN] - hash key, keylen
*              pData  [IN] - data value and length
*              pHmac  [MOD] - the hashed data
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static void hasher( const STUN_HMAC_TYPE  alg, 
                    const STUN_DATA       *pKey,
                    const STUN_DATA       *pData,
                    STUN_DATA             *pHmac )
{
   BOS_ASSERT( alg == STUN_HMAC_SHA1 );
   hmac_sha1( (const unsigned char *)pKey->pData, 
               pKey->len, 
               (const unsigned char *)pData->pData, 
               pData->len, 
               (unsigned char *)pHmac->pData, 
               20 );
   pHmac->len = 20;
}

/*****************************************************************************
*  FUNCTION:
*     checksum32
*
*  PURPOSE:
*     CRC32 routine
*
*  PARAMETERS:
*              alg    [IN] - hmac algorithm
*              pKey   [IN] - hash key, keylen
*              pData  [IN] - data value and length
*              pHmac  [MOD] - the hashed data
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static BOS_UINT32 checksum32( const BOS_UINT8   *pData,
                              const BOS_UINT16  len )
{
   return( (BOS_UINT32)generate_crc32c((unsigned char *)pData, (unsigned int) len ));
}

/*****************************************************************************
*
*  FUNCTION:   rxThread
*
*  PURPOSE:    This task listens for data from sockets.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void rxThread( void )
{
   int i;
   BOS_SOCKET_SET udpsockSet;
   BOS_STATUS status;
   int num = 0;
   int maxSocket;
   BOS_UINT8 buf[MAX_BUF_SIZE];
   BOS_IP_ADDRESS srcAddr;
   BOS_SOCKET_IP_PORT srcPort; 
   STUN_TRANSPORT lclTransport;
   STUN_TRANSPORT srcTransport;
   unsigned int len = 0;
   BOS_SOCKET *soc;
   BOS_UINT16 port;
   BOS_BOOL rc;
   BOS_BOOL bFound;

   memset( (void*)&lclTransport, 0, sizeof(STUN_TRANSPORT) );
   status = bosIpAddrCreateFromStr( gLocalHost, &lclTransport.ipAddr );
   BOS_ASSERT( BOS_STATUS_OK == status );

   for ( ;; )
   {
      memset( (void*)&srcTransport, 0, sizeof(STUN_TRANSPORT) );
      memset( (void*)&srcAddr, 0, sizeof(BOS_IP_ADDRESS) );
      memset( (void*)&srcPort, 0, sizeof(BOS_SOCKET_IP_PORT) );
      
      bFound = BOS_FALSE;
      maxSocket = 0;
      bosSocketSetZero( &udpsockSet );
      for ( i = 0; gUdpPort[i] != 0; i++ )
      {
         bosSocketSetAddSocket( &stunUdpSocket[i], &udpsockSet );
      }
      maxSocket = i;
      for ( i = 0; i < 3; i++ )
      {
         if ( gIceBRtp[i] && gIceRtpPort[i] )
         {
            bosSocketSetAddSocket( &iceRtpSocket[i], &udpsockSet );
            maxSocket++;
         }
         if ( gIceBRtcp[i] && gIceRtcpPort[i] )
         {
            bosSocketSetAddSocket( &iceRtcpSocket[i], &udpsockSet );
            maxSocket++;
         }
      }

      if ( maxSocket == 0 )
      {
         bosSleep( 100 );
         continue;
      }

      status = bosSocketSelect( maxSocket, &udpsockSet, NULL, NULL, BOS_WAIT_FOREVER, &num );
      switch( status )
      {
         case BOS_STATUS_OK:
         {
            /* scan for readable sockets */
            for ( i = 0; !bFound && gUdpPort[i] != 0; i++ )
            {
               if ( bosSocketSetIsSocketSet( &stunUdpSocket[i], &udpsockSet ) )
               {
                  status = bosSocketRecvFromIp( &stunUdpSocket[i],
                                                (void*)buf,
                                                MAX_BUF_SIZE,
                                                0,
                                                &srcAddr,
                                                &srcPort,
                                                &len);
                  if ( BOS_STATUS_OK != status )
                  {
                     LOG(("rxThread: recv error %d", status));
                  }
                  else
                  {
                     LOG(("rxThread: send packet size %d to STUNC stack", len));
                     srcTransport.ipAddr = srcAddr;
                     srcTransport.port = srcPort;
                     rc = stunIsStunMsg( buf, (const BOS_UINT16)len, STUN_INSP_MAGIC_COOKIE );
                     BOS_ASSERT( BOS_TRUE == rc );
                     lclTransport.port = (BOS_SOCKET_IP_PORT)gUdpPort[i];
                     stunRecvPacket( sidFromSoc(&stunUdpSocket[i]),
                                     &lclTransport,
                                     &srcTransport,
                                     buf,
                                     (const BOS_UINT16)len);
                     bFound = BOS_TRUE;
                     break;
                  }
               }
            }

            for ( i = 0; !bFound && i < 3; i++ )
            {
               soc = NULL;
               port = (BOS_UINT16)-1;
               if ( bosSocketSetIsSocketSet( &iceRtpSocket[i], &udpsockSet ))
               {
                  soc = &iceRtpSocket[i];
                  port = (BOS_UINT16)gIceRtpPort[i];
               }
               else if ( bosSocketSetIsSocketSet( &iceRtcpSocket[i], &udpsockSet ))
               {
                  soc = &iceRtcpSocket[i];
                  port = (BOS_UINT16)gIceRtcpPort[i];
               }

               if ( NULL != soc )
               {
                  status = bosSocketRecvFromIp( soc,
                                                (void*)buf,
                                                MAX_BUF_SIZE,
                                                0,
                                                &srcAddr,
                                                &srcPort,
                                                &len);
                  if ( BOS_STATUS_OK != status )
                  {
                     LOG(("rxThread: recv error %d", status));
                  }
                  else
                  {
                     LOG(("rxThread: send packet size %d to STUNC stack", len));
                     memcpy( (void*)&srcTransport.ipAddr, (void*)&srcAddr, sizeof(BOS_IP_ADDRESS) );
                     srcTransport.port = srcPort;                     
                     rc = stunIsStunMsg( buf, (const BOS_UINT16)len, STUN_INSP_MAGIC_COOKIE );
                     BOS_ASSERT( BOS_TRUE == rc );
                     lclTransport.port = port;
                     stunRecvPacket( sidFromSoc(soc),
                                     &lclTransport,
                                     &srcTransport,
                                     buf,
                                     (const BOS_UINT16)len);
                     bFound = BOS_TRUE;
                     break;
                  }
               }
            }
         }
         break;
         case BOS_STATUS_TIMEOUT:
         {
            BOS_ASSERT( BOS_FALSE );
         }
         break;
         default:
         {
            LOG(("rxThread: select error %d", status));
         }
      }
   }
}

/*****************************************************************************
*  FUNCTION:
*     turncEventHandler
*
*  PURPOSE:
*     Handler TURNC events
*
*  PARAMETERS:
*     reqHdl      [IN] - request handle
*     pEvtData    [IN] - event data
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static void turncEventHandler( const BOS_UINT32       reqHdl,
                               const TURNC_EVT_DATA   *pEvtData )
{
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];

   BOS_ASSERT( NULL != pEvtData );
   BOS_ASSERT( pEvtData->evt < TURNC_EVT_NUM );
   LOG(("turncEventHandler: evt [%s] reqHdl [%d]", turncEvtMap[pEvtData->evt].str, reqHdl));
   
   switch( pEvtData->evt )
   {
      case TURNC_EVT_ALLOCATE_SUCCESS:
      {
         STUN_TRANSPORT *pTransport;
         pTransport = (STUN_TRANSPORT*)&pEvtData->data.allocAddrs.relayAddr;
         (void)bosIpAddrGetStr(&pTransport->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
         LOG(("Relay IP Address %s, port %d", ipAddrStr, pTransport->port));
         pTransport = (STUN_TRANSPORT *)&pEvtData->data.allocAddrs.reflexiveAddr;
         (void)bosIpAddrGetStr(&pTransport->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
         LOG(("Reflexive IP Address %s, port %d", ipAddrStr, pTransport->port));
      }
      break;
      case TURNC_EVT_ACTIVE_DEST_SET:
      case TURNC_EVT_ACTIVE_DEST_UNSET:
      {
         return;
      }
      break;
      case TURNC_EVT_CONNECT_SUCCESS:
      {
         /* nothing to display */
      }
      break;
      case TURNC_EVT_CONNECT_STATUS:
      {
         /* nothing to display */
      }
      break;
      case TURNC_EVT_ALLOCATE_ERROR:
      case TURNC_EVT_ACTIVE_DEST_ERROR:
      case TURNC_EVT_CONNECT_ERROR:
      {
         TURNC_ERR *pErr = (TURNC_ERR *)&pEvtData->data.err;
         LOG(("Error number [%d],Phrase [%.*s]", pErr->errnoa, pErr->reason.len, pErr->reason.buf ));
      }
      break;
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }
   return;
}

/*****************************************************************************
*  FUNCTION:
*     turncSendData
*
*  PURPOSE:
*     Callback function for sending TURN Data as a result of 
*     Send Indication Request.
*
*  PARAMETERS:
*     sid     [IN] - socket id
*     pDest   [IN] - destination IP and port
*     pData   [IN] - pointer to sending data
*     len     [IN] - length of data in number of bytes
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static void turncSendData( const BOS_UINT32        sid,
                           const STUN_TRANSPORT    *pDest,
                           const BOS_UINT8         *pData,
                           const BOS_UINT16        len)
{
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];

   (void)bosIpAddrGetStr(&pDest->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);

   LOG(("turncSendData: *sid [%d] dest IP[%s] port[%d] *pData[0]=%d len[%d]", sid, ipAddrStr, pDest->port, *pData, len ));

   return;
}

/*****************************************************************************
*  FUNCTION:
*     turncRecvData
*
*  PURPOSE:
*     Callback function for Receiving TURN Data as a result of 
*     Data Indication from server.
*
*  PARAMETERS:
*     sid     [IN] - socket id
*     pDest   [IN] - source IP and port
*     pData   [IN] - pointer to received data
*     len     [IN] - length of data in number of bytes
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static void turncRecvData( const BOS_UINT32        sid,
                           const STUN_TRANSPORT    *pSrc,
                           const BOS_UINT8         *pData,
                           const BOS_UINT16        len)
{
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];

   (void)bosIpAddrGetStr(&pSrc->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);

   LOG(("turncRecvData: *sid [%d] source IP[%s] port[%d] *pData[0]=%d len[%d]", sid, ipAddrStr, pSrc->port, *pData, len ));

   return;
}

/*****************************************************************************
*  FUNCTION:
*      nattrvrsltestAllocateReq
*
*  PURPOSE:
*     Sending Allocate Request
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void  nattrvrsltestTurncAllocateReq( void )
{
   BOS_UINT32 reqHdl;
   STUN_TRANSPORT dest;
   BOS_SOCKET *pSoc;
   TURNC_ALLOCATE_OPTS  opts;

   if ( BOS_TRUE != getServer( &pSoc, &dest.ipAddr, &dest.port ) )
   {
      LOG(( " nattrvrsltestTurncAllocateReq: ERROR."));
      return;
   }

   fillAllocateOpts( &opts );

   if ( TURNC_SUCCESS != turncAllocate( sidFromSoc(pSoc),
                                        &dest,
                                        pSecret,
                                        &opts,
                                        &reqHdl) )
   {
      LOG((" nattrvrsltestTurncAllocateReq: Failed to issue req to stack"));
   }
   else
   {
      LOG((" nattrvrsltestTurncAllocateReq: req passed to stack"));
   }
}

/*****************************************************************************
*  FUNCTION:
*      nattrvrsltestActiveDestReq
*
*  PURPOSE:
*     Sending Active Destination Request
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void  nattrvrsltestTurncActiveDestReq( void )
{
   BOS_UINT32 reqHdl;
   STUN_TRANSPORT dest;
   BOS_SOCKET *pSoc;
   STUN_TRANSPORT remote;
   BOS_STATUS status;
   STUN_TRANSPORT *pTransport = NULL;

   if ( BOS_TRUE != getServer( &pSoc, &dest.ipAddr, &dest.port ) )
   {
      LOG(( " nattrvrsltestTurncActiveDestReq: ERROR."));
      return;
   }

   status = bosIpAddrCreateFromStr( gActiveDestIPAddr, &remote.ipAddr );
   if ( BOS_STATUS_OK == status )
   {
      remote.port = (BOS_SOCKET_IP_PORT)gActiveDestPort;
      pTransport = &remote;
   }
   else
   {
      LOG((" nattrvrsltestTurncActiveDestReq: Invalid Remote Address"));
      return;
   }

   if ( TURNC_SUCCESS != turncActiveDestination( sidFromSoc(pSoc),
                                                 &dest,
                                                 pSecret,
                                                 pTransport,
                                                 &reqHdl ) )
   {
      LOG((" nattrvrsltestTurncActiveDestReq: Failed to issue req to stack"));
   }
   else
   {
      LOG((" nattrvrsltestTurncActiveDestReq: req passed to stack"));
   }

}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestConnectReq
*
*  PURPOSE:
*     Sending Connect Request
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void  nattrvrsltestTurncConnectReq( void )
{
   BOS_STATUS status;
   BOS_UINT32 reqHdl;
   STUN_TRANSPORT dest;
   BOS_SOCKET *pSoc;
   STUN_TRANSPORT remote;
   STUN_TRANSPORT *pTransport = NULL;

   if ( BOS_TRUE != getServer( &pSoc, &dest.ipAddr, &dest.port ) )
   {
      LOG(( "nattrvrsltestTurncConnectReq: ERROR."));
      return;
   }

   status = bosIpAddrCreateFromStr( gCnxRemoteIPAddr, &remote.ipAddr );
   if ( BOS_STATUS_OK == status )
   {
      remote.port = (BOS_SOCKET_IP_PORT)gCnxRemotePort;
      pTransport = &remote;
   }
   else
   {
      LOG(("nattrvrsltestTurncConnectReq: Invalid Remote Address"));
      return;
   }

   if ( TURNC_SUCCESS != turncConnect( sidFromSoc(pSoc),
                                       &dest,
                                       pSecret,
                                       pTransport,
                                       &reqHdl) )
   {
      LOG(("nattrvrsltestTurncConnectReq: Failed to issue req to stack"));
   }
   else
   {
      LOG(("nattrvrsltestTurncConnectReq: req passed to stack"));
   }
}

/*****************************************************************************
*
*  FUNCTION:   nattrvrsltestTurncSendDataReq
*
*  PURPOSE:    Send Data through a Relay server.
*              If an Active Destination has been set, data will be
*              send directly to the destination through the relay
*              server without further encapsulation.
*              Otherwise, Send Indication and Remote Address
*              attributes will be used to encapsulate the data
*              sent to the relay server.
*
*  PARAMETERS:
*           none
*
*  RETURNS:    TURNC_SUCCESS if sending is successful, TURNC_FAIL otherwise.
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestTurncSendDataReq( void )
{
   BOS_STATUS status;
   STUN_TRANSPORT remote;
   STUN_TRANSPORT *pTransport = NULL;
   STUN_TRANSPORT dest;
   BOS_SOCKET *pSoc;

   if ( BOS_TRUE != getServer( &pSoc, &dest.ipAddr, &dest.port ) )
   {
      LOG(( "nattrvrsltestTurncSendDataReq: ERROR."));
      return;
   }

   status = bosIpAddrCreateFromStr( gSendDataRemoteIPAddr, &remote.ipAddr );
   if ( BOS_STATUS_OK == status )
   {
      remote.port = (BOS_SOCKET_IP_PORT)gSendDataRemotePort;
      pTransport = &remote;
   }
   else
   {
      LOG(( "nattrvrsltestTurncSendDataReq: Invalid remote destination."));
      return;
   }

   if ( TURNC_SUCCESS != turncSendData( sidFromSoc(pSoc),
                                        &dest,
                                        pTransport,
                                        (BOS_UINT8 *)testPayload,
                                        (const BOS_UINT16)strlen(testPayload) ) )
   {
      LOG(("nattrvrsltestTurncSendDataReq: Failed to issue req to stack"));
   }
   else
   {
      LOG(("nattrvrsltestTurncSendDataReq: req passed to stack"));
   }

}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestTurncDeallocate
*
*  PURPOSE:
*     Sending Allocate Request with 0 lifetime.
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestTurncDeallocate( void )
{
   TURNC_STATUS rc;

   rc = turncDeallocate( (BOS_UINT32)gAllocateHdl );
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestTurncReceiveData
*
*  PURPOSE:
*     Receive canned Data and pass it to the stack.
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestTurncReceiveData( void )
{
   BOS_STATUS status;
   BOS_BOOL rc;
   BOS_IP_ADDRESS localAddr;
   STUN_TRANSPORT transport;
   STUN_TRANSPORT src;
   BOS_SOCKET *pSoc = getOrgSocket( gTurnOrgTransport );

   src.ipAddr = turnServerIpAddr;
   src.port = (BOS_SOCKET_IP_PORT)gTurnPort;

   rc = stunIsStunMsg( recvPayload, sizeof(recvPayload), STUN_INSP_MAGIC_COOKIE );
   BOS_ASSERT( BOS_TRUE == rc );

   status = bosIpAddrCreateFromStr( gLocalHost, &localAddr );
   BOS_ASSERT( BOS_STATUS_OK == status );

   transport.ipAddr = localAddr;
   transport.port = (BOS_SOCKET_IP_PORT)gTurnUdpPort;

   stunRecvPacket( sidFromSoc(pSoc), &transport, &src, recvPayload, sizeof(recvPayload));
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestTurncConnectStatusInd
*
*  PURPOSE:
*     Receive Connect Status Indication
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestTurncConnectStatusInd( void )
{
   BOS_STATUS status;
   BOS_BOOL rc;
   BOS_IP_ADDRESS localAddr;
   STUN_TRANSPORT transport;
   STUN_TRANSPORT src;

   BOS_SOCKET *pSoc = getOrgSocket( gTurnOrgTransport );

   src.ipAddr = turnServerIpAddr;
   src.port = (BOS_SOCKET_IP_PORT)gTurnPort;

   rc = stunIsStunMsg( cnxStatus, sizeof(cnxStatus), STUN_INSP_MAGIC_COOKIE );
   BOS_ASSERT( BOS_TRUE == rc );

   status = bosIpAddrCreateFromStr( gLocalHost, &localAddr );
   BOS_ASSERT( BOS_STATUS_OK == status );

   transport.ipAddr = localAddr;
   transport.port = (BOS_SOCKET_IP_PORT)gTurnTcpPort;

   stunRecvPacket( sidFromSoc(pSoc), &transport, &src, cnxStatus, sizeof(cnxStatus));
}

/*****************************************************************************
*  FUNCTION:
*     getOrgSocket
*
*  PURPOSE:
*     get the originating Socket
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static BOS_SOCKET *getOrgSocket( int transport )
{
   BOS_SOCKET *pSoc = NULL;
   switch( transport )
   {
      case 0:
         pSoc = &turnUdpSocket;
         break;
      case 1:
         pSoc = &turnTcpSocket;
         break;
      case 2:
         pSoc = &turnTlsSocket;
         break;
      default:
         BOS_ASSERT( FALSE );
   }
   return ( pSoc );
}

/*****************************************************************************
*  FUNCTION:
*     fillAllocateOpts
*
*  PURPOSE:
*     Fill Allocate request options
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static void fillAllocateOpts( TURNC_ALLOCATE_OPTS *opts )
{
   BOS_STATUS status;
   static STUN_TRANSPORT reqdTransportAddr;
   BOS_ASSERT( NULL != opts );

   if ( gBandwidth >= 0 )
   {
      opts->pBandwidth = (BOS_UINT32*)&gBandwidth;
   }
   else
   {
      opts->pBandwidth = NULL;
   }

   if ( gLifetime >=0 )
   {
      opts->pLifetime = (BOS_UINT32 *)&gLifetime;
   }
   else
   {
      opts->pLifetime = NULL;
   }
   
   if ( gReqdTransport >=0 )
   {
      opts->pReqdTransport = (BOS_UINT32 *)&gReqdTransport;
   }
   else
   {
      opts->pReqdTransport = NULL;
   }

   status = bosIpAddrCreateFromStr( gReqdIPAddr, &reqdTransportAddr.ipAddr );
   if ( BOS_STATUS_OK == status )
   {
      reqdTransportAddr.port = (BOS_SOCKET_IP_PORT)gReqdPort;
      opts->pReqdIPAddr = &reqdTransportAddr;
   }
   else
   {
      opts->pReqdIPAddr = NULL;
   }

   if ( gPortProps >=0 )
   {
      opts->pPortProps = (BOS_UINT32 *)&gPortProps;
   }
   else
   {
      opts->pPortProps = NULL;
   }
}

/*****************************************************************************
*  FUNCTION:
*     getServer
*
*  PURPOSE:
*     Fill Allocate request options
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL getServer( BOS_SOCKET **ppSoc, BOS_IP_ADDRESS *pIpAddr, BOS_SOCKET_IP_PORT *pPort )
{
   if ( !bStarted )
   {
      LOG(( "getServer: TURNC client not started."));
      return BOS_FALSE;
   }

   *pIpAddr = turnServerIpAddr;
   *pPort = (BOS_SOCKET_IP_PORT)gTurnPort;

   *ppSoc = getOrgSocket( gTurnOrgTransport );
   
   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:
*     iceEventHandler
*
*  PURPOSE:
*     Handler ICE events
*
*  PARAMETERS:
*     evt     [IN] - evt type
*     pData   [IN] - event data
*     reqHdl  [IN] - request handle
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
static void iceEventHandler( const ICE_EVT      evt,
                             const void         *pData,
                             const BOS_UINT32   reqHdl )
{
   BOS_ASSERT( evt < ICE_EVT_NUM );
   LOG(("iceEventHandler: evt [%s] reqHdl [%d]", iceEvtMapStr[evt].str, reqHdl));

   switch( evt )
   {
      case ICE_EVT_CANDIDATES_GATHER_COMPLETE:
      case ICE_EVT_CANDIDATES_GATHER_TIMEOUT:
      case ICE_EVT_CONCLUDED_SUCCESS:
      case ICE_EVT_CONCLUDED_FAILURE:
      {
         (void)pData;
      }
      break;
      default:
      {
         /* unexpected event */
         BOS_ASSERT( BOS_FALSE );
      }
   }
}

// ICE TESTS
//
//
/*****************************************************************************
*  FUNCTION:
*     icetestCreateAgent
*
*  PURPOSE:
*     Create an Ice Agent
*
*  PARAMETERS:
*        none.
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestIcetestCreateAgent( void )
{
   int i;
   ICE_PREF pref;
   ICE_PREF *pPref = NULL;
   ICE_STUN_SERVER_LIST serverList;
   /* look up server IP */
   if ( strlen( gStunHost ) )
   {
      BOS_IP_ADDRESS ipAddr;
      BOS_SOCKET_IP_PORT ipPort;
      char errStr[MAX_STR];

      /* WARNING: this routine returns host byte ordering in 
       * ipAddr and ipPort 
       */
      if (BOS_STATUS_OK == bosSocketLookupServerAndPort( gStunHost,
                                                         &ipAddr,
                                                         &ipPort,
                                                         errStr,
                                                         MAX_STR ))
      {
         serverList.num = 1;
         serverList.server[0].id = 0; 
         serverList.server[0].type = ICE_SRV_STUN; 
         serverList.server[0].socAddr.port = (BOS_SOCKET_IP_PORT)gStunPort; 
         serverList.server[0].socAddr.ipAddr = ipAddr; 
      }
      else
      {
         serverList.num = 0;
         LOG(("nattrvrsltestStart: STUN Server lookup failed err %s, request not sent", errStr));
      }
   }
   else
   {
      LOG(("nattrvrsltestStart: invalid STUN Server, request not sent"));
      return;
   }

   if ( gIcePrefOverride )
   {
      pref.type.host = (BOS_UINT8)gIcePrefHost;
      pref.type.srflx = (BOS_UINT8)gIcePrefSrflx;
      pref.type.prflx = (BOS_UINT8)gIcePrefPrflx;
      pref.type.relayed = (BOS_UINT8)gIcePrefRelay;
      pref.local = (BOS_UINT16)gIcePrefLocal;
      pPref = &pref;
   }

   LOG(( "nattrvrsltestIcetestCreateAgent: Creating Agent(s).." ));
   for ( i = 0; i < 3; i++ )
   {
      agentId[i] = ICE_UNKNOWN;
      if ( gIceBRtp[i] || gIceBRtcp[i] )
      {
         ICE_AGENT_OPTIONS options;
         options.mode = gIceLiteMode? ICE_LITE : ICE_FULL;
         options.nom = gIceAggressiveNom? ICE_AGGRESSIVE : ICE_REGULAR;
         options.pPrefOverride = pPref;
         if ( ICE_SUCCESS == iceCreateAgent( &serverList, &options, &agentId[i] ) )
         {
            LOG(( "--->SUCCESSFUL - Agent Id = [%d]", agentId[i] ));
         }
         else
         {
            LOG(( "--->FAILED" ));
         }
      }
   }
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestIceDeleteAgent
*
*  PURPOSE:
*     Delete an Ice agent.
*
*  PARAMETERS:
*        none.
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestIceDeleteAgent( void )
{
   LOG(( "nattrvrsltestIceDeleteAgent: Deleting Agent.." ));

   if ( ICE_SUCCESS != iceDeleteAgent( gIceAgentId ) )
   {
      LOG(( "--->FAILED to delete Agent [%d]", gIceAgentId ));
   }
   else
   {
      LOG(( "--->SUCCESS... agent[%d] deleted", gIceAgentId ));
   }
}

//
/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestIceSetPortParms
*
*  PURPOSE:
*     Set Media Port Parmameters
*
*  PARAMETERS:
*        none.
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestIceSetPortParms( void )
{
   int i;
   ICE_PORT_PARMS portParms[3];
   ICE_PORT_PARMS *pPortParms[3];

   for ( i = 0; i < 3; i++ )
   {
      if ( gIceBRtp[i] )
      {
         portParms[0].component = ICE_COMP_RTP;
         portParms[0].localIfId = 0;
         portParms[0].stunServerId = 0;
         portParms[0].transType = ICE_TRANS_UDP;
         portParms[0].pSecret = &secret;
         portParms[0].sid = sidFromSoc(&iceRtpSocket[i]);
         portParms[0].port = (BOS_UINT16)gIceRtpPort[i];
         portParms[0].pSecret = &secret;
         pPortParms[0] = &portParms[0];

         if ( gIceBRtcp[i] )
         {
            portParms[1].component = ICE_COMP_RTCP;
            portParms[1].localIfId = 0;
            portParms[1].stunServerId = 0;
            portParms[1].transType = ICE_TRANS_UDP;
            portParms[1].pSecret = &secret;
            portParms[1].sid = sidFromSoc(&iceRtcpSocket[i]);
            portParms[1].port = (BOS_UINT16)gIceRtcpPort[i];
            portParms[1].pSecret = NULL;
            pPortParms[1] = &portParms[1];
         }
         else
         {
            pPortParms[1] = NULL;
         }

         pPortParms[2] = NULL;


         BOS_ASSERT(ICE_UNKNOWN != agentId[i]);
         LOG(( "nattrvrsltestIceSetPortParms: for Agent Id[%d]...", agentId[i]));
         if ( ICE_SUCCESS == iceSetPortParms( agentId[i], (const ICE_PORT_PARMS **)pPortParms ) )
         {
            LOG(( "--->Port Parms Set..."));
         }
         else
         {
            LOG(( "--->Failed to set Port Parms"));
         }
      }
   }
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestIceGatherCndts
*
*  PURPOSE:
*     Gather Candidates test.
*
*  PARAMETERS:
*        none.
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestIceGatherCndts( void )
{
   int i;
   for ( i = 0; i < 3; i++ )
   {
      if (ICE_UNKNOWN != agentId[i] )
      {
         LOG(( "nattrvrsltestIceGatherCndts: Gathering Candidate for Agent Id[%d]...", agentId[i]));
         if ( ICE_SUCCESS == iceGatherCandidates( agentId[i],
                                                  (BOS_UINT16)gIceMaxWait ))
         {
            LOG(( "--->Gathering Candidates..."));
         }
         else
         {
            LOG(( "--->Failed to Gather candidates"));
         }
      }
   }
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestIceAgentDump
*
*  PURPOSE:
*     Dump all candidate lists and contents for an agent
*
*  PARAMETERS:
*        none.
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestIceAgentDump( void )
{
#if ICEDBG_LOG
   LOG(( "nattrvrsltestIceAgentDump: Candidate Lists"));
   dumpCndtLstAll( gIceAgentId );

   LOG(( "nattrvrsltestIceAgentDump: Check List"));
   dumpChkLst( gIceAgentId );
#else
   LOG(( "nattrvrsltestIceAgentDump: Not Supported"));
#endif

}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestIceSdpEncode
*
*  PURPOSE:
*     Encode CndtList into SDP Alines.
*
*  PARAMETERS:
*        none.
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void nattrvrsltestIceSdpEncode( void )
{
   ICE_SDP_PARMS sdpParms;
   int i;
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];

   LOG(( "nattrvrsltestIceSdpEncode for Agent [%d]", gIceSdpAgentId ));

   if ( ICE_SUCCESS == iceProcessLocalSdp( gIceSdpAgentId,
                                           (gIceSdpRole==0?
                                                ICE_SDP_OFFERER:
                                                ICE_SDP_ANSWERER), 
                                           &sdpParms ))
   {
      LOG(( "--->SDP Encoded..."));
      LOG(("--ufrag = [%s]", sdpParms.credential.ufrag));
      LOG(("--pwd   = [%s]", sdpParms.credential.pwd));
      bosIpAddrGetStr(&sdpParms.addr.ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
      LOG(("c-line IP = [%s], port = [%d]", ipAddrStr, sdpParms.addr.port ));
      LOG(("rtcp Port %d", sdpParms.rtcpPort));
      for ( i = 0; i < ICE_CNDT_ALINE_NUM_MAX; i++ )
      {
         if ( '\0' == sdpParms.aline[i][0] )
         {
            break;
         }
         LOG(( "-- aline [%d] --", i));
         LOG(( "%s", sdpParms.aline[i] ));
      }
   }
   else
   {
      LOG(( "--->Failed to encode SDP"));
   }
}

/*****************************************************************************
*  FUNCTION:
*     nattrvrsltestIceSdpParse
*
*  PURPOSE:
*     Parse SDP alines to remote CndtList.
*
*  PARAMETERS:
*        none.
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
#define REMOTE_ICE_MODE    ""
void nattrvrsltestIceSdpParse( void )
{
   ICE_SDP_PARMS sdpParms;

   memset( &sdpParms, 0, sizeof( ICE_SDP_PARMS ) );
   LOG(( "nattrvrsltestIceSdpParse for Agent [%d]", gIceRSdpAgentId ));

   strcpy(sdpParms.iceMode, REMOTE_ICE_MODE);
   (void)bosIpAddrCreateFromStr( "10.136.34.17", &sdpParms.addr.ipAddr );
   sdpParms.addr.port = 0x2233;
   sdpParms.rtcpPort = 7000;
   strcpy(sdpParms.credential.ufrag, "u+f+");
   strcpy(sdpParms.credential.pwd, "p++++++w++++++d+++++++");
   strcpy(sdpParms.aline[0], gSdpLine1);
   strcpy(sdpParms.aline[1], gSdpLine2);
   strcpy(sdpParms.aline[2], gSdpLine3);
   strcpy(sdpParms.aline[3], gSdpLine4);

   if ( ICE_SUCCESS == iceProcessRemoteSdp( gIceRSdpAgentId,
                                            (gIceRSdpRole==0?
                                                 ICE_SDP_OFFERER:
                                                 ICE_SDP_ANSWERER), 
                                            &sdpParms ))
   {
      LOG(( "--->SDP Parsed."));
   }
   else
   {
      LOG(( "--->Failed to parse SDP"));
   }
}

void nattrvrsltestIceDLogSet( void )
{
   gIcedbg_RuntimeDLog ^= 1;
   LOG(( "DLOG %d", gIcedbg_RuntimeDLog ));
}

static BOS_UINT32 addSocToList( BOS_SOCKET *pSoc )
{
   int i;
   BOS_ASSERT( NULL != pSoc );
   for ( i = 0; i < MAX_SOCKET_NUM; i++ )
   {
      if ( socHdlList[i].sid == 0 )
      {
         socHdlList[i].pSoc = pSoc;
         return (socHdlList[i].sid = i+1);
      }
   }
   BOS_ASSERT( BOS_FALSE );
   return( 0 );
}

static BOS_SOCKET *pSocFromSid( BOS_UINT32 sid )
{
   BOS_ASSERT( sid > 0 && sid <= MAX_SOCKET_NUM );
   return( socHdlList[sid-1].pSoc );
}

static BOS_UINT32 sidFromSoc( BOS_SOCKET *pSoc )
{
   int i;
   BOS_ASSERT( NULL != pSoc );
   for ( i = 0; i < MAX_SOCKET_NUM; i++ )
   {
      if ( pSoc == socHdlList[i].pSoc )
      {
         return( socHdlList[i].sid );
      }
   }
   BOS_ASSERT( BOS_FALSE );
   return( 0 );
}
