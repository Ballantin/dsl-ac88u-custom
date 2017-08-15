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
*      TURN Message encoder Parser for the TURNC module
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <ntcfg.h>
#include <bosTypes.h>
#include <bosSocket.h>
#include "turn.h"
#include "turndbg.h"
/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */

#define VERIFY(x) if (BOS_TRUE != x) return (BOS_FALSE)
#define TRAP(x) if (x) return (BOS_FALSE)

/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*  FUNCTION:   turnMsgType
*
*  PURPOSE:    Return BOS_TRUE if a STUN Relay usage message.
*
*  PARAMETERS:
*              msgType - message type
*
*  RETURNS:    BOS_TRUE, BOS_FALSE otherwise
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL turnMsgType( BOS_UINT16 msgType )
{
   switch( msgType )
   {
      case STUN_MSG_ALLOCATE_RESPONSE:
      case STUN_MSG_ALLOCATE_ERROR_RESPONSE:
      case STUN_MSG_ACTIVE_DEST_RESPONSE:
      case STUN_MSG_ACTIVE_DEST_ERROR_RESPONSE:
      case STUN_MSG_CONNECT_RESPONSE:
      case STUN_MSG_CONNECT_ERROR_RESPONSE:
      case STUN_MSG_ALLOCATE_REQUEST:
      case STUN_MSG_ACTIVE_DEST_REQUEST:
      case STUN_MSG_CONNECT_REQUEST:
      case STUN_MSG_SEND_INDICATION:
      case STUN_MSG_DATA_INDICATION:
      case STUN_MSG_CONNECT_STATUS_INDICATION:
      {
         return( BOS_TRUE );

      }
      default:
      {
         return( BOS_FALSE );
      }
   }
}
