/**********************************************************************************
** Copyright © 2008 - 2009 Broadcom
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
**      This file defines the Payload Modifier Observer main.
**
***********************************************************************************/
#ifndef __PM_OBSERVER__H__INCLUDED__
#define __PM_OBSERVER__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>


/* ---- Constants and Types ----------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

/*
** This class defines the Call Engine main.
*/
class PmObserver : public MXD_GNS::ISceSipPacketObserver
{
   public:

      PmObserver( BOS_BOOL bRteHdrOverwriteFqdn );
      ~PmObserver();

      BOS_UINT32 lastChance( BOS_UINT8 **pNewData,
                             CCTKIPPORT *destInfo,
                             const BOS_UINT8 *pData,
                             BOS_UINT32 uSize );

   protected:

      /* ---- ISceSipPacketObserver
      */
      void    EvSendingRequest(
                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                 INOUT MXD_GNS::CSipPacket& rPacketOut );

      void    EvSendingResponse(
                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                 INOUT MXD_GNS::CSipPacket& rPacketOut );

      void    EvReceivedRequest(
                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                 IN const MXD_GNS::CSipPacket& rPacketIn );

      void    EvReceivedResponse(
                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                 IN const MXD_GNS::CSipPacket& rPacketIn );

   private:

      static  const BOS_UINT8 *puPktBuffer;
      
      /* This flag controls the feature 'Remove FQDN from top-most Route header'
      ** which is required for interoperability with some proxy.  The reason why
      ** this feature is statically defined as part of the observer is that this
      ** particular feature can only be applied within the context of the socket
      ** sender thread which typically cannot access the dynamic configuration at
      ** that time.  Therefore one cannot read the configuration to knwo whether
      ** to apply the feature, so we just hard-code the feature support.  Note
      ** that therefore this feature cannot be configured run time and requires
      ** a restart of the CCTK application for any change to it.
      */
      static  BOS_BOOL mbRteHdrOverwriteFqdn;

      void    insertGenHdr( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt );
      void    insertFeatHdr( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt );
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
      void    updatePktIms( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt );
#endif
      void    updatePktProperty( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt );
      void    insertFeatBody( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt );

};


/* ---- Variable Externs -------------------------------------------------------- */
extern PmObserver *gpPmObserver;


/* ---- Inline Functions -------------------------------------------------------- */


#endif /* __PM_OBSERVER__H__INCLUDED__ */
