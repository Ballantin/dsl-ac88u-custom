/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosSocketLinuxKernel.h
*
*  @brief   LinuxKernel specific definitions for the BOS Socket module.
*
****************************************************************************/

#if !defined( BOSSOCKETLINUXKERNEL_H )
#define BOSSOCKETLINUXKERNEL_H         /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/socket.h>
#include <linux/in.h>      /* Needed for INADDR_ANY in bosSocket.h */

#if BOS_CFG_IP_ADDR_V6_SUPPORT
#include <linux/in6.h>     /* Needed for socket datagram constants */
#endif
/**
 * @addtogroup bosSocket
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */


/**
 * Specifies that the generic socket interface file (generic/bosSocketGeneric.c)
 * can be used for the bulk of the socket implementation.
 */

#define  BOS_SOCKET_USE_GENERIC_IMPLEMENTATION  1

/**
 * This value is considered to be an "error" by LinuxKernel.
 */

#define  BOS_SOCKET_INVALID   (-1)

/**
 * LinuxKernel version of a BOS_SOCKET.
 */
typedef struct
{
   int         s;                /* Socket descriptor                         */
   BOS_BOOL    isNonBlocking;    /* True means socket is in non-blocking mode */
#if BOS_CFG_SOCKET_INTERFACE_SUPPORT
   int         ifno;             /* Socket's interface number                 */
#endif
   int          family;           /* Ipv4 or IPv6 */
} BOS_SOCKET;

#define bosSocketGetNativeId( pBosSocket )  ((pBosSocket)->s)

#if BOS_CFG_SOCKET_INTERFACE_SUPPORT
   #define BOS_SOCKET_IFNO_INVALID (-1)
   #define bosSocketSetInterfaceFlags( pSocket, flags )     /* empty */
   #define bosSocketSetInterfaceAddr( pSocket, pAddr )      /* empty */
#endif

/**
 * LinuxKernel version of a BOS_SOCKET_SET
 */
typedef int    BOS_SOCKET_ID;

typedef fd_set   BOS_SOCKET_SET;

typedef int socklen_t;

/* Setsockopt flags used to specify an interface to send directly to without
 * going through the LinuxKernel routing table */
#define BCM0_IFFLAG    0x0400
#define BCM1_IFFLAG    0x0800
#define BCM2_IFFLAG    0x0c00


/**
 * Error type appropriate for LinuxKernel.
 */

typedef int BOS_SOCKET_ERROR_TYPE;

/**
 * LinuxUser Socket error return value.
 */

#define  BOS_SOCKET_ERROR  SOCKET_ERROR

#define  bosSocketClearLastError()

/* Since sockets function should not be called in LinuxKernel
** always retun error "operation not permitted" when bosSocketGetLastError is called */
#define  bosSocketGetLastError()   EPERM

/* Redefine the standard socket functions to return -1 (error)
** since we don't want to use sockets in LinuxKernel */
#define closesocket(s)                                (-1)
#define ioctlsocket(s,c,v)                            (-(v == v))
#define accept(s, addr, addrlen)                      (-1)
#define bind(s, addr, addrlen)                        (-1)
#define close(s)                                      (-1)
#define connect(s, addr, addrlen)                     (-1)
#define socket(domain, type, protocol)                (-1)
#define getsockopt(s, level, optname, optval, optlen) (-1)
#define setsockopt(s, level, optname, optval, optlen) (-1)
#define listen(s, backlog)                            (-1)
#define recv(s, buf, len, flags)                      (-1)
#define recvfrom(s, buf, len, flags, from, fromlen)   (-1)
#define select(n, readfds, writefds, exceptfds, tout) (-1)
#define send(s, msg, len, flags)                      (-1)
#define sendto(s, msg, len, flags, to, tolen)         (-1)
#define shutdown(s, how)                              (-1)
#define getsockname(s,name,namelen)                   (-1)
#define getpeername(s,name,namelen)                   (-1)

#define ioctl(s,c,v) -1

#define strtol simple_strtol

#define IPPROTO_IP  -1
#define IPPROTO_TCP -1
#define TCP_NODELAY -1

/***************************************************************************/
/**
*  Converts an unsigned 32 bit number from host order to network order.
*/

#define  bosSocketHtoNL( num )   (htonl( num ))

/***************************************************************************/
/**
*  Converts an unsigned 16 bit number from host order to network order.
*/

#define  bosSocketHtoNS( num )   (htons( num ))

/***************************************************************************/
/**
*  Converts an unsigned 32 bit number from network order to host order.
*/

#define  bosSocketNtoHL( num )   (ntohl( num ))

/***************************************************************************/
/**
*  Converts an unsigned 16 bit number from network order to host order.
*/

#define  bosSocketNtoHS( num )   (ntohs( num ))


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSSOCKETLINUXKERNEL_H */

