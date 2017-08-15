/**********************************************************************************
** Copyright (c) 2009-2013 Broadcom Corporation
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
**      This file implements the Configuration Engine Class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cfgEngine.h>
#include <cfgMain.h>

MX_NAMESPACE_START(MXD_GNS)

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
/* Maximum shutdown pending time (in ms) */
const BOS_UINT64 CFGENGINE_MAX_SHUTDOWN_WAIT_TIME = 30000;   /* 30 seconds */

/* Maximum payload size (in bytes) */
const BOS_UINT32 CFGENGINE_MAX_PAYLOAD_SIZE = 8192;

/* Maximum packet logger header size (in bytes) */
const BOS_UINT32 CFGENGINE_MAX_PKTLOG_HDR_SIZE = 128;

/* Default packet header size (in bytes) */
const BOS_UINT32 CFGENGINE_DEF_PKT_HDR_SIZE = 1024;

/* Minimum number of connection alive */
const BOS_UINT32 CFGENGINE_MIN_CNX = 5;

/* Maximum number of simultaneous connections */
const BOS_UINT32 CFGENGINE_MAX_CNX = 10;

/* Maximum connection idle time */
const BOS_UINT32 CFGENGINE_MAX_CNX_IDLE_TIME = 64;

/* Invalid IP address type */
const int CFGENGINE_INVALID_IP_TYPE = -1;

/* Persistent connection retry delay (in ms) */
const BOS_UINT32 CFGENGINE_PST_CNX_RETRY_DELAY = 2000;

/* Persistent connection max retry period (in ms) */
const BOS_UINT32 CFGENGINE_PST_CNX_MAX_RETRY_PERIOD = 480000;

/* Persistent connection retry timeout value (in ms) */
const BOS_UINT32 CFGENGINE_PST_CNX_RETRY_TIMEOUT = 6000;

/* Default registration expiration time */
const BOS_UINT32 CFGENGINE_DEF_REG_EXP_TIME = 3600;

/* Default registration state subscription expiration time */
const BOS_UINT32 CFGENGINE_DEF_REG_STATE_SUB_EXP_TIME = 3761;

/* Default MWI expiration time */
const BOS_UINT32 CFGENGINE_DEF_MWI_EXP_TIME = 3600;

/* Default authentication loop threshold */
const BOS_UINT32 CFGENGINE_DEF_AUTH_LOOP_THRESHOLD = 2;

/* Default length of port string */
#define CFGENGINE_PORT_LEN 7

/* Quad-zeros string */
const BOS_UINT8 *CFGENGINE_QUAD_ZEROS_STR =
   reinterpret_cast< const BOS_UINT8* >( "0.0.0.0" );

/* ---- Private Variables ------------------------------------------------------- */
MX_NAMESPACE_END(MXD_GNS)

/* Config Engine instance */
static MX_NS CfgEngine *gpCfgEngine = NULL;

/* Return status of the core thread related resource cleanups.
** NOTE: This variable can only be accessed/modified in the application's task
**       context, or in other task context synchronous to the application's task
**       context.
*/
static CFGSTATUS geCleanupStatus = eCFGSTATUS_SUCCESS;

MX_NAMESPACE_START(MXD_GNS)

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */
MX_NAMESPACE_END(MXD_GNS)

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
extern void tkmMxTraceHandler( EMxTraceLevel eLevel,
                               uint32_t uTraceUniqueId,
                               const char *pszTrace,
                               int sMsgSize );
#endif

/* Callback functions */
static void pktLoggerCb( BOS_UINT32 uId, void *pData );
static void pktModifierCb( MX_NS CSipPacket &rPacket );
static void cleanCoreResCb( BOS_UINT32 uId, void *pData );
static void pstCnxTimeoutCb( BOS_UINT32 uId, void *pData );

MX_NAMESPACE_START(MXD_GNS)

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CfgEngine
**
**  PURPOSE:    Constructor of the Configuration Engine object.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    Nothing
**
**  NOTES:      The status of the Config Engine initialization can be retrieved
**              through getInitStatus().
***********************************************************************************/
CfgEngine::CfgEngine()
   : mpCoreThread( NULL ),
     mpMainFramework( NULL ),
     meCfgState( eCFG_STATE_NOTCFGED ),
     mpPktLogger( NULL ), 
     muTcpListenSocketHighestTos( 0 )
{
   /* No trace is added here because M5T trace utility has not been enabled yet.
   ** Trace can be added once CCTK custom tracing support is implemented.
   */

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   mxt_result res;

   /* Variable initialization */
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   gpCctkTraceCb = NULL;
#endif
   geCleanupStatus = eCFGSTATUS_SUCCESS;
   memset( &mLocAddr, 0, sizeof( mLocAddr ) );

   /* Initialize SDP Parser */
   res = CSdpParserInitializer::Initialize();
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CfgEngine: Error initializing SDP Parser (%d)", res );
      eStatus = eCFGSTATUS_NORSC;
   }

#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   /* Initialize trace support */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      initTrace();
   }
#endif

   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      gpCfgEngine = this;
   }

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
   CCTK_TRACED2( "CfgEngine(%p): Initialization status = %d.", this, eStatus );
#endif

   meInitStatus = eStatus;
}

/***********************************************************************************
**  FUNCTION:   ~CfgEngine
**
**  PURPOSE:    Destructor of the Configuration Engine object.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
CfgEngine::~CfgEngine()
{
   /* No trace is added here because M5T trace utility may not be enabled at this
   ** point. Trace can be added once CCTK custom tracing support is implemented.
   */

#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   /* Disable trace support */
   disableTrace();
#endif

   /* NOTE: "mpCoreThread" and "mpMainFramework" are accessed from the stack's core
   **       thread here because they are not expected to be changed during CCTK
   **       shutdown as the shutdown process is synchronous in the application's
   **       task context.
   */

   /* Final de-allocations, if necessary. Asserts are used because the items below
   ** should have already been de-allocated before this destructor is invoked.
   */
   CCTK_ASSERT( mpCoreThread == NULL );
   if ( mpCoreThread != NULL )
   {
      mpCoreThread->ReleaseIfRef();
      mpCoreThread = NULL;
   }
   CCTK_ASSERT( mpPktLogger == NULL );
   closeUdpSocket( mpPktLogger );

   /* NOTE: Since the Main Framework is not created by the Config Engine,
   **       "mpMainFramework" is left to the Main to be released.
   */
   mpMainFramework = NULL;

   /* Reset private variables */
   gpCfgEngine = NULL;

   /* Finalize SDP Parser */
   CSdpParserInitializer::Finalize();
}

/***********************************************************************************
**  FUNCTION:   config
**
**  PURPOSE:    Configure CCTK after startup.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::config( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - config()", this );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* This should have been checked by CCTK Framework before this function is
   ** called.  Check again for safety.
   */
   if ( !isFuncAllowed( eTKM_MSGID_CONFIG ) )
   {
      CCTK_TRACEI1( "CfgEngine - config: "
         "Failed to be called in state (%d).",
         meCfgState );
      eStatus = eCFGSTATUS_BADSTATE;
   }

   /* Start (re)configuration */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      if ( meCfgState == eCFG_STATE_CFGED )
      {
         /* Clean up stack configuration for re-configuration */
         eStatus = deConfigStack( BOS_FALSE );
         if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
         {
            /* Transition to "de-configuring" state */
            meCfgState = eCFG_STATE_DECFGING;
         }
         else
         {
            CCTK_TRACEI0(
               "CfgEngine - config: Error cleaning up stack config." );
         }
      }
      else /* meCfgState == eCFG_STATE_NOTCFGED */
      {
         /* Stack has not been configured yet. Start configuration immediately. */
         eStatus = configStack();
         if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
         {
            /* Transition to "configuring" state */
            meCfgState = eCFG_STATE_CFGING;
         }
         else
         {
            CCTK_TRACEI1(
               "CfgEngine - config: Error configuring stack (%d).",
               eStatus );
         }
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   deConfig
**
**  PURPOSE:    Prepare CCTK shutdown by de-configuring CCTK.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::deConfig( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - deConfig()", this );
   
   return deConfigStack( BOS_TRUE );
}

/***********************************************************************************
**  FUNCTION:   modifySys
**
**  PURPOSE:    Modify and re-configure the SCE system resource.
**
**  PARAMETERS:
**              eParm   - Parameter ID.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::modifySys( CCTKPARM eParm )
{
   CCTK_TRACED2( "CfgEngine(%p) - modifySys(%d)", this, eParm );

   /* This should have been checked by CCTK Framework before this function is
   ** called. Check again for safety.
   */
   if ( !isFuncAllowed( eTKM_MSGID_SETPARM ) )
   {
      CCTK_TRACEI1( "CfgEngine - modifySys: "
         "Failed to be called in state (%d).",
         meCfgState );
      return eCFGSTATUS_BADSTATE;
   }

   if ( !CCTK_IS_SYSPARM( eParm ) )
   {
      CCTK_TRACEI1( "CfgEngine - modifySys: Invalid parameter ID (%d).", eParm );
      return eCFGSTATUS_BADPARM;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Re-configure the SCE user config object */
   switch ( eParm )
   {
   case eCCTKPARM_EC_LOCPORT:
   case eCCTKPARM_EC_TRANSPORT:
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
   case eCCTKPARM_EC_IMSCFG:
#endif
   case eCCTKPARM_EC_GRUUCFG:
   case eCCTKPARM_EC_PKTLOG:
   case eCCTKPARM_EC_PANICFG:
#if CCTKCFG_ICE_LITE_SUPPORT
   case eCCTKPARM_EC_ICECTLCFG:
   case eCCTKPARM_EC_ICEIPLSTCFG:
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
#if CCTKCFG_REMOTE_USER_CONTROL
   case eCCTKPARM_EC_REMUSRSYS:
#endif /* CCTKCFG_REMOTE_USER_CONTROL */   
      {
         /* These config parameter types either:
         ** 1) Do not require SCE to be re-configured, or
         ** 2) Are not allowed to be modified at real-time after the SCE and
         **    the SIP stack start up.
         */
      }
      break;
#if CCTKCFG_TLS
   case eCCTKPARM_EC_TLSCFG:
      {
         eStatus = configTlsContext();
      }
      break;
#endif
   case eCCTKPARM_EC_CNXACTIVITY:
      {
         eStatus = configSipUaCnx();
      }
      break;
   case eCCTKPARM_EC_UAIDHDR:
      {
         eStatus = configSipUaIdHdr();
      }
      break;
   case eCCTKPARM_EC_ROUTINGHDR:
      {
         eStatus = configSipUaRoutingHdr();
      }
      break;
   case eCCTKPARM_EC_DNSMODE:
      {
         eStatus = configSipUaDns();
      }
      break;
   case eCCTKPARM_EC_SIGBEHAVE:
      {
         eStatus = configSipUaSigBehave();
      }
      break;
   case eCCTKPARM_EC_INTEROP:
      {
         eStatus = configSipUaInterop();
      }
      break;
   case eCCTKPARM_EC_TRANSTMR:
      {
         eStatus = configSipUaTransTmr();
      }
      break;
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   case eCCTKPARM_EC_TRACE:
      {
         configTrace();
      }
      break;
#endif
   case eCCTKPARM_EC_NATGENCFG:
      {
         eStatus = configSipUaNat();
      }
      break;
#if CCTKCFG_MEMORY_MONITORING
   case eCCTKPARM_EC_MEMPROTECT:
      {
         eStatus = configMxFramework();
      }
      break;
#endif
   case eCCTKPARM_MC_HOSTINFO:
      {
         CCTK_TRACEI1( "CfgEngine - modifySys: "
                       "Parameter eCCTKPARM_MC_HOSTINFO is not yet implemented.",
                       eParm );
         eStatus = eCFGSTATUS_BADPARM;
      }
      break;
   default:
      {
         CCTK_TRACEI1( "CfgEngine - modifySys: "
                       "Parameter %d is not a system config parameter.",
                       eParm );
         eStatus = eCFGSTATUS_BADPARM;
      }
      break;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   addUsr
**
**  PURPOSE:    Start up an SCE user resource.
**
**  PARAMETERS:
**              uIdx - The allocated user index.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::addUsr( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - addUsr(%lu)", this, uIdx );

   /* Index validity check */
   CCTK_ASSERT( uIdx < CFG_MAX_USR );
   if ( uIdx >= CFG_MAX_USR )
   {
      CCTK_TRACEI1( "CfgEngine - addUsr: Invalid index (%lu).", uIdx );
      return eCFGSTATUS_BADPARM;
   }

   /* This should have been checked by CCTK Framework before this function is
   ** called. Check again for safety.
   */
   if ( !isFuncAllowed( eTKM_MSGID_USERADD ) )
   {
      CCTK_TRACEI1( "CfgEngine - addUsr: "
         "Failed to be called in state (%d).",
         meCfgState );
      return eCFGSTATUS_BADSTATE;
   }

   /* Create an SCE user config object */
   ISceUserConfig *pSceConfig = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSceUserConfig, NULL, &pSceConfig );
   if ( MX_RIS_F( res ) || pSceConfig == NULL )
   {
      CCTK_TRACEI1(
         "CfgEngine - addUsr: Error creating user config object (%d)", res );
      return eCFGSTATUS_NORSC;
   }

   /* Store the user config object into the internal user config */
   CCTK_ASSERT( CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg == NULL );
   CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg = pSceConfig;
   pSceConfig = NULL;

   /* Configure main SCE user config */
   CFGSTATUS eStatus = configSceUsrMain( uIdx );
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - addUsr: "
         "Error configuring main SCE user config (%d).",
         eStatus );
   }

   /* Configure SCE user GRUU config */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSceUsrGruu( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - addUsr: "
            "Error configuring SCE user GRUU config (%d).",
            eStatus );
      }
   }

   /* Configure SCE user authentication config */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSceUsrAuth( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - addUsr: "
            "Error configuring SCE user authentication config (%d).",
            eStatus );
      }
   }

   /* Configure SCE user security config */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSceUsrSec( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - addUsr: "
            "Error configuring SCE user security config (%d).",
            eStatus );
      }
   }

   /* Configure SCE user identity config */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSceUsrId( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - addUsr: "
            "Error configuring SCE user identity config (%d).",
            eStatus );
      }
   }

   /* Configure SCE user capabilities config */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSceUsrUaCap( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - addUsr: "
            "Error configuring SCE user capabilities config (%d).",
            eStatus );
      }
   }

   /* Configure SCE user registration config */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSceUsrReg( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - addUsr: "
            "Error configuring SCE user registration config (%d).",
            eStatus );
      }
   }

   /* Configure SCE user call config */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSceUsrCall( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - addUsr: "
            "Error configuring SCE user call config (%d).",
            eStatus );
      }
   }

   /* Configure SCE user MWI config */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSceUsrMwi( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - addUsr: "
            "Error configuring SCE user MWI config (%d).",
            eStatus );
      }
   }

   /* Configure user persistent connection */
   if ( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.sec.nat.kaCfg.bUsePersistCnx )
   {
      if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
      {
         eStatus = configPersistCnx( uIdx );
         if ( CFGSTATUS_IS_FAIL( eStatus ) )
         {
            CCTK_TRACEI2( "CfgEngine - addUsr: "
               "Error configuring user persistent connection (%lu, %d).",
               uIdx,
               eStatus );
         }
      }
   }

   /* Clean up on failure */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg->ReleaseIfRef();
      CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg = NULL;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   modifyUsr
**
**  PURPOSE:    Modify and re-configure the SCE user resource.
**
**  PARAMETERS:
**              eParm   - Parameter ID.
**              uIdx    - The allocated user index.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::modifyUsr( CCTKPARM eParm, BOS_UINT32 uIdx )
{
   CCTK_TRACED3( "CfgEngine(%p) - modifyUsr(%d, %lu)", this, eParm, uIdx );

   /* Index validity check */
   CCTK_ASSERT( uIdx < CFG_MAX_USR );
   if ( uIdx >= CFG_MAX_USR )
   {
      CCTK_TRACEI1( "CfgEngine - modifyUsr: Invalid index (%lu).", uIdx );
      return eCFGSTATUS_BADPARM;
   }

   /* This should have been checked by CCTK Framework before this function is
   ** called. Check again for safety.
   */
   if ( !isFuncAllowed( eTKM_MSGID_SETPARM ) )
   {
      CCTK_TRACEI1( "CfgEngine - modifyUsr: "
         "Failed to be called in state (%d).",
         meCfgState );
      return eCFGSTATUS_BADSTATE;
   }
   if ( !CCTK_IS_USRPARM( eParm ) )
   {
      CCTK_TRACEI1( "CfgEngine - modifyUsr: Invalid parameter ID (%d).", eParm );
      return eCFGSTATUS_BADPARM;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Re-configure the SCE user config object */
   switch ( eParm )
   {
   case eCCTKPARM_MC_MEDIACFG:
   case eCCTKPARM_MC_MEDIALIST:
   case eCCTKPARM_EC_SUBEVT:
   case eCCTKPARM_EC_GENHDRCFG:
   case eCCTKPARM_EC_DIAGEVTCFG:
   case eCCTKPARM_EC_UAPEVTCFG:
   case eCCTKPARM_EC_USREMGCFG:
   case eCCTKPARM_EC_WHTLST:
   case eCCTKPARM_EC_BLKLST:
   case eCCTKPARM_EC_CALLHOLD:
   case eCCTKPARM_EC_CALLXFER:
   case eCCTKPARM_EC_MEDNEGCFG:
   case eCCTKPARM_EC_EVTTMR:
   case eCCTKPARM_EC_EVTPRIV:
   case eCCTKPARM_EC_EVTNAI:
      {
         /* FIXME: SCE does not allow these types of configurations yet. */
      }
      break;
   case eCCTKPARM_EC_USRPORTCFG:
      {
         BOS_UINT8 uTos = CFM_CFG.pUsr[uIdx]->
                             cfg.xtl.usr.net.portCfg.tosCfg.uDefault;
         /* set the TOS on the TCP Listening Socket, so new TCP connection will
         ** inherit the TOS.  */
         eStatus = configSipTcpListenerTos( uTos );
      }
      break;      
   case eCCTKPARM_MC_SIPNAME:
   case eCCTKPARM_EC_USRCONTACT:
      {
         eStatus = configSceUsrMain( uIdx );

         if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
         {
            eStatus = configSceUsrId( uIdx );
         }

         if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
         {
            eStatus = configSceUsrGruu( uIdx );
         }
      }
      break;
   case eCCTKPARM_MC_OBPROXYLST:
   case eCCTKPARM_EC_EMGCALLCFG:
   case eCCTKPARM_EC_SVCROUTE:
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
   case eCCTKPARM_EC_TELCALL:
#endif
      {
         eStatus = configSceUsrMain( uIdx );
      }
      break;
   case eCCTKPARM_EC_USRBRCFG:
   case eCCTKPARM_EC_REGTMR:
   case eCCTKPARM_EC_REGEVTCFG:
      {
         eStatus = configSceUsrReg( uIdx );
      }
      break;
   case eCCTKPARM_MC_MWISVRADDR:
   case eCCTKPARM_EC_MWICFG:
   case eCCTKPARM_EC_MWITMR:
      {
         eStatus = configSceUsrMwi( uIdx );
      }
      break;
   case eCCTKPARM_EC_PRACK:
   case eCCTKPARM_EC_CONNECTEDID:
   case eCCTKPARM_EC_SESTMR:
      {
         eStatus = configSceUsrCall( uIdx );
      }
      break;
   case eCCTKPARM_EC_CALLPRIV:
   case eCCTKPARM_EC_CALLNAI:
      {
         eStatus = configSceUsrId( uIdx );
      }
      break;
   case eCCTKPARM_EC_AUTHCFG:
      {  
         eStatus = configSceUsrAuth( uIdx );
      }
      break;
   default:
      {
         CCTK_TRACEI0(
            "CfgEngine - modifyUsr: Parameter is not a user config parameter." );
         eStatus = eCFGSTATUS_BADPARM;
      }
      break;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   removeUsr
**
**  PURPOSE:    Remove an SCE user resource.
**
**  PARAMETERS:
**              uIdx - The allocated user index.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::removeUsr( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - removeUsr(%lu)", this, uIdx );

   /* Index validity check */
   CCTK_ASSERT( uIdx < CFG_MAX_USR );
   if ( uIdx >= CFG_MAX_USR )
   {
      CCTK_TRACEI1( "CfgEngine - removeUsr: Invalid index (%lu).", uIdx );
      return eCFGSTATUS_BADPARM;
   }

   /* This should have been checked by CCTK Framework before this function is
   ** called. Check again for safety.
   */
   if ( !isFuncAllowed( eTKM_MSGID_USERREMOVE ) )
   {
      CCTK_TRACEI1( "CfgEngine - removeUsr: "
         "Failed to be called in state (%d).",
         meCfgState );
      return eCFGSTATUS_BADSTATE;
   }

   /* SCE user configuration object should be valid here */
   CCTK_ASSERT( CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg != NULL );
   if ( CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - removeUsr: "
         "Internal error. SCE user config object must be valid." );
      return eCFGSTATUS_INTERR;
   }

   /* Close any existing user persistent connection */
   CFGSTATUS eStatus = closePersistCnx( uIdx );
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI2( "CfgEngine - removeUsr: "
         "Error closing existing persistent connection (%lu, %d).",
         uIdx,
         eStatus );
      return eCFGSTATUS_INTERR;
   }

   if ( isAnyPersistCnxInState( 
            uIdx,
            static_cast< CFG_SOCKETSTATE >( eCFG_SOCKETSTATE_CLOSING ) ) )
   {
      /* Terminating persistent connection. Defer function status event until
      ** persistent connection termination is completed.
      */
      CFM_CFG.pUsr[uIdx]->cfg.in.bDeferEvtRemove = BOS_TRUE;
   }

   /* Release the SCE user config object */
   if ( CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg != NULL )
   {
      CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg->ReleaseIfRef();
      CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg = NULL;
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   modifyCall
**
**  PURPOSE:    Modify and re-configure the SCE call resource.
**
**  PARAMETERS:
**              eParm      - Parameter ID.
**              uIdxUsr    - User index.
**              uIdxCall   - Call index.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::modifyCall( CCTKPARM eParm,
                                 BOS_UINT32 uIdxUsr,
                                 BOS_UINT32 uIdxCall )
{
   CCTK_TRACED4(
      "CfgEngine(%p) - modifyCall(%d, %lu, %lu)", this, eParm, uIdxUsr, uIdxCall );

   /* Index validity check */
   CCTK_ASSERT( uIdxUsr < CFG_MAX_USR );
   CCTK_ASSERT( uIdxCall < CCTKCFG_MAX_CALL );
   if ( uIdxUsr >= CFG_MAX_USR || uIdxCall >= CCTKCFG_MAX_CALL )
   {
      CCTK_TRACEI2(
         "CfgEngine - modifyCall: Invalid index (%lu, %lu).", uIdxUsr, uIdxCall );
      return eCFGSTATUS_BADPARM;
   }

   /* This should have been checked by CCTK Framework before this function is
   ** called. Check again for safety.
   */
   if ( !isFuncAllowed( eTKM_MSGID_SETPARM ) )
   {
      CCTK_TRACEI1( "CfgEngine - modifyCall: "
         "Failed to be called in state (%d).",
         meCfgState );
      return eCFGSTATUS_BADSTATE;
   }
   if ( !CCTK_IS_CALLPARM( eParm ) )
   {
      CCTK_TRACEI1( "CfgEngine - modifyCall: Invalid parameter ID (%d).", eParm );
      return eCFGSTATUS_BADPARM;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Re-configure the SCE call config object */
   switch ( eParm )
   {
   case eCCTKPARM_MC_MEDIACFG:
   case eCCTKPARM_MC_MEDIALIST:
   case eCCTKPARM_EC_EMGCALLCFG:
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
   case eCCTKPARM_EC_TELCALL:
#endif
   case eCCTKPARM_EC_CALLSTATS:
   case eCCTKPARM_EC_PRACK:
   case eCCTKPARM_EC_SESTMR:
   case eCCTKPARM_EC_CONNECTEDID:
   case eCCTKPARM_EC_CALLHOLD:
   case eCCTKPARM_EC_CALLXFER:
   case eCCTKPARM_EC_MEDNEGCFG:
      {
         /* FIXME: SCE does not allow these types of configurations yet. */
      }
      break;
   case eCCTKPARM_EC_CALLPRIV:
   case eCCTKPARM_EC_CALLNAI:
      {
         eStatus = configSceUsrId( uIdxUsr, &uIdxCall );
      }
      break;
   default:
      {
         CCTK_TRACEI0(
            "CfgEngine - modifyCall: Parameter is not a call config parameter." );
         eStatus = eCFGSTATUS_BADPARM;
      }
      break;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   addRealm
**
**  PURPOSE:    Add a realm to form a new credential to be added to the user
**              authentication config.
**
**  PARAMETERS:
**              uIdx    - The allocated user index.
**              pcRealm - Realm string.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::addRealm( BOS_UINT32 uIdx, const BOS_UINT8 *pcRealm )
{
   CCTK_TRACED3( "CfgEngine(%p) - addRealm(%lu, %p)", this, uIdx, pcRealm );

   /* Index validity check */
   CCTK_ASSERT( uIdx < CFG_MAX_USR );
   if ( uIdx >= CFG_MAX_USR )
   {
      CCTK_TRACEI1( "CfgEngine - addRealm: Invalid index (%lu).", uIdx );
      return eCFGSTATUS_BADPARM;
   }

   /* Retrieve the SCE user config interface */
   ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg ;
   CCTK_ASSERT( pUsrConfig != NULL );
   if ( pUsrConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - addRealm: NULL user config interface." );
      return eCFGSTATUS_INTERR;
   }

   /* Query SCE user authentication config object */
   ISceUserAuthentication *pAuthConfig = NULL;
   mxt_result res = pUsrConfig->QueryIf( &pAuthConfig );
   if ( MX_RIS_F( res ) || pAuthConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - addRealm: "
         "Error creating user authentication config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   const CFGUEUSR *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr );

   const BOS_UINT8 *pcUser = pUsrCfg->sec.auth.client.pcAuthName,
                   *pcPwd = pUsrCfg->sec.auth.client.pcPwd;

   /* Use CCTK user name if authentication user name is not configured */
   if ( pcUser == NULL )
   {
      pcUser = pUsrCfg->net.sipName.pcUsrName;
   }

   CString realmStr, userStr, pwdStr;
   CFGSTATUS eStatus =
      cfgCvtRealmToMx( pcRealm, pcUser, pcPwd, realmStr, userStr, pwdStr );
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1(
         "CfgEngine - addRealm: Error converting realm (%d)",
         eStatus );
   }
   else
   {
      pAuthConfig->AddCredentials( realmStr, userStr, pwdStr, "ALL" );
   }

   pAuthConfig->ReleaseIfRef();
   pAuthConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   startUp
**
**  PURPOSE:    Start up SCE, SIP stack, and Config Engine by activating
**              the Main Framework.
**
**  PARAMETERS:
**              pMainFramework - Main framework to be started up.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is synchronous and MUST only be called within
**              the application task context.
***********************************************************************************/
CFGSTATUS CfgEngine::startUp( CEventDriven *pMainFramework )
{
   CCTK_TRACED2( "CfgEngine(%p) - startUp(%p)", this, pMainFramework );

   CCTK_ASSERT( pMainFramework != NULL );
   if ( pMainFramework == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - startUp: NULL Main Framework handle." );
      return eCFGSTATUS_BADPARM;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Create SCE config object */
   ISceCoreConfig *pCoreConfig = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSceCoreConfig, NULL, &pCoreConfig );
   if ( MX_RIS_F( res ) || pCoreConfig == NULL )
   {
      CCTK_TRACEI1(
         "CfgEngine - startUp: Error creating config object (%d)", res );
      return eCFGSTATUS_NORSC;
   }

   /* Create core thread */
   res = CreateEComInstance( CLSID_CServicingThread, NULL, &mpCoreThread );
   if ( MX_RIS_F( res ) || mpCoreThread == NULL )
   {
      CCTK_TRACEI1(
         "CfgEngine - startUp: Error creating core thread (%d)", res );
      eStatus = eCFGSTATUS_NORSC;
   }
   CCTK_TRACED2( "CfgEngine(%p) - startUp1(%p)", this, pMainFramework );
   /* Activate core thread */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = mpCoreThread->Activate();
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1(
            "CfgEngine - startUp: Error activating core thread (%d)", res );
         eStatus = eCFGSTATUS_NORSC;
      }
   }
   CCTK_TRACED2( "CfgEngine(%p) - startUp2(%p)", this, pMainFramework );
   /* Activate Main Framework */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = pMainFramework->Activate( mpCoreThread );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1(
            "CfgEngine - startUp: Error activating Main Framework (%d)",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }
   CCTK_TRACED2( "CfgEngine(%p) - startUp3(%p)", this, pMainFramework );
   /* Attach core thread to SCE */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = pCoreConfig->SetSceThread( mpCoreThread );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1(
            "CfgEngine - startUp: Error attaching core thread to SCE (%d)",
            res );
         eStatus = eCFGSTATUS_NORSC;
      }
   }

   /* Activate SCE */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = pCoreConfig->Startup();
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - startUp: Error activating SCE (%d)", res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }
   CCTK_TRACED2( "CfgEngine(%p) - startUp4(%p)", this, pMainFramework );
   /* Release config object */
   if ( pCoreConfig != NULL )
   {
      pCoreConfig->ReleaseIfRef();
      pCoreConfig = NULL;
   }

   /* Release core thread on failure */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      if ( mpCoreThread != NULL )
      {
         mpCoreThread->ReleaseIfRef();
         mpCoreThread = NULL;
      }
   }
   else
   {
      /* On success, keep the Main Framework handle for future use */
      mpMainFramework = pMainFramework;
   }
   CCTK_TRACED2( "CfgEngine(%p) - startUp5(%p)", this, pMainFramework );
   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   shutDown
**
**  PURPOSE:    Shutdown SCE, SIP stack, and the Config Engine.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is synchronous and MUST only be called within
**              the application task context.
***********************************************************************************/
CFGSTATUS CfgEngine::shutDown( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - shutDown()", this );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Shutdown the SCE with it being the manager */
   ISceCoreConfig *pCoreConfig = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSceCoreConfig, NULL, &pCoreConfig );
   if ( MX_RIS_F( res ) || pCoreConfig == NULL )
   {
      CCTK_TRACEI1(
         "CfgEngine - shutDown: Error creating config object (%d)", res );
      eStatus = eCFGSTATUS_NORSC;
   }

   /* Stop all scheduled timers */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = mpMainFramework->StopAllTimers();
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1(
            "CfgEngine - shutDown: Error stopping timers (%d)", res );
         eStatus = eCFGSTATUS_INTERR;
      }

      /* NOTE: Other timers are stopped in the core thread's context */
   }

   /* Perform the first shutdown process synchronously to clean up the resources
   ** that can only be accessed/modified by the stack's core thread.
   */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      CCTKSTATUS eCctkStatus = tkmMsgPost( BOS_TRUE, 0, NULL, &cleanCoreResCb );
      if ( CCTKSTATUS_IS_FAIL( eCctkStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - shutDown: "
            "Unable to synchronously post message (%d)",
            eCctkStatus );
         eStatus = eCFGSTATUS_INTERR;
      }
      else if ( CFGSTATUS_IS_FAIL( geCleanupStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - shutDown: "
            "Error cleaning up the core thread related resources (%d)",
            geCleanupStatus );
         eStatus = geCleanupStatus;
      }
   }

   /* Perform the second shutdown process to shut down the SCE and SIP stack */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = pCoreConfig->ShutdownA( this, NULL );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1(
            "CfgEngine - shutDown: Error shutting down SCE and SIP stack (%d)",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }

   /* Release config object */
   if ( pCoreConfig != NULL )
   {
      pCoreConfig->ReleaseIfRef();
      pCoreConfig = NULL;
   }

   /* Wait until SCE and SIP stack shut down completely */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      if ( !mShutdownSem.Wait( CFGENGINE_MAX_SHUTDOWN_WAIT_TIME ) )
      {
         CCTK_TRACEI0( "CfgEngine - shutDown: SCE/SIP stack shutdown timeout" );

         /* Resource leak */
         eStatus = eCFGSTATUS_RSCLEAK;
      }

      /* Release the core thread */
      mpCoreThread->ReleaseIfRef();
      mpCoreThread = NULL;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   EvOnShutdownCompleted
**
**  PURPOSE:    Event reported after the shutdown process is completed.
**
**  PARAMETERS:
**              opqShutdown - Opaque value passed to the ISceCoreConfig::Shutdown()
**                            method.
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::EvOnShutdownCompleted( mxt_opaque opqShutdown )
{
   CCTK_TRACED2( "CfgEngine(%p) - EvOnShutdownCompleted(%p)", this, opqShutdown );

   /* Opaque was not used when Shutdown() is called */
   CCTK_ASSERT( opqShutdown == NULL );

   /* Tell the app SCE and SIP stack have completely shut down.
   **
   ** NOTE: "mShutdownSem" is accessed from the stack's core thread here because
   **       the application's task is blocked by this function callback.
   */
   mShutdownSem.Signal();
}

/***********************************************************************************
**  FUNCTION:   EvOnPacketReceived
**
**  PURPOSE:    Event reported when a new incoming packet is received and not
**              handled by the lower layers of the stack yet (e.g. transaction).
**
**  PARAMETERS:
**              rPacket - The incoming packet.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::EvOnPacketReceived( const CSipPacket& rPacket )
{
   CCTK_TRACED2(
      "CfgEngine(%p) - EvOnPacketReceived(%p)", this, &rPacket );

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rPacket );

   /* FIXME: Ignored? or add extra packet handling */
}

/***********************************************************************************
**  FUNCTION:   EvShutdownCompleted
**
**  PURPOSE:    Event reported when the SIP stack shutdown process is completed.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::EvShutdownCompleted()
{
   CCTK_TRACED1(
      "CfgEngine(%p) - EvShutdownCompleted()", this );

   /* This should have been handled within SCE. No action is required here. */
}

/***********************************************************************************
**  FUNCTION:   EvCommandResult
**
**  PURPOSE:    Event reported on core user operations.
**
**  PARAMETERS:
**              res - The result code.
**              opq - Opaque parameter given to core config manager.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::EvCommandResult( mxt_result res, mxt_opaque opq )
{
   CCTK_TRACED3( "CfgEngine(%p) - EvCommandResult(%d, %p)", this, res, opq );

   /* NOTE: This function is exclusively used for receiving transport
   ** operational events
   */
   CFGSTATUS eStatus;
   BOS_UINT32 uOpq = reinterpret_cast< BOS_UINT32 >( opq );
   BOS_UINT32 uIdx = uOpq & 0x0000FFFF;
   CFG_TRANSPORTID eTransportId =
      static_cast< CFG_TRANSPORTID >( ( uOpq >> 16 ) & 0x0000FFFF );
   switch ( eTransportId )
   {
   case eCFG_TRANSPORTID_STOP_UDP:
      {
         if ( mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_CLOSING )
         {
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Error stopping UDP listening socket (%d).",
                  res );
            }

            /* Treat socket as "closed" even if the "stop" operation fails */
            mLocAddr[uIdx].cnx.udp.listen.eState = eCFG_SOCKETSTATE_CLOSED;
            mLocAddr[uIdx].cnx.udp.listen.socketId = 0;
         }
         else if ( mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_OPENED )
         {
            if ( MX_RIS_S( res ) )
            {
               /* Listening port is closed for some unknown reason.
               ** Attempt to re-open it again.
               */
               mLocAddr[uIdx].cnx.udp.listen.eState = eCFG_SOCKETSTATE_CLOSED;
               mLocAddr[uIdx].cnx.udp.listen.socketId = 0;

               eStatus = openListeningPort();
               if ( CFGSTATUS_IS_FAIL( eStatus ) )
               {
                  CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                     "Error re-openning UDP listening socket (%d).",
                     eStatus );
               }
            }
         }
         else
         {
            /* Ignore this event for other states.
            ** NOTE: We can be in a socket opening state when closeListeningPort()
            ** is immediately followed by openListeningPort().
            */
         }
      }
      break;
   case eCFG_TRANSPORTID_STOP_TCP:
      {
         if ( mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_CLOSING )
         {
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Error stopping TCP listening socket (%d).",
                  res );
            }

            /* Treat socket as "closed" even if the "stop" operation fails */
            mLocAddr[uIdx].cnx.tcp.listen.eState = eCFG_SOCKETSTATE_CLOSED;
            mLocAddr[uIdx].cnx.tcp.listen.socketId = 0;
         }
         else if ( mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_OPENED )
         {
            if ( MX_RIS_S( res ) )
            {
               /* Listening port is closed for some unknown reason.
               ** Attempt to re-open it again.
               */
               mLocAddr[uIdx].cnx.tcp.listen.eState = eCFG_SOCKETSTATE_CLOSED;
               mLocAddr[uIdx].cnx.tcp.listen.socketId = 0;

               eStatus = openListeningPort();
               if ( CFGSTATUS_IS_FAIL( eStatus ) )
               {
                  CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                     "Error re-openning TCP listening socket (%d).",
                     eStatus );
               }
            }
         }
         else
         {
            /* Ignore this event for other states.
            ** NOTE: We can be in a socket opening state when closeListeningPort()
            ** is immediately followed by openListeningPort().
            */
         }
      }
      break;
#if CCTKCFG_TLS
   case eCFG_TRANSPORTID_STOP_TLS:
      {
         if ( mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_CLOSING )
         {
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Error stopping TLS listening socket (%d).",
                  res );
            }

            /* Treat socket as "closed" even if the "stop" operation fails */
            mLocAddr[uIdx].cnx.tls.listen.eState = eCFG_SOCKETSTATE_CLOSED;
            mLocAddr[uIdx].cnx.tls.listen.socketId = 0;
         }
         else if ( mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_OPENED )
         {
            if ( MX_RIS_S( res ) )
            {
               /* Listening port is closed for some unknown reason.
               ** Attempt to re-open it again.
               */
               mLocAddr[uIdx].cnx.tls.listen.eState = eCFG_SOCKETSTATE_CLOSED;
               mLocAddr[uIdx].cnx.tls.listen.socketId = 0;

               eStatus = openListeningPort();
               if ( CFGSTATUS_IS_FAIL( eStatus ) )
               {
                  CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                     "Error re-openning TLS listening socket (%d).",
                     eStatus );
               }
            }
         }
         else
         {
            /* Ignore this event for other states.
            ** NOTE: We can be in a socket opening state when closeListeningPort()
            ** is immediately followed by openListeningPort().
            */
         }
      }
      break;
#endif
   case eCFG_TRANSPORTID_LISTEN_UDP:
      {
         if ( mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_OPENING )
         {
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Error opening UDP listening socket (%d).",
                  res );
               mLocAddr[uIdx].cnx.udp.listen.eState = eCFG_SOCKETSTATE_CLOSED;
               mLocAddr[uIdx].cnx.udp.listen.socketId = 0;
            }
            else
            {
               /* Listening socket opened successfully */
               mLocAddr[uIdx].cnx.udp.listen.eState = eCFG_SOCKETSTATE_OPENED;
            }
         }
         else if ( mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_CLOSED )
         {
            if ( MX_RIS_S( res ) )
            {
               /* Listening port somehow stays open again.
               ** Ignore this erroneous event.
               */
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Unexpected UDP listening socket open event (%d).",
                  res );
            }
         }
         else
         {
            /* Ignore this event for other states.
            ** NOTE: We can be in a socket closing state when openListeningPort() is
            ** immediately followed by closeListeningPort().
            */
         }
      }
      break;
   case eCFG_TRANSPORTID_LISTEN_TCP:
      {
         if ( mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_OPENING )
         {
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Error opening TCP listening socket (%d).",
                  res );
               mLocAddr[uIdx].cnx.tcp.listen.eState = eCFG_SOCKETSTATE_CLOSED;
               mLocAddr[uIdx].cnx.tcp.listen.socketId = 0;
            }
            else
            {
               /* Listening socket opened successfully */
               mLocAddr[uIdx].cnx.tcp.listen.eState = eCFG_SOCKETSTATE_OPENED;
            }
         }
         else if ( mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_CLOSED )
         {
            if ( MX_RIS_S( res ) )
            {
               /* Listening port somehow stays open again.
               ** Ignore this erroneous event.
               */
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Unexpected TCP listening socket open event (%d).",
                  res );
            }
         }
         else
         {
            /* Ignore this event for other states.
            ** NOTE: We can be in a socket closing state when openListeningPort() is
            ** immediately followed by closeListeningPort().
            */
         }
      }
      break;
#if CCTKCFG_TLS
   case eCFG_TRANSPORTID_LISTEN_TLS:
      {
         if ( mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_OPENING )
         {
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Error opening TLS listening socket (%d).",
                  res );
               mLocAddr[uIdx].cnx.tls.listen.eState = eCFG_SOCKETSTATE_CLOSED;
               mLocAddr[uIdx].cnx.tls.listen.socketId = 0;
            }
            else
            {
               /* Listening socket opened successfully */
               mLocAddr[uIdx].cnx.tls.listen.eState = eCFG_SOCKETSTATE_OPENED;
            }
         }
         else if ( mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_CLOSED )
         {
            if ( MX_RIS_S( res ) )
            {
               /* Listening port somehow stays open again.
               ** Ignore this erroneous event.
               */
               CCTK_TRACEI1( "CfgEngine - EvCommandResult: "
                  "Unexpected TLS listening socket open event (%d).",
                  res );
            }
         }
         else
         {
            /* Ignore this event for other states.
            ** NOTE: We can be in a socket closing state when openListeningPort() is
            ** immediately followed by closeListeningPort().
            */
         }
      }
      break;
#endif
   default:
      {
         /* Don't do anything on results for core user operations */
         return;
      }
      break;
   }

   /* Update the configuration state based on the current listening port status */
   updCfgState();
}

/***********************************************************************************
**  FUNCTION:   LogRawData
**
**  PURPOSE:    Event reported when raw data has been received or sent.
**
**  PARAMETERS:
**              eDir       - The direction of the packet being logged.
**              rLocalAddr - The local address associated with the raw data.
**              rPeerAddr  - The peer address associated with the raw data.
**              rRawData   - The raw data coming from or going on the network.
**              uSize      - The size of the raw data.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This event is called within the context of the transport threads.
**              Always queue up for core thread processing.
***********************************************************************************/
void CfgEngine::LogRawData( EDirection eDir,
                            const CSocketAddr &rLocalAddr,
                            const CSocketAddr &rPeerAddr,
                            const CBlob &rRawData,
                            unsigned int uSize )
{
   CCTK_TRACED4( "CfgEngine(%p) - LogRawData(%d, %p, %p)",
                 this,
                 eDir,
                 &rLocalAddr,
                 &rPeerAddr );

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uSize );

   /* Duplicate the parameters */
   CSocketAddr *pLocalAddr = BRCM_NEW( CSocketAddr )( rLocalAddr );
   CSocketAddr *pPeerAddr = BRCM_NEW( CSocketAddr )( rPeerAddr );
   CBlob *pRawData = 
      BRCM_NEW( CBlob )( rRawData.GetFirstIndexPtr(), 
                         rRawData.GetSize(),
                         rRawData.GetSize() + CFGENGINE_MAX_PKTLOG_HDR_SIZE );

   /* Post a message onto the message queue */
   CMarshaler *pParms = CPool< CMarshaler >::New();
   *pParms << eDir
           << pLocalAddr
           << pPeerAddr
           << pRawData;

   CCTKSTATUS eStatus = tkmMsgPost( BOS_FALSE,
                                    eCFG_PKTLOGFUNCID_RAW,
                                    pParms,
                                    &pktLoggerCb );
   if ( CCTKSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - LogRawData: "
         "Unable to asynchronously post message (%d)",
         eStatus );

      BRCM_DELETE( pLocalAddr );
      BRCM_DELETE( pPeerAddr );
      BRCM_DELETE( pRawData );
      CPool< CMarshaler >::Delete( pParms );
   }
}

/***********************************************************************************
**  FUNCTION:   LogSipPacket
**
**  PURPOSE:    Event reported when a SIP packet has been received or sent.
**
**  PARAMETERS:
**              eDir    - The direction of the packet being logged.
**              rPacket - The packet to be logged.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This event is called within the context of the transport threads.
**              Always queue up for core thread processing.
***********************************************************************************/
void CfgEngine::LogSipPacket( EDirection eDir, const CSipPacket& rPacket )
{
   CCTK_TRACED3( "CfgEngine(%p) - LogSipPacket(%d, %p)",
                 this,
                 eDir,
                 &rPacket );

   /* Duplicate the parameters */
   CSocketAddr *pLocalAddr = BRCM_NEW( CSocketAddr )( rPacket.GetLocalAddr() );
   CSocketAddr *pPeerAddr = BRCM_NEW( CSocketAddr )( rPacket.GetPeerAddr() );
   BOS_UINT32 uPayloadSize = 
      ( rPacket.GetPayload() == NULL ) ? 0 : rPacket.GetPayload()->GetSize();
   CBlob *pRawData = BRCM_NEW( CBlob )( CFGENGINE_DEF_PKT_HDR_SIZE + uPayloadSize );
   rPacket.Serialize( *pRawData );

   /* Post a message onto the message queue */
   CMarshaler *pParms = CPool< CMarshaler >::New();
   *pParms << eDir
           << pLocalAddr
           << pPeerAddr
           << pRawData;

   CCTKSTATUS eStatus = tkmMsgPost( BOS_FALSE,
                                    eCFG_PKTLOGFUNCID_SIP,
                                    pParms,
                                    &pktLoggerCb );
   if ( CCTKSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - LogSipPacket: "
         "Unable to asynchronously post message (%d)",
         eStatus );

      BRCM_DELETE( pLocalAddr );
      BRCM_DELETE( pPeerAddr );
      BRCM_DELETE( pRawData );
      CPool< CMarshaler >::Delete( pParms );
   }
}

/***********************************************************************************
**  FUNCTION:   EvConnectionEstablished
**
**  PURPOSE:    Event reported when a persistent connection has been established.
**
**  PARAMETERS:
**              opqConnection - The connection opaque received in the call to
**                              Establish.
**              rLocalAddr    - The effective local address used for the connection.
**                              If the local address was fully specified
**                              (address and port) in the call to Establish,
**                              rLocalAddr simply returns this same information. 
**                              If however the port or address were left
**                              unspecified, rLocalAddr contains the local address
**                              and port that were actually used.
**              rPeerAddr     - The remote peer address. When a FQDN is used this
**                              address is assigned by the stack following a DNS
**                              query.
**
**  RETURNS:    Nothing
**
**  NOTES:      This event is called within the context of the core threads, i.e.
**              this event does not needed to be queued.
***********************************************************************************/
void CfgEngine::EvConnectionEstablished( mxt_opaque opqConnection, 
                                         const CSocketAddr &rLocalAddr,
                                         const CSocketAddr &rPeerAddr )
{
   CCTK_TRACED4( "CfgEngine(%p) - EvConnectionEstablished(%p, %p, %p)",
                 this,
                 opqConnection,
                 &rLocalAddr,
                 &rPeerAddr );

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rLocalAddr );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rPeerAddr );

   BOS_UINT32 uAddrIdx = 0, uUsrIdx = 0, uTransport;
   CFG_TRANSPORTINFO *pPortInfo = 
      findPersistCnxIdx( BOS_FALSE, opqConnection, uAddrIdx, uUsrIdx, uTransport );
   if ( pPortInfo == NULL )
   {
      /* This is a connection event from a duplicated connection */
      CCTK_TRACED1( 
         "CfgEngine - EvConnectionEstablished: Duplicated connection opaque (%p).",
         opqConnection );

      return;
   }

   /* This is an active connection event */
   CCTK_ASSERT( !UTIL_BMP_IS_ENABLED( pPortInfo->persist[uUsrIdx].sock.eState,
                                      eCFG_SOCKETSTATE_DUPLCED ) );

   if ( UTIL_BMP_IS_ENABLED( pPortInfo->persist[uUsrIdx].sock.eState,
                             eCFG_SOCKETSTATE_OPENING ) )
   {
      /* Update persistent connection socket state. Keep the "sharing" state
      ** for re-opening listening ports later.
      */
      pPortInfo->persist[uUsrIdx].sock.eState =
         static_cast< CFG_SOCKETSTATE >(
            ( pPortInfo->persist[uUsrIdx].sock.eState & eCFG_SOCKETSTATE_SHARING ) |
            eCFG_SOCKETSTATE_OPENED );

      /* Update the stack's and SCE's persistent connection properties */
      updateStackPcProperty( eCFG_PERSISTCNXEVT_ESTABLISHED,
                             pPortInfo->persist[uUsrIdx].sock.socketId,
                             uUsrIdx );

      /* Update the other duplicated connection status, if any. */
      updateDupPcOnActEvt( eCFG_PERSISTCNXEVT_ESTABLISHED, *pPortInfo, uUsrIdx );
   }
   else if ( pPortInfo->persist[uUsrIdx].sock.eState == eCFG_SOCKETSTATE_CLOSED )
   {
      /* Persistent connection somehow stays open again.
      ** Ignore this erroneous event.
      */
      CCTK_TRACEI3( "CfgEngine - EvConnectionEstablished: "
         "Unexpected persistent connection establishment event (%lu, %lu, %lu).",
         uAddrIdx,
         uUsrIdx,
         uTransport );
   }
   else
   {
      /* Get to here if either:
      ** - we are in a socket closing state when openPersistCnx() is immediately
      **   followed by closePersistCnx(), or
      ** - the connection has previously gone down, i.e. EvErrorOnConnection() was
      **   called, and it is now re-established again.
      */
   }

   /* Notify the application when the function status events are deferred */
   handleDeferredEvtUserAdd( uUsrIdx );
   handleDeferredEvtUserRemove( uUsrIdx );
}

/***********************************************************************************
**  FUNCTION:   EvConnectionTerminated
**
**  PURPOSE:    Event reported when a persistent connection has been terminated.
**
**  PARAMETERS:
**              opqConnection - The connection opaque received in the call to
**                              Establish.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This event is called within the context of the core threads, i.e.
**              this event does not needed to be queued.
***********************************************************************************/
void CfgEngine::EvConnectionTerminated( mxt_opaque opqConnection )
{
   CCTK_TRACED2( "CfgEngine(%p) - EvConnectionTerminated(%p)",
                 this,
                 opqConnection );

   BOS_UINT32 uAddrIdx = 0, uUsrIdx = 0, uTransport;
   CFG_TRANSPORTINFO *pPortInfo =
      findPersistCnxIdx( BOS_FALSE, opqConnection, uAddrIdx, uUsrIdx, uTransport );
   if ( pPortInfo == NULL )
   {
      /* This is a connection event from a duplicated connection.
      ** This event is a result of a successful connection termination from
      ** handleDupPcOnErrEvt() and it can be ignored here.
      */
      CCTK_TRACED1( 
         "CfgEngine - EvConnectionTerminated: Duplicated connection opaque (%p).",
         opqConnection );

      return;
   }

   /* This is an active connection event */
   CCTK_ASSERT( !UTIL_BMP_IS_ENABLED( pPortInfo->persist[uUsrIdx].sock.eState,
                                      eCFG_SOCKETSTATE_DUPLCED ) );

   /* This function event can be called in one of the following scenarios:
   ** 1) Terminate() has been called and persistent socket is in "closing" state.
   ** 2) Terminate() has been called and persistent socket is in "opening" state
   **    as openPersistCnx() is called immediately after closePersistCnx().
   ** 3) ShutdownA() has been called without calling Terminate()
   **    (i.e. closePersistCnx() is not called before the stack shutdown).
   */
   if ( UTIL_BMP_IS_ENABLED( pPortInfo->persist[uUsrIdx].sock.eState,
                             eCFG_SOCKETSTATE_CLOSING | eCFG_SOCKETSTATE_OPENED ) )
   {
      /* Scenario (1) and (3) */

      /* Update the other duplicated connection status, if any. */
      updateDupPcOnActEvt( eCFG_PERSISTCNXEVT_TERMINATED, *pPortInfo, uUsrIdx );

      /* Update the stack's and SCE's persistent connection properties */
      updateStackPcProperty( eCFG_PERSISTCNXEVT_TERMINATED,
                             pPortInfo->persist[uUsrIdx].sock.socketId,
                             uUsrIdx );

      /* Open listening connection only when the stack is not shutting down */
      BOS_BOOL bOpenPortNeeded = 
         UTIL_BMP_IS_ENABLED_ALL( pPortInfo->persist[uUsrIdx].sock.eState,
                                  eCFG_SOCKETSTATE_CLOSING |
                                  eCFG_SOCKETSTATE_SHARING );

      /* Update persistent connection socket state. "Sharing" bit is reset here. */
      pPortInfo->persist[uUsrIdx].sock.eState = eCFG_SOCKETSTATE_CLOSED;
      pPortInfo->persist[uUsrIdx].sock.socketId = 0;

      /* Open listening connection, if necessary. Also check if there are any
      ** outstanding persistent connections that also share the listening ports.
      ** Wait until all persistent connections are terminated.
      */
      if ( bOpenPortNeeded && 
           meCfgState == eCFG_STATE_CFGED &&
           !isAnySharingPersistCnx( uAddrIdx ) )
      {
         CFGSTATUS eStatus = openListeningPort();
         if ( CFGSTATUS_IS_FAIL( eStatus ) )
         {
            CCTK_TRACEI1( "CfgEngine - EvConnectionTerminated: "
               "Error re-opening listening ports (%d).",
               eStatus );
         }
      }
   }
   else
   {
      /* Persistent connection has already been closed, or
      ** Scenario (2): We can be in a socket opening state when closePersistCnx()
      ** is immediately followed by openPersistCnx().
      */
   }

   /* Notify the application when the function status events are deferred */
   handleDeferredEvtUserAdd( uUsrIdx );
   handleDeferredEvtUserRemove( uUsrIdx );
}

/***********************************************************************************
**  FUNCTION:   EvErrorOnConnection
**
**  PURPOSE:    Event reported when a failure on a persistent connection occurs.
**
**  PARAMETERS:
**              opqConnection       - The connection opaque received in the call to
**                                    Establish.
**              resConnectionError  - The error that occured while trying to
**                                    establish or re-establish the persistent
**                                    connection.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This event is called within the context of the core threads, i.e.
**              this event does not needed to be queued.
***********************************************************************************/
void CfgEngine::EvErrorOnConnection( mxt_opaque opqConnection,
                                     mxt_result resConnectionError )
{
   CCTK_TRACED3( "CfgEngine(%p) - EvErrorOnConnection(%p, %d)",
                 this,
                 opqConnection,
                 resConnectionError );

   BOS_UINT32 uAddrIdx = 0, uUsrIdx = 0, uTransport;
   CFG_TRANSPORTINFO *pPortInfo = 
      findPersistCnxIdx( BOS_FALSE, opqConnection, uAddrIdx, uUsrIdx, uTransport );
   if ( pPortInfo == NULL )
   {
      /* This is a connection event from a duplicated connection */
      CCTK_TRACED2( 
         "CfgEngine - EvErrorOnConnection: Duplicated connection opaque (%p, %d).",
         opqConnection,
         resConnectionError );

      return;
   }

   /* This is an active connection event */
   CCTK_ASSERT( !UTIL_BMP_IS_ENABLED( pPortInfo->persist[uUsrIdx].sock.eState, 
                                      eCFG_SOCKETSTATE_DUPLCED ) );

   if ( resConnectionError == resFE_DUPLICATE )
   {
      /* Another user may have established another persistent connection which
      ** ends up (after DNS lookup) using the same local and remote address info.
      */
      CCTK_ASSERT( UTIL_BMP_IS_ENABLED( pPortInfo->persist[uUsrIdx].sock.eState,
                                        eCFG_SOCKETSTATE_OPENING ) );

      if ( CFGSTATUS_IS_FAIL(
               handleDupPcOnErrEvt( *pPortInfo, uUsrIdx, uTransport ) ) )
      {
         CCTK_TRACEI4( "CfgEngine - EvErrorOnConnection: "
            "Error handling duplicated persistent connection "
            "(%lu, %lu, %d, %p).",
            uTransport,
            uUsrIdx,
            pPortInfo->persist[uUsrIdx].sock.eState,
            pPortInfo->persist[uUsrIdx].sock.socketId );
      }
      else
      {
         CCTK_TRACED4( "CfgEngine - EvErrorOnConnection: "
            "There exist another connection with same address info "
            "(%lu, %lu, %d, %p).",
            uTransport,
            uUsrIdx,
            pPortInfo->persist[uUsrIdx].sock.eState,
            pPortInfo->persist[uUsrIdx].sock.socketId );
      }

      return;
   }

   if ( UTIL_BMP_IS_ENABLED_ALL( pPortInfo->persist[uUsrIdx].sock.eState, 
                                 eCFG_SOCKETSTATE_RETRYING |
                                 eCFG_SOCKETSTATE_OPENING) )
   {
      /* Error retrying the connection, implying there exist another persistent
      ** connection having the same local and remote address information.
      ** Terminate this persistent connection as it is not necessary anymore.
      */
      CFGSTATUS eStatus = closePersistCnx( uUsrIdx, uTransport, BOS_TRUE );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI3( "CfgEngine - EvErrorOnConnection: "
            "Error closing persistent connection (%lu, %lu, %d).",
            uUsrIdx,
            uTransport,
            eStatus );
      }
   }
   else if ( UTIL_BMP_IS_ENABLED( pPortInfo->persist[uUsrIdx].sock.eState, 
                                  eCFG_SOCKETSTATE_OPENING ) &&
             pPortInfo->listen.eState == eCFG_SOCKETSTATE_CLOSED )
   {
      /* Enable the "retrying" bit if the new persistent connection has
      ** problem establishing. We do not do anything else as the stack would
      ** periodically retry establishing the connection.
      */
      pPortInfo->persist[uUsrIdx].sock.eState =
         static_cast< CFG_SOCKETSTATE >(
            pPortInfo->persist[uUsrIdx].sock.eState | eCFG_SOCKETSTATE_RETRYING );
      
      /* Start the retry timeout timer to accelerate the persistent connection
      ** timeout (especially for TCP handshaking which can last up to one minute.
      ** No retry timeout timer should have been started earlier.
      */
      CCTK_ASSERT( pPortInfo->persist[uUsrIdx].uTmrId == 0 );

      CCTKSTATUS eCctkStatus =
         tkmTimerStart( &( pPortInfo->persist[uUsrIdx].uTmrId ),
                        CFGENGINE_PST_CNX_RETRY_TIMEOUT,
                        BOS_FALSE,
                        opqConnection,
                        &pstCnxTimeoutCb );
      if ( CCTKSTATUS_IS_FAIL( eCctkStatus ) )
      {
         CCTK_TRACEI3( "CfgEngine - EvErrorOnConnection: "
            "Error starting retry timeout timer (%lu, %lu, %d).",
            uUsrIdx,
            uTransport,
            eCctkStatus );

         pPortInfo->persist[uUsrIdx].uTmrId = 0;
      }
   }

   /* Ignore error events when persistent connection has once been established. */
}

/***********************************************************************************
**  FUNCTION:   EvSipErrorOnConnection
**
**  PURPOSE:    Event reported when a SIP failure on a persistent connection occurs.
**
**  PARAMETERS:
**              opqConnection       - The connection handle received in the call to
**                                    Establish.
**              rSentPacket         - The packet that failed to be sent.
**              resConnectionError  - The error that occurred at the SIP level on
**                                    the persistent connection.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This event is called within the context of the core threads, i.e.
**              this event does not needed to be queued.
***********************************************************************************/
void CfgEngine::EvSipErrorOnConnection( mxt_opaque opqConnectionHandle, 
                                        const CSipPacket& rSentPacket,
                                        mxt_result resConnectionError )
{
   CCTK_TRACED3(
      "CfgEngine(%p) - EvSipErrorOnConnection() - SIP error on connection %p (%x).",
      this,
      opqConnectionHandle,
      resConnectionError );

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( opqConnectionHandle );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rSentPacket );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( resConnectionError );

   // Just ignoring this error for now.
}

#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
/***********************************************************************************
**  FUNCTION:   initTrace
**
**  PURPOSE:    Initialize trace support.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::initTrace( void )
{
   /* No trace is added here because M5T trace utility has not been enabled yet.
   ** Trace can be added once CCTK custom tracing support is implemented.
   */

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
   /* Register CCTK tracing node */
   bool bRes = MxTraceRegisterNode( &g_stTraceRoot, &gCctkTraceNode, "Cctk" );
   CCTK_ASSERT( bRes );
#endif

   /* Initialize trace support based on default configuration */
   configTrace();

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
   /* Activate the internal CCTK trace output handler */
   MxTraceSetNewOutputHandler( tkmMxTraceHandler );
#endif
}

/***********************************************************************************
**  FUNCTION:   configTrace
**
**  PURPOSE:    Configure trace support.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::configTrace( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configTrace()", this );

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
   /* Configure trace levels and groups */
   setTraceLevel( CFM_CFG.pSys->xtl.loc.debug.trace.uLevel );
   setTraceGroup( CFM_CFG.pSys->xtl.loc.debug.trace.uGroup );
#endif

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_CCTK )
   /* Do not enable CCTK trace handler if CCTK trace group is not enabled. */
   if ( !UTIL_BMP_IS_ENABLED( CFM_CFG.pSys->xtl.loc.debug.trace.uGroup,
                              CCTK_TRACEGRP_CCTK ) )
   {
      gpCctkTraceCb = NULL;
   }
   else
#endif
   {
      /* Store the trace output handler pointer for faster access in tracing */
      gpCctkTraceCb = CFM_CFG.pSys->xtl.loc.debug.trace.pHandler;
   }
}

/***********************************************************************************
**  FUNCTION:   disableTrace
**
**  PURPOSE:    Disable trace support.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::disableTrace( void )
{
   /* Stop the CCTK trace callback */
   gpCctkTraceCb = NULL;

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
   /* Remove trace output handler */
   MxTraceRemoveOutputHandler( tkmMxTraceHandler );

   /* Turn off all trace levels and groups */
   setTraceLevel( CCTK_TRACELVL_NONE );
   setTraceGroup( CCTK_TRACEGRP_NONE );

   /* Unregister CCTK tracing node */
   MxTraceUnregisterNode( &g_stTraceRoot, &gCctkTraceNode );
#endif
}

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
/***********************************************************************************
**  FUNCTION:   setTraceLevel
**
**  PURPOSE:    Set trace level.
**
**  PARAMETERS:
**              uLevel - Trace level (CCTK_TRACELVL_XXX)
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::setTraceLevel( BOS_UINT16 uLevel )
{
   CCTK_TRACED2( "CfgEngine(%p) - setTraceLevel(%u)", this, uLevel );

   /* Disable all trace levels by default */
   MxTraceDisableLevel( eLEVEL_ALL );

   EMxTraceLevel eMxLevel = cfgCvtTraceLvlToMx( uLevel );
   MxTraceEnableLevel( eMxLevel );
}

/***********************************************************************************
**  FUNCTION:   setTraceGroup
**
**  PURPOSE:    Set trace group.
**
**  PARAMETERS:
**              uGroup - Trace group (CCTK_TRACEGRP_XXX)
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::setTraceGroup( BOS_UINT16 uGroup )
{
   CCTK_TRACED2( "CfgEngine(%p) - setTraceGroup(%u)", this, uGroup );

   BOS_BOOL bRes;

   /* Disable all trace groups by default */
   bRes = MxTraceDisableNode( "/Assert", true ); CCTK_ASSERT( bRes );
   bRes = MxTraceDisableNode( "/Framework", true ); CCTK_ASSERT( bRes );
   bRes = MxTraceDisableNode( "/SipStack", true ); CCTK_ASSERT( bRes );
   bRes = MxTraceDisableNode( "/SdpParser", true ); CCTK_ASSERT( bRes );
   bRes = MxTraceDisableNode( "/Sce", true ); CCTK_ASSERT( bRes );
   bRes = MxTraceDisableNode( "/Cctk", true ); CCTK_ASSERT( bRes );

   if ( UTIL_BMP_IS_ENABLED_ALL( uGroup, CCTK_TRACEGRP_CCTK ) )
   {
      bRes = MxTraceEnableNode( "/Cctk", true ); CCTK_ASSERT( bRes );
   }

   if ( UTIL_BMP_IS_ENABLED_ALL( uGroup, CCTK_TRACEGRP_SCE ) )
   {
      bRes = MxTraceEnableNode( "/Sce", true ); CCTK_ASSERT( bRes );
   }

   if ( UTIL_BMP_IS_ENABLED_ALL( uGroup, CCTK_TRACEGRP_SIP ) )
   {
      bRes = MxTraceEnableNode( "/SipStack", true ); CCTK_ASSERT( bRes );

      if ( !UTIL_BMP_IS_ENABLED_ALL( uGroup, CCTK_TRACEGRP_TRANSPORT ) )
      {
         bRes = MxTraceDisableNode( "/SipStack/SipTransport", true );
         CCTK_ASSERT( bRes );
      }
   }
   else if ( UTIL_BMP_IS_ENABLED_ALL( uGroup, CCTK_TRACEGRP_TRANSPORT ) )
   {
      bRes = MxTraceEnableNode( "/SipStack/SipTransport", true );
      CCTK_ASSERT( bRes );
   }

   if ( UTIL_BMP_IS_ENABLED_ALL( uGroup, CCTK_TRACEGRP_SDP ) )
   {
      bRes = MxTraceEnableNode( "/SdpParser", true ); CCTK_ASSERT( bRes );
   }

   if ( UTIL_BMP_IS_ENABLED_ALL( uGroup, CCTK_TRACEGRP_MISC ) )
   {
      bRes = MxTraceEnableNode( "/Assert", true ); CCTK_ASSERT( bRes );
      bRes = MxTraceEnableNode( "/Framework", true ); CCTK_ASSERT( bRes );
   }
}
#endif /* #if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL ) */
#endif /* #if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED ) */

/***********************************************************************************
**  FUNCTION:   configStack
**
**  PURPOSE:    Configure the SIP stack (and open listening ports).
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      All default configurations must be explicitly set to ensure
**              parameters are re-configured.
***********************************************************************************/
CFGSTATUS CfgEngine::configStack( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configStack()", this );

   CFGSTATUS eStatus;

#if CCTKCFG_TLS
   /* Configure TLS context factory.
   ** NOTE: This must be the first to be configured due to stack's requirement.
   */
   eStatus = configTlsContext();
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - configStack: "
         "Error configuring TLS context factory (%d).",
         eStatus );
      return eStatus;
   }
#endif

   /* Configure M5T Framework */
   eStatus = configMxFramework();
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - configStack: "
         "Error configuring MxFramework (%d).",
         eStatus );
      return eStatus;
   }

   /* Configure SCE */
   eStatus = configSce();
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - configStack: "
         "Error configuring SCE (%d).",
         eStatus );
      return eStatus;
   }

   /* Configure SIP-UA.
   ** NOTE: This must be the last to be called.
   */
   eStatus = configSipUa();
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - configStack: "
         "Error configuring SIP-UA (%d).",
         eStatus );
      return eStatus;
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   deConfigStack
**
**  PURPOSE:    De-configure the SIP stack (and close all listening ports).
**
**  PARAMETERS:
**              bShutdown - True to de-configure the stack for shutdown.
**                          False to de-configure the stack for re-configuring.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::deConfigStack( BOS_BOOL bShutdown )
{
   CCTK_TRACED2( "CfgEngine(%p) - deConfigStack(%d)", this, bShutdown );

   /* De-configure the SIP-UA stack */
   CFGSTATUS eStatus = deConfigSipUa( bShutdown );
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - deConfigStack: "
         "Error de-configuring the SIP-UA stack (%d).",
         eStatus );
   }

   /* De-configure the M5T Framework */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = deConfigMxFramework( bShutdown );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - deConfigStack: "
            "Error de-configuring the M5T Framework (%d).",
            eStatus );
      }
   }

   return eStatus;
}

#if CCTKCFG_TLS
/***********************************************************************************
**  FUNCTION:   configTlsContext
**
**  PURPOSE:    Configure the TLS context factory for the SIP stack.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      All default configurations must be explicitly set to ensure
**              parameters are re-configured.
***********************************************************************************/
CFGSTATUS CfgEngine::configTlsContext( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configTlsContext()", this );

   if ( UTIL_BMP_IS_ENABLED_ALL( CFM_CFG.pSys->xtl.net.gen.transport.uSigTrans,
                                 CCTK_TRANSPORT_TLS ) &&
        !CFM_CFG.pSys->in.bTlsEnabled )
   {
      CCTK_TRACEI0( "CfgEngine - configTlsContext: Invalid TLS config." );
      return eCFGSTATUS_BADPARM;
   }

   /* Return success even if TLS is disabled */
   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   if ( CFM_CFG.pSys->in.bTlsEnabled )
   {
      ISipCoreConfig *pConfig = NULL;
      mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pConfig );
      if ( MX_RIS_F( res ) || ( pConfig == NULL ) )
      {
         CCTK_TRACEI1(
            "CfgEngine - configTlsContext: Error configuring SIP stack (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }

      if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
      {
         CTlsContext tlsCxtClient, tlsCxtServer;
         CFGSTATUS eCfgStatus = cfgCvtTlsCfgToMx( CFM_CFG.pSys->xtl.net.gen.tls,
                                                  tlsCxtClient,
                                                  tlsCxtServer );
         if ( eCfgStatus != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI1(
               "CfgEngine - configTlsContext: Error reading TLS config (%d).",
               eCfgStatus );
            eStatus = eCFGSTATUS_BADPARM;
         }

         /* Retrieve the stack's TLS context factory */
         ISipTlsContextFactory *pTlsCxtFactory = NULL;
         if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
         {

            res = pConfig->GetTlsContextFactory( pTlsCxtFactory );
            if ( MX_RIS_F( res ) || pTlsCxtFactory == NULL )
            {
               CCTK_TRACEI1( "CfgEngine - configTlsContext: "
                  "Error retriving stack's TLS context factory (%d).",
                  res );
               eStatus = eCFGSTATUS_INTERR;
            }
         }

         /* Configure the default client TLS context */
         if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
         {
            res = pTlsCxtFactory->SetDefaultTlsClientContextS( tlsCxtClient );
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - configTlsContext: "
                  "Error configuring default client TLS context (%d).",
                  res );
               eStatus = eCFGSTATUS_INTERR;
            }
         }

         /* Configure the default server TLS context */
         if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
         {
            res = pTlsCxtFactory->SetDefaultTlsServerContextS( tlsCxtServer );
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - configTlsContext: "
                  "Error configuring default server TLS context (%d).",
                  res );
               eStatus = eCFGSTATUS_INTERR;
            }
         }

         if ( pTlsCxtFactory != NULL )
         {
            pTlsCxtFactory->ReleaseIfRef();
            pTlsCxtFactory = NULL;
         }

         pConfig->ReleaseIfRef();
      }
   }

   return eStatus;
}
#endif /* #if CCTKCFG_TLS */

/***********************************************************************************
**  FUNCTION:   configMxFramework
**
**  PURPOSE:    Configure the M5T framework.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      All default configurations must be explicitly set to ensure
**              parameters are re-configured.
***********************************************************************************/
CFGSTATUS CfgEngine::configMxFramework( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configMxFramework()", this );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const CFGSYSXTL *pCfg = &( CFM_CFG.pSys->xtl );

   /* NOTE: Trace settings are configured at the time before the stack
   ** starts up. No trace config is required here.
   */

#if CCTKCFG_MEMORY_MONITORING
   /* Configure memory monitoring support */
   mxt_pfnEventObserver pEvtObs = ( mxt_pfnEventObserver )( utilMemEvtObsGet() );
   CCTK_ASSERT( pEvtObs != NULL );
   if ( pEvtObs == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configMxFramework: "
         "Error retriving stack event observer." );
   }
   else
   {
      if ( pCfg->sec.dos.memProt.uThreshold > 0 )
      {
         RegisterEventObserver( pEvtObs );
      }
      else
      {
         UnregisterEventObserver( pEvtObs );
      }
   }
#endif

   /* Configure random seed to be used for randomized SIP tag generation.
   ** Current time is appended to the provisioned random seed to enhance randomness.
   */
   CTime currentTime;
   uint32_t uDate = 0, uTime = 0;
   currentTime.GetJulianDateAndTime( uDate, uTime );

   CString machineId;
   mxt_result res =
      machineId.Format( 0, "%s%d%d", pCfg->loc.gen.pcSeed, uTime, uDate );
   if ( MX_RIS_F( res ) )
   {
      /* Include random seed only if formatting time and date fails */
      machineId = reinterpret_cast< char *const >( pCfg->loc.gen.pcSeed );
   }
   CUuidGenerator::SetMachineId( machineId );

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   deConfigMxFramework
**
**  PURPOSE:    De-configure the M5T Framework.
**
**  PARAMETERS:
**              bShutdown - True to de-configure the Framework for shutdown.
**                          False to de-configure the Framework for re-configuring.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::deConfigMxFramework( BOS_BOOL bShutdown )
{
   CCTK_TRACED2( "CfgEngine(%p) - deConfigMxFramework(%d)", this, bShutdown );

#if CCTKCFG_MEMORY_MONITORING
   const CFGSYSXTL *pCfg = &( CFM_CFG.pSys->xtl );

   /* Remove stack event observer for CCTK shutdown */
   if ( bShutdown && pCfg->sec.dos.memProt.uThreshold > 0 )
   {
      mxt_pfnEventObserver pEvtObs =
              (mxt_pfnEventObserver) utilMemEvtObsGet();
      CCTK_ASSERT( pEvtObs != NULL );
      if ( pEvtObs == NULL )
      {
         CCTK_TRACEI0( "CfgEngine - deConfigMxFramework: "
            "Error retriving stack event observer." );
      }
      else
      {
         UnregisterEventObserver( pEvtObs );
      }
   }
#endif

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   configSce
**
**  PURPOSE:    Configure SCE.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      All default configurations must be explicitly set to ensure
**              parameters are re-configured.
***********************************************************************************/
CFGSTATUS CfgEngine::configSce( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSce()", this );

   /* Retrieve the SCE configuration interface */
   ISceCoreConfig *pSceCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSceCoreConfig, NULL, &pSceCfg );
   if ( MX_RIS_F( res ) || ( pSceCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSce: "
         "Error retrieving SCE config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   const CFGSYSXTL *pCfg = &( CFM_CFG.pSys->xtl );

   /* --- Configuration which returns status --- */
   /* NOTE: The following SCE configurations are performed at CE initialization.
   ** - OOD request handler (ISceCoreConfig::RegisterRequestHandler())
   ** - Authentication manager (ISceUserAuthentication::SetManager())
   ** - Default call/notification handler (ISceCoreConfig::GetHandlerInterface())
   **   - ISceNotifierHandler::SetManager()
   **   - IUaSspCallHandler::SetManager()
   */

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Initialize random number generator */
      BOS_TIME_MS tick;
      bosTimeGetMs( &tick );
      BOS_UINT32 uSeed = static_cast< BOS_UINT32 >( tick );
      pSceCfg->SetSeed( uSeed );

      /* FIXME: Supported list config provisioning at CCTK API */
      pSceCfg->RegisterSupportedPayload( eMEDIATYPE_APPLICATION_SDP );
      pSceCfg->RegisterSupportedPayload( eMEDIATYPE_MESSAGE_SIPFRAG );
      pSceCfg->RegisterSupportedPayload(
         eMEDIATYPE_APPLICATION_SIMPLE_MESSAGE_SUMMARY );
      pSceCfg->RegisterSupportedPayload( eMEDIATYPE_APPLICATION_XML );
#if CCTKCFG_MESSAGE_PASS_THROUGH
      pSceCfg->RegisterSupportedPayload( eMEDIATYPE_TEXT_XML_SMML );
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */      
      pSceCfg->RegisterSupportedEvent( eEVENTTYPE_MESSAGE_SUMMARY );
      pSceCfg->RegisterSupportedEvent( eEVENTTYPE_REFER );
      pSceCfg->RegisterSupportedEvent( eEVENTTYPE_REG );
      pSceCfg->RegisterSupportedEvent( eEVENTTYPE_DIALOG );
      pSceCfg->RegisterSupportedEvent( eEVENTTYPE_USR_AGT_PROF );

      /* Advertize the following extensions as supported if configured
      ** to do so.  For interoperability purposes, we may want to prevent
      ** advertizing those advanced extensions even though we can support
      ** them (feature is still enabled).
      */
      BOS_UINT32 uExt = pSceCfg->GetSupportedExtensions();
      
      if ( UTIL_BMP_IS_ENABLED_ALL( pCfg->net.prot.gen.uInterop,
                                    CCTK_INTEROP_ADVSUPEXT ) )
      {
         /* Add "answermode" extension */
         pSceCfg->RegisterSupportedExtension( eOPTIONTAG_ANSWERMODE );

         /* Add "from-change" extension */
         pSceCfg->RegisterSupportedExtension( eOPTIONTAG_FROM_CHANGE );

         /* Add "path" extension */
         pSceCfg->RegisterSupportedExtension( eOPTIONTAG_PATH );

         /* Add "outbound" extension - only if we are meant to use it */
         if ( pCfg->net.gen.transport.bKeepAlive )
         {
            pSceCfg->RegisterSupportedExtension( eOPTIONTAG_OUTBOUND );
         }
         /* Remove "outbound" if it was previously registered - this could occur after a reconfig.
         */
         else if ( UTIL_BMP_IS_ENABLED_ALL( uExt, eOPTIONTAG_OUTBOUND ) )
         {
            pSceCfg->RemoveSupportedExtension( eOPTIONTAG_OUTBOUND );
         }

         /* Add "join" extension */
         pSceCfg->RegisterSupportedExtension( eOPTIONTAG_JOIN );

         /* Add "early-session" extension */
         pSceCfg->RegisterSupportedExtension( eOPTIONTAG_EARLY_SESSION );

         /* Add "tdialog" extension */
         pSceCfg->RegisterSupportedExtension( eOPTIONTAG_TDIALOG );

         /* Add "precondition" extension */
         pSceCfg->RegisterSupportedExtension( eOPTIONTAG_PRECONDITION );

         /* Add "histinfo" extension */
         pSceCfg->RegisterSupportedExtension( eOPTIONTAG_HISTINFO );
      }

      /* Remove "gruu" extension if GRUU is disabled */
      if ( pCfg->net.prot.gen.gruu.eMode == eCCTKGRUUMODE_DISABLED )
      {
         if ( UTIL_BMP_IS_ENABLED( uExt, eOPTIONTAG_GRUU ) )
         {
            pSceCfg->RemoveSupportedExtension( eOPTIONTAG_GRUU );
         }
      }

      /* Add support for INFO method, may be used to check the status of a dialog. */
      pSceCfg->RegisterSupportedMethod( eMETHODTYPE_INFO );
   }

   /* End of SCE configuration */
   pSceCfg->ReleaseIfRef();
   pSceCfg = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSipUa
**
**  PURPOSE:    Configure the SIP-UA stack.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      All default configurations must be explicitly set to ensure
**              parameters are re-configured.
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUa( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUa()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUa: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const CFGSYSXTL *pCfg = &( CFM_CFG.pSys->xtl );

   /* --- Configuration which returns status --- */
   /* Configure the maximum payload size. The default size is 1024 bytes.
   ** Set it differently if an higher value is needed.
   */
   res = pCoreCfg->SetMaxPayloadSize( CFGENGINE_MAX_PAYLOAD_SIZE );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUa: "
         "Error setting maximum payload size (%d).",
         res );
      eStatus = eCFGSTATUS_INTERR;
   }

   /* Configure connection parameters */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSipUaCnx();
   }

   /* Configure SIP timer parameters */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configSipUaTransTmr();
   }

   /* Configure header separation preferences */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = pCoreCfg->SetCommaSeparatedHeader( eHDR_VIA,
                                               ISipCoreConfig::eCRLF_SEPARATED );
      if ( MX_RIS_S( res ) )
      {
         res = pCoreCfg->SetCommaSeparatedHeader( eHDR_CONTACT,
                                                  ISipCoreConfig::eCRLF_SEPARATED );
      }
      if ( MX_RIS_S( res ) )
      {
         res = pCoreCfg->SetCommaSeparatedHeader( eHDR_ROUTE,
                                                  ISipCoreConfig::eCRLF_SEPARATED );
      }
      if ( MX_RIS_S( res ) )
      {
         res = pCoreCfg->SetCommaSeparatedHeader( eHDR_RECORD_ROUTE,
                                                  ISipCoreConfig::eCRLF_SEPARATED );
      }

      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSipUa: "
            "Error setting header separation preferences (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }

   /* Configure stack callback function for received (parsed) packet */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = pCoreCfg->SetPacketModifierCallback( &pktModifierCb );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSipUa: "
            "Error setting packet modifier callback (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }

   /* Configure and add local address interfaces */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = configLocAddr();
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSipUa: "
            "Error configuring and adding local address interfaces (%d).",
            eStatus );
      }
   }

   /* Configure and open listening ports */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = openListeningPort();
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSipUa: "
            "Error configuring and opening listening ports (%d).",
            eStatus );
      }
   }

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* NOTE: Return status is not important for the following configuration
      **       functions, assuming that we can always query the stack's config
      **       interface successfully (see function inside).
      */

      /* Configure SIP signaling behaviours */
      configSipUaSigBehave();

      /* Configure interoperability options */
      configSipUaInterop();

      /* Configure the User-Agent and Server header */
      configSipUaIdHdr();

      /* Configure Max-Forwards */
      configSipUaRoutingHdr();

      /* Configure supported DNS queries option */
      configSipUaDns();

      /* Configure NAT settings */
      configSipUaNat();

      /* Disable UDP/TCP failover support in the stack */
      pCoreCfg->SetFailoverMode( ISipCoreConfig::eFAILOVER_NO_TCP_ASSUMPTION );

      /* Configure transport support */
      int sTransportMask = cfgCvtTransportToMx( pCfg->net.gen.transport.uSigTrans );
      pCoreCfg->SetSupportedSipTransport( sTransportMask );

      pCoreCfg->SetUdpMaxSizeThreshold(
                     pCfg->net.gen.transport.uTcpFallbackThresh );

      /* Configure persistent connection retry settings */
      CSipPersistentConnectionList* pPersistCnxList = 
         pCoreCfg->GetPersistentConnectionList();
      CCTK_ASSERT( pPersistCnxList != NULL );
      pPersistCnxList->SetRetryConfig( 
         CSipPersistentConnectionList::eRETRY_PERIODIC,
         CFGENGINE_PST_CNX_RETRY_DELAY,
         CFGENGINE_PST_CNX_MAX_RETRY_PERIOD );

      /* Configure packet logger */
      closePktLogger();
      mpPktLogger = preCfgPktLog( pCfg->loc.debug.pktLog );
      if ( mpPktLogger != NULL )
      {
         pCoreCfg->SetSipDataLogger( this );
      }

      /* Configure authentication service behavior when an unknown 'qop'
      ** is received in a challenge.
      */
      pCoreCfg->SetClientAuthUnknownQopBehavior(
                     ISipDigestClientAuthSvc::eIGNORE_UNKNOWN_QOP );
   }

   /* End of SIP-UA configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSipUaCnx
**
**  PURPOSE:    Configure the SIP-UA stack's connection parameters.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUaCnx( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUaCnx()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaCnx: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const CCTKCNXCFG *pCfg = &( CFM_CFG.pSys->xtl.net.gen.cnx );

   /* --- Configuration which returns status --- */
   BOS_UINT32 uMinCnx, uMaxCnx, uMaxIdleTime;
   preCfgCnx( *pCfg, uMinCnx, uMaxCnx, uMaxIdleTime );
   res = pCoreCfg->SetConnectionParameters( uMinCnx, uMaxCnx, uMaxIdleTime );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaCnx: "
         "Error setting connection parameters (%d).",
         res );
      eStatus = eCFGSTATUS_INTERR;
   }

   /* --- Configuration which does not return status --- */

   /* End of SIP-UA configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSipUaIdHdr
**
**  PURPOSE:    Configure the SIP-UA stack's User-Agent Identifier headers.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUaIdHdr( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUaIdHdr()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaIdHdr: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   const CCTKUAIDHDR *pCfg = &( CFM_CFG.pSys->xtl.net.prot.gen.uaIdHdr );

   /* --- Configuration which returns status --- */

   /* --- Configuration which does not return status --- */
   CString uaName, svrName;
   BOS_BOOL bAddStackVer;
   preCfgUaId( *pCfg, uaName, svrName, bAddStackVer );
   pCoreCfg->SetApplicationId( uaName, &svrName );
   pCoreCfg->AddStackVersionToId( bAddStackVer == BOS_TRUE );

   /* End of SIP-UA configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   configSipUaRoutingHdr
**
**  PURPOSE:    Configure the SIP-UA stack's routing headers.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUaRoutingHdr( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUaRoutingHdr()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaRoutingHdr: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   const CCTKROUTINGHDR *pCfg = &( CFM_CFG.pSys->xtl.net.prot.gen.routingHdr );

   /* --- Configuration which returns status --- */

   /* --- Configuration which does not return status --- */
   /* Configure Max-Forwards */
   BOS_UINT32 uMaxFwds = ( pCfg->uMaxFwds == 0 ) ? 70 : pCfg->uMaxFwds;
   pCoreCfg->SetMaxForwards( uMaxFwds );

   /* End of SIP-UA configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   configSipUaSigBehave
**
**  PURPOSE:    Configure the SIP-UA stack's signaling behaviours.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUaSigBehave( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUaSigBehave()", this );

   const CCTKSIGBEHAVE *pCfg = &( CFM_CFG.pSys->xtl.net.prot.gen.sigBehave );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaSigBehave: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   /* --- Configuration which returns status --- */
   /* Configure to reduce the size of the packet.
   **       - Use short form headers when possible.
   **       - Group headers.
   **/
   if ( UTIL_BMP_IS_ENABLED_ALL( pCfg->uTx, CCTK_SIGBEHVTX_REDUCESIZE ) )
   {
      res = pCoreCfg->SetHeaderFormPreference( ISipCoreConfig::ePREFER_SHORT );

      if ( MX_RIS_S( res ) )
      {
         res = pCoreCfg->SetCommaSeparatedHeader( eHDR_VIA,
                                                  ISipCoreConfig::eCOMMA_SEPARATED );

      }
      if ( MX_RIS_S( res ) )
      {
         res = pCoreCfg->SetCommaSeparatedHeader( eHDR_ROUTE,
                                                  ISipCoreConfig::eCOMMA_SEPARATED );

      }
      if ( MX_RIS_S( res ) )
      {
         res = pCoreCfg->SetCommaSeparatedHeader( eHDR_RECORD_ROUTE,
                                                  ISipCoreConfig::eCOMMA_SEPARATED );

      }

      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSipUaSigBehave: "
            "Error setting header reduction preferences (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Configure quoted display-name option */
      CNameAddr::SetForceQuotedDisplayName(
                           UTIL_BMP_IS_ENABLED_ALL( pCfg->uTx,
                                                    CCTK_SIGBEHVTX_FORCEQUOTES ) );

      /* Configure if star character '*' needs to be escaped in SIP-URI */
      g_bESCAPE_STAR_CHARACTER_IN_SIP_URI =
                           UTIL_BMP_IS_ENABLED_ALL( pCfg->uTx,
                                                    CCTK_SIGBEHVTX_ESCAPESTAR );
 
     /* Configure if pound character '#' needs to be escaped in SIP-URI */
      g_bESCAPE_POUND_CHARACTER_IN_URI =
                           UTIL_BMP_IS_ENABLED_ALL( pCfg->uTx,
                                                    CCTK_SIGBEHVTX_ESCAPEPOUND );

      /* Configure usage of FQDN in the Via header */
      if ( UTIL_BMP_IS_ENABLED_ALL( pCfg->uTx, CCTK_SIGBEHVTX_NOFQDNINVIA ) ||
           UTIL_BMP_IS_ENABLED_ALL( pCfg->uTx, CCTK_SIGBEHVTX_REDUCESIZE ) )
      {
         pCoreCfg->SetViaAddressType( ISipCoreConfig::eFORCE_IP_ADDRESS );
      }

      /* Configure to reduce the size of the packet.  Remove headers of no
      ** real interest */
      if ( UTIL_BMP_IS_ENABLED_ALL( pCfg->uTx, CCTK_SIGBEHVTX_REDUCESIZE ) )
      {
         pCoreCfg->AddUserAgentToRequests( false );
         pCoreCfg->AddUserAgentToResponses( false );
         pCoreCfg->AddStackVersionToId( false );
      }
   }

   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSipUaInterop
**
**  PURPOSE:    Configure the SIP-UA stack's interoperability options.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUaInterop( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUaInterop()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaInterop: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const BOS_UINT32 uCfg = CFM_CFG.pSys->xtl.net.prot.gen.uInterop;

   /* --- Configuration which returns status --- */
   /* Configure the ACK branch-id checking option */
   res = pCoreCfg->SetBypassAckBranchIdCheck(
                        UTIL_BMP_IS_ENABLED_ALL( uCfg,
                                                 CCTK_INTEROP_ACKBRANCH ) );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaInterop: "
         "Error configuring the ACK branch-id checking option (%d).",
         res );
      eStatus = eCFGSTATUS_INTERR;
   }

#if CCTKCFG_TLS
   /* Indicate if subjectAltName check needs to be bypassed */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = pCoreCfg->SetBypassSubjectAltNameCheck(
                        UTIL_BMP_IS_ENABLED_ALL( uCfg,
                                                 CCTK_INTEROP_SUBJECTALTNAME ) );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSipUaInterop: "
            "Error setting subjectAltName check option (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }
#endif

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Indicate if single digit date and time is allowed in Date GMT format */
      g_bDATE_GMT_FORMAT_PER_RFC_3261 =
                        !UTIL_BMP_IS_ENABLED_ALL( uCfg,
                                                  CCTK_INTEROP_DATEGMTFORMAT );
   }

   /* Include the P-Access-Network-Info header if directed to do so
   */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      if ( UTIL_BMP_IS_ENABLED_ALL( uCfg,
                                    CCTK_INTEROP_ACCESS_NETWORK_HDR ) )
      {
         /* Only append a header if enabled and the string to append is not null
         ** */
         if ( ( CFG_SYS->xtl.net.prot.gen.pani.bIncludeNetworkInfo ) &&
              strlen( reinterpret_cast<const char *>(
                                 CFG_SYS->xtl.net.prot.gen.pani.pcNetworkInfo)) )
         {
            pCoreCfg->SetPaniHdr( reinterpret_cast<const char *>(
                                  CFG_SYS->xtl.net.prot.gen.pani.pcNetworkInfo ) );
         }
      }
   }

   /* If configuration requires it indicate that the user-agent header should
   ** be added to all messages then make sure it's added to both requests and
   ** responses.
   */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      if ( UTIL_BMP_IS_ENABLED_ALL( uCfg,
                                    CCTK_INTEROP_USER_AGENT_IN_ALL ) )
      {
         pCoreCfg->AddUserAgentToRequests( BOS_TRUE );
         pCoreCfg->AddUserAgentToResponses( BOS_TRUE );
      }
   }
  

   /* End of SIP-UA configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSipUaDns
**
**  PURPOSE:    Configure the SIP-UA stack's DNS settings.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUaDns( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUaDns()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaDns: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   /* --- Configuration which returns status --- */

   /* --- Configuration which does not return status --- */
   /* Configure supported DNS queries option.
   ** NOTE: SRV queries cannot be disabled if NAPTR queries are enabled.
   */
   int sDnsMask = cfgCvtDnsModeToMx( CFM_CFG.pSys->xtl.net.prot.gen.eDnsMode );
   pCoreCfg->SetSupportedDnsQueries( sDnsMask );

   /* End of SIP-UA configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   configSipUaTransTmr
**
**  PURPOSE:    Configure the SIP-UA stack's transaction timer settings.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUaTransTmr( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUaTransTmr()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaTransTmr: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* --- Configuration which returns status --- */
   BOS_UINT32 uT1, uT2, uTimeout, uT4, uB, uD, uF, uH, uJ;
   preCfgTransTmr( CFM_CFG.pSys->xtl.net.prot.tmr.trans,
                   uT1,
                   uT2,
                   uTimeout,
                   uT4,
                   uB,
                   uD,
                   uF,
                   uH,
                   uJ );

   res = pCoreCfg->SetT1( uT1 );
   if ( MX_RIS_S( res ) )
   {
      res = pCoreCfg->SetT2( uT2 );
   }
   if ( MX_RIS_S( res ) )
   {
      res = pCoreCfg->SetTimeoutTimer( uTimeout );
   }
   if ( MX_RIS_S( res ) )
   {
      res = pCoreCfg->SetT4( uT4 );
   }
   if ( MX_RIS_S( res ) )
   {
      res = pCoreCfg->SetTimerB( uB );
   }
   if ( MX_RIS_S( res ) )
   {
      res = pCoreCfg->SetTimerD( uD );
   }
   if ( MX_RIS_S( res ) )
   {
      res = pCoreCfg->SetTimerF( uF );
   }
   if ( MX_RIS_S( res ) )
   {
      res = pCoreCfg->SetTimerH( uH );
   }
   if ( MX_RIS_S( res ) )
   {
      res = pCoreCfg->SetTimerJ( uJ );
   }

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaTransTmr: "
         "Error setting SIP transaction timer parameters (%d).",
         res );
      eStatus = eCFGSTATUS_INTERR;
   }

   /* --- Configuration which does not return status --- */

   /* End of SIP-UA configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSipUaNat
**
**  PURPOSE:    Configure the SIP-UA stack's NAT settings.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configSipUaNat( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configSipUaNat()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSipUaNat: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   /* --- Configuration which returns status --- */

   /* --- Configuration which does not return status --- */
   /* Configure rport */
   pCoreCfg->AddViaRportParam( !CFM_CFG.pSys->xtl.sec.nat.gen.cfg.bNoRport );

   /* End of SIP-UA configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   deConfigSipUa
**
**  PURPOSE:    De-configure the SIP-UA stack (and close all listening ports).
**
**  PARAMETERS:
**              bShutdown - True to de-configure the stack for shutdown.
**                          False to de-configure the stack for re-configuring.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::deConfigSipUa( BOS_BOOL bShutdown )
{
   CCTK_TRACED2( "CfgEngine(%p) - deConfigSipUa(%d)", this, bShutdown );

   /* Close and destroy packet logger */
   closePktLogger();

   /* Close all listening ports */
   CFGSTATUS eStatus = closeListeningPort();
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - deConfigSipUa: "
         "Error closing listening ports (%d).",
         eStatus );
   }

   /* Remove local address interfaces immediately for CCTK shutdown */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) && bShutdown )
   {
      eStatus = deConfigLocAddr();
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - deConfigSipUa: "
            "Error removing local address interfaces from the stack (%d).",
            eStatus );
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configLocAddr
**
**  PURPOSE:    Configure and add local address interfaces to the stack.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configLocAddr( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - configLocAddr()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - configLocAddr: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const CFGSENETGEN *pCfg = &( CFM_CFG.pSys->xtl.net.gen );

   BOS_UINT32 uIdx;

   for ( uIdx = 0; uIdx < pCfg->host.uNum; uIdx++ )
   {
      /* Prepare local address information, i.e. local address, FQDN, and
      ** accessible network.
      */
      CSocketAddr locAddr;
      CVector< CString > *pVecStrFqdn = NULL;
      CVector< ISipCoreConfig::SAccessibleNetwork > *pVecAccessNet = NULL;
      eStatus = preCfgLocAddr( pCfg->host.addr[uIdx],
                               locAddr,
                               pVecStrFqdn,
                               pVecAccessNet );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI2( "CfgEngine - configLocAddr: "
            "Error preparing local address info (%d, %lu).",
            eStatus,
            uIdx );
         break;
      }

      /* Add local address information to the stack's interface list */
      res = pCoreCfg->AddLocalAddress( locAddr,
                                       pVecStrFqdn,
                                       pVecAccessNet,
                                       mLocAddr[uIdx].addrId );
      pVecStrFqdn = NULL;
      pVecAccessNet = NULL;
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI2( "CfgEngine - configLocAddr: "
            "Error adding local address list (%d, %lu).",
            res,
            uIdx );
         eStatus = eCFGSTATUS_INTERR;
         break;
      }

      /* We assume that the assigned opaque would never be 0 as that is what
      ** we used for uninitialized opaque.
      */
      CCTK_ASSERT( mLocAddr[uIdx].addrId != 0 );
   }

   /* End of configuration */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   /* Clean up local address interfaces on failure */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      /* NOTE: Return code is not checked as it's not important here */
      deConfigLocAddr();
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   deConfigLocAddr
**
**  PURPOSE:    De-configure and remove local address interfaces from the stack.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::deConfigLocAddr( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - deConfigLocAddr()", this );

   /* Query for a stack configuration object */
   ISipCoreConfig *pConfig = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pConfig );
   if ( MX_RIS_F( res ) || ( pConfig == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - deConfigLocAddr: "
         "Error obtaining a stack configuration object. (%d)",
         res );
      return eCFGSTATUS_INTERR;
   }

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_HOSTADDR; uIdx++ )
   {
      if ( mLocAddr[uIdx].addrId != 0 )
      {
         /* Remove the local address configuration from the SIP stack.
         ** NOTE: Return code is not checked as this function may be called twice
         ** when shutting down CCTK while de-configuring. FIXME?
         */
         pConfig->RemoveLocalAddress( mLocAddr[uIdx].addrId );
         mLocAddr[uIdx].addrId = 0;
      }
   }

   /* End of de-configuration */
   pConfig->ReleaseIfRef();
   pConfig = NULL;

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   openListeningPort
**
**  PURPOSE:    Configure and open all listening ports.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::openListeningPort( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - openListeningPort()", this );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - openListeningPort: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const CFGSENETGEN *pCfg = &( CFM_CFG.pSys->xtl.net.gen );

   ISipCoreConfig::EIpVersionConfig eIpVer =
      static_cast< ISipCoreConfig::EIpVersionConfig >( CFGENGINE_INVALID_IP_TYPE );

   mxt_opaque opqParm;
   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < pCfg->host.uNum; uIdx++ )
   {
      /* Listen on UDP if necessary */
      if ( UTIL_BMP_IS_ENABLED_ALL( pCfg->transport.uSigTrans,
                                    CCTK_TRANSPORT_UDP ) &&
           ( mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_CLOSED ||
             mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_CLOSING ) )
      {
         /* Set an opaque parameter so we can tell through EvCommandResult when
         ** a listen work or failed.
         */
         opqParm = reinterpret_cast< mxt_opaque>(
            ( eCFG_TRANSPORTID_LISTEN_UDP << 16 ) | uIdx );
         res = pCoreCfg->ListenA( mLocAddr[uIdx].addrId,
                                  cfgCvtLocPortToMx( pCfg->port, BOS_FALSE ),
                                  eUDP,
                                  this,
                                  opqParm,
                                  mLocAddr[uIdx].cnx.udp.listen.socketId );
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI2( "CfgEngine - openListeningPort: "
               "Error opening listening UDP socket (%d, %lu).",
               res,
               uIdx );
         }
         else
         {
            mLocAddr[uIdx].cnx.udp.listen.eState = eCFG_SOCKETSTATE_OPENING;
         }
      }

      /* Listen on TCP if necessary */
      if ( UTIL_BMP_IS_ENABLED_ALL( pCfg->transport.uSigTrans,
                                    CCTK_TRANSPORT_TCP ) &&
           ( mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_CLOSED ||
             mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_CLOSING ) )
      {
         /* Set an opaque parameter so we can tell through EvCommandResult when
         ** a listen work or failed.
         */
         opqParm = reinterpret_cast< mxt_opaque >(
            ( eCFG_TRANSPORTID_LISTEN_TCP << 16 ) | uIdx );
         res = pCoreCfg->ListenA( mLocAddr[uIdx].addrId,
                                  cfgCvtLocPortToMx( pCfg->port, BOS_FALSE ),
                                  eTCP,
                                  this,
                                  opqParm,
                                  mLocAddr[uIdx].cnx.tcp.listen.socketId );
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI2( "CfgEngine - openListeningPort: "
               "Error opening listening TCP socket (%d, %lu).",
               res,
               uIdx );
         }
         else
         {
            mLocAddr[uIdx].cnx.tcp.listen.eState = eCFG_SOCKETSTATE_OPENING;
         }
      }

#if CCTKCFG_TLS
      /* Listen on TLS if necessary */
      if ( CFM_CFG.pSys->in.bTlsEnabled &&
           ( mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_CLOSED ||
             mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_CLOSING ) )
      {
         /* Set an opaque parameter so we can tell through EvCommandResult when
         ** a listen work or failed.
         */
         opqParm = reinterpret_cast< mxt_opaque>(
            ( eCFG_TRANSPORTID_LISTEN_TLS << 16 ) | uIdx );
         res = pCoreCfg->ListenA( mLocAddr[uIdx].addrId,
                                  cfgCvtLocPortToMx( pCfg->port, BOS_TRUE ),
                                  eTLS,
                                  this,
                                  opqParm,
                                  mLocAddr[uIdx].cnx.tls.listen.socketId );
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI2( "CfgEngine - openListeningPort: "
               "Error opening listening TLS socket (%d, %lu).",
               res,
               uIdx );
         }
         else
         {
            mLocAddr[uIdx].cnx.tls.listen.eState = eCFG_SOCKETSTATE_OPENING;
         }
      }
#endif

      /* Configure IP network address type */
#if CCTKCFG_IPV6
      if ( bosIpAddrIsV4( &( pCfg->host.addr[uIdx].ipAddr ) ) )
      {
         if ( eIpVer == ISipCoreConfig::eCONFIG_IPV6 )
         {
             eIpVer = ISipCoreConfig::eCONFIG_IPV4_AND_IPV6;
         }
         else if ( eIpVer == CFGENGINE_INVALID_IP_TYPE )
         {
             eIpVer = ISipCoreConfig::eCONFIG_IPV4;
         }
      }
      else
      {
         if ( eIpVer == ISipCoreConfig::eCONFIG_IPV4 )
         {
             eIpVer = ISipCoreConfig::eCONFIG_IPV4_AND_IPV6;
         }
         else if ( eIpVer == CFGENGINE_INVALID_IP_TYPE )
         {
             eIpVer = ISipCoreConfig::eCONFIG_IPV6;
         }
      }
#else
      eIpVer = ISipCoreConfig::eCONFIG_IPV4;
#endif
   }

   /* Configure IP version support */
   pCoreCfg->SetSupportedIpVersion( eIpVer );

   /* End of opening listening ports */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   /* Clean-ups on failure */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      /* NOTE: no return code check here as it's not important */
      closeListeningPort();
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   closeListeningPort
**
**  PURPOSE:    Close opened listening ports with the specified transports.
**
**  PARAMETERS:
**              uTransport - Transports to be closed.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::closeListeningPort( 
                                 BOS_UINT32 uTransport /* = CCTK_TRANSPORT_ALL */ )
{
   CCTK_TRACED2( "CfgEngine(%p) - closeListeningPort(%lx)", this, uTransport );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - closeListeningPort: "
         "Error retrieving SIP stack config interface (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   mxt_opaque opqParm;
   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_HOSTADDR; uIdx++ )
   {
      if ( mLocAddr[uIdx].addrId == 0 )
      {
         /* All ports are closed on this address interface. Go onto next one. */
         continue;
      }

      /* Stop listening on the UDP port if it is open */
      if ( UTIL_BMP_IS_ENABLED_ALL( uTransport, CCTK_TRANSPORT_UDP ) &&
           ( mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_OPENED ||
             mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_OPENING ) )
      {
         /* Set an opaque parameter so we can tell through EvCommandResult when
         ** stopping listen work or failed.
         */
         opqParm = reinterpret_cast< mxt_opaque>(
            ( eCFG_TRANSPORTID_STOP_UDP << 16 ) | uIdx );
         res = pCoreCfg->StopListeningA( mLocAddr[uIdx].cnx.udp.listen.socketId,
                                         this,
                                         opqParm );
         CCTK_ASSERT( MX_RIS_S( res ) );
         mLocAddr[uIdx].cnx.udp.listen.eState = eCFG_SOCKETSTATE_CLOSING;
      }

      /* Stop listening on the TCP port if it is open */
      if ( UTIL_BMP_IS_ENABLED_ALL( uTransport, CCTK_TRANSPORT_TCP ) &&
           ( mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_OPENED ||
             mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_OPENING ) )
      {
         /* Set an opaque parameter so we can tell through EvCommandResult when
         ** stopping listen work or failed.
         */
         opqParm = reinterpret_cast< mxt_opaque>(
            ( eCFG_TRANSPORTID_STOP_TCP << 16 ) | uIdx );
         res = pCoreCfg->StopListeningA( mLocAddr[uIdx].cnx.tcp.listen.socketId,
                                         this,
                                         opqParm );
         CCTK_ASSERT( MX_RIS_S( res ) );
         mLocAddr[uIdx].cnx.tcp.listen.eState = eCFG_SOCKETSTATE_CLOSING;
      }

#if CCTKCFG_TLS
      /* Stop listening on the TLS port if it is open */
      if ( UTIL_BMP_IS_ENABLED_ALL( uTransport, CCTK_TRANSPORT_TLS ) &&
           CFM_CFG.pSys->in.bTlsEnabled &&
           ( mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_OPENED ||
             mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_OPENING ) )
      {
         /* Set an opaque parameter so we can tell through EvCommandResult when
         ** stopping listen work or failed.
         */
         opqParm = reinterpret_cast< mxt_opaque>(
            ( eCFG_TRANSPORTID_STOP_TLS << 16 ) | uIdx );
         res = pCoreCfg->StopListeningA( mLocAddr[uIdx].cnx.tls.listen.socketId,
                                         this,
                                         opqParm );
         CCTK_ASSERT( MX_RIS_S( res ) );
         mLocAddr[uIdx].cnx.tls.listen.eState = eCFG_SOCKETSTATE_CLOSING;
      }
#endif

      /* NOTE: We defer removing local address interfaces
      ** until all ports are closed.
      */
   }

   /* End of closing listening ports */
   pCoreCfg->ReleaseIfRef();

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   updCfgState
**
**  PURPOSE:    Update configuration state based on current listening port status.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::updCfgState( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - updCfgState()", this );

   /* Check if all listening operations are finished on all address interfaces */
   BOS_BOOL bAllListenPortsClosed = BOS_TRUE;
   BOS_UINT32 uIdx;
   CCTKSTATUS eCctkStatus = eCCTKSTATUS_SUCCESS;

   for ( uIdx = 0; uIdx < CCTKCFG_MAX_HOSTADDR; uIdx++ )
   {
      if ( mLocAddr[uIdx].addrId == 0 )
      {
         /* All ports are closed on this address interface. Go onto next one. */
         continue;
      }

      if ( mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_OPENING ||
           mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_CLOSING ||
           mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_OPENING ||
           mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_CLOSING
#if CCTKCFG_TLS
           || mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_OPENING
           || mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_CLOSING
#endif
         )
      {
         /* Socket operation is still in progress. Don't update config state. */
         return;
      }

      /* All socket operations are done at the current address interface */
      if ( bAllListenPortsClosed &&
           ( mLocAddr[uIdx].cnx.udp.listen.eState == eCFG_SOCKETSTATE_OPENED ||
             mLocAddr[uIdx].cnx.tcp.listen.eState == eCFG_SOCKETSTATE_OPENED
#if CCTKCFG_TLS
             || mLocAddr[uIdx].cnx.tls.listen.eState == eCFG_SOCKETSTATE_OPENED
#endif
           ) )
      {
         bAllListenPortsClosed = BOS_FALSE;
      }
   }

   /* Update the configuration state */
   switch ( meCfgState )
   {
   case eCFG_STATE_CFGING:
      {
         if ( !bAllListenPortsClosed )
         {
            meCfgState = eCFG_STATE_CFGED;
            eCctkStatus = eCCTKSTATUS_SUCCESS;

            tkmEvent( guCeInvalidGuid,
                      guCeInvalidGuid,
                      eCCTKEVT_FN_CONFIG_SUCCESS,
                      reinterpret_cast< void* >( &eCctkStatus ) );
         }
         else
         {
            /* Failed to open listening ports */
            meCfgState = eCFG_STATE_NOTCFGED;
            eCctkStatus = eCCTKSTATUS_BADPARM;

            /* NOTE: No return code check here as there may be a case where the local
            ** address interface may have been removed already during shutdown.
            */
            deConfigLocAddr();

            tkmEvent( guCeInvalidGuid,
                      guCeInvalidGuid,
                      eCCTKEVT_FN_CONFIG_FAIL,
                      reinterpret_cast< void* >( &eCctkStatus ) );
         }
      }
      break;
   case eCFG_STATE_DECFGING:
      {
         /* All ports must be closed at this point */
         CCTK_ASSERT( bAllListenPortsClosed );
         if ( !bAllListenPortsClosed )
         {
            CCTK_TRACEI0( "CfgEngine - updCfgState: Unexpected state." );
            meCfgState = eCFG_STATE_CFGED;
            eCctkStatus = eCCTKSTATUS_INTERR;

            tkmEvent( guCeInvalidGuid,
                      guCeInvalidGuid,
                      eCCTKEVT_FN_CONFIG_FAIL,
                      reinterpret_cast< void* >( &eCctkStatus ) );
         }
         else
         {
            /* NOTE: No return code check here as there may be a case where the local
            ** address interface may have been removed already during shutdown.
            */
            deConfigLocAddr();

            /* Config the stack and transition to "configuring" state on success */
            CFGSTATUS eStatus = configStack();
            if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
            {
               /* Transition to "configuring" state */
               meCfgState = eCFG_STATE_CFGING;
            }
            else
            {
               CCTK_TRACEI1(
                  "CfgEngine - updCfgState: Error configuring stack (%d).",
                  eStatus );
               meCfgState = eCFG_STATE_NOTCFGED;
               eCctkStatus = eCCTKSTATUS_INTERR;

               tkmEvent( guCeInvalidGuid,
                         guCeInvalidGuid,
                         eCCTKEVT_FN_CONFIG_FAIL,
                         reinterpret_cast< void* >( &eCctkStatus ) );
            }
         }
      }
      break;
   case eCFG_STATE_CFGED:
      {
         /* Enter here when there are persistent connection operations on the
         ** existing listening ports. Configuration state does not get changed
         ** on persistent connection operations.
         */
      }
      break;
   default:
      {
         /* Ignore unexpected socket operation events */
         CCTK_TRACEI2( "CfgEngine - updCfgState: "
            "Unexpected socket operation event (%d, %d).",
            meCfgState,
            bAllListenPortsClosed );
      }
      break;
   }
}

/***********************************************************************************
**  FUNCTION:   closePktLogger
**
**  PURPOSE:    Close the packet logging socket.
**
**  PARAMETERS: None
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::closePktLogger( void )
{
   CCTK_TRACED1( "CfgEngine(%p) - closePktLogger()", this );

   closeUdpSocket( mpPktLogger );
   CCTK_ASSERT( mpPktLogger == NULL );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI1( "CfgEngine - closePktLogger: "
         "Error retrieving SIP stack config interface (%d).",
         res );
   }
   else
   {
      /* Disable packet logging support at the stack to avoid unnecessary logging
      ** event callbacks.
      */
      pCoreCfg->SetSipDataLogger( NULL );

      pCoreCfg->ReleaseIfRef();
      pCoreCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   closeUdpSocket
**
**  PURPOSE:    Close a UDP socket (forcefully).
**
**  PARAMETERS:
**              rpSocket - UDP socket to be closed.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::closeUdpSocket( CUdpSocket *&rpSocket )
{
   CCTK_TRACED2( "CfgEngine(%p) - closeUdpSocket(%p)", this, rpSocket );

   if ( rpSocket != NULL )
   {
      /* NOTE: No return code check as it is not important here */
      rpSocket->Close( ISocket::eFORCE );
      rpSocket->Release();
      rpSocket = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   configSceUsrMain
**
**  PURPOSE:    Configure main SCE user configuration.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - The SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrMain( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configSceUsrMain(%lu)", this, uIdx );

   ISceUserConfig *pSceConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg;
   CCTK_ASSERT( pSceConfig != NULL );
   if ( pSceConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configSceUsrMain: NULL SCE user config object." );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   const CFGSYSXTL *pSysCfg = &( CFM_CFG.pSys->xtl );
   const CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl );
   const CFGCALLXTL *pCallCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call );

   /* --- Configuration which returns status --- */
   /* Configure pre-loaded route set */
   CSipHeader *pRouteSet = cfgCvtRouteLstToMx( pUsrCfg->usr.net.obProxyLst );
   mxt_result res = pSceConfig->SetPreLoadedRoute( pRouteSet );
   pRouteSet = NULL;
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrMain: "
         "Error setting pre-loaded route set (%d)",
         res );
      eStatus = eCFGSTATUS_INTERR;
   }

   /* Configure Service-Route header override */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      pRouteSet = cfgCvtSvcRouteToMx( pUsrCfg->usr.net.svcRoute );
      res = pSceConfig->SetServiceRoute( pRouteSet );
      pRouteSet = NULL;
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSceUsrMain: "
            "Error configuring Service-Route header override (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }

   /* Configure WAN address */
   if ( CFGSTATUS_IS_SUCCESS ( eStatus ) )
   {
      CSipHeader *pContactHdr = preCfgForcedContact( uIdx );
      res = pSceConfig->SetForcedContact( pContactHdr );
      pContactHdr = NULL;
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSceUsrMain: "
            "Error configuring WAN address (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Associate DNS user identifier to the SCE user configuration object */
      mxt_opaque opqUser = 
         ( pUsrCfg->usr.net.uDnsId != 0 ) ?
            reinterpret_cast< mxt_opaque >( pUsrCfg->usr.net.uDnsId ) :
            reinterpret_cast< mxt_opaque >( CFM_CFG.pUsr[uIdx]->uGuid );
      pSceConfig->SetDnsUserOpaque( opqUser );

      /* Specify if '@host' needs to be appended to Call-Id header */
      pSceConfig->SetHostInCallId( 
         !UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.sigBehave.uTx,
                                   CCTK_SIGBEHVTX_CALLIDNOHOST ) );

      /* Specify if to-tag matching needs to be disabled */
      pSceConfig->SetToTagMatch( 
         !UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.uInterop,
                                   CCTK_INTEROP_NO_TOTAGMATCH ) );

      /* Specify if symmetric UDP needs to be disabled */
      pSceConfig->SetSymmetricUdp( !pSysCfg->net.gen.port.bNoSymUdp );

      /* Specify if invalidate the registration if no P-Associated-URI
      ** was present. */
      pSceConfig->SetNoPauUnreg(
         pSysCfg->net.prot.gen.pau.bNoPauUnreg == BOS_TRUE );

#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
      /* Configure the P-Asserted-Identity URI handling */
      pSceConfig->SetPaiIgnoreInvalidUri( 
                     pSysCfg->net.prot.gen.imsCfg.bIgnoreInvalidPai == BOS_TRUE );
#endif

      /* Specify if not enforcing the validation of the P-Associated-URI
      ** per default behavior. */
      pSceConfig->SetPauNoValidate(
         pSysCfg->net.prot.gen.pau.bPauNoValid == BOS_TRUE );
         
      /* Specify if subscription is to be kept even if a refresh request fails */
      pSceConfig->SetKeepSubOnRefreshFail(
         UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.uInterop,
                                  CCTK_INTEROP_KEEPSUBREFRESHFAIL ) );

      /* Specify if registration failover needs to be applied to all responses */
      pSceConfig->SetRegFvrAllRsp(
         UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.uInterop,
                                  CCTK_INTEROP_REGFVRALLRSP ) );

      /* Specify if registration failover needs to the standard set of failover
      ** trigger responses.
      */
      pSceConfig->SetRegFvrOnStdErr(
         UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.sigBehave.uRx,
                                  CCTK_SIGBEHVRX_REGFVRONSTDERR ) );

      /* Specify if registration failover needs to be applied to 305 registration
      ** error response. */
      pSceConfig->SetRegFvrOn305(
         UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.sigBehave.uRx,
                                  CCTK_SIGBEHVRX_REGFVRRESTARTON305 ) );

      /* Specify if registration needs to be ended on 403 response, this is
      ** only applicable when CCTK_INTEROP_REGFVRALLRSP is also enabled and
      ** it would overwrite that behavior */
      pSceConfig->SetRegEndOn403(
         UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.uInterop,
                                  CCTK_INTEROP_REGENDON403 ) );
                                  
      /* Specify if active calls should be allowed to continue and not silently   
      ** terminated in case of registration failure triggered by timerF expiry. This 
      ** will be applicable to other registration failure responses as well if 
      ** CCTK_INTEROP_REGFVRALLRSP is enabled. 
      */
      pSceConfig->SetActCallNoSilentTerm(
         UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.uInterop,
                                  CCTK_INTEROP_MAINTCALLREGFAIL ) );

      /* specify if we need to include the default port in the contact header. */
      pSceConfig->IncludeDefaultPortInContactHeader(
         UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.uInterop,
                                  CCTK_INTEROP_DEFAULT_PORT_IN_CONTACT_HDR ) );

      /* Configure AOR */
      CNameAddr *pUsrAddr = preCfgUsrAddr( uIdx );
      if ( pUsrAddr != NULL )
      {
         pSceConfig->SetUserAddress( *pUsrAddr );
         BRCM_DELETE( pUsrAddr );
         pUsrAddr = NULL;
      }

      /* Configure user name */
      pSceConfig->SetLocalContactIdentity(
         reinterpret_cast< const char* >( pUsrCfg->usr.net.sipName.pcUsrName ) );

#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
      /* Configure tel-URI behaviour */
      ISceUserConfig::ETelUriBehavior eTelBehav =
         ISceUserConfig::eTELURI_DONTRESOLVE;
#if ( CCTKCFG_TEL_MODE == CCTKCFG_TEL_ALL )
      if ( !pCallCfg->basic.gen.tel.bNoEnum )
      {
         eTelBehav = ISceUserConfig::eTELURI_RESOLVE;
      }
#endif
      pSceConfig->SetTelUriBehavior( eTelBehav );
#endif

      /* Configure the Service-Route handling policy
      ** FIXME: Make service route treatment configurable
      */
      ISceUserConfig::EServiceRouteTreatment eSvcRoute =
         ISceUserConfig::eSR_APPEND_TO_PRE_LOADED_ROUTE;
      if ( pCallCfg->basic.gen.emgCallCfg.bEmergency )
      {
         /* Do not include service route info when emergency service is applied */
         eSvcRoute = ISceUserConfig::eSR_IGNORE;
      }
      pSceConfig->SetServiceRouteTreatment( eSvcRoute );

      /* Configure User Agent instance identifier */
      CString instIdStr;
      if ( pSysCfg->net.prot.gen.uaIdHdr.pcSipInstId != NULL )
      {
         instIdStr =
            reinterpret_cast< const char* >(
               pSysCfg->net.prot.gen.uaIdHdr.pcSipInstId );
      }

      if (pSysCfg->net.prot.gen.gruu.eMode != eCCTKGRUUMODE_DISABLED)
      {
         pSceConfig->SetInstanceId( instIdStr );
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSceUsrGruu
**
**  PURPOSE:    Configure SCE GRUU user configuration.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - The SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrGruu( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configSceUsrGruu(%lu)", this, uIdx );

   ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg;
   CCTK_ASSERT( pUsrConfig != NULL );
   if ( pUsrConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configSceUsrGruu: NULL SCE user config object." );
      return eCFGSTATUS_INTERR;
   }

   /* Query SCE GRUU user config object */
   ISceGruuConfig *pGruuConfig = NULL;
   mxt_result res = pUsrConfig->QueryIf( &pGruuConfig );
   if ( MX_RIS_F( res ) || pGruuConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrGruu: "
         "Error creating GRUU user config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* --- Configuration which returns status --- */

   /* --- Configuration which does not return status --- */
   /* Configure self-made GRUU */
   CSipUri *pGruu = preCfgPubGruu( uIdx );
   pGruuConfig->SetPublicGruu( pGruu );

   pGruuConfig->ReleaseIfRef();
   pGruuConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSceUsrAuth
**
**  PURPOSE:    Configure SCE user authentication configuration.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - The SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrAuth( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configSceUsrAuth(%lu)", this, uIdx );

   ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg;
   CCTK_ASSERT( pUsrConfig != NULL );
   if ( pUsrConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configSceUsrAuth: NULL SCE user config object." );
      return eCFGSTATUS_INTERR;
   }

   /* Query SCE user authentication config object */
   ISceUserAuthentication *pAuthConfig = NULL;
   mxt_result res = pUsrConfig->QueryIf( &pAuthConfig );
   if ( MX_RIS_F( res ) || pAuthConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrAuth: "
         "Error creating user authentication config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl );

   /* --- Configuration which returns status --- */

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Configure realm list.  Note that the CE is expected to dynamically update
      ** the credential for the user/password in the "default" realm.
      */
      CString realmStr, userStr, pwdStr;
      BOS_UINT32 uRlmIdx;
      const CCTKRLMLST *pRlmLst = &pUsrCfg->usr.sec.auth.client.rlmlst;

      for ( uRlmIdx = 0; uRlmIdx < pRlmLst->uNum; uRlmIdx++ )
      {
         eStatus = cfgCvtRealmToMx( pRlmLst->rlm[uRlmIdx],
                                    realmStr,
                                    userStr,
                                    pwdStr );
         if ( CFGSTATUS_IS_FAIL( eStatus ) )
         {
            CCTK_TRACEI1(
               "CfgEngine - configSceUsrAuth: Error converting realm (%d)",
               eStatus );
            break;
         }

         pAuthConfig->AddCredentials( realmStr, userStr, pwdStr, "ALL" );
      }
   }
   
   /* Configure the authentication loop threshold */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      BOS_UINT32 uLoop = 
         ( pUsrCfg->usr.sec.auth.cfg.uloopMax > CFGENGINE_DEF_AUTH_LOOP_THRESHOLD ) ?
            pUsrCfg->usr.sec.auth.cfg.uloopMax : CFGENGINE_DEF_AUTH_LOOP_THRESHOLD;
      res =  pAuthConfig->SetAuthenticationLoopThreshold( uLoop );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSceUsrAuth: "
            "Error configuring authentication loop threshold (%d).",
            res );
         eStatus = eCFGSTATUS_BADPARM;
      }
   } 

   pAuthConfig->ReleaseIfRef();
   pAuthConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSceUsrSec
**
**  PURPOSE:    Configure SCE user security configuration.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - The SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrSec( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configSceUsrSec(%lu)", this, uIdx );

   ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg;
   CCTK_ASSERT( pUsrConfig != NULL );
   if ( pUsrConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configSceUsrSec: NULL SCE user config object." );
      return eCFGSTATUS_INTERR;
   }

   /* Query SCE user authentication config object */
   ISceUserSecurityConfig *pSecConfig = NULL;
   mxt_result res = pUsrConfig->QueryIf( &pSecConfig );
   if ( MX_RIS_F( res ) || pSecConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrSec: "
         "Error creating user security config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* --- Configuration which returns status --- */

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* FIXME: Needs to be configurable at CCTK */
      pSecConfig->EnablePersistentTlsConnection( false );

      /* Configure SIPS */
      ISceUserSecurityConfig::ESigSecurityPolicy eSipsPolicy =
         ISceUserSecurityConfig::eSIGSEC_NONE;
#if CCTKCFG_TLS
      if ( CFM_CFG.pSys->in.bTlsEnabled )
      {
         CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl );

         eSipsPolicy =
            pUsrCfg->usr.net.bSecOnly ? ISceUserSecurityConfig::eSIGSEC_FORCE :
                                        ISceUserSecurityConfig::eSIGSEC_BESTEFFORT;
      }
#endif
      pSecConfig->SetSignalingSecurityPolicy( eSipsPolicy );
   }

   pSecConfig->ReleaseIfRef();
   pSecConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSceUsrId
**
**  PURPOSE:    Configure SCE user identity configuration.
**
**  PARAMETERS:
**              uIdxUsr    - User index.
**              pIdxCall   - Call index. Used only when configuring a call.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrId( BOS_UINT32 uIdxUsr,
                                     const BOS_UINT32 *pIdxCall /* = NULL */ )
{
   CCTK_TRACED3(
      "CfgEngine(%p) - configSceUsrId(%lu, %p)", this, uIdxUsr, pIdxCall );

   mxt_result res;
   ISceUserIdentityConfig *pIdConfig = NULL;
   const CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdxUsr]->cfg.xtl );
   const CFGCALLXTL *pCallCfg = NULL;

   if ( pIdxCall == NULL )
   {
      ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdxUsr]->cfg.in.pSceUsrCfg;

      CCTK_ASSERT( pUsrConfig != NULL );
      if ( pUsrConfig == NULL )
      {
         CCTK_TRACEI0( "CfgEngine - configSceUsrId: NULL SCE user config object." );
         return eCFGSTATUS_INTERR;
      }

      pCallCfg = &( CFM_CFG.pUsr[uIdxUsr]->cfg.xtl.dft.call );

      /* Query SCE user identity config object */
      res = pUsrConfig->QueryIf( &pIdConfig );
   }
   else
   {
      pCallCfg = &( CFM_CFG.pCall[*pIdxCall]->cfg.xtl );

      /* Retrieve the call resource handle */
      IUaSspCall *pHdlCall =
         reinterpret_cast< IUaSspCall * >(
            ceGetNetHdl( CFM_CFG.pCall[*pIdxCall]->uGuid ) );
      if ( pHdlCall == NULL )
      {
         CCTK_TRACEI0(
            "CfgEngine - configSceUsrId: Error retrieving call resource handle." );
         return eCFGSTATUS_INTERR;
      }

      /* Query SCE user identity config object */
      res = pHdlCall->QueryIf( &pIdConfig );
   }

   if ( MX_RIS_F( res ) || pIdConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrId: "
         "Error creating user identity config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* --- Configuration which returns status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      CNameAddr *pPrefId = preCfgPrefId( *pUsrCfg, *pCallCfg );
      res = pIdConfig->SetPreferredIdentity( pPrefId, NULL );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSceUsrId: "
            "Error configuring preferred identity (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Configure network asserted identity policy */
      ISceUserIdentityConfig::ENetworkAssertedIdMode eNaiMode =
         cfgCvtNaiModeToMx( pCallCfg->basic.adv.nai );
      pIdConfig->SetNetworkAssertedIdentityMode( eNaiMode );

      /* Configure privacy policy */
      ISceUserIdentityConfig::EPrivacyMode ePrivMode =
         cfgCvtPrivLvlToMx( pCallCfg->basic.adv.priv.eLevel,
                            pCallCfg->basic.adv.priv.bCritical,
                            pCallCfg->basic.adv.nai.cli.bSec );
      pIdConfig->SetPrivacyMode( ePrivMode );
   }

   pIdConfig->ReleaseIfRef();
   pIdConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSceUsrUaCap
**
**  PURPOSE:    Configure SCE user UA capabilities configuration.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - The SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrUaCap( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configSceUsrUaCap(%lu)", this, uIdx );

   ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg;
   CCTK_ASSERT( pUsrConfig != NULL );
   if ( pUsrConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configSceUsrUaCap: NULL SCE user config object." );
      return eCFGSTATUS_INTERR;
   }

   /* Query SCE user UA cap config object */
   ISceUaCapabilitiesConfig *pUaCapConfig = NULL;
   mxt_result res = pUsrConfig->QueryIf( &pUaCapConfig );
   if ( MX_RIS_F( res ) || pUaCapConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrUaCap: "
         "Error creating user UA cap config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* --- Configuration which returns status --- */
   /* TODO: UA Capabilities (RFC 3840) support */

   /* --- Configuration which does not return status --- */

   pUaCapConfig->ReleaseIfRef();
   pUaCapConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSceUsrReg
**
**  PURPOSE:    Configure SCE user registration configuration.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - The SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrReg( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configSceUsrReg(%lu)", this, uIdx );

   ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg;
   CCTK_ASSERT( pUsrConfig != NULL );
   if ( pUsrConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configSceUsrReg: NULL SCE user config object." );
      return eCFGSTATUS_INTERR;
   }

   /* Query SCE user registration config object */
   IUaSspRegistrationConfig *pRegConfig = NULL;
   mxt_result res = pUsrConfig->QueryIf( &pRegConfig );
   if ( MX_RIS_F( res ) || pRegConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrReg: "
         "Error creating user registration config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   const CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl );
   const CFGSYSXTL *pSysCfg = &( CFM_CFG.pSys->xtl );

   /* --- Configuration which returns status --- */
   /* Configure the registrar address.
   ** NOTE: Bogus registrar address is added even if no registrar is configured
   ** because CCTK needs to make use of UASSP registration component for the
   ** included features of user matching and caller preferences processing.
   ** IUaSspRegistration::Register() must not be called with bogus address.
   */
   CVector< CHostPort* > *pRegList = pRegConfig->GetRegistrarList();
   if ( pRegList != NULL )
   {
      /* Remove the previous configured registrars */
      BOS_UINT32 uIndex = 0;

      for (; uIndex < pRegList->GetSize(); uIndex++)
      {
         BRCM_DELETE((*pRegList)[uIndex]);
         (*pRegList)[uIndex] = NULL;
      }

      pRegList->EraseAll();
   }

   CHostPort *pRegHostPort =
      BRCM_NEW( CHostPort )(
         reinterpret_cast< const char* >( CFGENGINE_QUAD_ZEROS_STR ) );
   if ( cfgIsSipHostEnabled( pUsrCfg->reg.gen.registrar ) )
   {
      eStatus =
         cfgCvtHostPortToMx( pUsrCfg->reg.gen.registrar.hostPort, *pRegHostPort );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSceUsrReg: "
            "Error converting registration host-port (%d).",
            eStatus );
         BRCM_DELETE( pRegHostPort );
         pRegHostPort = NULL;
      }
   }

   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      res = pRegConfig->AddRegistrar( pRegHostPort );
      pRegHostPort = NULL;
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSceUsrReg: "
            "Error adding registrar host-port (%d).",
            res );
         eStatus = eCFGSTATUS_INTERR;
      }
   }

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Configure registration retry delay */
      pRegConfig->SetRegistrationRetryDelay( pUsrCfg->reg.gen.tmr.uRegRetryTmr );

      /* Configure the registration expiration time */
      BOS_UINT32 uRegExp = ( pUsrCfg->reg.gen.tmr.uRegExpTmr > 0 ) ?
                           pUsrCfg->reg.gen.tmr.uRegExpTmr :
                           CFGENGINE_DEF_REG_EXP_TIME;
      pRegConfig->SetRegistrationExpirationTime( uRegExp );

      /* Configure the registration expiration threshold */
      pRegConfig->SetRegistrationExpiringThreshold(
                                 pUsrCfg->reg.gen.tmr.uThreshold );

      pRegConfig->SetRandomReReg(
                     UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.uInterop,
                                               CCTK_INTEROP_RANDOM_REREG ) );
                                               
      pRegConfig->SetRegTimeoutSilentTerm( 
                     pUsrCfg->usr.net.brCfg.bFailover ? true : false );

      /* Configure strict Contact matching in registration response */
      pRegConfig->SetStrictContactMatch(
                     !UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.uInterop,
                                               CCTK_INTEROP_REGCONTACTHDR ) );

      /* Configure GRUU enable only if the GRUU mode is "registration" */
      pRegConfig->SetGruuEnable(pSysCfg->net.prot.gen.gruu.eMode == eCCTKGRUUMODE_REG);

      /* Configure the registration state subscription expiration time */
      BOS_UINT32 uSubExp = ( pUsrCfg->reg.gen.tmr.uSubExpTmr > 0 ) ?
                           pUsrCfg->reg.gen.tmr.uSubExpTmr :
                           CFGENGINE_DEF_REG_STATE_SUB_EXP_TIME;
      pRegConfig->SetRegistrationStateSubscriptionDuration( uSubExp );

      /* Configure the registration state subscription delay */
      pRegConfig->SetRegistrationStateSubscriptionRetryDelay(
                                             pUsrCfg->reg.gen.tmr.uSubRetryTmr );

      /* Configure the registration state subscription behavior on error */
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
      pRegConfig->SetSubscriptionBehaviorOnError(
                                 IUaSspRegistrationConfig::eREGERROR_IMS );
      pRegConfig->SetRegTimeoutSilentTerm( true );
#endif

     /* Configure registration state monitoring mode based on the current user
     ** (re)configuration */
      if ( ( CFG_SYS->xtl.net.prot.gen.pau.bPauUsrSwap ) &&
           ( pUsrCfg->usr.net.sipName.bUsrNameFromTempId ) )
      {
         /* If user swap is enabled and the current user name is derived
         ** from temporary IDs, we will defer the subscription until
         ** the user swap is completed.
         */
         pRegConfig->SetRegistrationStateMonitoringMode(
               MXD_GNS::IUaSspRegistrationConfig::eMONITORING_NONE );
      }
      else
      {
         pRegConfig->SetRegistrationStateMonitoringMode(
                              ( pUsrCfg->reg.gen.regEvt.bRegEvtSupport ?
                                    IUaSspRegistrationConfig::eMONITORING_STATE :
                                    IUaSspRegistrationConfig::eMONITORING_NONE ) );
      }

      /* Configure registration event state mapping of 'rejected' event
      ** behavior to 'deactivated' event behavior */
      pUsrConfig->SetRejCtcAsDeactCtc(
         pUsrCfg->reg.gen.regEvt.bMapRejToDeact ? true : false );
         
      /* Specify if registration retry timer will be used on 403 response.  
      ** This is only applicable when CCTK_INTEROP_REGFVRALLRSP is also enabled 
      ** and it would overwrite that behavior */
      pRegConfig->SetRegistrationRetryTimerOn403( pUsrCfg->reg.gen.tmr.uRegRetryTmrOn403 );
   }

   pRegConfig->ReleaseIfRef();
   pRegConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSceUsrCall
**
**  PURPOSE:    Configure SCE user call configuration.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - The SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrCall( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configSceUsrCall(%lu)", this, uIdx );

   ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg;
   CCTK_ASSERT( pUsrConfig != NULL );
   if ( pUsrConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configSceUsrCall: NULL SCE user config object." );
      return eCFGSTATUS_INTERR;
   }

   /* Query SCE user call config object */
   IUaSspCallConfig *pCallConfig = NULL;
   mxt_result res = pUsrConfig->QueryIf( &pCallConfig );
   if ( MX_RIS_F( res ) || pCallConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrCall: "
         "Error creating user call config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const CFGCALLXTL *pCallCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call );
   const CFGSYSXTL *pSysCfg = &( CFM_CFG.pSys->xtl );

   /* --- Configuration which returns status --- */

   /* --- Configuration which does not return status --- */

   /* PRACK/100rel support */
   IUaSspCallConfig::ERel1xxConfig ePrackCfg =
      cfgCvtPrackCfgToMx( pCallCfg->basic.adv.ePrack );
   pCallConfig->SetReliable1xxConfig( ePrackCfg );

   /* Connected ID support */
   IUaSspCallConfig::EConnectedIdConfig eConnectedIdCfg =
      cfgCvtConnectedIdCfgToMx( pCallCfg->basic.adv.eConnectedId );
   pCallConfig->SetConnectedIdConfig( eConnectedIdCfg );

   /* Session expiration time */
   pCallConfig->SetSessionExpiration(
      static_cast< uint32_t >( pCallCfg->basic.adv.sesTmr.uTmr ) );

   /* Set Min-SE time - disabled if session expiration is not enabled */
   pCallConfig->SetMinSE( 
      ( pCallCfg->basic.adv.sesTmr.uTmr ) ? ( pCallCfg->basic.adv.sesTmr.uMinSe ) : 0 );
   /* Session refresh method */
   IUaSspCallConfig::ESessionRefreshConfig sesRefreshCfg =
      cfgCvtSesRefreshCfgToMx( pCallCfg->basic.adv.sesTmr.eRefresh );
   pCallConfig->SetSessionRefreshMethod( sesRefreshCfg );

   /* Two-step termination process config */
   pCallConfig->SetTwoStepByeConfig(
      UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.sigBehave.uRx,
                               CCTK_SIGBEHVRX_TWOSTEPBYE ) );

   /* Call termination on Reinvite timeout assosciated to call hold */
   pCallConfig->SetNoTerm408ReInvMspConfig(
      UTIL_BMP_IS_ENABLED_ALL( pSysCfg->net.prot.gen.sigBehave.uRx,
                               CCTK_SIGBEHVRX_REGONHLDREINVTIMEOUT ) );

   pCallConfig->ReleaseIfRef();
   pCallConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configSceUsrMwi
**
**  PURPOSE:    Configure SCE user MWI configuration.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      - All default configurations must be explicitly set to ensure
**                parameters are re-configured.
**              - The SCE user config object MUST NOT be released.
***********************************************************************************/
CFGSTATUS CfgEngine::configSceUsrMwi( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configSceUsrMwi(%lu)", this, uIdx );

   ISceUserConfig *pUsrConfig = CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg;
   CCTK_ASSERT( pUsrConfig != NULL );
   if ( pUsrConfig == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - configSceUsrMwi: NULL SCE user config object." );
      return eCFGSTATUS_INTERR;
   }

   /* Query SCE user MWI config object */
   IUaSspMwiConfig *pMwiConfig = NULL;
   mxt_result res = pUsrConfig->QueryIf( &pMwiConfig );
   if ( MX_RIS_F( res ) || pMwiConfig == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - configSceUsrMwi: "
         "Error creating user MWI config object (%d)",
         res );
      return eCFGSTATUS_NORSC;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   const CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl );

   /* --- Configuration which returns status --- */
   /* MWI address config support */
   CSipUri *pMwiUri = cfgCvtSipUriToMx( pUsrCfg->mwi.gen.server.sipHost.hostPort,
                                        pUsrCfg->mwi.gen.server.sipHost.bSec,
                                        pUsrCfg->mwi.gen.server.pcUsrName );
   if ( pMwiUri != NULL )
   {
      res = pMwiConfig->SetMwiServerAddress( pMwiUri );
      pMwiUri = NULL;
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSceUsrMwi: "
            "Error setting MWI address (%d).",
            res );
         eStatus = eCFGSTATUS_BADPARM;
      }
   }

   /* Configure MWI expiration timer */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      BOS_UINT32 uExpTmr =
         ( pUsrCfg->mwi.gen.tmr.uExpTmr == 0 ) ?
         CFGENGINE_DEF_MWI_EXP_TIME :
         pUsrCfg->mwi.gen.tmr.uExpTmr;
      res = pMwiConfig->SetSubscriptionExpirationTime( uExpTmr );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - configSceUsrMwi: "
            "Error configuring MWI expiration timer (%d).",
            res );
         eStatus = eCFGSTATUS_BADPARM;
      }
   }

   /* --- Configuration which does not return status --- */
   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      pMwiConfig->SetSubscriptionRetryDelay( pUsrCfg->mwi.gen.tmr.uRetryTmr );
      pUsrConfig->SetSubFail500(
                        pUsrCfg->dft.evt.gen.tmr.bMap500To408 == BOS_TRUE );
   }

   pMwiConfig->ReleaseIfRef();
   pMwiConfig = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   configPersistCnx
**
**  PURPOSE:    Configure user persistent connections.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::configPersistCnx( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - configPersistCnx(%lu)", this, uIdx );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   BOS_UINT32 uUsrGuid = CFM_CFG.pUsr[uIdx]->uGuid;
   CFGUSR *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg );

   if ( cfgIsSipHostEnabled( pUsrCfg->xtl.usr.sec.nat.kaCfg.server ) )
   {
      CCTK_TRACEI1( "CfgEngine - configPersistCnx: "
         "Enabling keep-alive for user 0x%lX...",
         uUsrGuid );

      /* Close any existing persistent connection */
      eStatus = closePersistCnx( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI2( "CfgEngine - configPersistCnx: "
            "Error closing existing persistent connection (%lu, %d).",
            uIdx,
            eStatus );
      }

      /* Open the persistent connection */
      if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
      {
         eStatus = openPersistCnx( uIdx );
         if ( CFGSTATUS_IS_FAIL( eStatus ) )
         {
            CCTK_TRACEI2( "CfgEngine - configPersistCnx: "
               "Error opening existing persistent connection (%lu, %d).",
               uIdx,
               eStatus );
         }
      }

      if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - configPersistCnx: "
            "...Keep-alive for user 0x%lX is enabled",
            uUsrGuid );

         /* Establishing persistent connection. Defer function status event until
         ** persistent connection operation is completed.
         */
         pUsrCfg->in.bDeferEvtAdd = BOS_TRUE;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   openPersistCnx
**
**  PURPOSE:    Open user persistent connections.
**
**  PARAMETERS:
**              uIdx    - User index.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::openPersistCnx( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - openPersistCnx(%lu)", this, uIdx );

   const CCTKNATKACFG *pKaCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.sec.nat.kaCfg );

   /* Check if it is required to open persistent connection */
   if ( !cfgIsSipHostEnabled( pKaCfg->server ) )
   {
      CCTK_TRACEI1( "CfgEngine - openPersistCnx: "
         "Keep-alive is not enabled for user 0x%lX.",
         CFM_CFG.pUsr[uIdx]->uGuid );
      return eCFGSTATUS_INTERR;
   }

#if !CCTKCFG_TLS
   /* Check if the server requires TLS connection */
   if ( pKaCfg->server.bSec )
   {
      CCTK_TRACEI1( 
         "CfgEngine - openPersistCnx: TLS support is disabled (0x%lX).",
         CFM_CFG.pUsr[uIdx]->uGuid );
      return eCFGSTATUS_BADPARM;
   }
#endif

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI2( "CfgEngine - openPersistCnx: "
         "Error retrieving SIP stack config interface (%lu, %d).",
         uIdx,
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Retrieve the stack's persistent connection list */
   CSipPersistentConnectionList* pPersistCnxList = 
      pCoreCfg->GetPersistentConnectionList();
   CCTK_ASSERT( pPersistCnxList != NULL );
   if ( pPersistCnxList == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - openPersistCnx: "
         "Error retrieving retriving persistent connection list (%lu).",
         uIdx );
      eStatus = eCFGSTATUS_INTERR;
   }

   const CFGSENETGEN *pNetCfg = &( CFM_CFG.pSys->xtl.net.gen );
   BOS_UINT32 uDnsId = ( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.uDnsId != 0 ) ?
                       CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.uDnsId :
                       CFM_CFG.pUsr[uIdx]->uGuid;

   /* Loop through each local address interface and each transport to establish 
   ** their corresponding persistent connections.
   */
   CFG_LOCADDRINFO *pAddrInfo = NULL;
   BOS_UINT32 uAddrIdx;
   for ( uAddrIdx = 0; 
         CFGSTATUS_IS_SUCCESS( eStatus ) && uAddrIdx < CCTKCFG_MAX_HOSTADDR;
         uAddrIdx++ )
   {
      pAddrInfo = &( mLocAddr[uAddrIdx] );
      if ( pAddrInfo->addrId != 0 )
      {
         /* Local address interface configured. Establish persistent connection
         ** for each transport.
         */
#if CCTKCFG_TLS
         if ( pKaCfg->server.bSec )
         {
            /* TLS */
            if ( pAddrInfo->cnx.tls.persist[uIdx].sock.socketId != 0 )
            {
               CCTK_TRACEI2( "CfgEngine - openPersistCnx: "
                  "A TLS persistent connection still exists (%lu, %lu).",
                  uIdx,
                  uAddrIdx );

               /* Socket must not be closed if socket ID is valid */
               CCTK_ASSERT( pAddrInfo->cnx.tls.persist[uIdx].sock.eState !=
                            eCFG_SOCKETSTATE_CLOSED );

               eStatus = eCFGSTATUS_BADSTATE;
            }
            else if ( 
               UTIL_BMP_IS_ENABLED_ALL( pNetCfg->transport.uSigTrans,
                                        CCTK_TRANSPORT_TLS ) )
            {
               /* Socket must be closed if socket ID is reset */
               CCTK_ASSERT( pAddrInfo->cnx.tls.persist[uIdx].sock.eState ==
                            eCFG_SOCKETSTATE_CLOSED );

               /* Establish the TLS persistent connection */
               res = 
                  pPersistCnxList->Establish(
                     eTLS,
                     pAddrInfo->addrId,
                     pKaCfg->uSrcPort,
                     reinterpret_cast< const char* >( 
                        pKaCfg->server.hostPort.pcAddr ),
                     pKaCfg->server.hostPort.uPort,
                     this,
                     pAddrInfo->cnx.tls.persist[uIdx].sock.socketId,
                     CSipPersistentConnectionList::eDEFAULT,
                     0,
                     reinterpret_cast< mxt_opaque >(
                        CFM_CFG.pUsr[uIdx]->uGuid ),
                     reinterpret_cast< mxt_opaque >(
                        uDnsId ) );
               if ( res == resFE_DUPLICATE )
               {
                  /* There exist a persistent connection with the same local and
                  ** remote address information. Skip establishing this connection.
                  */
                  CCTK_TRACED3( "CfgEngine - openPersistCnx: "
                     "There exist another TLS connection with same address info "
                     "(%lu, %lu, %p).",
                     uIdx,
                     uAddrIdx,
                     pAddrInfo->cnx.tls.persist[uIdx].sock.socketId );

                  /* "persist[].sock.socketId" should now contain the duplicated
                  ** connection ID. It should be reset after the user opaque has
                  ** been configured.
                  */
                  CCTK_ASSERT(
                     pAddrInfo->cnx.tls.persist[uIdx].sock.socketId != NULL );

                  eStatus = handleDupPcOnEst(
                     pAddrInfo->cnx.tls, uIdx, CCTK_TRANSPORT_TLS );
                  if ( CFGSTATUS_IS_FAIL( eStatus ) )
                  {
                     CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                        "Error handling duplicated TLS persistent connection "
                        "(%lu, %lu, %d).",
                        uIdx,
                        uAddrIdx,
                        eStatus );

                     /* Reset the duplicated connection's transport info */
                     pAddrInfo->cnx.tls.persist[uIdx].sock.eState =
                        eCFG_SOCKETSTATE_CLOSED;
                     pAddrInfo->cnx.tls.persist[uIdx].sock.socketId = 0;
                  }
               }
               else if ( MX_RIS_F( res ) ||
                         pAddrInfo->cnx.tls.persist[uIdx].sock.socketId == 0 )
               {
                  CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                     "Error opening TLS persistent connection (%lu, %lu, %d).",
                     uIdx,
                     uAddrIdx,
                     res );

                  eStatus = eCFGSTATUS_BADPARM;
               }
               else
               {
                  CCTK_TRACED3( "CfgEngine - openPersistCnx: "
                     "Establishing TLS connection (%lu, %lu, %lu).",
                     uIdx,
                     uAddrIdx,
                     pAddrInfo->cnx.tls.persist[uIdx].sock.socketId );

                  pAddrInfo->cnx.tls.persist[uIdx].sock.eState = 
                     eCFG_SOCKETSTATE_OPENING;
               }

               /* Stop TLS listening port if it is equal to the source port of
               ** the persistent connection.
               ** NOTE: This is intentionally placed after Establish() is called
               ** to prevent listening ports from terminating while Establish()
               ** returns a failure, given the fact that Establish() will return
               ** successfully even if the listening port has not been terminated.
               */
               if ( CFGSTATUS_IS_SUCCESS( eStatus ) && res != resFE_DUPLICATE )
               {
                  if ( pKaCfg->uSrcPort == 
                         cfgCvtLocPortToMx( pNetCfg->port, BOS_TRUE ) &&
                       pAddrInfo->cnx.tls.listen.socketId != 0 &&
                       ( pAddrInfo->cnx.tls.listen.eState 
                           == eCFG_SOCKETSTATE_OPENING ||
                         pAddrInfo->cnx.tls.listen.eState
                           == eCFG_SOCKETSTATE_OPENED ) )
                  {
                     CCTK_TRACEI2( "CfgEngine - openPersistCnx: "
                        "Stopping TLS listening port for persistent connection "
                        "(%lu, %lu).",
                        uIdx,
                        uAddrIdx );

                     eStatus = closeListeningPort( CCTK_TRANSPORT_TLS );
                     if ( CFGSTATUS_IS_FAIL( eStatus ) )
                     {
                        CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                           "Error stopping TLS listening port (%lu, %lu, %d).",
                           uIdx,
                           uAddrIdx,
                           eStatus );
                     }
                     else
                     {
                        /* Indicate the persistent connection is now sharing the
                        ** listening socket.
                        */
                        pAddrInfo->cnx.tls.persist[uIdx].sock.eState =
                           static_cast< CFG_SOCKETSTATE>(
                              pAddrInfo->cnx.tls.persist[uIdx].sock.eState |
                              eCFG_SOCKETSTATE_SHARING );
                     }
                  }
               }
            }
         }
         else
#endif /* CCTKCFG_TLS */
         {
            /* UDP */
            if ( pAddrInfo->cnx.udp.persist[uIdx].sock.socketId != 0 )
            {
               CCTK_TRACEI2( "CfgEngine - openPersistCnx: "
                  "A UDP persistent connection still exists (%lu, %lu).",
                  uIdx,
                  uAddrIdx );

               /* Socket must not be closed if socket ID is valid */
               CCTK_ASSERT( pAddrInfo->cnx.udp.persist[uIdx].sock.eState !=
                            eCFG_SOCKETSTATE_CLOSED );

               eStatus = eCFGSTATUS_BADSTATE;
            }
            else if ( 
               UTIL_BMP_IS_ENABLED_ALL( pNetCfg->transport.uSigTrans,
                                        CCTK_TRANSPORT_UDP ) )
            {
               /* Socket must be closed if socket ID is reset */
               CCTK_ASSERT( pAddrInfo->cnx.udp.persist[uIdx].sock.eState ==
                            eCFG_SOCKETSTATE_CLOSED );

               /* Establish the UDP persistent connection */
               res = 
                  pPersistCnxList->Establish(
                     eUDP,
                     pAddrInfo->addrId,
                     pKaCfg->uSrcPort,
                     reinterpret_cast< const char* >( 
                        pKaCfg->server.hostPort.pcAddr ),
                     pKaCfg->server.hostPort.uPort,
                     this,
                     pAddrInfo->cnx.udp.persist[uIdx].sock.socketId,
                     CSipPersistentConnectionList::eDEFAULT,
                     0,
                     reinterpret_cast< mxt_opaque >(
                        CFM_CFG.pUsr[uIdx]->uGuid ),
                     reinterpret_cast< mxt_opaque >(
                        uDnsId ) );
               if ( res == resFE_DUPLICATE )
               {
                  /* There exist a persistent connection with the same local and
                  ** remote address information. Skip establishing this connection.
                  */
                  CCTK_TRACED3( "CfgEngine - openPersistCnx: "
                     "There exist another UDP connection with same address info "
                     "(%lu, %lu, %p).",
                     uIdx,
                     uAddrIdx,
                     pAddrInfo->cnx.udp.persist[uIdx].sock.socketId);

                  /* "persist[].sock.socketId" should now contain the duplicated
                  ** connection ID. It should be reset after the user opaque has
                  ** been configured.
                  */
                  CCTK_ASSERT(
                     pAddrInfo->cnx.udp.persist[uIdx].sock.socketId != 0 );

                  eStatus = handleDupPcOnEst(
                     pAddrInfo->cnx.udp, uIdx, CCTK_TRANSPORT_UDP );
                  if ( CFGSTATUS_IS_FAIL( eStatus ) )
                  {
                     CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                        "Error handling duplicated UDP persistent connection "
                        "(%lu, %lu, %d).",
                        uIdx,
                        uAddrIdx,
                        eStatus );

                     /* Reset the duplicated connection's transport info */
                     pAddrInfo->cnx.udp.persist[uIdx].sock.eState =
                        eCFG_SOCKETSTATE_CLOSED;
                     pAddrInfo->cnx.udp.persist[uIdx].sock.socketId = 0;
                  }
               }
               else if ( MX_RIS_F( res ) ||
                         pAddrInfo->cnx.udp.persist[uIdx].sock.socketId == 0 )
               {
                  CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                     "Error opening UDP persistent connection (%lu, %lu, %d).",
                     uIdx,
                     uAddrIdx,
                     res );

                  eStatus = eCFGSTATUS_BADPARM;
               }
               else
               {
                  CCTK_TRACED3( "CfgEngine - openPersistCnx: "
                     "Establishing UDP connection (%lu, %lu, %p).",
                     uIdx,
                     uAddrIdx,
                     pAddrInfo->cnx.udp.persist[uIdx].sock.socketId );

                  pAddrInfo->cnx.udp.persist[uIdx].sock.eState = 
                     eCFG_SOCKETSTATE_OPENING;
               }

               /* Stop UDP listening port if it is equal to the source port of
               ** the persistent connection.
               ** NOTE: This is intentionally placed after Establish() is called
               ** to prevent listening ports from terminating while Establish()
               ** returns a failure, given the fact that Establish() will return
               ** successfully even if the listening port has not been
               ** terminated.
               */
               if ( CFGSTATUS_IS_SUCCESS( eStatus ) && res != resFE_DUPLICATE )
               {
                  if ( pKaCfg->uSrcPort == 
                         cfgCvtLocPortToMx( pNetCfg->port, BOS_FALSE ) &&
                       pAddrInfo->cnx.udp.listen.socketId != 0 &&
                       ( pAddrInfo->cnx.udp.listen.eState 
                           == eCFG_SOCKETSTATE_OPENING ||
                         pAddrInfo->cnx.udp.listen.eState
                           == eCFG_SOCKETSTATE_OPENED ) )
                  {
                     CCTK_TRACEI2( "CfgEngine - openPersistCnx: "
                        "Stopping UDP listening port for persistent connection "
                        "(%lu, %lu).",
                        uIdx,
                        uAddrIdx );

                     eStatus = closeListeningPort( CCTK_TRANSPORT_UDP );
                     if ( CFGSTATUS_IS_FAIL( eStatus ) )
                     {
                        CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                           "Error stopping UDP listening port (%lu, %lu, %d).",
                           uIdx,
                           uAddrIdx,
                           eStatus );
                     }
                     else
                     {
                        /* Indicate the persistent connection is now sharing the
                        ** listening socket.
                        */
                        pAddrInfo->cnx.udp.persist[uIdx].sock.eState =
                           static_cast< CFG_SOCKETSTATE>(
                              pAddrInfo->cnx.udp.persist[uIdx].sock.eState |
                              eCFG_SOCKETSTATE_SHARING );
                     }
                  }
               }
            }

            /* TCP */
            if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
            {
               if ( pAddrInfo->cnx.tcp.persist[uIdx].sock.socketId != 0 )
               {
                  CCTK_TRACEI2( "CfgEngine - openPersistCnx: "
                     "A TCP persistent connection still exists (%lu, %lu).",
                     uIdx,
                     uAddrIdx );

                  /* Socket must not be closed if socket ID is valid */
                  CCTK_ASSERT( pAddrInfo->cnx.tcp.persist[uIdx].sock.eState !=
                               eCFG_SOCKETSTATE_CLOSED );

                  eStatus = eCFGSTATUS_BADSTATE;
               }
               else if ( UTIL_BMP_IS_ENABLED_ALL( pNetCfg->transport.uSigTrans, 
                                                  CCTK_TRANSPORT_TCP ) )
               {
                  /* Socket must be closed if socket ID is reset */
                  CCTK_ASSERT( pAddrInfo->cnx.tcp.persist[uIdx].sock.eState ==
                               eCFG_SOCKETSTATE_CLOSED );

                  /* Establish the TCP persistent connection */
                  res = 
                     pPersistCnxList->Establish(
                        eTCP,
                        pAddrInfo->addrId,
                        pKaCfg->uSrcPort,
                        reinterpret_cast< const char* >( 
                           pKaCfg->server.hostPort.pcAddr ),
                        pKaCfg->server.hostPort.uPort,
                        this,
                        pAddrInfo->cnx.tcp.persist[uIdx].sock.socketId,
                        CSipPersistentConnectionList::eDEFAULT,
                        0,
                        reinterpret_cast< mxt_opaque >(
                           CFM_CFG.pUsr[uIdx]->uGuid ),
                        reinterpret_cast< mxt_opaque >(
                           uDnsId ) );
                  if ( res == resFE_DUPLICATE )
                  {
                     /* There exist a persistent connection with the same local and
                     ** remote address information. Skip establishing this
                     ** connection.
                     */
                     CCTK_TRACED3( "CfgEngine - openPersistCnx: "
                        "There exist another TCP connection with same address info "
                        "(%lu, %lu, %p).",
                        uIdx,
                        uAddrIdx,
                        pAddrInfo->cnx.tcp.persist[uIdx].sock.socketId );

                     /* "persist[].sock.socketId" should now contain the duplicated
                     ** connection ID. It should be reset after the user opaque has
                     ** been configured.
                     */
                     CCTK_ASSERT(
                        pAddrInfo->cnx.tcp.persist[uIdx].sock.socketId != NULL );

                     eStatus = handleDupPcOnEst(
                        pAddrInfo->cnx.tcp, uIdx, CCTK_TRANSPORT_TCP );
                     if ( CFGSTATUS_IS_FAIL( eStatus ) )
                     {
                        CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                           "Error handling duplicated TCP persistent connection "
                           "(%lu, %lu, %d).",
                           uIdx,
                           uAddrIdx,
                           eStatus );

                        /* Reset the duplicated connection's transport info */
                        pAddrInfo->cnx.tcp.persist[uIdx].sock.eState =
                           eCFG_SOCKETSTATE_CLOSED;
                        pAddrInfo->cnx.tcp.persist[uIdx].sock.socketId = 0;
                     }
                  }
                  else if ( MX_RIS_F( res ) ||
                            pAddrInfo->cnx.tcp.persist[uIdx].sock.socketId == 0 )
                  {
                     CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                        "Error opening TCP persistent connection (%lu, %lu, %d).",
                        uIdx,
                        uAddrIdx,
                        res );

                     eStatus = eCFGSTATUS_BADPARM;
                  }
                  else
                  {
                     CCTK_TRACED3( "CfgEngine - openPersistCnx: "
                        "Establishing TCP connection (%lu, %lu, %p).",
                        uIdx,
                        uAddrIdx,
                        pAddrInfo->cnx.tcp.persist[uIdx].sock.socketId );

                     pAddrInfo->cnx.tcp.persist[uIdx].sock.eState = 
                        eCFG_SOCKETSTATE_OPENING;
                  }

                  /* Stop TCP listening port if it is equal to the source port of
                  ** the persistent connection.
                  ** NOTE: This is intentionally placed after Establish() is called
                  ** to prevent listening ports from terminating while Establish()
                  ** returns a failure, given the fact that Establish() will return
                  ** successfully even if the listening port has not been
                  ** terminated.
                  */
                  if ( CFGSTATUS_IS_SUCCESS( eStatus ) && res != resFE_DUPLICATE )
                  {
                     if ( pKaCfg->uSrcPort == 
                            cfgCvtLocPortToMx( pNetCfg->port, BOS_FALSE ) &&
                          pAddrInfo->cnx.tcp.listen.socketId != 0 &&
                          ( pAddrInfo->cnx.tcp.listen.eState 
                              == eCFG_SOCKETSTATE_OPENING ||
                            pAddrInfo->cnx.tcp.listen.eState
                              == eCFG_SOCKETSTATE_OPENED ) )
                     {
                        CCTK_TRACEI2( "CfgEngine - openPersistCnx: "
                           "Stopping TCP listening port for persistent connection "
                           "(%lu, %lu).",
                           uIdx,
                           uAddrIdx );

                        eStatus = closeListeningPort( CCTK_TRANSPORT_TCP );
                        if ( CFGSTATUS_IS_FAIL( eStatus ) )
                        {
                           CCTK_TRACEI3( "CfgEngine - openPersistCnx: "
                              "Error stopping TCP listening port (%lu, %lu, %d).",
                              uIdx,
                              uAddrIdx,
                              eStatus );
                        }
                        else
                        {
                           /* Indicate the persistent connection is now sharing the
                           ** listening socket.
                           */
                           pAddrInfo->cnx.tcp.persist[uIdx].sock.eState =
                              static_cast< CFG_SOCKETSTATE>(
                                 pAddrInfo->cnx.tcp.persist[uIdx].sock.eState |
                                 eCFG_SOCKETSTATE_SHARING );
                        }
                     }
                  }
               }
            }
         }
      }
   }

   /* End of opening persistent connections */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   /* Clean-ups on failure */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      /* NOTE: no return code check here as it is not important */
      closePersistCnx( uIdx );
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   closePersistCnx
**
**  PURPOSE:    Close all existing persistent connections for a user.
**
**  PARAMETERS:
**              uIdx       - User index.
**              uTransport - Transports to be closed.
**              bAllCnx    - True to close all persistent connections (both active
**                           and duplicated connections for other users).
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::closePersistCnx( BOS_UINT32 uIdx,
                                      BOS_UINT32 uTransport
                                          /* = CCTK_TRANSPORT_ALL */,
                                      BOS_BOOL bAllCnx /* = BOS_FALSE */ )
{
   CCTK_TRACED4( "CfgEngine(%p) - closePersistCnx(%lu, %lu, %d)",
                 this,
                 uIdx,
                 uTransport,
                 bAllCnx );

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI2( "CfgEngine - closePersistCnx: "
         "Error retrieving SIP stack config interface (%lu, %d).",
         uIdx,
         res );
      return eCFGSTATUS_INTERR;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Retrieve the stack's persistent connection list */
   CSipPersistentConnectionList* pPersistCnxList = 
      pCoreCfg->GetPersistentConnectionList();
   CCTK_ASSERT( pPersistCnxList != NULL );
   if ( pPersistCnxList == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - closePersistCnx: "
         "Error retrieving retriving persistent connection list (%lu).",
         uIdx );
      eStatus = eCFGSTATUS_INTERR;
   }

   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Loop through each local address interface and each transport to terminate 
      ** any existing persistent connections.
      */
      CFG_LOCADDRINFO *pAddrInfo = NULL;
      BOS_UINT32 uAddrIdx;
      for ( uAddrIdx = 0; uAddrIdx < CCTKCFG_MAX_HOSTADDR; uAddrIdx++ )
      {
         pAddrInfo = &( mLocAddr[uAddrIdx] );
         if ( pAddrInfo->addrId == 0 )
         {
            /* This local address interface is not active. Go onto next one. */
            continue;
         }

         /* Stop UDP persistent connection */
         if ( UTIL_BMP_IS_ENABLED( uTransport, CCTK_TRANSPORT_UDP ) )
         {
            closePersistCnx( pPersistCnxList,
                             pAddrInfo->cnx.udp,
                             uIdx,
                             CCTK_TRANSPORT_UDP,
                             bAllCnx );
         }

         /* Stop TCP persistent connection */
         if ( UTIL_BMP_IS_ENABLED( uTransport, CCTK_TRANSPORT_TCP ) )
         {
            closePersistCnx( pPersistCnxList,
                             pAddrInfo->cnx.tcp,
                             uIdx,
                             CCTK_TRANSPORT_TCP,
                             bAllCnx );
         }

#if CCTKCFG_TLS
         /* Stop TLS persistent connection */
         if ( UTIL_BMP_IS_ENABLED( uTransport, CCTK_TRANSPORT_TLS ) )
         {
            closePersistCnx( pPersistCnxList,
                             pAddrInfo->cnx.tls,
                             uIdx,
                             CCTK_TRANSPORT_TLS,
                             bAllCnx );
         }
#endif
      }
   }

   /* End of closing listening ports */
   pCoreCfg->ReleaseIfRef();
   pCoreCfg = NULL;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   closePersistCnx
**
**  PURPOSE:    Close all existing persistent connections for a user.
**
**  PARAMETERS:
**              pCnxList   - Stack's persistent connection list.
**              rPortInfo  - Transport information of persistion connection to be
**                           closed.
**              uUsrIdx    - User index of the persistent connection.
**              uTransport - The persistent connection's transport type.
**              bAllCnx    - True to close all persistent connections (both active
**                           and duplicated connections for other users).
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::closePersistCnx( CSipPersistentConnectionList *pCnxList,
                                 CFG_TRANSPORTINFO &rPortInfo,
                                 BOS_UINT32 uUsrIdx,
                                 BOS_UINT32 uTransport,
                                 BOS_BOOL bAllCnx )
{
   CCTK_TRACED4( "CfgEngine(%p) - closePersistCnx(%lu, %lu, %d)",
                 this,
                 uUsrIdx,
                 uTransport,
                 bAllCnx );

   if ( pCnxList == NULL )
   {
      CCTK_TRACEI2( "CfgEngine - closePersistCnx: NULL connection list (%lu, %lu)",
                    uUsrIdx,
                    uTransport );
      return;
   }

   /* Stop any existing timeout timer */
   if ( rPortInfo.persist[uUsrIdx].uTmrId != 0 )
   {
      CCTKSTATUS eCctkStatus = tkmTimerStop( rPortInfo.persist[uUsrIdx].uTmrId );
      if ( CCTKSTATUS_IS_FAIL( eCctkStatus ) )
      {
         CCTK_TRACEI3( "CfgEngine - closePersistCnx: "
                           "Error stopping timeout timer (%d, %lu, %lu)",
                       eCctkStatus,
                       uUsrIdx,
                       uTransport );
      }
      else
      {
         rPortInfo.persist[uUsrIdx].uTmrId = 0;
      }
   }

   if ( rPortInfo.persist[uUsrIdx].sock.socketId != 0 &&
        ( UTIL_BMP_IS_ENABLED( rPortInfo.persist[uUsrIdx].sock.eState,
                               eCFG_SOCKETSTATE_OPENED ) ||
          UTIL_BMP_IS_ENABLED( rPortInfo.persist[uUsrIdx].sock.eState,
                               eCFG_SOCKETSTATE_OPENING ) ) )
   {
      BOS_UINT32 uDummy = 0, uActUsrIdx = 0, uActTransport = 0;
      CFG_TRANSPORTINFO *pActPortInfo = NULL;

      if ( bAllCnx )
      {
         /* Stop both the active persistent connection and the other duplicated
         ** persistent connections established by other users.
         */
         if ( UTIL_BMP_IS_ENABLED( rPortInfo.persist[uUsrIdx].sock.eState,
                                   eCFG_SOCKETSTATE_DUPLCED ) )
         {
            /* Look for the active persistent connection if this is a duplicated
            ** persistent connection.
            */
            pActPortInfo =
               findPersistCnxIdx( BOS_FALSE,
                                  rPortInfo.persist[uUsrIdx].sock.socketId,
                                  uDummy,
                                  uActUsrIdx,
                                  uActTransport );
         }
         else
         {
            uActUsrIdx = uUsrIdx;
            uActTransport = uTransport;
            pActPortInfo = &rPortInfo;
         }

         CCTK_ASSERT( pActPortInfo != NULL );
      }
      else
      {
         if ( UTIL_BMP_IS_ENABLED( rPortInfo.persist[uUsrIdx].sock.eState,
                                   eCFG_SOCKETSTATE_DUPLCED ) )
         {
            /* No stack action is required for stopping a duplicated persistent
            ** connection. Only connection status reset is needed.
            */
            rPortInfo.persist[uUsrIdx].sock.eState = eCFG_SOCKETSTATE_CLOSED;
            rPortInfo.persist[uUsrIdx].sock.socketId = 0;
         }
         else
         {
            /* Stopping an active persistent connection. Do not terminate the existing
            ** persistent connection if there are duplicated connections.
            */
            BOS_UINT32 uDupAddrIdx = 0, uDupUsrIdx = 0, uDupTransport;
            CFG_TRANSPORTINFO *pDupPortInfo =
               findPersistCnxIdx( BOS_TRUE,
                                  rPortInfo.persist[uUsrIdx].sock.socketId,
                                  uDupAddrIdx,
                                  uDupUsrIdx,
                                  uDupTransport );
            if ( pDupPortInfo != NULL )
            {
               /* Set the first returned duplicated connection to "active" by copying
               ** the active connection's status.
               */
               pDupPortInfo->persist[uDupUsrIdx].sock.eState =
                  rPortInfo.persist[uUsrIdx].sock.eState;
               pDupPortInfo->persist[uDupUsrIdx].sock.socketId =
                  rPortInfo.persist[uUsrIdx].sock.socketId;

               /* Reset the original active connection's status */
               rPortInfo.persist[uUsrIdx].sock.eState = eCFG_SOCKETSTATE_CLOSED;
               rPortInfo.persist[uUsrIdx].sock.socketId = 0;
            }
            else
            {
               /* No duplicated connection is found. Terminate the active
               ** persistent connection.
               */
               uActUsrIdx = uUsrIdx;
               uActTransport = uTransport;
               pActPortInfo = &rPortInfo;
            }
         }
      }

      if ( pActPortInfo != NULL )
      {
         mxt_result res =
            pCnxList->Terminate( pActPortInfo->persist[uActUsrIdx].sock.socketId );
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI3( "CfgEngine - closePersistCnx: "
               "Error closing persistent connection (%lu, %lu, %d).",
               uActTransport,
               uActUsrIdx,
               res );
         }
         else if ( pActPortInfo->persist[uActUsrIdx].sock.eState ==
                                                eCFG_SOCKETSTATE_CLOSED )
         {
            /* The persistent connection state may have been changed by
            ** Terminate() as it may invoke EvConnectionTerminated()
            ** synchronously.
            */
            CCTK_ASSERT( pActPortInfo->persist[uActUsrIdx].sock.socketId == 0 );
         }
         else
         {
            /*  Keep the "sharing" state for re-opening listening ports later */
            pActPortInfo->persist[uActUsrIdx].sock.eState =
               static_cast< CFG_SOCKETSTATE >(
                  ( pActPortInfo->persist[uActUsrIdx].sock.eState &
                    eCFG_SOCKETSTATE_SHARING ) |
                  eCFG_SOCKETSTATE_CLOSING );

            /* Update the duplicated persistent connection states if we are closing
            ** all persistent connections related to this active connection.
            */
            if ( bAllCnx )
            {
               updateDupPcOnActEvt( eCFG_PERSISTCNXEVT_TERMINATING,
                                    *pActPortInfo,
                                    uActUsrIdx );
            }
         }
      }

      /* Notify the application when the function status events are deferred */
      handleDeferredEvtUserAdd( uUsrIdx );
      handleDeferredEvtUserRemove( uUsrIdx );
   }
}

/***********************************************************************************
**  FUNCTION:   configSipTcpListenerTos
**
**  PURPOSE:    Update the current SIP TCP listening socket to the given TOS.
**
**  PARAMETERS:
**              uTos       - new TOS for the SIP TCP listening socket.
**
**
**  RETURNS:    status.
**
**  NOTES:      Since there can be many users, but only one TCP listening 
**              socket, TCP listening socket only updates its TOS if the new 
**              TOS is higher than the current TOS.
***********************************************************************************/
CFGSTATUS CfgEngine::configSipTcpListenerTos( BOS_UINT8 uTos )
{
   /* Since M5T IAsyncSocketQualityOfServiceOptions interface only has a
   ** SetTos() API, and does not have a GetTos() API.  There is no way to get
   ** the current TOS from the current M5T API.  Keep track of the
   ** highest TOS from private member variable muTcpListenSocketHighestTos.
   */
   
   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   mxt_result res;
   
   /* only update the TOS if the new TOS is higher than 
   ** the current TOS */   
   if ( uTos > muTcpListenSocketHighestTos )
   {
      muTcpListenSocketHighestTos = uTos;
      
      MXD_GNS::CList<MXD_GNS::IAsyncSocket*> list;
      MXD_GNS::IAsyncSocketQualityOfServiceOptions *pSockQosOptions;
      res = MXD_GNS::CAsyncSocketFactory::GetSocketList(OUT &list);
      
      if ( MX_RIS_F( res ) ) 
      {
         eStatus = eCFGSTATUS_INTERR;
      }
      else
      {
         unsigned int uIndex;
         unsigned int uIndex2;
         unsigned int uListSize = list.GetSize();
         unsigned int uSocketTypeSize;
         const char* const* pSocketType;

         for ( uIndex = 0; uIndex < uListSize; uIndex++ )
         {
            uSocketTypeSize = 0;
            pSocketType = list[uIndex]->GetSocketType( &uSocketTypeSize );
            for ( uIndex2 = 0; uIndex2 < uSocketTypeSize; uIndex2++ )
            {
               /* looking for socket type = "TCP, m=server" */
               if ( strncmp ( pSocketType[uIndex2], 
                       MXD_GNS::gs_apszSOCKET_TYPE_TCP_SERVER[0], 
                       strlen ( MXD_GNS::gs_apszSOCKET_TYPE_TCP_SERVER[0] )
                       ) == 0 )
               {
                  /* set the TOS */
                  res = list[uIndex]->QueryIf ( &pSockQosOptions );
                  if ( MX_RIS_S( res ) && ( pSockQosOptions != NULL ) )
                  {
                     res = pSockQosOptions->SetTos ( uTos );
                     if ( MX_RIS_F( res ) ) 
                     {
                        CCTK_TRACEI1( "CfgEngine - configSipTcpListenerTos: "
                           "Error configuring TCP listening port (%d).",
                           res );
                        eStatus = eCFGSTATUS_INTERR;
                     }                     
                     pSockQosOptions->ReleaseIfRef();
                     pSockQosOptions = NULL;
                  }
                  else
                  {
                     eStatus = eCFGSTATUS_INTERR;
                  }
                  break;
               }
            }
         }
         for ( uIndex = 0; uIndex < uListSize; uIndex++ )
         {
            list[uIndex]->ReleaseIfRef();
         }
         list.EraseAll();
      }
   }
   return eStatus;
}
/***********************************************************************************
**  FUNCTION:   updateDupPcOnActEvt
**
**  PURPOSE:    Update the other duplicated persistent connections upon receiving a
**              active connection establishment event.
**
**  PARAMETERS: eEvt       - Active persistent connection event.
**              rPortInfo  - Transport information of the active persistent
**                           connection.
**              uUsrIdx    - User index of the active persistent connection.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void CfgEngine::updateDupPcOnActEvt( CFG_PERSISTCNXEVT eEvt,
                                     const CFG_TRANSPORTINFO &rPortInfo,
                                     BOS_UINT32 uUsrIdx )
{
   CCTK_TRACED4( "CfgEngine(%p) - updateDupPcOnActEvt(%d, %p, %lu)",
                 this,
                 eEvt,
                 &rPortInfo,
                 uUsrIdx );

   BOS_UINT32 uDupAddrIdx = 0, uDupUsrIdx = 0, uDupTransport;
   CFG_TRANSPORTINFO *pDupPortInfo = NULL;
   
   do
   {
      pDupPortInfo = findPersistCnxIdx( BOS_TRUE,
                                        rPortInfo.persist[uUsrIdx].sock.socketId,
                                        uDupAddrIdx,
                                        uDupUsrIdx,
                                        uDupTransport );
      if ( pDupPortInfo != NULL )
      {
         CCTK_ASSERT( uDupUsrIdx < CFG_MAX_USR );

         switch ( eEvt )
         {
         case eCFG_PERSISTCNXEVT_TERMINATED:
            {
               /* Reset the duplicated connection state information */
               pDupPortInfo->persist[uDupUsrIdx].sock.eState =
                  eCFG_SOCKETSTATE_CLOSED;
               pDupPortInfo->persist[uDupUsrIdx].sock.socketId = 0;
            }
            break;
         default:
            {
               /* Update the duplicated connection state from the active connection.
               ** Note that we assume that the active connection state has already
               ** been updated before this function is called.
               */
               pDupPortInfo->persist[uDupUsrIdx].sock.eState =
                  static_cast< CFG_SOCKETSTATE >(
                     rPortInfo.persist[uUsrIdx].sock.eState |
                     eCFG_SOCKETSTATE_DUPLCED );
            }
            break;
         }

         /* Update the stack's and SCE's persistent connection properties */
         updateStackPcProperty( eEvt,
                                pDupPortInfo->persist[uDupUsrIdx].sock.socketId,
                                uDupUsrIdx );

         /* Notify the application when the function status events are deferred */
         handleDeferredEvtUserAdd( uDupUsrIdx );
         handleDeferredEvtUserRemove( uDupUsrIdx );

         /* Increment the user index to search for the next duplicated connection */
         uDupUsrIdx++;
      }
   }
   while ( pDupPortInfo != NULL );
}

/***********************************************************************************
**  FUNCTION:   updateStackPcProperty
**
**  PURPOSE:    Update the stack's and SCE's persistent connection properties.
**
**  PARAMETERS: eEvt          - Persistent connection event.
**              socketId      - Stack's persistent connection socket identifier
**                              returned from Establish().
**              uUsrIdx       - User index for the persistent connection to update.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void CfgEngine::updateStackPcProperty( CFG_PERSISTCNXEVT eEvt,
                                       mxt_opaque socketId,
                                       BOS_UINT32 uUsrIdx )
{
   CCTK_TRACED4( "CfgEngine(%p) - updateStackPcProperty(%d, %p, %lu)",
                 this,
                 eEvt,
                 socketId,
                 uUsrIdx );

   switch ( eEvt )
   {
   case eCFG_PERSISTCNXEVT_ESTABLISHED:
      {
         /* Update persistent connection properties on the above events */
      }
      break;
   case eCFG_PERSISTCNXEVT_TERMINATED:
      {
         /* Do not update persistent connection properties if there are still other
         ** outstanding connections (with different transports) for the same user.
         */
         if ( isAnyPersistCnxInState( uUsrIdx, eCFG_SOCKETSTATE_OPENED ) ||
              isAnyPersistCnxInState( uUsrIdx, eCFG_SOCKETSTATE_OPENING ) ||
              isAnyPersistCnxInState( uUsrIdx, eCFG_SOCKETSTATE_CLOSING ) )
         {
            return;
         }
      }
      break;
   default:
      {
         /* Do not update persistent connection properties on other events */
         return;
      }
      break;
   }

   /* Update the SIP stack's persistent connection properties. First retrieve the
   ** SipUa configuration interface.
   */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI0( "CfgEngine - updateStackPcProperty: "
         "Error retrieving SIP stack config interface." );
   }
   else
   {
      /* Retrieve the stack's persistent connection list */
      CSipPersistentConnectionList *pPersistCnxList =
                                          pCoreCfg->GetPersistentConnectionList();
      CCTK_ASSERT( pPersistCnxList != NULL );
      if ( pPersistCnxList == NULL )
      {
         CCTK_TRACEI0( "CfgEngine - updateStackPcProperty: "
            "Error retrieving persistent connection list." );
      }
      else
      {
         if ( eEvt == eCFG_PERSISTCNXEVT_ESTABLISHED )
         {
            /* Add user opaque to the stack's established persistion
            ** connection.
            */
            pPersistCnxList->AddUserOpaque( socketId,
                                            reinterpret_cast< mxt_opaque >(
                                               CFM_CFG.pUsr[uUsrIdx]->uGuid ) );
         }

         /* Update the SCE persistent connection properties */
         ISceUserConfig *pSceUsrConfig = CFM_CFG.pUsr[uUsrIdx]->cfg.in.pSceUsrCfg;
         if ( pSceUsrConfig == NULL )
         {
            CCTK_TRACED0( "CfgEngine - updateStackPcProperty: "
               "SCE user configuration has already been destroyed." );
         }
         else
         {
            /* Update the local address for the SCE internal persistent connection
            ** service. Note that this is a workaround to address a Contact header
            ** construction issue in SCE when persistent connection is enabled
            ** (CSP243049).
            */
            ISceUserSecurityConfig *pSecConfig = NULL;
            res = pSceUsrConfig->QueryIf( &pSecConfig );
            if ( MX_RIS_F( res ) || pSecConfig == NULL )
            {
               CCTK_TRACEI0( "CfgEngine - updateStackPcProperty: "
                  "Error retrieving SCE user security configuration." );
            }
            else
            {
               const CSocketAddr *pLocalAddr = NULL;

               if ( eEvt == eCFG_PERSISTCNXEVT_ESTABLISHED )
               {
                  ESipTransport eTransport;
                  const CSocketAddr *pPeerAddr = NULL;

                  res = pPersistCnxList->GetConnection( socketId,
                                                        eTransport,
                                                        pLocalAddr,
                                                        pPeerAddr );
                  if ( MX_RIS_F( res ) || pLocalAddr == NULL )
                  {
                     CCTK_TRACEI0( "CfgEngine - updateStackPcProperty: "
                        "Error retrieving local address socket info." );
                  }
               }

               /* NOTE: SetPersistentConnectionLocalAddress() is an internal SCE
               ** function.
               */
               pSecConfig->SetPersistentConnectionLocalAddress( pLocalAddr );

               pSecConfig->ReleaseIfRef();
               pSecConfig = NULL;
            }
         }
      }
   
      pCoreCfg->ReleaseIfRef();
      pCoreCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   handleDupPcOnEst
**
**  PURPOSE:    Handle the scenario when the stack immediately (i.e. synchronously)
**              finds a duplicated persistent connection (i.e.
**              CPersistentConnectionList::Establish() returns resFE_DUPLICATE).
**
**  PARAMETERS:
**              rDupPortInfo  - Transport information of the duplicated persistent
**                              connection.
**              uDupUsrIdx    - User index of the duplicated persistent connection.
**              uDupTransport - Duplicated persistent connection's transport type.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::handleDupPcOnEst( CFG_TRANSPORTINFO &rDupPortInfo,
                                       BOS_UINT32 uDupUsrIdx,
                                       BOS_UINT32 uDupTransport )
{
   CCTK_TRACED4( "CfgEngine(%p) - handleDupPcOnEst(%p, %lu, %lu)",
                 this,
                 &rDupPortInfo,
                 uDupUsrIdx,
                 uDupTransport );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* "rDupPortInfo.persist[uDupUsrIdx].sock.socketId" should now contain the active
   ** connection ID but the socket state is still closed. Temporarily enable the
   ** "duplicated" flag to bypass this duplicated user for the active connection
   ** search below.
   */
   CCTK_ASSERT(
      rDupPortInfo.persist[uDupUsrIdx].sock.eState == eCFG_SOCKETSTATE_CLOSED );
   rDupPortInfo.persist[uDupUsrIdx].sock.eState = 
      static_cast< CFG_SOCKETSTATE >( eCFG_SOCKETSTATE_OPENED |
                                      eCFG_SOCKETSTATE_DUPLCED );

   /* Search for the active connection transport info */
   BOS_UINT32 uActiveAddrIdx = 0, uActiveUsrIdx = 0, uActiveTransport;
   CFG_TRANSPORTINFO *pActivePortInfo = 
      findPersistCnxIdx( BOS_FALSE,
                         rDupPortInfo.persist[uDupUsrIdx].sock.socketId,
                         uActiveAddrIdx,
                         uActiveUsrIdx,
                         uActiveTransport );
   
   CCTK_ASSERT( pActivePortInfo != NULL );
   CCTK_ASSERT( pActivePortInfo->persist[uActiveUsrIdx].sock.socketId != NULL );
   CCTK_ASSERT( uActiveTransport == uDupTransport );

   if ( uActiveUsrIdx == uDupUsrIdx )
   {
      /* openPersistCnx() is called immediately after closePersistCnx(), i.e.
      ** the stack would not have closed the persistent connection yet.
      ** Fail the user add operation.
      */
      CCTK_ASSERT(
         UTIL_BMP_IS_ENABLED( pActivePortInfo->persist[uActiveUsrIdx].sock.eState,
                              eCFG_SOCKETSTATE_CLOSING ) );
      eStatus = eCFGSTATUS_NOTCMPL;
   }
   else
   {
      /* Enable the "duplicated" socket state condition flag for indicating this is
      ** a duplicated persistent connection.
      */
      rDupPortInfo.persist[uDupUsrIdx].sock.eState =
         static_cast< CFG_SOCKETSTATE >(
            pActivePortInfo->persist[uActiveUsrIdx].sock.eState |
               eCFG_SOCKETSTATE_DUPLCED );

      if ( UTIL_BMP_IS_ENABLED(
               pActivePortInfo->persist[uActiveUsrIdx].sock.eState,
               eCFG_SOCKETSTATE_OPENED ) )
      {
         /* Update the stack's and SCE's persistent connection properties with
         ** the duplicated connection info.
         */
         updateStackPcProperty( eCFG_PERSISTCNXEVT_ESTABLISHED,
                                rDupPortInfo.persist[uDupUsrIdx].sock.socketId,
                                uDupUsrIdx );
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   handleDupPcOnErrEvt
**
**  PURPOSE:    Handle the scenario when the stack asynchronously finds a duplicated
**              persistent connection, particularly after DNS is received (i.e.
**              ISipPersistentConnectionMgr::EvErrorOnConnection() returns
**              resFE_DUPLICATE).
**
**  PARAMETERS:
**              rDupPortInfo  - Transport information of the duplicated persistent
**                              connection.
**              uDupUsrIdx    - User index of the duplicated persistent connection.
**              uDupTransport - Duplicated persistent connection's transport type.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS CfgEngine::handleDupPcOnErrEvt( CFG_TRANSPORTINFO &rDupPortInfo,
                                          BOS_UINT32 uDupUsrIdx,
                                          BOS_UINT32 uDupTransport )
{
   CCTK_TRACED4( "CfgEngine(%p) - handleDupPcOnErrEvt(%p, %lu, %lu)",
                 this,
                 &rDupPortInfo,
                 uDupUsrIdx,
                 uDupTransport );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Retrieve the SipUa configuration interface */
   ISipCoreConfig *pCoreCfg = NULL;
   mxt_result res = CreateEComInstance( CLSID_CSipCoreConfig, NULL, &pCoreCfg );
   if ( MX_RIS_F( res ) || ( pCoreCfg == NULL ) )
   {
      CCTK_TRACEI0( "CfgEngine - handleDupPcOnErrEvt: "
         "Error retrieving SIP stack config interface." );

      return eCFGSTATUS_INTERR;
   }

   /* Retrieve the stack's persistent connection list */
   CSipPersistentConnectionList *pPersistCnxList =
      pCoreCfg->GetPersistentConnectionList();
   CCTK_ASSERT( pPersistCnxList != NULL );
   if ( pPersistCnxList == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - handleDupPcOnErrEvt: "
         "Error retrieving retriving persistent connection list." );

      eStatus = eCFGSTATUS_INTERR;
   }
   else
   {
      /* Retrieve the active persistent connection socket ID */
      mxt_opaque activeSocketId =
         pPersistCnxList->GetDuplicatedConnection(
                                 rDupPortInfo.persist[uDupUsrIdx].sock.socketId );
      CCTK_ASSERT( activeSocketId != 0 );

      /* Search for the active connection transport info */
      BOS_UINT32 uActiveAddrIdx = 0, uActiveUsrIdx = 0, uActiveTransport;
      CFG_TRANSPORTINFO *pActivePortInfo = 
         findPersistCnxIdx( BOS_FALSE,
                            activeSocketId,
                            uActiveAddrIdx,
                            uActiveUsrIdx,
                            uActiveTransport );
      
      CCTK_ASSERT( pActivePortInfo != NULL );
      CCTK_ASSERT( pActivePortInfo->persist[uActiveUsrIdx].sock.socketId != NULL );
      CCTK_ASSERT( uActiveTransport == uDupTransport );

      /* Skip handling duplicated connection if the active connection has already
      ** been terminated.
      */
      if ( pActivePortInfo->persist[uActiveUsrIdx].sock.eState !=
           eCFG_SOCKETSTATE_CLOSED )
      {
         /* Tell the stack to stop retrying establishing the duplicated
         ** persistent connection.
         */
         res = pPersistCnxList->Terminate( 
                                 rDupPortInfo.persist[uDupUsrIdx].sock.socketId );
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI4( "CfgEngine - handleDupPcOnErrEvt: "
               "Error closing duplicated persistent connection (%lu, %lu, %p, %d).",
               uDupTransport,
               uDupUsrIdx,
               rDupPortInfo.persist[uDupUsrIdx].sock.socketId,
               res );

            eStatus = eCFGSTATUS_INTERR;

            /* NOTE: We do not enable "duplicated" condition flag as duplicated
            ** connection termination fails. The termination will be retried again
            ** when the stack reports the same error event next time.
            */
         }
         else
         {
            if ( UTIL_BMP_IS_ENABLED(
                           pActivePortInfo->persist[uActiveUsrIdx].sock.eState,
                           eCFG_SOCKETSTATE_OPENED ) )
            {
               /* Update the stack's and SCE's persistent connection properties with
               ** the duplicated connection info.
               */
               updateStackPcProperty( eCFG_PERSISTCNXEVT_ESTABLISHED,
                                      activeSocketId,
                                      uDupUsrIdx );
            }

            /* Store the active persistent connection ID in the duplicated
            ** connection transport info.
            */
            rDupPortInfo.persist[uDupUsrIdx].sock.socketId = activeSocketId;

            /* Enable the "duplicated" socket state condition flag for indicating
            ** this is a duplicated persistent connection.
            */
            rDupPortInfo.persist[uDupUsrIdx].sock.eState =
               static_cast< CFG_SOCKETSTATE >(
                  pActivePortInfo->persist[uActiveUsrIdx].sock.eState |
                     eCFG_SOCKETSTATE_DUPLCED );

            /* Notify the application when the function status events are deferred */
            handleDeferredEvtUserAdd( uDupUsrIdx );
            handleDeferredEvtUserRemove( uDupUsrIdx );
         }
      }
   }

   if ( pCoreCfg != NULL )
   {
      pCoreCfg->ReleaseIfRef();
      pCoreCfg = NULL;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   findPersistCnxIdx
**
**  PURPOSE:    Find the first corresponding local address index and user index
**              from the stack's persistent connection socket identifier.
**
**  PARAMETERS:
**              bFindDup      - [IN] True to find the first duplicated persistent
**                              connection. False to find the active persistent
**                              connection.
**              socketId      - [IN] Stack's persistent connection socket ID.
**              ruAddrIdx     - [INOUT] Starting local address index of the search.
**                              If a match is found, this field will be overwritten
**                              and contain the corresponding local address index.
**              ruUsrIdx      - [INOUT] Starting user index of the search.
**                              If a match is found, this field will be overwritten
**                              and contain the corresponding user index.
**              ruTransport   - [OUT] The corresponding transport if a match is
**                              found.
**
**
**  RETURNS:    Pointer to the transport information block.
**              NULL if socket identifier does not match any transport info block.
**
**  NOTES:      None
***********************************************************************************/
CfgEngine::CFG_TRANSPORTINFO *CfgEngine::findPersistCnxIdx(
                                                         BOS_BOOL bFindDup,
                                                         mxt_opaque socketId,
                                                         BOS_UINT32 &ruAddrIdx, 
                                                         BOS_UINT32 &ruUsrIdx,
                                                         BOS_UINT32 &ruTransport )
{
   for ( ; ruAddrIdx < CCTKCFG_MAX_HOSTADDR; ruAddrIdx++ )
   {
      if ( mLocAddr[ruAddrIdx].addrId != 0 )
      {
         for ( ; ruUsrIdx < CFG_MAX_USR; ruUsrIdx++ )
         {
            if ( ( UTIL_BMP_IS_ENABLED(
                     mLocAddr[ruAddrIdx].cnx.udp.persist[ruUsrIdx].sock.eState,
                     eCFG_SOCKETSTATE_DUPLCED ) ==
                        ( bFindDup == BOS_TRUE ) ) &&
                 mLocAddr[ruAddrIdx].cnx.udp.persist[ruUsrIdx].sock.socketId ==
                     socketId )
            {
               ruTransport = CCTK_TRANSPORT_UDP;
               return &mLocAddr[ruAddrIdx].cnx.udp;
            }
            if ( ( UTIL_BMP_IS_ENABLED(
                     mLocAddr[ruAddrIdx].cnx.tcp.persist[ruUsrIdx].sock.eState,
                     eCFG_SOCKETSTATE_DUPLCED ) ==
                        ( bFindDup == BOS_TRUE ) ) &&
                 mLocAddr[ruAddrIdx].cnx.tcp.persist[ruUsrIdx].sock.socketId ==
                     socketId )
            {
               ruTransport = CCTK_TRANSPORT_TCP;
               return &mLocAddr[ruAddrIdx].cnx.tcp;
            }
#if CCTKCFG_TLS
            if ( ( UTIL_BMP_IS_ENABLED(
                     mLocAddr[ruAddrIdx].cnx.tls.persist[ruUsrIdx].sock.eState,
                     eCFG_SOCKETSTATE_DUPLCED ) ==
                        ( bFindDup == BOS_TRUE ) ) &&
                 mLocAddr[ruAddrIdx].cnx.tls.persist[ruUsrIdx].sock.socketId ==
                     socketId )
            {
               ruTransport = CCTK_TRANSPORT_TLS;
               return &mLocAddr[ruAddrIdx].cnx.tls;
            }
#endif
         }
      }
   }

   /* No match with the input socket ID */
   return NULL;
}

/***********************************************************************************
**  FUNCTION:   isAnySharingPersistCnx
**
**  PURPOSE:    Check if there is any sharing persistent connection on a local
**              address interface.
**
**  PARAMETERS:
**              uAddrIdx   - Local address interface index.
**
**
**  RETURNS:    True if there exists one or more persistent connections that share
**              any of the listening ports on the local address interface.
**              False otherwise.
**
**  NOTES:      None
***********************************************************************************/
BOS_BOOL CfgEngine::isAnySharingPersistCnx( BOS_UINT32 uAddrIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - isAnySharingPersistCnx(%lu)", this, uAddrIdx );

   CCTK_ASSERT( mLocAddr[uAddrIdx].addrId != 0 );

   CFG_CNXINFO *pCnxInfo = &mLocAddr[uAddrIdx].cnx;

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
   {
      if ( ( pCnxInfo->udp.persist[uIdx].sock.socketId != 0 &&
             UTIL_BMP_IS_ENABLED( pCnxInfo->udp.persist[uIdx].sock.eState, 
                                  eCFG_SOCKETSTATE_SHARING ) ) ||
           ( pCnxInfo->tcp.persist[uIdx].sock.socketId != 0 &&
             UTIL_BMP_IS_ENABLED( pCnxInfo->tcp.persist[uIdx].sock.eState, 
                                  eCFG_SOCKETSTATE_SHARING ) )
#if CCTKCFG_TLS
           || ( pCnxInfo->tls.persist[uIdx].sock.socketId != 0 &&
                UTIL_BMP_IS_ENABLED( pCnxInfo->tls.persist[uIdx].sock.eState,
                                     eCFG_SOCKETSTATE_SHARING ) )
#endif
         )
      {
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   isAnyPersistCnxInState
**
**  PURPOSE:    Check if there is any persistent connection in the specified state
**              for a specified user.
**
**  PARAMETERS:
**              uUsrIdx    - User index.
**              eState     - Connection state.
**
**
**  RETURNS:    True if there exists one or more persistent connections that are in
**              the specified state for a specified user.
**              False otherwise.
**
**  NOTES:      None
***********************************************************************************/
BOS_BOOL CfgEngine::isAnyPersistCnxInState( BOS_UINT32 uUsrIdx, 
                                            CFG_SOCKETSTATE eState )
{
   CCTK_TRACED3( 
      "CfgEngine(%p) - isAnyPersistCnxInState(%lu, %d)", this, uUsrIdx, eState );

   CFG_CNXINFO *pCnxInfo = NULL;
   
   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_HOSTADDR; uIdx++ )
   {
      pCnxInfo = &mLocAddr[uIdx].cnx;

      if ( UTIL_BMP_IS_ENABLED( pCnxInfo->udp.persist[uUsrIdx].sock.eState, 
                                eState ) ||
           UTIL_BMP_IS_ENABLED( pCnxInfo->tcp.persist[uUsrIdx].sock.eState,
                                eState )
#if CCTKCFG_TLS
           || UTIL_BMP_IS_ENABLED( pCnxInfo->tls.persist[uUsrIdx].sock.eState, 
                                   eState )
#endif
         )
      {
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   isAnyPersistCnxUsed
**
**  PURPOSE:    Check if any other user has a persistent connection which uses the
**              same address information as the connection the specified user uses.
**
**  PARAMETERS:
**              uUsrIdx    - User index.
**
**
**  RETURNS:    True if there exists one or more persistent connections that are in
**              the specified state for a specified user.
**              False otherwise.
**
**  NOTES:      None
***********************************************************************************/
BOS_BOOL CfgEngine::isAnyPersistCnxUsed( BOS_UINT32 uUsrIdx )
{
   CCTK_TRACED2( 
      "CfgEngine(%p) - isAnyPersistCnxUsed(%lu)", this, uUsrIdx );

   CCTKNATKACFG *pRefKaCfg = &( CFM_CFG.pUsr[uUsrIdx]->cfg.xtl.usr.sec.nat.kaCfg ),
                *pCurrKaCfg = NULL;
   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
   {
      if ( uUsrIdx == uIdx ||
           CFM_CFG.pUsr[uIdx] == NULL ||
           CFM_CFG.pUsr[uIdx]->uGuid == guCeInvalidGuid )
      {
         continue;
      }

      pCurrKaCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.sec.nat.kaCfg );

      /* NOTE: We do not compare the server address as different FQDNs can be
      ** resolved into the same IP address.
      */
      if ( pCurrKaCfg->uSrcPort == pRefKaCfg->uSrcPort &&
           pCurrKaCfg->server.bSec == pRefKaCfg->server.bSec )
      {
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   handleDeferredEvtUserAdd
**
**  PURPOSE:    Manage any deferred cctkUserAdd() function status event.
**
**  PARAMETERS:
**              uUsrIdx    - User index.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::handleDeferredEvtUserAdd( BOS_UINT32 uUsrIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - handleDeferredEvtUserAdd(%lu)", this, uUsrIdx );
   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( CFM_CFG.pUsr[uUsrIdx] != NULL &&
        CFM_CFG.pUsr[uUsrIdx]->cfg.in.bDeferEvtAdd &&
        !isAnyPersistCnxInState( 
            uUsrIdx, 
            static_cast< CFG_SOCKETSTATE >( eCFG_SOCKETSTATE_OPENING | 
                                            eCFG_SOCKETSTATE_CLOSING ) ) )
   {
      CFM_CFG.pUsr[uUsrIdx]->cfg.in.bDeferEvtAdd = BOS_FALSE;

      if ( isAnyPersistCnxInState( uUsrIdx, eCFG_SOCKETSTATE_OPENED ) )
      {
         tkmEvent( CFM_CFG.pUsr[uUsrIdx]->uGuid,
                   guCeInvalidGuid,
                   eCCTKEVT_FN_USERADD_SUCCESS,
                   reinterpret_cast< void* >( &eStatus ) );
      }
      else
      {
         /* The connection fails to be established and no other user has a
         ** connection using the same address info.
         */
         eStatus = eCCTKSTATUS_BADCNX;
         tkmEvent( CFM_CFG.pUsr[uUsrIdx]->uGuid,
                   guCeInvalidGuid,
                   eCCTKEVT_FN_USERADD_FAIL,
                   reinterpret_cast< void* >( &eStatus ) );

         /* Clean up on failure.
         ** NOTE: Return status is not important here.
         */
         cfgRemoveUsr( CFM_CFG.pUsr[uUsrIdx]->uGuid );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   handleDeferredEvtUserRemove
**
**  PURPOSE:    Manage any deferred cctkUserRemove() function status event.
**
**  PARAMETERS:
**              uUsrIdx    - User index.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::handleDeferredEvtUserRemove( BOS_UINT32 uUsrIdx )
{
   CCTK_TRACED2(
      "CfgEngine(%p) - handleDeferredEvtUserRemove(%lu)", this, uUsrIdx );
   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( CFM_CFG.pUsr[uUsrIdx] != NULL &&
        CFM_CFG.pUsr[uUsrIdx]->cfg.in.bDeferEvtRemove &&
        !isAnyPersistCnxInState( 
            uUsrIdx, 
            static_cast< CFG_SOCKETSTATE >( eCFG_SOCKETSTATE_OPENING | 
                                            eCFG_SOCKETSTATE_CLOSING ) ) )
   {
      CFM_CFG.pUsr[uUsrIdx]->cfg.in.bDeferEvtRemove = BOS_FALSE;

      if ( isAnyPersistCnxInState( uUsrIdx, eCFG_SOCKETSTATE_OPENED ) )
      {
         eStatus = eCCTKSTATUS_INTERR;

         /* The connection fails to be terminated */
         tkmEvent( CFM_CFG.pUsr[uUsrIdx]->uGuid,
                   guCeInvalidGuid,
                   eCCTKEVT_FN_USERREMOVE_FAIL,
                   reinterpret_cast< void* >( &eStatus ) );
      }
      else
      {
         tkmEvent( CFM_CFG.pUsr[uUsrIdx]->uGuid,
                   guCeInvalidGuid,
                   eCCTKEVT_FN_USERREMOVE_SUCCESS,
                   reinterpret_cast< void* >( &eStatus ) );
      }

      /* Now it is safe to clean up the user configuration content */
      cfmClrUsrCfg( uUsrIdx );
   }
}

/***********************************************************************************
**  FUNCTION:   preCfgUaId
**
**  PURPOSE:    Prepare User-Agent and Server headers configuration.
**
**  PARAMETERS:
**              rCfg          - CCTK config.
**              rUaName       - User-Agent header value.
**              rSvrName      - Server header value.
**              rbAddStackVer - True to add stack version info to headers.
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::preCfgUaId( const CCTKUAIDHDR &rCfg,
                            CString &rUaName,
                            CString &rSvrName,
                            BOS_BOOL &rbAddStackVer )
{
   CCTK_TRACED2( "CfgEngine(%p) - preCfgUaId(%p)", this, &rCfg );

   /* Configure User-Agent header */
   switch ( rCfg.eUaHdrFmt )
   {
   case eCCTKUAHDRFMT_FULL:
      {
         rUaName.EraseAll();

         /* Append CCTK version info */
         rUaName.Format( 0,
                        "Brcm-Cctk/v%d.%d.%d",
                        CCTKVER_MAJOR,
                        CCTKVER_MINOR,
                        CCTKVER_PATCH );

         if ( rCfg.pcUaHdr && rCfg.pcUaHdr[0] != '\0' )
         {
            /* Use user configured value as User-Agent header.
            ** i.e. User-Agent: xxx Brcm-Cctk/vx.x.x M5T SIP Stack/vx.x.x.x
            */
            rUaName.Insert( 0, 1, " " );
            rUaName.Insert( 0, 1, reinterpret_cast< const char* >( rCfg.pcUaHdr ) );
         }
         else
         {
            /* Include CCTK and SIP stack versions only
            ** i.e. User-Agent: Brcm-Cctk/vx.x.x M5T SIP Stack/vx.x.x.x
            */
         }
         rbAddStackVer = BOS_TRUE;
      }
      break;
   case eCCTKUAHDRFMT_APPONLY:
      {
         if ( rCfg.pcUaHdr && rCfg.pcUaHdr[0] != '\0' )
         {
            /* Use user configured value as User-Agent header.
            ** i.e. User-Agent: xxx
            */
            rUaName = reinterpret_cast< const char* >( rCfg.pcUaHdr );
         }
         else
         {
            /* Do not include User-Agent header */
            rUaName.EraseAll();
         }
         rbAddStackVer = BOS_FALSE;
      }
      break;
   default:
      {
         /* Invalid setting. Do not include any User-Agent header */
         CCTK_TRACEI1(
            "CfgEngine - preCfgUaId: Unknown UA header format (%d).",
            rCfg.eUaHdrFmt );

         rUaName.EraseAll();
         rbAddStackVer = BOS_FALSE;
      }
      break;
   }

   /* Configure Server header */
   if ( rCfg.pcSvrHdr != NULL )
   {
      rSvrName = reinterpret_cast< const char* >( rCfg.pcSvrHdr );
   }
}

/***********************************************************************************
**  FUNCTION:   preCfgCnx
**
**  PURPOSE:    Prepare connection parameter configuration.
**
**  PARAMETERS:
**              rCfg          - CCTK config.
**              ruMinCnx      - Min number of connections to keep alive.
**              ruMaxCnx      - Max number of simultaneous connections.
**              ruMaxIdleTime - Max connection idle time.
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::preCfgCnx( const CCTKCNXCFG &rCfg,
                               BOS_UINT32 &ruMinCnx,
                               BOS_UINT32 &ruMaxCnx,
                               BOS_UINT32 &ruMaxIdleTime )
{
   CCTK_TRACED2( "CfgEngine(%p) - preCfgCnx(%p)", this, &rCfg );

   /* if set to default CCTKCFG_DEFAULT_CNX_CFG use the default values,
   ** otherwise use the values that are set through cfg
   */
   ruMinCnx = ( rCfg.uMinCnx == CCTKCFG_DEFAULT_CNX_CFG ) ? 
      CFGENGINE_MIN_CNX : rCfg.uMinCnx;
   ruMaxCnx = ( rCfg.uMaxCnx == CCTKCFG_DEFAULT_CNX_CFG ) ? 
      CFGENGINE_MAX_CNX : rCfg.uMaxCnx;
   ruMaxIdleTime = ( rCfg.uMaxIdleTime == CCTKCFG_DEFAULT_CNX_CFG ) ? 
      CFGENGINE_MAX_CNX_IDLE_TIME : rCfg.uMaxIdleTime;

}

/***********************************************************************************
**  FUNCTION:   preCfgPktLog
**
**  PURPOSE:    Prepare packet logger config. Create logger socket if config valid.
**
**  PARAMETERS:
**              rCfg          - CCTK config.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
CUdpSocket *CfgEngine::preCfgPktLog( const CCTKPKTLOG &rCfg )
{
   CCTK_TRACED2( "CfgEngine(%p) - preCfgPktLog(%p)", this, &rCfg );

   if ( bosIpAddrIsZero( &( rCfg.ipPort.addr ) ) )
   {
      /* Packet logger is disabled.
      ** NOTE: No trace here because we don't treat this as error.
      */
      return NULL;
   }

   CSocketAddr logAddr;
   CFGSTATUS eStatus = cfgCvtIpPortToMx( rCfg.ipPort, logAddr );
   if ( eStatus != eCFGSTATUS_SUCCESS )
   {
      CCTK_TRACEI1( "CfgEngine - preCfgPktLog: "
         "Error setting socket address (%d).",
         eStatus );
      return NULL;
   }

   CUdpSocket *pRes = BRCM_NEW( CUdpSocket );
   if ( pRes == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - preCfgPktLog: Error allocating socket." );
      return NULL;
   }

   mxt_result res;
#if CCTKCFG_IPV6
   if ( bosIpAddrIsV6( &( rCfg.ipPort.addr ) ) )
   {
      res = pRes->Create( CSocketAddr::eINET6 );
   }
   else
#endif
   {
      res = pRes->Create( CSocketAddr::eINET );
   }
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1(
         "CfgEngine - preCfgPktLog: Error creating socket (%d).",
         res );
   }

   if ( MX_RIS_S( res ) )
   {
      res = pRes->Bind( NULL, NULL );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1(
            "CfgEngine - preCfgPktLog: Error binding socket (%d).",
            res );
      }
   }

   if ( MX_RIS_S( res ) )
   {
      res = pRes->Connect( &logAddr );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1(
            "CfgEngine - preCfgPktLog: Error connecting socket (%d).",
            res );
      }
   }

   /* Clean up UDP socket on failure */
   CCTK_ASSERT( pRes != NULL );
   if ( MX_RIS_F( res ) && pRes != NULL )
   {
      closeUdpSocket( pRes );
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   preCfgTransTmr
**
**  PURPOSE:    Prepare transaction timer config.
**
**  PARAMETERS:
**              rCfg       - CCTK config
**              ruT1       - T1
**              ruT2       - T2
**              ruTimeout  - Time-out timer
**              ruT4       - T4
**              ruB        - Timer B
**              ruD        - Timer D
**              ruF        - Timer F
**              ruH        - Timer H
**              ruJ        - Timer J
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
void CfgEngine::preCfgTransTmr( const CCTKTRANSTMRCFG &rCfg,
                                    BOS_UINT32 &ruT1,
                                    BOS_UINT32 &ruT2,
                                    BOS_UINT32 &ruTimeout,
                                    BOS_UINT32 &ruT4,
                                    BOS_UINT32 &ruB,
                                    BOS_UINT32 &ruD,
                                    BOS_UINT32 &ruF,
                                    BOS_UINT32 &ruH,
                                    BOS_UINT32 &ruJ )
{
   CCTK_TRACED2( "CfgEngine(%p) - preCfgTransTmr(%p)", this, &rCfg );

   /* The following timers MUST be set with non-zero values.
   ** (Due to the nature of stack APIs)
   */
   ruT1 = ( rCfg.uT1 > 0 ) ? rCfg.uT1 : uDEFAULT_T1_MS;
   ruT2 = ( rCfg.uT2 > 0 ) ? rCfg.uT2 : uDEFAULT_T2_MS;
   ruTimeout = ( rCfg.uTimeout > 0 ) ? rCfg.uTimeout : ( 64 * uDEFAULT_T1_MS );
   ruT4 = ( rCfg.uT4 > 0 ) ? rCfg.uT4 : uDEFAULT_T4_MS;

   /* The following timers can be set with 0 to use default values. */
   ruB = rCfg.uB;
   ruD = rCfg.uD;
   ruF = rCfg.uF;
   ruH = rCfg.uH;
   ruJ = rCfg.uJ;
}

/***********************************************************************************
**  FUNCTION:   preCfgLocAddr
**
**  PURPOSE:    Prepare local address config.
**
**  PARAMETERS:
**              rCfg       - CCTK config
**              rLocAddr   - Local address
**              rpFqdn     - FQDN vector
**              rpAccNet   - Accessible network vector
**
**
**  RETURNS:    Return status
**
**  NOTES:      None
***********************************************************************************/
CFGSTATUS CfgEngine::preCfgLocAddr(
   const CCTKHOSTADDR &rCfg,
   CSocketAddr &rLocAddr,
   CVector< CString > *&rpFqdn,
   CVector< ISipCoreConfig::SAccessibleNetwork > *&rpAccNet )
{
   CCTK_TRACED2( "CfgEngine(%p) - preCfgLocAddr(%p)", this, &rCfg );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   mxt_result res;

   rLocAddr.Reset();
   rpFqdn = NULL;
   rpAccNet = NULL;

   /* Configure local address */
   CSocketAddr locAddr;
   CFGSTATUS eCfgStatus = cfgCvtIpAddrToMx( rCfg.ipAddr, locAddr );
   if ( eCfgStatus != eCFGSTATUS_SUCCESS )
   {
      CCTK_TRACEI1( "CfgEngine - preCfgLocAddr: "
         "Error configuring local address (%d).",
         eCfgStatus );
      return eCFGSTATUS_BADPARM;
   }

#if CCTKCFG_IPV6
   /* Configure IPv6 scope ID if used */
   if ( bosIpAddrIsV6( &rCfg.ipAddr ) && rCfg.uScopeId != 0 )
   {
      locAddr.SetScopeId( rCfg.uScopeId );
   }
#endif

   /* Configure FQDN */
   CVector< CString > *pVecStrFqdn = NULL;
   if ( rCfg.pcFqdn != NULL )
   {
      CString strFqdn( reinterpret_cast< const char*>( rCfg.pcFqdn ) );

      pVecStrFqdn = BRCM_NEW( CVector< CString > );

      res = pVecStrFqdn->Append( strFqdn );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "CfgEngine - preCfgLocAddr: "
            "Error appending FQDN list (%d).",
            res );

         BRCM_DELETE( pVecStrFqdn );
         pVecStrFqdn = NULL;

         return eCFGSTATUS_INTERR;
      }
   }

   /* Configure accessible network */
   CVector< ISipCoreConfig::SAccessibleNetwork > *pVecAccessNet = NULL;
   if ( rCfg.access.uNum > 0 )
   {
      BOS_UINT32 uAccIdx;
      const CCTKACCESSINFO *pAccessInfo = &( rCfg.access );

      pVecAccessNet =
         BRCM_NEW( CVector< MX_NS ISipCoreConfig::SAccessibleNetwork > );
      for ( uAccIdx = 0; uAccIdx < pAccessInfo->uNum; uAccIdx++ )
      {
         ISipCoreConfig::SAccessibleNetwork accessNet;

         eCfgStatus = cfgCvtIpAddrToMx( pAccessInfo->net[uAccIdx].addr,
                                        accessNet.m_address );
         if ( eCfgStatus != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI2( "CfgEngine - preCfgLocAddr: "
               "Error configuring accessible network address (%d, %lu).",
               eCfgStatus,
               uAccIdx );
            eStatus = eCFGSTATUS_BADPARM;
            break;
         }

         eCfgStatus = cfgCvtIpAddrToMx( pAccessInfo->net[uAccIdx].mask,
                                        accessNet.m_netmask );
         if ( eCfgStatus != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI2( "CfgEngine - preCfgLocAddr: "
               "Error configuring accessible network mask (%d, %lu).",
               eCfgStatus,
               uAccIdx );
            eStatus = eCFGSTATUS_BADPARM;
            break;
         }

         res = pVecAccessNet->Append( accessNet );
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI2( "CfgEngine - preCfgLocAddr: "
               "Error appending accessible network list (%d, %lu).",
               res,
               uAccIdx );
            eStatus = eCFGSTATUS_INTERR;
            break;
         }
      }
   }

   /* Clean up on failure */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      BRCM_DELETE( pVecStrFqdn );
      pVecStrFqdn = NULL;

      BRCM_DELETE( pVecAccessNet );
      pVecAccessNet = NULL;
   }
   else
   {
      rLocAddr = locAddr;
      rpFqdn = pVecStrFqdn;
      rpAccNet = pVecAccessNet;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   preCfgPrefId
**
**  PURPOSE:    Prepare preferred identity config.
**
**  PARAMETERS:
**              rCfgUsr    - User configuration.
**              rCfgCall   - Call configuration.
**
**
**  RETURNS:    Preferred identity name-addr. Can be NULL based on config settings.
**
**  NOTES:      None
***********************************************************************************/
CNameAddr *CfgEngine::preCfgPrefId( const CFGUSRXTL &rCfgUsr,
                                    const CFGCALLXTL &rCfgCall )
{
   CCTK_TRACED3(
      "CfgEngine(%p) - preCfgPrefId(%p, %p)", this, &rCfgUsr, &rCfgCall );

   CNameAddr *pRes = NULL;
   const CCTKHOSTPORT *pHostPort = NULL;
   BOS_BOOL bSec = BOS_FALSE;

   /* Pre-loaded route must be configured when either preferred identity or
   ** network-asserted identity is enabled.
   ** (Section 6 of RFC3325).
   */
   if ( cfgIsNaiEnabled( rCfgCall.basic.adv.nai ) &&
        ( rCfgUsr.usr.net.obProxyLst.eType != eCCTKROUTETYPE_PRELOADED ||
          !cfgIsSipHostEnabled( rCfgUsr.usr.net.obProxyLst.route[0].addr ) ) )
   {
      CCTK_TRACEI0( "CfgEngine - preCfgPrefId: "
         "Outbound proxy list must be configured for preferred ID or "
         "network-asserted ID support." );
      return NULL;
   }

   if ( rCfgCall.basic.adv.nai.cli.pPreferredId != NULL )
   {
      CCTKSIPNAME sipName;
      
      memset( &sipName, 0, sizeof( sipName ) );
      sipName.pcUsrName = rCfgCall.basic.adv.nai.cli.pPreferredId->pcUsrName;

      pRes = cfgCvtNameAddrToMx( rCfgCall.basic.adv.nai.cli.pPreferredId->sipHost,
                                 sipName );
   }
   else
   {
      switch ( rCfgCall.basic.adv.nai.cli.eMode )
      {
      case eCCTKNAICLTMODE_NET:
         {
            /* No preferred identity is inserted */
         }
         break;
      case eCCTKNAICLTMODE_AOR:
         {
            /* Use AOR as preferred identity */
            if ( cfgIsHostPortEnabled( rCfgUsr.reg.gen.aor ) )
            {
               pHostPort = &( rCfgUsr.reg.gen.aor );
            }
            else if ( cfgIsSipHostEnabled( rCfgUsr.reg.gen.registrar ) )
            {
               pHostPort = &( rCfgUsr.reg.gen.registrar.hostPort );
            }
            else
            {
               CCTK_TRACEI0( "CfgEngine - preCfgPrefId: "
                  "Error setting AOR as preferred identity." );
            }

            /* Use registrar security setting for AOR */
            bSec = rCfgUsr.reg.gen.registrar.bSec;
         }
         break;
      case eCCTKNAICLTMODE_CONTACT:
         {
            /* Use Contact header as preferred identity */
            IUri *pContact =
               cfgCvtAddrToMx( rCfgUsr.usr.net.contact.hdrOverride.pcAddr );
            if ( pContact != NULL )
            {
               /* Use the user specific contact address if specified */
               pRes = BRCM_NEW( CNameAddr )( *pContact );
               
               BRCM_DELETE( pContact );
               pContact = NULL;
               
               return pRes;
            }
            else
            {
               /* Generate the Contact header from local address information */
               return getContactFromLocAddr( rCfgUsr );
            }
         }
         break;
      default:
         {
            CCTK_TRACEI1( "CfgEngine - preCfgPrefId: Invalid NAI mode (%d).",
                          rCfgCall.basic.adv.nai.cli.eMode );

            /* Must assert here */
            CCTK_ASSERT( pRes != NULL );
            return pRes;
         }
         break;
      }

      /* Override security flag if user is published as secured */
      if ( rCfgUsr.usr.net.bSecOnly )
      {
         bSec = BOS_TRUE;
      }

      if ( pHostPort != NULL )
      {
         pRes = cfgCvtNameAddrToMx( *pHostPort,
                                    bSec,
                                    rCfgUsr.usr.net.sipName );
         if ( pRes == NULL )
         {
            CCTK_TRACEI0( "CfgEngine - preCfgPrefId: Error converting name-addr." );
         }
      }
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   preCfgUsrAddr
**
**  PURPOSE:    Prepare public user address config.
**
**  PARAMETERS:
**              uIdx       - User index.
**              rUsrAddr   - Output user address name-addr.
**
**
**  RETURNS:    Public user address name-addr. Can be NULL if failure occurs.
**
**  NOTES:      None.
***********************************************************************************/
CNameAddr *CfgEngine::preCfgUsrAddr( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - preCfgUsrAddr(%lu)", this, uIdx );

   CNameAddr *pRes = NULL;

   const CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl );

   const CCTKHOSTPORT *pUsrAddr = NULL;
   BOS_BOOL bSec = pUsrCfg->reg.gen.registrar.bSec;
   if ( cfgIsHostPortEnabled( pUsrCfg->reg.gen.aor ) )
   {
      pUsrAddr = &( pUsrCfg->reg.gen.aor );
   }
   else if ( cfgIsSipHostEnabled( pUsrCfg->reg.gen.registrar ) )
   {
      pUsrAddr = &( pUsrCfg->reg.gen.registrar.hostPort );
   }
   else if ( pUsrCfg->usr.net.contact.hdrOverride.pcAddr != NULL )
   {
      /* AOR and registration are disabled. Use configured Contact address. */
      IUri *pContact =
         cfgCvtAddrToMx( pUsrCfg->usr.net.contact.hdrOverride.pcAddr );
      if ( pContact != NULL )
      {
         pRes = BRCM_NEW( CNameAddr )( *pContact );
         
         BRCM_DELETE( pContact );
         pContact = NULL;
         
         return pRes;
      }
   }

   /* Override security flag if user is published as secured */
   if ( pUsrCfg->usr.net.bSecOnly )
   {
      bSec = BOS_TRUE;
   }

   if ( pUsrAddr != NULL )
   {
      pRes = cfgCvtNameAddrToMx( *pUsrAddr,
                                 bSec,
                                 pUsrCfg->usr.net.sipName );
      if ( pRes == NULL )
      {
         CCTK_TRACEI0(
            "CfgEngine - preCfgUsrAddr: Error converting user address." );
      }
   }
   else
   {
      /* No Contact address is configured */
      return getContactFromLocAddr( *pUsrCfg );
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   preCfgPubGruu
**
**  PURPOSE:    Prepare a self-made public GRUU SIP-URI.
**
**  PARAMETERS:
**              uIdx       - User index.
**
**
**  RETURNS:    Public GRUU SIP-URI. Can be NULL.
**
**  NOTES:      None.
***********************************************************************************/
CSipUri *CfgEngine::preCfgPubGruu( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - preCfgPubGruu(%lu)", this, uIdx );

   CSipUri *pRes = NULL;

   const CFGSYSXTL *pSysCfg = &( CFM_CFG.pSys->xtl );
   const CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl );

   if ( pSysCfg->net.prot.gen.gruu.eMode != eCCTKGRUUMODE_SELFMADE )
   {
      return NULL;
   }

   if ( pSysCfg->net.prot.gen.uaIdHdr.pcSipInstId == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - preCfgPubGruu: "
         "Instance ID needs to be defined for generating self-made GRUU." );
      return NULL;
   }

   const CCTKHOSTPORT *pHostPort = NULL;
   BOS_BOOL bSec = BOS_FALSE;

   /* Use AOR as the URI's domain part if AOR is configured.
   ** Otherwise, use hostname or IP address instead.
   */
   if ( cfgIsHostPortEnabled( pUsrCfg->reg.gen.aor ) )
   {
      pHostPort = &( pUsrCfg->reg.gen.aor );
      bSec = pUsrCfg->reg.gen.registrar.bSec;
   }
   else if ( pUsrCfg->usr.net.contact.hdrOverride.pcAddr != NULL )
   {
      IUri *pContact =
         cfgCvtAddrToMx( pUsrCfg->usr.net.contact.hdrOverride.pcAddr );
      if ( pContact != NULL )
      {
         if ( pContact->GetUriType() == IUri::eSIP ||
              pContact->GetUriType() == IUri::eSIPS )
         {
            pRes =
               BRCM_NEW( CSipUri )( *reinterpret_cast< CSipUri * >( pContact ) );
         }
         
         BRCM_DELETE( pContact );
         pContact = NULL;
      }
   }

   if ( pRes != NULL )
   {
      return pRes;
   }

   /* Override security flag if user is advertised as secured */
   if ( pUsrCfg->usr.net.bSecOnly )
   {
      bSec = BOS_TRUE;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   CHostPort hostPort;
   if ( pHostPort != NULL )
   {
      eStatus = cfgCvtHostPortToMx( *pHostPort, hostPort );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "CfgEngine - preCfgPubGruu: "
            "Error converting host-port (%d).",
            eStatus );
      }
   }
   else
   {
      /* The reason we create a CNameAddr object here is to obtain the host-port
      ** from the local address information.
      */
      CNameAddr *pNameAddr = getContactFromLocAddr( *pUsrCfg );
      if ( pNameAddr == NULL )
      {
         CCTK_TRACEI0( "CfgEngine - preCfgPubGruu: "
            "Error creating contact from local address." );
         eStatus = eCFGSTATUS_BADPARM;
      }

      if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
      {
         CSipUri *pUriCopy = pNameAddr->GetSipUri();
         CCTK_ASSERT( pUriCopy != NULL );
         if ( pUriCopy == NULL )
         {
            CCTK_TRACEI0( "CfgEngine - preCfgPubGruu: Error retriving SIP-URI." );
            eStatus = eCFGSTATUS_INTERR;
         }
         else
         {
            hostPort = pUriCopy->GetHostPort();
         }

         BRCM_DELETE( pNameAddr );
         pNameAddr = NULL;
      }
   }

   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      CSipUri::ESecurityFlag eSecFlag;
      eSecFlag = bSec ? CSipUri::eSECURE : CSipUri::eUNSECURE;

      pRes = BRCM_NEW( CSipUri );
      pRes->SetSecured( eSecFlag );
      pRes->GetHostPort() = hostPort;
      pRes->GetUser() =
         reinterpret_cast< const char* >( pUsrCfg->usr.net.sipName.pcUsrName );
      pRes->SetParam(
         "gr",
         reinterpret_cast< const char* >(
            pSysCfg->net.prot.gen.uaIdHdr.pcSipInstId ) );
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   preCfgForcedContact
**
**  PURPOSE:    Prepare forced Contact headers.
**
**  PARAMETERS:
**              uIdx       - User index.
**
**
**  RETURNS:    Forced Contact headers. Can be NULL.
**
**  NOTES:      None.
***********************************************************************************/
CSipHeader *CfgEngine::preCfgForcedContact( BOS_UINT32 uIdx )
{
   CCTK_TRACED2( "CfgEngine(%p) - preCfgForcedContact(%lu)", this, uIdx );

   CSipHeader *pRes = NULL;

   const CFGUSRXTL *pUsrCfg = &( CFM_CFG.pUsr[uIdx]->cfg.xtl );

   if ( pUsrCfg->usr.net.contact.hdrOverride.pcAddr == NULL )
   {
      return NULL;
   }

   /* Construct the Contact name-addr */
   IUri *pContact = cfgCvtAddrToMx( pUsrCfg->usr.net.contact.hdrOverride.pcAddr );
   if ( pContact == NULL )
   {
      return NULL;
   }

   /* Create the Contact header */
   pRes = BRCM_NEW( CSipHeader )( eHDR_CONTACT );
   if ( pRes != NULL )
   {
      CNameAddr contactAddr( *pContact );
      pRes->GetContact() = contactAddr;

      /* Append header parameters, if configured. */
      if ( pUsrCfg->usr.net.contact.hdrOverride.pcHdrParm != NULL )
      {
         const char *pcParm =
            reinterpret_cast< const char * >(
               pUsrCfg->usr.net.contact.hdrOverride.pcHdrParm );
         CGenParamList *pParmLst = BRCM_NEW( CGenParamList );

         if ( MX_RIS_S(
                  pParmLst->Parse( CGenericParam::eCS_SIP_HEADER,
                                   pcParm,
                                   CGenParamList::eNO_SEP ) ) )
         {
            pRes->SetParamList( pParmLst );
         }
         else
         {
            BRCM_DELETE( pParmLst );
         }
         pParmLst = NULL;
      }

      /* Append a SIPS contact, if necessary */
      if ( CFM_CFG.pSys->in.bTlsEnabled &&
           contactAddr.GetUri()->GetUriType() == IUri::eSIP )
      {
         CSipHeader *pHdr = BRCM_NEW( CSipHeader )( *pRes );
         if ( pHdr != NULL )
         {
            pHdr->GetContact().GetSipUri()->SetSecured( CSipUri::eSECURE );

            /* NOTE: We do not care about the return status */
            pRes->AppendNextHeader( pHdr );
            pHdr = NULL;
         }
      }
   }

   BRCM_DELETE( pContact );
   pContact = NULL;

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   getContactFromLocAddr
**
**  PURPOSE:    Generate and allocate a Contact name-addr from the local address
**              information.
**
**  PARAMETERS:
**              rCfg - User configuration.
**
**
**  RETURNS:    The pointer of the Contact name-addr.
**              NULL if local address info is invalid.
**
**  NOTES:      None
***********************************************************************************/
CNameAddr *CfgEngine::getContactFromLocAddr( const CFGUSRXTL &rCfg )
{
   CCTK_TRACED2( "CfgEngine(%p) - getContactFromLocAddr(%p)", this, &rCfg );

   CNameAddr *pRes = NULL;

   const CFGSYSXTL *pSysCfg = &( CFM_CFG.pSys->xtl );

   char cIpAddrStr[64];
   CCTKHOSTPORT hostPort;
   if ( pSysCfg->net.gen.host.addr[0].pcFqdn != NULL )
   {
      /* Use the first FQDN if it is configured */
      hostPort.pcAddr = pSysCfg->net.gen.host.addr[0].pcFqdn;
   }
   else
   {
      /* Otherwise, use the first local address. */
      BOS_STATUS eBosStatus =
         bosIpAddrGetStr( &( pSysCfg->net.gen.host.addr[0].ipAddr ),
                          cIpAddrStr,
                          sizeof( cIpAddrStr ) );
      if ( eBosStatus != BOS_STATUS_OK )
      {
         CCTK_TRACEI1( "CfgEngine - getContactFromLocAddr: "
            "Error converting address from BOS local address (%d).",
            eBosStatus );
         return NULL;
      }

      hostPort.pcAddr = reinterpret_cast< BOS_UINT8* >( cIpAddrStr );
   }
   hostPort.uPort = pSysCfg->net.gen.port.uSipPort;

   /* Convert to name-addr in M5T format */
   pRes = cfgCvtNameAddrToMx( hostPort,
                              rCfg.usr.net.bSecOnly,
                              rCfg.usr.net.sipName );
   if ( pRes == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - getContactFromLocAddr: "
         "Error converting user address from local address info." );
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   isFuncAllowed
**
**  PURPOSE:    Check if a function can be called at the current config state.
**
**  PARAMETERS:
**              eFuncId - CCTK API function ID.
**
**
**  RETURNS:    False if the function specified by eFuncId cannot be called at the
**              current configuration state.
**
**  NOTES:      None
***********************************************************************************/
BOS_BOOL CfgEngine::isFuncAllowed( TKM_MSGID eFuncId )
{
   /* Always allow synchronous or non CCTK API functions */
   if ( eFuncId < eTKM_MSGID_ASYNCFUNC_START ||
        eFuncId > eTKM_MSGID_ASYNCFUNC_END )
   {
      return BOS_TRUE;
   }

   BOS_BOOL bRes = BOS_FALSE;

   switch ( meCfgState )
   {
   case eCFG_STATE_NOTCFGED:
      {
         if ( eFuncId == eTKM_MSGID_CONFIG ||
              eFuncId == eTKM_MSGID_SETPARM )
         {
            bRes = BOS_TRUE;
         }
      }
      break;
   case eCFG_STATE_CFGING:
   case eCFG_STATE_DECFGING:
      {
         /* No CCTK API functions can be called when configuring and
         ** de-configuring.
         */
      }
      break;
   case eCFG_STATE_CFGED:
      {
         /* Check to see if there are any outstanding calls/regs/MSE */
         if ( !cfgIsAllCfgRel() || ceResAlloc() )
         {
            /* Active calls/regs/MSP */
            if ( eFuncId != eTKM_MSGID_CONFIG )
            {
               bRes = BOS_TRUE;
            }
         }
         else
         {
            /* No active calls/regs/MSPs */
            if ( eFuncId == eTKM_MSGID_CONFIG ||
                 eFuncId == eTKM_MSGID_SETPARM ||
                 eFuncId == eTKM_MSGID_USERADD ||
                 eFuncId == eTKM_MSGID_RESET )
            {
               bRes = BOS_TRUE;
            }
         }
      }
      break;
   default:
      {
         CCTK_TRACEI1( "CfgEngine - isFuncAllowed: "
            "Invalid configuration state (%d).",
            eFuncId );

         /* Assert here as bRes is initially set to false */
         CCTK_ASSERT( bRes );
      }
      break;
   }

   return bRes;
}

/***********************************************************************************
**  FUNCTION:   handlePstCnxTimeout
**
**  PURPOSE:    Handle the persistent connection retry timeout event.
**
**  PARAMETERS:
**              uTmrId     - Timer ID
**              cnxOpaque  - Persistent connection opaque
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void CfgEngine::handlePstCnxTimeout( BOS_UINT32 uTmrId, mxt_opaque cnxOpaque )
{
   CCTK_TRACED3(
      "CfgEngine(%p) - handlePstCnxTimeout(%lu, %p)", this, uTmrId, cnxOpaque );

   BOS_UINT32 uAddrIdx = 0, uUsrIdx = 0, uTransport;
   CFG_TRANSPORTINFO *pPortInfo = findPersistCnxIdx( BOS_FALSE,
                                                     cnxOpaque,
                                                     uAddrIdx,
                                                     uUsrIdx,
                                                     uTransport );
   if ( pPortInfo == NULL )
   {
      CCTK_TRACEI1( "CfgEngine - handlePstCnxTimeout: "
         "Invalid persistent connection opaque (%p)",
         cnxOpaque );

      return;
   }

   /* Close the persistent connection for this timeout only if the stack is
   ** still re-attempting to establish the connection.
   */
   if ( pPortInfo->persist[uUsrIdx].uTmrId == uTmrId &&
        UTIL_BMP_IS_ENABLED_ALL( pPortInfo->persist[uUsrIdx].sock.eState, 
                                 eCFG_SOCKETSTATE_RETRYING |
                                 eCFG_SOCKETSTATE_OPENING) )
   {
      CFGSTATUS eStatus = closePersistCnx( uUsrIdx, uTransport, BOS_TRUE );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI3( "CfgEngine - handlePstCnxTimeout: "
            "Error closing persistent connection (%lu, %lu, %d).",
            uUsrIdx,
            uTransport,
            eStatus );
      }
   }
}

MX_NAMESPACE_END(MXD_GNS)

/***********************************************************************************
**  FUNCTION:   pktLoggerCb
**
**  PURPOSE:    Callback function that the Main Framework calls when the processing
**              of the packet logging operation is ready.
**
**  PARAMETERS:
**              uId     - Message ID
**              pData   - Optional data
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void pktLoggerCb( BOS_UINT32 uId, void *pData )
{
   CCTK_TRACED2( "CfgEngine - pktLoggerCb(%lu, %p)", uId, pData );

   MX_NS CMarshaler *pParms = static_cast< MX_NS CMarshaler* >( pData );

   BOS_SINT32 sDir;
   MX_NS ISipDataLogger::EDirection eDir;
   MX_NS CSocketAddr *pLocalAddr, *pPeerAddr;
   MX_NS CBlob *pRawData;
   *pParms >> sDir
           >> pLocalAddr
           >> pPeerAddr
           >> pRawData;
   eDir = static_cast< MX_NS ISipDataLogger::EDirection >( sDir );

   CCTK_ASSERT( gpCfgEngine != NULL );

   const CCTKPKTLOG *pCfg = &( CFM_CFG.pSys->xtl.loc.debug.pktLog );
   if ( gpCfgEngine->getPktLogger() != NULL &&
        !bosIpAddrIsZero( &( pCfg->ipPort.addr ) ) )
   {
      MX_NS CString headerStr;
      BOS_BOOL bSend = BOS_TRUE;

      if ( ( uId == MX_NS CfgEngine::eCFG_PKTLOGFUNCID_RAW &&
             pCfg->eMode == eCCTKPKTLOGMODE_RAW ) ||
           ( uId == MX_NS CfgEngine::eCFG_PKTLOGFUNCID_SIP &&
             pCfg->eMode == eCCTKPKTLOGMODE_SIP ) )
      {
         /* Pre-pend additional address information to the logged packet */
         headerStr = "==>";
         headerStr += pLocalAddr->GetAddress();
         if ( pLocalAddr->IsValidPort() )
         {
            char szPort[CFGENGINE_PORT_LEN];
            snprintf( szPort, CFGENGINE_PORT_LEN, 
                      ":%d", pLocalAddr->GetPort() );
            szPort[6] = '\0';
            headerStr += szPort;
         }

         /* Spiralling service should be disabled */
         CCTK_ASSERT( eDir != MX_NS ISipDataLogger::eLOOPBACK );
         if ( eDir == MX_NS ISipDataLogger::eSENT )
         {
            headerStr += ": Sending packet to: ";
         }
         else
         {
            headerStr += ": Received packet from: ";
         }

         headerStr += pPeerAddr->GetAddress();
         if ( pPeerAddr->IsValidPort() )
         {
            char szPort[CFGENGINE_PORT_LEN];
            snprintf( szPort, CFGENGINE_PORT_LEN, 
                      ":%d", pPeerAddr->GetPort() );
            szPort[6] = '\0';
            headerStr += szPort;
         }
         headerStr += "\r\n";
      }
      else if ( ( uId == MX_NS CfgEngine::eCFG_PKTLOGFUNCID_RAW &&
                  pCfg->eMode != eCCTKPKTLOGMODE_RAWONLY ) ||
                ( uId == MX_NS CfgEngine::eCFG_PKTLOGFUNCID_SIP &&
                  pCfg->eMode != eCCTKPKTLOGMODE_SIPONLY ) )
      {
         /* Ignored */
         bSend = BOS_FALSE;
      }

      if ( bSend )
      {
         /* Note we don't care the return code here */
         pRawData->Insert( 0,
                           reinterpret_cast< const uint8_t* >( headerStr.CStr() ),
                           headerStr.GetSize() );

         /* Send the packet out */
         unsigned int uSizeSent;
         mxt_result res;
         while ( pRawData->GetSize() > 0 )
         {
            res = gpCfgEngine->getPktLogger()->Send( pRawData, &uSizeSent );
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "CfgEngine - pktLoggerCb: "
                  "Error sending logged packets (%d).",
                  res );
               break;
            }
            
            if ( uSizeSent > 0 )
            {
               pRawData->Erase( 0, uSizeSent );
            }
         }
      }
   }

   BRCM_DELETE( pLocalAddr );
   BRCM_DELETE( pPeerAddr );
   BRCM_DELETE( pRawData );
   MX_NS CPool< MX_NS CMarshaler >::Delete( pParms );
}

/***********************************************************************************
**  FUNCTION:   pktModifierCb
**
**  PURPOSE:    Callback function that the SIP stack calls upon the reception and
**              parsing of a complete SIP packet before updated by stack services.
**
**  PARAMETERS:
**              rPacket - Packet to be modified during the callback.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void pktModifierCb( MX_NS CSipPacket &rPacket )
{
   CCTK_TRACED1( "CfgEngine - pktModifierCb(%p)", &rPacket );

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rPacket );

   /* FIXME: Modify incoming packet here (e.g. extracfg) */
}

/***********************************************************************************
**  FUNCTION:   cleanCoreResCb
**
**  PURPOSE:    Callback function that the Main Framework calls for cleaning up
**              the resources that can only be accessed by the core thread.
**
**  PARAMETERS:
**              uId     - Message ID
**              pData   - Optional data
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      "geCleanupStatus" is accessed from the stack's core thread here
**              because this function is expected to be synchronous to the
**              application's task context.
***********************************************************************************/
static void cleanCoreResCb( BOS_UINT32 uId, void *pData )
{
   CCTK_TRACED2( "CfgEngine - cleanCoreResCb(%u, %p)", (unsigned int)uId, pData );

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uId );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );

   CCTK_ASSERT( uId == 0 );
   CCTK_ASSERT( pData == NULL );
   CCTK_ASSERT( gpCfgEngine != NULL );
   if ( gpCfgEngine == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - cleanCoreResCb: NULL config engine pointer" );
      geCleanupStatus = eCFGSTATUS_BADPARM;
      return;
   }

   /* Stop all other timers */
   CCTKSTATUS eCctkStatus = nsNatEndTimer();
   if ( CCTKSTATUS_IS_FAIL( eCctkStatus ) )
   {
      CCTK_TRACEI1(
         "CfgEngine - cleanCoreResCb: Error stopping NAT timers (%d)",
         eCctkStatus );
      geCleanupStatus = eCFGSTATUS_INTERR;
      return;
   }

   /* De-configure the SIP stack before shutdown */
   geCleanupStatus = gpCfgEngine->deConfig();
   if ( CFGSTATUS_IS_FAIL( geCleanupStatus ) )
   {
      CCTK_TRACEI1( "CfgEngine - cleanCoreResCb: "
         "Error de-configuring the SIP stack (%d)",
         geCleanupStatus );
      return;
   }
}

/***********************************************************************************
**  FUNCTION:   pstCnxTimeoutCb
**
**  PURPOSE:    Callback function that the Main Framework calls when the retry
**              timer started by the persistent connection has expired.
**
**  PARAMETERS:
**              uId     - Timer ID
**              pData   - Optional data
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void pstCnxTimeoutCb( BOS_UINT32 uId, void *pData )
{
   CCTK_TRACED2( "CfgEngine - pstCnxTimeoutCb(%lu, %p)", uId, pData );

   CCTK_ASSERT( gpCfgEngine != NULL );
   if ( gpCfgEngine == NULL )
   {
      CCTK_TRACEI0( "CfgEngine - pstCnxTimeoutCb: NULL config engine pointer" );
      return;
   }

   CCTK_ASSERT( pData != NULL );
   if ( pData != NULL )
   {
      gpCfgEngine->handlePstCnxTimeout( uId,
                                        reinterpret_cast< mxt_opaque >( pData ) );
   }
}
