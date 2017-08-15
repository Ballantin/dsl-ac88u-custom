/****************************************************************************
*
* Copyright (c) 2002-2012 Broadcom Corporation
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
*      STUN Base module, main features:
*        - Encoding/Parsing of STUN messages
*        - Transaction retransmission
*        - Transaction authentication
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <bosTypes.h>
#include <ntcfg.h>
#include "./inc/stun.h"
#include "stunmsg.h"
#include "stuni.h"
#include "stuntx.h"
#include "stunutil.h"
#include "stundbg.h"
#if STUNDBG_LOG
#include <bosIpAddr.h>
#endif

#define STUN_SELFTEST 0

/* ---- Public Variables ------------------------------------- */
STUN_SENDPKTCB stunSendPktCB = NULL;
STUN_RAND32CB stunRand32CB = NULL;
STUN_HMACCB stunHmacCB = NULL;
STUN_CRC32CB stunCrc32CB = NULL;
STUN_LOGCB stunLogCB = NULL;
STUN_REQCB  stunBindingReqCB = NULL;
STUN_INDCB  stunDataIndCB = NULL;
STUN_INDCB  stunConnectStatusIndCB = NULL;

/* ---- Private Constants and Types -------------------------- */
#define MAGIC_COOKIE_OFFSET      4
/* ---- Private Variables ------------------------------------ */
static BOS_BOOL bStunInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------- */
static BOS_UINT32 newReqHdl( void );
static void selftest();
/* ---- Functions -------------------------------------------- */
/*****************************************************************************
*  FUNCTION:   stunInit
*
*  PURPOSE:    Initialize the base STUN module.
*
*  PARAMETERS:
*              pStunCfg [IN]- configuration parameters
*
*  RETURNS:    BOS_TRUE if initialization OK, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunInit( const STUN_CFG *pStunCfg )
{
   if ( bStunInitialized )
   {
      LOG(("stunInit: error. module already started"));
      return( BOS_FALSE );
   }

   BOS_ASSERT( NULL != pStunCfg );
   BOS_ASSERT( NULL != pStunCfg->sendPktCB );
   BOS_ASSERT( NULL != pStunCfg->hmacCB );
   
   stunSendPktCB = pStunCfg->sendPktCB;
   stunRand32CB = pStunCfg->rand32CB;
   stunHmacCB = pStunCfg->hmacCB;
   stunCrc32CB = pStunCfg->crc32CB;
   stunLogCB = pStunCfg->logCB;

   if ( BOS_TRUE != stuntx_Init() )
   {
      return( BOS_FALSE );
   }

   bStunInitialized = BOS_TRUE;
#if STUN_SELFTEST
   selftest();
#endif 
   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunDeinit
*
*  PURPOSE:    Deinit the base STUN module.
*
*  PARAMETERS:
*              none
*
*  RETURNS:    BOS_TRUE if de-initialization OK, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunDeinit( void )
{
   if ( !bStunInitialized )
   {
      return( BOS_FALSE );
   }

   if ( BOS_TRUE != stuntx_Deinit() )
   {
      return( BOS_FALSE );
   }

   bStunInitialized = BOS_FALSE;

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunSetMsgType
*
*  PURPOSE:    Set STUN message type
*
*  PARAMETERS:
*              pBuf     [IN] - STUN message buffer
*              msgType  [IN] - STUN message type
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void stunSetMsgType( BOS_UINT8 *pBuf, const BOS_UINT16 msgType )
{
   stunmsgSetType( (STUN_MSG *)pBuf, msgType );
}

/*****************************************************************************
*  FUNCTION:   stunGetMsgType
*
*  PURPOSE:    Get STUN message type
*
*  PARAMETERS:
*              pBuf     [IN] - STUN message buffer
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
BOS_UINT16 stunGetMsgType( const BOS_UINT8 *pBuf )
{
   return( stunmsgGetType( (STUN_MSG *)pBuf ) );
}

/*****************************************************************************
*  FUNCTION:   stunSetTransactionId
*
*  PURPOSE:    Set transaction id
*
*  PARAMETERS:
*              pBuf     [IN] - STUN message buffer
*              pTxid    [IN] - Transaction Id
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void stunSetTransactionId( BOS_UINT8 *pBuf, const STUN_TRANSACTION_ID *pTxid )
{
   STUN_MSG *pMsg = (STUN_MSG *)pBuf;
   BOS_ASSERT( NULL != pBuf );
   if ( NULL != pTxid )
   {
      memcpy( &(pMsg->msgHdr.id), pTxid, sizeof( STUN_TRANSACTION_ID ) );
   }
   else
   {
      /* Insert transaction ID */
      stuntx_GenerateId( &(pMsg->msgHdr.id) );
   }
}

/*****************************************************************************
*  FUNCTION:   stunGetTransactionId
*
*  PURPOSE:    Get transaction id
*
*  PARAMETERS:
*              pBuf     [IN] - STUN message buffer
*              pTxid    [IN] - Transaction Id
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
STUN_TRANSACTION_ID *stunGetTransactionId( const BOS_UINT8 *pBuf )
{
   STUN_MSG *pMsg = (STUN_MSG *)pBuf;
   BOS_ASSERT( NULL != pBuf );
   return( &(pMsg->msgHdr.id));
}

/*****************************************************************************
*  FUNCTION:   stunSetAttribute
*
*  PURPOSE:    Set STUN Attribute and data
*
*  PARAMETERS:
*              pBuf     [IN] - STUN message buffer
*              attr     [IN] - attribute type
*              pData    [IN] - attribute Data
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
void stunSetAttribute( BOS_UINT8 *pBuf, const BOS_UINT16 attr, const void *pData )
{
   switch( attr )
   {
      case STUN_ATTR_MAPPED_ADDRESS:
      case STUN_ATTR_RESPONSE_ADDRESS:
      case STUN_ATTR_SOURCE_ADDRESS:
      case STUN_ATTR_CHANGED_ADDRESS:
      case STUN_ATTR_REFLECTED_FROM:
#if NTCFG_TURN_SUPPORT
      case STUN_ATTR_REMOTE_ADDRESS:
      case STUN_ATTR_RELAY_ADDRESS:
      case STUN_ATTR_REQUESTED_IP:
#endif
      {
         STUN_ATTR_ADDR stunAddr;
         stunutilBosToStunTransport( (STUN_TRANSPORT *)pData, &stunAddr );
         stunmsgSetAttr( (STUN_MSG *)pBuf, attr, (void *)&stunAddr );
      }
      break;

      case STUN_ATTR_XORMAPPED_ADDRESS:
      {
         STUN_ATTR_ADDR stunAddr;
         STUN_MSG_HDR *pMsgHdr = (STUN_MSG_HDR *)pBuf;
 
         BOS_UINT16 id16 = pMsgHdr->id.octet[0]<<8
                        | pMsgHdr->id.octet[1];
         BOS_UINT32 id32 = pMsgHdr->id.octet[0]<<24
                        | pMsgHdr->id.octet[1]<<16
                        | pMsgHdr->id.octet[2]<<8
                        | pMsgHdr->id.octet[3];
 
         stunutilBosToStunTransport( (STUN_TRANSPORT *)pData, &stunAddr );
         if ( STUN_IPV4_FAMILY == stunAddr.family )
         {
            stunAddr.ipAddr.v4.port ^= id16;
            stunAddr.ipAddr.v4.addr ^= id32;
            stunmsgSetAttr( (STUN_MSG *)pBuf, attr, (void *)&stunAddr );
         }
         else if ( STUN_IPV6_FAMILY == stunAddr.family )
         {
            int i;
            stunAddr.ipAddr.v6.port ^= id16;
            for ( i=0;i<STUN_IP_ADDR_LEN_BYTES_V6;i++ )
            {
                stunAddr.ipAddr.v6.d8[i] ^=  pMsgHdr->id.octet[i];
            }
            stunmsgSetAttr( (STUN_MSG *)pBuf, attr, (void *)&stunAddr );
         }
         else
         {
            BOS_ASSERT( BOS_FALSE );
         }
      }
      break;
      case STUN_ATTR_CHANGE_REQUEST:
      case STUN_ATTR_REFRESH_INTERVAL:
      case STUN_ATTR_FINGERPRINT:
#if NTCFG_TURN_SUPPORT
      case STUN_ATTR_LIFETIME:
      case STUN_ATTR_BANDWIDTH:
      case STUN_ATTR_REQUESTED_PORT_PROPS:
      case STUN_ATTR_REQUESTED_TRANSPORT:
#endif
#if NTCFG_ICE_SUPPORT
      case STUN_ATTR_PRIORITY:
#endif
      {
         stunmsgSetAttr( (STUN_MSG *)pBuf, attr, (void *)pData );
      }
      break;

      case STUN_ATTR_USERNAME:
      case STUN_ATTR_PASSWORD:
      case STUN_ATTR_MESSAGE_INTEGRITY:
      case STUN_ATTR_REALM:
      case STUN_ATTR_NONCE:
      {
         STUN_ATTR_OPAQUE  opqData;
         STUN_DATA *pStunData = (STUN_DATA *)pData;
         BOS_ASSERT( pStunData->len <= STUN_MAX_DATA_LEN );

         memcpy( opqData.val, pStunData->pData, pStunData->len );
         opqData.iLen = pStunData->len;
         stunmsgSetAttr( (STUN_MSG *)pBuf, attr, (void *)&opqData );
      }
      break;

      case STUN_ATTR_ERRORCODE:
      {
         STUN_ATTR_ERRCODE errCode;
         BOS_UINT32 errnoa = ((STUN_ERR *)pData)->errnoa;
         memset( &errCode, 0, sizeof(STUN_ATTR_ERRCODE) );

         errCode.class = (BOS_UINT8)(errnoa / 100);
         errCode.number = (BOS_UINT8)(errnoa % 100);
         if ( (NULL == ((STUN_ERR *)pData)->pPhrase ) ||
              (NULL == ((STUN_ERR *)pData)->pPhrase->pData ))
         {
            strncpy( (char *)errCode.phrase,
                     stunmsgReasonStr( (BOS_UINT16)errnoa),
                     STUN_MAX_DATA_LEN-1);
         }
         else
         {
            strncpy( (char *)errCode.phrase,
                     (char *)((STUN_ERR *)pData)->pPhrase->pData,
                     STUN_MAX_DATA_LEN-1);
         }
         errCode.iLen = (BOS_UINT8)strlen( (char *)errCode.phrase );
         stunmsgSetAttr( (STUN_MSG *)pBuf, attr, (void *)&errCode );
      }
      break;

      case STUN_ATTR_XORONLY:
#if NTCFG_ICE_SUPPORT
      case STUN_ATTR_USECANDIDATE:
#endif
      {
         stunmsgSetAttr( (STUN_MSG *)pBuf, attr, NULL );
      }
      break;

#if NTCFG_TURN_SUPPORT
      case STUN_ATTR_DATA:
      {
         STUN_ATTR_GENDATA genData;
         genData.pBuf = ((STUN_DATA *)pData)->pData;
         genData.len = ((STUN_DATA *)pData)->len;
         stunmsgSetAttr( (STUN_MSG *)pBuf, attr, (void *)&genData );
      }
      break;
#endif
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }
}

/*****************************************************************************
*  FUNCTION:   stunGetAttribute
*
*  PURPOSE:    Get Attribute data. Return BOS_FALSE if attribute
*              is not present.
*
*  PARAMETERS:
*              pBuf     [IN] - STUN message buffer
*              msgType  [IN] - STUN message type
*              pData    [MOD] - attribute data
*
*  RETURNS:
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunGetAttribute( const BOS_UINT8 *pBuf, const BOS_UINT16 attr, void *pData )
{
   void *pAttrData;

   if ( NULL == ( pAttrData = stunmsgGetAttr( (STUN_MSG *)pBuf, attr ) ) )
   {
      return( BOS_FALSE );
   }

   switch( attr )
   {
      case STUN_ATTR_MAPPED_ADDRESS:
      case STUN_ATTR_RESPONSE_ADDRESS:
      case STUN_ATTR_SOURCE_ADDRESS:
      case STUN_ATTR_CHANGED_ADDRESS:
      case STUN_ATTR_REFLECTED_FROM:
#if NTCFG_TURN_SUPPORT
      case STUN_ATTR_REMOTE_ADDRESS:
      case STUN_ATTR_RELAY_ADDRESS:
      case STUN_ATTR_REQUESTED_IP:
#endif
      {
         STUN_ATTR_ADDR *pStunAddr = (STUN_ATTR_ADDR *)(pAttrData);
         STUN_TRANSPORT *pSocAddr = (STUN_TRANSPORT *)pData;
         stunutilStunToBosTransport( pStunAddr, pSocAddr );
      }
      break;

      case STUN_ATTR_XORMAPPED_ADDRESS:
      {
         STUN_MSG_HDR *pMsgHdr = (STUN_MSG_HDR *)pBuf;
         STUN_TRANSPORT *pSocAddr = (STUN_TRANSPORT *)pData;
         STUN_ATTR_ADDR *pStunAddr = (STUN_ATTR_ADDR *)(pAttrData);
 
         BOS_UINT16 id16 = pMsgHdr->id.octet[0]<<8
                        | pMsgHdr->id.octet[1];
         BOS_UINT32 id32 = pMsgHdr->id.octet[0]<<24
                        | pMsgHdr->id.octet[1]<<16
                        | pMsgHdr->id.octet[2]<<8
                        | pMsgHdr->id.octet[3];
 
         if ( STUN_IPV4_FAMILY == pStunAddr->family )
         {
            pStunAddr->ipAddr.v4.port ^= id16;
            pStunAddr->ipAddr.v4.addr ^= id32;
         }
         else if ( STUN_IPV6_FAMILY == pStunAddr->family )
         {
            int i;
            pStunAddr->ipAddr.v6.port ^= id16;
            for ( i=0;i<STUN_IP_ADDR_LEN_BYTES_V6;i++ )
            {
               pStunAddr->ipAddr.v6.d8[i] ^=  pMsgHdr->id.octet[i];
            }
         }
         else
         {
            BOS_ASSERT( BOS_FALSE );
         }
         stunutilStunToBosTransport( pStunAddr, pSocAddr );
      }
      break;
      case STUN_ATTR_CHANGE_REQUEST:
      case STUN_ATTR_REFRESH_INTERVAL:
      case STUN_ATTR_FINGERPRINT:
#if NTCFG_TURN_SUPPORT
      case STUN_ATTR_LIFETIME:
      case STUN_ATTR_BANDWIDTH:
      case STUN_ATTR_REQUESTED_PORT_PROPS:
      case STUN_ATTR_REQUESTED_TRANSPORT:
#endif
#if NTCFG_ICE_SUPPORT
      case STUN_ATTR_PRIORITY:
#endif
      {
         *((BOS_UINT32 *)pData) = *((BOS_UINT32 *)pAttrData);
      }
      break;

      case STUN_ATTR_USERNAME:
      case STUN_ATTR_PASSWORD:
      case STUN_ATTR_MESSAGE_INTEGRITY:
      case STUN_ATTR_REALM:
      case STUN_ATTR_NONCE:
      {
         STUN_DATA *pStunData = (STUN_DATA *)pData;
         STUN_ATTR_OPAQUE  *pOpqData = (STUN_ATTR_OPAQUE *)pAttrData;
         BOS_ASSERT( pStunData->len >= STUN_MAX_DATA_LEN );

         memcpy( pStunData->pData, pOpqData->val, pOpqData->iLen );
         pStunData->len = pOpqData->iLen;
      }
      break;

      case STUN_ATTR_ERRORCODE:
      {
         STUN_ERR *pStunErr = (STUN_ERR *)pData;
         STUN_ATTR_ERRCODE *pErrCode = (STUN_ATTR_ERRCODE *)pAttrData;

         pStunErr->errnoa = (pErrCode->class * 100) + pErrCode->number;
         BOS_ASSERT( NULL != pStunErr->pPhrase );
         strncpy((char *)pStunErr->pPhrase->pData, (char *)pErrCode->phrase, pStunErr->pPhrase->len-1);
         pStunErr->pPhrase->len = (BOS_UINT16)strlen((char *)pStunErr->pPhrase->pData);
      }
      break;

      case STUN_ATTR_XORONLY:
#if NTCFG_ICE_SUPPORT
      case STUN_ATTR_USECANDIDATE:
#endif
      {
         (void)pData;
      }
      break;

#if NTCFG_TURN_SUPPORT
      case STUN_ATTR_DATA:
      {
         STUN_DATA *pStunData = (STUN_DATA *)pData;
         STUN_ATTR_GENDATA *pGenData = (STUN_ATTR_GENDATA *)pAttrData;
         pStunData->pData = pGenData->pBuf;
         pStunData->len = pGenData->len;
      }
      break;
#endif
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }

   return(BOS_TRUE);
}


/*****************************************************************************
*  FUNCTION:   stunEncode
*
*  PURPOSE:    Encode STUN message for sending into network.
*              Take in the STUN message stored in the <pBuf> and
*              overwrite it with encoded payload for sending into
*              the network.
*  PARAMETERS:
*              pBuf     [MOD] - STUN message buffer
*              pLen     [OUT] - payload length
*              pPwd     [IN]  - password for message integrity
*
*  RETURNS:    BOS_TRUE if encoding is successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunEncode( BOS_UINT8                  *pBuf,
                     BOS_UINT16                 *pLen,
                     const STUN_HMAC_PWD        *pPwd,
                     const STUN_OPTS            options )
{
   STUN_MSG msg;
   BOS_BOOL bFingerprint = (options & STUN_OPT_FINGERPRINT)?BOS_TRUE:BOS_FALSE;
 
   memcpy( &msg, pBuf, sizeof(STUN_MSG) );
   memset( pBuf+sizeof(STUN_MSG_HDR), 0, STUN_MSG_BUFFER_SIZE-sizeof(STUN_MSG_HDR) );

   return ( stunmsgEncode( &msg, pBuf, pLen, pPwd, bFingerprint ));
}

/*****************************************************************************
*  FUNCTION:   stunSendRequest
*
*  PURPOSE:    Send an encoded STUN message stored in buffer <pBuf> of
*              length <len> to network destination <pDest> using
*              the socket id <sid>.
*
*              The request is queued and sent by an internal
*              transaction management module.
*
*              <pPwd> is the password that was used in Message Integrity
*              of the request. It's used by the transaction module for
*              response authentication purposes when stunRecvPacket() is
*              called to receive the response.
*
*              If a response is not received, the request is retransmitted
*              per RFC3489/5389.
*
*              The request transaction terminates per the parameters specified
*              via pRetxData or per RFC defines if none are specified
*
*              Response event will be returned synchronously via the
*              <resCB> if the STUN_OPT_SYNCD_RES option ist set in
*              the <options>.
*
*              The caller can also pass in some opaque data <pOpaq> with
*              the request. The data will be passed back in the <resCB>.
*
*  PARAMETERS:
*              sid      [IN]  - Socket id
*              pDest    [IN]  - Destination
*              pBuf     [IN]  - STUN message buffer
*              len      [IN]  - Message length
*              pReqHdl  [OUT] - Request handle
*              resCB    [IN]  - Response Callback
*              pRetxData[IN]  - Retransmission configuration
*              pPwd     [IN]  - Password for message integrity
*              options  [IN]  - STUN options
*              pOpaq    [IN]  - Opaque data to be returned in the
*                               response.
*
*  RETURNS:    BOS_TRUE if encoding is successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunSendRequest( const BOS_UINT32            sid,
                          const STUN_TRANSPORT        *pDest,
                          const BOS_UINT8             *pBuf,
                          const BOS_UINT16            len,
                          BOS_UINT32                  *pReqHdl,
                          const STUN_RESCB            resCB,
                          const STUN_RETRANS          *pRetxData,
                          const STUN_HMAC_PWD         *pPwd,
                          const STUN_OPTS             options,
                          const void                  *pOpaq )
{
   BOS_BOOL rc;
#if STUNDBG_LOG
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
#endif

   STUN_TXID   *pTxid = &((STUN_MSG*)pBuf)->msgHdr.id;

   BOS_ASSERT( (NULL != pDest) &&
               (NULL != pBuf) &&
               (NULL != pReqHdl) &&
               (NULL != resCB) );
 
   /* return request handle to client */
   *pReqHdl = newReqHdl();

   rc = stuntx_Send( sid,
                     pDest,
                     pBuf,
                     len,
                     pTxid,
                     *pReqHdl,
                     resCB,
                     pRetxData,
                     pPwd,
                     options,
                     pOpaq );
#if STUNDBG_LOG
   (void)bosIpAddrGetStr(&pDest->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
   DLOG(( "stunSendRequest: request queued: sid [%lx], dest[addr %s, port %d], txid [%s], reqHdl [%lx]", sid, ipAddrStr, pDest->port, stuntx_IdStr(pTxid), *pReqHdl ));
#endif

   return( rc );
}

/*****************************************************************************
*  FUNCTION:   stunSendResponse
*
*  PURPOSE:    Send an encoded STUN response message stored in buffer
*              <pBuf> of length <len> to network destination <pDest> using
*              the socket id <sid>.
*  PARAMETERS:
*              sid   [IN]  - Socket Id
*              pDest [IN]  - Destination
*              pBuf  [IN]  - Data
*              len   [IN]  - Data len
*
*  RETURNS:    BOS_TRUE if sending is successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunSendResponse( const BOS_UINT32           sid,
                           const STUN_TRANSPORT       *pDest,
                           const BOS_UINT8            *pBuf,
                           const BOS_UINT16           len )
{
#if STUNDBG_LOG
   STUN_TXID   *pTxid = &((STUN_MSG *)pBuf)->msgHdr.id;
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
#endif

   (*stunSendPktCB)( sid, pDest, pBuf, len );

#if STUNDBG_LOG
   (void)bosIpAddrGetStr(&pDest->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
   DLOG(( "stunSendResponse: sid [%lx], dest[addr %s, port %d], txid [%s]", sid, ipAddrStr, pDest->port, stuntx_IdStr(pTxid) ));
#endif

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunSendIndication
*
*  PURPOSE:    Send an encoded STUN Indcation message stored in buffer
*              <pBuf> of length <len> to network destination <pDest> using
*              the socket id <sid>.
*  PARAMETERS:
*              sid   [IN]  - Socket Id
*              pDest [IN]  - Destination
*              pBuf  [IN]  - Data
*              len   [IN]  - Data len
*
*  RETURNS:    BOS_TRUE if sending is successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunSendIndication( const BOS_UINT32         sid,
                             const STUN_TRANSPORT     *pDest,
                             const BOS_UINT8          *pBuf,
                             const BOS_UINT16         len )
{
#if STUNDBG_LOG
   STUN_TXID   *pTxid = &((STUN_MSG *)pBuf)->msgHdr.id;
   char ipAddrStr[BOS_IP_ADDRESS_MAX_STR_LEN];
#endif

   (*stunSendPktCB)( sid, pDest, pBuf, len );

#if STUNDBG_LOG
   (void)bosIpAddrGetStr(&pDest->ipAddr, ipAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN);
   DLOG(( "stunSendIndication: sid [%lx], dest[addr %s, port %d], txid [%s]", sid, ipAddrStr, pDest->port, stuntx_IdStr(pTxid) ));
#endif

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:
*     stunIsStunMsg
*
*  PURPOSE:
*     Detect whether <pData> is a stun message.
*
*  PARAMETERS:
*        pData    [IN] - payload data
*        len      [IN] - data length
*        mode     [IN] - detection mode
*
*  RETURNS:
*        BOS_TRUE if it's a STUN message. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunIsStunMsg( const BOS_UINT8            *pData,
                        const BOS_UINT16           len,
                        const STUN_INSP_MODE       mode )
{
   if ( NULL != pData && len >= (sizeof( STUN_MSG_HDR )) )
   {
      switch( mode )
      {
         case STUN_INSP_BASIC:
         {
            /* Per RFC3489/5389, checks the first
             * 2 bits of the input buffer
             * for 00
             */
            return(((*pData & 0xc0) == 0x00) ? BOS_TRUE:BOS_FALSE);
         }
         break;
#if NTCFG_STUN_MAGIC_COOKIE_SUPPORT
         case STUN_INSP_MAGIC_COOKIE:
         {
            BOS_UINT32 value;
            /* use memcpy due to code aligment issue */            
            memcpy( &value, (pData+MAGIC_COOKIE_OFFSET), sizeof (BOS_UINT32));
            return( (bosSocketNtoHL( value ) == STUN_MAGIC_COOKIE ) ? BOS_TRUE : BOS_FALSE );
         }
         break;
         case STUN_INSP_FINGERPRINT:
         {
            BOS_BOOL rc;
            BOS_UINT32 value;
            /* use memcpy due to code aligment issue */
            memcpy( &value, (pData+MAGIC_COOKIE_OFFSET), sizeof (BOS_UINT32));
            rc = ( (bosSocketNtoHL( value ) == STUN_MAGIC_COOKIE ) ? BOS_TRUE : BOS_FALSE );         
            
            BOS_ASSERT( NULL != stunCrc32CB );
            if ( rc )
            {
               return (stunmsgIsFingerprinted( pData, len ));
            }
            else
            {
               return( BOS_FALSE );
            }
         }
         break;
#endif
         default:
         {
            BOS_ASSERT( BOS_FALSE );
         }
      }
   }

   return( BOS_FALSE );
}

/*****************************************************************************
*
*  FUNCTION:   stunRecvPacket
*
*  PURPOSE:    Process STUN packets received from network.
*
*              The STUN message is first parsed.
*
*              If it's a Response, authentication verification
*              is performed for.
*
*              If parsing fails (including unknown message type),
*              the packet is dropped.
*
*  PARAMETERS:
*           sid      [IN] - socket where the packet is received
*           pLclAddr [IN] - Local Interface address receiving the msg
*           pSrcAddr [IN] - source address of the remote sender
*           pData    [IN] - pointer to data
*           len      [IN] - data length in bytes
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void stunRecvPacket( const BOS_UINT32       sid,
                     const STUN_TRANSPORT   *pLclAddr,
                     const STUN_TRANSPORT   *pSrcAddr,
                     const BOS_UINT8        *pData,
                     const BOS_UINT16       len )
{
   STUN_MSG msg;              /* Buffer for ther parsed incoming message */
   BOS_UINT16 hmacMsgLen;
   BOS_UINT8 hash[STUN_MAX_HASH_LEN];
   STUN_MSGCLASS msgClass;
#if STUNDBG_LOG
   char ipAddrStr1[BOS_IP_ADDRESS_MAX_STR_LEN];
   char ipAddrStr2[BOS_IP_ADDRESS_MAX_STR_LEN];
#endif

   memset( &msg, 0, sizeof( STUN_MSG ) );

#if STUNDBG_LOG
   (void)bosIpAddrGetStr(&pLclAddr->ipAddr, ipAddrStr1, BOS_IP_ADDRESS_MAX_STR_LEN);
   (void)bosIpAddrGetStr(&pSrcAddr->ipAddr, ipAddrStr2, BOS_IP_ADDRESS_MAX_STR_LEN);
   DLOG(("stunRecvPacket: sid [%lx], local[%s, %d], srcaddr[%s, %d], len[%d]", sid, ipAddrStr1, pLclAddr->port, ipAddrStr2, pSrcAddr->port, len ));
#endif

   if ( (NULL == pData) || (len <= 0) )
   {
      /* ignore message */
      return;
   }
 
   /* Parse stun message and extract authentication data */
   if ( BOS_TRUE != stunmsgParse( pData, len, hash, &hmacMsgLen, &msg ) )
   {
      return;
   }

   msgClass = stunmsgClass( stunmsgGetType(&msg) );

   switch( msgClass )
   {
      case STUN_SUCCESS_RESPONSE:
      case STUN_ERROR_RESPONSE:
      {
         STUN_RES_DATA resData;
         BOS_UINT32 reqHdl = 0;
         STUN_RESCB resCB = NULL;
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
         BOS_SEM  *pSyncdResSem;
#endif
         memset( &resData, 0, sizeof(STUN_RES_DATA) );
         /* Authenticate responses */
         if ( BOS_TRUE != stuntx_Authenticate( pData,
                                               hash,
                                               hmacMsgLen,
                                               &reqHdl,
                                               &resData.resCnt,
                                               &resData.reqSid,
                                               &resData.pReqDest,
                                               &resData.pOpaq,
                                               &resCB,
                                               &resData.options
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
                                               ,
                                               &pSyncdResSem
#endif
                                               ) )
         {
            DLOG(("stunRecvPacket: STUN response authentication failed"));
            return;
         }

         resData.evt = STUN_RESPONSE;
         resData.sid = sid;
         resData.pLclAddr = (STUN_TRANSPORT  *)pLclAddr;
         resData.pSrcAddr = (STUN_TRANSPORT  *)pSrcAddr;
         resData.pMsg = (BOS_UINT8 *)&msg;

         BOS_ASSERT( NULL != resCB );

         if ( BOS_TRUE != (*resCB)( reqHdl, &resData ) )
         {
            DLOG(("stunRecvPacket: error processing stun response message"));
         }
#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
         if ( NULL != pSyncdResSem )
         {
            bosSemGive( pSyncdResSem );
         }
#endif
      }
      break;
      case STUN_REQUEST:
      case STUN_INDICATION:
      {
         BOS_UINT16 msgType;
         STUN_REQ_DATA reqData;
         memset( &reqData, 0, sizeof(STUN_REQ_DATA) );

         reqData.sid = sid;
         reqData.pLclAddr = (STUN_TRANSPORT  *)pLclAddr;
         reqData.pSrcAddr = (STUN_TRANSPORT  *)pSrcAddr;
         reqData.pMsg = (BOS_UINT8 *)&msg;
         reqData.pHashData = (BOS_UINT8 *)pData;
         reqData.hashDataLen = hmacMsgLen;
         reqData.pHash = hash;

         msgType = stunGetMsgType( (BOS_UINT8 *)&msg );
         switch( msgType )
         {
            case STUN_MSG_BINDING_REQUEST:
            {
               if ( NULL != stunBindingReqCB )
               {
                  (*stunBindingReqCB)( &reqData );
               }
            }
            break;
#if NTCFG_TURN_SUPPORT
            case STUN_MSG_DATA_INDICATION:
            {
               if ( NULL != stunDataIndCB )
               {
                  (*stunDataIndCB)( &reqData );
               }
            }
            break;
            case STUN_MSG_CONNECT_STATUS_INDICATION:
            {
               if ( NULL != stunConnectStatusIndCB )
               {
                  (*stunConnectStatusIndCB)( &reqData );
               }
            }
            break;
#endif
            default:
            {
               /* Ignore other requests */
               DLOG(("stunRecvPacket: STUN request [%d] ignored", msgType));
            }
         }
      }
      break;
      default:
      {
         DLOG(("stunRecvPacket: STUN msg class [%d] not processed", msgClass));
      }
   }
}

/*****************************************************************************
*
*  FUNCTION:   stunSetRequestCallback
*
*  PURPOSE:    Set STUN reqest callback per request type.
*
*  PARAMETERS:
*           request  [IN] - STUN request type
*           reqCB    [IN] - Callback
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunSetRequestCallback( const BOS_UINT16   request,
                                 const STUN_REQCB   reqCB )
{
   switch( request )
   {
      case STUN_MSG_BINDING_REQUEST:
      {
         stunBindingReqCB = reqCB;
         return( BOS_TRUE );
      }
      break;
      default:
      {
         return( BOS_FALSE );
      }
   }
}

/*****************************************************************************
*
*  FUNCTION:   stunSetIndicationCallback
*
*  PURPOSE:    Set STUN Indication callback per Indication type.
*
*  PARAMETERS:
*           request  [IN] - STUN request type
*           reqCB    [IN] - Callback
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunSetIndicationCallback( const BOS_UINT16   indication,
                                    const STUN_INDCB   indCB )
{
#if !NTCFG_TURN_SUPPORT
   (void) indCB;
#endif

   switch( indication )
   {
#if NTCFG_TURN_SUPPORT
      case STUN_MSG_DATA_INDICATION:
      {
         stunDataIndCB = indCB;
         return( BOS_TRUE );
      }
      break;
      case STUN_MSG_CONNECT_STATUS_INDICATION:
      {
         stunConnectStatusIndCB = indCB;
         return( BOS_TRUE );
      }
      break;
#endif
      default:
      {
         return( BOS_FALSE );
      }
   }
}

/*****************************************************************************
*
*  FUNCTION:   stunIntegrityCheck
*
*  PURPOSE:    Check message integrity, return BOS_TRUE if pass, BOS_FALSE
*              otherwise.
*              Supports HMAC SH1 only.
*
*  PARAMETERS:
*           pMsg[IN] - raw Stun message to perform integrity check on
*           pHash[IN] - hash value to compare against
*           len [IN] - message length for integrity check
*                      (excluding the Integrity attribute)
*           pPwd [IN] - password
*
*  RETURNS:    BOS_TRUE - integrity check passed
*              BOS_FAIL - failed check.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunIntegrityCheck( const void *pMsg, const void *pHash, const BOS_UINT16 len, const STUN_HMAC_PWD *pPwd )
{
   STUN_DATA key;
   STUN_DATA data;
   STUN_DATA  hmac;
   unsigned char mac[STUN_MAX_HASH_LEN];
   unsigned char rawMsg[STUN_MSG_BUFFER_SIZE];

   BOS_ASSERT( NULL != pPwd );
   BOS_ASSERT( NULL != pMsg );
   if ( NULL == stunHmacCB )
   {
      return ( BOS_FALSE );
   }
   /* copy the STUN message */
   memcpy( rawMsg, pMsg, len );
   /* Update the length in the STUN header to exclude the fingerprint Attribute */
   ((STUN_MSG_HDR *)rawMsg)->len = bosSocketHtoNS( len - sizeof(STUN_MSG_HDR) + STUN_MAX_HASH_LEN + sizeof(STUN_ATTR_HDR));

   hmac.pData = (void*)mac;
   hmac.len = 0;

   key.pData = (void *)pPwd->val;
   key.len = pPwd->len;
   data.pData = (void*)rawMsg;
   data.len = len;

   (*stunHmacCB)( STUN_HMAC_SHA1,
                  &key,
                  &data,
                  &hmac );
   /* make sure our HMAC CB return a MAC with a size of STUN_MAX_HASH_LEN */
   BOS_ASSERT( STUN_MAX_HASH_LEN == hmac.len );

   if ( memcmp( hmac.pData, pHash, hmac.len ) != 0 )
   {
      int i;
      /* integrity check failed */
      DLOG(("msgIntegrityCheck: Failed, pwd=%s", pPwd->val));
      
      for (i = 0; i < 20; i += 4)
      {
         int value1;
         int value2;
         memcpy(&value1,  &(((char*)pHash)[i]), sizeof(int));
         memcpy(&value2,  &hmac.pData[i], sizeof(int));
         DLOG(("    Given Hash (0x%0x), Calc Hash (0x%0x)", value1, value2 ));
      }
      return( BOS_FALSE );
   }
   DLOG(("msgIntegrityCheck: Passed"));
   return( BOS_TRUE );
}


/*****************************************************************************
*
*  FUNCTION:   stunCancelRequest
*
*  PURPOSE:    Cancels the STUN Binding request outstanding.
*
*  PARAMETERS:
*              uReqHdl    [IN]- Request Handle
*
*  RETURNS:    BOS_TRUE if OK. BOS_FALSE otherwise.
*
*  NOTES:
*           If the request was still outstanding (i.e. being
*           retransmitted), upon this action a timeout event for
*           the request will be issued and the application should be
*           handling it accordingly.
*
*****************************************************************************/
BOS_BOOL stunCancelRequest( const BOS_UINT32 uReqHdl )
{
   return stuntx_CancelRequest( uReqHdl );
}


/*****************************************************************************
*  FUNCTION:
*     newReqHdl
*
*  PURPOSE:
*     return a new request handle.
*
*  PARAMETERS:
*
*  RETURNS:
*     reqHdl
*
*  NOTES:
*
*****************************************************************************/
static BOS_UINT32 newReqHdl( void )
{
   static BOS_UINT16 reqHdl = 0;

   return( (BOS_UINT32) reqHdl++ );
} /* newReqHdl */

#if STUN_SELFTEST
void selftest()
{
   // Test samples from RFC5769
   STUN_DATA testIntegrity;
   BOS_UINT8 pHash[20];
   BOS_UINT16 HmacMsgLen;
   STUN_MSG msgStun;
   STUN_HMAC_PWD testPwd;
   BOS_UINT8 iBuf[STUN_MAX_DATA_LEN];
   BOS_BOOL bCheckResult;
   const char * pTestPassword = "VOkJxbRl1RmTxUk/WvJxBt";

   {
      // Sample 1.  Sample Request
      //Username:  "evtj:h6vY" (without quotes)
      //Password:  "VOkJxbRl1RmTxUk/WvJxBt" (without quotes)
      unsigned char stunMsgExample[] = {
         0x00, 0x01, 0x00, 0x58,
         0x21, 0x12, 0xa4, 0x42,
         0xb7, 0xe7, 0xa7, 0x01,
         0xbc, 0x34, 0xd6, 0x86,
         0xfa, 0x87, 0xdf, 0xae,
         0x80, 0x22, 0x00, 0x10,
         0x53, 0x54, 0x55, 0x4e,
         0x20, 0x74, 0x65, 0x73,
         0x74, 0x20, 0x63, 0x6c,
         0x69, 0x65, 0x6e, 0x74,
         0x00, 0x24, 0x00, 0x04,
         0x6e, 0x00, 0x01, 0xff,
         0x80, 0x29, 0x00, 0x08,
         0x93, 0x2f, 0xf9, 0xb1,
         0x51, 0x26, 0x3b, 0x36,
         0x00, 0x06, 0x00, 0x09,
         0x65, 0x76, 0x74, 0x6a,
         0x3a, 0x68, 0x36, 0x76,
         0x59, 0x20, 0x20, 0x20,
         0x00, 0x08, 0x00, 0x14,
         0x9a, 0xea, 0xa7, 0x0c,
         0xbf, 0xd8, 0xcb, 0x56,
         0x78, 0x1e, 0xf2, 0xb5,
         0xb2, 0xd3, 0xf2, 0x49,
         0xc1, 0xb5, 0x71, 0xa2,
         0x80, 0x28, 0x00, 0x04,
         0xe5, 0x7a, 0x3b, 0xcf };


      testIntegrity.pData = iBuf;
      testIntegrity.len = STUN_MAX_DATA_LEN;
      testPwd.len = (BOS_UINT16)strlen(pTestPassword);
      memcpy( testPwd.val, pTestPassword, testPwd.len);
      stunmsgParse( stunMsgExample, 
                     sizeof(stunMsgExample), 
                     pHash, 
                     &HmacMsgLen,
                     &msgStun );
      bCheckResult = stunIntegrityCheck( stunMsgExample,
                                pHash,
                                HmacMsgLen,
                                &testPwd );
      BOS_ASSERT( bCheckResult == BOS_TRUE );  /* failing self-test */
   }

   {
      // Sample 2.  Sample IPv4 Response
      // Password:  "VOkJxbRl1RmTxUk/WvJxBt" (without quotes)
      // Software name:  "test vector" (without quotes)
      // Mapped address:  192.0.2.1 port 32853

      unsigned char stunMsgExample2[] = {      
         0x01, 0x01, 0x00, 0x3c,
         0x21, 0x12, 0xa4, 0x42,
         0xb7, 0xe7, 0xa7, 0x01,
         0xbc, 0x34, 0xd6, 0x86,
         0xfa, 0x87, 0xdf, 0xae,
         0x80, 0x22, 0x00, 0x0b,
         0x74, 0x65, 0x73, 0x74,
         0x20, 0x76, 0x65, 0x63,
         0x74, 0x6f, 0x72, 0x20,
         0x00, 0x20, 0x00, 0x08,
         0x00, 0x01, 0xa1, 0x47,
         0xe1, 0x12, 0xa6, 0x43,
         0x00, 0x08, 0x00, 0x14,
         0x2b, 0x91, 0xf5, 0x99,
         0xfd, 0x9e, 0x90, 0xc3,
         0x8c, 0x74, 0x89, 0xf9,
         0x2a, 0xf9, 0xba, 0x53,
         0xf0, 0x6b, 0xe7, 0xd7,
         0x80, 0x28, 0x00, 0x04,
         0xc0, 0x7d, 0x4c, 0x96 };
      testIntegrity.pData = iBuf;
      testIntegrity.len = STUN_MAX_DATA_LEN;
      testPwd.len = (BOS_UINT16)strlen(pTestPassword);
      memcpy( testPwd.val, pTestPassword, testPwd.len);
      stunmsgParse( stunMsgExample2, 
                     sizeof(stunMsgExample2), 
                     pHash, 
                     &HmacMsgLen,
                     &msgStun );
      bCheckResult = stunIntegrityCheck( stunMsgExample2,
                                pHash,
                                HmacMsgLen,
                                &testPwd );
      BOS_ASSERT( bCheckResult == BOS_TRUE );  /* failing self-test */
   }
   {
      // Sample 3.  Sample IPv6 Response
      // Password:  "VOkJxbRl1RmTxUk/WvJxBt" (without quotes)
      // Software name:  "test vector" (without quotes)
      // Mapped address:  2001:db8:1234:5678:11:2233:4455:6677 port 32853

      unsigned char stunMsgExample3[] = {      
         0x01, 0x01, 0x00, 0x48,
         0x21, 0x12, 0xa4, 0x42,
         0xb7, 0xe7, 0xa7, 0x01,
         0xbc, 0x34, 0xd6, 0x86,
         0xfa, 0x87, 0xdf, 0xae,
         0x80, 0x22, 0x00, 0x0b,
         0x74, 0x65, 0x73, 0x74,
         0x20, 0x76, 0x65, 0x63,
         0x74, 0x6f, 0x72, 0x20,
         0x00, 0x20, 0x00, 0x14,
         0x00, 0x02, 0xa1, 0x47,
         0x01, 0x13, 0xa9, 0xfa,
         0xa5, 0xd3, 0xf1, 0x79,
         0xbc, 0x25, 0xf4, 0xb5,
         0xbe, 0xd2, 0xb9, 0xd9,
         0x00, 0x08, 0x00, 0x14,
         0xa3, 0x82, 0x95, 0x4e,
         0x4b, 0xe6, 0x7b, 0xf1,
         0x17, 0x84, 0xc9, 0x7c,
         0x82, 0x92, 0xc2, 0x75,
         0xbf, 0xe3, 0xed, 0x41,
         0x80, 0x28, 0x00, 0x04,
         0xc8, 0xfb, 0x0b, 0x4c };
      testIntegrity.pData = iBuf;
      testIntegrity.len = STUN_MAX_DATA_LEN;
      testPwd.len = (BOS_UINT16)strlen(pTestPassword);
      memcpy( testPwd.val, pTestPassword, testPwd.len);
      stunmsgParse( stunMsgExample3, 
                     sizeof(stunMsgExample3), 
                     pHash, 
                     &HmacMsgLen,
                     &msgStun );
      
      bCheckResult = stunIntegrityCheck( stunMsgExample3,
                                pHash,
                                HmacMsgLen,
                                &testPwd );
      BOS_ASSERT( bCheckResult == BOS_TRUE );  /* failing self-test */
   }
}
#endif
