/**********************************************************************************
** Copyright © 2007-2009 Broadcom
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
**      This file defines the Configuration Engine Class.
**
***********************************************************************************/
#ifndef __CFG_ENGINE__H__INCLUDED__
#define __CFG_ENGINE__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

MX_NAMESPACE_START(MXD_GNS)

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

class CfgEngine : public ISceShutdownMgr,
                  public ISipCoreUser,
                  public ISipDataLogger,
                  public ISipPersistentConnectionMgr
{
/* ---- Constants and Types ----------------------------------------------------- */
public:
   /* Packet logging function ID */
   typedef enum CFG_PKTLOGFUNCID
   {
      eCFG_PKTLOGFUNCID_RAW,           /* LogRawData */
      eCFG_PKTLOGFUNCID_SIP            /* LogSipPacket */
   } CFG_PKTLOGFUNCID;

protected:
   /* Transport base opaque ID */
   static const BOS_SINT16 CFGMAIN_TRANSPORTID_BASE = 0x0100;

   /* Configuration states */
   typedef enum CFG_STATE
   {
      eCFG_STATE_NOTCFGED,             /* Not configured */
      eCFG_STATE_CFGING,               /* Configuration in progress */
      eCFG_STATE_CFGED,                /* Configured */
      eCFG_STATE_DECFGING,             /* De-configuration in progress */
      eCFG_STATE_MAX
   } CFG_STATE;

   /* Opaque ID passed to the transport layer */
   typedef enum CFG_TRANSPORTID
   {
      eCFG_TRANSPORTID_LISTEN_UDP = CFGMAIN_TRANSPORTID_BASE,
                                       /* Listening to UDP */
      eCFG_TRANSPORTID_LISTEN_TCP,     /* Listening to TCP */
      eCFG_TRANSPORTID_LISTEN_TLS,     /* Listening to TLS */
      eCFG_TRANSPORTID_STOP_UDP,       /* Stop listening to UDP */
      eCFG_TRANSPORTID_STOP_TCP,       /* Stop listening to TCP */
      eCFG_TRANSPORTID_STOP_TLS        /* Stop listening to TLS */
   } CFG_TRANSPORTID;

private:
   /* Socket state condition bit starting position */
   static const BOS_UINT32 CFGMAIN_SOCKETSTATE_COND_BITPOS = 16;
   
   /* Socket status */
   typedef enum CFG_SOCKETSTATE
   {
      /* -- State bits -- */
      eCFG_SOCKETSTATE_CLOSED    = 0,           /* Socket closed */
      eCFG_SOCKETSTATE_OPENING   = ( 1 << 0 ),  /* Socket opening */
      eCFG_SOCKETSTATE_OPENED    = ( 1 << 1 ),  /* Socket opened */
      eCFG_SOCKETSTATE_CLOSING   = ( 1 << 2 ),  /* Socket closing */

      /* -- State condition bits (used by persistent socket only) -- */
      /* Persistent socket retrying */
      eCFG_SOCKETSTATE_RETRYING  = ( 1 << ( CFGMAIN_SOCKETSTATE_COND_BITPOS + 0 ) ),
      
      /* Persistent socket sharing with the listening socket */
      eCFG_SOCKETSTATE_SHARING   = ( 1 << ( CFGMAIN_SOCKETSTATE_COND_BITPOS + 1 ) ),

      /* Duplicated persistent socket */
      eCFG_SOCKETSTATE_DUPLCED   = ( 1 << ( CFGMAIN_SOCKETSTATE_COND_BITPOS + 2 ) )
   } CFG_SOCKETSTATE;

   /* Socket information */
   typedef struct CFG_SOCKETINFO
   {
      mxt_opaque        socketId;      /* Stack provided socket identifier */
      CFG_SOCKETSTATE   eState;        /* Socket status */
   } CFG_SOCKETINFO;

   /* Persistent socket information */
   typedef struct CFG_PSTSOCKETINFO
   {
      CFG_SOCKETINFO    sock;
      BOS_UINT32        uTmrId;
   } CFG_PSTSOCKETINFO;

   /* Transport information */
   typedef struct CFG_TRANSPORTINFO
   {
      CFG_SOCKETINFO    listen;        /* Listening socket info */
      CFG_PSTSOCKETINFO persist[CFG_MAX_USR];
                                       /* Persistent socket info */
   } CFG_TRANSPORTINFO;

   /* Connection information */
   typedef struct CFG_CNXINFO
   {
      CFG_TRANSPORTINFO udp;           /* UDP transport info */
      CFG_TRANSPORTINFO tcp;           /* TCP transport info */
#if CCTKCFG_TLS
      CFG_TRANSPORTINFO tls;           /* TLS transport info */
#endif
   } CFG_CNXINFO;

   /* Local address information */
   typedef struct CFG_LOCADDRINFO
   {
      mxt_opaque        addrId;        /* Stack specific local address identifier */
      CFG_CNXINFO       cnx;           /* Connection info */
   } CFG_LOCADDRINFO;

   /* Persistent connection events */
   typedef enum CFG_PERSISTCNXEVT
   {
      eCFG_PERSISTCNXEVT_ESTABLISHED,  /* Active connection is established */
      eCFG_PERSISTCNXEVT_TERMINATED,   /* Active connection is terminated */
      eCFG_PERSISTCNXEVT_TERMINATING   /* Active connection is terminating */
   } CFG_PERSISTCNXEVT;

/* ---- Function Prototypes ----------------------------------------------------- */
public:
   /* Constructors / Destructors */
   CfgEngine();
   ~CfgEngine();

   /* Config Engine API functions */
   CFGSTATUS startUp( CEventDriven *pMainFramework );
   CFGSTATUS shutDown( void );
   CFGSTATUS config( void );
   CFGSTATUS deConfig( void );
   CFGSTATUS modifySys( CCTKPARM eParm );
   CFGSTATUS addUsr( BOS_UINT32 uIdx );
   CFGSTATUS modifyUsr( CCTKPARM eParm, BOS_UINT32 uIdx );
   CFGSTATUS removeUsr( BOS_UINT32 uIdx );
   CFGSTATUS modifyCall( CCTKPARM eParm, BOS_UINT32 uIdxUsr, BOS_UINT32 uIdxCall );
   CFGSTATUS addRealm( BOS_UINT32 uIdx, const BOS_UINT8 *pcRealm );
   CFGSTATUS getInitStatus( void );
   CUdpSocket *getPktLogger( void );
   BOS_BOOL isFuncAllowed( TKM_MSGID eFuncId );
   void handlePstCnxTimeout( BOS_UINT32 uTmrId, mxt_opaque cnxOpaque );

protected:
   /* ISceShutdownMgr */ 
   void EvOnShutdownCompleted( mxt_opaque opqShutdown );

   /* ISipCoreUser */
   void EvOnPacketReceived( const CSipPacket& rPacket );
   void EvShutdownCompleted();
   void EvCommandResult( mxt_result res, mxt_opaque opq );

   /* ISipDataLogger */ 
   void LogRawData( EDirection eDir,
                    const CSocketAddr &rLocalAddr,
                    const CSocketAddr &rPeerAddr,
                    const CBlob &rRawData,
                    unsigned int uSize );

   void LogSipPacket( EDirection eDir, 
                      const CSipPacket& rPacket );

   /* ISipPersistentConnectionMgr */
   void EvConnectionEstablished( mxt_opaque opqConnection, 
                                 const CSocketAddr &rLocalAddr,
                                 const CSocketAddr &rPeerAddr );
   void EvConnectionTerminated( mxt_opaque opqConnection );
   void EvErrorOnConnection( mxt_opaque opqConnection,
                             mxt_result resConnectionError );
   void EvSipErrorOnConnection( mxt_opaque opqConnectionHandle, 
                                const CSipPacket& rSentPacket,
                                mxt_result resConnectionError );
private:

#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   /* Trace helper functions */
   void initTrace( void );
   void configTrace( void );
   void disableTrace( void );
#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
   void setTraceLevel( BOS_UINT16 uLevel );
   void setTraceGroup( BOS_UINT16 uGroup );
#endif
#endif

   /* System configuration helper functions */
   CFGSTATUS configStack( void );
   CFGSTATUS deConfigStack( BOS_BOOL bShutdown );
#if CCTKCFG_TLS
   CFGSTATUS configTlsContext( void );
#endif
   CFGSTATUS configMxFramework( void );
   CFGSTATUS deConfigMxFramework( BOS_BOOL bShutdown );
   CFGSTATUS configSce( void );
   CFGSTATUS configSipUa( void );
   CFGSTATUS configSipUaCnx( void );
   CFGSTATUS configSipUaIdHdr( void );
   CFGSTATUS configSipUaRoutingHdr( void );
   CFGSTATUS configSipUaSigBehave( void );
   CFGSTATUS configSipUaInterop( void );
   CFGSTATUS configSipUaDns( void );
   CFGSTATUS configSipUaTransTmr( void );
   CFGSTATUS configSipUaNat( void );
   CFGSTATUS deConfigSipUa( BOS_BOOL bShutdown );
   CFGSTATUS configLocAddr( void );
   CFGSTATUS deConfigLocAddr( void );
   CFGSTATUS openListeningPort( void );
   CFGSTATUS closeListeningPort( BOS_UINT32 uTransport = CCTK_TRANSPORT_ALL );
   void updCfgState( void );
   void closePktLogger( void );
   void closeUdpSocket( CUdpSocket *&rpSocket );

   /* User configuration helper functions */
   CFGSTATUS configSceUsrMain( BOS_UINT32 uIdx );
   CFGSTATUS configSceUsrGruu( BOS_UINT32 uIdx );
   CFGSTATUS configSceUsrAuth( BOS_UINT32 uIdx );
   CFGSTATUS configSceUsrSec( BOS_UINT32 uIdx );
   CFGSTATUS configSceUsrId( BOS_UINT32 uIdxUsr,
                             const BOS_UINT32 *pIdxCall = NULL );
   CFGSTATUS configSceUsrUaCap( BOS_UINT32 uIdx );
   CFGSTATUS configSceUsrReg( BOS_UINT32 uIdx );
   CFGSTATUS configSceUsrCall( BOS_UINT32 uIdx );
   CFGSTATUS configSceUsrMwi( BOS_UINT32 uIdx );
   CFGSTATUS configPersistCnx( BOS_UINT32 uIdx );
   CFGSTATUS openPersistCnx( BOS_UINT32 uIdx );
   CFGSTATUS closePersistCnx( BOS_UINT32 uIdx, 
                              BOS_UINT32 uTransport = CCTK_TRANSPORT_ALL,
                              BOS_BOOL bAllCnx = BOS_FALSE );
   void closePersistCnx( CSipPersistentConnectionList *pCnxList,
                         CFG_TRANSPORTINFO &rPortInfo,
                         BOS_UINT32 uUsrIdx,
                         BOS_UINT32 uTransport,
                         BOS_BOOL bAllCnx );
   void updateDupPcOnActEvt( CFG_PERSISTCNXEVT eEvt,
                             const CFG_TRANSPORTINFO &rPortInfo,
                             BOS_UINT32 uUsrIdx );
   void updateStackPcProperty( CFG_PERSISTCNXEVT eEvt,
                               mxt_opaque socketId,
                               BOS_UINT32 uUsrIdx );
   CFGSTATUS handleDupPcOnEst( CFG_TRANSPORTINFO &rDupPortInfo,
                               BOS_UINT32 uDupUsrIdx,
                               BOS_UINT32 uDupTransport );
   CFGSTATUS handleDupPcOnErrEvt( CFG_TRANSPORTINFO &rDupPortInfo,
                                  BOS_UINT32 uDupUsrIdx,
                                  BOS_UINT32 uDupTransport );
   CFG_TRANSPORTINFO *findPersistCnxIdx( BOS_BOOL bFindDup,
                                         mxt_opaque socketId,
                                         BOS_UINT32 &ruAddrIdx, 
                                         BOS_UINT32 &ruUsrIdx,
                                         BOS_UINT32 &ruTransport );
   BOS_BOOL isAnySharingPersistCnx( BOS_UINT32 uAddrIdx );
   BOS_BOOL isAnyPersistCnxInState( BOS_UINT32 uUsrIdx, CFG_SOCKETSTATE eState );
   BOS_BOOL isAnyPersistCnxUsed( BOS_UINT32 uUsrIdx );
   void handleDeferredEvtUserAdd( BOS_UINT32 uUsrIdx );
   void handleDeferredEvtUserRemove( BOS_UINT32 uUsrIdx );
   
   /* Pre-configuration helper functions */
   void preCfgUaId( const CCTKUAIDHDR &rCfg, 
                    CString &rUaName, 
                    CString &rSvrName,
                    BOS_BOOL &rbAddStackVer );
   void preCfgCnx( const CCTKCNXCFG &rCfg, 
                   BOS_UINT32 &ruMinCnx, 
                   BOS_UINT32 &ruMaxCnx,
                   BOS_UINT32 &ruMaxIdleTime );
   void preCfgTransTmr( const CCTKTRANSTMRCFG &rCfg, 
                        BOS_UINT32 &ruT1, 
                        BOS_UINT32 &ruT2, 
                        BOS_UINT32 &ruTimeout, 
                        BOS_UINT32 &ruT4, 
                        BOS_UINT32 &ruB, 
                        BOS_UINT32 &ruD,
                        BOS_UINT32 &ruF,
                        BOS_UINT32 &ruH,
                        BOS_UINT32 &ruJ );
   CUdpSocket *preCfgPktLog( const CCTKPKTLOG &rCfg );
   CFGSTATUS preCfgLocAddr( 
      const CCTKHOSTADDR &rCfg, 
      CSocketAddr &rLocAddr,
      CVector< CString > *&rpFqdn,
      CVector< ISipCoreConfig::SAccessibleNetwork > *&rpAccNet );
   CFGSTATUS preCfgCredentials( const CCTKRLM &rCfg,
                                CString &rRealm,
                                CString &rUser,
                                CString &rCredit );
   CFGSTATUS preCfgCredentials( const CFGUEUSR &rCfg,
                                const BOS_UINT8 *pcRealm,
                                CString &rRealm,
                                CString &rUser,
                                CString &rCredit );
   CNameAddr *preCfgPrefId( const CFGUSRXTL &rCfgUsr, const CFGCALLXTL &rCfgCall );
   CNameAddr *preCfgUsrAddr( BOS_UINT32 uIdx );
   CSipUri *preCfgPubGruu( BOS_UINT32 uIdx );
   CSipHeader *preCfgForcedContact( BOS_UINT32 uIdx );

   /* Other helper functions */
   CFGSTATUS configSipTcpListenerTos( BOS_UINT8 uTos );   
   CNameAddr *getContactFromLocAddr( const CFGUSRXTL &rCfg );

/* ---- Variables --------------------------------------------------------------- */
private:
   /* NOTE: The following member variables can only be accessed/modified in the
   **       application's task context, or in other task context synchronous to
   **       the application's task context.
   */

   /* Core thread pointer */
   IActivationService *mpCoreThread;

   /* CCTK Main Framework handle */
   CEventDriven *mpMainFramework;

   /* Semaphore for shutdown synchronization */
   CBinarySemaphore mShutdownSem;

   /* NOTE: The following member variables can only be accessed/modified by the
   **       stack's core thread.
   */

   /* Config engine initialization status */
   CFGSTATUS meInitStatus;

   /* Configuration state */
   CFG_STATE meCfgState;

   /* Packet logger */
   CUdpSocket *mpPktLogger;

   /* Local address information */
   CFG_LOCADDRINFO mLocAddr[CCTKCFG_MAX_HOSTADDR];
   
   /* TOS for the SIP TCP Listening Socket */
   BOS_UINT8 muTcpListenSocketHighestTos; 
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
**  RETURNS:    Return status.
**
**  NOTES:      None
***********************************************************************************/
inline CFGSTATUS CfgEngine::getInitStatus( void )
{
    return meInitStatus;
}

/***********************************************************************************
**  FUNCTION:   getPktLogger
**
**  PURPOSE:    Retrieve the packet logger socket.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Packet logger reference.
**
**  NOTES:      None
***********************************************************************************/
inline CUdpSocket *CfgEngine::getPktLogger( void )
{
   return mpPktLogger;
}

MX_NAMESPACE_END(MXD_GNS)

#endif /* __CFG_ENGINE__H__INCLUDED__ */
