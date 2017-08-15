/****************************************************************************
*
* Copyright © 2002-2008 Broadcom Corporation
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
*      TURNC  - debug module.
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <ntcfg.h>
#include <bosTypes.h>
#include <bosError.h>
#include <bosIpAddr.h>
#include "turn.h"
#include "turndbg.h"
#include "turnci.h"

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
#if TURNDBG_LOCAL_RESPONSE

#define USERNAME  "u-s-e-r-n-a-m-e-"
#define USERNAME_SIZE  16
#define PASSWORD  "p+a+s+s+w+o+r+d+"
#define PASSWORD_SIZE  16


#define BANDWIDTH_DEFAULT  1000000

/* test flags */
extern int gTurnctest_AllocateError;
extern int gTurnctest_ActiveDestError;
extern int gTurnctest_ConnectError;

int gTurndbg_AllocateLifetime = 20;
int gTurndbg_AllocateErrorCode = 442;
int gTurndbg_ActiveDestErrorCode = 439;
int gTurndbg_ConnectErrorCode = 446;

typedef struct
{
   int errCode;
   char *errStr;
} ERRMAP;

ERRMAP errMap[] =
{
   {437, "No Binding  "},
   {439, "Transitioning   "},
   {441, "Wrong Username  "},
   {442, "Unsupported Transport Protocol  "},
   {443, "Invalid IP Address  "},
   {444, "Invalid Port"},
   {445, "Operation for TCP Only  "},
   {446, "Connection Failure  "},
   {447, "Connection Timeout  "},
   {-1, "BAD ERROR STRING" },
};
#endif

/* ---- Private Variables ------------------------------------ */

/* ---- Private Function Prototypes -------------------------- */
static char *getErrStr( int errCode );

/* ---- Functions -------------------------------------------- */


/* The following section of code is written for debugging and unit
 * testing purposes only
 */
#if TURNDBG_LOCAL_RESPONSE

/*****************************************************************************
*  FUNCTION:
*     turndbg_GenerateTurnResponse
*
*  PURPOSE:
*     Send canned responses.
*     For Allocate, Set Active Destination, Connect requests.
*
*  PARAMETERS:
*     sid     [IN] - socket id
*     pDest   [IN] - destination IP and port
*     pData   [IN] - point to data to be sent (network byte ordered)
*     len     [IN] - length of data in number of bytes
*
*  RETURNS:
*     none
*
*  NOTES:
*
*****************************************************************************/
void turndbg_GenerateTurnResponse( const BOS_UINT32      sid,
                                   const STUN_TRANSPORT  *pDest,
                                   BOS_UINT8             *pParsedMsgBuf )
{
   BOS_UINT16 msgType;
   STUN_TRANSACTION_ID *pTxid;
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   STUN_TRANSPORT lclAddr;
   BOS_UINT16 size;

   lclAddr.port = 999;
   (void)bosIpAddrV4CreateFromU32( 0x0a88407a, &lclAddr.ipAddr );

   memset( msgBuf, 0, sizeof( msgBuf ) );

   msgType = stunGetMsgType( pParsedMsgBuf );

   pTxid = stunGetTransactionId( pParsedMsgBuf );
   /* set transaction id */
   stunSetTransactionId( msgBuf, pTxid );

   switch( msgType )
   {
      case STUN_MSG_ALLOCATE_REQUEST:
      {
         STUN_HMAC_PWD pwd;
         STUN_HMAC_PWD *pPwd = NULL;
         STUN_DATA username;
         STUN_DATA integrity;
         STUN_TRANSPORT socAddr;
         BOS_UINT32 bandwidth;
         BOS_UINT8 uBuf[STUN_MAX_DATA_LEN];
         BOS_UINT8 iBuf[STUN_MAX_DATA_LEN];

         pwd.len = 0;
         username.pData = uBuf;
         integrity.pData = iBuf;

         if ( !stunGetAttribute( pParsedMsgBuf, STUN_ATTR_BANDWIDTH, &bandwidth ) )
         {
            bandwidth = BANDWIDTH_DEFAULT;
         }

         /*
          * check for username and integrity, assert error if missing
          */
         if ( stunGetAttribute( pParsedMsgBuf, STUN_ATTR_USERNAME, &username ) &&
              stunGetAttribute( pParsedMsgBuf, STUN_ATTR_MESSAGE_INTEGRITY, &integrity ) )
         {
            memcpy(pwd.val, PASSWORD, PASSWORD_SIZE);
            pwd.len = PASSWORD_SIZE;
            pPwd = &pwd;
         }
         else
         {
            BOS_ASSERT( BOS_FALSE );
         }

         if ( !gTurnctest_AllocateError )
         {
            /* Success response to contain:
             * 1. RELAY-ADDRESS
             * 2. XOR-MAPPED-ADDRESS
             * 3. BANDWIDTH
             * 4. LIFETIME (optional)
             */

            stunSetMsgType( msgBuf, STUN_MSG_ALLOCATE_RESPONSE );
            /* RELAY-ADDRESS (24.85.161.117:20000) */
            socAddr.port = 20000;
            (void)bosIpAddrV4CreateFromU32( 0x1855a175, &socAddr.ipAddr );
            stunSetAttribute( msgBuf, STUN_ATTR_RELAY_ADDRESS, (void *)&socAddr );

            /* XOR-MAPPED-ADDRESS (10.136.64.90:10000) */
            socAddr.port = 10000;
            (void)bosIpAddrV4CreateFromU32( 0x0a88405a, &socAddr.ipAddr );
            stunSetAttribute( msgBuf, STUN_ATTR_XORMAPPED_ADDRESS, (void *)&socAddr );
 
            /* BANDWIDTH
             * Extract from request if present, if not insert one with
             * default value
             */
            stunSetAttribute( msgBuf, STUN_ATTR_XORMAPPED_ADDRESS, (void *)&socAddr );
            stunSetAttribute( msgBuf, STUN_ATTR_BANDWIDTH, (void *)&bandwidth );
            /* LIFETIME */
            if ( gTurndbg_AllocateLifetime )
            {
               stunSetAttribute( msgBuf, STUN_ATTR_LIFETIME, (void *)&gTurndbg_AllocateLifetime );
            }
         }
         else
         {
            STUN_ERR stunErr;
            STUN_DATA errPhrase;
            stunErr.errnoa = (BOS_UINT32)gTurndbg_AllocateErrorCode;
            stunErr.pPhrase = &errPhrase;
            errPhrase.pData = (BOS_UINT8 *)getErrStr(gTurndbg_AllocateErrorCode);
            errPhrase.len = (BOS_UINT16)strlen( getErrStr(gTurndbg_AllocateErrorCode));
            /* Allocate Error response */
            stunSetMsgType( msgBuf, STUN_MSG_ALLOCATE_ERROR_RESPONSE );
            stunSetAttribute( msgBuf, STUN_ATTR_ERRORCODE, (void *)&stunErr );
         }

         size = sizeof( msgBuf );
         if ( BOS_TRUE != stunEncode( msgBuf, &size, pPwd, 0 ) )
         {
            BOS_ASSERT( BOS_FALSE );
         }

         /* send canned response */
         stunRecvPacket( sid, &lclAddr, pDest, msgBuf, size );
      }
      break;
      case STUN_MSG_ACTIVE_DEST_REQUEST:
      {
         STUN_HMAC_PWD pwd;
         STUN_HMAC_PWD *pPwd = NULL;
         STUN_DATA username;
         STUN_DATA integrity;
         STUN_TRANSPORT bosAddr;
         BOS_UINT8 uBuf[STUN_MAX_DATA_LEN];
         BOS_UINT8 iBuf[STUN_MAX_DATA_LEN];

         pwd.len = 0;
         username.pData = uBuf;
         integrity.pData = iBuf;

         /*
          * check for username and integrity, assert error if missing
          */
         if ( stunGetAttribute( pParsedMsgBuf, STUN_ATTR_USERNAME, &username ) &&
              stunGetAttribute( pParsedMsgBuf, STUN_ATTR_MESSAGE_INTEGRITY, &integrity ) )
         {
            memcpy(pwd.val, PASSWORD, PASSWORD_SIZE);
            pwd.len = PASSWORD_SIZE;
            pPwd = &pwd;
         }
         else
         {
            BOS_ASSERT( BOS_FALSE );
         }

         if ( !gTurnctest_ActiveDestError )
         {
            if ( !stunGetAttribute( pParsedMsgBuf, STUN_ATTR_REMOTE_ADDRESS, &bosAddr ))
            {
               DLOG(( "Preferred External Client cleared"));
            }

            stunSetMsgType( msgBuf, STUN_MSG_ACTIVE_DEST_RESPONSE );
         }
         else
         {
            STUN_ERR stunErr;
            STUN_DATA errPhrase;
            stunErr.errnoa = (BOS_UINT32)gTurndbg_ActiveDestErrorCode;
            stunErr.pPhrase = &errPhrase;
            errPhrase.pData = (BOS_UINT8 *)getErrStr(gTurndbg_ActiveDestErrorCode);
            errPhrase.len = (BOS_UINT16)strlen(getErrStr(gTurndbg_ActiveDestErrorCode));
            /* Allocate Error response */
            stunSetMsgType( msgBuf, STUN_MSG_ALLOCATE_ERROR_RESPONSE );
            stunSetAttribute( msgBuf, STUN_ATTR_ERRORCODE, (void *)&stunErr );
         }

         size = sizeof( msgBuf );
         if ( BOS_TRUE != stunEncode( msgBuf, &size, pPwd, 0 ) )
         {
            BOS_ASSERT( BOS_FALSE );
         }

         /* send canned response */
         stunRecvPacket( sid, &lclAddr, pDest, msgBuf, size );
      }
      break;
      case STUN_MSG_CONNECT_REQUEST:
      {
         STUN_HMAC_PWD pwd;
         STUN_HMAC_PWD *pPwd = NULL;
         STUN_DATA username;
         STUN_DATA integrity;
         STUN_TRANSPORT socAddr;
         BOS_UINT8 uBuf[STUN_MAX_DATA_LEN];
         BOS_UINT8 iBuf[STUN_MAX_DATA_LEN];

         pwd.len = 0;
         username.pData = uBuf;
         integrity.pData = iBuf;

         /*
          * check for username and integrity, assert error if missing
          */
         if ( stunGetAttribute( pParsedMsgBuf, STUN_ATTR_USERNAME, &username ) &&
              stunGetAttribute( pParsedMsgBuf, STUN_ATTR_MESSAGE_INTEGRITY, &integrity ) )
         {
            memcpy(pwd.val, PASSWORD, PASSWORD_SIZE);
            pwd.len = PASSWORD_SIZE;
            pPwd = &pwd;
         }
         else
         {
            BOS_ASSERT( BOS_FALSE );
         }

         if ( !gTurnctest_ConnectError )
         {
 
            if ( !stunGetAttribute( pParsedMsgBuf, STUN_ATTR_REMOTE_ADDRESS, &socAddr ))
            {
               /* Missing Remote Address attribute */
               BOS_ASSERT( BOS_FALSE );
            }

            stunSetMsgType( msgBuf, STUN_MSG_CONNECT_RESPONSE );
         }
         else
         {
            STUN_ERR stunErr;
            STUN_DATA errPhrase;
            stunErr.errnoa = (BOS_UINT32)gTurndbg_ConnectErrorCode;
            stunErr.pPhrase = &errPhrase;
            errPhrase.pData = (BOS_UINT8 *)getErrStr(gTurndbg_ConnectErrorCode);
            errPhrase.len = (BOS_UINT16)strlen(getErrStr(gTurndbg_ConnectErrorCode));
            /* Allocate Error response */
            stunSetMsgType( msgBuf, STUN_MSG_CONNECT_ERROR_RESPONSE );
            stunSetAttribute( msgBuf, STUN_ATTR_ERRORCODE, (void *)&stunErr );
         }

         size = sizeof( msgBuf );
         if ( BOS_TRUE != stunEncode( msgBuf, &size, pPwd, 0 ) )
         {
            BOS_ASSERT( BOS_FALSE );
         }

         /* send canned response */
         stunRecvPacket( sid, &lclAddr, pDest, msgBuf, size );
      }
      default:
      {
      }
   }
}


static char *getErrStr( int errCode )
{
   int i;
   for ( i = 0; errMap[i].errCode != -1; i++ )
   {
      if ( errMap[i].errCode == errCode )
      {
         return( errMap[i].errStr );
      }
   }
   return( NULL );
}

#endif
