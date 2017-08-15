/****************************************************************************
*
* Copyright © 2002-2008 Broadcom
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
*  @file    turnc.h
*
*  @brief   TURNC Client header file
*
*  This file contains software interface information for the TURNC Client
*  Module
*/
/****************************************************************************/

#ifndef TURNC_H    /* support nested includes */
#define TURNC_H

/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <stun.h>

/* ---- Constants and Types ---------------------------------- */
#define TURNC_VERSION               "BRCM TURNC 2.0"
#define TURNC_MAX_ACTIVE_DEST       1
#define TURNC_MAX_DATA_SIZE         128

/* Request sending options */

/* TURN Client status */
typedef enum
{
   TURNC_SUCCESS = 0,
   TURNC_FAILURE
} TURNC_STATUS;

/* TURNC TRANSPORT */
#define TURNC_UDP    0x00000000
#define TURNC_TCP    0x00000001

/*  TURNC PORT Properties */
#define TURNC_NO_ALIGN                0x00000000
#define TURNC_ODD_PORT                0x00000001
#define TURNC_EVEN_PORT               0x00000002
#define TURNC_EVEN_PORT_RES_NEXT      0x00000003

/*
 * ALLOCATE Request options
 */
typedef struct
{
   BOS_UINT32           *pBandwidth;      /* Bandwidth
                                           * set to NULL if not used
                                           */

   BOS_UINT32           *pLifetime;       /* Lifetime
                                           * set to NULL if not used
                                           */

   BOS_UINT32           *pReqdTransport;  /* Requested Transport,
                                           * set to NULL if not used
                                           */

   STUN_TRANSPORT       *pReqdIPAddr;     /* Requested IP Address,
                                           * set to NULL if not used
                                           */

   BOS_UINT32           *pPortProps;      /* Port Properties,
                                           * set to NULL if not used
                                           */
} TURNC_ALLOCATE_OPTS;

/* TURNC CONNECT STATUS */
typedef enum
{
   TURNC_CONNECT_LISTEN,
   TURNC_CONNECT_ESTABLISHED,
   TURNC_CLOSED
} TURNC_CONNECT_STATUS;

/* TURN Client Events */
typedef enum
{
   TURNC_EVT_ALLOCATE_SUCCESS,         /* pData = TURNC_ALLOCATE_DATA */
   TURNC_EVT_ALLOCATE_ERROR,           /* pData = TURNC_ERR */
   TURNC_EVT_ACTIVE_DEST_SET,          /* pData = NULL */
   TURNC_EVT_ACTIVE_DEST_UNSET,        /* pData = NULL */
   TURNC_EVT_ACTIVE_DEST_ERROR,        /* pData = TURNC_ERR */
   TURNC_EVT_CONNECT_SUCCESS,          /* pData = NULL */
   TURNC_EVT_CONNECT_ERROR,            /* pData = TURNC_ERR */
   TURNC_EVT_CONNECT_STATUS,           /* pData = TURNC_CONNECT_STATUS */
   TURNC_EVT_TX_TIMEOUT,               /* pData = NULL */
   TURNC_EVT_SYSTEM_ERR,               /* pData = NULL */
   TURNC_EVT_NUM
} TURNC_EVT;


/* data for successful ALLOCATE response */
typedef struct
{
   STUN_TRANSPORT    relayAddr;
   STUN_TRANSPORT    reflexiveAddr;
} TURNC_ALLOCATE_DATA;

typedef struct
{
   BOS_UINT8   buf[TURNC_MAX_DATA_SIZE];
   BOS_UINT16  len;
} TURNC_DATA;

typedef struct
{
   BOS_UINT32  errnoa;
   TURNC_DATA  reason;
} TURNC_ERR;

typedef struct
{
   TURNC_EVT   evt;
   union
   {
      TURNC_ALLOCATE_DATA  allocAddrs;
      TURNC_ERR            err;
      TURNC_CONNECT_STATUS status;
   } data;
} TURNC_EVT_DATA;

/* Shared secret */
typedef struct
{
   TURNC_DATA  username;
   TURNC_DATA  password;
} TURNC_SHARED_SECRET;

/*****************************************************************************
*  FUNCTION:   TURNC_EVTCB - TURN event Callback
*
*  PURPOSE:    A callback function for receiving TURN events.
*
*  PARAMETERS:
*              evt     [IN] - evt type
*              pData   [IN] - event data
*              reqHdl  [IN] - request handle of the original request
*
*  RETURNS:    TURNC_STATUS
*
*  NOTES:
*
*****************************************************************************/
typedef void (*TURNC_EVTCB)( const BOS_UINT32      reqHdl,
                             const TURNC_EVT_DATA  *pEvtData );

/*****************************************************************************
*  FUNCTION:   TURNC_SENDCB - TURN send data callback
*
*  PURPOSE:    A callback function for sending DATA.
*
*  PARAMETERS:
*              sid     [IN] - socket
*              pDest   [IN] - destination
*              pData   [IN] - data
*              len     [IN] - data len
*
*  RETURNS:    TURNC_STATUS
*
*  NOTES:
*
*****************************************************************************/
typedef void (*TURNC_SENDCB)( const BOS_UINT32        sid,
                              const STUN_TRANSPORT    *pDest,
                              const BOS_UINT8         *pData,
                              const BOS_UINT16        len);

/*****************************************************************************
*  FUNCTION:   TURNC_RECVCB - TURN receive data callback
*
*  PURPOSE:    A callback function for receiving DATA.
*
*  PARAMETERS:
*              sid      [IN] - socket
*              pSrcAddr [IN] - source
*              pData    [IN] - data
*              len      [IN] - data len
*
*  RETURNS:    TURNC_STATUS
*
*  NOTES:
*
*****************************************************************************/
typedef void (*TURNC_RECVCB)( const BOS_UINT32        sid,
                              const STUN_TRANSPORT    *pSrcAddr,
                              const BOS_UINT8         *pData,
                              const BOS_UINT16        len);

/*****************************************************************************
*
*  FUNCTION:   TURNC_LOGCB - log handler
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
typedef void (*TURNC_LOGCB)( const char *format, ... );


/* TURN client configuration */
typedef struct
{
   TURNC_EVTCB       evtCB;      /* event callback */
   TURNC_SENDCB      sendCB;     /* Send data callback */
   TURNC_RECVCB      recvCB;     /* Receive data callback */
   TURNC_LOGCB       logCB;      /* Logger callback */
} TURNC_CFG;

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
*  FUNCTION:   turnStartup
*
*  PURPOSE:    Configure and startup TURN Client software module
*
*  PARAMETERS:
*          turncCfgp - pointer to configuration parameters
*
*
*  RETURNS:    TURNC_SUCCESS if configuration successful. TURNC_FAILURE
*              otherwise.
*
*  NOTES:
*
*****************************************************************************/
TURNC_STATUS turncStartup( const TURNC_CFG *pTurncCfg );

/*****************************************************************************
*
*  FUNCTION:   turncShutdown
*
*  PURPOSE:    Shutdown TURN Client software module and free resources.
*
*  PARAMETERS:
*              none.
*
*
*  RETURNS:    TURNC_SUCESSS if OK. TURNC_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
TURNC_STATUS turncShutdown( void );

/*****************************************************************************
*
*  FUNCTION:   turncAllocate
*
*  PURPOSE:    Send Allocate Request to a STUN Relay server
*
*  PARAMETERS:
*           sid     [IN] - socket id for sending the request
*           pDest   [IN] - Destination IP port (network byte ordered)
*           pSecret [IN] - shared secret
*           pOptions[IN] - attribute options
*           pReqHdl [OUT]- Request Handle
*
*  RETURNS:    TURNC_SUCCESS
*
*  NOTES:
*
*****************************************************************************/
TURNC_STATUS turncAllocate( const BOS_UINT32            sid,
                            const STUN_TRANSPORT        *pDest,
                            const TURNC_SHARED_SECRET   *pSecret,
                            const TURNC_ALLOCATE_OPTS   *pOptions,
                            BOS_UINT32                  *pReqHdl );

/*****************************************************************************
*
*  FUNCTION:   turncDeallocate
*
*  PURPOSE:    Explicitly remove prior allocation.
*
*  PARAMETERS:
*           reqHdl [IN]- Request Handle
*
*  RETURNS:    TURNC_SUCCESS
*
*  NOTES:
*
*****************************************************************************/
TURNC_STATUS turncDeallocate( BOS_UINT32 reqHdl );

/*****************************************************************************
*
*  FUNCTION:   turncActiveDestination
*
*  PURPOSE:    Send Set Active Destination Request to a STUN Relay server.
*
*  PARAMETERS:
*           sid     [IN] - socket id for sending the request
*           pDest   [IN] - Destination IP port (network byte ordered)
*           pSecret [IN] - shared secret
*           pOptions[IN] - attribute options
*           pReqHdl [OUT]- Request Handle
*
*  RETURNS:    TURNC_SUCCESS
*
*  NOTES:
*
*****************************************************************************/
TURNC_STATUS turncActiveDestination( const BOS_UINT32          sid,
                                     const STUN_TRANSPORT      *pDest,
                                     const TURNC_SHARED_SECRET *pSecret,
                                     const STUN_TRANSPORT      *pRemoteAddr,
                                     BOS_UINT32                *pReqHdl );

/*****************************************************************************
*
*  FUNCTION:   turncConnect
*
*  PURPOSE:    Send Connect Request to a STUN Relay server.
*
*  PARAMETERS:
*           sid         [IN] - socket id for sending the request
*           pDest       [IN] - Destination IP port (network byte ordered)
*           pSecret     [IN] - shared secret
*           pRemoteAddr [IN] - Remote Address
*           pReqHdl     [OUT]- Request Handle
*
*  RETURNS:    TURNC_SUCCESS
*
*  NOTES:
*
*****************************************************************************/
TURNC_STATUS turncConnect( const BOS_UINT32          sid,
                           const STUN_TRANSPORT      *pDest,
                           const TURNC_SHARED_SECRET *pSecret,
                           const STUN_TRANSPORT      *pRemoteAddr,
                           BOS_UINT32                *pReqHdl );

/*****************************************************************************
*
*  FUNCTION:   turncSendData
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
*           sid         [IN] - socket id for sending the request
*           pDest       [IN] - Destination IP port (network byte ordered)
*           pRemoteAddr [IN] - remote peer
*           pData       [IN] - data to be sent
*           len         [IN] - len of data
*
*  RETURNS:    TURNC_SUCCESS if sending is successful, TURNC_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
TURNC_STATUS turncSendData( const BOS_UINT32        sid,
                            const STUN_TRANSPORT    *pDest,
                            const STUN_TRANSPORT    *pRemoteAddr,
                            const BOS_UINT8         *pData,
                            const BOS_UINT16        len );

/*****************************************************************************
*
*  FUNCTION:   turncRecvPacket
*
*  PURPOSE:    Process TURN packets received from network.
*
*  PARAMETERS:
*           sid      [IN] - socket id where packet is received
*           pSrcAddr [IN] - source address
*           pData    [IN] - data
*           len      [IN] - data length in bytes
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void turncRecvPacket( const BOS_UINT32       sid,
                      const STUN_TRANSPORT   *pSrcAddr,
                      const BOS_UINT8        *pData,
                      const BOS_UINT16       len );
#if defined(__cplusplus)
}
#endif
#endif
 

