/****************************************************************************
*
* Copyright © 2006-2008 Broadcom Corporation
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
*      TURN Client Reception layer
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <turnc.h>
#include <stun.h>
#include "turn.h"
#include "turnci.h"
#include "turnrx.h"
#include "turntx.h"

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */
/*****************************************************************************
*
*  FUNCTION:   turnrx_ResponseHandler
*
*  PURPOSE:    Event handler for TURN requests sent through the STUN
*              module.
*
*  PARAMETERS:
*
*  RETURNS:    BOS_BOOL if successful or BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL turnrx_ResponseHandler( const BOS_UINT32          reqHdl,
                                 const STUN_RES_DATA       *pResData )
{
   BOS_UINT16 msgType;
   TURNC_EVTCB evtCB;
   TURNC_EVT_DATA evtData;
   BOS_BOOL bNotify = BOS_TRUE;
   BOS_BOOL rc = BOS_TRUE;

   BOS_ASSERT( NULL != pResData );

   memset( &evtData, 0, sizeof(TURNC_EVT_DATA) );

   evtCB = *(TURNC_EVTCB *)(pResData->pOpaq);
   BOS_ASSERT( NULL != evtCB );

   switch( pResData->evt )
   {
      case STUN_TIMEOUT:
      {
         evtData.evt = TURNC_EVT_TX_TIMEOUT;
      }
      break;
      case STUN_SYSTEM_ERR:
      {
         evtData.evt = TURNC_EVT_SYSTEM_ERR;
      }
      break;
      case STUN_RESPONSE:
      {
         msgType = stunGetMsgType( pResData->pMsg );
         switch( msgType )
         {
            case STUN_MSG_ALLOCATE_RESPONSE:
            {
               BOS_UINT32 cnt = 0;
               BOS_UINT32 lifetime = 0;
               /*
                * The response is expected to contain both the
                * RELAY-ADDRESS nad XOR-MAPPED-ADDRESS attributes.
                */
               if ( !(stunGetAttribute( pResData->pMsg,
                                      STUN_ATTR_RELAY_ADDRESS,
                                      (void *)&evtData.data.allocAddrs.relayAddr)
                    && stunGetAttribute( pResData->pMsg,
                                         STUN_ATTR_XORMAPPED_ADDRESS,
                                         (void *)&evtData.data.allocAddrs.reflexiveAddr ) ) )
               {
                  /* server protocol error */
                  evtData.data.err.errnoa = (BOS_UINT32)0xffffffff;
                  strncpy((char *)evtData.data.err.reason.buf,
                          "SERVER PROTOCOL ERROR",
                          TURNC_MAX_DATA_SIZE-1);
                  evtData.data.err.reason.len = (BOS_UINT16)strlen((char *)evtData.data.err.reason.buf);
                  evtData.evt = TURNC_EVT_ALLOCATE_ERROR;
                  (*evtCB)( reqHdl, &evtData );
                  break;
               }
               else
               {
                  evtData.evt = TURNC_EVT_ALLOCATE_SUCCESS;
               }

               if ( stunGetAttribute( pResData->pMsg,
                                      STUN_ATTR_LIFETIME,
                                      (void *)&lifetime) )
               {
                  /* activate refresh, set time to 3/4 of lifetime */
                  cnt = turntx_Refresh( reqHdl, (lifetime*3)/4 );
               }
               else
               {
                  /* no refreshing needed, remove pending request
                   * from the refresh queue
                   */
                  turntx_Purge( reqHdl );
               }

               if ( cnt > 0 )
               {
                  bNotify = BOS_FALSE;
                  /* do not report success event from refreshes */
                  DLOG(("turnrx_ResponseHandler: Subsequent Allocate success from refreshes"));
               }
            }
            break;
            case STUN_MSG_ALLOCATE_ERROR_RESPONSE:
            case STUN_MSG_ACTIVE_DEST_ERROR_RESPONSE:
            case STUN_MSG_CONNECT_ERROR_RESPONSE:
            {
               STUN_ERR stunErr;
               STUN_DATA stunData;
               stunErr.pPhrase = &stunData;
               stunData.pData = evtData.data.err.reason.buf;
               stunData.len = TURNC_MAX_DATA_SIZE;
 
               turntx_Purge( reqHdl );

               if ( stunGetAttribute( pResData->pMsg, STUN_ATTR_ERRORCODE, (void *)&stunErr ) )
               {
                  if ( STUN_MSG_ALLOCATE_ERROR_RESPONSE == msgType )
                  {
                     evtData.evt = TURNC_EVT_ALLOCATE_ERROR;
                  }
                  else if ( STUN_MSG_ACTIVE_DEST_ERROR_RESPONSE == msgType )
                  {
                     evtData.evt = TURNC_EVT_ACTIVE_DEST_ERROR;
                  }
                  else
                  {
                     evtData.evt = TURNC_EVT_CONNECT_ERROR;
                  }
                  evtData.data.err.errnoa = stunErr.errnoa;
                  evtData.data.err.reason.len = stunErr.pPhrase->len;
               }
            }
            break;
            case STUN_MSG_ACTIVE_DEST_RESPONSE:
            {
               BOS_BOOL rc;
 
               rc = turntxSetActiveDest( pResData->sid,
                                         pResData->pSrcAddr,
                                         reqHdl );

               if ( rc )
               {
                   evtData.evt = TURNC_EVT_ACTIVE_DEST_SET;
               }
               else
               {
                  /* active destination unset */
                   evtData.evt = TURNC_EVT_ACTIVE_DEST_UNSET;
               }
            }
            break;
            case STUN_MSG_CONNECT_RESPONSE:
            {
               evtData.evt = TURNC_EVT_CONNECT_SUCCESS;
            }
            break;
            default:
            {
               bNotify = BOS_FALSE;
               rc = BOS_FALSE;
               /* unexpected messages */
               DLOG(("turnrx_ResponseHandler: Unexpected message %d", msgType));
            }
         }/* switch */
      }
      break;
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }/* switch */

   if ( bNotify )
   {
      (*evtCB)( reqHdl, &evtData );
   }

   return( rc );
}

/*****************************************************************************
*
*  FUNCTION:   turnrx_IndicationHandler
*
*  PURPOSE:    Event handler for TURN Indication received from the STUN
*              module.
*
*  PARAMETERS:
*
*  RETURNS:    BOS_BOOL if successful or BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL turnrx_IndicationHandler( const STUN_REQ_DATA   *pData )
{
   BOS_UINT16 msgType;

   BOS_ASSERT( NULL != pData );

   msgType = stunGetMsgType( pData->pMsg );

   switch( msgType )
   {
      case STUN_MSG_DATA_INDICATION:
      {
 
         STUN_TRANSPORT remoteAddr;
         STUN_DATA   stunData;
         if ( stunGetAttribute( pData->pMsg, STUN_ATTR_REMOTE_ADDRESS, &remoteAddr) && stunGetAttribute( pData->pMsg, STUN_ATTR_DATA, &stunData))
         {
            if ( (NULL != stunData.pData) &&
                 (stunData.len > 0) )
            {
               (*turncRecvCB)( pData->sid,
                               &remoteAddr,
                               stunData.pData,
                               stunData.len );
            }
         }
         else
         {
            /* Protocol error. drop data silently */
            DLOG(("turnrx_Process: Data Indication dropping data"));
         }
      }
      break;
      case STUN_MSG_CONNECT_STATUS_INDICATION:
      {
         /*
          * TBD specification incomplete
          */
         DLOG(("turnrx_Process: received Connect Status Indication"));
      }
      break;
      default:
      {
         /* unexpected messages */
         return( BOS_FALSE );
      }

   }

   return( BOS_TRUE );
}

