/****************************************************************************
*
* Copyright (c) 2002-2011 Broadcom
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
*  @file    stunc.h
*
*  @brief   STUN Client header file
*
*  This file contains software interface information for the STUN Client Module
*/
/****************************************************************************/

#ifndef STUNC_H    /* support nested includes */
#define STUNC_H

/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <bosSocket.h>
#include <ntcfg.h>
#include <stun.h>

/* ---- Constants and Types ---------------------------------- */
/* Generic data size */
#define STUNC_MAX_DATA_SIZE       128

/* STUN Client status */
typedef enum
{
   STUNC_SUCCESS = 0,
   STUNC_FAILURE
} STUNC_STATUS;

/* STUN Client Events */
typedef enum
{
   STUNC_EVT_MAPPED_ADDRESS = 0, /* mapped address */
                                 /* pData = (STUN_TRANSPORT *) */

   STUNC_EVT_SHARED_SECRET,      /* shared-secret */
                                 /* pData = (STUNC_SHARED_SECRET *) */

   STUNC_EVT_TX_TIMEOUT,         /* transmission timeout */
                                 /* pData = NULL */

   STUNC_EVT_RES_ERROR,          /* error response */
                                 /* pData = (STUNC_ERR *) */

   STUNC_EVT_SYSTEM_ERR,         /* System error */
                                 /* pData = NULL */

   STUNC_EVT_NUM
} STUNC_EVT;

/* Generic data type */
typedef struct
{
   BOS_UINT8   buf[STUNC_MAX_DATA_SIZE];
   BOS_UINT16  size;
} STUNC_DATA;

/* Shared Secret data */
typedef struct
{
   STUNC_DATA  username;
   STUNC_DATA  password;
} STUNC_SHARED_SECRET;

/* Error event data */
typedef struct
{
   BOS_UINT32  errnoa;
   STUNC_DATA  reason;
}STUNC_ERR;

/* Synchronous event data */
typedef struct
{
   STUNC_EVT   evt;
   union
   {
      STUN_TRANSPORT       socAddr;
      STUNC_SHARED_SECRET  sharedSecret;
      STUNC_ERR            err;
   } data;
} STUNC_EVT_DATA;

/*****************************************************************************
*
*  FUNCTION:   STUNC_EVTCB - STUN event Callback
*
*  PURPOSE:    A callback function for receiving STUN events.
*
*  PARAMETERS:
*              reqHdl   [IN] - Request handle of the original request
*              pEvtData [IN] - Event Data
*
*  RETURNS:    none.
*
*  NOTES:
*
*****************************************************************************/
typedef void (*STUNC_EVTCB)( const BOS_UINT32      reqHdl,
                             const STUNC_EVT_DATA  *pEvtData );

/* STUN Client Configuration */
typedef struct
{
   STUN_CFG    *pStunCfg;

} STUNC_CFG;

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
*  FUNCTION:   stuncStartup
*
*  PURPOSE:    Configure and startup STUN Client software module
*
*  PARAMETERS:
*          pStuncCfg - pointer to configuration parameters
*
*
*  RETURNS:    STUNC_SUCCESS if configuration successful. STUNC_FAILURE
*              otherwise.
*
*  NOTES:      It calls stunInit() to start the STUN module.
*
*****************************************************************************/
extern STUNC_STATUS stuncStartup( const STUNC_CFG *pStuncCfg );

/*****************************************************************************
*
*  FUNCTION:   stuncShutdown
*
*  PURPOSE:    Shutdown STUN Client software module and free resources.
*
*  PARAMETERS:
*              none.
*
*
*  RETURNS:    STUNC_SUCESSS if OK. STUNC_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
extern STUNC_STATUS stuncShutdown( void );


/*****************************************************************************
*
*  FUNCTION:   stuncSharedSecret
*
*  PURPOSE:    Send Shared Secret STUN request to a STUN server <pDest> to
*              obtain username and password for use in integrity check in
*              STUN Binding request.
*
*              If the STUN_OPT_SYNCD_RES option is used and <pEvtData> not
*              NULL, this function is blocked until a response is received
*              or timeout has occurred. The STUN event is
*              returned in the <pEvtData>.
*
*              Timeout occurs when parameters specified via pRetxData are 
*              reached.  If none are specified, the defaults from
*              RFC 3489/5389 is used.
*
*              A request handle is returned in <pReqHdl>.
*
*              The shared-secret is returned in STUNC_EVT_SHARED_SECRET.
*
*  PARAMETERS:
*           sid       [IN] - TLS socket id for sending shared-secret requests
*           pDest     [IN] - Destination IP port (network byte ordered)
*           options   [IN] - send options
*           pReqHdl   [OUT]- Request Handle
*           stuncEvtCB[IN] - Callback for receiving STUNC event
*           pRetxData [IN] - Retransmission configuration
*           pEvtData  [OUT]- Synchronous event data, set to NULL if not
*                            used. Needed when STUN_OPT_SYNCD_RES option
*                            is set.
*
*  RETURNS:    STUNC_SUCCESS if OK. STUNC_FAILURE otherwise.
*
*  NOTES:
*           For the current implementation, TLS is outside the scope of the
*           STUN client module. The module expects the client application
*           to pre-negotiate a TLS connection for exchanging Shared-Secret
*           Request/Responses.
*
*           For requests sent over TLS, retransmission should be disabled.
*           This can be set via the <options> parameter.
*
*****************************************************************************/
extern STUNC_STATUS stuncSharedSecret( const BOS_UINT32         sid,
                                       const STUN_TRANSPORT     *pDest,
                                       const STUN_OPTS          options,
                                       BOS_UINT32               *pReqHdl,
                                       const STUNC_EVTCB        stuncEvtCB,
                                       const STUN_RETRANS       *pRetxData
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                                       ,
                                       STUNC_EVT_DATA           *pEvtData
#endif
                                       );


/*****************************************************************************
*
*  FUNCTION:   stuncPublicAddr
*
*  PURPOSE:    Send STUN Binding request to a STUN server to discover its
*              public address.
*
*              If <pSecret> is provisioned, message integrity mechanism is
*              invoked.
*
*              If the STUN_OPT_SYNCD_RES option is used and <pEvtData> not
*              NULL, this function is blocked until a response is received
*              or timeout has occurred. The STUN event is
*              returned in the <pEvtData>.
*
*              Timeout occurs when parameters specified via pRetxData are 
*              reached.  If none are specified, the defaults from
*              RFC 3489/5389 is used.
*
*              A request handle is returned in <pReqHdl>.
*
*              The public address is returned in STUNC_EVT_MAPPED_ADDRESS.
*
*  PARAMETERS:
*           sid        [IN] - UDP socket id for sending Binding requests
*           pDest      [IN] - Destination IP port (network byte ordered)
*           pSecret    [IN] - Shared secret
*           options    [IN] - Send options
*           pReqHdl    [OUT]- Request Handle
*           stuncEvtCB [IN] - Callback for receiving STUNC event
*           pRetxData  [IN] - Retransmission configuration
*           pEvtData   [OUT]- Synchronous event data, set to NULL if not
*                             used. Needed when STUN_OPT_SYNCD_RES option
*                             is set.
*
*  RETURNS:    STUNC_SUCCESS if OK. STUNC_FAILURE otherwise.
*
*  NOTES:
*           For requests sent over TCP or TLS, retransmission should
*           be disabled.  This can be set via the <options> parameter.
*
*****************************************************************************/
extern STUNC_STATUS stuncPublicAddr( const BOS_UINT32           sid,
                                     const STUN_TRANSPORT       *pDest,
                                     const STUNC_SHARED_SECRET  *pSecret,
                                     const STUN_OPTS            options,
                                     BOS_UINT32                 *pReqHdl,
                                     const STUNC_EVTCB          stuncEvtCB,
                                     const STUN_RETRANS         *pRetxData
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                                     ,
                                     STUNC_EVT_DATA             *pEvtData
#endif
                                     );


/*****************************************************************************
*
*  FUNCTION:   stuncCancelRequest
*
*  PURPOSE:    Cancels the STUN Binding request outstanding.
*
*  PARAMETERS:
*              uReqHdl    [IN]- Request Handle
*
*  RETURNS:    STUNC_SUCCESS if OK. STUNC_FAILURE otherwise.
*
*  NOTES:
*           If the request was still outstanding (i.e. being
*           retransmitted), upon this action a timeout event for
*           the request will be issued and the application should be
*           handling it accordingly.
*
*****************************************************************************/
extern STUNC_STATUS stuncCancelRequest( const BOS_UINT32 uReqHdl );

#if defined(__cplusplus)
}
#endif
#endif
 

