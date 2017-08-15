/****************************************************************************
*
*  Copyright (c) 2002-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
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
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
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
*  Filename: bosSocketGeneric.c
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    bosSocketGeneric.c
*
*  @brief   Contains OS independant definitions for the BOS Socket module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_SOCKET
#if !BOS_OS_LINUXKERNEL

#include <blog.h>
#include <bosLog.h>

#include <bosSocket.h>
#include <bosError.h>

#include <bosSocketPrivate.h>

#include <blog.h>
#include <bosLog.h>



#if BOS_CFG_RESET_SUPPORT
#include <bosTask.h>
#endif

#if BOS_CFG_IP_ADDRESSS
#include <bosIpAddr.h>
#endif

#if BOS_CFG_TIME_FOR_TIMED_FUNCTION
#  if BOS_CFG_TIME
#     include <bosTime.h>
#  else
#     error "Using System time for Timed function requires Time module support (not present)"
#  endif
#endif


#if BOS_SOCKET_USE_GENERIC_IMPLEMENTATION

#include <str.h>
#include <string.h>
#include <time.h>    /* Needed for timeval */
#include <stdlib.h>  /* for strtol() */

/**
 * @addtogroup bosSocket
 * @{
 */

/* ---- Public Variables ------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

const char *gBosSocketFunctionName[ BOS_SOCKET_NUM_FUNC_ID ];

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/**
 * Indicates that the Socket module has been initialized.
 */

BOS_BOOL gBosSocketInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/

BOS_STATUS bosSocketGenericInit( void )
{
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   BOS_ASSERT( sizeof( BOS_SOCKET_ADDR_STORAGE ) >= sizeof( BOS_SOCKET_ADDR_IN6) );
   BOS_ASSERT( sizeof( BOS_IP_ADDRESS_V4 ) == sizeof( struct in_addr ) );
   BOS_ASSERT( sizeof( BOS_SOCKET_ADDR_IN4 ) == sizeof( struct sockaddr_in ) );
   BOS_ASSERT( sizeof( BOS_IP_ADDRESS_V6 ) == sizeof( struct in6_addr ) );
   BOS_ASSERT( sizeof( BOS_SOCKET_ADDR_IN6 ) == sizeof( struct sockaddr_in6 ) );
#endif

   return BOS_STATUS_OK;

} /* bosSocketGenericInit */

/***************************************************************************/

static BOS_UINT32 bosSocketGetProtocolStructSize( BOS_SOCKET_ADDR *bosSockAddr )
{
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   switch ( bosSockAddr->family )
   {
      case BOS_SOCKET_AF_INET:
         return ( sizeof( BOS_SOCKET_ADDR_IN4 ) );
      case BOS_SOCKET_AF_INET6:
         return ( sizeof( BOS_SOCKET_ADDR_IN6 ) );
      default:
         blogStrErr(( gBosBlog, "bosSocketGenericSizeOf  failed bad ip type %d\n",
                  (bosSockAddr->family) ));
         BOS_ASSERT( 0 );
         return ( 0 );    /* to get rid of compile error */
         break;
   }
#else
   (void) bosSockAddr;

   return ( sizeof( BOS_SOCKET_ADDR_IN4 ) );
#endif

}

/***************************************************************************/

BOS_STATUS bosSocketAccept
(
   BOS_SOCKET        *listenSocket,
   BOS_SOCKET_ADDR   *remoteAddr,
   size_t            *remoteAddrLen,
   BOS_SOCKET        *acceptedSocket
)
{
   BOS_SOCKET  newSocket;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   BOS_SOCKET_ADDR_IN6   *sin6;
   (void)sin6;
#endif
   memset(&newSocket, 0, sizeof(newSocket));

   bosSocketClearLastError();

   /* Initializing newSocket, newSocket is supposed to inherit all 
    * properties of listenSocket, and only the handle will be different */
   newSocket = *listenSocket; 

#if BOS_CFG_IP_ADDR_V6_SUPPORT
   sin6 = (BOS_SOCKET_ADDR_IN6 *)remoteAddr;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketAccept called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       v6 listenSocket = %d\n",
                GetSocket( listenSocket ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.family = %d\n",
                sin6->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.port = %d\n",
                bosSocketHtoNS( sin6->sin6_port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.flowinfo = %d\n",
                bosSocketHtoNL( sin6->sin6_flowinfo )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.scope_id = %d\n",
                bosSocketHtoNL( sin6->sin6_scope_id )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.ipAddr = %x:%x:%x:%x:%x:%x:%x:%x.\n",
           sin6->sin6_addr.s6Addr[0],
           sin6->sin6_addr.s6Addr[1],
           sin6->sin6_addr.s6Addr[2],
           sin6->sin6_addr.s6Addr[3],
           sin6->sin6_addr.s6Addr[4],
           sin6->sin6_addr.s6Addr[5],
           sin6->sin6_addr.s6Addr[6],
           sin6->sin6_addr.s6Addr[7]
           ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "      remoteAddrLen = %d\n", *remoteAddrLen ));
#else
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketAccept called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       listenSocket = %d\n",
                GetSocket( listenSocket ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.family = %d\n",
                remoteAddr->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.port = %d\n",
                bosSocketHtoNS( remoteAddr->port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.ipAddr = %d.%d.%d.%d\n",
                                                    remoteAddr->ipAddr.u8[ 0 ],
                                                    remoteAddr->ipAddr.u8[ 1 ],
                                                    remoteAddr->ipAddr.u8[ 2 ],
                                                    remoteAddr->ipAddr.u8[ 3 ] ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "      remoteAddrLen = %d\n", *remoteAddrLen ));
#endif

   GetSocket( &newSocket ) = accept(   GetSocket( listenSocket ),
                                       (struct sockaddr *)remoteAddr,
                                       (socklen_t *)remoteAddrLen );

   if ( GetSocket( &newSocket ) == BOS_SOCKET_INVALID )
   {
       return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_ACCEPT_FUNC_ID );
   }

   *acceptedSocket = newSocket;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketAccept succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketAccept */

/***************************************************************************/
BOS_STATUS bosSocketAcceptIp
(
   BOS_SOCKET           *listenSocket,
   BOS_IP_ADDRESS       *remoteAddr,
   BOS_SOCKET_IP_PORT   *remotePort,
   BOS_SOCKET           *acceptedSocket
)
{
   BOS_SOCKET_ADDR       ss;
   size_t                tsize;
   BOS_STATUS            status;
   int                   type;

//LH   char                  ipstr[64];

//LH I think remotePort and remoteAddr are both output parameters, so I is invalid
//LH to access them before being initialized.
//LH
//LH   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketAcceptIp called\n" ));
//LH   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       listenSocket = %d\n",
//LH                GetSocket( listenSocket ) ));
//LH   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       remoteport = %d\n",
//LH                *remotePort ));
//LH   bosIpAddrGetStr(remoteAddr, ipstr, sizeof(ipstr));
//LH   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       IP address = %s\n", ipstr));

   tsize = sizeof( ss );
   status = bosSocketAccept( listenSocket, &ss, &tsize, acceptedSocket );
   if ( status != BOS_STATUS_OK )
   {
      blogStrErr(( gBosBlog, "bosSocketAcceptIp failed status = %d.\n", status ));
      return ( BOS_STATUS_ERR );
   }

   if ( bosSocketAddrGetIpInfo(
            &ss,
            &type,
            remotePort,
            remoteAddr) != BOS_STATUS_OK)
   {
      blogStrErr(( gBosBlog, "bosSocketAcceptIp failed when getting returned IP info.\n" ));
      return ( BOS_STATUS_ERR );
   }

   return BOS_STATUS_OK;
}
 

BOS_STATUS bosSocketBind
(
   BOS_SOCKET             *s,
   const BOS_SOCKET_ADDR  *localAddr,
   size_t                  localAddrLen
)
{
   int                   status;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   BOS_SOCKET_ADDR_IN6   *sin6;
   (void) sin6;
#endif


   bosSocketClearLastError();

#if BOS_CFG_IP_ADDR_V6_SUPPORT
   sin6 = (BOS_SOCKET_ADDR_IN6 *)localAddr;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketBind called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       socket = %d\n", GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  addr.family = %d\n",
            sin6->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    localAddr.port = %d\n",
                bosSocketHtoNS( sin6->sin6_port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    localAddr.flowinfo = %d\n",
                bosSocketHtoNL( sin6->sin6_flowinfo )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    localAddr.scope_id = %d\n",
                bosSocketHtoNL( sin6->sin6_scope_id )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  localAddr.ipAddr = %x:%x:%x:%x:%x:%x:%x:%x.\n",
           sin6->sin6_addr.s6Addr[0],
           sin6->sin6_addr.s6Addr[1],
           sin6->sin6_addr.s6Addr[2],
           sin6->sin6_addr.s6Addr[3],
           sin6->sin6_addr.s6Addr[4],
           sin6->sin6_addr.s6Addr[5],
           sin6->sin6_addr.s6Addr[6],
           sin6->sin6_addr.s6Addr[7]
           ));
#else
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketBind called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       socket = %d\n", GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  addr.family = %d\n", localAddr->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  addr.sa_family = %d\n",
                ((struct sockaddr *)localAddr)->sa_family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    addr.port = %d\n",
                bosSocketHtoNS( localAddr->port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  addr.ipAddr = %d.%d.%d.%d\n",
                                                    localAddr->ipAddr.u8[ 0 ],
                                                    localAddr->ipAddr.u8[ 1 ],
                                                    localAddr->ipAddr.u8[ 2 ],
                                                    localAddr->ipAddr.u8[ 3 ] ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  localAddrLen = %d\n", localAddrLen));
#endif
   if (( status = bind( GetSocket( s ), (struct sockaddr *)localAddr, (int)localAddrLen )) != 0 )
   {
      (void)status;  /* Get rid of warning if the next line is compiled out */

      blogStrErr(( gBosBlog, "bosSocketBind failed: status = %d",  status ));
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_BIND_FUNC_ID );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketBind succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketBind */

/***************************************************************************/
BOS_STATUS bosSocketBindIp
(
   BOS_SOCKET             *s,       /**< (mod) Socket to bind to an address   */
   int                    scope_id, /**< (in)  scopeid for v6 sockets         */
   BOS_IP_ADDRESS         *ipAddr,  /**< (in)  IP Address to bind to          */
   BOS_SOCKET_IP_PORT      port     /**< (in)  IP port to bind to             */
)
{
   BOS_SOCKET_ADDR       ss;
   char                  ipstr[64];

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketBindIp called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       port = %d\n",
                port ));
   bosIpAddrGetStr(ipAddr, ipstr, sizeof(ipstr));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       IP address = %s\n", ipstr));


   bosSocketAddrCreate(ipAddr, port, scope_id, &ss);

   return bosSocketBind( s, &ss, bosSocketGetProtocolStructSize( &ss ));

} /* bosSocketBindIp */

/***************************************************************************/

BOS_STATUS bosSocketClose
(
   BOS_SOCKET  *s
)
{
   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketClose called with socket: %d\n",
                GetSocket( s ) ));

   if ( closesocket( GetSocket( s ) ) != 0 )
   {
      blogStrErr(( gBosBlog, "bosSocketClose failed" ));

      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_CLOSE_FUNC_ID );
   }

   GetSocket( s ) = BOS_SOCKET_INVALID;

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketClose succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketClose */

/***************************************************************************/

BOS_STATUS bosSocketConnect
(
   BOS_SOCKET             *s,
   const BOS_SOCKET_ADDR  *remoteAddr,
   size_t                  remoteAddrLen
)
{
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   BOS_SOCKET_ADDR_IN6   *sin6;
   (void) sin6;
#endif

   bosSocketClearLastError();


#if BOS_CFG_IP_ADDR_V6_SUPPORT
   sin6 = (BOS_SOCKET_ADDR_IN6 *)remoteAddr;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketConnect called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             socket = %d\n",
                GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.family = %d\n",
                sin6->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.port = %d\n",
                bosSocketHtoNS( sin6->sin6_port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.flowinfo = %d\n",
                bosSocketHtoNL( sin6->sin6_flowinfo )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.scope_id = %d\n",
                bosSocketHtoNL( sin6->sin6_scope_id )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.ipAddr = %x:%x:%x:%x:%x:%x:%x:%x.\n",
           sin6->sin6_addr.s6Addr[0],
           sin6->sin6_addr.s6Addr[1],
           sin6->sin6_addr.s6Addr[2],
           sin6->sin6_addr.s6Addr[3],
           sin6->sin6_addr.s6Addr[4],
           sin6->sin6_addr.s6Addr[5],
           sin6->sin6_addr.s6Addr[6],
           sin6->sin6_addr.s6Addr[7]
           ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "      remoteAddrLen = %d\n", remoteAddrLen ));
#else
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketConnect called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             socket = %d\n",
                GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.family = %d\n",
                remoteAddr->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.port = %d\n",
                bosSocketHtoNS( remoteAddr->port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.ipAddr = %d.%d.%d.%d\n",
                                                    remoteAddr->ipAddr.u8[ 0 ],
                                                    remoteAddr->ipAddr.u8[ 1 ],
                                                    remoteAddr->ipAddr.u8[ 2 ],
                                                    remoteAddr->ipAddr.u8[ 3 ] ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "      remoteAddrLen = %d\n", remoteAddrLen ));
#endif
   blogStrErr(( gBosBlog, "      remoteAddrLen = %d", remoteAddrLen ));

   /*
    * NOTE: We cast away the const, because some OS's still have prototypes
    *       with a non-const pointer.
    */

   if ( connect( GetSocket( s ), (struct sockaddr *)remoteAddr, (int)remoteAddrLen ) != 0 )
   {
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_CONNECT_FUNC_ID );
   }

   blogStrErr(( gBosBlog, "bosSocketConnect succeeded" ));
   return BOS_STATUS_OK;

} /* bosSocketConnect */

/***************************************************************************/

BOS_STATUS bosSocketConnectIp
(
   BOS_SOCKET             *s,
   BOS_IP_ADDRESS         *ipAddr,
   BOS_SOCKET_IP_PORT      port
)
{
   BOS_SOCKET_ADDR       ss;
   char                  ipstr[64];

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketAcceptIp called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       listenSocket = %d\n",
                GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       remoteport = %d\n",
                port ));
   bosIpAddrGetStr(ipAddr, ipstr, sizeof(ipstr));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       IP address = %s\n", ipstr));


   bosSocketAddrCreate(ipAddr, port, 0, &ss);

   return bosSocketConnect( s, &ss, bosSocketGetProtocolStructSize( &ss ));

} /* bosSocketConnectIp */

/***************************************************************************/

BOS_STATUS bosSocketCreate
(
   int         addrFamily,
   int         socketType,
   int         protocol,
   BOS_SOCKET *outSocket
)
{
   BOS_SOCKET  newSocket;
   memset(&newSocket, 0, sizeof(newSocket));

   BOS_ASSERT( gBosSocketInitialized );

   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketCreate called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  addrFamily = %d\n", addrFamily ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  socketType = %d\n", socketType ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    protocol = %d\n", protocol ));

   if (( GetSocket( &newSocket ) = socket( addrFamily, socketType, protocol )) == BOS_SOCKET_INVALID )
   {
      blogStrErr(( gBosBlog, "bosSocketCreate failed" ));
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_CREATE_FUNC_ID );
   }

   newSocket.isNonBlocking = BOS_FALSE;
   newSocket.family = addrFamily;

#if BOS_CFG_SOCKET_INTERFACE_SUPPORT
   newSocket.ifno = BOS_SOCKET_IFNO_INVALID;
#endif   /* BOS_CFG_SOCKET_INTERFACE_SUPPORT */

   *outSocket = newSocket;

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketCreate succeeded: socket %d was opened\n",
                GetSocket( outSocket ) ));

   return BOS_STATUS_OK;

} /* bosSocketCreate */

/***************************************************************************/
BOS_STATUS bosSocketAddrCreate
(
   BOS_IP_ADDRESS       *addr,
   BOS_SOCKET_IP_PORT   addrport,
   int                  scope_id,
   BOS_SOCKET_ADDR      *rAddr
)
{
   BOS_UINT32            ipnum;

#if BOS_CFG_IP_ADDR_V6_SUPPORT
   char                  ipstr[64];
   BOS_SOCKET_ADDR_IN6   sin6;
   BOS_SOCKET_ADDR_IN4   sin;
   BOS_SOCKET_ADDR       *ss;
   unsigned int          len;

   (void) scope_id;
   bosSocketClearLastError();

   memset( &sin, 0, sizeof( sin ));
   memset( &sin6, 0, sizeof( sin6 ));

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketAddrCreate called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  family = %d\n", addr->ipType ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  port = %d\n", addrport ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  scope_id = %d\n", scope_id ));
   bosIpAddrGetStr(addr, ipstr, sizeof(ipstr));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  addr = %s\n", ipstr ));

   switch (addr->ipType)
   {
      case BOS_IPADDRESS_TYPE_V4:
         ss = (BOS_SOCKET_ADDR *)&sin;
#if BOS_CFG_SOCKET_BSD_4_4
         sin.length     = sizeof(BOS_SOCKET_ADDR_IN4);
#endif
         sin.family     = BOS_SOCKET_AF_INET;
         sin.port       = bosSocketHtoNS( addrport );
         bosIpAddrV4GetU32( addr, &ipnum );
         sin.ipAddr.u32 = bosSocketHtoNL(ipnum);
         break;
      case BOS_IPADDRESS_TYPE_V6:
         ss = (BOS_SOCKET_ADDR *)&sin6;
#if BOS_CFG_SOCKET_BSD_4_4
         sin6.length = sizeof(BOS_SOCKET_ADDR_IN6);
#endif
         sin6.family = BOS_SOCKET_AF_INET6;
         sin6.sin6_port = bosSocketHtoNS( addrport );
         sin6.sin6_flowinfo = 0;
         sin6.sin6_scope_id = scope_id;
         bosIpAddrGetU8(addr, (BOS_UINT8 *)&sin6.sin6_addr, sizeof(sin6.sin6_addr), &len);
         break;
      default:
         blogStrErr(( gBosBlog, "bosSocketAddrCreate failed bad ip type %d\n",
                  addr->ipType));
         return bosSocketCheckStatus( bosSocketGetLastError(),
               BOS_SOCKET_CREATE_ADDR_FUNC_ID );
   }   /* switch */

   *rAddr = *ss;
#else
   BOS_SOCKET_ADDR_IN4   sin;
   (void) scope_id;
   memset( &sin, 0, sizeof( sin ));
#if BOS_CFG_SOCKET_BSD_4_4
   sin.length     = sizeof(BOS_SOCKET_ADDR_IN4);
#endif
   sin.family     = BOS_SOCKET_AF_INET;
   sin.port       = bosSocketHtoNS( addrport );
   bosIpAddrV4GetU32( addr, &ipnum );
   sin.ipAddr.u32 = bosSocketHtoNL(ipnum);
   *rAddr = sin;
#endif
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketAddrCreate succeeded.\n"));
   return BOS_STATUS_OK;

} /* bosSocketAddrCreate */

/***************************************************************************/
BOS_STATUS bosSocketAddrGetIpInfo
(
   BOS_SOCKET_ADDR      *addr,
   int                  *addrfamily,
   BOS_SOCKET_IP_PORT   *addrport,
   BOS_IP_ADDRESS       *ipaddr
)
{
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   BOS_SOCKET_ADDR_IN6   *sin6;
   BOS_SOCKET_ADDR_IN4   *sin;

   switch (addr->family)
   {
      case BOS_SOCKET_AF_INET:
         sin = (BOS_SOCKET_ADDR_IN4 *)addr;
         *addrfamily = sin->family;
         *addrport = bosSocketNtoHS(sin->port);
         bosIpAddrV4CreateFromU32(bosSocketNtoHL(sin->ipAddr.u32), ipaddr);
         break;
      case BOS_SOCKET_AF_INET6:
         sin6 = (BOS_SOCKET_ADDR_IN6 *)addr;
         *addrfamily = sin6->family;
         *addrport = bosSocketNtoHS(sin6->sin6_port);
         bosIpAddrCreateFromU8(BOS_IPADDRESS_TYPE_V6, (BOS_UINT8 *)&sin6->sin6_addr,
               ipaddr);
         break;
      default:
         blogStrErr(( gBosBlog, "bosSocketAddrGetIpInfo failed bad ip type %d\n",
                  (addr->family) ));
         return bosSocketCheckStatus( bosSocketGetLastError(),
               BOS_SOCKET_ADDR_GETIPINFO_FUNC_ID );
         break;
   }   /* switch */
#else
   *addrfamily = addr->family;
   *addrport = bosSocketNtoHS(addr->port);
   bosIpAddrV4CreateFromU32(bosSocketNtoHL(addr->ipAddr.u32), ipaddr);
#endif
   return BOS_STATUS_OK;
}


/***************************************************************************/
BOS_STATUS bosSocketGetIpAddrFamily
(
   BOS_IP_ADDRESS       *ipaddr,
   int                  *addrFamily
)
{
  (void)  ipaddr;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   switch (ipaddr->ipType)
   {
      case BOS_IPADDRESS_TYPE_V4:
         *addrFamily = BOS_SOCKET_AF_INET;
         break;
      case BOS_IPADDRESS_TYPE_V6:
         *addrFamily = BOS_SOCKET_AF_INET6;
         break;
      default:
         blogStrErr(( gBosBlog, "bosSocketGetIpAddrFamily failed bad ip type %d\n",
                  ipaddr->ipType ));
         return BOS_STATUS_ERR;
   }   /* switch */
#else
   *addrFamily = BOS_SOCKET_AF_INET;
#endif
   return BOS_STATUS_OK;
}

/***************************************************************************/
BOS_STATUS bosSocketGetOption
(
   BOS_SOCKET *s,
   int         level,
   int         option,
   void       *optionVal,
   size_t     *optionLen
)
{
   bosSocketClearLastError();

   if ( getsockopt( GetSocket( s ), level, option, (char *)optionVal, (socklen_t *)optionLen ) != 0 )
   {
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_GET_OPTION_FUNC_ID );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketGetOption succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketGetOption */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

/***************************************************************************/
/**
*  Initializes the contents of @a gBosSocketFunctionName, which is used to
*  translate function IDs into character strings.
*/

void bosSocketInitFunctionNames( void )
{
   gBosSocketFunctionName[ BOS_SOCKET_INIT_FUNC_ID ]              = "bosSocketInit";
   gBosSocketFunctionName[ BOS_SOCKET_TERM_FUNC_ID ]              = "bosSocketTerm";
   gBosSocketFunctionName[ BOS_SOCKET_ACCEPT_FUNC_ID ]            = "bosSocketAccept";
   gBosSocketFunctionName[ BOS_SOCKET_BIND_FUNC_ID ]              = "bosSocketBind";
   gBosSocketFunctionName[ BOS_SOCKET_CLOSE_FUNC_ID ]             = "bosSocketClose";

#if BOS_CFG_TASK
   gBosSocketFunctionName[ BOS_SOCKET_CLOSE_SHARED_FUNC_ID ]      = "bosSocketCloseShared";
#endif   /* BOS_CFG_TASK */

   gBosSocketFunctionName[ BOS_SOCKET_CONNECT_FUNC_ID ]           = "bosSocketConnect";
   gBosSocketFunctionName[ BOS_SOCKET_CREATE_FUNC_ID ]            = "bosSocketCreate";
   gBosSocketFunctionName[ BOS_SOCKET_CREATE_ADDR_FUNC_ID ]       = "bosSocketAddrCreate";
   gBosSocketFunctionName[BOS_SOCKET_ADDR_GETIPINFO_FUNC_ID]      = "bosSocketAddrGetIpInfo";
   gBosSocketFunctionName[ BOS_SOCKET_GET_OPTION_FUNC_ID ]        = "bosSocketGetOption";
   gBosSocketFunctionName[ BOS_SOCKET_LISTEN_FUNC_ID ]            = "bosSocketListen";
   gBosSocketFunctionName[ BOS_SOCKET_RECV_FUNC_ID ]              = "bosSocketRecv";
   gBosSocketFunctionName[ BOS_SOCKET_RECV_FROM_FUNC_ID ]         = "bosSocketRecvFrom";

#if BOS_CFG_TASK
   gBosSocketFunctionName[ BOS_SOCKET_SHARE_FUNC_ID ]             = "bosSocketShare";
#endif   /* BOS_CFG_TASK */

   gBosSocketFunctionName[ BOS_SOCKET_SELECT_FUNC_ID ]            = "bosSocketSelect";
   gBosSocketFunctionName[ BOS_SOCKET_SEND_FUNC_ID ]              = "bosSocketSend";
   gBosSocketFunctionName[ BOS_SOCKET_SEND_TO_FUNC_ID ]           = "bosSocketSendTo";
   gBosSocketFunctionName[ BOS_SOCKET_SET_NO_DELAY_FUNC_ID ]      = "bosSocketSetNoDelay";
   gBosSocketFunctionName[ BOS_SOCKET_SET_NON_BLOCKING_FUNC_ID ]  = "bosSocketSetNonBlocking";
   gBosSocketFunctionName[ BOS_SOCKET_SET_OPTION_FUNC_ID ]        = "bosSocketSetOption";
   gBosSocketFunctionName[ BOS_SOCKET_SHUTDOWN_FUNC_ID ]          = "bosSocketShutdown";
   gBosSocketFunctionName[ BOS_SOCKET_SET_TOS_ID ]                = "bosSocketSetIpTos";
   gBosSocketFunctionName[ BOS_SOCKET_GET_NAME_FUNC_ID ]          = "bosSocketGetName";
   gBosSocketFunctionName[ BOS_SOCKET_GET_PEERNAME_FUNC_ID ]      = "bosSocketGetPeerName";

#if BOS_CFG_SOCKET_INTERFACE_SUPPORT
   gBosSocketFunctionName[ BOS_SOCKET_SET_INTERFACE_ID ]          = "bosSocketSetInterface";
#endif   /* BOS_CFG_SOCKET_INTERFACE_SUPPORT */

   bosErrorAddFunctionStrings( BOS_SOCKET_MODULE, gBosSocketFunctionName, BOS_SOCKET_NUM_FUNC_ID );

} /* bosSocketInitFunctioNames */

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/***************************************************************************/

BOS_STATUS bosSocketListen
(
   BOS_SOCKET  *s,
   int          backLog
)
{
   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketListen called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "        socket = %d\n", GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       backLog = %d\n", backLog ));

   if ( listen( GetSocket( s ), backLog ) != 0 )
   {
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_LISTEN_FUNC_ID );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketListen succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSOcketListen */

/***************************************************************************/

BOS_STATUS bosSocketLookupServerAndPort
(
   const char           *name,
   BOS_IP_ADDRESS       *ipAddr,
   BOS_SOCKET_IP_PORT   *ipPort,
   char                 *errStr,
   size_t                maxLen
)
{
   char  serverName[ BOS_CFG_SOCKET_MAX_HOSTNAME_LEN ];
   const char *portStr;
   BOS_UINT32 ipnum;

   /*
    * Don't zap *ipAddr and *ipPort. This allows the caller to specify
    * defaults, which we'll leave alone if the corresponding portion of
    * the address is missing.
    */

   *errStr = '\0';

   if (( portStr = strchr( name, ':' )) != NULL )
   {
      char *endStr;
      long  longPort;

      /*
       * A port was specified.
       */

      portStr++;  /* Skip over the colon */

      /*
       * Note, we need to allow for a terminating null, and the colon
       * is used as the placeholder for sizing purposes. Otherwise we'd
       * have to add a bunch of +1's below.
       */

      if (( portStr - name ) >= (ptrdiff_t)sizeof( serverName ))
      {
         strMaxCpy( serverName, name, sizeof( serverName ));
      }
      else
      {
         /*
          * It fits, so we'll use the length upto the colon as the length
          * of the buffer so that it gets truncated at the colon.
          */

         strMaxCpy( serverName, name, portStr - name );
      }

      longPort = strtol( portStr, &endStr, 0 );
      if ( *endStr != '\0' )
      {
         strPrintf( errStr, maxLen,
                    "Expecting a positive numeric port specifier; Found '%s'",
                    portStr );

         return BOS_STATUS_ERR;
      }

      if (( longPort <= 0 ) || ( longPort > 65535 ))
      {
         strPrintf( errStr, maxLen,
                    "Expecting port to be between 1 and 65535; Found: %ld",
                    longPort );

         return BOS_STATUS_ERR;
      }
      *ipPort = (BOS_SOCKET_IP_PORT)longPort;
   }
   else
   {
      /*
       * No colon in the name; it's just a servername
       */

      strMaxCpy( serverName, name, sizeof( serverName ));
   }

   /*
    * Check to see if the servername is only made up of numbers and dots.
    */

   if ( strspn( serverName, ".0123456789" ) == strlen( serverName ))
   {
      int   seg;
      char *segStr  = serverName;
      char *endPtr;

      /*
       * The servername is only made up of numbers and dots. We'll parse
       * it ourselves.
       *
       * We expect it to be of the form a.b.c.d, where a, b, c, and d are
       * all positive integers in the range 0 thru 255.
       */

      bosIpAddrCreateZero(BOS_IPADDRESS_TYPE_V4, ipAddr);

      for ( seg = 0; seg < 4; seg++ )
      {
         long  longByte = strtol( segStr, &endPtr, 10 );

         if (( longByte < 0 ) || ( longByte > 255 ))
         {
            strPrintf( errStr, maxLen, "Expecting a positive number between 0 and 255, found: '%s'", segStr );

            return BOS_STATUS_ERR;
         }

         if (  (( *endPtr == '.' ) && ( seg == 3 ))
            || (( *endPtr != '.' ) && ( seg <  3 )))
         {
            strPrintf( errStr, maxLen, "Expecting IP address of the form a.b.c.d, found: '%s'", serverName );

            return BOS_STATUS_ERR;
         }

         bosIpAddrV4GetU32(ipAddr, &ipnum);
         ipnum <<= 8;
         ipnum += longByte;
         bosIpAddrV4CreateFromU32(ipnum, ipAddr);

         segStr = endPtr + 1;
      }

      return BOS_STATUS_OK;
   }

#if BOS_CFG_SOCKET_HAS_GETHOSTNAME
   {
      struct hostent      *hostent;

      hostent = gethostbyname( serverName );
      if ( hostent == NULL )
      {
         strPrintf( errStr, maxLen,
                    "Unable to translate '%s' into an IP address", serverName );

         return BOS_STATUS_ERR;
      }

      /* 
       * Note: gethostbyname() returns IP address in Host Byte ordering 
       * fashion. The address has to be converted to Network Byte ordering 
       * for using with BOS socket APIs. 
       */
      bosIpAddrV4CreateFromU32(bosSocketHtoNL(*((BOS_UINT32 *)(hostent->h_addr_list[ 0 ]))), ipAddr);

      return BOS_STATUS_OK;
   }
#else

   strPrintf( errStr, maxLen, "Non-numeric IP address encountered: '%s'", serverName );

   return BOS_STATUS_ERR;
#endif

} /* bosSocketLookupServerAndPort */

/***************************************************************************/

BOS_STATUS bosSocketNtoHU16Array( BOS_UINT16 *dst, BOS_UINT16 *src, size_t n  )
{
#if BOS_CFG_BIG_ENDIAN

   /*
    * Network order is big endian, so no swapping is required
    */

   if ( dst != src )
   {
      /*
       * Use memmove, just in case src and dst overlap.
       */

      memmove( dst,  src,  n );
   }

#elif BOS_CFG_LITTLE_ENDIAN

   /*
    * We could call swab, but I wanted to make this completely OS independant.
    */

   char  *bSrc = (char *)src;
   char  *bDst = (char *)dst;

   while ( n > 1 )
   {
      /*
       * IMPORTANT: We need to read 2 source bytes and write 2 destination
       *            bytes since bSrc could be equal to bDst.
       */

      char b1 = *bSrc++;
      char b2 = *bSrc++;

      *bDst++ = b2;
      *bDst++ = b1;

      n -= 2;
   }

#else
#error   What kind of Endian are we dealing with here anyways!
#endif

   return BOS_STATUS_OK;

} /* bosSocketNtoHU16Array */

/***************************************************************************/

BOS_STATUS bosSocketRecv
(
   BOS_SOCKET *s,
   void       *buf,
   size_t      bufLen,
   int         flags,
   size_t     *bytesRcvd
)
{
   int   rcvd;
   BOS_SOCKET_ERROR_TYPE   errCode;

   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecv called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       socket = %d\n", GetSocket( s ) ));

   if (( rcvd = recv( GetSocket( s ), buf, bufLen, flags )) < 0 )
   {
     errCode = bosSocketGetLastError();

     *bytesRcvd = 0;

      if ( errCode == EWOULDBLOCK )
      {
         return BOS_STATUS_WOULD_BLOCK;
      }

      return bosSocketCheckStatus( errCode, BOS_SOCKET_RECV_FUNC_ID );
   }

   *bytesRcvd = (size_t)rcvd;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecv succeeded, %d bytes received\n",
                *bytesRcvd ));

   return BOS_STATUS_OK;

} /* bosSocketRecv */

/***************************************************************************/
BOS_STATUS bosSocketRecvFrom
(
   BOS_SOCKET       *s,
   void             *buf,
   size_t            bufLen,
   int               flags,
   BOS_SOCKET_ADDR  *fromAddr,
   int              *fromLen,
   size_t           *bytesRcvd
)
{
   int   rcvd;
   BOS_SOCKET_ERROR_TYPE   errCode;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   BOS_SOCKET_ADDR_IN6   *sin6;
   (void) sin6;
#endif

   bosSocketClearLastError();

#if BOS_CFG_SOCKET_INTERFACE_SUPPORT
   bosSocketSetInterfaceFlags( s, flags );
   bosSocketSetInterfaceAddr( s, fromAddr );
#endif

   rcvd = recvfrom(  GetSocket( s ),
                     buf,
                     bufLen,
                     flags,
                     (struct sockaddr *)fromAddr,
                     (socklen_t *)fromLen );

   if ( rcvd < 0 )
   {
      errCode = bosSocketGetLastError();

      *bytesRcvd = 0;

      if ( errCode == EWOULDBLOCK )
      {
         return BOS_STATUS_WOULD_BLOCK;
      }
      return bosSocketCheckStatus( errCode, BOS_SOCKET_RECV_FROM_FUNC_ID );
   }

   *bytesRcvd = (size_t)rcvd;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecvFrom succeeded, %d bytes received\n",
                *bytesRcvd ));

#if BOS_CFG_IP_ADDR_V6_SUPPORT
   sin6 = (BOS_SOCKET_ADDR_IN6 *)fromAddr;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecvFrom called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             socket = %d\n",
                GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.family = %d\n",
                sin6->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.port = %d\n",
                bosSocketHtoNS( sin6->sin6_port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.flowinfo = %d\n",
                bosSocketHtoNL( sin6->sin6_flowinfo )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.scope_id = %d\n",
                bosSocketHtoNL( sin6->sin6_scope_id )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.ipAddr = %x:%x:%x:%x:%x:%x:%x:%x.\n",
           sin6->sin6_addr.s6Addr[0],
           sin6->sin6_addr.s6Addr[1],
           sin6->sin6_addr.s6Addr[2],
           sin6->sin6_addr.s6Addr[3],
           sin6->sin6_addr.s6Addr[4],
           sin6->sin6_addr.s6Addr[5],
           sin6->sin6_addr.s6Addr[6],
           sin6->sin6_addr.s6Addr[7]
           ));
#else
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecvFrom called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             socket = %d\n",
                GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    fromAddr.family = %d\n",
                fromAddr->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "      fromAddr.port = %d\n",
                bosSocketHtoNS( fromAddr->port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    fromAddr.ipAddr = %d.%d.%d.%d\n",
                                                    fromAddr->ipAddr.u8[ 0 ],
                                                    fromAddr->ipAddr.u8[ 1 ],
                                                    fromAddr->ipAddr.u8[ 2 ],
                                                    fromAddr->ipAddr.u8[ 3 ] ));
#endif


   return BOS_STATUS_OK;

} /* bosSocketRecvFrom */

/***************************************************************************/
BOS_STATUS bosSocketRecvFromIp
(
   BOS_SOCKET           *s,          /**< (mod) Socket to receive the data from.     */
   void                 *buf,        /**< (out) Place to store the received data.    */
   size_t                bufLen,     /**< (in) Size of @a buf.                       */
   int                   flags,      /**< (in) Flags which modify how recv functions.*/
   BOS_IP_ADDRESS       *fromAddr,   /**< (out) Address data was received from.      */
   BOS_SOCKET_IP_PORT   *fromPort,    /**< (out) port              .                 */
   size_t               *bytesRcvd   /**< (out) Number of bytes actually received.   */
)
{
   BOS_SOCKET_ADDR       ss;
   int                   tsize;
   BOS_STATUS            status;
   int                   type;
   char                  ipstr[64];

   memset(&ss, 0, sizeof(ss));

   ss.family = (BOS_SOCKET_FAMILY)s->family;
   tsize = sizeof(BOS_SOCKET_ADDR);
   status = bosSocketRecvFrom(s, buf, bufLen, flags, &ss, &tsize, bytesRcvd);
   if ( status != BOS_STATUS_OK )
   {
      blogStrErr(( gBosBlog, "bosSocketRecvFromIp failed status = %d.\n", status ));
      return ( BOS_STATUS_ERR );
   }
   if ( bosSocketAddrGetIpInfo(
            &ss,
            &type,
            fromPort,
            fromAddr) != BOS_STATUS_OK)
   {
      blogStrErr(( gBosBlog, "bosSocketRecvFromIp failed when getting returned IP info.\n" ));
      return ( BOS_STATUS_ERR );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecvFromIp called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       socket = %d\n",
                GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       remoteport = %d\n",
                *fromPort ));
   bosIpAddrGetStr(fromAddr, ipstr, sizeof(ipstr));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       IP address = %s\n", ipstr));


   return BOS_STATUS_OK;

}

/***************************************************************************/

BOS_STATUS bosSocketMapIpAddrType
(
   BOS_IP_ADDRESS      *ipAddr,    /**< IP Address */
   int                 *sockType
)
{
  (void) ipAddr;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   switch ( bosIpAddrGetType(ipAddr) )
   {
      case BOS_IPADDRESS_TYPE_V4:
         *sockType = BOS_SOCKET_AF_INET;
         break;
      case BOS_IPADDRESS_TYPE_V6:
         *sockType = BOS_SOCKET_AF_INET6;
         break;
      default:
         blogStrErr(( gBosBlog, "ERROR: bosSocketMapIpAddrType - Bad server IP address type %d.\n",
                  bosIpAddrGetType(ipAddr) ));
         return ( BOS_STATUS_ERR);
   }   /* switch */
#else
   *sockType = BOS_SOCKET_AF_INET;
#endif
   return ( BOS_STATUS_OK);
}

/***************************************************************************/

BOS_STATUS bosSocketSelect
(
   int             maxSockets,
   BOS_SOCKET_SET *readSet,
   BOS_SOCKET_SET *writeSet,
   BOS_SOCKET_SET *exceptSet,
   BOS_TIME_MS     timeLimit,
   int            *socketsSelected
)
{
   struct timeval  tv;
   struct timeval *tvp = NULL;
   BOS_STATUS      result = BOS_STATUS_ERR;

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSelect called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       maxSockets = %d\n", maxSockets ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             time = %u\n", timeLimit ));

   if ( timeLimit == BOS_WAIT_FOREVER )
   {
      tvp = NULL;
   }
   else
   {
      tvp = &tv;

      tv.tv_sec = timeLimit / 1000;
      tv.tv_usec = ( timeLimit % 1000 ) * 1000;
   }

   bosSocketClearLastError();

   *socketsSelected = select( maxSockets, readSet, writeSet, exceptSet, tvp );

   if ( *socketsSelected < 0 )
   {
      result = bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_SELECT_FUNC_ID );
   }
   else if ( *socketsSelected == 0 )
   {
      result = BOS_STATUS_TIMEOUT;
   }
   else
   {
      blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSelect succeeded, %d sockets selected\n",
                   *socketsSelected ));

      result = BOS_STATUS_OK;
   }

   if (result != BOS_STATUS_OK)
   {
      /* Clear the socket sets in case of a failure */
      if ( readSet)
      {
         memset(readSet,0, sizeof(BOS_SOCKET_SET));
      }
      if ( writeSet)
      {
         memset(writeSet,0, sizeof(BOS_SOCKET_SET));
      }
      if ( exceptSet)
      {
         memset(exceptSet,0, sizeof(BOS_SOCKET_SET));
      }
   }

   return result;
} /* bosSocketSelect */

/***************************************************************************/

BOS_STATUS bosSocketSend
(
   BOS_SOCKET  *s,
   const void  *data,
   size_t       dataLen,
   int          flags,
   size_t      *bytesSent
)
{
   int                     sent;
   BOS_SOCKET_ERROR_TYPE   errCode;

   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSend called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "        socket = %d\n", GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       dataLen = %d\n", dataLen ));

   /*
    * NOTE: We cast away the const, because some OS's still have prototypes
    *       with a non-const pointer.
    */

   if (( sent = send( GetSocket( s ), (char *)data, (int)dataLen, flags )) < 0 )
   {
      errCode = bosSocketGetLastError();

      *bytesSent = 0;

      if ( errCode == EWOULDBLOCK )
      {
         return BOS_STATUS_WOULD_BLOCK;
      }
      return bosSocketCheckStatus( errCode, BOS_SOCKET_SEND_FUNC_ID );
   }

   *bytesSent = (size_t)sent;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSend succeeded, %d bytes sent\n",
                *bytesSent ));

   return BOS_STATUS_OK;

} /* bosSocketSend */

/***************************************************************************/

BOS_STATUS bosSocketSendTo
(
   BOS_SOCKET       *s,
   const void       *data,
   size_t            dataLen,
   int               flags,
   BOS_SOCKET_ADDR  *toAddr,
   int               toLen,
   size_t           *bytesSent
)
{
   int                     sent;
   BOS_SOCKET_ERROR_TYPE   errCode;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   BOS_SOCKET_ADDR_IN6   *sin6;
   (void) sin6;
#endif

   bosSocketClearLastError();

#if BOS_CFG_IP_ADDR_V6_SUPPORT
   sin6 = (BOS_SOCKET_ADDR_IN6 *)toAddr;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSendTo called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "           socket = %d\n",
                GetSocket( s ) ));
      blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.family = %d\n",
                sin6->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.port = %d\n",
                bosSocketHtoNS( sin6->sin6_port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.flowinfo = %d\n",
                bosSocketHtoNL( sin6->sin6_flowinfo )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    remoteAddr.scope_id = %d\n",
                bosSocketHtoNL( sin6->sin6_scope_id )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  remoteAddr.ipAddr = %x:%x:%x:%x:%x:%x:%x:%x.\n",
           sin6->sin6_addr.s6Addr[0],
           sin6->sin6_addr.s6Addr[1],
           sin6->sin6_addr.s6Addr[2],
           sin6->sin6_addr.s6Addr[3],
           sin6->sin6_addr.s6Addr[4],
           sin6->sin6_addr.s6Addr[5],
           sin6->sin6_addr.s6Addr[6],
           sin6->sin6_addr.s6Addr[7]
           ));
#else
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSendTo called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "           socket = %d\n",
                GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "          dataLen = %d\n",
                dataLen ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    toAddr.family = %d\n",
                toAddr->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "      toAddr.port = %d\n",
                bosSocketHtoNS( toAddr->port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    toAddr.ipAddr = %d.%d.%d.%d\n",
                                                        toAddr->ipAddr.u8[ 0 ],
                                                        toAddr->ipAddr.u8[ 1 ],
                                                        toAddr->ipAddr.u8[ 2 ],
                                                        toAddr->ipAddr.u8[ 3 ] ));
#endif

#if BOS_CFG_SOCKET_INTERFACE_SUPPORT
   bosSocketSetInterfaceFlags( s, flags );
   bosSocketSetInterfaceAddr( s, toAddr );
#endif

   /*
    * NOTE: We cast away the const, because some OS's still have prototypes
    *       with a non-const pointer.
    */
   sent = sendto( GetSocket( s ),
                  (char *)data,
                  (int)dataLen,
                  flags,
                  (struct sockaddr *)toAddr,
                  toLen );

   if ( sent < 0 )
   {
      errCode = bosSocketGetLastError();

      *bytesSent = 0;
      if ( errCode == EWOULDBLOCK )
      {
         return BOS_STATUS_WOULD_BLOCK;
      }
      return bosSocketCheckStatus( errCode, BOS_SOCKET_SEND_TO_FUNC_ID );
   }

   *bytesSent = (size_t)sent;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSendTo succeeded, %d bytes sent\n",
                *bytesSent ));

   return BOS_STATUS_OK;

} /* bosSocketSendTo */

/***************************************************************************/
BOS_STATUS bosSocketSendToIp
(
   BOS_SOCKET          *s,         /**< Socket to send the data to.              */
   const void          *data,      /**< Pointer to data to send.                 */
   size_t               dataLen,   /**< Number of bytes of data to send.         */
   int                  flags,     /**< Flags indicating how data should be sent.*/
   BOS_IP_ADDRESS      *toAddr,    /**< Address to send data to.                 */
   BOS_SOCKET_IP_PORT  toPort,     /**< port                                     */
   int                 scopeId,    /**< scopeId for v6 multiple interfaces       */
   size_t              *bytesSent  /**< Number of bytes actually sent.           */
)
{
   BOS_SOCKET_ADDR       ss;
   char                  ipstr[64];

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketAcceptIp called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       Socket = %d\n",
                GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       toPort = %d\n",
                toPort ));
   bosIpAddrGetStr(toAddr, ipstr, sizeof(ipstr));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       IP address = %s\n", ipstr));


   bosSocketAddrCreate(toAddr, toPort, scopeId, &ss);


   return bosSocketSendTo( s, data, dataLen, flags, &ss, bosSocketGetProtocolStructSize( &ss ), bytesSent);
}

/***************************************************************************/

BOS_STATUS bosSocketSetNoDelay( BOS_SOCKET *s, BOS_BOOL noDelay )
{
   BOS_STATUS     status;
   unsigned long  optVal = noDelay ? 1 : 0;

   if (( status = bosSocketSetOption( s, BOS_SOCKET_IPPROTO_TCP, BOS_SOCKET_TCP_NODELAY,
                                      &optVal, sizeof( optVal ))) != BOS_STATUS_OK )
   {
      (void)status;  /* Get rid of warning if the next line is compiled out */

      blogStrErr(( gBosBlog, "bosSocketSetNoDelay failed: status = %d", status ));
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_SET_NO_DELAY_FUNC_ID );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSetNoDelay succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketSetNoDelay */

/***************************************************************************/

BOS_STATUS bosSocketSetNonBlocking( BOS_SOCKET *s, BOS_BOOL nonBlocking )
{
   int            status;
   unsigned long  optVal = nonBlocking ? 1 : 0;

   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSetNonBlocking called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "         socket = %d\n", GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    nonBlocking = %d\n", nonBlocking ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    s->isNonBlocking = %u\n", s->isNonBlocking ));

   if (( status = ioctlsocket( GetSocket( s ), (int)FIONBIO, (void *)&optVal )) != 0 )
   {
      (void)status;  /* Get rid of warning if the next line is compiled out */

      blogStrErr(( gBosBlog, "bosSocketSetNonBlocking failed: status = %d",  status ));
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_SET_NON_BLOCKING_FUNC_ID );
   }

   /* Record socket's blocking/non-blocking mode */
   s->isNonBlocking = nonBlocking;

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSetNonBlocking succeeded,"
                "socket: %d, isNonBlocking: %d\n", s->s, s->isNonBlocking ));

   return BOS_STATUS_OK;

} /* bosSocketSetNonBlocking */

/***************************************************************************/

BOS_STATUS bosSocketSetOption
(
   BOS_SOCKET *s,
   int         level,
   int         option,
   const void *optionVal,
   size_t      optionLen
)
{
   int status;

   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSetOption called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "      socket = %d\n", GetSocket( s ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       level = %d\n", level ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "      option = %d\n", option ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  *optionVal = %d\n", *(int *)optionVal ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "   optionLen = %d\n", optionLen ));

   /*
    * NOTE: We cast away the const, because some OS's still have prototypes
    *       with a non-const pointer.
    */

   if (( status = setsockopt( GetSocket( s ), level, option, (char *)optionVal, (int)optionLen )) != 0 )
   {
      (void)status;  /* Get rid of warning if the next line is compiled out */

      blogStrErr(( gBosBlog, "bosSocketSetOption failed: status = %d", status ));
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_SET_OPTION_FUNC_ID );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSetOption succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketSetOption */

/***************************************************************************/

BOS_STATUS bosSocketShutdown
(
   BOS_SOCKET             *s,
   BOS_SOCKET_SHUTDOWN_HOW how
)
{
   bosSocketClearLastError();

   if ( shutdown( GetSocket( s ), (int)how ) != 0 )
   {
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_SHUTDOWN_FUNC_ID );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketShutdown succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketShutdown */

/***************************************************************************/

BOS_STATUS bosSocketGetName
(
   BOS_SOCKET *s,
   BOS_SOCKET_ADDR *sockName,
   size_t *nameLen
)
{
   int status;

   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketGetName called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "     socket  = %d\n", GetSocket( s ) ));

   /*
    * NOTE: We cast away the const, because some OS's still have prototypes
    *       with a non-const pointer.
    */

   if (( status = getsockname( GetSocket( s ), (struct sockaddr *)sockName, (socklen_t *)nameLen ))!= 0 )
   {
      (void)status;  /* Get rid of warning if the next line is compiled out */

      blogStrErr(( gBosBlog, "bosSocketGetName failed: status = %d", status ));
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_GET_NAME_FUNC_ID );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketGetName succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketGetName */

/***************************************************************************/

BOS_STATUS bosSocketGetPeerName
(
   BOS_SOCKET *s,
   BOS_SOCKET_ADDR *sockName,
   size_t *nameLen
)
{
   int status;

   bosSocketClearLastError();

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketGetPeerName called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "     socket  = %d\n", GetSocket( s ) ));

   /*
    * NOTE: We cast away the const, because some OS's still have prototypes
    *       with a non-const pointer.
    */

   if (( status = getpeername( GetSocket( s ), (struct sockaddr *)sockName, (socklen_t *)nameLen ))!= 0 )
   {
      (void)status;  /* Get rid of warning if the next line is compiled out */

      blogStrErr(( gBosBlog, "bosSocketGetPeerName failed: status = %d", status ));
      return bosSocketCheckStatus( bosSocketGetLastError(), BOS_SOCKET_GET_PEERNAME_FUNC_ID );
   }

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketGetPeerName succeeded\n" ));
   return BOS_STATUS_OK;

} /* bosSocketGetPeerName */

/***************************************************************************/

#if BOS_CFG_RESET_SUPPORT

BOS_STATUS bosSocketSelectR
(
   int             maxSockets,
   BOS_SOCKET_SET *readset,
   BOS_SOCKET_SET *writeset,
   BOS_SOCKET_SET *exceptset,
   BOS_TIME_MS     timeout,
   int            *numDesc
)
{
   BOS_STATUS     retval;
   BOS_SOCKET_SET origRdSet;
   BOS_SOCKET_SET origWrSet;
   BOS_SOCKET_SET origExSet;
   BOS_BOOL       bRdSet;
   BOS_BOOL       bWrSet;
   BOS_BOOL       bExSet;
#if BOS_CFG_TIME_FOR_TIMED_FUNCTION
   BOS_TIME_MS start;
   BOS_TIME_MS now;
   BOS_TIME_MS delta = 0;
   BOS_TIME_MS localTimeout;
   BOS_TIME_MS timeRemaining = timeout;
   bosTimeGetMs(&start);
#endif


   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketSelectR called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "       maxSockets = %d\n", maxSockets ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             time = %u\n", timeout ));

   bRdSet = ( readset != NULL );
   bWrSet = ( writeset != NULL );
   bExSet = ( exceptset != NULL );

   /*
   ** Save the original fdsets to restore after multiple select() calls
   */
   if ( bRdSet ) origRdSet = *readset;
   if ( bWrSet ) origWrSet = *writeset;
   if ( bExSet ) origExSet = *exceptset;

   /*
   ** Repeat the select loop until the true timeout expires, a reset occurs, or select succeeds
   */
   retval = BOS_STATUS_RESET;
   while ( !bosResourceIsResetPending() )
   {
      /* Restore the original sets */
      if ( bRdSet ) *readset = origRdSet;
      if ( bWrSet ) *writeset = origWrSet;
      if ( bExSet ) *exceptset = origExSet;
#if BOS_CFG_TIME_FOR_TIMED_FUNCTION
      timeRemaining = timeout - delta;
      if (timeRemaining > BOS_CFG_RESET_GRANULARITY )
      {
         localTimeout = BOS_CFG_RESET_GRANULARITY;
      }
      else
      {
         localTimeout = timeRemaining;
      }
      retval = bosSocketSelect( maxSockets, readset, writeset, exceptset, localTimeout, numDesc );
      if (retval == BOS_STATUS_TIMEOUT)
      {
         bosTimeGetMs(&now);
         bosTimeCalcDeltaMs(&start, &now, &delta);
         if (timeout > delta)
         {
            continue; /* loop again */
         }
         else
         {
            break; /* BOS_STATUS_TIMEOUT */
         }
      }
      else
      {
         break; /* BOS_STATUS_OK or BOS_STATUS_ERR */
      }
#else
      if (( timeout != BOS_WAIT_FOREVER ) && ( timeout <= BOS_CFG_RESET_GRANULARITY ))
      {
         /* timeout is less than the reset granularity, select normally */
         retval = bosSocketSelect( maxSockets, readset, writeset, exceptset, timeout, numDesc );
         break;
      }
      else
      {
         /* Timeout is more than reset granularity, decrement timeout remaining
         ** and timeout for the maximum allowed before checking for a reset */
         if ( timeout != BOS_WAIT_FOREVER )
         {
            timeout -= BOS_CFG_RESET_GRANULARITY;
         }
         retval = bosSocketSelect( FD_SETSIZE, readset, writeset, exceptset, BOS_CFG_RESET_GRANULARITY, numDesc );
         if ( retval != BOS_STATUS_TIMEOUT )
         {
            /* Anything but the early timeout is returned to the caller */
            break;
         }
      }
#endif
      retval = BOS_STATUS_RESET;
   }
   return ( retval );

} /* bosSocketSelectR */


/***************************************************************************/

BOS_STATUS bosSocketRecvFromR( BOS_SOCKET       *sd,
                              char             *buf,
                              int               buflen,
                              int               flags,
                              BOS_SOCKET_ADDR  *from,
                              int              *fromLen,
                              int              *numBytesRec)
{

   BOS_SOCKET_SET   fdSet;
   BOS_STATUS   status;
   int         nDesc;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   BOS_SOCKET_ADDR_IN6   *sin6;
#endif

   /* Call bosSocketRecvFrom directly for non-blocking socket */
   if ( !sd->isNonBlocking )
   {

       /* Clear the file descriptor set */
       bosSocketSetZero( &fdSet );

       /* Add receive socket to the file descriptor set */
       bosSocketSetAddSocket( sd, &fdSet );


      /* Block until data is ready to be received from the socket */
      status = bosSocketSelectR( FD_SETSIZE, &fdSet, NULL, NULL, BOS_WAIT_FOREVER, &nDesc );
      if ( status != BOS_STATUS_OK )
      {
         return ( status );
      }
   }

   /* Read received data from the socket */
   status = bosSocketRecvFrom( sd, buf, buflen, flags, from, fromLen, (size_t *)numBytesRec );

#if BOS_CFG_IP_ADDR_V6_SUPPORT
   sin6 = (BOS_SOCKET_ADDR_IN6 *)from;
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecvFromR called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             socket = %d\n",
                GetSocket( sd ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  from.family = %d\n",
                sin6->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    from.port = %d\n",
                bosSocketHtoNS( sin6->sin6_port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    from.flowinfo = %d\n",
                bosSocketHtoNL( sin6->sin6_flowinfo )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "    from.scope_id = %d\n",
                bosSocketHtoNL( sin6->sin6_scope_id )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "  from.ipAddr = %x:%x:%x:%x:%x:%x:%x:%x.\n",
           sin6->sin6_addr.s6Addr[0],
           sin6->sin6_addr.s6Addr[1],
           sin6->sin6_addr.s6Addr[2],
           sin6->sin6_addr.s6Addr[3],
           sin6->sin6_addr.s6Addr[4],
           sin6->sin6_addr.s6Addr[5],
           sin6->sin6_addr.s6Addr[6],
           sin6->sin6_addr.s6Addr[7]
           ));
#else

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecvFromR called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             socket = %d\n",
                GetSocket( sd ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             nonblocking = %d\n",
                sd->isNonBlocking ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "        from.family = %d\n",
                from->family ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "          from.port = %d\n",
                bosSocketHtoNS( from->port )));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "        from.ipAddr = %d.%d.%d.%d\n",
                                                    from->ipAddr.u8[ 0 ],
                                                    from->ipAddr.u8[ 1 ],
                                                    from->ipAddr.u8[ 2 ],
                                                    from->ipAddr.u8[ 3 ] ));
#endif


   return ( status );

} /* bosSocketRecvFromR */

/***************************************************************************/
BOS_STATUS bosSocketRecvFromIpR
(
 BOS_SOCKET          *sd,
 char                *buf,
 int                  buflen,
 int                  flags,
 BOS_IP_ADDRESS       *from,
 BOS_SOCKET_IP_PORT   *port,
 int                  *numBytesRec
)
{

   BOS_SOCKET_SET   fdSet;
   BOS_STATUS   status;
   int         nDesc;
   char        tmpstr[64];

   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "bosSocketRecvFromIpR called\n" ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             socket = %d\n",
                GetSocket( sd ) ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "             nonblocking = %d\n",
                sd->isNonBlocking ));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "        from.family = %d\n",
                bosIpAddrGetType(from)));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "          from.port = %d\n",
                *port ));

   bosIpAddrGetStr(from, tmpstr, sizeof(tmpstr));
   blogStrUsr(( gBosBlog, BOS_LOG_SOCKET_PARAMS, "        from.ipAddr = %s.\n",
            tmpstr));

   /* Call bosSocketRecvFrom directly for non-blocking socket */
   if ( !sd->isNonBlocking )
   {

       /* Clear the file descriptor set */
       bosSocketSetZero( &fdSet );

       /* Add receive socket to the file descriptor set */
       bosSocketSetAddSocket( sd, &fdSet );


      /* Block until data is ready to be received from the socket */
      status = bosSocketSelectR( FD_SETSIZE, &fdSet, NULL, NULL, BOS_WAIT_FOREVER, &nDesc );
      if ( status != BOS_STATUS_OK )
      {
         return ( status );
      }
   }

   /* Read received data from the socket */
   return ( bosSocketRecvFromIp( sd, buf, buflen, flags, from, port, (size_t *)numBytesRec ));

} /* bosSocketRecvFromIpR */

#endif   /* BOS_CFG_RESET_SUPPORT */

/** @} */

#endif   /* BOS_SOCKET_USE_GENERIC_IMPLEMENTATION */
#endif   /* BOS_OS_LINUXKERNEL */
#endif   /* BOS_CFG_SOCKET */

