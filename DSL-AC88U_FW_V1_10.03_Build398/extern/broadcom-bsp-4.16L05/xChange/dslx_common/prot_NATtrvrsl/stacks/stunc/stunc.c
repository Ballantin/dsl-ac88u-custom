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
*      STUN Client (STUNC) Module per RFC 3489. The implementation also
*      takes into account of the newer RFC 5389.
*
*      The draft-ietf-behave-rfc3489bis-04 defines four 'usages'.
*            1. Binding
*            2. Connectivity Check
*            3. Binding Keepalives
*            4. Short term credential.
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <bosTypes.h>
#include <bosIpAddr.h>
#include <bosMutex.h>
#if BOS_OS_ECOS || BOS_OS_VXWORKS
#include <bosTime.h>
#endif
#include <stunc.h>
#include <stun.h>

/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */
#define LOG( x ) if ( NULL != logCB ) {(*logCB) x;} else {printf x;}
/* ---- Private Variables ------------------------------------ */
static STUN_LOGCB logCB = NULL;
/* ---- Private Function Prototypes -------------------------- */
static BOS_BOOL insertMsgInt( BOS_UINT8 *pMsgBuf, const STUNC_SHARED_SECRET *pSecret, STUN_HMAC_PWD *pPwd );
static BOS_BOOL responseHandler( const BOS_UINT32          reqHdl,
                                 const STUN_RES_DATA       *pData);
/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   stuncStartup
*
*  PURPOSE:    Configure and startup STUNC software module
*
*  PARAMETERS:
*          pStuncCfg - pointer to configuration parameters
*
*
*  RETURNS:    STUNC_SUCCESS if successful or STUNC_FAILURE otherwise.
*
*  NOTES:
*
*****************************************************************************/
STUNC_STATUS stuncStartup( const STUNC_CFG *pStuncCfg )
{
   BOS_ASSERT( NULL != pStuncCfg );

   if ( BOS_TRUE != stunInit( pStuncCfg->pStunCfg ) )
   {
      return( STUNC_FAILURE );
   }

   BOS_ASSERT( NULL != pStuncCfg->pStunCfg );

   logCB = pStuncCfg->pStunCfg->logCB;

   LOG(( "STUNC configuration successful" ));

   return( STUNC_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   stuncShutdown
*
*  PURPOSE:    Shutdown STUNC module and free all resources.
*
*  PARAMETERS:
*              none
*
*
*  RETURNS:    STUNC_SUCCESS
*
*  NOTES:
*
*****************************************************************************/
STUNC_STATUS stuncShutdown( void )
{
   if ( BOS_TRUE != stunDeinit() )
   {
      LOG(( "stuncShutdown failed to deinit STUN module" ));
      return( STUNC_FAILURE );
   }

   LOG(( "stuncShutdown Successful" ));
 
   return( STUNC_SUCCESS );
}

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
STUNC_STATUS stuncSharedSecret( const BOS_UINT32            sid,
                                const STUN_TRANSPORT        *pDest,
                                const STUN_OPTS             options,
                                BOS_UINT32                  *pReqHdl,
                                const STUNC_EVTCB           stuncEvtCB,
                                const STUN_RETRANS          *pRetxData
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                                ,
                                STUNC_EVT_DATA              *pEvtData
#endif
                                )
{
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   BOS_UINT16 len = STUN_MSG_BUFFER_SIZE;
   STUN_OPTS opts = 0;
   void *pOpaque = NULL;

   BOS_ASSERT( (NULL != pDest) &&
               (NULL != pReqHdl) );

   memset( msgBuf, 0, sizeof( msgBuf ) );

   stunSetMsgType( msgBuf, STUN_MSG_SHARED_SECRET_REQUEST );

   stunSetTransactionId( msgBuf, NULL );

   /* encode message to a network packet */
   if ( BOS_TRUE != stunEncode( msgBuf, &len, NULL, options ) )
   {
      return( STUNC_FAILURE );
   }

   pOpaque = (void *) *(BOS_UINT32 *)&(stuncEvtCB);

   opts = options;
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   if ( NULL != pEvtData )
   {
      opts |= STUN_OPT_SYNCD_RES;
      pOpaque = pEvtData;
   }
#endif
   /* queue the packet for sending into network */
   if ( BOS_TRUE != stunSendRequest( sid,
                                     pDest,
                                     msgBuf,
                                     len,
                                     pReqHdl,
                                     responseHandler,
                                     pRetxData,
                                     NULL,
                                     opts,
                                     (void *)pOpaque ) )
   {
      return ( STUNC_FAILURE );
   }

   return( STUNC_SUCCESS );
}

/*****************************************************************************
*
*  FUNCTION:   stuncPublicAddr
*
*  PURPOSE:    Send STUN Binding request to a STUN server <pDest> to
*              discover its public address.
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
STUNC_STATUS stuncPublicAddr( const BOS_UINT32           sid,
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
                              )
 
{
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   STUN_HMAC_PWD password;
   STUN_HMAC_PWD *pPwd = &password;
   BOS_UINT16 len = STUN_MSG_BUFFER_SIZE;
   STUN_OPTS opts = 0;
   void *pOpaque = NULL;

   BOS_ASSERT( (NULL != pDest) &&
               (NULL != pReqHdl ) );

   memset( msgBuf, 0, sizeof( msgBuf ) );

   stunSetMsgType( msgBuf, STUN_MSG_BINDING_REQUEST );

   /* Message Integrity needed */
   if ( NULL != pSecret )
   {
      if ( !insertMsgInt( msgBuf, pSecret, &password ) )
      {
         return( STUNC_FAILURE );
      }
   }
   else
   {
      pPwd = NULL;
   }

   opts = options;

   stunSetTransactionId( msgBuf, NULL );

   /* encode message to a network packet */
   if ( BOS_TRUE != stunEncode( msgBuf, &len, pPwd, options ) )
   {
      return( STUNC_FAILURE );
   }

   pOpaque = (void *)*(BOS_UINT32 *)&stuncEvtCB;

#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   if ( NULL != pEvtData )
   {
      opts |= STUN_OPT_SYNCD_RES;
      pOpaque = pEvtData;
   }
 
#endif

   /* queue the packet for sending into network */
   if ( BOS_TRUE != stunSendRequest( sid,
                                     pDest,
                                     msgBuf,
                                     len,
                                     pReqHdl,
                                     responseHandler,
                                     pRetxData,
                                     pPwd,
                                     opts,
                                     (void *)pOpaque) )
   {
      return( STUNC_FAILURE );
   }

   return( STUNC_SUCCESS );
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
static BOS_BOOL insertMsgInt( BOS_UINT8 *pMsgBuf, const STUNC_SHARED_SECRET *pSecret, STUN_HMAC_PWD *pPwd )
{
   BOS_ASSERT( NULL != pMsgBuf && NULL != pPwd );

   pPwd->len = 0;

   /* Message Integrity needed */
   if ( (pSecret->username.size > STUN_MAX_DATA_LEN) ||
        (pSecret->password.size > STUN_MAX_DATA_LEN) )
   {
      return(BOS_FALSE);
   }

   if ( pSecret->username.size > 0 && pSecret->password.size > 0 )
   {
      STUN_DATA stunData;

      stunData.pData = (BOS_UINT8 *)pSecret->username.buf;
      stunData.len = pSecret->username.size;
      stunSetAttribute( pMsgBuf, STUN_ATTR_USERNAME, &stunData );

      /* use password for integrity check */
      memcpy( pPwd->val,
              pSecret->password.buf,
              pSecret->password.size);
      pPwd->len = pSecret->password.size ;
      LOG(( "insertMsgInt: username %.*s password %.*s",
             pSecret->username.size, pSecret->username.buf,
             pPwd->len, pPwd->val));
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*
*  FUNCTION:   responseHandler
*
*  PURPOSE:    STUNC response handler
*
*  PARAMETERS:
*          reqHdl    [IN] - The request handle
*          pResData  [IN] - Response data
*
*  RETURNS:    BOS_BOOL if successful or BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL responseHandler( const BOS_UINT32          reqHdl,
                                 const STUN_RES_DATA       *pResData)
{
   BOS_UINT16 msgType;
   STUNC_EVTCB evtCB;
   STUNC_EVT_DATA  evtData;

   BOS_ASSERT( NULL != pResData );

   memset( &evtData, 0, sizeof(STUNC_EVT_DATA) );

   evtCB = (STUNC_EVTCB)*(BOS_UINT32 *)&(pResData->pOpaq);

   switch( pResData->evt )
   {
      case STUN_TIMEOUT:
      {
         evtData.evt = STUNC_EVT_TX_TIMEOUT;
      }
      break;

      case STUN_SYSTEM_ERR:
      {
         evtData.evt = STUNC_EVT_SYSTEM_ERR;
      }
      break;

      case STUN_RESPONSE:
      {
         msgType = stunGetMsgType( pResData->pMsg );
         switch( msgType )
         {
            case STUN_MSG_BINDING_RESPONSE:
            {
               evtData.evt = STUNC_EVT_MAPPED_ADDRESS;

               /* use XOR'd address if present (bis draft) */
               if ( stunGetAttribute( pResData->pMsg, STUN_ATTR_XORMAPPED_ADDRESS, (void *)&evtData.data.socAddr ) )
               {
                  LOG(("responseHandler: has XOR-Mapped-Address"));
               }
               else if ( stunGetAttribute( pResData->pMsg, STUN_ATTR_MAPPED_ADDRESS, (void *)&evtData.data.socAddr ) )
               {
                  LOG(("responseHandler: has Mapped-Address"));
               }
               else
               {
                  /* server mis-behaving,
                   * Mapped address is not present
                   * By returning here, it will force
                   * retransmission of the request
                   */
                  evtData.evt = STUNC_EVT_SYSTEM_ERR;
                  break;
               }
            }
            break;

            case STUN_MSG_SHARED_SECRET_RESPONSE:
            {
               STUN_DATA   username;
               STUN_DATA   password;
               username.pData = evtData.data.sharedSecret.username.buf;
               username.len = STUNC_MAX_DATA_SIZE;
               password.pData = evtData.data.sharedSecret.password.buf;
               password.len = STUNC_MAX_DATA_SIZE;
               LOG(("responseHandler: Shared Secret Response" ));
               if ( stunGetAttribute( pResData->pMsg, STUN_ATTR_USERNAME, &username )
                  && stunGetAttribute( pResData->pMsg, STUN_ATTR_PASSWORD, &password ))
               {
                  evtData.evt = STUNC_EVT_SHARED_SECRET;
                  evtData.data.sharedSecret.username.size = username.len;
                  evtData.data.sharedSecret.password.size = password.len;
               }
            }
            break;

            case STUN_MSG_BINDING_ERROR_RESPONSE:
            case STUN_MSG_SHARED_SECRET_ERROR_RESPONSE:
            {
               STUN_ERR stunErr;
               STUN_DATA stunData;
               stunErr.pPhrase = &stunData;
               stunData.pData = evtData.data.err.reason.buf;
               stunData.len = STUNC_MAX_DATA_SIZE;
               if ( stunGetAttribute( pResData->pMsg, STUN_ATTR_ERRORCODE, (void *)&stunErr ) )
               {
                  evtData.evt = STUNC_EVT_RES_ERROR;
                  evtData.data.err.errnoa = stunErr.errnoa;
                  evtData.data.err.reason.size = stunErr.pPhrase->len;
               }
            }
            break;

            default:
            {
               return( BOS_FALSE );
            }
         }/* switch msgType */
      }
      break;

      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }/* switch response type */

#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   if ( !(pResData->options & STUN_OPT_SYNCD_RES) )
#endif
   {
      BOS_ASSERT( NULL != evtCB );
      (*evtCB)( reqHdl, &evtData );
   }


#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   if ( pResData->options & STUN_OPT_SYNCD_RES )
   {
      memcpy( pResData->pOpaq, &evtData, sizeof( STUNC_EVT_DATA ) );
   }
#endif

   return ( BOS_TRUE );
}


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
STUNC_STATUS stuncCancelRequest( const BOS_UINT32 uReqHdl )
{
   if ( !stunCancelRequest( uReqHdl ) )
   {
      return( STUNC_FAILURE );
   }

   return( STUNC_SUCCESS );
}
