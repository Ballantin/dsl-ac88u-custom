/****************************************************************************
*
* Copyright © 2006-2008 Broadcom
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
*  @file    stunmsg.h
*
*  @brief   STUN Message data structures and defines
*/
/****************************************************************************/
#ifndef STUNMSG_H    /* support nested includes */
#define STUNMSG_H

#include <bosTypes.h>
#include "./inc/stun.h"
/* ---- Include Files ---------------------------------------- */
/* ---- Constants and Types ---------------------------------- */
/*
 * NOTE: The following structures follow closely to the 
 *       data format of the STUN messages and attributes as 
 *       defined in RFC 3489.
 *
 *       For internal use data elements, they are marked with a 
 *       prefix 'i'
 *       
 *       Data held in the structures are in host-byte-ordering 
 *       fashion. Except for STUN_TXID, since data is not processed,
 *       it's stored in network-byte-ordering fashion.
 */

/* transaction id */
typedef STUN_TRANSACTION_ID STUN_TXID;

/* STUN message header */
typedef struct
{
   BOS_UINT16  msgType;
   BOS_UINT16  len;
   STUN_TXID   id;
} STUN_MSG_HDR;

/* STUN attribute header */
typedef struct
{
   BOS_UINT16  attrType;
   BOS_UINT16  len;
} STUN_ATTR_HDR;

/* IPv4 address */
typedef struct
{
   BOS_UINT16  port;
   BOS_UINT32  addr;
} STUN_IPV4ADDR;

#define STUN_IP_ADDR_LEN_BYTES_V6   16
/* IPv6 address */
typedef struct
{
   BOS_UINT16  port;
   BOS_UINT8  d8[STUN_IP_ADDR_LEN_BYTES_V6];
} STUN_IPV6ADDR;

/* Address attribute, generic, used in the following attributes
 *    MAPPED-ADDRESS,  
 *    RESPONSE-ADDRESS,  
 *    CHANGED-ADDRESS,  
 *    SOURCE-ADDRESS,  
 *    REFLECTED-FROM,  
 *    XOR-MAPPED-ADDRESS  
 */
#define STUN_IPV4_FAMILY   (BOS_UINT8)0x01
#define STUN_IPV6_FAMILY   (BOS_UINT8)0x02
#define STUN_ATTR_ADDRV4_SIZE 8
#define STUN_ATTR_ADDRV6_SIZE 20
typedef struct
{
   BOS_UINT8   pad;
   BOS_UINT8   family;
   union
   {
      STUN_IPV4ADDR v4;
      STUN_IPV6ADDR v6;
   } ipAddr;
} STUN_ATTR_ADDR;

/* CHANGE-REQUEST attribute */
#define CHGREQ_CHANGE_IP_FLAG    (BOS_UINT32)0x00000004
#define CHGREQ_CHANGE_PORT_FLAG  (BOS_UINT32)0x00000002
typedef struct
{
   BOS_UINT32  val;
} STUN_ATTR_UINT32;

/* variable length opaque attribute, generic, 
 * used in the following attributes
 *    USERNAME,  
 *    PASSWORD,  
 *    SERVER
 */
typedef struct
{
   BOS_UINT8   val[STUN_MAX_DATA_LEN]; /* MUST be 4-byte aligned */
   BOS_UINT16  iLen;                /* actual length */
} STUN_ATTR_OPAQUE;

/* MESSAGE-INTEGRITY attribute */
#define STUN_MAX_HASH_LEN       20

/* ERROR-CODE attribute */
/*     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |                   0                     |Class|     Number    |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |      Reason Phrase (variable)                                ..
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    Reason Phrase MUST be 4 byte aligned.
*/
typedef struct
{
   BOS_UINT16  pad;
   BOS_UINT8   class; 
   BOS_UINT8   number;
   BOS_UINT8   phrase[STUN_MAX_DATA_LEN]; /* 4-byte aligned */
   BOS_UINT8   iLen;                   /* actual length */
} STUN_ATTR_ERRCODE;

/* UNKNOWN-ATTRIBUTES attribute */
#define STUN_MAX_ATTR_LIST_SIZE 16
typedef struct
{
   BOS_UINT16  attr[STUN_MAX_ATTR_LIST_SIZE];
   BOS_UINT8   iNum;          /* actual number of attributes */
} STUN_ATTR_UNKNOWN_ATTRS;

#define ATTR_BIT_MAPPED_ADDR           (BOS_UINT32)(1<<0)
#define ATTR_BIT_RESP_ADDR             (BOS_UINT32)(1<<1)
#define ATTR_BIT_CHANGE_REQ            (BOS_UINT32)(1<<2)
#define ATTR_BIT_SOURCE_ADDR           (BOS_UINT32)(1<<3)
#define ATTR_BIT_CHANGED_ADDR          (BOS_UINT32)(1<<4)
#define ATTR_BIT_USERNAME              (BOS_UINT32)(1<<5)
#define ATTR_BIT_PASSWORD              (BOS_UINT32)(1<<6)
#define ATTR_BIT_MSG_INTEGRITY         (BOS_UINT32)(1<<7)
#define ATTR_BIT_ERR_CODE              (BOS_UINT32)(1<<8)
#define ATTR_BIT_UNKNOWN_ATTRS         (BOS_UINT32)(1<<9)
#define ATTR_BIT_REFLECTED_FROM        (BOS_UINT32)(1<<10)
#define ATTR_BIT_XOR_MAPPED_ADDR       (BOS_UINT32)(1<<11)
#define ATTR_BIT_XOR_ONLY              (BOS_UINT32)(1<<12)
#define ATTR_BIT_SERVER                (BOS_UINT32)(1<<13)
#define ATTR_BIT_FINGERPRINT           (BOS_UINT32)(1<<14)
#if NTCFG_ICE_SUPPORT
#define ATTR_BIT_PRIORITY              (BOS_UINT32)(1<<15)
#define ATTR_BIT_USECANDIDATE          (BOS_UINT32)(1<<16)
#endif
#if NTCFG_TURN_SUPPORT
#define ATTR_BIT_LIFETIME              (BOS_UINT32)(1<<20)
#define ATTR_BIT_BANDWIDTH             (BOS_UINT32)(1<<21)
#define ATTR_BIT_REMOTE_ADDR           (BOS_UINT32)(1<<22)
#define ATTR_BIT_RELAY_ADDR            (BOS_UINT32)(1<<23)
#define ATTR_BIT_REQUESTED_PORT_PROPS  (BOS_UINT32)(1<<24)
#define ATTR_BIT_REQUESTED_TRANSPORT   (BOS_UINT32)(1<<25)
#define ATTR_BIT_REQUESTED_IP          (BOS_UINT32)(1<<26)
#define ATTR_BIT_TIMER_VAL             (BOS_UINT32)(1<<27)
/* ATTR_BIT_DATA must be last to be encoded */
#define ATTR_BIT_DATA                  (BOS_UINT32)(1<<30)
#endif
#define ATTR_BIT_UNSUPPORTED           (BOS_UINT32)(1<<31)

/* Attribute type that contains no data */
typedef struct
{
   BOS_BOOL                bPresent;         /* Attribute Present */
} STUN_ATTR_FLAG;

typedef struct
{
   STUN_ATTR_ADDR          mappedAddr;       /* mapped address */
   STUN_ATTR_ADDR          respAddr;         /* response address */
   STUN_ATTR_UINT32        chgReq;           /* Change request */
   STUN_ATTR_ADDR          srcAddr;          /* source address */
   STUN_ATTR_ADDR          chgdAddr;         /* Changed address */
   STUN_ATTR_OPAQUE        password;         /* password */
   STUN_ATTR_ADDR          reflectedFrom;    /* reflected from */
   STUN_ATTR_FLAG          xorOnly;          /* Xor Only */
   STUN_ATTR_OPAQUE        server;           /* server */
#if NTCFG_ICE_SUPPORT
   STUN_ATTR_UINT32        priority;         /* Priority */
   STUN_ATTR_FLAG          useCandidate;     /* Use Candidate flag */
#endif
} STUN_ATTR_SET;

#if NTCFG_TURN_SUPPORT
typedef struct
{
   BOS_UINT8      *pBuf;
   BOS_UINT16     len;
} STUN_ATTR_GENDATA;
#endif

#if NTCFG_TURN_SUPPORT
typedef struct
{
   STUN_ATTR_UINT32           lifetime;      /* Lifetime */
   STUN_ATTR_UINT32           bandwidth;     /* bandwidth */
   STUN_ATTR_ADDR             remoteAddr;    /* Remote Address */
   STUN_ATTR_GENDATA          data;          /* Data */
   STUN_ATTR_ADDR             relayAddr;     /* Relay-Address */
   STUN_ATTR_UINT32           reqdPortProps;  /* Requested Port Properties */
   STUN_ATTR_UINT32           reqdTransport; /* Requested Transport */
   STUN_ATTR_ADDR             reqdIP;        /* Requested IP */
   STUN_ATTR_UINT32           timerVal;       /* time value */
} TURN_ATTR_SET;
#endif

/* stun message */
typedef struct
{
   STUN_MSG_HDR            msgHdr;           /* message header */

   BOS_UINT32              attrBitMap;       /* attribute bit map */
   /*
    * protocol specific attributes 
    */
   union
   {
      STUN_ATTR_SET           stun;          /* STUN specific attributes */
#if NTCFG_TURN_SUPPORT
      TURN_ATTR_SET           turn;          /* TURN specific attributes */
#endif
   } attr;

   /*
    * common attributes
    */
   STUN_ATTR_OPAQUE        username;         /* username */
   STUN_ATTR_ADDR          xorMappedAddr;    /* XOR mapped address */
   STUN_ATTR_OPAQUE        msgIntegrity;     /* message integrity */
   STUN_ATTR_UINT32        fingerprint;      /* Fingerprint */ 
   STUN_ATTR_ERRCODE       errCode;          /* error code */
   STUN_ATTR_UNKNOWN_ATTRS unknown;          /* unknown attributes */
} STUN_MSG;

extern void stunmsgSetType( STUN_MSG *pMsg, BOS_UINT16 msgType );
extern BOS_UINT16 stunmsgGetType( STUN_MSG *pMsg );
extern void stunmsgSetAttr( STUN_MSG *pMsg, BOS_UINT16 attr, void *pData );
extern void *stunmsgGetAttr( STUN_MSG *pMsg, BOS_UINT16 attr );
extern BOS_BOOL stunmsgEncode( STUN_MSG *pMsg, 
                               BOS_UINT8 *pBuf, 
                               BOS_UINT16 *pBufLen, 
                               const STUN_HMAC_PWD *pPwd, 
                               const BOS_BOOL bFingerprint );
extern STUN_MSGCLASS stunmsgClass( const BOS_UINT16 msgType );
extern BOS_BOOL stunmsgParse( const BOS_UINT8       *pData, 
                              const BOS_UINT16      len, 
                              BOS_UINT8             *pHash, 
                              BOS_UINT16            *pHmacMsgLen,
                              STUN_MSG              *pMsg );
extern BOS_BOOL stunmsgIsFingerprinted( const BOS_UINT8     *pData, 
                                        const BOS_UINT16    len );
extern char *stunmsgReasonStr( BOS_UINT16 errnoa );
extern BOS_BOOL stunmsgType( BOS_UINT16 msgType );

#endif
