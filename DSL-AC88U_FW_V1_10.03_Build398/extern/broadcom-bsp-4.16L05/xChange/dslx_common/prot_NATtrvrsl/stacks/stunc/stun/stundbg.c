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
*      STUN  - debug module.
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <ntcfg.h>
#include <stun.h>
#include <bosIpAddr.h>
#include "stuni.h"
#include "./inc/stun.h"
#include "stunmsg.h"
#include "stuntx.h"
#include "stundbg.h"

#if NTCFG_TURN_SUPPORT
#if TURNDBG_LOCAL_RESPONSE
#include <turndbg.h>
extern BOS_BOOL turnMsgType( BOS_UINT16 msgType );
#endif
#endif

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
#if STUNDBG_LOCAL_RESPONSE
/* test flags */
extern int gStuntest_BindingError;
extern int gStuntest_SharedSecretError;

#define ERROR_REASON_401      "Unauthorized    " //error 401
#define ERROR_REASON_433      "Use TLS         " //error 433
#define ERROR_REASON_SIZE     16

/* use printable characters as username and password (4-byte aligned)*/
#define USERNAME_SIZE  16
#define PASSWORD_SIZE  16
#endif

/* ---- Private Variables ------------------------------------ */
#if STUNDBG_LOCAL_RESPONSE
static char dbg_username[] =  "u-s-e-r-n-a-m-e-";
static char dbg_password[] =  "p+a+s+s+w+o+r+d+";
#endif

/* ---- Private Function Prototypes -------------------------- */
#if STUNDBG_LOCAL_RESPONSE
static void generateStunResponse( const BOS_UINT32      sid,
                                  const STUN_TRANSPORT  *pDest,
                                  BOS_UINT8             *pParsedMsgBuf );
#endif

/* ---- Functions -------------------------------------------- */


/* The following section of code is written for debugging and unit
 * testing purposes only
 */
#if STUNDBG_LOCAL_RESPONSE
/*****************************************************************************
*  FUNCTION:
*     stundbg_GenerateLocalResponse
*
*  PURPOSE:
*     Send canned response.
*     For Shared Secret request, send response with Username and password
*     attributes.
*     For Binding Request, send response 200
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
void stundbg_GenerateLocalResponse( const BOS_UINT32      sid,
                                    const STUN_TRANSPORT  *pDest,
                                    const BOS_UINT8       *pData,
                                    const BOS_UINT16      len )
{
   BOS_UINT16 msgType;
   BOS_UINT8 parsedMsgBuf[STUN_MSG_BUFFER_SIZE];
   BOS_UINT8 hash[STUN_MAX_HASH_LEN];
   BOS_UINT16 hmacMsgLen;

   memset( parsedMsgBuf, 0, sizeof( parsedMsgBuf ) );

   if ( BOS_TRUE != stunIsStunMsg( pData, len, STUN_INSP_MAGIC_COOKIE ) )
   {
      LOG(( "stundbg_GenerateStunResponse: NOT STUN MSG " ));
      return;
   }

   if ( BOS_TRUE != stunmsgParse( pData, len, hash, &hmacMsgLen, (STUN_MSG*)parsedMsgBuf ) )
   {
      LOG(("stundbg_GenerateStunResponse: parsing error"));
      return;
   }

   msgType = stunGetMsgType( parsedMsgBuf );
   if ( STUN_REQUEST != stunmsgClass( msgType ))
   {
      LOG(("stundbg_GenerateStunResponse: NOT a request. discard"));
      return;
   }

   DLOG(( "stundbg_GenerateLocalResponse: msgType %x", msgType));

   if ( BOS_TRUE == stunmsgType( msgType ) )
   {
      generateStunResponse( sid, pDest, parsedMsgBuf );
   }
#if NTCFG_TURN_SUPPORT
#if TURNDBG_LOCAL_RESPONSE
   else if ( BOS_TRUE == turnMsgType( msgType ) )
   {
      turndbg_GenerateTurnResponse( sid, pDest, parsedMsgBuf );
   }
#endif
#endif
   else
   {
      DLOG(( "stundbg_GenerateLocalResponse: No response generated, unsupported msgType %x", msgType));
   }
}

/*****************************************************************************
*  FUNCTION:
*     generateStunResponse
*
*  PURPOSE:
*     Send canned response.
*     For Shared Secret request, send response with Username and password
*     attributes.
*     For Binding Request, send response 200
*
*  PARAMETERS:
*     sid     [IN] - socket id
*     pDest   [IN] - destination IP and port
*     pData   [IN] - point to data to be sent (network byte ordered)
*     len     [IN] - length of data in number of bytes
*
*  RETURNS:
*      none
*
*  NOTES:
******************************************************************************/
static void generateStunResponse( const BOS_UINT32      sid,
                                  const STUN_TRANSPORT  *pDest,
                                  BOS_UINT8              *pParsedMsgBuf )
{
   BOS_UINT16 msgType;
   STUN_TRANSACTION_ID *pTxid;
   BOS_UINT8 msgBuf[STUN_MSG_BUFFER_SIZE];
   STUN_TRANSPORT  lclAddr;

//   lclAddr.port = bosSocketHtoNS(888);
//   lclAddr.ipAddr.u32 = bosSocketHtoNL(0x0a88406a);
// 10.136.65.183:6000
   lclAddr.port = 6000;
   (void)bosIpAddrV4CreateFromU32( 0x0a8841b7, &lclAddr.ipAddr );

   memset( msgBuf, 0, sizeof( msgBuf ) );

   msgType = stunGetMsgType( pParsedMsgBuf );

   pTxid = stunGetTransactionId( pParsedMsgBuf );
   /* set transaction id */
   stunSetTransactionId( msgBuf, pTxid );

   switch( msgType )
   {
      /* binding request */
      case STUN_MSG_BINDING_REQUEST:
      {
         /* Binding response */
         BOS_UINT16 size;
         STUN_HMAC_PWD pwd;
         STUN_HMAC_PWD *pPwd = NULL;
         pwd.len = 0;
         if ( !gStuntest_BindingError )
         {
            STUN_TRANSPORT socAddr;
            STUN_DATA username;
            STUN_DATA integrity;
            BOS_UINT8 uBuf[STUN_MAX_DATA_LEN];
            BOS_UINT8 iBuf[STUN_MAX_DATA_LEN];
 
            username.pData = uBuf;
            integrity.pData = iBuf;

            stunSetMsgType( msgBuf, STUN_MSG_BINDING_RESPONSE );

            socAddr.port = 10000;
            (void)bosIpAddrV4CreateFromU32( 0x0a88405a, &socAddr.ipAddr );
            stunSetAttribute( msgBuf, STUN_ATTR_MAPPED_ADDRESS, (void *)&socAddr );

            stunSetAttribute( msgBuf, STUN_ATTR_XORMAPPED_ADDRESS, (void *)&socAddr );

            socAddr.port = 20000;
            (void)bosIpAddrV4CreateFromU32( 0x0a88405b, &socAddr.ipAddr );
            stunSetAttribute( msgBuf, STUN_ATTR_SOURCE_ADDRESS, (void *)&socAddr );

            socAddr.port = bosSocketHtoNS(20001);
            (void)bosIpAddrV4CreateFromU32( 0x0a88405b, &socAddr.ipAddr );
            stunSetAttribute( msgBuf, STUN_ATTR_CHANGED_ADDRESS, (void *)&socAddr );

            /* make sure both or none username and Integrity attributes are
             * present
             */
            if ( stunGetAttribute( pParsedMsgBuf, STUN_ATTR_USERNAME, &username ) &&
                 stunGetAttribute( pParsedMsgBuf, STUN_ATTR_MESSAGE_INTEGRITY, &integrity ) )
            {

               /* Send response with Integrity if request contains
                * username and integrity
                */
               memcpy(pwd.val, dbg_password, PASSWORD_SIZE);
               pwd.len = PASSWORD_SIZE;
               pPwd = &pwd;
            }
         }
         else
         {
            STUN_ERR stunErr;
            stunErr.errnoa = 401;
            stunErr.pPhrase = NULL;
            /* Binding Error response */

            stunSetMsgType( msgBuf, STUN_MSG_BINDING_ERROR_RESPONSE );
            stunSetAttribute( msgBuf, STUN_ATTR_ERRORCODE, (void *)&stunErr );
         }

         size = sizeof( msgBuf );
         if ( BOS_TRUE != stunEncode( msgBuf, &size, pPwd, STUN_OPT_FINGERPRINT ) )
         {
            BOS_ASSERT( BOS_FALSE );
         }
         /* send canned response */
         stunIsStunMsg( msgBuf, size, STUN_INSP_FINGERPRINT );
         stunRecvPacket( sid, &lclAddr, pDest, msgBuf, size );
         //stunSendResponse( sid, &lclAddr, msgBuf, size );
      }
      break;
      /* shared secret request */
      case STUN_MSG_SHARED_SECRET_REQUEST:
      {
         BOS_UINT16 size;

         if ( !gStuntest_SharedSecretError )
         {
            STUN_DATA username;
            STUN_DATA password;
            username.pData = (BOS_UINT8 *)dbg_username;
            username.len = USERNAME_SIZE;
            password.pData = (BOS_UINT8 *)dbg_password;
            password.len = PASSWORD_SIZE;
 
            stunSetMsgType( msgBuf, STUN_MSG_SHARED_SECRET_RESPONSE );
            /* insert user name and password */
            stunSetAttribute( msgBuf, STUN_ATTR_USERNAME, (void *)&username );
            stunSetAttribute( msgBuf, STUN_ATTR_PASSWORD, (void *)&password );
         }
         else
         {
            STUN_ERR stunErr;
            STUN_DATA errPhrase;
            /* shared secret error response */
            stunSetMsgType( msgBuf, STUN_MSG_SHARED_SECRET_ERROR_RESPONSE );
            stunErr.errnoa = 433;
            errPhrase.pData = (BOS_UINT8 *)"433 error";
            errPhrase.len = (BOS_UINT16)strlen("433 error");
            stunErr.pPhrase = &errPhrase;
            stunSetAttribute( msgBuf, STUN_ATTR_ERRORCODE, (void *)&stunErr );
         }

         size = sizeof( msgBuf );
         if ( BOS_TRUE != stunEncode( msgBuf, &size, NULL, 0 ) )
         {
            BOS_ASSERT( BOS_FALSE );
         }
 
         /* canned response */
         stunRecvPacket( sid, &lclAddr, pDest, msgBuf, size );
         //stunSendResponse( sid, &lclAddr, msgBuf, size );
      }
      break;
      default:
      {
         DLOG(( "generateStunResponse: No response generated, unsupported request msgType %x", msgType));
      }
   }
}
#endif
