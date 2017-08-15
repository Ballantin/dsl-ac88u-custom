/**********************************************************************************
** Copyright (c) 2007-2012 Broadcom
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
**      This file defines the CCTK Framework Class.
**
***********************************************************************************/
#ifndef __CCTKFW__H__INCLUDED__
#define __CCTKFW__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

MX_NAMESPACE_START(MXD_GNS)

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

class CctkFramework : public CEventDriven
{
/* ---- Constants and Types ----------------------------------------------------- */
protected:

   typedef enum MFW_MSGID
   {
      eMFW_MSGID_INTMSG = eTKM_MSGID_MAX,    /* Internal message */

      eMFW_MSGID_MAX
   } MFW_MSGID;

/* ---- Function Prototypes ----------------------------------------------------- */
public:
   CctkFramework();
   ~CctkFramework();

   /* Asynchonous functions */
   CCTKSTATUS reset( CCTKRSTMODE eRstMode );
   CCTKSTATUS config( const CCTKCFG *pCfg );
   CCTKSTATUS setParm( BOS_UINT32 uId, CCTKPARM eParm, const void *pVal );
   CCTKSTATUS userAdd( BOS_UINT32 uUid, const CCTKUSRCFG *pCfg );
   CCTKSTATUS userRegister( BOS_UINT32 uUid );
   CCTKSTATUS userUnregister( BOS_UINT32 uUid, BOS_BOOL bClear );
   CCTKSTATUS userRemove( BOS_UINT32 uUid );
   CCTKSTATUS eventCreate( BOS_UINT32 uEid, 
                           BOS_UINT32 uUid, 
                           CCTKSUBEVT eSubtype,
                           BOS_UINT32 uExpTime,
                           const BOS_UINT8 *pcTo );
   CCTKSTATUS eventSubscribe( BOS_UINT32 uEid );
   CCTKSTATUS eventUnsubscribe( BOS_UINT32 uEid );
   CCTKSTATUS eventDelete( BOS_UINT32 uEid );
   CCTKSTATUS callCreate( BOS_UINT32 uCid, BOS_UINT32 uUid, const BOS_UINT8 *pcTo );
   CCTKSTATUS callMake( BOS_UINT32 uCid );
   CCTKSTATUS callAnswer( BOS_UINT32 uCid );
   CCTKSTATUS callForward( BOS_UINT32 uCid, const BOS_UINT8 *pcFwdTo );
   CCTKSTATUS callHold( BOS_UINT32 uCid );
   CCTKSTATUS callUnhold( BOS_UINT32 uCid );
   CCTKSTATUS callTransfer( BOS_UINT32 uCid, BOS_UINT32 uXcid, BOS_BOOL bNoCallTermination );
   CCTKSTATUS callTransferBlind( BOS_UINT32 uCid, const BOS_UINT8 *pcAddr );
   CCTKSTATUS callMediaNotify( BOS_UINT32 uCid, CCTKMUPDRSN eReason );
   CCTKSTATUS callDelete( BOS_UINT32 uCid, CCTKCALLDELRSN eReason );
   CCTKSTATUS callDtmfInfo( BOS_UINT32 uCid, BOS_UINT32 uDtmf, BOS_UINT32 uDur );
   CCTKSTATUS callFlashInfo( BOS_UINT32 uCid,
                             const BOS_UINT8 *pcHfMimeType,
                             const BOS_UINT8 *pcHfContent );
   CCTKSTATUS callPrivInfo( BOS_UINT32 uCid, const CCTKPRIVCFG *pPrivCfg );
#if CCTKCFG_REMOTE_USER_CONTROL
   CCTKSTATUS callProxy( BOS_UINT32 uCid, BOS_UINT32 uUid );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
#if CCTKCFG_MESSAGE_PASS_THROUGH
   CCTKSTATUS passThruMsg( BOS_UINT32 uResGuid,
                           BOS_UINT32 uUsrGuid,
                           const BOS_UINT8 *pcTo,
                           const BOS_UINT8 *pcBody,
                           const BOS_UINT32 uSize );
   CCTKSTATUS passThruRsp( BOS_UINT32 uResGuid, BOS_UINT32 uFinalRsp );
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */
   CCTKSTATUS kaSuccess( BOS_UINT32 uUid, TKMTRANSPORT eType, BOS_UINT8 *puAddr );
   CCTKSTATUS kaFailure( BOS_UINT32 uUid );
   CCTKSTATUS infoPublish( BOS_UINT32 uUsrGuid,
                           BOS_UINT32 uResGuid,
                           CCTKPUBLISH eType,
                           const BOS_UINT8 *pcTo );
   CCTKSTATUS serviceControl( BOS_UINT32 uUsrGuid,
                              CCTKSUBEVT eType,
                              BOS_BOOL bEnable );

   CCTKSTATUS postMsg( BOS_BOOL bWait,
                       BOS_UINT32 uId,
                       void *pData,
                       TKMMSGCB msgCb );
   CCTKSTATUS startTimer( BOS_UINT32 uId,
                          BOS_UINT32 uMsec,
                          BOS_BOOL bRecurring,
                          void *pData,
                          TKMMSGCB tmrCb );
   CCTKSTATUS stopTimer( BOS_UINT32 uId );

#if CCTKCFG_MEMORY_MONITORING && CCTKCFG_DEBUG
   CCTKSTATUS showMemStatus( BOS_BOOL bCctkOnly, BOS_BOOL bTraceTable );
#endif

#if CCTKCFG_APPLICATION_STATUS
   CCTKSTATUS showStatus();
#endif

   void release( CBinarySemaphore *pSem );

   /* Synchronous functions */
   CCTKSTATUS startUp( void );
   CCTKSTATUS shutDown( void );
   CCTKSTATUS getInitStatus( void );
   CCTKSTATUS getParm( BOS_UINT32 uId, CCTKPARM eParm, void *pVal );
   CCTKSTATUS setParmHelper( BOS_UINT32 uId, CCTKPARM eParm, const void *pVal );

   void internalResetCont( void );

protected:
   /* Asynchronous functions */
   void internalReset( CMarshaler *pParms );
   void internalConfig( CMarshaler *pParms );
   void internalSetParm( CMarshaler *pParms );
   void internalUserAdd( CMarshaler *pParms );
   void internalUserRegister( CMarshaler *pParms );
   void internalUserUnregister( CMarshaler *pParms );
   void internalUserRemove( CMarshaler *pParms );
   void internalEventCreate( CMarshaler *pParms );
   void internalEventSubscribe( CMarshaler *pParms );
   void internalEventUnsubscribe( CMarshaler *pParms );
   void internalEventDelete( CMarshaler *pParms );
   void internalCallCreate( CMarshaler *pParms );
   void internalCallMake( CMarshaler *pParms );
   void internalCallAnswer( CMarshaler *pParms );
   void internalCallForward( CMarshaler *pParms );
   void internalCallHold( CMarshaler *pParms );
   void internalCallUnhold( CMarshaler *pParms );
   void internalCallTransfer( CMarshaler *pParms );
   void internalCallTransferBlind( CMarshaler *pParms );
   void internalCallMediaNotify( CMarshaler *pParms );
   void internalCallDelete( CMarshaler *pParms );
   void internalCallDtmfInfo( CMarshaler *pParms );
   void internalCallFlashInfo( CMarshaler *pParms );
   void internalCallPrivInfo( CMarshaler *pParms );
#if CCTKCFG_REMOTE_USER_CONTROL
   void internalCallProxy( CMarshaler *pParms );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
#if CCTKCFG_MESSAGE_PASS_THROUGH
   void internalPassThruMsg( CMarshaler *pParms );
   void internalPassThruRsp( CMarshaler *pParms );
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */
   void internalKaSuccess( CMarshaler *pParms );
   void internalKaFailure( CMarshaler *pParms );
   void internalInfoPublish( CMarshaler *pParms );
   void internalServiceControl( CMarshaler *pParms );

   void internalPostMsg( CMarshaler *pParms );

#if CCTKCFG_MEMORY_MONITORING && CCTKCFG_DEBUG
   void internalShowMemStatus( CMarshaler *pParms );
#endif
#if CCTKCFG_APPLICATION_STATUS
   void internalShowStatus( CMarshaler *pParms );
#endif

   /* Synchronous functions */
   void internalGetParm( CMarshaler *pParms );

   /* CEventDriven */
   virtual void EvMessageServiceMgrAwaken( bool bWaitingCompletion,
                                           unsigned int uMessage,
                                           CMarshaler *pParameter );

   virtual void EvTimerServiceMgrAwaken( bool bStopped,
                                         unsigned int uTimer,
                                         mxt_opaque opq );

private:
   /* Helper functions */
   void getId( BOS_UINT32 uId, BOS_UINT32 &ruUsrId, BOS_UINT32 &ruCallId );

/* ---- Variables --------------------------------------------------------------- */
private:
   /* Framework initialization status */
   CCTKSTATUS meInitStatus;

   /* Semaphore for synchronizing the destruction of the CCTK Main Framework object
   ** and the application.
   */
   CBinarySemaphore *mpReleaseSem;
};

/* ---- Inline Functions -------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   getInitStatus
**
**  PURPOSE:    Retrieve initialization status.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
inline CCTKSTATUS CctkFramework::getInitStatus( void )
{
    return meInitStatus;
}

/***********************************************************************************
**  FUNCTION:   release
**
**  PURPOSE:    Release the CCTK Main Framework object.
**
**  PARAMETERS:
**              pSem - Application specified semaphore pointer used to ensure
**                     the CCTK Main Framework object is completely released.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
inline void CctkFramework::release( CBinarySemaphore *pSem )
{
   mpReleaseSem = pSem;
   Release();
}

MX_NAMESPACE_END(MXD_GNS)

#endif /* __CCTKFW__H__INCLUDED__ */
