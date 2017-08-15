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
*      STUN utility module.
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <bosIpAddr.h>
#include "stunutil.h"
#include "stunmsg.h"

/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */
/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */
/*****************************************************************************
*
*  FUNCTION:   stunutilStunToBosTransport
*
*  PURPOSE:    convert from STUN address data to BOS socket address data.
*
*  PARAMETERS:
*           pStunAddr [IN] - STUN address data
*           pBosAddr  [OUT] - BOS socket address data
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void stunutilStunToBosTransport( const STUN_ATTR_ADDR *pStunAddr, STUN_TRANSPORT *pBosAddr )
{
   BOS_STATUS status = BOS_STATUS_OK;

   BOS_ASSERT( NULL != pStunAddr && NULL != pBosAddr );

   switch ( pStunAddr->family )
   {
      case STUN_IPV4_FAMILY: 
      {
         pBosAddr->port = pStunAddr->ipAddr.v4.port;
         status = bosIpAddrV4CreateFromU32( pStunAddr->ipAddr.v4.addr, &pBosAddr->ipAddr );
      }
      break;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
      case STUN_IPV6_FAMILY: 
      {
         pBosAddr->port = pStunAddr->ipAddr.v6.port;
         status = bosIpAddrV6CreateFromU8( pStunAddr->ipAddr.v6.d8, &pBosAddr->ipAddr );
      }
      break;
#endif
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }

   BOS_ASSERT( BOS_STATUS_OK == status );
}

/*****************************************************************************
*
*  FUNCTION:   stunutilBosToStunTransport
*
*  PURPOSE:    convert from BOS socket address data to Stun address data.
*
*  PARAMETERS:
*           pBosAddr  [IN] - BOS socket address data
*           pStunAddr [OUT] - STUN address data
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void stunutilBosToStunTransport( const STUN_TRANSPORT *pBosAddr, STUN_ATTR_ADDR *pStunAddr )
{
   BOS_STATUS status = BOS_STATUS_OK;
   BOS_IP_ADDRESS_TYPE addrType;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
   unsigned int len;
#endif

   BOS_ASSERT( NULL != pStunAddr && NULL != pBosAddr );

   addrType = bosIpAddrGetType( &pBosAddr->ipAddr );

   pStunAddr->pad = 0x00;

   switch ( addrType )
   {
      case BOS_IPADDRESS_TYPE_V4: 
      {
         pStunAddr->family = STUN_IPV4_FAMILY;
         pStunAddr->ipAddr.v4.port = pBosAddr->port;
         status = bosIpAddrV4GetU32( &pBosAddr->ipAddr, &pStunAddr->ipAddr.v4.addr );
      }
      break;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
      case BOS_IPADDRESS_TYPE_V6: 
      {
         pStunAddr->family = STUN_IPV6_FAMILY;
         pStunAddr->ipAddr.v6.port = pBosAddr->port;
         status = bosIpAddrGetU8( &pBosAddr->ipAddr, pStunAddr->ipAddr.v6.d8, STUN_IP_ADDR_LEN_BYTES_V6, &len );
         BOS_ASSERT ( STUN_IP_ADDR_LEN_BYTES_V6 == len );
      }
      break;
#endif
      default:
      {
         BOS_ASSERT( BOS_FALSE );
      }
   }

   BOS_ASSERT( BOS_STATUS_OK == status );
}

