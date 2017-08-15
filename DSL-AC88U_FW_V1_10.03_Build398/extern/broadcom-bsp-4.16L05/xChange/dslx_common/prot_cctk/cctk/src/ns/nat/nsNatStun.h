/**********************************************************************************
** Copyright (c) 2010-2011 Broadcom
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file defines the STUN support for use within the Call Control
**      Toolkit.
**
***********************************************************************************/
#ifndef __NS_NAT_STUN__H__INCLUDED__
#define __NS_NAT_STUN__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <stunc.h>

/* ---- Constants and Types ----------------------------------------------------- */
#define NS__NAT_STUN__MAX_CONCURENT_REQUESTS \
                        2 * ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR))

typedef enum
{
   eNSNATSTUN_KEEPALIVE,
   eNSNATSTUN_UNKNOWN,

} NSNATSTUN;

typedef struct
{
   BOS_UINT32 uResGuid;
   BOS_UINT32 uStunHdl;
   NSNATSTUN  eType;
   
} NSNATSTUNBLK;

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */
class NsNatKa;

/*
** This class defines the STUN element.
*/
class NsNatStun
{
   public:

      friend class NsNatKa;

      NsNatStun( NsNatKa *gpNatKa );

      virtual ~NsNatStun();

      static void nsNatStunBind( BOS_UINT32 uResGuid,
                                 BOS_UINT32 uSockGuid,
                                 NSNATSTUN eType,
                                 BOS_UINT32 uMaxTx );

      static BOS_BOOL nsNatStunIsStun( BOS_UINT8 *puData,
                                       BOS_UINT32 uLen );

      static void nsNatStunProcess( BOS_UINT32 uSockGuid,
                                    BOS_UINT8 *puData,
                                    BOS_UINT32 uLen );

   private:

      static NSNATSTUNBLK *mpNsNatReq[NS__NAT_STUN__MAX_CONCURENT_REQUESTS];
      static NsNatKa *mpNsNatKa;

      /*---- Callback interface from the STUN module.
      */
      static void EvStunSendPtk( const BOS_UINT32      sid,
                                 const STUN_TRANSPORT  *pDest,
                                 const BOS_UINT8       *pData,
                                 const BOS_UINT16      len );

      static void EvStunRand32( BOS_UINT32 *pRand );

      static void EvStunHmac( const STUN_HMAC_TYPE alg,
                              const STUN_DATA      *pKey,
                              const STUN_DATA      *pData,
                              STUN_DATA            *pHmac );

      static BOS_UINT32 EvStunCrc32( const BOS_UINT8   *pData,
                                     const BOS_UINT16  len );

      static void EvStunLog( const char *format, ... );

      static void EvStunCb( const BOS_UINT32 reqHdl,
                            const STUNC_EVT_DATA *pEvtData );
};


/* ---- Inline Functions -------------------------------------------------------- */

#endif /* __NS_NAT_STUN__H__INCLUDED__ */
