/**********************************************************************************
** Copyright (c) 2008-2011 Broadcom
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
**      This file defines the Keep Alive module behavior.
**
***********************************************************************************/
#ifndef __NS_NAT_KA__H__INCLUDED__
#define __NS_NAT_KA__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Constants and Types ----------------------------------------------------- */
typedef struct
{
   BOS_UINT32  uResGuid;
   BOS_UINT32  uKaGuid;
   BOS_UINT32  uTmrId;
   BOS_BOOL    bKaSent;
   BOS_UINT32  uBackOffTmr;
   BOS_UINT32  uMaxTx;     /* Max number of transmissions before KA failure */

   BOS_UINT16  uPort;
   BOS_UINT8   *puAddr;

} NSNATKABLK;


/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */
class NsNatStun;
class MXD_GNS::IAsyncSocket;

/*
** This class defines the keep alive element.
**
** NOTE: This class runs on the stack's transport thread. Data exchange between
**       the core (CCTK) and the transport threads needs synchronization.
*/
class NsNatKa : public MXD_GNS::CBrcmTransportSvc
{
public:

#if CCTKCFG_KEEP_ALIVE
   friend class NsNatStun;
#endif /* CCTKCFG_KEEP_ALIVE */

   typedef enum NSNATKA_MSGID
   {
      /* Asynchronous NS NAT Keep-Alive API functions */
      eNSNATKA_MSGID_START,
      eNSNATKA_MSGID_STOP,
      eNSNATKA_MSGID_SUCCESS,
      eNSNATKA_MSGID_FAILURE,
      eNSNATKA_MSGID_SEND,
      eNSNATKA_MSGID_ENDTIMER,

      eNSNATKA_MSGID_MAX,
   } NSNATKA_MSGID;

   /* Constructor */
   NsNatKa();

#if CCTKCFG_KEEP_ALIVE
   /* Asynchronous functions */
   BOS_BOOL    nsNatKaStartA( BOS_UINT32 uResGuid, MXD_GNS::CSocketAddr *pPeerAddr );
   BOS_BOOL    nsNatKaStopA( BOS_UINT32 uResGuid );

   void        nsNatKaFailureA( BOS_UINT32 uResGuid );
   void        nsNatKaSuccessA( BOS_UINT32 uResGuid );

   void        nsNatKaSendA( const BOS_UINT32 uSockGuid,
                             const BOS_UINT8 *puData,
                             const BOS_UINT32 uLen );

   /* Synchronous functions */
   CCTKSTATUS  nsNatKaEndTimer( void );
   BOS_UINT32  nsNatKaBackOff( BOS_UINT32 uGuid );
#endif /* CCTKCFG_KEEP_ALIVE */

protected:

   /* Destructor */
   virtual     ~NsNatKa();

#if CCTKCFG_KEEP_ALIVE
   void        nsNatKaTick( BOS_UINT32 uTimerId );
   void        nsNatKaFailure( BOS_UINT32 uGuid );
   void        nsNatKaSuccess( BOS_UINT32 uGuid );

   TKMTRANSPORT nsNatKaTransportType( MXD_GNS::IAsyncSocket *pSocket );
   void        nsNatKaReceived( BOS_UINT32 uResGuid, BOS_UINT32 uKaGuid );

   MXD_GNS::IAsyncSocket* nsNatKaSocket( BOS_UINT32 uGuid,
                                         TKMTRANSPORT eTrans = eTKMTRANSPORT_MAX );

   void        nsNatKaSetTmr( BOS_UINT32 uGuid );
   NSNATKABLK *nsNatKaBlk( BOS_UINT32 uKaGuid );

   /* Internal functions */
   void        internalNsNatKaStartA( MXD_GNS::CMarshaler *pParms );
   void        internalNsNatKaStopA( MXD_GNS::CMarshaler *pParms );
   void        internalNsNatKaFailureA( MXD_GNS::CMarshaler *pParms );
   void        internalNsNatKaSuccessA( MXD_GNS::CMarshaler *pParms );
   void        internalNsNatKaSendA( MXD_GNS::CMarshaler *pParms );
   void        internalNsNatKaEndTimer( void );
#endif /* CCTKCFG_KEEP_ALIVE */

   /* CBrcmTransportSvc */
   void        ReceivedFromNetwork( MXD_GNS::CSipPacket *pPacket, 
                                    MXD_GNS::CSipClientSocket *pSocket );
   mxt_result  SendToNetwork( MXD_GNS::CSipPacket &rPacketOut,
                              MXD_GNS::CSipClientSocket *pSource,
                              MXD_GNS::ISipTransportUser *pUser,
                              mxt_opaque opq );
   virtual void EvMessageServiceMgrAwaken( bool bWaitingCompletion,
                                           unsigned int uMessage,
                                           MXD_GNS::CMarshaler *pParameter );
   virtual void EvTimerServiceMgrAwaken( bool bStopped,
                                         unsigned int uTimer,
                                         mxt_opaque opq );

private:

#if CCTKCFG_KEEP_ALIVE
   NSNATKABLK  *mpNsNatUsr[ ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR)) ];
#endif /* CCTKCFG_KEEP_ALIVE */
};


/* ---- Inline Functions -------------------------------------------------------- */


#endif /* __NS_NAT_KA__H__INCLUDED__ */
