/****************************************************************************
*
* Copyright (c) 2006-2011 Broadcom Corporation
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
*      TURN Client (TURNC) Module per draft-ietf-behave-turn-02.txt. 
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <bosTypes.h>
#include <ntcfg.h>
#include <stun.h>
#include "turn.h"
#include "turntx.h"
#include "turnrx.h"
#include "turnci.h"
#include "turndbg.h"

/* ---- Public Variables ------------------------------------- */
TURNC_SENDCB turncSendCB = NULL;      
TURNC_RECVCB turncRecvCB = NULL;      
TURNC_LOGCB turncLogCB = NULL;      
TURNC_EVTCB turncEvtCB = NULL;      

/* ---- Private Constants and Types -------------------------- */

#define MAX_SEND_DATA_BUF_SIZE   1400

/* ---- Private Variables ------------------------------------ */
static BOS_BOOL bTurncConfigured = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------- */
static BOS_BOOL insertMsgInt( BOS_UINT8 *pMsgBuf, const TURNC_SHARED_SECRET *pSecret, STUN_HMAC_PWD *pPwd );
/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   turncStartup
*
*  PURPOSE:    configure and startup TURN Client software module
*
*  PARAMETERS:
*          pTurncCfg - pointer to configuration parameters
*
*
*  RETURNS:    TURNC_SUCCESS if successful or TURNC_FAILURE otherwise.
*
*  NOTES:
*        stun Client must be started before turn client.
*
*****************************************************************************/
TURNC_STATUS turncStartup( const TURNC_CFG *pTurncCfg )
{
   BOS_BOOL rc;
   BOS_ASSERT( ( NULL != pTurncCfg ) && 
               ( NULL != pTurncCfg->evtCB ) &&
               ( NULL != pTurncCfg->sendCB ) &&
               ( NULL != pTurncCfg->recvCB ) &&
               ( NULL != pTurncCfg->logCB ) );

   if ( bTurncConfigured )
   {
      LOG(( "turncStartup: TURNC already configured" ));
      return( TURNC_SUCCESS );
   }

   if ( BOS_TRUE != turntx_Init() )
   {
      LOG(( "TURNC failed to initialize Request refresh module" ));
      return( TURNC_FAILURE );
   }

   turncEvtCB = pTurncCfg->evtCB;
   turncSendCB = pTurncCfg->sendCB;
   turncRecvCB = pTurncCfg->recvCB;
   turncLogCB = pTurncCfg->logCB;

   rc = stunSetIndicationCallback( STUN_MSG_DATA_INDICATION, turnrx_IndicationHandler );
   BOS_ASSERT( BOS_TRUE == rc );
   rc = stunSetIndicationCallback( STUN_MSG_CONNECT_STATUS_INDICATION, turnrx_IndicationHandler );
   BOS_ASSERT( BOS_TRUE == rc );

   bTurncConfigured = BOS_TRUE;

   LOG(( "turncStartup: TURNC configuration successful" ));

   return( TURNC_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   turncShutdown
*
*  PURPOSE:    shutdown TURNC Client module and free all resources.
*
*  PARAMETERS:
*              none
*
*
*  RETURNS:    TURNC_SUCCESS
*
*  NOTES:
*
*****************************************************************************/
TURNC_STATUS turncShutdown( void )
{
   if ( !bTurncConfigured )
   {
      LOG(( "turncShutdown not configured" ));
      return( TURNC_FAILURE );
   }
   
   if (BOS_TRUE != turntx_Deinit() )
   {
      LOG(( "turncShutdown failed to deinit refresh module" ));
      return (TURNC_FAILURE);
   }

   bTurncConfigured = BOS_FALSE;

   LOG(( "turncShutdown Successful" ));
   
   return( TURNC_SUCCESS );
}

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
                            BOS_UINT32                  *pReqHdl )
{
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   BOS_UINT8 refreshMsgBuf[STUN_MSG_BUFFER_SIZE];
   STUN_HMAC_PWD password;
   BOS_UINT16 len;
   STUN_RETRANS reTx;

   BOS_ASSERT( (BOS_TRUE == bTurncConfigured) &&
               (NULL != pDest) &&
               (NULL != pReqHdl) );

   memset( msgBuf, 0, sizeof( msgBuf ) );

   stunSetMsgType( msgBuf, STUN_MSG_ALLOCATE_REQUEST );

   /* Message Integrity needed */
   if ( pSecret )
   {
      if ( !insertMsgInt( msgBuf, pSecret, &password ) )
      {
         return( TURNC_FAILURE );
      }
   }

   /* Set Allocate Request options */
   if ( NULL != pOptions )
   {
      if( NULL != pOptions->pLifetime )
      {
         stunSetAttribute( msgBuf, STUN_ATTR_LIFETIME, (void *)(pOptions->pLifetime) );
      }
      if ( NULL != pOptions->pBandwidth )
      {
         stunSetAttribute( msgBuf, STUN_ATTR_BANDWIDTH, (void *)(pOptions->pBandwidth) );
      }
      if ( NULL != pOptions->pReqdTransport )
      {
         stunSetAttribute( msgBuf, STUN_ATTR_REQUESTED_TRANSPORT, (void *)(pOptions->pReqdTransport));
      }
      if ( NULL != pOptions->pPortProps )
      {
         stunSetAttribute( msgBuf, STUN_ATTR_REQUESTED_PORT_PROPS, (void *)(pOptions->pPortProps));
      }
      if ( NULL != pOptions->pReqdIPAddr )
      {
         stunSetAttribute( msgBuf, STUN_ATTR_REQUESTED_IP, (void *)(pOptions->pReqdIPAddr));
      }
   }

   stunSetTransactionId( msgBuf, NULL );
   
   /* Make a copy of the msg before being encoded.  */
   memcpy( refreshMsgBuf, msgBuf, STUN_MSG_BUFFER_SIZE ); 

   if ( BOS_TRUE != stunEncode( msgBuf, &len, &password, 0 ) )
   {
      return( TURNC_FAILURE );
   }

   memset ( &reTx,
            0,
            sizeof ( STUN_RETRANS ) );

   /* queue the packet for sending into network */
   if ( BOS_TRUE != stunSendRequest( sid, 
                                     pDest, 
                                     msgBuf, 
                                     len, 
                                     pReqHdl, 
                                     turnrx_ResponseHandler,
                                     &reTx,
                                     &password, 
                                     0,
                                     (void *)&turncEvtCB) )
   {
      return ( TURNC_FAILURE );
   }

   /* place request on the refresh queue, pending on refreshing */
   turntx_Pending( sid, pDest, *pReqHdl, refreshMsgBuf, &password );

   return( TURNC_SUCCESS );
}

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
*           When terminating an allocation, and where the socket is
*           TCP, it's the responsibility of the application to close the 
*           TCP connection between the client and the STUN server,
*           as required by the standard.
*
*****************************************************************************/
TURNC_STATUS turncDeallocate( BOS_UINT32 reqHdl )
{
   BOS_ASSERT( BOS_TRUE == bTurncConfigured );
   turntx_Terminate( reqHdl );
   return( TURNC_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   turncActiveDestination
*
*  PURPOSE:    Send Set Active Destination Request to a STUN Relay server.
*              When pRemoteAddr is NULL, this unsets the active destination
*              with the server.
*
*  PARAMETERS:
*           sid     [IN] - socket id for sending the request
*           pDest   [IN] - Destination IP port where the request is sent to
*           pSecret [IN] - shared secret
*           pRemoteAddr [IN] - the remote active destination
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
                                     BOS_UINT32                *pReqHdl )
{
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   STUN_HMAC_PWD password;
   BOS_UINT16 len;
   BOS_BOOL rc;
   STUN_RETRANS reTx;

   BOS_ASSERT( (BOS_TRUE == bTurncConfigured ) &&
               (NULL != pDest) &&
               (NULL != pReqHdl) );

   memset( msgBuf, 0, sizeof( msgBuf ) );

   stunSetMsgType( msgBuf, STUN_MSG_ACTIVE_DEST_REQUEST );

   /* Message Integrity needed */
   if ( !insertMsgInt( msgBuf, pSecret, &password ) )
   {
      return( TURNC_FAILURE );
   }

   /* Set Request options */
   if ( NULL != pRemoteAddr ) 
   {
      stunSetAttribute( msgBuf, STUN_ATTR_REMOTE_ADDRESS, (void *)pRemoteAddr);
      rc = turntxAddActiveDest( sid, pDest, pRemoteAddr, *pReqHdl );
      BOS_ASSERT( rc == BOS_TRUE );
   }
   else
   {
      /* unset the active destination associated with the socket */
      turntxRemoveActiveDest( sid, pDest );
   }

   stunSetTransactionId( msgBuf, NULL );

   if ( BOS_TRUE != stunEncode( msgBuf, &len, &password, 0 ) )
   {
      return( TURNC_FAILURE );
   }

   memset ( &reTx,
            0,
            sizeof ( STUN_RETRANS ) );

   /* queue the packet for sending into network */
   if ( BOS_TRUE != stunSendRequest( sid, 
                                     pDest, 
                                     msgBuf, 
                                     len, 
                                     pReqHdl, 
                                     turnrx_ResponseHandler,
                                     &reTx,
                                     &password, 
                                     0, 
                                     (void *)&turncEvtCB ) )
   {
      return ( TURNC_FAILURE );
   }

   return( TURNC_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   turncConnect
*
*  PURPOSE:    Send Connect Request to a STUN Relay server.
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
TURNC_STATUS turncConnect( const BOS_UINT32          sid,
                           const STUN_TRANSPORT      *pDest, 
                           const TURNC_SHARED_SECRET *pSecret,
                           const STUN_TRANSPORT      *pRemoteAddr,
                           BOS_UINT32                *pReqHdl )
{
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   STUN_HMAC_PWD password;
   BOS_UINT16 len;
   STUN_RETRANS reTx;

   BOS_ASSERT( (BOS_TRUE == bTurncConfigured ) &&
               (NULL != pDest) &&
               (NULL != pRemoteAddr) &&
               (NULL != pReqHdl) );

   memset( msgBuf, 0, sizeof( msgBuf ) );

   stunSetMsgType( msgBuf, STUN_MSG_CONNECT_REQUEST );

   /* Message Integrity needed */
   if ( !insertMsgInt( msgBuf, pSecret, &password ) )
   {
      return( TURNC_FAILURE );
   }

   /* Set Request options */
   stunSetAttribute( msgBuf, STUN_ATTR_REMOTE_ADDRESS, (void *)pRemoteAddr );

   stunSetTransactionId( msgBuf, NULL );

   if ( BOS_TRUE != stunEncode( msgBuf,&len, &password, 0) )
   {
      return( TURNC_FAILURE );
   }

   memset ( &reTx,
            0,
            sizeof ( STUN_RETRANS ) );

   /* queue the packet for sending into network */
   if ( BOS_TRUE != stunSendRequest( sid, 
                                     pDest, 
                                     msgBuf, 
                                     len, 
                                     pReqHdl, 
                                     turnrx_ResponseHandler,
                                     &reTx,
                                     &password, 
                                     0,
                                     (void *)&turncEvtCB ) )
   {
      return ( TURNC_FAILURE );
   }

   return( TURNC_SUCCESS );
}

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
                            const BOS_UINT16        len )
{
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   BOS_UINT16 bufSize;

   BOS_ASSERT( (BOS_TRUE == bTurncConfigured ) &&
               (NULL != pDest) &&
               (NULL != pRemoteAddr) );
   
   if ( len > 0 )
   {
      BOS_ASSERT( NULL != pData );
   }

   if ( turntxIsActiveDest( sid, pDest, pRemoteAddr ) )
   {
      /* already an active destination, just sent data without
       * encapsulation */
      stunSendIndication( sid, pDest, pData, len );
   }
   else
   {
      STUN_DATA stunData;
      /* 
       * data encapsulation needed 
       */
      memset( msgBuf, 0, sizeof( msgBuf ) );

      stunSetMsgType( msgBuf, STUN_MSG_SEND_INDICATION );
      stunSetAttribute( msgBuf, STUN_ATTR_REMOTE_ADDRESS, (void *)pRemoteAddr );
      stunData.pData = (BOS_UINT8 *)pData;
      stunData.len = len;
      stunSetAttribute( msgBuf, STUN_ATTR_DATA, (void *)&stunData );

      stunSetTransactionId( msgBuf, NULL );
      if ( BOS_TRUE != stunEncode( msgBuf, &bufSize, NULL, 0 ) )
      {
         return( TURNC_FAILURE );
      }
      stunSendIndication( sid, pDest, msgBuf, bufSize );
   }

   return( TURNC_SUCCESS );
}

/*****************************************************************************
*  FUNCTION:
*     insertMsgInt
*
*  PURPOSE:
*     Insert Messsage Integration attribue into message if secret is not 
*     NULL.
*
*  PARAMETERS:
*        pMsgBuf  [MOD] - STUN message buffer
*        pSecret  [IN]  - shared secret for message integrity
*        pwd      [MOD] - buffer to contain the converted password info.
*
*  RETURNS:
*     BOS_TRUE - insertion OK. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL insertMsgInt( BOS_UINT8 *pMsgBuf, const TURNC_SHARED_SECRET *pSecret, STUN_HMAC_PWD *pPwd )
{
   BOS_ASSERT( NULL != pMsgBuf && NULL != pPwd );

   pPwd->len = 0;

   /* Message Integrity needed */
   if ( (pSecret->username.len > STUN_MAX_DATA_LEN) || 
        (pSecret->password.len > STUN_MAX_DATA_LEN) )
   {
      return(BOS_FALSE);
   }

   if ( pSecret->username.len > 0 && pSecret->password.len > 0 )
   {
      STUN_DATA stunData;

      stunData.pData = (BOS_UINT8 *)pSecret->username.buf;
      stunData.len = pSecret->username.len;
      stunSetAttribute( pMsgBuf, STUN_ATTR_USERNAME, &stunData );

      /* use password for integrity check */
      memcpy( pPwd->val, 
              pSecret->password.buf, 
              pSecret->password.len); 
      pPwd->len = pSecret->password.len ;
      DLOG(( "insertMsgInt: username %.*s password %.*s", 
             pMsg->username.iLen, pMsg->username.val,
             pPwd->len, pPwd->val));
   }

   return( BOS_TRUE );
}

