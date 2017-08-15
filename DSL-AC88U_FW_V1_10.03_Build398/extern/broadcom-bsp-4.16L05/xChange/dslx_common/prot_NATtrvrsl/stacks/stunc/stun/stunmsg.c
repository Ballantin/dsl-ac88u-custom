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
*      STUN Message Encoder Parser for the STUN module
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <bosTypes.h>
#include <bosSocket.h>
#include <ntcfg.h>
#include "./inc/stun.h"
#include "stunmsg.h"
#include "stuni.h"
#include "stundbg.h"

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
#define VERIFY(x) if (BOS_TRUE != x) return (BOS_FALSE)
#define TRAP(x) if (x) return (BOS_FALSE)
#define MESSAGE_INTEGRITY_ATTR_SIZE (sizeof(STUN_ATTR_HDR)+sizeof(STUN_ATTR_OPAQUE))
#define FINGERPRINT_XOR_VAL         0x5354554e
#define FINGERPRINT_ATTR_SIZE       (sizeof(STUN_ATTR_HDR)+sizeof(STUN_ATTR_UINT32))

/* Attribute encode function */
typedef BOS_BOOL (*ATTR_ENCODE_FUNC)( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData );

/* Attribute parsing function */
typedef BOS_BOOL (*ATTR_PARSE_FUNC)( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData );

/* Attribute data types */
typedef enum
{
   ATTR_ADDR,
   ATTR_UINT32,
   ATTR_OPAQUE,
   ATTR_FLAG,
   ATTR_ERRCODE,
   ATTR_DATA,
   ATTR_UNKNOWN,
   ATTR_UNSUPPORTED = -1
} ATTR_DATATYPE;

typedef struct
{
   BOS_UINT16        stunAttr;
   BOS_UINT32        attrBit;
   ATTR_DATATYPE     dataType;
} STUN_ATTRMAP;

/* ---- Private Variables ------------------------------------ */

static const STUN_ATTRMAP stunAttrMapList[] =
{
   {STUN_ATTR_MAPPED_ADDRESS,    ATTR_BIT_MAPPED_ADDR,   ATTR_ADDR},
   {STUN_ATTR_RESPONSE_ADDRESS,  ATTR_BIT_RESP_ADDR,     ATTR_ADDR},
   {STUN_ATTR_CHANGE_REQUEST,    ATTR_BIT_CHANGE_REQ,    ATTR_UINT32},
   {STUN_ATTR_SOURCE_ADDRESS,    ATTR_BIT_SOURCE_ADDR,   ATTR_ADDR},
   {STUN_ATTR_CHANGED_ADDRESS,   ATTR_BIT_CHANGED_ADDR,  ATTR_ADDR},
   {STUN_ATTR_USERNAME,          ATTR_BIT_USERNAME,      ATTR_OPAQUE},
   {STUN_ATTR_PASSWORD,          ATTR_BIT_PASSWORD,      ATTR_OPAQUE},
   {STUN_ATTR_MESSAGE_INTEGRITY, ATTR_BIT_MSG_INTEGRITY, ATTR_OPAQUE},
   {STUN_ATTR_ERRORCODE,         ATTR_BIT_ERR_CODE,      ATTR_ERRCODE},
   {STUN_ATTR_UNKNOWN_ATTRIBUTE, ATTR_BIT_UNKNOWN_ATTRS, ATTR_UNKNOWN },
   {STUN_ATTR_REFLECTED_FROM,    ATTR_BIT_REFLECTED_FROM, ATTR_ADDR},
   {STUN_ATTR_XORMAPPED_ADDRESS, ATTR_BIT_XOR_MAPPED_ADDR, ATTR_ADDR},
   {STUN_ATTR_XORONLY,           ATTR_BIT_XOR_ONLY,      ATTR_FLAG},
   {STUN_ATTR_SERVERNAME,        ATTR_BIT_SERVER,        ATTR_OPAQUE},
   {STUN_ATTR_FINGERPRINT,       ATTR_BIT_FINGERPRINT,   ATTR_UINT32},
#if NTCFG_TURN_SUPPORT
   {STUN_ATTR_LIFETIME,             ATTR_BIT_LIFETIME,   ATTR_UINT32},
   {STUN_ATTR_BANDWIDTH,            ATTR_BIT_BANDWIDTH,  ATTR_UINT32},
   {STUN_ATTR_REMOTE_ADDRESS,       ATTR_BIT_REMOTE_ADDR, ATTR_ADDR},
   {STUN_ATTR_DATA,                 ATTR_BIT_DATA,       ATTR_DATA},
   {STUN_ATTR_RELAY_ADDRESS,        ATTR_BIT_RELAY_ADDR, ATTR_ADDR},
   {STUN_ATTR_REQUESTED_PORT_PROPS, ATTR_BIT_REQUESTED_PORT_PROPS, ATTR_UINT32},
   {STUN_ATTR_REQUESTED_TRANSPORT,  ATTR_BIT_REQUESTED_TRANSPORT, ATTR_UINT32},
   {STUN_ATTR_REQUESTED_IP,         ATTR_BIT_REQUESTED_IP, ATTR_ADDR},
   {STUN_ATTR_TIMER_VAL,            ATTR_BIT_TIMER_VAL,  ATTR_UINT32},
#endif
   
#if NTCFG_ICE_SUPPORT
   {STUN_ATTR_PRIORITY,          ATTR_BIT_PRIORITY,      ATTR_UINT32},
   {STUN_ATTR_USECANDIDATE,      ATTR_BIT_USECANDIDATE,  ATTR_FLAG},
#endif

   {(BOS_UINT16)-1,              ATTR_BIT_UNSUPPORTED,   ATTR_UNSUPPORTED},
};

/* Reason phrase map */
typedef struct
{
   BOS_UINT16  number;
   char        *pPhrase;
} REASONSTR_MAP;

static REASONSTR_MAP  reasonStrMap[] = 
{
   {400, "Bad Request "},
   {401, "Unauthorized"},
   {420, "Unknown Attribute   "},
   {430, "Stale Credentials   "},
   {431, "Integrity Check Failure "},
   {432, "Missing Username"},
   {433, "Use TLS "},

   {437, "No Binding  "},
   {439, "Transitioning   "},
   {441, "Wrong Username  "},
   {442, "Unsupported Transport Protocol  "},
   {443, "Invalid IP Address  "},
   {444, "Invalid Port"},
   {445, "Operation for TCP Only  "},
   {446, "Connection Failure  "},
   {447, "Connection Timeout  "},
   {(BOS_UINT16)-1, ""},
};


/* ---- Private Function Prototypes -------------------------- */
static void *getAttrStructDataPtr( STUN_MSG *pMsg, BOS_UINT16 attr );
static ATTR_DATATYPE getAttrDataType( BOS_UINT16 attr );
static ATTR_ENCODE_FUNC getAttrEncodeFunc( BOS_UINT16 attr );
static ATTR_PARSE_FUNC getAttrParseFunc( BOS_UINT16 attr );
static BOS_BOOL stunEncodeAttrs( STUN_MSG *pMsg, BOS_UINT8 **ppBuf, BOS_UINT8 *pBufEnd );
static BOS_BOOL stunParseAttrs( const BOS_UINT8 *pBuf, const BOS_UINT16 bufLen, STUN_MSG *pMsg, BOS_UINT8 *pHash, BOS_UINT16 *pHmacAttrLen );
static void stunSetAttrBitMap( BOS_UINT16 attrType, BOS_UINT32 *pAttrBitMap );
static void stunAttrBitToAttr( const STUN_ATTRMAP *pMapList, BOS_UINT32 attrBit, BOS_UINT16 *pAttrType );
static BOS_BOOL stunParseAttrAddr( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData );
static BOS_BOOL stunParseAttrErrCode( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData );
static BOS_BOOL stunParseAttrUnknown( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData );
static BOS_BOOL stunParseAttrOpq( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData );
#if NTCFG_TURN_SUPPORT
static BOS_BOOL stunParseAttrData( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData );
#endif /* NTCFG_TURN_SUPPORT */
static BOS_BOOL stunParseAttrUINT32( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData );
static BOS_BOOL stunParseAttrFlag( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData );
static BOS_BOOL stunEncodeAttrAddr( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData );
static BOS_BOOL stunEncodeAttrErrCode( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData );
static BOS_BOOL stunEncodeAttrUnknown( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData );
static BOS_BOOL stunEncodeAttrOpq( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData );
#if NTCFG_TURN_SUPPORT
static BOS_BOOL stunEncodeAttrData( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData );
#endif /* NTCFG_TURN_SUPPORT */
static BOS_BOOL stunEncodeAttrUINT32( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData );
static BOS_BOOL stunEncodeAttrFlag( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData );
static BOS_BOOL stunHmac( const BOS_UINT8 *pData, const BOS_UINT16 dataLen, const BOS_UINT8 *pKey, const BOS_UINT16 keyLen, BOS_UINT8 *pHmacData );
static BOS_UINT16 adjustedAttrLen( const BOS_UINT16 attrLen );
static BOS_BOOL stunmsgAttrPresent( const STUN_MSG *pMsg, BOS_UINT16 attrType );
static BOS_BOOL stunParseMsgHdr( const BOS_UINT8 *pBuf, const BOS_UINT16 bufLen, STUN_MSG *pMsg );
/* ---- Functions -------------------------------------------- */
/*****************************************************************************
*  FUNCTION:   stunmsgSetType
*
*  PURPOSE:    Set message type for the STUN message to be encoded
*
*  PARAMETERS:
*          pMsg   [IN]  - stun message
*
*  RETURNS:    
*
*  NOTES:
*
*****************************************************************************/
void stunmsgSetType( STUN_MSG *pMsg, BOS_UINT16 msgType )
{
   BOS_ASSERT( NULL != pMsg );
   pMsg->msgHdr.msgType = msgType;
}

/*****************************************************************************
*  FUNCTION:   stunmsgGetType
*
*  PURPOSE:    Get message type of the parsed STUN message
*
*  PARAMETERS:
*          pMsg   [IN]  - stun message
*
*  RETURNS:    
*
*  NOTES:
*
*****************************************************************************/
BOS_UINT16 stunmsgGetType( STUN_MSG *pMsg )
{
   BOS_ASSERT( NULL != pMsg );
   return ( pMsg->msgHdr.msgType );
}

/*****************************************************************************
*  FUNCTION:   stunmsgSetAttr
*
*  PURPOSE:    Set Attribute and data 
*
*  PARAMETERS:
*          pMsg   [IN]  - stun message
*          attr   [IN]  - STUN attribute
*          pData  [IN]  - attribute data
*
*  RETURNS:    
*
*  NOTES:
*
*****************************************************************************/
void stunmsgSetAttr( STUN_MSG *pMsg, BOS_UINT16 attr, void *pData )
{
   void *pStructData;
   int structSize = 0;
   ATTR_DATATYPE dataType = getAttrDataType( attr );

   BOS_ASSERT( NULL != pMsg );

   if ( NULL == (pStructData = getAttrStructDataPtr( pMsg, attr )))
   {
      BOS_ASSERT( BOS_FALSE );
   }

   /* Set attribute data */
   switch( dataType )
   {
      case ATTR_ADDR:
      { structSize = sizeof( STUN_ATTR_ADDR ); break; }
      case ATTR_UINT32:
      { structSize = sizeof( BOS_UINT32 ); break; }
      case ATTR_OPAQUE:
      { structSize = sizeof( STUN_ATTR_OPAQUE ); break; }
      break;
      case ATTR_FLAG:
      { structSize = sizeof( BOS_BOOL ); break; }
      case ATTR_ERRCODE:
      { structSize = sizeof( STUN_ATTR_ERRCODE ); break; }
      case ATTR_UNKNOWN:
      { structSize = sizeof( STUN_ATTR_UNKNOWN_ATTRS ); break; }
#if NTCFG_TURN_SUPPORT
      case ATTR_DATA:
      { structSize = sizeof( STUN_ATTR_GENDATA ); break; }
#endif
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }

   if ( NULL != pData )
   {
      memcpy( pStructData, pData, structSize );
   }

   /* Set attribute Bit Map */
   stunSetAttrBitMap( attr, &pMsg->attrBitMap );
}

/*****************************************************************************
*  FUNCTION:   stunmsgGetAttr
*
*  PURPOSE:    Get Attribute data.
*
*  PARAMETERS:
*          pMsg   [IN]  - stun message
*          attr   [IN]  - STUN attribute
*
*  RETURNS:   NULL if attribute is not present 
*
*  NOTES:
*
*****************************************************************************/
void *stunmsgGetAttr( STUN_MSG *pMsg, BOS_UINT16 attr )
{
   void *pStructData;
   ATTR_DATATYPE dataType;

   BOS_ASSERT( NULL != pMsg );

   if ( !stunmsgAttrPresent( pMsg, attr ) )
   {
      return( NULL );
   }

   dataType = getAttrDataType( attr );

   if ( NULL == (pStructData = getAttrStructDataPtr( pMsg, attr )))
   {
      BOS_ASSERT( BOS_FALSE );
   }

   return( pStructData );
}

/*****************************************************************************
*  FUNCTION:   getAttrStructDataPtr
*
*  PURPOSE:    Return a pointer to the attribute data structure.
*
*  PARAMETERS:
*          pMsg   [IN]  - stun message
*          attr   [IN]  - STUN attribute
*
*  RETURNS:    
*
*  NOTES:
*
*****************************************************************************/
static void *getAttrStructDataPtr( STUN_MSG *pMsg, BOS_UINT16 attr )
{
   BOS_ASSERT( NULL != pMsg );

   switch( attr )
   {
      case STUN_ATTR_MAPPED_ADDRESS:
         return( (void*)&pMsg->attr.stun.mappedAddr );break;
      case STUN_ATTR_RESPONSE_ADDRESS:
         return( (void*)&pMsg->attr.stun.respAddr );break;
      case STUN_ATTR_CHANGE_REQUEST:
         return( (void*)&pMsg->attr.stun.chgReq );break;
      case STUN_ATTR_SOURCE_ADDRESS:
         return( (void*)&pMsg->attr.stun.srcAddr );break;
      case STUN_ATTR_CHANGED_ADDRESS:
         return( (void*)&pMsg->attr.stun.chgdAddr );break;
      case STUN_ATTR_PASSWORD:
         return( (void*)&pMsg->attr.stun.password );break;
      case STUN_ATTR_REFLECTED_FROM:
         return( (void*)&pMsg->attr.stun.reflectedFrom );break;

      case STUN_ATTR_USERNAME:
         return( (void*)&pMsg->username );break;
      case STUN_ATTR_MESSAGE_INTEGRITY:
         return( (void*)&pMsg->msgIntegrity );break;
      case STUN_ATTR_ERRORCODE:
         return( (void*)&pMsg->errCode );break;

/* bis-extensions */
      case STUN_ATTR_REALM:
         return( (void*)NULL );break;
      case STUN_ATTR_NONCE:
         return( (void*)NULL );break;
      case STUN_ATTR_XORMAPPED_ADDRESS:
         return( (void*)&pMsg->xorMappedAddr );break;
      case STUN_ATTR_XORONLY:
         return( (void*)&pMsg->attr.stun.xorOnly );break;
      case STUN_ATTR_SERVERNAME:
         return( (void*)&pMsg->attr.stun.server);break;
      case STUN_ATTR_ALTERNATE_SERVER:
         return( (void*)NULL);break;
      case STUN_ATTR_REFRESH_INTERVAL:
         return( (void*)NULL);break;
      case STUN_ATTR_FINGERPRINT:
         return( (void*)&pMsg->fingerprint );break;

#if NTCFG_TURN_SUPPORT
/* draft-ietf-behave-turn - Usage: Relay */
      case STUN_ATTR_LIFETIME:
         return( (void*)&pMsg->attr.turn.lifetime );break;
      case STUN_ATTR_BANDWIDTH:
         return( (void*)&pMsg->attr.turn.bandwidth );break;
      case STUN_ATTR_REMOTE_ADDRESS:
         return( (void*)&pMsg->attr.turn.remoteAddr );break;
      case STUN_ATTR_DATA:
         return( (void*)&pMsg->attr.turn.data );break;
      case STUN_ATTR_RELAY_ADDRESS:
         return( (void*)&pMsg->attr.turn.relayAddr );break;
      case STUN_ATTR_REQUESTED_PORT_PROPS:
         return( (void*)&pMsg->attr.turn.reqdPortProps );break;
      case STUN_ATTR_REQUESTED_TRANSPORT:
         return( (void*)&pMsg->attr.turn.reqdTransport );break;
      case STUN_ATTR_REQUESTED_IP:
         return( (void*)&pMsg->attr.turn.reqdIP );break;
      case STUN_ATTR_TIMER_VAL:
         return( (void*)&pMsg->attr.turn.timerVal );break;
#endif

#if NTCFG_ICE_SUPPORT
/* draft-ietf-mmusic-ice - Usage: Connectivity Check */
      case STUN_ATTR_PRIORITY:
         return( (void*)&pMsg->attr.stun.priority );break;
      case STUN_ATTR_USECANDIDATE:
         return( (void*)&pMsg->attr.stun.useCandidate );break;
#endif
      default:
      {
         LOG(("getAttrStructDataPtr: Unsupported attribute [%d]", attr));
         return ((void *)NULL);
      }
   }
}

/*****************************************************************************
*  FUNCTION:   getAttrDataType
*
*  PURPOSE:    Get attribute Data Type
*
*  PARAMETERS:
*          attr   [IN]  - STUN attribute
*
*  RETURNS:    
*
*  NOTES:
*
*****************************************************************************/
static ATTR_DATATYPE getAttrDataType( BOS_UINT16 attr )
{
   int i;
   for ( i = 0; stunAttrMapList[i].stunAttr != (BOS_UINT16)-1; i++ )
   {
      if ( stunAttrMapList[i].stunAttr == attr )
      {
         return( stunAttrMapList[i].dataType );
      }
   }

   return( ATTR_UNSUPPORTED );
}

/*****************************************************************************
*  FUNCTION:   getAttrEncodeFunc
*
*  PURPOSE:    Get attribute encoding function
*
*  PARAMETERS:
*          attr   [IN]  - STUN attribute
*
*  RETURNS:    
*
*  NOTES:
*
*****************************************************************************/
static ATTR_ENCODE_FUNC getAttrEncodeFunc( BOS_UINT16 attr )
{
   ATTR_DATATYPE dataType = getAttrDataType( attr );
   switch( dataType )
   {
      case ATTR_ADDR:
         return stunEncodeAttrAddr; break;
      case ATTR_UINT32:
         return stunEncodeAttrUINT32; break;
      case ATTR_OPAQUE:
         return stunEncodeAttrOpq; break;
      case ATTR_FLAG:
         return stunEncodeAttrFlag; break;
      case ATTR_ERRCODE:
         return stunEncodeAttrErrCode; break;
#if NTCFG_TURN_SUPPORT
      case ATTR_DATA:
         return stunEncodeAttrData; break;
#endif
      case ATTR_UNKNOWN:
         return stunEncodeAttrUnknown; break;
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }

   return( NULL );
}

/*****************************************************************************
*  FUNCTION:   getAttrParseFunc
*
*  PURPOSE:    Get attribute parsing function
*
*  PARAMETERS:
*          attr   [IN]  - STUN attribute
*
*  RETURNS:    
*
*  NOTES:
*
*****************************************************************************/
static ATTR_PARSE_FUNC getAttrParseFunc( BOS_UINT16 attr )
{
   ATTR_DATATYPE dataType = getAttrDataType( attr );

   switch( dataType )
   {
      case ATTR_ADDR:
         return stunParseAttrAddr; break;
      case ATTR_UINT32:
         return stunParseAttrUINT32; break;
      case ATTR_OPAQUE:
         return stunParseAttrOpq; break;
      case ATTR_FLAG:
         return stunParseAttrFlag; break;
      case ATTR_ERRCODE:
         return stunParseAttrErrCode; break;
#if NTCFG_TURN_SUPPORT
      case ATTR_DATA:
         return stunParseAttrData; break;
#endif
      case ATTR_UNKNOWN:
         return stunParseAttrUnknown; break;
      case ATTR_UNSUPPORTED:
      default:
      {
         DLOG(("getAttrParseFunc: No parsing function for unsupported attribute type"));
      }
   }
   
   return( NULL );
}


/*****************************************************************************
*  FUNCTION:   stunEncode
*
*  PURPOSE:    Encode the stun message <pMsg> to <pBuf> for sending into
*              network.
*
*  PARAMETERS:
*          pMsg   [IN]  - stun message
*          pBuf   [OUT] - buffer to contain the data
*          pBufLen [MOD] - size of buffer, and actual size used
*          pPwd   [IN]  - password if not NULL, Messsage Integrity
*                         will be inserted
*
*  RETURNS:    BOS_TRUE of encoding is successful. BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunmsgEncode( STUN_MSG *pMsg, BOS_UINT8 *pBuf, BOS_UINT16 *pBufLen, const STUN_HMAC_PWD *pPwd, const BOS_BOOL bFingerprint )
{
   BOS_UINT8 *pBufHead = pBuf; /* beginning of input buffer */
   BOS_UINT8 *pBufEnd;  /* end of input buffer */
   BOS_UINT16 msgLen;
   BOS_UINT8 hmac[STUN_MAX_HASH_LEN];
   BOS_BOOL bMsgIntegrity = BOS_FALSE;
   BOS_UINT16 fingerprintLen = 0;

   BOS_ASSERT( (NULL != pBuf) && (NULL != pBufLen) && (NULL != pMsg) );

   pBufEnd = pBufHead + *pBufLen - 1;

   /* skip over the msg header */
   pBuf += sizeof( STUN_MSG_HDR );
   TRAP ( pBuf > pBufEnd );

   VERIFY(stunEncodeAttrs( pMsg, &pBuf, pBufEnd ));

   /* Check if message integrity is required */
   if ( (NULL != pPwd) && (pPwd->len != 0) )
   {
      bMsgIntegrity = BOS_TRUE;
   }

   /* calculate the msg length */
   msgLen = (BOS_UINT16)(pBuf - pBufHead - sizeof( STUN_MSG_HDR ));
   if ( bMsgIntegrity )
   {
      /*
       * RFC 5389 15.4
       * Do not include attribute after message-integrity attribute during 
       * message-integrity encoding, but do include message-integrity attribute
       * itself.
       */
      msgLen += (sizeof( STUN_ATTR_HDR ) + STUN_MAX_HASH_LEN);
   }

   /* Set message header */
   ((STUN_MSG_HDR *)pBufHead)->msgType = bosSocketHtoNS( pMsg->msgHdr.msgType );
   ((STUN_MSG_HDR *)pBufHead)->len = bosSocketHtoNS( msgLen );
   memcpy(&(((STUN_MSG_HDR *)pBufHead)->id), &pMsg->msgHdr.id, sizeof( STUN_TXID ));

   /* Insert Message Integrity */
   if ( bMsgIntegrity )
   {
      VERIFY ( stunHmac( pBufHead, (BOS_UINT16)(pBuf-pBufHead), pPwd->val, (BOS_UINT16)pPwd->len, hmac ));

      TRAP ( pBuf + sizeof( STUN_ATTR_HDR) + sizeof( STUN_ATTR_OPAQUE ) > pBufEnd );

      ((STUN_ATTR_HDR *)pBuf)->attrType = bosSocketHtoNS( STUN_ATTR_MESSAGE_INTEGRITY );
      ((STUN_ATTR_HDR *)pBuf)->len = bosSocketHtoNS(STUN_MAX_HASH_LEN);
      pBuf += sizeof ( STUN_ATTR_HDR );
      memcpy( pBuf, hmac, STUN_MAX_HASH_LEN );
      pBuf += STUN_MAX_HASH_LEN;
   }

   /* Insert Fingerprint */
   if ( bFingerprint )
   {
      BOS_UINT32 crc32Val;
      BOS_ASSERT( NULL != stunCrc32CB );

      /* re-update the msg length in the STUN header to include fingerprint */
      msgLen += (sizeof( STUN_ATTR_HDR ) + sizeof(BOS_UINT32));      
      ((STUN_MSG_HDR *)pBufHead)->len = bosSocketHtoNS( msgLen );      
      
      fingerprintLen = (BOS_UINT16)(pBuf-pBufHead);

      crc32Val = (*stunCrc32CB)( pBufHead, fingerprintLen );
      crc32Val ^= FINGERPRINT_XOR_VAL;

      TRAP ( pBuf + sizeof( STUN_ATTR_HDR) + sizeof( BOS_UINT32 ) > pBufEnd );

      ((STUN_ATTR_HDR *)pBuf)->attrType = bosSocketHtoNS( STUN_ATTR_FINGERPRINT );
      ((STUN_ATTR_HDR *)pBuf)->len = bosSocketHtoNS(sizeof( BOS_UINT32 ));
      
      pBuf += sizeof ( STUN_ATTR_HDR );
      *((BOS_UINT32 *)pBuf) = bosSocketHtoNL(crc32Val);
      pBuf += sizeof( BOS_UINT32 );
   }

   *pBufLen = (BOS_UINT16)(pBuf - pBufHead);
   return ( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunmsgClass
*
*  PURPOSE:    return the type of STUN message
*
*  PARAMETERS:
*              msgType - message type
*
*  RETURNS:    BOS_TRUE, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
STUN_MSGCLASS stunmsgClass( const BOS_UINT16 msgType )
{
   BOS_UINT16 type = msgType & 0x0110;
   STUN_MSGCLASS class = 0;
   switch( type )
   {
      case 0x0000:
      {
         class = STUN_REQUEST;
      }
      break;

      case 0x0010:
      {
         class = STUN_INDICATION;
      }
      break;

      case 0x0100:
      {
         class = STUN_SUCCESS_RESPONSE;
      }
      break;

      case 0x0110:
      {
         class = STUN_ERROR_RESPONSE;
      }
      break;

      default:
      {
         /* The impossible case */
         BOS_ASSERT( BOS_FALSE );
      }
   }
   return( class );
}

/*****************************************************************************
*  FUNCTION:   stunmsgType
*
*  PURPOSE:    Return BOS_TRUE if it's a STUN message of Binding Usage or
*              Short Term Password Usage.
*
*  PARAMETERS:
*              msgType - message type
*
*  RETURNS:    BOS_TRUE, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunmsgType( BOS_UINT16 msgType )
{
   switch( msgType )
   {
      case STUN_MSG_BINDING_REQUEST:
      case STUN_MSG_SHARED_SECRET_REQUEST:
      case STUN_MSG_BINDING_RESPONSE:
      case STUN_MSG_BINDING_ERROR_RESPONSE:
      case STUN_MSG_SHARED_SECRET_RESPONSE:
      case STUN_MSG_SHARED_SECRET_ERROR_RESPONSE:
      {
         return( BOS_TRUE );
      }
      default:
      {
         return( BOS_FALSE );
      }
   }
}


/*****************************************************************************
*  FUNCTION:   stunEncodeAttrs
*
*  PURPOSE:    Encode stun Attributes
*
*  PARAMETERS:
*          pMsg   [IN]  - stun message
*          pBuf   [OUT] - buffer to contain the data
*
*  RETURNS:    BOS_TRUE of encoding is successful. BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunEncodeAttrs( STUN_MSG *pMsg, BOS_UINT8 **ppBuf, BOS_UINT8 *pBufEnd )
{
   BOS_UINT16 attrLen = 0;
   BOS_UINT16 adjustedLen = 0;
   BOS_UINT32 mask = 1;
   STUN_ATTR_HDR *pAttrHdr;
   BOS_UINT16 attrType;
   ATTR_ENCODE_FUNC encodeFunc;
   void *pStructData;

   BOS_UINT32 attrBitMap = pMsg->attrBitMap;

   while ( attrBitMap != 0 )
   {
      if ( !(attrBitMap & mask ) )
      {
         mask <<= 1;
          continue;
      }

      /* get attribute type */
      stunAttrBitToAttr( stunAttrMapList, (attrBitMap & mask), &attrType );

      /* remember location of the attribute header */
      pAttrHdr = (STUN_ATTR_HDR *)*ppBuf;
      /* skip over to the beginning of body of the attribute */
      *ppBuf += sizeof( STUN_ATTR_HDR );

      TRAP ( *ppBuf > pBufEnd );

      encodeFunc = getAttrEncodeFunc( attrType );
      pStructData = getAttrStructDataPtr( pMsg, attrType );
      
      (*encodeFunc)( *ppBuf, pBufEnd, &attrLen, pStructData );

      /* encode the attribute header */
      pAttrHdr->attrType = bosSocketHtoNS( attrType );
      pAttrHdr->len = bosSocketHtoNS( attrLen );
      
      adjustedLen = adjustedAttrLen( attrLen );
      *ppBuf += adjustedLen;

      /* clear bitmap */
      attrBitMap &= ~mask;
      mask <<= 1;
   }

   return ( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunParseAttrs
*
*  PURPOSE:    Parse STUN attributes
*
*  PARAMETERS:
*          pBuf   [IN]        - buffer containing the stun message skipped over msg header
*          bufLen [IN]        - size of buffer
*          pMsg   [OUT]       - parsed stun message
*          pHmacMsgLen   [OUT] - length of the message to be considered for
*                               hmac integrity check
*
*  RETURNS:    BOS_TRUE if aligned, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseAttrs( const BOS_UINT8 *pBuf, const BOS_UINT16 bufLen, STUN_MSG *pMsg, BOS_UINT8 *pHash, BOS_UINT16 *pHmacAttrLen )
{
   BOS_UINT8 *ptr = (BOS_UINT8 *)pBuf;
   BOS_UINT16 attrType;
   BOS_UINT16 attrLen;
   BOS_UINT16 adjustedLen;
   BOS_BOOL bParseResult;
   ATTR_PARSE_FUNC parseFunc;
   void *pStructData;

   while ( (ptr + sizeof( STUN_ATTR_HDR )) < ((BOS_UINT8 *)pBuf + bufLen) )
   {
      bParseResult = BOS_FALSE;
      attrType = bosSocketNtoHS( *((BOS_UINT16 *)ptr) );
      if ( STUN_ATTR_MESSAGE_INTEGRITY == attrType )
      {
         *pHmacAttrLen = (BOS_UINT16)(ptr - pBuf);
         VERIFY( (STUN_MAX_HASH_LEN == bosSocketNtoHS(*((BOS_UINT16*)(ptr + sizeof(BOS_UINT16)) ))));
         memcpy( pHash, ptr + sizeof(STUN_ATTR_HDR), STUN_MAX_HASH_LEN );
      }
      ptr += sizeof(attrType);
      attrLen = bosSocketNtoHS( *((BOS_UINT16 *)ptr) );

      ptr += sizeof(attrLen);
      /* check if attribute length is not exceeding message len */
      adjustedLen = adjustedAttrLen( attrLen );
      TRAP ( (ptr + adjustedLen) > ((BOS_UINT8 *)pBuf + bufLen));

      parseFunc = getAttrParseFunc( attrType );

      if ( NULL != parseFunc )
      {
         pStructData = getAttrStructDataPtr( pMsg, attrType );
         bParseResult = (*parseFunc)( ptr, attrLen, pStructData );
      }
      else
      {
         /*
          * For unsupported attributes 
          * ignore the response
          */
         LOG(("stunParseAttrs: ignore unsupported attribute [%x]", attrType));
         TRAP ( attrType <= 0x7fff );
      }

      ptr += adjustedLen;
      if ( bParseResult == BOS_FALSE )
      {
         char logBuffer[256];
         char *tmpPtr = logBuffer;
         int i;
         int nWriten;
         LOG(("stunParseAttrs: attribute [%x] len [%d] is INVALID, attribute is ignored, parsing continues.", attrType, attrLen )); 
         nWriten = sprintf( tmpPtr, "Attribut data:\n" );
         tmpPtr += nWriten;
         for( i = 0; i < attrLen; i++ );
         {
            nWriten = sprintf ( tmpPtr, "0x%x ", (int)(ptr[i]) );
            tmpPtr += nWriten;
         }
         nWriten = sprintf( tmpPtr, "\n");
         LOG(( logBuffer )); 
      }
      else
      {
         /* attribute parsed, set the corresponding attribute bit */
         stunSetAttrBitMap( attrType, &pMsg->attrBitMap );
      }
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunSetAttrBitMap
*
*  PURPOSE:    set attribute bitmap
*
*  PARAMETERS:
*          attrtype   [IN]  - STUN attribute type
*          attrBitMap [MOD]  - bitmap
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void stunSetAttrBitMap( BOS_UINT16 attrType, BOS_UINT32 *pAttrBitMap )
{
   int i;
   for ( i = 0; stunAttrMapList[i].stunAttr != (BOS_UINT16)-1; i++ )
   {
      if ( stunAttrMapList[i].stunAttr == attrType )
      {
         *pAttrBitMap |= stunAttrMapList[i].attrBit;
         return;
      }
   }

   *pAttrBitMap |= ATTR_BIT_UNSUPPORTED;
}

/*****************************************************************************
*  FUNCTION:   stunAttrBitToAttr
*
*  PURPOSE:    look up Stun Attr from attr bit
*
*  PARAMETERS:
*          attrBit [IN]  - bitmap
*          attrtype[OUT]  - STUN attribute type
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
static void stunAttrBitToAttr( const STUN_ATTRMAP *pMapList, BOS_UINT32 attrBit, BOS_UINT16 *pAttrType )
{
   int i;
   for ( i = 0; pMapList[i].stunAttr != (BOS_UINT16)-1; i++ )
   {
      if ( pMapList[i].attrBit == attrBit )
      {
         *pAttrType = pMapList[i].stunAttr;
         return;
      }
   }
   *pAttrType = (BOS_UINT16)-1;
}

/*****************************************************************************
*  FUNCTION:   stunmsgAttrPresent
*
*  PURPOSE:    Return BOS_TRUE if attr is present in the message.
*
*  PARAMETERS:
*          attrBit [IN]  - bitmap
*          attrtype[OUT]  - STUN attribute type
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/

static BOS_BOOL stunmsgAttrPresent( const STUN_MSG *pMsg, BOS_UINT16 attrType )
{
   int i;
   BOS_BOOL rc = BOS_FALSE;

   for ( i = 0; stunAttrMapList[i].stunAttr != (BOS_UINT16)-1; i++ )
   {
      if ( stunAttrMapList[i].stunAttr == attrType )
      {
         if (pMsg->attrBitMap & stunAttrMapList[i].attrBit)
         {
            rc = BOS_TRUE;
         }
         break;
      }
   }
 
   return( rc );
}

/*****************************************************************************
*  FUNCTION:   stunParseAttrAddr
*
*  PURPOSE:    parse Address type attribute
*
*  PARAMETERS:
*          *ptr    [IN]  - pointer to the body of the attribute
*          attrLen [IN]  - length of attribute
*          pAddr   [IN]  - pointer to parsed data buffer
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseAttrAddr( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData )
{
   STUN_ATTR_ADDR *pAddr = (STUN_ATTR_ADDR *)pData;
   BOS_UINT8 *tmp = (BOS_UINT8 *)ptr;
   pAddr->pad = *((BOS_UINT8 *)tmp++);
   pAddr->family = *((BOS_UINT8 *)tmp++);
   if ( STUN_IPV4_FAMILY == pAddr->family )
   {
      TRAP ( attrLen != STUN_ATTR_ADDRV4_SIZE );  /* pad + family + 32bit addr */
      pAddr->ipAddr.v4.port = bosSocketNtoHS( *((BOS_UINT16 *)tmp) );
      tmp += sizeof(BOS_UINT16);
      pAddr->ipAddr.v4.addr = bosSocketNtoHL( *((BOS_UINT32 *)tmp) );
      tmp += sizeof(BOS_UINT32);
   }
#if BOS_CFG_IP_ADDR_V6_SUPPORT   
   else if ( STUN_IPV6_FAMILY == pAddr->family )
   {
      TRAP ( attrLen != STUN_ATTR_ADDRV6_SIZE );  /* pad + family + 128bit addr */
      pAddr->ipAddr.v6.port = bosSocketNtoHS( *((BOS_UINT16 *)tmp) );
      tmp += sizeof(BOS_UINT16);
      memcpy( pAddr->ipAddr.v6.d8, tmp, sizeof( pAddr->ipAddr.v6.d8 ) );
      tmp += sizeof( pAddr->ipAddr.v6.d8 );
   }
#endif
   else
   {
      return ( BOS_FALSE );
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunParseAttrErrCode
*
*  PURPOSE:    parse Error Code attribute
*
*  PARAMETERSparsing
*          *ptr    [IN]  - pointer to the body of the attribute,
*          attrLen [IN]  - length of attribute
*          pAddr   [IN]  - pointer to parsed data buffer
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseAttrErrCode( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData )
{
   BOS_UINT8 *tmp = (BOS_UINT8 *)ptr;
   STUN_ATTR_ERRCODE *pErrCode = (STUN_ATTR_ERRCODE *)pData;
   pErrCode->pad = bosSocketNtoHS( *((BOS_UINT16 *)tmp) );
   tmp += sizeof(BOS_UINT16);
   pErrCode->class = *(tmp++);
   pErrCode->number = *(tmp++);
   TRAP ( attrLen - 4 > STUN_MAX_DATA_LEN );
   memcpy( pErrCode->phrase, tmp, attrLen - 4 );
   tmp += pErrCode->iLen = (BOS_UINT8)(attrLen - 4);
   /* insert NULL to terminate the string */
   if ( pErrCode->iLen == STUN_MAX_DATA_LEN )
   {
      /* truncate */
      pErrCode->phrase[pErrCode->iLen-1] = '\0';
   }
   else
   {
      pErrCode->phrase[pErrCode->iLen] = '\0';
   }
   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunParseAttrUnknown
*
*  PURPOSE:    parse Unknown Attributes attribute
*
*  PARAMETERS:
*          ptr     [IN]  - pointer to the body of the attribute,
*          attrLen [IN]  - length of attribute
*          pUnknown[OUT] - pointer to parsed data buffer
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseAttrUnknown( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData )
{
   BOS_UINT8 *tmp = (BOS_UINT8 *)ptr;
   STUN_ATTR_UNKNOWN_ATTRS *pUnknown = (STUN_ATTR_UNKNOWN_ATTRS *)pData;
   BOS_UINT8 *pEnd = (BOS_UINT8 *)(tmp + attrLen);
   int i = 0;
   while( tmp < (BOS_UINT8 *)pEnd && i < STUN_MAX_ATTR_LIST_SIZE )
   {
      pUnknown->attr[i++] = bosSocketNtoHS(*((BOS_UINT16 *)tmp));
      tmp += sizeof(BOS_UINT16);
   }
   pUnknown->iNum = (BOS_UINT8)i;

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunParseAttrOpq
*
*  PURPOSE:    parse opaque type attribute
*
*  PARAMETERS:
*          ptr     [IN]  - pointer to the body of the attribute
*          attrLen [IN]  - length of attribute
*          pOpq    [OUT]  - pointer to parsed data buffer
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseAttrOpq( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData )
{
   BOS_UINT8 *tmp = (BOS_UINT8 *)ptr;
   STUN_ATTR_OPAQUE *pOpq = (STUN_ATTR_OPAQUE *)pData;
   TRAP ( attrLen > STUN_MAX_DATA_LEN );

   memcpy( pOpq->val, tmp, attrLen );
   pOpq->iLen = attrLen;

   tmp += attrLen;

   return( BOS_TRUE );
}

#if NTCFG_TURN_SUPPORT
/*****************************************************************************
*  FUNCTION:   stunParseAttrData
*
*  PURPOSE:    parse Generic Data type attrbiute
*
*  PARAMETERS:
*          ptr       [IN]  - pointer to the body of the attribute
*          attrLen   [IN]  - length of attribute
*          pGenData  [OUT]  - pointer to generic data
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseAttrData( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData )
{
   BOS_UINT8 *tmp = (BOS_UINT8 *)ptr;
   STUN_ATTR_GENDATA *pGenData = (STUN_ATTR_GENDATA *)pData;

   pGenData->pBuf = (BOS_UINT8 *)ptr;
   pGenData->len = attrLen;

   tmp += attrLen;

   return( BOS_TRUE );
}
#endif /* NTCFG_TURN_SUPPORT */


/*****************************************************************************
*  FUNCTION:   stunParseAttrUINT32
*
*  PURPOSE:    parse UINT32 values.
*              This is used in parsing:
*              - LIFETIME
*              - BANDWIDTH
*              - REQUESTED-PORT-PROPRS
*              - REQUESTED-TRANSPORT
*              - TIMER-VAL
*              - PRIORITY
*
*  PARAMETERS:
*          ptr     [IN]  - pointer to the body of the attribute
*          attrLen [IN]  - length of attribute
*          pData    [OUT] - pointer to parsed data buffer
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseAttrUINT32( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData )
{
   TRAP (attrLen != 4 );
   *(BOS_UINT32 *)pData = bosSocketNtoHL(*(BOS_UINT32 *)ptr);
   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunParseAttrFlag
*
*  PURPOSE:    parse attribute with 0 length.
*              This is used in parsing:
*              - USE-CANDIDATE
*
*  PARAMETERS:
*          ptr     [IN]  - pointer to the body of the attribute
*          attrLen [IN]  - length of attribute
*          pBuf    [OUT] - pointer to parsed data buffer
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseAttrFlag( const BOS_UINT8 *ptr, const BOS_UINT16 attrLen, void *pData )
{
   BOS_BOOL *pBuf = (BOS_BOOL *)pData;
   (void)ptr;
   TRAP (attrLen != 0 );

   *pBuf = BOS_TRUE;

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunEncodeAttrAddr
*
*  PURPOSE:    encode Address type attribute
*
*  PARAMETERS:
*          *ptr    [MOD]  - pointer to message buffer
*          *pBufEnd[IN]  - point to the end of the buffer
*          pAttrLen [MOD] - length of attribute
*          pAddr   [IN]  - pointer to internal STUN message
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunEncodeAttrAddr( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData )
{
   STUN_ATTR_ADDR *pAddr = (STUN_ATTR_ADDR *)pData;
   BOS_ASSERT( (NULL != ptr) && (NULL != pAttrLen) && (NULL != pAddr) );

   /* sanity check for buffer size. Use worst case */
   TRAP ( ptr + sizeof( STUN_ATTR_ADDR ) > pBufEnd );

   *ptr++ = pAddr->pad;
   *ptr++ = pAddr->family;
   if ( pAddr->family == STUN_IPV4_FAMILY )
   {
      *(BOS_UINT16 *)ptr = bosSocketHtoNS(pAddr->ipAddr.v4.port);
      ptr += sizeof(BOS_UINT16);
      *(BOS_UINT32 *)ptr = bosSocketHtoNL(pAddr->ipAddr.v4.addr);
      ptr += sizeof(BOS_UINT32);
      *pAttrLen = STUN_ATTR_ADDRV4_SIZE;
   }
#if BOS_CFG_IP_ADDR_V6_SUPPORT    
   else if ( pAddr->family == STUN_IPV6_FAMILY )
   {
      *(BOS_UINT16 *)ptr = bosSocketHtoNS(pAddr->ipAddr.v6.port);
      ptr += sizeof(BOS_UINT16);
      memcpy( ptr, pAddr->ipAddr.v6.d8, sizeof( pAddr->ipAddr.v6.d8 ));
      *pAttrLen = STUN_ATTR_ADDRV6_SIZE;
   }
#endif
   else
   {
      BOS_ASSERT( BOS_FALSE );
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunEncodeAttrErrCode
*
*  PURPOSE:    encode Error Code attribute
*
*  PARAMETERS:
*          *ptr    [MOD]  - pointer to message buffer
*          *pBufEnd[IN]  - point to the end of the buffer
*          pAttrLen [MOD] - length of attribute
*          pErrCode   [IN]  - pointer to internal STUN message
*
*  RETURNS:    BOS_TRUE if encoding is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunEncodeAttrErrCode( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData )
{
   STUN_ATTR_ERRCODE *pErrCode = (STUN_ATTR_ERRCODE *)pData;
   (void)pBufEnd;
   ((STUN_ATTR_ERRCODE *)ptr)->pad = bosSocketHtoNS(pErrCode->pad);
   ((STUN_ATTR_ERRCODE *)ptr)->class = pErrCode->class;
   ((STUN_ATTR_ERRCODE *)ptr)->number = pErrCode->number;
   memcpy( ((STUN_ATTR_ERRCODE *)ptr)->phrase, pErrCode->phrase, pErrCode->iLen );
   *pAttrLen = 4 + pErrCode->iLen;

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunEncodeAttrUnknown
*
*  PURPOSE:    encode Unknown attribute
*
*  PARAMETERS:
*          *ptr    [MOD]  - pointer to message buffer
*          *pBufEnd[IN]  - point to the end of the buffer
*          pAttrLen [MOD] - length of attribute
*          unknownp   [IN]  - pointer to internal STUN message
*
*  RETURNS:    BOS_TRUE if encoding is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunEncodeAttrUnknown( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData )
{
   STUN_ATTR_UNKNOWN_ATTRS *pUnknown = (STUN_ATTR_UNKNOWN_ATTRS *)pData;
   int i;
   BOS_UINT16 *tmp = (BOS_UINT16 *)ptr;
   BOS_ASSERT( pUnknown->iNum > 1 && (pUnknown->iNum % 2 == 0)); /* should be even number of attributes */
   (void)pBufEnd;
   for ( i = 0; i < pUnknown->iNum; i++ )
   {
      *tmp++ = pUnknown->attr[i];
   }

   *pAttrLen = pUnknown->iNum * sizeof( BOS_UINT16 );

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:   stunEncodeAttrOpq
*
*  PURPOSE:    encode Opaque type attribute
*
*  PARAMETERS:
*          *ptr    [MOD]  - pointer to message buffer
*          *pBufEnd[IN]  - point to the end of the buffer
*          pAttrLen [MOD] - length of attribute
*          pAddr   [IN]  - pointer to internal STUN message
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunEncodeAttrOpq( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData )
{
   STUN_ATTR_OPAQUE *pOpq = (STUN_ATTR_OPAQUE *)pData;
   BOS_ASSERT( (NULL != ptr) && (NULL != pAttrLen) && (NULL != pOpq) );

   /* sanity check for buffer size. Use worst case */
   TRAP ( ptr + sizeof( STUN_ATTR_OPAQUE ) > pBufEnd );

   memcpy( ptr, pOpq->val, pOpq->iLen );

   *pAttrLen = pOpq->iLen;

   return( BOS_TRUE );
}
#if NTCFG_TURN_SUPPORT
/*****************************************************************************
*  FUNCTION:   stunEncodeAttrData
*
*  PURPOSE:    encode Generic Data type attribute
*
*  PARAMETERS:
*          *ptr    [MOD]  - pointer to message buffer
*          *pBufEnd[IN]  - point to the end of the buffer
*          pAttrLen [MOD] - length of attribute
*          pAddr   [IN]  - pointer to internal STUN message
*
*  RETURNS:    BOS_TRUE if parsing is OK, BOS_FALSE otherwise
*
*  NOTES:   The STUN_DATA attribute is the last one to be encoded and no
*           padding or adjustment to attribute length is performed.
*
*****************************************************************************/
static BOS_BOOL stunEncodeAttrData( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData )
{
   STUN_ATTR_GENDATA *pGenData = (STUN_ATTR_GENDATA *)pData;
   BOS_ASSERT( (NULL != ptr) && (NULL != pAttrLen) && (NULL != pData) &&
               (NULL != pGenData) && (NULL != pGenData->pBuf));

   /* sanity check for buffer size. Use worst case */
   TRAP ( (ptr + pGenData->len) > pBufEnd );

   memcpy( ptr, pGenData->pBuf, pGenData->len );

   *pAttrLen = pGenData->len;

   return( BOS_TRUE );
}
#endif

/*****************************************************************************
*  FUNCTION:   stunEncodeAttrUINT32
*
*  PURPOSE:    encode UINT32 type attribute
*
*  PARAMETERS:
*          *ptr    [MOD]  - pointer to message buffer
*          *pBufEnd[IN]  - point to the end of the buffer
*          pAttrLen [MOD] - length of attribute
*          pData    [IN]  - pointer to internal STUN message
*
*  RETURNS:    BOS_TRUE if encoding is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunEncodeAttrUINT32( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData )
{
   BOS_ASSERT( (NULL != ptr) && (NULL != pAttrLen) && (NULL != pData) );

   TRAP( ptr + sizeof( BOS_UINT32 ) > pBufEnd );

   *((BOS_UINT32 *)ptr) = bosSocketHtoNL(*(BOS_UINT32*)pData);

   *pAttrLen = sizeof(BOS_UINT32);

   return (BOS_TRUE);
}
/*****************************************************************************
*  FUNCTION:   stunEncodeAttrFlag
*
*  PURPOSE:    encode flag type attribute
*
*  PARAMETERS:
*          *ptr    [MOD]  - pointer to message buffer
*          *pBufEnd[IN]  - point to the end of the buffer
*          pAttrLen [MOD] - length of attribute
*          pData    [IN]  - pointer to internal STUN message
*
*  RETURNS:    BOS_TRUE if encoding is OK, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunEncodeAttrFlag( BOS_UINT8 *ptr, const BOS_UINT8 *pBufEnd, BOS_UINT16 *pAttrLen, const void *pData )
{
   BOS_ASSERT( (NULL != ptr) && (NULL != pAttrLen) );
   (void)pData;

   TRAP( ptr > pBufEnd );

   *pAttrLen = 0;

   return (BOS_TRUE);
}

/*****************************************************************************
*  FUNCTION:
*     stunHmac
*
*  PURPOSE:
*     Get Hmac for the specified data and key. USed in message integratity check. 
*  PARAMETERS:
*
*  RETURNS:
*     BOS_TRUE if hamc is generated.  BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunHmac( const BOS_UINT8 *pData, const BOS_UINT16 dataLen, const BOS_UINT8 *pKey, const BOS_UINT16 keyLen, BOS_UINT8 *pHmacData )
{
   BOS_ASSERT( NULL != pData );

   if ( (NULL != stunHmacCB) && (keyLen > 0))
   {
      STUN_DATA sData;
      STUN_DATA sKey;
      STUN_DATA  sHmac;
      sHmac.pData = pHmacData;
      sHmac.len = 0;

      sData.pData = (BOS_UINT8 *)pData;
      sData.len = (BOS_UINT16)dataLen;
      sKey.pData = (BOS_UINT8 *)pKey;
      sKey.len = (BOS_UINT16)keyLen;

      (*stunHmacCB)( STUN_HMAC_SHA1,
                     &sKey,
                     &sData,
                     &sHmac );

      if ( STUN_MAX_HASH_LEN != sHmac.len  )
      {
         return( BOS_FALSE );
      }
   }
   else
   {
      return( BOS_FALSE );
   }

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:
*     stunParse
*
*  PURPOSE:
*     Parse stun mssage and to extract Message Integrity hash and 
*     Fingerprint, if present. Length for message test is also returned.
*
*  PARAMETERS:
*     pData - [IN] Input STUN message
*     len -   [IN] Input message length
*     pHash - [OUT] hash value
*     pHmacMsgLen - [OUT] message length used in hashing
*     pFingerPrint - [OUT] finger print value
*
*  RETURNS:
*     BOS_TRUE if parsing is successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunmsgParse( const BOS_UINT8       *pData, 
                       const BOS_UINT16      len, 
                       BOS_UINT8             *pHash, 
                       BOS_UINT16            *pHmacMsgLen,
                       STUN_MSG              *pMsg )
{
   BOS_UINT8 *ptr = (BOS_UINT8 *)pData;            /* temp pointer */
   BOS_UINT16 hmacAttrLen = 0;

   BOS_ASSERT( (NULL != pData) && 
               (NULL != pHash) && 
               (NULL != pHmacMsgLen) );

   *pHmacMsgLen = 0;

   TRAP ( len < sizeof( STUN_MSG_HDR ) );

   if ( BOS_TRUE != stunParseMsgHdr( pData, len, pMsg ) )
   {
      return( BOS_FALSE );
   }

   /* Skip over message header */
   ptr += sizeof( STUN_MSG_HDR );

   if ( BOS_TRUE != stunParseAttrs( ptr, 
                                    (len - sizeof(STUN_MSG_HDR)),
                                    pMsg,
                                    pHash,
                                    &hmacAttrLen ))
   {
      return( BOS_FALSE );
   }

   *pHmacMsgLen = hmacAttrLen + sizeof( STUN_MSG_HDR );

   return( BOS_TRUE );
}

/*****************************************************************************
*  FUNCTION:
*     stunIsFingerPrinted
*
*  PURPOSE:
*     FingerPrint check of the STUN message.
*
*     Return BOS_TRUE only if 
*     1. fingerprint attribute present at the very end of the pData, and
*     2. the fingerprint value matches the computed CRC32 of the data,
*        excluding the FINGERPRINT attribute, XOR'd with 0x5354554e.
*
*  PARAMETERS:
*     pData - [IN] Input STUN message
*     len -   [IN] Input message length
*
*  RETURNS:
*     BOS_TRUE if pass is successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL stunmsgIsFingerprinted( const BOS_UINT8     *pData, 
                                 const BOS_UINT16    len )
{
   BOS_UINT8 *ptr;
   BOS_UINT8 *pEnd = (BOS_UINT8 *)(pData + len);   /* End of input data */
   BOS_UINT32 fingerprint = 0;
   BOS_UINT16 fingerprintLen = 0;
   BOS_UINT32 crc32Val = 0;

   BOS_ASSERT( (NULL != pData) );

   TRAP ( len < (sizeof( STUN_MSG_HDR ) + FINGERPRINT_ATTR_SIZE) );

   if ( NULL == stunCrc32CB )
   {
      LOG(("stunmsgIsFingerprinted: Error, CRC32 CB not defined"));
      return( BOS_FALSE );
   }

   fingerprintLen = len - FINGERPRINT_ATTR_SIZE;
   ptr = pEnd - FINGERPRINT_ATTR_SIZE;
   if ( STUN_ATTR_FINGERPRINT == bosSocketNtoHS(((STUN_ATTR_HDR *)ptr)->attrType ))
   {
      ptr += sizeof(STUN_ATTR_HDR); 
      fingerprint = bosSocketNtoHL( *(BOS_UINT32 *)ptr);
      crc32Val = (*stunCrc32CB)( pData, fingerprintLen );

      if ( (crc32Val ^ FINGERPRINT_XOR_VAL) == fingerprint )
      {
         return( BOS_TRUE );
      }
   }

   return( BOS_FALSE );
}

/*****************************************************************************
*  FUNCTION:   stunParseMsgHdr
*
*  PURPOSE:    Parse the stun message header in <buf> as received
*              from the network.
*
*  PARAMETERS:
*          pBuf   [IN]        - buffer containing the stun message
*          bufLen [IN]        - size of buffer
*          pMsg   [OUT]       - parsed stun message
*
*  RETURNS:    BOS_TRUE if no parsing errors. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
static BOS_BOOL stunParseMsgHdr( const BOS_UINT8 *pBuf, const BOS_UINT16 bufLen, STUN_MSG *pMsg )
{
   BOS_UINT8 *ptr = (BOS_UINT8 *)pBuf;
   BOS_ASSERT( (NULL != pBuf) && (NULL != pMsg) );
 
   TRAP ( bufLen < sizeof( STUN_MSG_HDR ) );

   /* message header */
   pMsg->msgHdr.msgType = bosSocketNtoHS( *(BOS_UINT16 *)ptr );
   ptr += sizeof( BOS_UINT16 );

   /* message len */
   pMsg->msgHdr.len = bosSocketNtoHS( *(BOS_UINT16 *)ptr );
   ptr += sizeof( BOS_UINT16 );

   /* transaction id */
   memcpy(&pMsg->msgHdr.id, (STUN_TXID*)ptr, sizeof( STUN_TXID ));
   ptr += sizeof( STUN_TXID );

   return( BOS_TRUE );

}

/*****************************************************************************
*  FUNCTION:
*     adjustedAttrLen
*
*  PURPOSE:
*       Adjust for 4 byte alignment per newer bis draft:
*
*           Since STUN aligns attributes on 32 bit boundaries, 
*           attributes whose content is not a multiple of 4 bytes 
*           are padded with 1, 2 or 3 bytes of padding so 
*           that they are a multiple of 4 bytes.  
*
*  PARAMETERS:
*     attrLen [MOD] - attribute length to be adjusted for padding
*
*  RETURNS:
*     The adjusted length.
*
*  NOTES:
*
*****************************************************************************/
static BOS_UINT16 adjustedAttrLen( const BOS_UINT16 attrLen )
{
   BOS_UINT16 r;

   r = (attrLen % 4);
   if ( r )
   {
      r = 4 - r;
   }

   return( (attrLen + r) );
}

/*****************************************************************************
*  FUNCTION:
*     stunmsgErrStr
*
*  PURPOSE:
*     Return a defined error string for the given error number.
*
*  PARAMETERS:
*     errnoa [IN] - error number
*
*  RETURNS:
*     The error string
*
*  NOTES:
*
*****************************************************************************/
char *stunmsgReasonStr( BOS_UINT16 errnoa )
{
   int i;

   for ( i = 0; (BOS_UINT16)(-1) != reasonStrMap[i].number; i++ )
   {
      if ( reasonStrMap[i].number == errnoa )
      {
         break;
      }
   }

   return( reasonStrMap[i].pPhrase );
}
