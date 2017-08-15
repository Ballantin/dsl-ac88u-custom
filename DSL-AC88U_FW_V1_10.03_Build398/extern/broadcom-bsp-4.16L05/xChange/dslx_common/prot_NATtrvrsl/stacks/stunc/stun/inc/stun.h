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
*  @file    stun.h
*
*  @brief   STUN Module interface file.
*
*  This file contains software interface information for the STUN module.
*/
/****************************************************************************/
#ifndef STUN_H    /* support nested includes */
#define STUN_H

/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <bosSocket.h>
#include <bosIpAddr.h>
#include <ntcfg.h>
#include <string.h>
#include <stdlib.h>

/* ---- Constants and Types ---------------------------------- */
#define STUN_VERSION "BRCM STUN 2.0"

/* General maximum data len */
#define STUN_MAX_DATA_LEN              128
/* Maximum STUN message size */
#define STUN_MSG_BUFFER_SIZE           1400

/* STUN options */
#define STUN_OPT_RETRANSMIT_DISABLE    0x0001   /* Disable timeout
                                                 * retransmission per
                                                 * RFC3489
                                                 */

/* STUN bis draft Extensions */
#define STUN_OPT_FINGERPRINT           0x0020   /* Fingerprint the request
                                                 * per bis-draft
                                                 */

#if NTCFG_STUN_SYNCD_RESPONSE_SUPPORT
   #define STUN_OPT_SYNCD_RES          0x0100   /* Wait for response
                                                 * synchronously
                                                 */
#endif

#if NTCFG_STUN_MAGIC_COOKIE_SUPPORT
   #define STUN_MAGIC_COOKIE              0x2112A442
#endif

/* STUN message types. values are as defined in RFC */
#define STUN_MSG_BINDING_REQUEST                (BOS_UINT16)0x0001
#define STUN_MSG_BINDING_RESPONSE               (BOS_UINT16)0x0101
#define STUN_MSG_BINDING_ERROR_RESPONSE         (BOS_UINT16)0x0111
#define STUN_MSG_SHARED_SECRET_REQUEST          (BOS_UINT16)0x0002
#define STUN_MSG_SHARED_SECRET_RESPONSE         (BOS_UINT16)0x0102
#define STUN_MSG_SHARED_SECRET_ERROR_RESPONSE   (BOS_UINT16)0x0112

#if NTCFG_TURN_SUPPORT
   #define STUN_MSG_ALLOCATE_REQUEST               (BOS_UINT16)0x0003
   #define STUN_MSG_ALLOCATE_RESPONSE              (BOS_UINT16)0x0103
   #define STUN_MSG_ALLOCATE_ERROR_RESPONSE        (BOS_UINT16)0x0113
   #define STUN_MSG_SEND_INDICATION                (BOS_UINT16)0x0004
   #define STUN_MSG_DATA_INDICATION                (BOS_UINT16)0x0015
   #define STUN_MSG_ACTIVE_DEST_REQUEST            (BOS_UINT16)0x0006
   #define STUN_MSG_ACTIVE_DEST_RESPONSE           (BOS_UINT16)0x0106
   #define STUN_MSG_ACTIVE_DEST_ERROR_RESPONSE     (BOS_UINT16)0x0116
   #define STUN_MSG_CONNECT_REQUEST                (BOS_UINT16)0x0007
   #define STUN_MSG_CONNECT_RESPONSE               (BOS_UINT16)0x0107
   #define STUN_MSG_CONNECT_ERROR_RESPONSE         (BOS_UINT16)0x0117
   #define STUN_MSG_CONNECT_STATUS_INDICATION      (BOS_UINT16)0x0118
#endif

/* STUN attribute types, values are as defined in RFC */
/*
 * RFC3489 - Usages: Binding discovery
 *                   NAT keepalives
 *                   Short-term password
 */
#define STUN_ATTR_MAPPED_ADDRESS                (BOS_UINT16)0x0001
#define STUN_ATTR_RESPONSE_ADDRESS              (BOS_UINT16)0x0002
#define STUN_ATTR_CHANGE_REQUEST                (BOS_UINT16)0x0003
#define STUN_ATTR_SOURCE_ADDRESS                (BOS_UINT16)0x0004
#define STUN_ATTR_CHANGED_ADDRESS               (BOS_UINT16)0x0005
#define STUN_ATTR_USERNAME                      (BOS_UINT16)0x0006
#define STUN_ATTR_PASSWORD                      (BOS_UINT16)0x0007
#define STUN_ATTR_MESSAGE_INTEGRITY             (BOS_UINT16)0x0008
#define STUN_ATTR_ERRORCODE                     (BOS_UINT16)0x0009
#define STUN_ATTR_UNKNOWN_ATTRIBUTE             (BOS_UINT16)0x000a
#define STUN_ATTR_REFLECTED_FROM                (BOS_UINT16)0x000b
/* bis-extensions */
#define STUN_ATTR_REALM                         (BOS_UINT16)0x0014
#define STUN_ATTR_NONCE                         (BOS_UINT16)0x0015
#define STUN_ATTR_XORMAPPED_ADDRESS             (BOS_UINT16)0x0020
#define STUN_ATTR_XORONLY                       (BOS_UINT16)0x8021
#define STUN_ATTR_SERVERNAME                    (BOS_UINT16)0x8022
#define STUN_ATTR_ALTERNATE_SERVER              (BOS_UINT16)0x8023
#define STUN_ATTR_REFRESH_INTERVAL              (BOS_UINT16)0x8024
#define STUN_ATTR_FINGERPRINT                   (BOS_UINT16)0x8028

#if NTCFG_TURN_SUPPORT
/* draft-ietf-behave-turn - Usage: Relay */
   #define STUN_ATTR_LIFETIME                      (BOS_UINT16)0x000D
   #define STUN_ATTR_BANDWIDTH                     (BOS_UINT16)0x0010
   #define STUN_ATTR_REMOTE_ADDRESS                (BOS_UINT16)0x0012
   #define STUN_ATTR_DATA                          (BOS_UINT16)0x0013
   #define STUN_ATTR_RELAY_ADDRESS                 (BOS_UINT16)0x0016
   #define STUN_ATTR_REQUESTED_PORT_PROPS          (BOS_UINT16)0x0018
   #define STUN_ATTR_REQUESTED_TRANSPORT           (BOS_UINT16)0x0019
   #define STUN_ATTR_REQUESTED_IP                  (BOS_UINT16)0x0022
   #define STUN_ATTR_TIMER_VAL                     (BOS_UINT16)0x0021
/* Attribute values */
   #define STUN_ATTRV_PORT_NONE                    (BOS_UINT32)0x00000000
   #define STUN_ATTRV_ODDPORT                      (BOS_UINT32)0x00000001
   #define STUN_ATTRV_EVENPORT                     (BOS_UINT32)0x00000002
   #define STUN_ATTRV_EVENPORT_RES_NXT             (BOS_UINT32)0x00000003
   #define STUN_ATTRV_UDP                          (BOS_UINT32)0x00000000
   #define STUN_ATTRV_TCP                          (BOS_UINT32)0x00000001
#endif

#if NTCFG_ICE_SUPPORT
/* draft-ietf-mmusic-ice - Usage: Connectivity Check */
   #define STUN_ATTR_PRIORITY                      (BOS_UINT16)0x0024
   #define STUN_ATTR_USECANDIDATE                  (BOS_UINT16)0x0025
#endif

/* STUN Transport Address */
typedef struct
{
   BOS_IP_ADDRESS       ipAddr;
   BOS_SOCKET_IP_PORT   port;
} STUN_TRANSPORT;

/* Request sending options */
typedef BOS_UINT32 STUN_OPTS;

/* Transaction Id */
typedef struct
{
   BOS_UINT8   octet[16];
} STUN_TRANSACTION_ID;

/* STUN message class */
typedef enum
{
   STUN_REQUEST,
   STUN_INDICATION,
   STUN_SUCCESS_RESPONSE,
   STUN_ERROR_RESPONSE
}STUN_MSGCLASS;

/* HMAC algorithm */
typedef enum
{
   STUN_HMAC_SHA1 = 0
} STUN_HMAC_TYPE;

/*
 * Attribute data type
 *
 *    STUN_TRANSPORT
 *    ---------------
 *       STUN_ATTR_MAPPED_ADDRESS
 *       STUN_ATTR_RESPONSE_ADDRESS
 *       STUN_ATTR_SOURCE_ADDRESS
 *       STUN_ATTR_CHANGED_ADDRESS
 *       STUN_ATTR_REFLECTED_FROM
 *       STUN_ATTR_XORMAPPED_ADDRESS
 *
 *       STUN_ATTR_REMOTE_ADDRESS
 *       STUN_ATTR_RELAY_ADDRESS
 *       STUN_ATTR_REQUESTED_IP
 *
 *
 *    BOS_UINT32
 *    ----------
 *       STUN_ATTR_CHANGE_REQUEST
 *       STUN_ATTR_REFRESH_INTERVAL
 *       STUN_ATTR_FINGERPRINT
 *
 *       STUN_ATTR_LIFETIME
 *       STUN_ATTR_BANDWIDTH
 *       STUN_ATTR_REQUESTED_PORT_PROPS
 *       STUN_ATTR_REQUESTED_TRANSPORT
 *
 *       STUN_ATTR_PRIORITY
 *
 *    STUN_DATA
 *    ---------
 *       STUN_ATTR_USERNAME
 *       STUN_ATTR_PASSWORD
 *       STUN_ATTR_MESSAGE_INTEGRITY
 *       STUN_ATTR_REALM
 *       STUN_ATTR_NONCE
 *
 *       STUN_ATTR_DATA
 *
 *    STUN_ERR
 *    ---------
 *       STUN_ATTR_ERRORCODE
 *
 */

/* Generic STUN data */
typedef struct
{
   BOS_UINT8   *pData;
   BOS_UINT16  len;        /* Data length in number of bytes */
} STUN_DATA;

/* STUN error code and phrase */
typedef struct
{
   BOS_UINT32  errnoa;      /* Error number */
   STUN_DATA   *pPhrase;   /* Error phrase */
} STUN_ERR;

/* Response event */
typedef enum
{
   STUN_RESPONSE,          /* Remote response */
                           /* pData = STUNTX_RES */

   STUN_TIMEOUT,           /* Request timed out */
                           /* pData = NULL */

   STUN_SYSTEM_ERR,        /* Internal system error */
                           /* pData = error string */

} STUN_EVT;

/* Response data block */
typedef struct
{
   STUN_EVT             evt;           /* Response event type */
   BOS_UINT32           sid;           /* Socket Id - receiving response */
   STUN_TRANSPORT       *pLclAddr;     /* Local Socket address */
   STUN_TRANSPORT       *pSrcAddr;     /* Remote sender IP address */
   BOS_UINT32           reqSid;        /* Socket Id - sending orignal req */
   STUN_TRANSPORT       *pReqDest;     /* Orignal Request destination */
   BOS_UINT8            *pMsg;         /* Parsed STUN msg */
   BOS_UINT8            resCnt;        /* Response count */
   STUN_OPTS            options;       /* Send options used in sending
                                          the request */
   void                 *pOpaq;        /* Opaque data pointer */

} STUN_RES_DATA;

/* Request data block */
typedef struct
{
   BOS_UINT8            *pMsg;         /* Parsed STUN msg */
   BOS_UINT32           sid;           /* Socket Id - sending request */
   STUN_TRANSPORT       *pLclAddr;     /* Local Socket address */
   STUN_TRANSPORT       *pSrcAddr;     /* Remote sender IP address */
   BOS_UINT8            *pHashData;
   BOS_UINT16           hashDataLen;
   BOS_UINT8            *pHash;

} STUN_REQ_DATA;

/* Hmac password for use in Message-Integrity */
typedef struct
{
   BOS_UINT8   val[STUN_MAX_DATA_LEN];
   BOS_UINT16  len;
} STUN_HMAC_PWD;


/*
 * Defines the duration to wait for STUN responses.  The request
 * transaction terminates per the retransmission parameters or at
 * 'uMaxWaitTime', whichever expires first.  If none of the parameters
 * are defined, the defaults from RFC 3489/5389 is used.
 */
typedef struct
{
   /* Maximum wait time for STUN responses.  Specify 0 to use RFC defaults */
   BOS_TIME_MS     uMaxWaitTime;

#if NTCFG_STUN_RFC5389_RETRANS_SUPPORT
   /* Parameters for retransmission algorithm:
    *
    *    - STUN retransmit starts at 'Rto' interval, doubling after each
    *      retransmission
    *    - Continue retransmission until a total of 'Rc' requests have been
    *      sent
    *    - If, after the last request, a duration equal to 'Rm' * 'Rto' has
    *      passed without a response, STUN transaction is considered
    *      failed
    *
    *    NOTE: When a configuration value is deemed out of range of the 
    *          capabilities of the support offered, the algorithm will default 
    *          to the one defined in RFC 5389
    */
   BOS_UINT32      uRto;       /* Request retransmission RTO. */
   BOS_UINT32      uRc;        /* Request transmission max count. */  
   BOS_UINT32      uRm;        /* Request transmission timeout before 
                               ** reporting STUN failures */
#endif

} STUN_RETRANS;

/*****************************************************************************
*
*  FUNCTION:   STUN_SENDPKTCB - Send packet callback
*
*  PURPOSE:    A callback function for sending the STUN packets into
*              the network.
*
*  PARAMETERS:
*              sid     [IN] - socket id
*              pDest   [IN] - destination IP and port
*              pData   [IN] - pointer to data to be sent
*              len     [IN] - length of data in number of bytes
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
typedef void (*STUN_SENDPKTCB)( const BOS_UINT32      sid,
                                const STUN_TRANSPORT  *pDest,
                                const BOS_UINT8       *pData,
                                const BOS_UINT16      len );

/*****************************************************************************
*
*  FUNCTION:   STUN_RAND32CB - 32 bit random number generator callback
*
*  PURPOSE:    A callback function for generating 32bit random values.
*              This allows the application to make use of its
*              platform specific random generator for increased security.
*              If not provisioned, the default random generator is used.
*
*  PARAMETERS:
*              pRand [OUT] - the generated random value
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
typedef void (*STUN_RAND32CB)( BOS_UINT32 *pRand );

/*****************************************************************************
*
*  FUNCTION:   STUN_HMACCB - HMAC (hashing message authentication
*                             code callback
*
*  PURPOSE:    A callback function for peforming the hashing of the input
*              data.
*
*              This allows the application to make use of its
*              platform specific hashing capability.
*
*              The callback function is required for supporting
*              shared-secret and integrity check operations.
*
*  PARAMETERS:
*              alg    [IN] - hmac algorithm
*              pKey   [IN] - hash key, keylen
*              pData  [IN] - data value and length
*              pHmac [OUT] - the hashed data
*
*  RETURNS:
*
*  NOTES:
*              The data must be multiple of 512 bits by padding with zeros.
*
*****************************************************************************/
typedef void (*STUN_HMACCB)( const STUN_HMAC_TYPE alg,
                             const STUN_DATA      *pKey,
                             const STUN_DATA      *pData,
                             STUN_DATA            *pHmac );

/*****************************************************************************
*
*  FUNCTION:   STUN_CRC32CB - CRC32 callback
*
*  PURPOSE:    A callback function for peforming CRC32 check of the input
*              data.
*
*              This callback is required for the support of
*              STUN message inspection using
*              STUN_INSP_FINGERPRINT
*
*  PARAMETERS:
*              pData [IN] - data
*              len   [IN] - data length
*
*  RETURNS:    32-bit CRC value.
*
*  NOTES:
*
*****************************************************************************/
typedef BOS_UINT32 (*STUN_CRC32CB)( const BOS_UINT8   *pData,
                                    const BOS_UINT16  len );

/*****************************************************************************
*
*  FUNCTION:   STUN_LOGCB - Log handler
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
typedef void (*STUN_LOGCB)( const char *format, ... );

/*****************************************************************************
*
*  FUNCTION:   STUN_RESCB - Response Callback
*
*  PURPOSE:    A callback function for receiving Response event for
*              Request/response transaction.
*
*  PARAMETERS:
*              reqHdl   [IN] - request handle
*              pData    [IN] - response data

*  RETURNS:
*           none
*
*  NOTES:

*****************************************************************************/
typedef BOS_BOOL (*STUN_RESCB)( const BOS_UINT32       reqHdl,
                                const STUN_RES_DATA    *pData );

/*****************************************************************************
*
*  FUNCTION:   STUN_REQCB - Request Callback
*
*  PURPOSE:    A callback function for receiving STUN requests.
*
*  PARAMETERS:
*              pData    [IN] - Request data

*  RETURNS:
*           none
*
*  NOTES:

*****************************************************************************/
typedef BOS_BOOL (*STUN_REQCB)( const STUN_REQ_DATA   *pData );

/*****************************************************************************
*
*  FUNCTION:   STUN_INDCB - Indication Callback
*
*  PURPOSE:    A callback function for receiving STUN Indications.
*
*  PARAMETERS:
*              pData    [IN] - Indication data

*  RETURNS:
*           none
*
*  NOTES:

*****************************************************************************/
typedef BOS_BOOL (*STUN_INDCB)( const STUN_REQ_DATA   *pData );

/* STUN configuration */
typedef struct
{
   STUN_SENDPKTCB sendPktCB;  /* Send packet callback */
   STUN_RAND32CB  rand32CB;   /* 32 bit random number generator callback */
   STUN_HMACCB    hmacCB;     /* HMAC hashing callback */
   STUN_CRC32CB   crc32CB;    /* CRC32 check callback */
   STUN_LOGCB     logCB;      /* LOG callback */

} STUN_CFG;

/*
 * Inspection of data for STUN message
 *
 * (Quoted from RFC3489bis-05.txt)
 * Some usages (such as the binding keepalive) require STUN messages to
 * be sent on the same transport address as some application protocol,
 * such as RTP or SIP.  To facilitate the demultiplexing of the two,
 * STUN defines a special field in the message called the magic cookie,
 * which is a fixed 32 bit value that identifies STUN traffic.  STUN
 * requests also contain a fingerprint, which is a cryptographic hash of
 * the message, that allow for validation that the message was a STUN
 * request and not a data packet that happened to have the same 32 bit
 * value in the right place in the message.
 */
typedef enum
{
   STUN_INSP_BASIC,                 /* Basic mode
                                     * Per RFC3489, checks the first
                                     * 2 bits of the input buffer
                                     * for 00
                                     */
#if NTCFG_STUN_MAGIC_COOKIE_SUPPORT
   STUN_INSP_MAGIC_COOKIE,          /* Magic cooke mode
                                     * Per RFC3489-bis-05, checks
                                     * the first 32bit of the the
                                     * transaction Id to looking
                                     * for magic cookie 0x2112A442
                                     */
#endif
   STUN_INSP_FINGERPRINT            /* Fingerprinting mode
                                     * Per RFC3489-bis-05, checks
                                     * for both MAGIC_COOKIE and
                                     * for the FINGERPRINT attribute
                                     * at the end of the STUN message.
                                     * This mode eliminates the
                                     * uncertainty of non STUN data may
                                     * contain the 0x2112A442 value
                                     * The support of FINGERPRINT
                                     * is optional.
                                     */
} STUN_INSP_MODE;

/*
** Exports
*/
#if defined(__cplusplus)
extern "C"
{
#endif
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
extern BOS_BOOL stunInit( const STUN_CFG *pStunCfg );

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
extern BOS_BOOL stunDeinit( void );

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
extern void stunSetMsgType( BOS_UINT8 *pBuf, const BOS_UINT16 msgType );

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
extern BOS_UINT16 stunGetMsgType( const BOS_UINT8 *pBuf );

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
extern void stunSetTransactionId( BOS_UINT8 *pBuf, const STUN_TRANSACTION_ID *pTxid );

/*****************************************************************************
*  FUNCTION:   stunGetTransactionId
*
*  PURPOSE:    Get transaction id
*
*  PARAMETERS:
*              pBuf     [IN] - STUN message buffer
*
*  RETURNS:    Transaction id
*
*  NOTES:
*
*****************************************************************************/
extern STUN_TRANSACTION_ID *stunGetTransactionId( const BOS_UINT8 *pBuf );


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
extern void stunSetAttribute( BOS_UINT8 *pBuf, const BOS_UINT16 attr, const void *pData );

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
extern BOS_BOOL stunGetAttribute( const BOS_UINT8 *pBuf,
                                  const BOS_UINT16 attr,
                                  void *pData );


/*****************************************************************************
*  FUNCTION:   stunEncode
*
*  PURPOSE:    Encode STUN message for sending into network.
*              Take in the STUN message stored in the <pBuf> and
*              overwrite it with encoded payload for sending into
*              the network.
*
*  PARAMETERS:
*              pBuf     [MOD] - STUN message buffer
*              pLen     [OUT] - payload length
*              pPwd     [IN]  - password for message integrity
*              options  [IN]  - STUN options
*
*  RETURNS:    BOS_TRUE if encoding is successful. BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
extern BOS_BOOL stunEncode( BOS_UINT8              *pBuf,
                            BOS_UINT16             *pLen,
                            const STUN_HMAC_PWD    *pPwd,
                            const STUN_OPTS        options );

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
extern BOS_BOOL stunSendRequest( const BOS_UINT32            sid,
                                 const STUN_TRANSPORT        *pDest,
                                 const BOS_UINT8             *pBuf,
                                 const BOS_UINT16            len,
                                 BOS_UINT32                  *pReqHdl,
                                 const STUN_RESCB            resCB,
                                 const STUN_RETRANS          *pRetxData,
                                 const STUN_HMAC_PWD         *pPwd,
                                 const STUN_OPTS             options,
                                 const void                  *pOpaq );

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
extern BOS_BOOL stunSendResponse( const BOS_UINT32           sid,
                                  const STUN_TRANSPORT       *pDest,
                                  const BOS_UINT8            *pBuf,
                                  const BOS_UINT16           len );

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
extern BOS_BOOL stunSendIndication( const BOS_UINT32         sid,
                                    const STUN_TRANSPORT     *pDest,
                                    const BOS_UINT8          *pBuf,
                                    const BOS_UINT16         len );

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
extern BOS_BOOL stunIsStunMsg( const BOS_UINT8            *pData,
                               const BOS_UINT16           len,
                               const STUN_INSP_MODE       mode );

/*****************************************************************************
*
*  FUNCTION:   stunRecvPacket
*
*  PURPOSE:    Process STUN packets received from network.
*
*              The STUN message is first parsed.
*
*              If parsing fails (including unknown message type),
*              the packet is dropped.
*
*              If it's a Response, authentication verification
*              is performed for.
*
*              If it's a Request, the supported request callback is
*              invoked.
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
extern void stunRecvPacket( const BOS_UINT32       sid,
                            const STUN_TRANSPORT   *pLclAddr,
                            const STUN_TRANSPORT   *pSrcAddr,
                            const BOS_UINT8        *pData,
                            const BOS_UINT16       len );

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
extern BOS_BOOL stunIntegrityCheck( const void *pMsg,
                                    const void *pHash,
                                    const BOS_UINT16 len,
                                    const STUN_HMAC_PWD *pPwd );

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
*  NOTES:      Only supports STUN_MSG_BINDING_REQUEST
*
*****************************************************************************/
extern BOS_BOOL stunSetRequestCallback( const BOS_UINT16 request, const STUN_REQCB reqCB );

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
                                    const STUN_INDCB   indCB );


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
extern BOS_BOOL stunCancelRequest( const BOS_UINT32 uReqHdl );

#if defined(__cplusplus)
}
#endif
#endif
