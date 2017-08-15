/**********************************************************************************
** Copyright © 2009 Broadcom Corporation
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

/* ---- Include Files ----------------------------------------------------------- */

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

#define SC_CORE__UNASSIGNED_SLOT             -1

/* Define the maximum number of socket information one can cache at any given
** time.
*/
#define SC_CORE__MAX_SOCKET_INFO             10

/* Defines the socket type that is associated with a particular socket
** information.
*/
typedef enum
{
   SCSKTTYPE_UNASSIGNED,
   SCSKTTYPE_UDP,
   SCSKTTYPE_TCP,
   SCSKTTYPE_TLS,

} SCSKTTYPE;


/* Base structure which maps the socket information to the actual IP address and
** port associated with this socket.
*/
typedef struct
{
   MXD_GNS::mxt_hSocket  hSocket;
   SCSKTTYPE             type;
   CCTKIPPORT            local;
   CCTKIPPORT            peer;

} SCINFO;


/* ---- Private Variables ------------------------------------------------------- */

static SCINFO scInfoCache[ SC_CORE__MAX_SOCKET_INFO ];


/* ---- Private Function Prototypes --------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   nsScUdpBindCB
**
**  PURPOSE:    Bind callback for UDP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pLocalAddr - the local address this socket is bound to.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScUdpBindCB( MXD_GNS::mxt_hSocket socket,
                           const MXD_GNS::CSocketAddr *pLocalAddr );


/***********************************************************************************
**  FUNCTION:   nsScTcpBindCB
**
**  PURPOSE:    Bind callback for TCP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pLocalAddr - the local address this socket is bound to.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScTcpBindCB( MXD_GNS::mxt_hSocket socket,
                           const MXD_GNS::CSocketAddr *pLocalAddr );


/***********************************************************************************
**  FUNCTION:   nsScTcpServerBindCB
**
**  PURPOSE:    Bind callback for TCP server socket (TCP socket connecting to us).
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pLocalAddr - the local address this socket is bound to.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScTcpServerBindCB( MXD_GNS::mxt_hSocket socket,
                                 const MXD_GNS::CSocketAddr *pLocalAddr );


/***********************************************************************************
**  FUNCTION:   nsScTcpSendCB
**
**  PURPOSE:    Send callback for packet sent over TCP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pData - the data sent over the socket.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScTcpSendCB( MXD_GNS::mxt_hSocket socket,
                           const MXD_GNS::CBlob *pData );


/***********************************************************************************
**  FUNCTION:   nsScUdpSendCB
**
**  PURPOSE:    Send callback for packet sent over UDP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pData - the data sent over the socket.
**              puSize - the size of the data sent over the socket.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScUdpSendCB( MXD_GNS::mxt_hSocket socket,
                           uint8_t **ppData,
                           unsigned int *puSize );


/***********************************************************************************
**  FUNCTION:   nsScSendToCB
**
**  PURPOSE:    Send callback for packet sent over UDP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pPeerAddr - the remote address information.
**              pData - the data sent over the socket.
**              puSize - the size of the data sent over the socket.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScSendToCB( MXD_GNS::mxt_hSocket socket,
                          const MXD_GNS::CSocketAddr *pPeerAddr,
                          uint8_t **ppData,
                          unsigned int *puSize );


/***********************************************************************************
**  FUNCTION:   nsScRecvCB
**
**  PURPOSE:    Receive callback for packet sent over TCP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              puData - the data received.
**              uCapacity - the maximum size of the data that can be received.
**              nSizeReceived - the actual amount of data received.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScRecvCB( MXD_GNS::mxt_hSocket socket,
                        const uint8_t *puData,
                        const unsigned int uCapacity,
                        int *nSizeReceived );


/***********************************************************************************
**  FUNCTION:   nsScRecvFromCB
**
**  PURPOSE:    Receive callback for packet sent over UDP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pPeerAddr - the remote address information.
**              puData - the data received.
**              uCapacity - the maximum size of the data that can be received.
**              nSizeReceived - the actual amount of data received.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScRecvFromCB( MXD_GNS::mxt_hSocket socket,
                            const MXD_GNS::CSocketAddr *pPeerAddr,
                            const uint8_t *puData,
                            const unsigned int uCapacity,
                            int *nSizeReceived );


/***********************************************************************************
**  FUNCTION:   nsScConnectCB
**
**  PURPOSE:    Connect callback for socket operation
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pPeerAddr - the remote address information.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScConnectCB( MXD_GNS::mxt_hSocket socket,
                           const MXD_GNS::CSocketAddr *pPeerAddr );


/***********************************************************************************
**  FUNCTION:   nsScAcceptCB
**
**  PURPOSE:    Accept callback for socket operation
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pPeerAddr - the remote address information.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScAcceptCB( MXD_GNS::mxt_hSocket socket,
                          const MXD_GNS::CSocketAddr *pPeerAddr );


/***********************************************************************************
**  FUNCTION:   nsScCloseCB
**
**  PURPOSE:    Close callback for socket operation
**
**  PARAMETERS:
**              socket - the socket identifier.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScCloseCB( MXD_GNS::mxt_hSocket socket );


/***********************************************************************************
**  FUNCTION:   nsScAdd2Cache
**
**  PURPOSE:    Adds a socket information received from the stack to the cache
**              information that we store for this socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              eType - the socket type being allocated.
**              addr - the corresponding address information.
**              bLocal - whether this is setting the local address information
**                       or the remote one.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScAdd2Cache( MXD_GNS::mxt_hSocket socket,
                           SCSKTTYPE eType,
                           CCTKIPPORT &addr,
                           BOS_BOOL bLocal );


/***********************************************************************************
**  FUNCTION:   nsScRemFromCache
**
**  PURPOSE:    Removes a socket information from the cache data.
**
**  PARAMETERS:
**              socket - the socket identifier.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
static void nsScRemFromCache( MXD_GNS::mxt_hSocket socket );


/***********************************************************************************
**  FUNCTION:   nsScLkupCache
**
**  PURPOSE:    Lookups a socket information from the cache data.
**
**  PARAMETERS:
**              socket - the socket identifier.
**
**
**  RETURNS:    The index for the socket in cache if found, 
**              SC_CORE__UNASSIGNED_SLOT otherwise.
**
**  NOTES:      .
***********************************************************************************/
static BOS_SINT32 nsScLkupCache( MXD_GNS::mxt_hSocket socket );


/***********************************************************************************
**  FUNCTION:   nsScCacheLocal
**
**  PURPOSE:    Checks whether a socket cached identified by the index corresponds
**              to a local address or not.
**
**  PARAMETERS:
**              uIx - the index for the cached socket.
**
**
**  RETURNS:    BOS_TRUE if the cached socket corresponds to a local address,
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
static BOS_BOOL nsScCacheLocal( BOS_UINT32 uIx );


/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   nsScInit
**
**  PURPOSE:    Socket Control module initialization.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    None
**
**  NOTES:      This function initializes the socket control module.
***********************************************************************************/
void nsScInit( void )
{
   /* Mapping the UDP socket operation callbacks.
   */
   MXD_GNS::g_CUdpSocket_BindCB = (MXD_GNS::CUDPSOCKET_BIND)nsScUdpBindCB;
   MXD_GNS::g_CUdpSocket_SendToCB = (MXD_GNS::CUDPSOCKET_SENDTO)nsScSendToCB;
   MXD_GNS::g_CUdpSocket_ConnectCB = (MXD_GNS::CUDPSOCKET_CONNECT)nsScConnectCB;
   MXD_GNS::g_CUdpSocket_SendCB = (MXD_GNS::CUDPSOCKET_SEND)nsScUdpSendCB;
   MXD_GNS::g_CUdpSocket_RecvFromCB = (MXD_GNS::CUDPSOCKET_RECVFROM)nsScRecvFromCB;
   MXD_GNS::g_CUdpSocket_CloseCB = (MXD_GNS::CUDPSOCKET_CLOSE)nsScCloseCB;

   /* Mapping the TCP socket operation callbacks.
   */
   MXD_GNS::g_CTcpSocket_BindCB = (MXD_GNS::CTCPSOCKET_BIND)nsScTcpBindCB;
   MXD_GNS::g_CTcpServerSocket_BindCB = (MXD_GNS::CTCPSERVERSOCKET_BIND)nsScTcpServerBindCB;
   MXD_GNS::g_CTcpSocket_CloseCB = (MXD_GNS::CTCPSOCKET_CLOSE)nsScCloseCB;
   MXD_GNS::g_CTcpSocket_ConnectCB = (MXD_GNS::CTCPSOCKET_CONNECT)nsScConnectCB;
   MXD_GNS::g_CTcpSocket_SendCB = (MXD_GNS::CTCPSOCKET_SEND)nsScTcpSendCB;
   MXD_GNS::g_CTcpServerSocket_AcceptCB = (MXD_GNS::CTCPSERVERSOCKET_ACCEPT)nsScAcceptCB;
   MXD_GNS::g_CTcpSocket_RecvCB = (MXD_GNS::CTCPSOCKET_RECV)nsScRecvCB;

   memset ( scInfoCache, 0, sizeof ( scInfoCache ) );
}


/***********************************************************************************
**  FUNCTION:   nsScUdpBindCB
**
**  PURPOSE:    Bind callback for UDP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pLocalAddr - the local address this socket is bound to.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScUdpBindCB( MXD_GNS::mxt_hSocket socket,
                    const MXD_GNS::CSocketAddr *pLocalAddr )
{
   CCTKIPPORT local;

   cfgCvtSockAddrFromMx( *pLocalAddr, local );

   nsScAdd2Cache( socket,
                  SCSKTTYPE_UDP,
                  local,
                  BOS_TRUE );
}


/***********************************************************************************
**  FUNCTION:   nsScTcpBindCB
**
**  PURPOSE:    Bind callback for TCP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pLocalAddr - the local address this socket is bound to.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScTcpBindCB( MXD_GNS::mxt_hSocket socket,
                    const MXD_GNS::CSocketAddr *pLocalAddr )
{
   CCTKIPPORT local;

   cfgCvtSockAddrFromMx( *pLocalAddr, local );

   nsScAdd2Cache( socket,
                  SCSKTTYPE_TCP,
                  local,
                  BOS_TRUE );
}


/***********************************************************************************
**  FUNCTION:   nsScTcpServerBindCB
**
**  PURPOSE:    Bind callback for TCP server socket (TCP socket connecting to us).
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pLocalAddr - the local address this socket is bound to.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScTcpServerBindCB( MXD_GNS::mxt_hSocket socket,
                          const MXD_GNS::CSocketAddr *pLocalAddr )
{
   CCTKIPPORT local;

   cfgCvtSockAddrFromMx( *pLocalAddr, local );

   nsScAdd2Cache( socket,
                  SCSKTTYPE_TCP,
                  local,
                  BOS_TRUE );
}


/***********************************************************************************
**  FUNCTION:   nsScTcpSendCB
**
**  PURPOSE:    Send callback for packet sent over TCP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pData - the data sent over the socket.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScTcpSendCB( MXD_GNS::mxt_hSocket socket,
                    const MXD_GNS::CBlob *pData )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( socket );

   BOS_UINT8 *pNewData = NULL;
   BOS_UINT32 uLen;
   MXD_GNS::CBlob *pTmp = (MXD_GNS::CBlob *)pData;
   BOS_SINT32 sIx = SC_CORE__UNASSIGNED_SLOT;

   uLen = pData->GetSize();

   if ( ((sIx = nsScLkupCache( socket )) != SC_CORE__UNASSIGNED_SLOT) &&
        !nsScCacheLocal( sIx ) )
   {
      /* Modify possibly the content of the message at this time, we need to
      ** allocate a new message buffer containing the information we would
      ** want to be sent instead of the one currently being sent out.
      */
      uLen = pmLastChance( &pNewData,
                           &scInfoCache[ sIx ].peer,
                           const_cast<BOS_UINT8 *> (pData->GetFirstIndexPtr()),
                           uLen );
   }
   else
   {
      uLen = 0;
   }

   /* Modify possibly the content of the message at this time, we need to
   ** allocate a new message buffer containing the information we would
   ** want to be sent instead of the one currently being sent out.
   */
   if ( uLen && ( pNewData != NULL ) )
   {
      pTmp->EraseAll();
      pTmp->Resize( static_cast<unsigned int> (uLen) );
      memcpy( (void *)(pData->GetFirstIndexPtr()),
              (void *)(pNewData),
              uLen );
   }
}


/***********************************************************************************
**  FUNCTION:   nsScUdpSendCB
**
**  PURPOSE:    Send callback for packet sent over UDP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pData - the data sent over the socket.
**              puSize - the size of the data sent over the socket.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScUdpSendCB( MXD_GNS::mxt_hSocket socket,
                    uint8_t **ppData,
                    unsigned int *puSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( socket );

   BOS_UINT8 *pNewData = NULL;
   BOS_UINT32 uLen = static_cast<BOS_UINT32> (*puSize);
   BOS_SINT32 sIx = SC_CORE__UNASSIGNED_SLOT;

   if ( ((sIx = nsScLkupCache( socket )) != SC_CORE__UNASSIGNED_SLOT) &&
        !nsScCacheLocal( sIx ) )
   {
      /* Modify possibly the content of the message at this time, we need to
      ** allocate a new message buffer containing the information we would
      ** want to be sent instead of the one currently being sent out.
      */
      uLen = pmLastChance( &pNewData,
                           &scInfoCache[ sIx ].peer,
                           const_cast<BOS_UINT8 *> (*ppData),
                           uLen );
   }
   else
   {
      uLen = 0;
   }

   if ( uLen && ( pNewData != NULL ) )
   {
      *puSize = static_cast<unsigned int> (uLen);
      *ppData = static_cast<uint8_t *> (pNewData);
   }
}


/***********************************************************************************
**  FUNCTION:   nsScSendToCB
**
**  PURPOSE:    Send callback for packet sent over UDP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pPeerAddr - the remote address information.
**              pData - the data sent over the socket.
**              puSize - the size of the data sent over the socket.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScSendToCB( MXD_GNS::mxt_hSocket socket,
                   const MXD_GNS::CSocketAddr *pPeerAddr,
                   uint8_t **ppData,
                   unsigned int *puSize )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( socket );

   BOS_UINT8 *pNewData = NULL;
   BOS_UINT32 uLen = static_cast<BOS_UINT32> (*puSize);
   BOS_SINT32 sIx = SC_CORE__UNASSIGNED_SLOT;

   if ( ((sIx = nsScLkupCache( socket )) != SC_CORE__UNASSIGNED_SLOT) &&
        !nsScCacheLocal( sIx ) )
   {
      CCTKIPPORT remote;

      cfgCvtSockAddrFromMx( *pPeerAddr, remote );

      /* Modify possibly the content of the message at this time, we need to
      ** allocate a new message buffer containing the information we would
      ** want to be sent instead of the one currently being sent out.
      */
      uLen = pmLastChance( &pNewData,
                           &remote,
                           const_cast<BOS_UINT8 *> (*ppData),
                           uLen );
   }

   if ( uLen && ( pNewData != NULL ) )
   {
      *puSize = static_cast<unsigned int> (uLen);
      *ppData = static_cast<uint8_t *> (pNewData);
   }
}


/***********************************************************************************
**  FUNCTION:   nsScRecvCB
**
**  PURPOSE:    Receive callback for packet sent over TCP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              puData - the data received.
**              uCapacity - the maximum size of the data that can be received.
**              nSizeReceived - the actual amount of data received.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScRecvCB( MXD_GNS::mxt_hSocket socket,
                 const uint8_t *puData,
                 const unsigned int uCapacity,
                 int *nSizeReceived )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( socket );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( puData );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uCapacity );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( nSizeReceived );
}


/***********************************************************************************
**  FUNCTION:   nsScRecvFromCB
**
**  PURPOSE:    Receive callback for packet sent over UDP socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pPeerAddr - the remote address information.
**              puData - the data received.
**              uCapacity - the maximum size of the data that can be received.
**              nSizeReceived - the actual amount of data received.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScRecvFromCB( MXD_GNS::mxt_hSocket socket,
                     const MXD_GNS::CSocketAddr *pPeerAddr,
                     const uint8_t *puData,
                     const unsigned int uCapacity,
                     int *nSizeReceived )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( socket );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pPeerAddr );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( puData );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uCapacity );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( nSizeReceived );
}


/***********************************************************************************
**  FUNCTION:   nsScConnectCB
**
**  PURPOSE:    Connect callback for socket operation
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pPeerAddr - the remote address information.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScConnectCB( MXD_GNS::mxt_hSocket socket,
                    const MXD_GNS::CSocketAddr *pPeerAddr )
{
   CCTKIPPORT remote;

   cfgCvtSockAddrFromMx( *pPeerAddr, remote );

   nsScAdd2Cache( socket,
                  SCSKTTYPE_TCP,
                  remote,
                  BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   nsScAcceptCB
**
**  PURPOSE:    Accept callback for socket operation
**
**  PARAMETERS:
**              socket - the socket identifier.
**              pPeerAddr - the remote address information.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScAcceptCB( MXD_GNS::mxt_hSocket socket,
                   const MXD_GNS::CSocketAddr *pPeerAddr )
{
   CCTKIPPORT remote;

   cfgCvtSockAddrFromMx( *pPeerAddr, remote );

   nsScAdd2Cache( socket,
                  SCSKTTYPE_TCP,
                  remote,
                  BOS_FALSE );
}


/***********************************************************************************
**  FUNCTION:   nsScCloseCB
**
**  PURPOSE:    Close callback for socket operation
**
**  PARAMETERS:
**              socket - the socket identifier.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScCloseCB( MXD_GNS::mxt_hSocket socket )
{
   nsScRemFromCache( socket );
}


/***********************************************************************************
**  FUNCTION:   nsScAdd2Cache
**
**  PURPOSE:    Adds a socket information received from the stack to the cache
**              information that we store for this socket.
**
**  PARAMETERS:
**              socket - the socket identifier.
**              eType - the socket type being allocated.
**              addr - the corresponding address information.
**              bLocal - whether this is setting the local address information
**                       or the remote one.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScAdd2Cache( MXD_GNS::mxt_hSocket socket,
                    SCSKTTYPE eType,
                    CCTKIPPORT &addr,
                    BOS_BOOL bLocal )
{
   BOS_UINT32 uIx;
   BOS_SINT32 uSlot = SC_CORE__UNASSIGNED_SLOT;

   for ( uIx = 0 ; uIx < SC_CORE__MAX_SOCKET_INFO ; uIx++ )
   {
      if ( (uSlot == SC_CORE__UNASSIGNED_SLOT ) &&
           (scInfoCache[ uIx ].type != SCSKTTYPE_UNASSIGNED) &&
           (scInfoCache[ uIx ].hSocket == socket) )
      {
         uSlot = static_cast<BOS_SINT32> (uIx);
         break;
      }
   }

   if ( uSlot == SC_CORE__UNASSIGNED_SLOT )
   {
      for ( uIx = 0 ; uIx < SC_CORE__MAX_SOCKET_INFO ; uIx++ )
      {
         if ( scInfoCache[ uIx ].type == SCSKTTYPE_UNASSIGNED )
         {
            uSlot = static_cast<BOS_SINT32> (uIx);
            break;
         }
      }
   }

   if ( uSlot != SC_CORE__UNASSIGNED_SLOT )
   {
      scInfoCache[ uSlot ].hSocket = socket;
      scInfoCache[ uSlot ].type = eType;
      memcpy ( (bLocal ? &scInfoCache[ uSlot ].local :
                           &scInfoCache[ uSlot ].peer),
               &addr,
               sizeof ( CCTKIPPORT ) );
   }
}


/***********************************************************************************
**  FUNCTION:   nsScRemFromCache
**
**  PURPOSE:    Removes a socket information from the cache data.
**
**  PARAMETERS:
**              socket - the socket identifier.
**
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void nsScRemFromCache( MXD_GNS::mxt_hSocket socket )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < SC_CORE__MAX_SOCKET_INFO ; uIx++ )
   {
      if ( scInfoCache[ uIx ].hSocket == socket )
      {
         memset ( &scInfoCache[ uIx ],
                  0,
                  sizeof ( SCINFO ) );
         break;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   nsScLkupCache
**
**  PURPOSE:    Lookups a socket information from the cache data.
**
**  PARAMETERS:
**              socket - the socket identifier.
**
**
**  RETURNS:    The index for the socket in cache if found, 
**              SC_CORE__UNASSIGNED_SLOT otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_SINT32 nsScLkupCache( MXD_GNS::mxt_hSocket socket )
{
   BOS_SINT32 sIx;

   for ( sIx = 0 ; sIx < SC_CORE__MAX_SOCKET_INFO ; sIx++ )
   {
      if ( scInfoCache[ sIx ].hSocket == socket )
      {
         return sIx;
      }
   }

   return SC_CORE__UNASSIGNED_SLOT;
}


/***********************************************************************************
**  FUNCTION:   nsScCacheLocal
**
**  PURPOSE:    Checks whether a socket cached identified by the index corresponds
**              to a local address or not.
**
**  PARAMETERS:
**              uIx - the index for the cached socket.
**
**
**  RETURNS:    BOS_TRUE if the cached socket corresponds to a local address,
**              BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL nsScCacheLocal( BOS_UINT32 uIx )
{
   BOS_BOOL bRet = BOS_FALSE;

   if ( static_cast<BOS_SINT32> (uIx) != SC_CORE__UNASSIGNED_SLOT )
   {
      if ( bosIpAddrIsZero ( &scInfoCache[ uIx ].local.addr ) )
      {
         bRet = BOS_TRUE;
      }
      else
      {
         BOS_IP_ADDRESS ipLcl;

         if ( bosIpAddrCreateLoopback ( BOS_IPADDRESS_TYPE_V4,
                                        &ipLcl ) == BOS_STATUS_OK )
         {
            if ( bosIpAddrIsEqual ( &ipLcl, &scInfoCache[ uIx ].local.addr ) )
            {
               bRet = BOS_TRUE;
            }
         }
      }
   }

   return bRet;
}
