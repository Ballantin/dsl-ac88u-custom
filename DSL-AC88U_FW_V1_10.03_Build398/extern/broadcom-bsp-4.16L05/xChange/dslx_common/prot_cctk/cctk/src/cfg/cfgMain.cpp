/**********************************************************************************
** Copyright (c) 2007-2013 Broadcom Corporation
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
**      This file implements the CCTK Configuration Framework main.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cfgMain.h>

/* ---- Public Variables -------------------------------------------------------- */
/* Global config main control block.
** NOTE: pEngine must be the first item in CFMCTRLBLK for initialization purpose.
*/
CFMCTRLBLK gCfmCtrlBlk = { NULL };

/* Used to safely free memory pointed to
** by void pointers throughout various function calls.
*/
#define DeletePtr( pVar )       ({  \
      BRCM_DELETE( *pVar );         \
      *pVar = NULL;                 \
    })

/* ---- Private Constants and Types --------------------------------------------- */
/* Variable/Class manupulation macros */
#define CFM_VARNEW( pRes, varType )                                                \
                     {                                                             \
                        pRes = BRCM_NEW( varType );                                \
                        if ( pRes != NULL )                                        \
                        {                                                          \
                           memset( pRes, 0, sizeof( varType ) );                   \
                        }                                                          \
                     }

#define CFM_VARDUP( pSrc, pDst, varType )                                          \
                     {                                                             \
                        pDst = NULL;                                               \
                        if ( pSrc != NULL )                                        \
                        {                                                          \
                           varType *pTemp = BRCM_NEW( varType );                   \
                           CCTK_ASSERT( pTemp != NULL );                           \
                           if ( pTemp != NULL )                                    \
                           {                                                       \
                              *pTemp = *( pSrc );                                  \
                              pDst = pTemp;                                        \
                           }                                                       \
                        }                                                          \
                     }
/* Validation macros */
#define CFM_CHK( expn, pcErrStr )                                                  \
                     {                                                             \
                        if ( expn )                                                \
                        {                                                          \
                           CCTK_TRACEI0( pcErrStr );                               \
                           return BOS_FALSE;                                       \
                        }                                                          \
                     }

/* ---- Private Variables ------------------------------------------------------- */
#if CCTKCFG_DEBUG

/* External config parameter type debug strings.
** NOTE: Must be in the same order as CCTKPARM (external config portion only).
*/
static const BOS_UINT8 *gpcCfgParmStr[] =
{
   /* --------------------- MAIN CONFIG --------------------- */
   /* -------------- SYS CONFIG -------------- */
   CCTK_STR( "eCCTKPARM_MC_HOSTINFO" ),
   /* -------------- USER CONFIG -------------- */
   CCTK_STR( "eCCTKPARM_MC_SIPNAME" ),
   CCTK_STR( "eCCTKPARM_MC_OBPROXYLST" ),
   CCTK_STR( "eCCTKPARM_MC_MWISVRADDR" ),

   /* -------------- CALL CONFIG -------------- */
   CCTK_STR( "eCCTKPARM_MC_MEDIACFG" ),
   CCTK_STR( "eCCTKPARM_MC_MEDIALIST" ),

   /* -------------- EVENT CONFIG -------------- */

   /* --------------------- EXTENDED CONFIG --------------------- */
   /* -------------- USER CONFIG -------------- */
   CCTK_STR( "eCCTKPARM_EC_SUBEVT" ),
   CCTK_STR( "eCCTKPARM_EC_GENHDRCFG" ),
   CCTK_STR( "eCCTKPARM_EC_USRPORTCFG" ),
   CCTK_STR( "eCCTKPARM_EC_DIAGEVTCFG" ),
   CCTK_STR( "eCCTKPARM_EC_UAPEVTCFG" ),
   CCTK_STR( "eCCTKPARM_EC_USREMGCFG" ),
   CCTK_STR( "eCCTKPARM_EC_USRBRCFG" ),
   CCTK_STR( "eCCTKPARM_EC_SVCROUTE" ),
   CCTK_STR( "eCCTKPARM_EC_USRCONTACT" ),
   CCTK_STR( "eCCTKPARM_EC_AUTHCFG" ),
   CCTK_STR( "eCCTKPARM_EC_WHTLST" ),
   CCTK_STR( "eCCTKPARM_EC_BLKLST" ),
   CCTK_STR( "eCCTKPARM_EC_REGTMR" ),
   CCTK_STR( "eCCTKPARM_EC_REGEVTCFG" ),
   CCTK_STR( "eCCTKPARM_EC_MWICFG" ),
   CCTK_STR( "eCCTKPARM_EC_MWITMR" ),
#if CCTKCFG_REMOTE_USER_CONTROL
   CCTK_STR( "eCCTKPARM_EC_REMUSRCFG" ),
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

   /* -------------- CALL CONFIG -------------- */
   CCTK_STR( "eCCTKPARM_EC_EMGCALLCFG" ),
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
   CCTK_STR( "eCCTKPARM_EC_TELCALL" ),
#endif
   CCTK_STR( "eCCTKPARM_EC_CALLSTATS" ),
   CCTK_STR( "eCCTKPARM_EC_PRACK" ),
   CCTK_STR( "eCCTKPARM_EC_SESTMR" ),
   CCTK_STR( "eCCTKPARM_EC_CALLPRIV" ),
   CCTK_STR( "eCCTKPARM_EC_CALLNAI" ),
   CCTK_STR( "eCCTKPARM_EC_CONNECTEDID" ),
   CCTK_STR( "eCCTKPARM_EC_CALLHOLD" ),
   CCTK_STR( "eCCTKPARM_EC_CALLXFER" ),
   CCTK_STR( "eCCTKPARM_EC_MEDNEGCFG" ),

   /* -------------- EVENT CONFIG -------------- */
   CCTK_STR( "eCCTKPARM_EC_EVTTMR" ),
   CCTK_STR( "eCCTKPARM_EC_EVTPRIV" ),
   CCTK_STR( "eCCTKPARM_EC_EVTNAI" ),

   /* -------------- SYSTEM CONFIG -------------- */
   CCTK_STR( "eCCTKPARM_EC_LOCPORT" ),
   CCTK_STR( "eCCTKPARM_EC_TRANSPORT" ),
#if CCTKCFG_TLS
   CCTK_STR( "eCCTKPARM_EC_TLSCFG" ),
#endif
   CCTK_STR( "eCCTKPARM_EC_CNXACTIVITY" ),
   CCTK_STR( "eCCTKPARM_EC_UAIDHDR" ),
   CCTK_STR( "eCCTKPARM_EC_ROUTINGHDR" ),
   CCTK_STR( "eCCTKPARM_EC_DNSMODE" ),
   CCTK_STR( "eCCTKPARM_EC_SIGBEHAVE" ),
   CCTK_STR( "eCCTKPARM_EC_INTEROP" ),
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
   CCTK_STR( "eCCTKPARM_EC_IMSCFG" ),
#endif
   CCTK_STR( "eCCTKPARM_EC_GRUUCFG" ),
   CCTK_STR( "eCCTKPARM_EC_PAUCFG" ),
   CCTK_STR( "eCCTKPARM_EC_PANICFG" ),
#if CCTKCFG_ICE_LITE_SUPPORT
   CCTK_STR( "eCCTKPARM_EC_ICECTLCFG" ),
   CCTK_STR( "eCCTKPARM_EC_ICEIPLSTCFG" ),
#endif  /* CCTKCFG_ICE_LITE_SUPPORT */
   CCTK_STR( "eCCTKPARM_EC_TRANSTMR" ),
   CCTK_STR( "eCCTKPARM_EC_PKTLOG" ),
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   CCTK_STR( "eCCTKPARM_EC_TRACE" ),
#endif
   CCTK_STR( "eCCTKPARM_EC_NATGENCFG" ),
   CCTK_STR( "eCCTKPARM_EC_NATRGCTL" ),
#if CCTKCFG_MEMORY_MONITORING
   CCTK_STR( "eCCTKPARM_EC_MEMPROTECT" ),
#endif
#if CCTKCFG_REMOTE_USER_CONTROL
   CCTK_STR( "eCCTKPARM_EC_REMUSRSYS" ),
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

};

#endif

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */
/* Configuration (de)allocation functions */
static void          clrSysCfg( CFGSYS *pCfg );
static void          clrSysXtlCfg( CFGSYSXTL *pCfg, BOS_BOOL bMainOnly );
static void          clrSysIntCfg( CFGSYSINT *pCfg );
static void          clrUsrCfg( CFMUSR *pCfg );
static void          clrUsrXtlCfg( CFGUSRXTL *pCfg, BOS_BOOL bMainOnly );
static void          clrUsrIntCfg( CFGUSRINT *pCfg );
static void          clrCallCfg( CFMCALL *pCfg );
static void          clrCallXtlCfg( CFGCALLXTL *pCfg,
                                    BOS_BOOL bDftCfg,
                                    BOS_BOOL bMainOnly );
static void          clrCallIntCfg( CFGCALLINT *pCfg );
static void          clrEvtCfg( CFMEVT *pCfg );
static void          clrEvtXtlCfg( CFGEVTXTL *pCfg,
                                   BOS_BOOL bDftCfg,
                                   BOS_BOOL bMainOnly );
static void          clrEvtIntCfg( CFGEVTINT *pCfg );
static CFGSTATUS     cpyUsrXtlCfg( const CFGUSRXTL *pSrc, CFGUSRXTL *pDst );
static CFGSTATUS cpyCallXtlCfg( const CFGCALLXTL *pSrc, CFGCALLXTL *pDst,
                                BOS_BOOL bDftCfg );
static CFGSTATUS cpyEvtXtlCfg( const CFGEVTXTL *pSrc, CFGEVTXTL *pDst,
                                BOS_BOOL bDftCfg );
static void          delSysCfg( CFGSYS *&rpCfg );
static void          delUsrCfg( CFMUSR *&rpCfg );
static void          delCallCfg( CFMCALL *&rpCfg );
static void          delEvtCfg( CFMEVT *&rpCfg );

/* Other (de)allocation functions */
static void          clrAuthcInfo( CCTKAUTHCINFO *pInfo );
static CFGSTATUS     cpyAuthcInfo( const CCTKAUTHCINFO *pSrc, CCTKAUTHCINFO *pDst );
static void          clrContact( CCTKCONTACT *pCfg );
static CFGSTATUS     cpyContact( const CCTKCONTACT *pSrc, CCTKCONTACT *pDst );
static void          delContact( CCTKCONTACT *&rpCfg );
static CCTKCONTACT   *dupContact( const CCTKCONTACT *pSrc );
static void          clrDiagEvtCfg( CCTKDIAGEVTCFG *pCfg );
static CFGSTATUS     cpyDiagEvtCfg( const CCTKDIAGEVTCFG *pSrc,
                                    CCTKDIAGEVTCFG *pDst );
static void          delDiagEvtCfg( CCTKDIAGEVTCFG *&rpCfg );
static CCTKDIAGEVTCFG
                     *dupDiagEvtCfg( const CCTKDIAGEVTCFG *pSrc );
static void          clrEmgAddr( CCTKEMGADDR *pAddr );
static CFGSTATUS     cpyEmgAddr( const CCTKEMGADDR *pSrc, CCTKEMGADDR *pDst );
static void          clrGenHdr( CCTKGENHDRCFG *pCfg );
static CFGSTATUS     cpyGenHdr( const CCTKGENHDRCFG *pSrc, CCTKGENHDRCFG *pDst );
static void          delGenHdr( CCTKGENHDRCFG *&rpCfg );
static CCTKGENHDRCFG *dupGenHdr( const CCTKGENHDRCFG *pSrc );
static void          clrHdr( CCTKHDR *pHdr );
static CFGSTATUS     cpyHdr( const CCTKHDR *pSrc, CCTKHDR *pDst );
static void          clrHostPort( CCTKHOSTPORT *pCfg );
static CFGSTATUS     cpyHostPort( const CCTKHOSTPORT *pSrc, CCTKHOSTPORT *pDst );
static void          clrMediaCfg( CCTKMEDCFG *pCfg );
static CFGSTATUS     cpyMediaCfg( const CCTKMEDCFG *pSrc, CCTKMEDCFG *pDst );
static void          delMediaCfg( CCTKMEDCFG *&rpCfg );
static CCTKMEDCFG    *dupMediaCfg( const CCTKMEDCFG *pSrc );
static void          clrNaiCfg( CCTKNAICFG *pCfg );
static CFGSTATUS     cpyNaiCfg( const CCTKNAICFG *pSrc, CCTKNAICFG *pDst );
static void          delNaiCfg( CCTKNAICFG *&rpCfg );
static CCTKNAICFG    *dupNaiCfg( const CCTKNAICFG *pSrc );
static void          clrRoute( CCTKROUTE route[] );
static CFGSTATUS     cpyRoute( const CCTKROUTE src[], CCTKROUTE dst[] );
static void          clrRouteLst( CCTKROUTELST *pLst );
static CFGSTATUS     cpyRouteLst( const CCTKROUTELST *pSrc, CCTKROUTELST *pDst );
static void          delRouteLst( CCTKROUTELST *&rpLst );
static CCTKROUTELST  *dupRouteLst( const CCTKROUTELST *pSrc );
static void          clrSipHost( CCTKSIPHOST *pSipHost );
static CFGSTATUS     cpySipHost( const CCTKSIPHOST *pSrc, CCTKSIPHOST *pDst );
static void          clrSipName( CCTKSIPNAME *pName );
static CFGSTATUS     cpySipName( const CCTKSIPNAME *pSrc, CCTKSIPNAME *pDst );
static void          delSipName( CCTKSIPNAME *&rpName );
static CCTKSIPNAME   *dupSipName( const CCTKSIPNAME *pSrc );
static void          clrSipUsrHost( CCTKSIPUSRHOST *pUsrHost );
static CFGSTATUS     cpySipUsrHost( const CCTKSIPUSRHOST *pSrc,
                                    CCTKSIPUSRHOST *pDst );
static void          delSipUsrHost( CCTKSIPUSRHOST *&rpUsrHost );
static CCTKSIPUSRHOST
                     *dupSipUsrHost( const CCTKSIPUSRHOST *pSrc );
static void          clrSvcRoute( CCTKSVCROUTE *pRoute );
static CFGSTATUS     cpySvcRoute( const CCTKSVCROUTE *pSrc, CCTKSVCROUTE *pDst );
static void          delSvcRoute( CCTKSVCROUTE *&rpRoute );
static CCTKSVCROUTE  *dupSvcRoute( const CCTKSVCROUTE *pSrc );
#if CCTKCFG_TLS
static void          clrTlsCfg( CCTKTLSCFG *pTlsCfg );
static CFGSTATUS     cpyTlsCfg( const CCTKTLSCFG *pSrc, CCTKTLSCFG *pDst );
#endif
static void          clrUaIdHdr( CCTKUAIDHDR *pHdr );
static CFGSTATUS     cpyUaIdHdr( const CCTKUAIDHDR *pSrc, CCTKUAIDHDR *pDst );
static void          clrUapEvtCfg( CCTKUAPEVTCFG *pCfg );
static CFGSTATUS     cpyUapEvtCfg( const CCTKUAPEVTCFG *pSrc, CCTKUAPEVTCFG *pDst );
static void          delUapEvtCfg( CCTKUAPEVTCFG *&rpCfg );
static CCTKUAPEVTCFG
                     *dupUapEvtCfg( const CCTKUAPEVTCFG *pSrc );
#if CCTKCFG_VIDEO
static void          delVideoCfg( CCTKMEDVID *&rpCfg );
static CCTKMEDVID    *dupVideoCfg( const CCTKMEDVID *pSrc );
#endif
#if CCTKCFG_REMOTE_USER_CONTROL
static void          clrRemUsrSysCfg( CCTKREMUSRSYS *pCfg );
static CFGSTATUS     cpyRemUsrSysCfg( const CCTKREMUSRSYS *pSrc, CCTKREMUSRSYS *pDst );
static void          clrRemUsrXtlCfg( CFGUEREMUSR *pCfg, BOS_BOOL bMainOnly );
static CFGSTATUS     cpyRemUsrXtlCfg( const CFGUEREMUSR *pSrc, CFGUEREMUSR *pDst );
static void          clrRemUsrCfg( CCTKREMUSRCFG *pCfg );
static CFGSTATUS     cpyRemUsrCfg( const CCTKREMUSRCFG *pSrc, CCTKREMUSRCFG *pDst );
#endif

static void clrAccessNetworkInfoCfg( CCTKPANICFG *pCfg );
static CCTKPANICFG *dupAccessNetworkInfoCfg( const CCTKPANICFG *pSrc );

/* Config framework (de)initialization helper functions */
static CFGSTATUS     initCfgBlk( void );
static CFGSTATUS     initExtSysCfg( void );
static CFGSTATUS     initExtUsrCfg( BOS_UINT32 uIdx );
static void          deInitCfgBlk( void );
static CFGSTATUS     deInitCfgFw( void );

/* Config framework utility functions */
static BOS_SINT32    getIdx( CFGTYPE eType, BOS_UINT32 uGuid );

/* Functions for setting external (main + extended) configurations */
static CFGSTATUS     setSysXtlCfg( CCTKPARM eParm, void *&rpCfg );
static CFGSTATUS     setUsrXtlCfg( BOS_UINT32 uIdx, CCTKPARM eParm, void *&rpCfg );
static CFGSTATUS     setCallXtlCfg( BOS_UINT32 uIdx,
                                    CCTKPARM eParm,
                                    void *&pCfg );
static CFGSTATUS     setEvtXtlCfg( BOS_UINT32 uIdx, CCTKPARM eParm, void *&rpCfg );

/* Functions for updating internal configurations */
static void          updSysIntCfg( void );

/* Configuration validation functions */
static BOS_BOOL      isSysCfgValid( const CCTKCFG *pCfg );
static BOS_BOOL      isUsrCfgValid( const CCTKUSRCFG *pCfg );
static BOS_BOOL      isXtlCfgValid( CCTKPARM eParm, const void *pCfg );
static BOS_BOOL      isXtlUsrCfgValid( BOS_UINT32 uGuid,
                                       CCTKPARM eParm,
                                       const void *pCfg );

/* ---- Functions --------------------------------------------------------------- */
/***********************************************************************************
**  FUNCTION:   cfgInit
**
**  PURPOSE:    Initialize the Configuration Framework module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function must be called prior to any Configuration
**                Framework API is called.
**              - This function is synchronous and MUST only be called within
**                the application task context.
***********************************************************************************/
CFGSTATUS cfgInit( void )
{
   CCTK_TRACED0( "cfg - cfgInit()" );

   if ( gCfmCtrlBlk.pEngine != NULL )
   {
      CCTK_TRACEI0( "cfg - cfgInit: Config Framework already initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   /* -- Internal validity check -- */
#if CCTKCFG_DEBUG
   /* Check for external config parameter debug string
   ** (for detecting the enum list size change)
   */
   BOS_SINT32 sLstSize = ( eCCTKPARM_MC_END - eCCTKPARM_MC_START + 1 ) +
                         ( eCCTKPARM_EC_END - eCCTKPARM_EC_START + 1 );
   CCTK_ASSERT( sizeof( gpcCfgParmStr ) / sizeof( BOS_UINT8 * ) == sLstSize );
#endif

   /* Check for status code mis-alignment */
   BOS_SINT32 sCodeSize =
      ( eCFGSTATUS_SUCCESS_END - eCFGSTATUS_SUCCESS_START + 1 ) +
      ( eCFGSTATUS_FAIL_END - eCFGSTATUS_FAIL_START + 1);
   CCTK_ASSERT( sCodeSize == eCFGSTATUS_MAX );
   /* -- Internal validity check (END) -- */

   /* Initialize configuration block */
   CFGSTATUS eRes = initCfgBlk();
   CCTK_ASSERT( eRes == eCFGSTATUS_SUCCESS );
   if ( CFGSTATUS_IS_FAIL( eRes ) )
   {
      CCTK_TRACEI1( "cfg - cfgInit: Out of resources (%d)", eRes );
      return eRes;
   }

   /* Update the system configuration block with the pre-defined extended config */
   eRes = initExtSysCfg();
   if ( CFGSTATUS_IS_FAIL( eRes ) )
   {
      CCTK_TRACEI1(
         "cfg - cfgInit: Invalid pre-defined extended system config (%d).", eRes );
   }

   /* Initialize Config Engine */
   if ( CFGSTATUS_IS_SUCCESS( eRes ) )
   {
      /* CFG engine can only be created in the application task context */
      gCfmCtrlBlk.pEngine = BRCM_NEW( MX_NS CfgEngine );
      CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
      if ( gCfmCtrlBlk.pEngine == NULL )
      {
         CCTK_TRACEI0( "cfg - cfgInit: Error creating Config Engine." );
         eRes = eCFGSTATUS_NORSC;
      }
   }

   /* Check Config Engine initialization status */
   if ( CFGSTATUS_IS_SUCCESS( eRes ) )
   {
      eRes = gCfmCtrlBlk.pEngine->getInitStatus();
      if ( CFGSTATUS_IS_FAIL( eRes ) )
      {
         CCTK_TRACEI1(
            "cfg - cfgInit: Error initializing Config Engine (%d)", eRes );
      }
   }

   /* Clean-ups on failure */
   if ( CFGSTATUS_IS_FAIL( eRes ) )
   {
      deInitCfgFw();
   }

   return eRes;
}

/***********************************************************************************
**  FUNCTION:   cfgDeInit
**
**  PURPOSE:    De-initialize the Configuration Framework module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - Further Configuration Framework API MUST not be called after
**                this API has been called.
**              - If cfgStartup() has been called successfully, cfgShutDown()
**                MUST be called prior to calling this function.
**              - This function is synchronous and MUST only be called within
**                the application task context.
***********************************************************************************/
CFGSTATUS cfgDeInit( void )
{
   CCTK_TRACED0( "cfg - cfgDeInit()" );

   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgDeInit: Config Framework has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   return deInitCfgFw();
}

/***********************************************************************************
**  FUNCTION:   cfgIsAllCfgRel
**
**  PURPOSE:    Check for any outstanding config resources.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Return true if there are outstanding config resources allocated.
**
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsAllCfgRel( void )
{
   CCTK_TRACED0( "cfg - cfgIsAllCfgRel()" );

   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0(
         "cfg - cfgIsAllCfgRel: Config Framework has not been initialized." );
      return BOS_TRUE;
   }

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
   {
      if ( CFM_CFG.pUsr[uIdx] != NULL &&
           CFM_CFG.pUsr[uIdx]->uGuid != guCeInvalidGuid )
      {
         return BOS_FALSE;
      }
   }

   for ( uIdx = 0; uIdx < CCTKCFG_MAX_CALL; uIdx++ )
   {
      if ( CFM_CFG.pCall[uIdx] != NULL &&
           CFM_CFG.pCall[uIdx]->uGuid != guCeInvalidGuid )
      {
         return BOS_FALSE;
      }
   }

   for ( uIdx = 0; uIdx < CCTKCFG_MAX_EVT; uIdx++ )
   {
      if ( CFM_CFG.pEvt[uIdx] != NULL &&
           CFM_CFG.pEvt[uIdx]->uGuid != guCeInvalidGuid )
      {
         return BOS_FALSE;
      }
   }

   return BOS_TRUE;
}

/***********************************************************************************
**  FUNCTION:   cfgIsFuncAllowed
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
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsFuncAllowed( TKM_MSGID eFuncId )
{
   CCTK_TRACED1( "cfg - cfgIsFuncAllowed(%d)", eFuncId );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0(
         "cfg - cfgIsFuncAllowed: Config Framework has not been initialized." );
      return BOS_FALSE;
   }

   return gCfmCtrlBlk.pEngine->isFuncAllowed( eFuncId );
}

/***********************************************************************************
**  FUNCTION:   cfgDupSystem
**
**  PURPOSE:    Duplicate main system configuration.
**
**  PARAMETERS:
**              pSrc - The main system config to be duplicated.
**
**
**  RETURNS:    Pointer of the duplicated system config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      None.
***********************************************************************************/
CCTKCFG *cfgDupSystem( const CCTKCFG *pSrc )
{
   CCTK_TRACED1( "cfg - cfgDupSystem(%p)", pSrc );

   CCTKCFG *pRes = NULL;

   if ( pSrc != NULL && isSysCfgValid( pSrc ) )
   {
      CFM_VARDUP( pSrc, pRes, CCTKCFG );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         BOS_UINT32 uIdx;

         pRes->loc.gen.pcSeed = utilStrDup( pSrc->loc.gen.pcSeed );
         for ( uIdx = 0; uIdx < pSrc->net.gen.host.uNum; uIdx++ )
         {
            pRes->net.gen.host.addr[uIdx].pcFqdn =
               utilStrDup( pSrc->net.gen.host.addr[uIdx].pcFqdn );
         }
         cpyEmgAddr( &( pSrc->net.prot.gen.emgAddr ),
                     &( pRes->net.prot.gen.emgAddr ) );
      }
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgDelSystem
**
**  PURPOSE:    Delete main system configuration and de-allocate its content.
**
**  PARAMETERS:
**              rpCfg - The main system config to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void cfgDelSystem( CCTKCFG *&rpCfg )
{
   CCTK_TRACED1( "cfg - cfgDelSystem(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      BOS_UINT32 uIdx;

      BRCM_DELETE_ARRAY( rpCfg->loc.gen.pcSeed );
      /* Array boundary check. Use assert only because it should never happen
      ** since we have similar check in cfgDupSystem().
      */
      CCTK_ASSERT( rpCfg->net.gen.host.uNum <= CCTKCFG_MAX_HOSTADDR );
      for ( uIdx = 0; uIdx < rpCfg->net.gen.host.uNum; uIdx++ )
      {
         BRCM_DELETE_ARRAY( rpCfg->net.gen.host.addr[uIdx].pcFqdn );
      }
      clrEmgAddr( &( rpCfg->net.prot.gen.emgAddr ) );

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   cfgDupUsrCfg
**
**  PURPOSE:    Duplicate main user configuration.
**
**  PARAMETERS:
**              pSrc - The main user config to be duplicated.
**
**
**  RETURNS:    Pointer of the duplicated user config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      None.
***********************************************************************************/
CCTKUSRCFG *cfgDupUsrCfg( const CCTKUSRCFG *pSrc )
{
   CCTK_TRACED1( "cfg - cfgDupUsrCfg(%p)", pSrc );

   CCTKUSRCFG *pRes = NULL;

   if ( pSrc != NULL && isUsrCfgValid( pSrc ) )
   {
      CFM_VARDUP( pSrc, pRes, CCTKUSRCFG );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         cpySipHost( &( pSrc->dft.call.supp.mohServer ),
                     &( pRes->dft.call.supp.mohServer ) );
         cpyMediaCfg( &( pSrc->dft.call.media ), &( pRes->dft.call.media ) );
         cpySipUsrHost( &( pSrc->mwi.gen.server ), &( pRes->mwi.gen.server ) );
         cpyHostPort( &( pSrc->reg.gen.aor ), &( pRes->reg.gen.aor ) );
         cpySipHost( &( pSrc->reg.gen.registrar ), &( pRes->reg.gen.registrar ) );
         cpyRouteLst( &( pSrc->usr.net.obProxyLst ),
                      &( pRes->usr.net.obProxyLst ) );
         cpySipName( &( pSrc->usr.net.sipName ), &( pRes->usr.net.sipName ) );
         cpyAuthcInfo( &( pSrc->usr.sec.auth.client ),
                       &( pRes->usr.sec.auth.client ) );
         cpySipHost( &( pSrc->usr.sec.nat.kaCfg.server ),
                     &( pRes->usr.sec.nat.kaCfg.server ) );
      }
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgDelUsrCfg
**
**  PURPOSE:    Delete main user configuration and de-allocate its content.
**
**  PARAMETERS:
**              rpCfg - The main user config to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void cfgDelUsrCfg( CCTKUSRCFG *&rpCfg )
{
   CCTK_TRACED1( "cfg - cfgDelUsrCfg(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      /* Array boundary check. Use assert only because it should never happen
      ** since we have similar check in cfgDupUsrCfg().
      */
      CCTK_ASSERT( rpCfg->usr.sec.auth.client.rlmlst.uNum <= CCTKCFG_MAX_REALM );

      clrSipHost( &( rpCfg->dft.call.supp.mohServer ) );
      clrMediaCfg( &( rpCfg->dft.call.media ) );
      clrSipUsrHost( &( rpCfg->mwi.gen.server ) );
      clrHostPort( & ( rpCfg->reg.gen.aor ) );
      clrSipHost( &( rpCfg->reg.gen.registrar ) );
      clrRouteLst( &( rpCfg->usr.net.obProxyLst ) );
      clrSipName( &( rpCfg->usr.net.sipName ) );
      clrAuthcInfo( &( rpCfg->usr.sec.auth.client ) );
      clrSipHost( &( rpCfg->usr.sec.nat.kaCfg.server ) );

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   cfgDupXtlCfg
**
**  PURPOSE:    Duplicate external configuration (main + extended config).
**
**  PARAMETERS:
**              eParm   - Parameter ID.
**              pSrc    - The external config to be duplicated.
**
**
**  RETURNS:    Pointer of the duplicated external config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      None.
***********************************************************************************/
void *cfgDupXtlCfg( CCTKPARM eParm, const void *pSrc )
{
   CCTK_TRACED2( "cfg - cfgDupXtlCfg(%d, %p)", eParm, pSrc );

   void *pRes = NULL;

   if ( pSrc != NULL && isXtlCfgValid( eParm, pSrc ) )
   {
      switch ( eParm )
      {
      case eCCTKPARM_MC_SIPNAME:
         {
            pRes = dupSipName( static_cast< const CCTKSIPNAME * >( pSrc ) );
         }
         break;
      case eCCTKPARM_MC_OBPROXYLST:
         {
            pRes = dupRouteLst( static_cast< const CCTKROUTELST * >( pSrc ) );
         }
         break;
      case eCCTKPARM_MC_MWISVRADDR:
         {
            pRes = dupSipUsrHost( static_cast< const CCTKSIPUSRHOST * >( pSrc ) );
         }
         break;
      case eCCTKPARM_MC_MEDIACFG:
         {
            pRes = dupMediaCfg( static_cast< const CCTKMEDCFG * >( pSrc ) );
         }
         break;
      case eCCTKPARM_MC_MEDIALIST:
         {
            CFM_VARDUP( static_cast< const CCTKMEDLST * >( pSrc ),
                        pRes,
                        CCTKMEDLST );
         }
         break;
      case eCCTKPARM_EC_SUBEVT:
      case eCCTKPARM_EC_INTEROP:
         {
            CFM_VARDUP( static_cast< const BOS_UINT32 * >( pSrc ),
                        pRes,
                        BOS_UINT32 );
         }
         break;
      case eCCTKPARM_EC_TRANSPORT:
         {
            CFM_VARDUP( static_cast< const CCTKSIGTRANS * >( pSrc ),
                        pRes,
                        CCTKSIGTRANS );
         }
         break;
      case eCCTKPARM_EC_GENHDRCFG:
         {
            pRes = dupGenHdr( static_cast< const CCTKGENHDRCFG * >( pSrc ) );
         }
         break;
      case eCCTKPARM_EC_USRPORTCFG:
         {
            CFM_VARDUP( static_cast< const CCTKUSRPORTCFG * >( pSrc ),
                        pRes,
                        CCTKUSRPORTCFG );
         }
         break;
      case eCCTKPARM_EC_DIAGEVTCFG:
         {
            pRes = dupDiagEvtCfg( static_cast< const CCTKDIAGEVTCFG * >( pSrc ) );
         }
         break;
      case eCCTKPARM_EC_UAPEVTCFG:
         {
            pRes = dupUapEvtCfg( static_cast< const CCTKUAPEVTCFG * >( pSrc ) );
         }
         break;
      case eCCTKPARM_EC_USREMGCFG:
         {
            CFM_VARDUP( static_cast< const CCTKUSREMGCFG * >( pSrc ),
                        pRes,
                        CCTKUSREMGCFG );
         }
         break;
      case eCCTKPARM_EC_USRBRCFG:
         {
            CFM_VARDUP( static_cast< const CCTKUSRBRCFG * >( pSrc ),
                        pRes,
                        CCTKUSRBRCFG );
         }
         break;
      case eCCTKPARM_EC_SVCROUTE:
         {
            pRes = dupSvcRoute( static_cast< const CCTKSVCROUTE * >( pSrc ) );
         }
         break;
      case eCCTKPARM_EC_USRCONTACT:
         {
            pRes = dupContact( static_cast< const CCTKCONTACT * >( pSrc ) );
         }
         break;
      case eCCTKPARM_EC_AUTHCFG:
         {
            CFM_VARDUP( static_cast< const CCTKAUTHCFG * >( pSrc ),
                        pRes,
                        CCTKAUTHCFG );
         }
         break;
      case eCCTKPARM_EC_WHTLST:
         {
            CFM_VARDUP( static_cast< const CCTKWHTLSTCFG * >( pSrc ),
                        pRes,
                        CCTKWHTLSTCFG );
         }
         break;
      case eCCTKPARM_EC_BLKLST:
         {
            CFM_VARDUP( static_cast< const CCTKIPADDRLST * >( pSrc ),
                        pRes,
                        CCTKIPADDRLST );
         }
         break;
      case eCCTKPARM_EC_REGTMR:
         {
            CFM_VARDUP( static_cast< const CCTKREGTMR * >( pSrc ),
                        pRes,
                        CCTKREGTMR );
         }
         break;
      case eCCTKPARM_EC_REGEVTCFG:
         {
            CFM_VARDUP( static_cast< const CCTKREGEVTCFG * >( pSrc ),
                        pRes,
                        CCTKREGEVTCFG );
         }
         break;
      case eCCTKPARM_EC_MWICFG:
         {
            CFM_VARDUP( static_cast< const CCTKMWICFG * >( pSrc ),
                        pRes,
                        CCTKMWICFG );
         }
         break;
      case eCCTKPARM_EC_MWITMR:
         {
            CFM_VARDUP( static_cast< const CCTKMWITMR * >( pSrc ),
                        pRes,
                        CCTKMWITMR );
         }
         break;
      case eCCTKPARM_EC_EMGCALLCFG:
         {
            CFM_VARDUP( static_cast< const CCTKEMGCALLCFG * >( pSrc ),
                        pRes,
                        CCTKEMGCALLCFG );
         }
         break;
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
      case eCCTKPARM_EC_TELCALL:
         {
            CFM_VARDUP( static_cast< const CCTKTELCALL * >( pSrc ),
                        pRes,
                        CCTKTELCALL );
         }
         break;
#endif
      case eCCTKPARM_EC_CALLSTATS:
         {
            CFM_VARDUP( static_cast< const CCTKCALLMETRICS * >( pSrc ),
                        pRes,
                        CCTKCALLMETRICS );
         }
         break;
      case eCCTKPARM_EC_PRACK:
         {
            CFM_VARDUP( static_cast< const CCTKPRACK * >( pSrc ),
                        pRes,
                        CCTKPRACK );
         }
         break;
      case eCCTKPARM_EC_SESTMR:
         {
            CFM_VARDUP( static_cast< const CCTKSESTMR * >( pSrc ),
                        pRes,
                        CCTKSESTMR );
         }
         break;
      case eCCTKPARM_EC_CALLPRIV:
      case eCCTKPARM_EC_EVTPRIV:
         {
            CFM_VARDUP( static_cast< const CCTKPRIVCFG * >( pSrc ),
                        pRes,
                        CCTKPRIVCFG );
         }
         break;
      case eCCTKPARM_EC_CALLNAI:
      case eCCTKPARM_EC_EVTNAI:
         {
            pRes = dupNaiCfg( static_cast< const CCTKNAICFG * >( pSrc ) );
         }
         break;
      case eCCTKPARM_EC_CONNECTEDID:
         {
            CFM_VARDUP( static_cast< const CCTKCONNECTEDID * >( pSrc ),
                        pRes,
                        CCTKCONNECTEDID );
         }
         break;
      case eCCTKPARM_EC_CALLHOLD:
         {
            CFM_VARDUP( static_cast< const CCTKHOLDCFG * >( pSrc ),
                        pRes,
                        CCTKHOLDCFG );
         }
         break;
      case eCCTKPARM_EC_CALLXFER:
         {
            CFM_VARDUP( static_cast< const CCTKXFERCFG * >( pSrc ),
                        pRes,
                        CCTKXFERCFG );
         }
         break;
      case eCCTKPARM_EC_MEDNEGCFG:
         {
            CFM_VARDUP( static_cast< const CCTKMEDNEGCFG * >( pSrc ),
                        pRes,
                        CCTKMEDNEGCFG );
         }
         break;
      case eCCTKPARM_EC_EVTTMR:
         {
            CFM_VARDUP( static_cast< const CCTKEVTTMR * >( pSrc ),
                        pRes,
                        CCTKEVTTMR );
         }
         break;
      case eCCTKPARM_EC_LOCPORT:
         {
            CFM_VARDUP( static_cast< const CCTKLOCPORT * >( pSrc ),
                        pRes,
                        CCTKLOCPORT );
         }
         break;
#if CCTKCFG_TLS
      case eCCTKPARM_EC_TLSCFG:
         {
            CFM_VARDUP( static_cast< const CCTKTLSCFG * >( pSrc ),
                        pRes,
                        CCTKTLSCFG );

            /* Copy the ones that require dynamic memory allocation */
            if ( pRes != NULL )
            {
               const CCTKTLSCFG *pFrom = static_cast< const CCTKTLSCFG * >( pSrc );
               CCTKTLSCFG *pTo = static_cast< CCTKTLSCFG * >( pRes );

               if ( cpyTlsCfg( pFrom, pTo ) != eCFGSTATUS_SUCCESS )
               {
                  BRCM_DELETE( pRes );
                  pRes = NULL;
               }
            }
         }
         break;
#endif
      case eCCTKPARM_EC_CNXACTIVITY:
         {
            CFM_VARDUP( static_cast< const CCTKCNXCFG * >( pSrc ),
                        pRes,
                        CCTKCNXCFG );
         }
         break;
      case eCCTKPARM_EC_UAIDHDR:
         {
            CFM_VARDUP( static_cast< const CCTKUAIDHDR * >( pSrc ),
                        pRes,
                        CCTKUAIDHDR );

            /* Copy the ones that require dynamic memory allocation */
            if ( pRes != NULL )
            {
               const CCTKUAIDHDR *pFrom =
                  static_cast< const CCTKUAIDHDR * >( pSrc );
               CCTKUAIDHDR *pTo = static_cast< CCTKUAIDHDR * >( pRes );

               if ( cpyUaIdHdr( pFrom, pTo ) != eCFGSTATUS_SUCCESS )
               {
                  DeletePtr((CCTKUAIDHDR**) &pRes);
               }
            }
         }
         break;
      case eCCTKPARM_EC_ROUTINGHDR:
         {
            CFM_VARDUP( static_cast< const CCTKROUTINGHDR * >( pSrc ),
                        pRes,
                        CCTKROUTINGHDR );
         }
         break;
      case eCCTKPARM_EC_DNSMODE:
         {
            CFM_VARDUP( static_cast< const CCTKDNSMODE * >( pSrc ),
                        pRes,
                        CCTKDNSMODE );
         }
         break;
      case eCCTKPARM_EC_SIGBEHAVE:
         {
            CFM_VARDUP( static_cast< const CCTKSIGBEHAVE * >( pSrc ),
                        pRes,
                        CCTKSIGBEHAVE );
         }
         break;
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
      case eCCTKPARM_EC_IMSCFG:
         {
            CFM_VARDUP( static_cast< const CCTKIMSCFG * >( pSrc ),
                        pRes,
                        CCTKIMSCFG );
         }
         break;
#endif
      case eCCTKPARM_EC_GRUUCFG:
         {
            CFM_VARDUP( static_cast< const CCTKGRUUCFG * >( pSrc ),
                        pRes,
                        CCTKGRUUCFG );
         }
         break;
      case eCCTKPARM_EC_PAUCFG:
         {
            CFM_VARDUP( static_cast< const CCTKPAUCFG * >( pSrc ),
                        pRes,
                        CCTKPAUCFG );
         }
         break;
      case eCCTKPARM_EC_PANICFG:
         {
            pRes = dupAccessNetworkInfoCfg( static_cast< const CCTKPANICFG * >(
                                            pSrc ) );
         }
         break;
      case eCCTKPARM_EC_TRANSTMR:
         {
            CFM_VARDUP( static_cast< const CCTKTRANSTMRCFG * >( pSrc ),
                        pRes,
                        CCTKTRANSTMRCFG );
         }
         break;
      case eCCTKPARM_EC_PKTLOG:
         {
            CFM_VARDUP( static_cast< const CCTKPKTLOG * >( pSrc ),
                        pRes,
                        CCTKPKTLOG );
         }
         break;
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
      case eCCTKPARM_EC_TRACE:
         {
            CFM_VARDUP( static_cast< const CCTKTRACE * >( pSrc ),
                        pRes,
                        CCTKTRACE );
         }
         break;
#endif
      case eCCTKPARM_EC_NATGENCFG:
         {
            CFM_VARDUP( static_cast< const CCTKNATGENCFG * >( pSrc ),
                        pRes,
                        CCTKNATGENCFG );
         }
         break;
      case eCCTKPARM_EC_NATRGCTL:
         {
            CFM_VARDUP( static_cast< const CCTKRGNATCTL * >( pSrc ),
                        pRes,
                        CCTKRGNATCTL );
         }
         break;
#if CCTKCFG_MEMORY_MONITORING
      case eCCTKPARM_EC_MEMPROTECT:
         {
            CFM_VARDUP( static_cast< const CCTKMEMPROTCFG * >( pSrc ),
                        pRes,
                        CCTKMEMPROTCFG );
         }
         break;
#endif
#if CCTKCFG_REMOTE_USER_CONTROL
      case eCCTKPARM_EC_REMUSRCFG:
         {
            CFM_VARDUP( static_cast< const CCTKREMUSRCFG * >( pSrc ),
                        pRes,
                        CCTKREMUSRCFG );

            /* Copy the ones that require dynamic memory allocation */
            if ( pRes != NULL )
            {
               const CCTKREMUSRCFG *pFrom = static_cast< const CCTKREMUSRCFG * >( pSrc );
               CCTKREMUSRCFG *pTo = static_cast< CCTKREMUSRCFG * >( pRes );

               if ( cpyRemUsrCfg( pFrom, pTo ) != eCFGSTATUS_SUCCESS )
               {
                  BRCM_DELETE( pRes );
                  pRes = NULL;
               }
            }
         }
         break;
      case eCCTKPARM_EC_REMUSRSYS:
         {
            CFM_VARDUP( static_cast< const CCTKREMUSRSYS * >( pSrc ),
                        pRes,
                        CCTKREMUSRSYS );

            /* Copy the ones that require dynamic memory allocation */
            if ( pRes != NULL )
            {
               const CCTKREMUSRSYS *pFrom = static_cast< const CCTKREMUSRSYS * >( pSrc );
               CCTKREMUSRSYS *pTo = static_cast< CCTKREMUSRSYS * >( pRes );

               if ( cpyRemUsrSysCfg( pFrom, pTo ) != eCFGSTATUS_SUCCESS )
               {
                  BRCM_DELETE( pRes );
                  pRes = NULL;
               }
            }
         }
         break;
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
#if CCTKCFG_ICE_LITE_SUPPORT
      case eCCTKPARM_EC_ICECTLCFG:
         {
            CFM_VARDUP( static_cast< const CCTKICECTLCFG * >( pSrc ),
                        pRes,
                        CCTKICECTLCFG );
         }
         break;
      case eCCTKPARM_EC_ICEIPLSTCFG:
      {
         CFM_VARDUP( static_cast< const CCTKIPADDRLST * >( pSrc ),
                        pRes,
                        CCTKIPADDRLST );
      }
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
      default:
         {
            CCTK_TRACEI1( "cfg - cfgDupXtlCfg: Invalid parameter = %d", eParm );
         }
         break;
      }
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgDelXtlCfg
**
**  PURPOSE:    Delete external configuration and de-allocate its content.
**
**  PARAMETERS:
**              eParm   - Parameter ID.
**              rpCfg   - The external config to be deleted.
**                        Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void cfgDelXtlCfg( CCTKPARM eParm, void *&rpCfg )
{
   CCTK_TRACED2( "cfg - cfgDelXtlCfg(%d, %p)", eParm, rpCfg );

   if ( rpCfg != NULL )
   {
      switch ( eParm )
      {
      case eCCTKPARM_MC_SIPNAME:
         {
            clrSipName( static_cast< CCTKSIPNAME * >( rpCfg ) );
            DeletePtr((CCTKSIPNAME**) &rpCfg);
         }
         break;
      case eCCTKPARM_MC_OBPROXYLST:
         {
            clrRouteLst( static_cast< CCTKROUTELST * >( rpCfg ) );
            DeletePtr((CCTKROUTELST**) &rpCfg);
         }
         break;
      case eCCTKPARM_MC_MWISVRADDR:
         {
            clrSipUsrHost( static_cast< CCTKSIPUSRHOST * >( rpCfg ) );
            DeletePtr((CCTKSIPUSRHOST**) &rpCfg);
         }
         break;
      case eCCTKPARM_EC_GENHDRCFG:
         {
            clrGenHdr( static_cast< CCTKGENHDRCFG * >( rpCfg ) );
            DeletePtr((CCTKGENHDRCFG**) &rpCfg);
         }
         break;
      case eCCTKPARM_EC_DIAGEVTCFG:
         {
            clrDiagEvtCfg( static_cast< CCTKDIAGEVTCFG * >( rpCfg ) );
            DeletePtr((CCTKDIAGEVTCFG**) &rpCfg);
         }
         break;
      case eCCTKPARM_EC_UAPEVTCFG:
         {
            clrUapEvtCfg( static_cast< CCTKUAPEVTCFG * >( rpCfg ) );
            DeletePtr((CCTKUAPEVTCFG**) &rpCfg);
         }
         break;
      case eCCTKPARM_EC_SVCROUTE:
         {
            clrSvcRoute( static_cast< CCTKSVCROUTE * >( rpCfg ) );
            DeletePtr((CCTKSVCROUTE**) &rpCfg);
         }
         break;
      case eCCTKPARM_EC_USRCONTACT:
         {
            clrContact( static_cast< CCTKCONTACT * >( rpCfg ) );
            DeletePtr((CCTKCONTACT**) &rpCfg);
         }
         break;
      case eCCTKPARM_EC_CALLNAI:
      case eCCTKPARM_EC_EVTNAI:
         {
            clrNaiCfg( static_cast< CCTKNAICFG * >( rpCfg ) );
            DeletePtr((CCTKNAICFG**) &rpCfg);
         }
         break;
#if CCTKCFG_TLS
      case eCCTKPARM_EC_TLSCFG:
         {
            clrTlsCfg( static_cast< CCTKTLSCFG * >( rpCfg ) );
            DeletePtr((CCTKTLSCFG**) &rpCfg);
         }
         break;
#endif
      case eCCTKPARM_EC_UAIDHDR:
         {
            clrUaIdHdr( static_cast< CCTKUAIDHDR * >( rpCfg ) );
            DeletePtr((CCTKUAIDHDR**) &rpCfg);
         }
         break;
     case eCCTKPARM_EC_PANICFG:
         {
            clrAccessNetworkInfoCfg( static_cast< CCTKPANICFG * >( rpCfg ) );
            DeletePtr((CCTKPANICFG**) &rpCfg);
         }
         break;
#if CCTKCFG_REMOTE_USER_CONTROL
      case eCCTKPARM_EC_REMUSRSYS:
         {
            clrRemUsrSysCfg ( static_cast< CCTKREMUSRSYS * >( rpCfg ) );
            DeletePtr((CCTKREMUSRSYS**) &rpCfg);
         }
         break;
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
      default:
         {
            /* Cast to a random type since we are
            ** not sure what will be passed in exactly.
            */
            DeletePtr((CCTKEVTTMR**) &rpCfg);
         }
         break;
      }
   }
}

/***********************************************************************************
**  FUNCTION:   cfgStartUp
**
**  PURPOSE:    Start up Config Framework and activate the Main Framework.
**
**  PARAMETERS:
**              pMainFramework - Main framework to be started up.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous and MUST only be called within
**                the application task context.
***********************************************************************************/
CFGSTATUS cfgStartUp( MX_NS CEventDriven *pMainFramework )
{
   CCTK_TRACED1( "cfg - cfgStartUp(%p)", pMainFramework );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgStartUp: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( pMainFramework == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgStartUp: NULL Main Framework handle." );
      return eCFGSTATUS_BADPARM;
   }

   return gCfmCtrlBlk.pEngine->startUp( pMainFramework );
}

/***********************************************************************************
**  FUNCTION:   cfgShutDown
**
**  PURPOSE:    Shut down Config Framework.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous and MUST only be called within
**                the application task context.
***********************************************************************************/
CFGSTATUS cfgShutDown( void )
{
   CCTK_TRACED0( "cfg - cfgShutDown()" );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgShutDown: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   return gCfmCtrlBlk.pEngine->shutDown();
}

/***********************************************************************************
**  FUNCTION:   cfgReset
**
**  PURPOSE:    Reset the Config Framework by releasing all outstanding resources.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgReset( void )
{
   CCTK_TRACED0( "cfg - cfgReset()" );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0(
         "cfg - cfgReset: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   /* Release all call resoureces */
   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_CALL; uIdx++ )
   {
      if ( CFM_CFG.pCall[uIdx]->uGuid != guCeInvalidGuid )
      {
         eStatus = cfgRemoveCall( CFM_CFG.pCall[uIdx]->uGuid );
         if ( CFGSTATUS_IS_FAIL ( eStatus ) )
         {
            CCTK_TRACEI2(
               "cfg - cfgReset: Error removing call resource (%lu, %d).",
               uIdx,
               eStatus );
            return eStatus;
         }
      }
   }

   /* Release all event resources */
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_EVT; uIdx++ )
   {
      if ( CFM_CFG.pEvt[uIdx]->uGuid != guCeInvalidGuid )
      {
         eStatus = cfgRemoveEvt( CFM_CFG.pEvt[uIdx]->uGuid );
         if ( CFGSTATUS_IS_FAIL ( eStatus ) )
         {
            CCTK_TRACEI2(
               "cfg - cfgReset: Error removing event resource (%lu, %d).",
               uIdx,
               eStatus );
            return eStatus;
         }
      }
   }

   /* Release all user resources */
   for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
   {
      if ( CFM_CFG.pUsr[uIdx]->uGuid != guCeInvalidGuid )
      {
         eStatus = cfgRemoveUsr( CFM_CFG.pUsr[uIdx]->uGuid );
         if ( CFGSTATUS_IS_FAIL ( eStatus ) )
         {
            CCTK_TRACEI2(
               "cfg - cfgReset: Error removing user resource (%lu, %d).",
               uIdx,
               eStatus );
            return eStatus;
         }
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   cfgSetupSystem
**
**  PURPOSE:    Set up the system main configuration and (re)configure the stack.
**
**  PARAMETERS:
**              rpSysCfg   - The system main config to be configured.
**                           Ownership is taken by the function on success.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgSetupSystem( CCTKCFG *&rpSysCfg )
{
   CCTK_TRACED1( "cfg - cfgSetupSystem(%p)", rpSysCfg );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0(
         "cfg - cfgSetupSystem: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( rpSysCfg == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgSetupSystem: NULL config pointer." );
      return eCFGSTATUS_BADPARM;
   }

   if ( !isSysCfgValid( rpSysCfg ) )
   {
      CCTK_TRACEI0( "cfg - cfgSetupSystem: Invalid system config block." );
      return eCFGSTATUS_BADPARM;
   }

   /* De-allocate any previously allocated memory */
   clrSysXtlCfg( &( CFM_CFG.pSys->xtl ), BOS_TRUE );

   /* Copy to config framework structure */
   memcpy( &( CFM_CFG.pSys->xtl.loc.gen ),
           &( rpSysCfg->loc.gen ),
           sizeof( CCTKLOCGEN ) );
   memcpy( &( CFM_CFG.pSys->xtl.net.gen ),
           &( rpSysCfg->net.gen ),
           sizeof( CCTKNETGEN ) );
   memcpy( &( CFM_CFG.pSys->xtl.net.prot.gen ),
           &( rpSysCfg->net.prot.gen ),
           sizeof( CCTKNETPROTGEN ) );

#if CCTKCFG_ICE_LITE_SUPPORT
   /* CFG_SYS->xtl.net.gen.host is updated */
   /* host local address list is ready, start ICE */
   nsIceRun();
#endif /* CCTKCFG_ICE_LITE_SUPPORT */

   BRCM_DELETE( rpSysCfg );
   rpSysCfg = NULL;

   /* Update internal system configuration */
   updSysIntCfg();

   /* (Re)Configure SCE and the SIP stack */
   return gCfmCtrlBlk.pEngine->config();
}

/***********************************************************************************
**  FUNCTION:   cfgAddUsr
**
**  PURPOSE:    Set up and add a user configuration resource.
**
**  PARAMETERS:
**              uGuid      - GUID.
**              rpUsrCfg   - The user main config to be configured.
**                           Ownership is taken by the function on success.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgAddUsr( BOS_UINT32 uGuid, CCTKUSRCFG *&rpUsrCfg )
{
   CCTK_TRACED2( "cfg - cfgAddUsr(0x%lX, %p)", uGuid, rpUsrCfg );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgAddUsr: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( uGuid == guCeInvalidGuid || rpUsrCfg == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgAddUsr: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   /* Check if the provided GUID is already in use by other resources */
   if ( cfgGetType( uGuid ) != eCFGTYPE_MAX )
   {
      CCTK_TRACEI0( "cfg - cfgAddUsr: User GUID is already in use." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
   {
      if ( CFM_CFG.pUsr[uIdx]->uGuid == guCeInvalidGuid )
      {
         /* Unused user resource found */
         break;
      }
   }

   if ( uIdx >= CFG_MAX_USR )
   {
      CCTK_TRACEI0( "cfg - cfgAddUsr: No user resource available." );
      return eCFGSTATUS_NORSC;
   }

   if ( !isUsrCfgValid( rpUsrCfg ) )
   {
      CCTK_TRACEI0( "cfg - cfgAddUsr: Invalid user config block." );
      return eCFGSTATUS_BADPARM;
   }

   /* Associate GUID to user config. This will be reset on failure. */
   CFM_CFG.pUsr[uIdx]->uGuid = uGuid;

   /* Update the user config block with the pre-defined extended config */
   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;
   if ( initExtUsrCfg( uIdx ) != eCFGSTATUS_SUCCESS )
   {
      CCTK_TRACEI0(
         "cfg - cfgAddUsr: Invalid pre-defined extended user config." );
      eStatus = eCFGSTATUS_BADEXTCFG;
   }

   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* De-allocate any previously allocated memory */
      clrUsrXtlCfg( &( CFM_CFG.pUsr[uIdx]->cfg.xtl ), BOS_TRUE );

      /* Copy and map to config framework structure */
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.gen ),
              &( rpUsrCfg->dft.call.basic.gen ),
              sizeof( CCTKCALLBASGEN ) );
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.supp ),
              &( rpUsrCfg->dft.call.supp ),
              sizeof( CCTKCALLSUPP ) );
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.media ),
              &( rpUsrCfg->dft.call.media ),
              sizeof( CCTKMEDCFG ) );
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.evt.gen ),
              &( rpUsrCfg->dft.evt.gen ),
              sizeof( CCTKEVTGEN ) );
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.mwi.gen ),
              &( rpUsrCfg->mwi.gen ),
              sizeof( CCTKMWIGEN ) );
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.reg.gen ),
              &( rpUsrCfg->reg.gen ),
              sizeof( CCTKREGGEN ) );
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net ),
              &( rpUsrCfg->usr.net ),
              sizeof( CCTKUSRNET ) );
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.sec.auth ),
              &( rpUsrCfg->usr.sec.auth ),
              sizeof( CCTKAUTH ) );
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.sec.nat ),
              &( rpUsrCfg->usr.sec.nat ),
              sizeof( CCTKNAT ) );
#if CCTKCFG_REMOTE_USER_CONTROL
      memcpy( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.remUsr ),
              &( rpUsrCfg->remUsr ),
              sizeof( CCTKREMUSR ) );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
      /* Not use cfgDelUsrCfg() intentionally */
      BRCM_DELETE( rpUsrCfg );
      rpUsrCfg = NULL;

      /* Set user-specific default call and event subscription configurations
      ** to the ones in the user configuration.
      */
      CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.usr.pGenHdr =
         &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.loc.genHdr );
      CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.evt.usr.pGenHdr =
         &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.loc.genHdr );
      CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.usr.pObProxyLst =
         &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.obProxyLst );
      CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.evt.usr.pObProxyLst =
         &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.obProxyLst );

      /* Add a user to Config Engine */
      eStatus = gCfmCtrlBlk.pEngine->addUsr( uIdx );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI2( "cfg - cfgAddUsr: "
            "Error adding a user to Config Engine (%u, %d).",
            (unsigned int)uGuid,
            eStatus );
      }
   }

   /* Clean ups */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      clrUsrCfg( CFM_CFG.pUsr[uIdx] );
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   cfgDupUsr
**
**  PURPOSE:    Duplicate a user configuration resource from an existing user
**              configuration resource.
**
**  PARAMETERS:
**              uGuid      - GUID of the duplicated user configuration resource.
**              uGuidSrc   - Source GUID of an existing user configuration resource.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgDupUsr( BOS_UINT32 uGuid, BOS_UINT32 uGuidSrc )
{
   CCTK_TRACED2( "cfg - cfgDupUsr(0x%lX, 0x%1X)", uGuid, uGuidSrc );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgDupUsr: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( uGuid == guCeInvalidGuid || uGuidSrc == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - cfgDupUsr: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   /* Check if the provided GUID is already in use by other resources */
   if ( cfgGetType( uGuid ) != eCFGTYPE_MAX )
   {
      CCTK_TRACEI0( "cfg - cfgDupUsr: User GUID is already in use." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 sIdx;
   CFGTYPE eGuidType = cfgGetType( uGuidSrc );
   switch ( eGuidType )
   {
   case eCFGTYPE_USR:
      {
         sIdx = getIdx( eGuidType, uGuidSrc );
         if ( sIdx == -1 )
         {
            CCTK_TRACEI0( "cfg - cfgDupUsr: Invalid source user GUID." );
            return eCFGSTATUS_BADPARM;
         }
      }
      break;
   default:
      {
         CCTK_TRACEI0( "cfg - cfgDupUsr: Source GUID is not a user GUID." );
         return eCFGSTATUS_BADPARM;
      }
      break;
   }

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
   {
      if ( CFM_CFG.pUsr[uIdx]->uGuid == guCeInvalidGuid )
      {
         /* Unused user resource found */
         break;
      }
   }

   if ( uIdx >= CFG_MAX_USR )
   {
      CCTK_TRACEI0( "cfg - cfgDupUsr: No user resource available." );
      return eCFGSTATUS_NORSC;
   }


   /* De-allocate any previously alloced memory */
   clrUsrCfg( CFM_CFG.pUsr[uIdx] );

   /* Copy external user configuration values */
   CFGSTATUS eStatus =
      cpyUsrXtlCfg( &( CFM_CFG.pUsr[sIdx]->cfg.xtl ),
                    &( CFM_CFG.pUsr[uIdx]->cfg.xtl ) );
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACED1( "cfg - cfgDupUsr: Error copying user config structure (%d).",
                    eStatus );
      return eCFGSTATUS_INTERR;
   }

   /* Copy internal user configuration values */
   CFM_CFG.pUsr[uIdx]->cfg.in = CFM_CFG.pUsr[sIdx]->cfg.in;

   /* The following internal user configurations require dynamic allocations */
   cpyHdr( &( CFM_CFG.pUsr[sIdx]->cfg.in.cachedAuth ),
           &( CFM_CFG.pUsr[uIdx]->cfg.in.cachedAuth ) );

   /* Do not copy the SCE user config object as it will be created by cfgEngine */
   CFM_CFG.pUsr[uIdx]->cfg.in.pSceUsrCfg = NULL;

   /* Associate GUID to user config. This will be reset on failure. */
   CFM_CFG.pUsr[uIdx]->uGuid = uGuid;

   /* Add a user to Config Engine */
   eStatus = gCfmCtrlBlk.pEngine->addUsr( uIdx );
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI2( "cfg - cfgDupUsr: "
         "Error adding a user to Config Engine (%lu, %d).",
         uGuid,
         eStatus );
   }

   /* Clean ups */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      clrUsrCfg( CFM_CFG.pUsr[uIdx] );
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   cfgRemoveUsr
**
**  PURPOSE:    Remove an existing user configuration resource.
**
**  PARAMETERS:
**              uGuid      - GUID.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgRemoveUsr( BOS_UINT32 uGuid )
{
   CCTK_TRACED1( "cfg - cfgRemoveUsr(0x%lX)", uGuid );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0(
         "cfg - cfgRemoveUsr: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( uGuid == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - cfgRemoveUsr: Invalid GUID." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 sIdx = getIdx( eCFGTYPE_USR, uGuid );
   if ( sIdx == -1 )
   {
      CCTK_TRACEI1( "cfg - cfgRemoveUsr: GUID 0x%1X not found.", uGuid );
      return eCFGSTATUS_BADPARM;
   }

   /* Check if an earlier user remove command is still in progress */
   if ( CFM_CFG.pUsr[sIdx]->cfg.in.bDeferEvtRemove )
   {
      CCTK_TRACEI1(
         "cfg - cfgRemoveUsr: Removing of user 0x%1X not complete yet.",
         uGuid );
      return eCFGSTATUS_NOTCMPL;
   }

   /* Remove a user from Config Engine */
   CFGSTATUS eStatus = gCfmCtrlBlk.pEngine->removeUsr( sIdx );
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI2( "cfg - cfgRemoveUsr: "
         "Error removing a user from Config Engine (0x%1X, %d).",
         uGuid,
         eStatus );
   }
   else
   {
      /* Do not clear the user configuration until the deferred user remove
      ** function event is sent.
      */
      if ( !CFM_CFG.pUsr[sIdx]->cfg.in.bDeferEvtRemove )
      {
         clrUsrCfg( CFM_CFG.pUsr[sIdx] );
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   cfgAddCall
**
**  PURPOSE:    Set up and add a call configuration resource.
**
**  PARAMETERS:
**              uGuid      - GUID of the call config resource.
**              uGuidSrc   - Source GUID. Can be either
**                             (1) GUID of the associated user config resource, or
**                             (2) GUID of an existing call.
**                           If (1) is used, the new call config resource is copied
**                           from the default call config settings in the user
**                           config resource specified in "uGuidSrc". The new call
**                           config resource is associated to the specified user
**                           config resource. If (2) is provided, the new call
**                           config resource is copied from the existing call
**                           config resource specified in "uGuidSrc". The new call
**                           config resource is associated to the same user config
**                           resource of the existing call config resource.
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgAddCall( BOS_UINT32 uGuid, BOS_UINT32 uGuidSrc )
{
   CCTK_TRACED2( "cfg - cfgAddCall(0x%lX, 0x%lX)", uGuid, uGuidSrc );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgAddCall: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( uGuid == guCeInvalidGuid || uGuidSrc == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - cfgAddCall: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   /* Check if the provided GUID is already in use by other resources */
   if ( cfgGetType( uGuid ) != eCFGTYPE_MAX )
   {
      CCTK_TRACEI0( "cfg - cfgAddCall: Call GUID is already in use." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 sIdx;
   CFGTYPE eGuidType = cfgGetType( uGuidSrc );
   switch ( eGuidType )
   {
   case eCFGTYPE_USR:
   case eCFGTYPE_CALL:
      {
         sIdx = getIdx( eGuidType, uGuidSrc );
         if ( sIdx == -1 )
         {
            CCTK_TRACEI0( "cfg - cfgAddCall: Invalid source GUID." );
            return eCFGSTATUS_BADPARM;
         }
      }
      break;
   default:
      {
         CCTK_TRACEI1(
            "cfg - cfgAddCall: Unknown source GUID type (%d).", eGuidType );
         return eCFGSTATUS_BADPARM;
      }
      break;
   }

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_CALL; uIdx++ )
   {
      if ( CFM_CFG.pCall[uIdx]->uGuid == guCeInvalidGuid )
      {
         /* Unused call resource found */
         break;
      }
   }

   if ( uIdx >= CCTKCFG_MAX_CALL )
   {
      CCTK_TRACED0( "cfg - cfgAddCall: No call resource available." );
      return eCFGSTATUS_NORSC;
   }

   /* De-allocate any previously alloced memory */
   clrCallCfg( CFM_CFG.pCall[uIdx] );

   /* Copy call configurations based on GUID type */
   const CFGCALLXTL *pSrcCfg = ( eGuidType == eCFGTYPE_USR ) ?
                               &( CFM_CFG.pUsr[sIdx]->cfg.xtl.dft.call ) :
                               &( CFM_CFG.pCall[sIdx]->cfg.xtl );

   CFGSTATUS eStatus = cpyCallXtlCfg( pSrcCfg, &( CFM_CFG.pCall[uIdx]->cfg.xtl ),
                                      BOS_FALSE );
   if ( eStatus != eCFGSTATUS_SUCCESS )
   {
      CCTK_TRACED1( "cfg - cfgAddCall: Error copying call config structure (%d).",
                    eStatus );
      return eCFGSTATUS_INTERR;
   }

   /* Call config setup successful. Associate GUID. */
   CFM_CFG.pCall[uIdx]->uGuid = uGuid;
   CFM_CFG.pCall[uIdx]->cfg.in.uUsrId = ( eGuidType == eCFGTYPE_USR ) ?
                                        uGuidSrc :
                                        CFM_CFG.pCall[sIdx]->cfg.in.uUsrId;

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cfgRemoveCall
**
**  PURPOSE:    Remove an existing call configuration resource.
**
**  PARAMETERS:
**              uGuid      - GUID.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgRemoveCall( BOS_UINT32 uGuid )
{
   CCTK_TRACED1( "cfg - cfgRemoveCall(0x%lX)", uGuid );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0(
         "cfg - cfgRemoveCall: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( uGuid == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - cfgRemoveCall: Invalid GUID." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 sIdx = getIdx( eCFGTYPE_CALL, uGuid );
   if ( sIdx == -1 )
   {
      CCTK_TRACEI0( "cfg - cfgRemoveCall: GUID not found." );
      return eCFGSTATUS_BADPARM;
   }

   clrCallCfg( CFM_CFG.pCall[sIdx] );

   return eCFGSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   cfgUpdCallUsr
**
**  PURPOSE:    Update the user of an existing call configuration resource.
**
**  PARAMETERS:
**              uGuid      - GUID.
**              uGuidCurrUser - the call's current user GUID.
**              uGuidNewUser - the call's new user GUID.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS   cfgUpdCallUsr( BOS_UINT32 uGuid, BOS_UINT32 uGuidCurrUser,
                           BOS_UINT32 uGuidNewUser)
{
   CCTK_TRACED3( "cfg - cfgUpdCallUsr(0x%lX), (0x%lX) -> (0x%lX)",
                     uGuid, uGuidCurrUser, uGuidNewUser );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0(
         "cfg - cfgUpdCallUsr: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( ( uGuid == guCeInvalidGuid ) ||
         ( uGuidCurrUser == guCeInvalidGuid ) ||
         ( uGuidNewUser == guCeInvalidGuid) )
   {
      CCTK_TRACEI0( "cfg - cfgUpdCallUsr: Invalid GUID." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 cIdx = getIdx( eCFGTYPE_CALL, uGuid );
   if ( cIdx == -1 )
   {
      CCTK_TRACEI0( "cfg - cfgUpdCallUsr: call GUID not found." );
      return eCFGSTATUS_BADPARM;
   }

   if ( CFM_CFG.pCall[cIdx]->cfg.in.uUsrId != uGuidCurrUser )
   {
      CCTK_TRACEI0( "cfg - cfgUpdCallUsr: user GUID doesn't match." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 nIdx = getIdx( eCFGTYPE_USR, uGuidNewUser );
   if ( nIdx == -1 )
   {
      CCTK_TRACEI0( "cfg - cfgUpdCallUsr: new user GUID not found." );
      return eCFGSTATUS_BADPARM;
   }

   /* All is well, can update call's user
   */
   CFM_CFG.pCall[cIdx]->cfg.in.uUsrId = uGuidNewUser;

   return eCFGSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   cfgAddEvt
**
**  PURPOSE:    Set up and add an event subscription configuration resource.
**
**  PARAMETERS:
**              uGuid      - GUID of the event subscription config resource.
**              uGuidSrc   - Source GUID. Can be either
**                             (1) GUID of the associated user config resource, or
**                             (2) GUID of an existing event subscription.
**                           If (1) is used, the new event config resource is copied
**                           from the default event config settings in the user
**                           config resource specified in "uGuidSrc". The new event
**                           config resource is associated to the specified user
**                           config resource. If (2) is provided, the new event
**                           config resource is copied from the existing event
**                           config resource specified in "uGuidSrc". The new event
**                           config resource is associated to the same user config
**                           resource of the existing event config resource.
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgAddEvt( BOS_UINT32 uGuid, BOS_UINT32 uGuidSrc )
{
   CCTK_TRACED2( "cfg - cfgAddEvt(0x%lX, 0x%lX)", uGuid, uGuidSrc );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgAddEvt: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( uGuid == guCeInvalidGuid || uGuidSrc == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - cfgAddEvt: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   /* Check if the provided GUID is already in use by other resources */
   if ( cfgGetType( uGuid ) != eCFGTYPE_MAX )
   {
      CCTK_TRACEI0( "cfg - cfgAddEvt: Event GUID is already in use." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 sIdx;
   CFGTYPE eGuidType = cfgGetType( uGuidSrc );
   switch ( eGuidType )
   {
   case eCFGTYPE_USR:
   case eCFGTYPE_EVT:
      {
         sIdx = getIdx( eGuidType, uGuidSrc );
         if ( sIdx == -1 )
         {
            CCTK_TRACEI0( "cfg - cfgAddEvt: Invalid source GUID." );
            return eCFGSTATUS_BADPARM;
         }
      }
      break;
   default:
      {
         CCTK_TRACEI1(
            "cfg - cfgAddEvt: Unknown source GUID type (%d).", eGuidType );
         return eCFGSTATUS_BADPARM;
      }
      break;
   }

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_EVT; uIdx++ )
   {
      if ( CFM_CFG.pEvt[uIdx]->uGuid == guCeInvalidGuid )
      {
         /* Unused user resource found */
         break;
      }
   }

   if ( uIdx >= CCTKCFG_MAX_EVT )
   {
      CCTK_TRACED0( "cfg - cfgAddEvt: No event resource available." );
      return eCFGSTATUS_NORSC;
   }

   /* De-allocate any previously alloced memory */
   clrEvtCfg( CFM_CFG.pEvt[uIdx] );

   /* Copy event subscription configurations based on GUID type */
   const CFGEVTXTL *pSrcCfg = ( eGuidType == eCFGTYPE_USR ) ?
                              &( CFM_CFG.pUsr[sIdx]->cfg.xtl.dft.evt ) :
                              &( CFM_CFG.pEvt[sIdx]->cfg.xtl );

   CFGSTATUS eStatus = cpyEvtXtlCfg( pSrcCfg, &( CFM_CFG.pEvt[uIdx]->cfg.xtl ),
                                     BOS_FALSE );
   if ( eStatus != eCFGSTATUS_SUCCESS )
   {
      CCTK_TRACED1( "cfg - cfgAddEvt: Error copying event config structure (%d).",
                    eStatus );
      return eCFGSTATUS_INTERR;
   }

   /* Event config setup successful. Associate GUID. */
   CFM_CFG.pEvt[uIdx]->uGuid = uGuid;
   CFM_CFG.pEvt[uIdx]->cfg.in.uUsrId = ( eGuidType == eCFGTYPE_USR ) ?
                                       uGuidSrc :
                                       CFM_CFG.pEvt[sIdx]->cfg.in.uUsrId;

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cfgRemoveEvt
**
**  PURPOSE:    Remove an existing event subscription config resource.
**
**  PARAMETERS:
**              uGuid - GUID.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgRemoveEvt( BOS_UINT32 uGuid )
{
   CCTK_TRACED1( "cfg - cfgRemoveEvt(0x%lX)", uGuid );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0(
         "cfg - cfgRemoveEvt: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( uGuid == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - cfgRemoveEvt: Invalid GUID." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 sIdx = getIdx( eCFGTYPE_EVT, uGuid );
   if ( sIdx == -1 )
   {
      CCTK_TRACEI0( "cfg - cfgRemoveEvt: GUID not found." );
      return eCFGSTATUS_BADPARM;
   }

   clrEvtCfg( CFM_CFG.pEvt[sIdx] );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cfgSetXtlCfg
**
**  PURPOSE:    Set external configuration (main + extended config).
**
**  PARAMETERS:
**              uGuid   - GUID.
**              eParm   - Parameter ID.
**              rpCfg   - Config value.
**                        Ownership is taken by the function on success.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgSetXtlCfg( BOS_UINT32 uGuid,
                        CCTKPARM eParm,
                        void *&rpCfg )
{
   CCTK_TRACED3( "cfg - cfgSetXtlCfg(0x%lX, %d, %p)", uGuid, eParm, rpCfg );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgSetXtlCfg: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( rpCfg == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgSetXtlCfg: NULL config pointer." );
      return eCFGSTATUS_BADPARM;
   }

   /* Generic parameter value check */
   if ( !isXtlCfgValid( eParm, rpCfg ) )
   {
      CCTK_TRACEI0( "cfg - cfgSetXtlCfg: Invalid external config block." );
      return eCFGSTATUS_BADPARM;
   }

   CFGTYPE eType = cfgGetType( uGuid );
   if ( eType == eCFGTYPE_MAX )
   {
      CCTK_TRACEI1( "cfg - cfgSetXtlCfg: ID is unknown (%u).", (unsigned int)uGuid );
      return eCFGSTATUS_BADPARM;
   }

   /* FIXME: Add parameter checking. */
   CFGSTATUS eRes = eCFGSTATUS_INTERR;
   BOS_UINT32 uIdx = CFG_MAX_USR;
   switch ( eType )
   {
   case eCFGTYPE_SYS:
      {
         /* Set external system config */
         eRes = setSysXtlCfg( eParm, rpCfg );
      }
      break;
   case eCFGTYPE_USR:
      {
         /* User parameter value check */
         if ( !isXtlUsrCfgValid( uGuid, eParm, rpCfg ) )
         {
            CCTK_TRACEI0( "cfg - cfgSetXtlCfg: Invalid external call config"
                          " block." );
            eRes = eCFGSTATUS_BADPARM;
         }
         else
         {
            /* Set external user config */
            uIdx = static_cast< BOS_UINT32 >( getIdx( eCFGTYPE_USR, uGuid ) );
            eRes = setUsrXtlCfg( uIdx , eParm, rpCfg );
         }
      }
      break;
   case eCFGTYPE_CALL:
      {
         /* Set external call config */
         uIdx = static_cast< BOS_UINT32 >( getIdx( eCFGTYPE_CALL, uGuid ) );
         eRes = setCallXtlCfg( uIdx, eParm, rpCfg );
      }
      break;
   case eCFGTYPE_EVT:
      {
         /* Set external event config */
         uIdx = static_cast< BOS_UINT32 >( getIdx( eCFGTYPE_EVT, uGuid ) );
         eRes = setEvtXtlCfg( uIdx, eParm, rpCfg );
      }
      break;
   default:
      {
         CCTK_TRACEI0( "cfg - cfgSetXtlCfg: Unknown GUID." );
         eRes = eCFGSTATUS_BADPARM;
      }
      break;
   }

   /* Re-configure the SCE */
   if ( CFGSTATUS_IS_SUCCESS( eRes ) )
   {
      switch ( eType )
      {
      case eCFGTYPE_SYS:
         {
            eRes = gCfmCtrlBlk.pEngine->modifySys( eParm );
         }
         break;
      case eCFGTYPE_USR:
         {
            eRes = gCfmCtrlBlk.pEngine->modifyUsr( eParm, uIdx );
         }
         break;
      case eCFGTYPE_CALL:
         {
            BOS_SINT32 sIdxUsr =
               getIdx( eCFGTYPE_USR, CFM_CFG.pCall[uIdx]->cfg.in.uUsrId );

            CCTK_ASSERT( sIdxUsr != -1 );
            if ( sIdxUsr != -1 )
            {
               eRes = gCfmCtrlBlk.pEngine->modifyCall( eParm, sIdxUsr, uIdx );
            }
         }
         break;
      case eCFGTYPE_EVT:
         {
            /* No per-event configuration is supported now */
         }
         break;
      default:
         {
            /* Unknown configuration type. Assert should be triggered given type
            ** check should have been done earlier.
            */
            eRes = eCFGSTATUS_BADPARM;
            CCTK_ASSERT( CFGSTATUS_IS_SUCCESS( eRes ) );
         }
         break;
      }

      if ( CFGSTATUS_IS_FAIL( eRes ) )
      {
         CCTK_TRACEI4( "cfg - cfgSetXtlCfg: "
            "Error re-configuring SCE configurations (%d, %d, %lu, %d).",
            eParm,
            eType,
            uIdx,
            eRes );
      }
   }

   return eRes;
}

/***********************************************************************************
**  FUNCTION:   cfgAddRealm
**
**  PURPOSE:    Add a realm to a credential which will be used in user
**              authentication. The default user name and password will be used for
**              configuring the credential.
**
**  PARAMETERS:
**              uGuidUsr   - User GUID.
**              pcRealm    - Realm string.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgAddRealm( BOS_UINT32 uGuidUsr, const BOS_UINT8 *pcRealm )
{
   CCTK_TRACED2( "cfg - cfgAddRealm(0x%lX, %p)", uGuidUsr, pcRealm );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgAddRealm: Config module has not been initialized." );
      return eCFGSTATUS_BADSTATE;
   }

   if ( pcRealm == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgAddRealm: NULL realm pointer." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_SINT32 sIdx = getIdx( eCFGTYPE_USR, uGuidUsr );
   if ( sIdx == -1 )
   {
      CCTK_TRACEI0( "cfg - cfgAddRealm: GUID not found." );
      return eCFGSTATUS_BADPARM;
   }

   /* Add a realm to the user authentication config */
   CFGSTATUS eStatus = gCfmCtrlBlk.pEngine->addRealm( sIdx, pcRealm );
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      CCTK_TRACEI2( "cfg - cfgAddRealm: Error adding a realm (%u, %d).",
                    (unsigned int)uGuidUsr,
                    eStatus );
   }

   return eStatus;
}



/***********************************************************************************
**  FUNCTION:   cfgGet
**
**  PURPOSE:    Retrieve the pointer of a configuration block.
**
**  PARAMETERS:
**              eType   - Configuration type.
**              uGuid   - GUID.
**
**
**  RETURNS:    Return the pointer of the configuration block. NULL if
**              configuration block not found.
**
**  NOTES:      None.
***********************************************************************************/
const void *cfgGet( CFGTYPE eType, BOS_UINT32 uGuid )
{
   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgGet: Config module has not been initialized." );
      return NULL;
   }

   BOS_SINT32 sIdx = -1;
   const void *pRes = NULL;

   switch ( eType )
   {
   case eCFGTYPE_SYS:
      {
         pRes = CFM_CFG.pSys;
      }
      break;
   case eCFGTYPE_USR:
      {
         sIdx = getIdx( eType, uGuid );
         if ( sIdx != -1 && sIdx < CFG_MAX_USR )
         {
            pRes = &( CFM_CFG.pUsr[sIdx]->cfg );
         }
      }
      break;
   case eCFGTYPE_CALL:
      {
         sIdx = getIdx( eType, uGuid );
         if ( sIdx != -1 && sIdx < CCTKCFG_MAX_CALL )
         {
            pRes = &( CFM_CFG.pCall[sIdx]->cfg );
         }
      }
      break;
   case eCFGTYPE_EVT:
      {
         sIdx = getIdx( eType, uGuid );
         if ( sIdx != -1 && sIdx < CCTKCFG_MAX_EVT )
         {
            pRes = &( CFM_CFG.pEvt[sIdx]->cfg );
         }
      }
      break;
   default:
      {
         CCTK_TRACEI1( "cfg - cfgGet: Unknown config type (%d).", eType );
      }
      break;
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgGetType
**
**  PURPOSE:    Retrieve the config type of the input GUID.
**
**  PARAMETERS:
**              uGuid   - GUID.
**
**
**  RETURNS:    Return the config type of the GUID.
**              eCFGTYPE_MAX if GUID is unknown or other failures occur.
**
**  NOTES:      None.
***********************************************************************************/
CFGTYPE cfgGetType( BOS_UINT32 uGuid )
{
   CCTK_TRACED1( "cfg - cfgGetType(0x%lX)", uGuid );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgGetType: Config module has not been initialized." );
      return eCFGTYPE_MAX;
   }

   if ( uGuid == guCeInvalidGuid )
   {
      return eCFGTYPE_SYS;
   }

   /* Internal check for the for-loop below */
   CFGTYPE eType = eCFGTYPE_SYS;
   CCTK_ASSERT( eType == 0 );

   /* Loop through all config types. Return the GUID itself if it is a user ID */
   BOS_SINT32 sType;
   for ( sType = eCFGTYPE_MAX - 1; sType > eCFGTYPE_SYS; sType-- )
   {
      if ( getIdx( static_cast< CFGTYPE >( sType ), uGuid ) != -1 )
      {
         return static_cast< CFGTYPE >( sType );
      }
   }

   /* Unknown GUID */
   return eCFGTYPE_MAX;
}

/***********************************************************************************
**  FUNCTION:   cfgGetUsrId
**
**  PURPOSE:    Retrieve the associated user GUID to the input GUID.
**
**  PARAMETERS:
**              uGuid   - GUID.
**
**
**  RETURNS:    Return the associated user GUID.
**              guCeInvalidGuid if no user is associated with the input GUID.
**
**  NOTES:      None.
***********************************************************************************/
BOS_UINT32 cfgGetUsrId( BOS_UINT32 uGuid )
{
   CCTK_TRACED1( "cfg - cfgGetUsrId(0x%lX)", uGuid );

   CCTK_ASSERT( gCfmCtrlBlk.pEngine != NULL );
   if ( gCfmCtrlBlk.pEngine == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgGetUsrId: Config module has not been initialized." );
      return guCeInvalidGuid;
   }

   if ( uGuid == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - cfgGetUsrId: Bad identifier." );
      return guCeInvalidGuid;
   }

   BOS_UINT32 uRes = guCeInvalidGuid;
   CFGTYPE eType = cfgGetType( uGuid );
   switch ( eType )
   {
   case eCFGTYPE_CALL:
   case eCFGTYPE_EVT:
      {
         BOS_SINT32 sIdx = getIdx( eType, uGuid );
         CCTK_ASSERT( sIdx != -1 );
         if ( sIdx != -1 )
         {
            uRes = ( eType == eCFGTYPE_CALL ) ?
                   CFM_CFG.pCall[sIdx]->cfg.in.uUsrId :
                   CFM_CFG.pEvt[sIdx]->cfg.in.uUsrId;
         }
      }
      break;
   case eCFGTYPE_USR:
      {
         /* Return the GUID itself if it is a user ID */
         uRes = uGuid;
      }
      break;
   default:
      {
         /* We cannot find any matching ID here */
         CCTK_TRACEI1( "cfg - cfgGetUsrId: GUID not found (0x%lX).", uGuid );
      }
      break;
   }

   return uRes;
}

/***********************************************************************************
**  FUNCTION:   cfgIsAddrValid
**
**  PURPOSE:    Check if the input address is valid.
**
**  PARAMETERS:
**              pcAddr  - Address to be checked against.
**
**
**  RETURNS:    False if the address is set to NULL or it is set to "0.0.0.0".
**
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsAddrValid( const BOS_UINT8 *pcAddr )
{
   CCTK_TRACED1( "cfg - cfgIsAddrValid(%p)", pcAddr );

   return ( pcAddr != NULL &&
            pcAddr[0] != '\0' &&
            strstr( reinterpret_cast< const char* >( pcAddr ),
                    ":0.0.0.0" ) == NULL );
}

/***********************************************************************************
**  FUNCTION:   cfgIsHdrValid
**
**  PURPOSE:    Check if the input header is valid.
**
**  PARAMETERS:
**              rHdr  - Header to be checked against.
**
**
**  RETURNS:    False if the header contains NULL pointers or empty strings.
**
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsHdrValid( const CCTKHDR &rHdr )
{
   CCTK_TRACED1( "cfg - cfgIsHdrValid(%p)", &rHdr );

   return ( rHdr.pcValue != NULL && rHdr.pcValue[0] != '\0' &&
            ( rHdr.eType != eCCTKHDRTYPE_PRIVATE ||
              ( rHdr.pcName != NULL && rHdr.pcName[0] != '\0' )
            )
          );
}

/***********************************************************************************
**  FUNCTION:   cfgIsHostPortEnabled
**
**  PURPOSE:    Check if the host-port structure is enabled.
**
**  PARAMETERS:
**              rHostPort  - Host-port structure to be checked against.
**
**
**  RETURNS:    False if the address of host-port is set to NULL or it is set to
**              "0.0.0.0". True, otherwise.
**
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsHostPortEnabled( const CCTKHOSTPORT &rHostPort )
{
   CCTK_TRACED1( "cfg - cfgIsHostPortEnabled(%p)", &rHostPort );

   return ( rHostPort.pcAddr != NULL &&
            rHostPort.pcAddr[0] != '\0' &&
            strcmp( reinterpret_cast< const char* >( rHostPort.pcAddr ),
                    "0.0.0.0" ) != 0 );
}

/***********************************************************************************
**  FUNCTION:   cfgIsHostPortIpv6
**
**  PURPOSE:    Check if the input host-port has an IPv6 address.
**
**  PARAMETERS:
**              rHostPort - Host-port to be checked against.
**
**
**  RETURNS:    True if input host-port has an IPv6 address.
**
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsHostPortIpv6( const CCTKHOSTPORT &rHostPort )
{
   CCTK_TRACED1( "cfg - cfgIsHostPortIpv6(%p)", &rHostPort );

   return ( cfgIsHostPortEnabled( rHostPort ) &&
            strchr( reinterpret_cast< const char* >( rHostPort.pcAddr ),
                    ':' ) != 0 );
}

/***********************************************************************************
**  FUNCTION:   cfgIsNaiEnabled
**
**  PURPOSE:    Check if network-asserted identity support is enabled.
**
**  PARAMETERS:
**              rNaiCfg - Network-asserted identity configuration.
**
**
**  RETURNS:    True if network-asserted identity support is enabled.
**
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsNaiEnabled( const CCTKNAICFG &rNaiCfg )
{
   CCTK_TRACED1( "cfg - cfgIsNaiEnabled(%p)", &rNaiCfg );

   return rNaiCfg.cli.eMode != eCCTKNAICLTMODE_NET ||
          rNaiCfg.cli.pPreferredId != NULL ||
          rNaiCfg.svr.eMode != eCCTKNAISVRMODE_IGNORED;
}

/***********************************************************************************
**  FUNCTION:   cfgIsSipHostEnabled
**
**  PURPOSE:    Check if the SIP host-port structure is enabled.
**
**  PARAMETERS:
**              rSipHost   - SIP host-port structure to be checked against.
**
**
**  RETURNS:    False if the address of host-port is set to NULL or it is set to
**              "0.0.0.0". True, otherwise.
**
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsSipHostEnabled( const CCTKSIPHOST &rSipHost )
{
   CCTK_TRACED1( "cfg - cfgIsSipHostEnabled(%p)", &rSipHost );

   return cfgIsHostPortEnabled( rSipHost.hostPort );
}

/***********************************************************************************
**  FUNCTION:   cfgIsSipHostIpv6
**
**  PURPOSE:    Check if the input SIP host-port has an IPv6 address.
**
**  PARAMETERS:
**              rSipHost   - SIP host-port to be checked against.
**
**
**  RETURNS:    True if input SIP host-port has an IPv6 address.
**
**  NOTES:      None.
***********************************************************************************/
BOS_BOOL cfgIsSipHostIpv6( const CCTKSIPHOST &rSipHost )
{
   CCTK_TRACED1( "cfg - cfgIsSipHostIpv6(%p)", &rSipHost );

   return cfgIsHostPortIpv6( rSipHost.hostPort );
}

/***********************************************************************************
**  FUNCTION:   cfmClrUsrCfg
**
**  PURPOSE:    Clear user configuration block and de-allocate its content.
**
**  PARAMETERS:
**              uUsrIdx - User index.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void cfmClrUsrCfg( BOS_UINT32 uUsrIdx )
{
   clrUsrCfg( CFM_CFG.pUsr[uUsrIdx] );
}

/***********************************************************************************
**  FUNCTION:   clrMediaCfg
**
**  PURPOSE:    Clear media config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Config block to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrMediaCfg( CCTKMEDCFG *pCfg )
{
   CCTK_TRACED1( "cfg - clrMediaCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      BRCM_DELETE_ARRAY( pCfg->pcSesName );
      BRCM_DELETE( pCfg->pAudio );
#if CCTKCFG_VIDEO
      delVideoCfg( pCfg->pVideo );
#endif
      BRCM_DELETE( pCfg->pFax );
      BRCM_DELETE( pCfg->pMediaLst );

      memset( pCfg, 0, sizeof( CCTKMEDCFG ) );
   }
}

/***********************************************************************************
**  FUNCTION:   clrRoute
**
**  PURPOSE:    Clear routing information and de-allocate its content.
**
**  PARAMETERS:
**              route - Routing information to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrRoute( CCTKROUTE route[] )
{
   CCTK_TRACED1( "cfg - clrRoute(%p)", route );

   if ( route != NULL )
   {
      BOS_UINT32 uIdx;

      for ( uIdx = 0; uIdx < CCTKCFG_MAX_OBPROXY; uIdx++ )
      {
         clrSipHost( &( route[uIdx].addr ) );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   clrRouteLst
**
**  PURPOSE:    Clear route list and de-allocate its content.
**
**  PARAMETERS:
**              pLst - Route list to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrRouteLst( CCTKROUTELST *pLst )
{
   CCTK_TRACED1( "cfg - clrRouteLst(%p)", pLst );

   if ( pLst != NULL )
   {
      clrRoute( pLst->route );
      memset( pLst, 0, sizeof( CCTKROUTELST ) );
   }
}

/***********************************************************************************
**  FUNCTION:   clrSvcRoute
**
**  PURPOSE:    Clear service-Route configuration and de-allocate its content.
**
**  PARAMETERS:
**              pRoute - Service-Route configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrSvcRoute( CCTKSVCROUTE *pRoute )
{
   CCTK_TRACED1( "cfg - clrSvcRoute(%p)", pRoute );

   if ( pRoute != NULL )
   {
      clrRoute( pRoute->route );
      memset( pRoute, 0, sizeof( CCTKSVCROUTE ) );
   }
}

/***********************************************************************************
**  FUNCTION:   clrSysCfg
**
**  PURPOSE:    Clear system configuration block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - System configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrSysCfg( CFGSYS *pCfg )
{
   CCTK_TRACED1( "cfg - clrSysCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      /* Clear external system configuration */
      clrSysXtlCfg( &( CFM_CFG.pSys->xtl ), BOS_FALSE );

      /* Clear internal system configuration */
      clrSysIntCfg( &( CFM_CFG.pSys->in ) );

      memset( pCfg, 0, sizeof( CFGSYS ) );
   }
}

/***********************************************************************************
**  FUNCTION:   clrSysXtlCfg
**
**  PURPOSE:    Clear external system config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg       - External system configuration to be cleared.
**              bMainOnly  - True to clear main config only.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrSysXtlCfg( CFGSYSXTL *pCfg, BOS_BOOL bMainOnly )
{
   CCTK_TRACED2( "cfg - clrSysXtlCfg(%p, %d)", pCfg, bMainOnly );

   if ( pCfg != NULL )
   {
      /* Clear main system configuration */
      BOS_UINT32 uIdx;

      BRCM_DELETE_ARRAY( pCfg->loc.gen.pcSeed );
      for ( uIdx = 0; uIdx < pCfg->net.gen.host.uNum; uIdx++ )
      {
         BRCM_DELETE_ARRAY( pCfg->net.gen.host.addr[uIdx].pcFqdn );
      }
      clrEmgAddr( &( pCfg->net.prot.gen.emgAddr ) );

      memset( &( pCfg->loc.gen ), 0, sizeof( CCTKLOCGEN ) );
      memset( &( pCfg->net.gen ), 0, sizeof( CCTKNETGEN ) );
      memset( &( pCfg->net.prot.gen ), 0, sizeof( CCTKNETPROTGEN ) );

      /* Clear extended system configuration */
      if ( !bMainOnly )
      {
#if CCTKCFG_TLS
         clrTlsCfg( &( pCfg->net.gen.tls ) );
#endif
         clrUaIdHdr( &( pCfg->net.prot.gen.uaIdHdr ) );

         clrAccessNetworkInfoCfg( &( pCfg->net.prot.gen.pani ) );
#if CCTKCFG_REMOTE_USER_CONTROL
         clrRemUsrSysCfg ( &(pCfg->remUsr.gen) );
#endif

         memset( pCfg, 0, sizeof( CFGSYSXTL ) );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   clrSysIntCfg
**
**  PURPOSE:    Clear internal system config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Internal system configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrSysIntCfg( CFGSYSINT *pCfg )
{
   CCTK_TRACED1( "cfg - clrSysIntCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      memset( pCfg, 0, sizeof( CFGSYSINT ) );
   }
}

/***********************************************************************************
**  FUNCTION:   clrUsrCfg
**
**  PURPOSE:    Clear user configuration block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - User configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrUsrCfg( CFMUSR *pCfg )
{
   CCTK_TRACED1( "cfg - clrUsrCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      /* Clear external user configuration */
      clrUsrXtlCfg( &( pCfg->cfg.xtl ), BOS_FALSE );

      /* Clear internal user configuration */
      clrUsrIntCfg( &( pCfg->cfg.in ) );

      memset( pCfg, 0, sizeof( CFMUSR ) );
      pCfg->uGuid = guCeInvalidGuid;
   }
}

/***********************************************************************************
**  FUNCTION:   clrUsrXtlCfg
**
**  PURPOSE:    Clear external user config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg       - User configuration which contains the external user
**                           config to be cleared.
**              bMainOnly  - True to clear main config only.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrUsrXtlCfg( CFGUSRXTL *pCfg, BOS_BOOL bMainOnly )
{
   CCTK_TRACED2( "cfg - clrUsrXtlCfg(%p, %d)", pCfg, bMainOnly );

   if ( pCfg != NULL )
   {
      /* Clear main user configuration */
      clrSipUsrHost( &( pCfg->mwi.gen.server ) );
      clrHostPort( &( pCfg->reg.gen.aor ) );
      clrSipHost( &( pCfg->reg.gen.registrar ) );
      clrRouteLst( &( pCfg->usr.net.obProxyLst ) );
      clrSipName( &( pCfg->usr.net.sipName ) );
      clrAuthcInfo( &( pCfg->usr.sec.auth.client ) );
      clrSipHost( &( pCfg->usr.sec.nat.kaCfg.server) );

      memset( &( pCfg->mwi.gen ), 0, sizeof( CCTKMWIGEN ) );
      memset( &( pCfg->reg.gen ), 0, sizeof( CCTKREGGEN ) );
      memset( &( pCfg->usr.net ), 0, sizeof( CCTKUSRNET ) );
      memset( &( pCfg->usr.sec.auth ), 0, sizeof( CCTKAUTH ) );
      memset( &( pCfg->usr.sec.nat ), 0, sizeof( CCTKNAT ) );

      /* Clear external user default configuration */
      clrCallXtlCfg( &( pCfg->dft.call ), BOS_TRUE, bMainOnly );
      clrEvtXtlCfg( &( pCfg->dft.evt ), BOS_TRUE, bMainOnly );

#if CCTKCFG_REMOTE_USER_CONTROL
      clrRemUsrXtlCfg( &( pCfg->remUsr ), bMainOnly );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

      /* Clear extended user configuration */
      if ( !bMainOnly )
      {
         clrGenHdr( &( pCfg->usr.loc.genHdr ) );
         clrDiagEvtCfg( &( pCfg->usr.net.diagEvt ) );
         clrUapEvtCfg( &( pCfg->usr.net.uapEvt ) );
         clrSvcRoute( &( pCfg->usr.net.svcRoute ) );
         clrContact( &( pCfg->usr.net.contact ) );
         memset( pCfg, 0, sizeof( CFGUSRXTL ) );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   clrUsrIntCfg
**
**  PURPOSE:    Clear internal user config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Internal user configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrUsrIntCfg( CFGUSRINT *pCfg )
{
   CCTK_TRACED1( "cfg - clrUsrIntCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      clrHdr( &( pCfg->cachedAuth ) );
      if ( pCfg->pSceUsrCfg != NULL )
      {
         pCfg->pSceUsrCfg->ReleaseIfRef();
      }

      memset( pCfg, 0, sizeof( CFGUSRINT ) );
   }
}

/***********************************************************************************
**  FUNCTION:   clrCallCfg
**
**  PURPOSE:    Clear call configuration block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Call configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrCallCfg( CFMCALL *pCfg )
{
   CCTK_TRACED1( "cfg - clrCallCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      /* Clear external call configuration */
      clrCallXtlCfg( &( pCfg->cfg.xtl ), BOS_FALSE, BOS_FALSE );

      /* Clear internal call configuration */
      clrCallIntCfg( &( pCfg->cfg.in ) );

      memset( pCfg, 0, sizeof( CFMCALL ) );
      pCfg->uGuid = guCeInvalidGuid;
   }
}

/***********************************************************************************
**  FUNCTION:   clrCallXtlCfg
**
**  PURPOSE:    Clear external (main + extended) call config block and
**              de-allocate its content.
**
**  PARAMETERS:
**              pCfg       - External call configuration to be cleared.
**              bDftCfg    - True to indicate pCfg is a user default config.
**              bMainOnly  - True to clear the main config only.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrCallXtlCfg( CFGCALLXTL *pCfg, BOS_BOOL bDftCfg, BOS_BOOL bMainOnly )
{
   CCTK_TRACED3( "cfg - clrCallXtlCfg(%p, %d, %d)", pCfg, bDftCfg, bMainOnly );

   if ( pCfg != NULL )
   {
      /* Clear main call configuration */
      clrSipHost( &( pCfg->supp.mohServer ) );
      BRCM_DELETE_ARRAY( pCfg->media.pcSesName );
      BRCM_DELETE( pCfg->media.pAudio );
#if CCTKCFG_VIDEO
      delVideoCfg( pCfg->media.pVideo );
      BRCM_DELETE( pCfg->media.pVideo );
#endif
      BRCM_DELETE( pCfg->media.pFax );
      BRCM_DELETE( pCfg->media.pMediaLst );

      memset( &( pCfg->basic.gen ), 0, sizeof( CCTKCALLBASGEN ) );
      memset( &( pCfg->supp ), 0, sizeof( CCTKCALLSUPP ) );
      memset( &( pCfg->media ), 0, sizeof( CCTKMEDCFG ) );

      /* Clear extended call configuration */
      if ( !bMainOnly )
      {
         /* Delete user-specific config */
         if ( !bDftCfg )
         {
            delGenHdr( pCfg->usr.pGenHdr );
            delRouteLst( pCfg->usr.pObProxyLst );
         }

         /* Clear the rest of the config */
         delSipUsrHost( pCfg->basic.adv.nai.cli.pPreferredId );

         memset( pCfg, 0, sizeof( CFGCALLXTL ) );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   clrCallIntCfg
**
**  PURPOSE:    Clear internal call config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg       - Internal call configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrCallIntCfg( CFGCALLINT *pCfg )
{
   CCTK_TRACED1( "cfg - clrCallIntCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      memset( pCfg, 0, sizeof( CFGCALLINT ) );
   }
}

/***********************************************************************************
**  FUNCTION:   clrEvtCfg
**
**  PURPOSE:    Clear event subscription config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Event subscription configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrEvtCfg( CFMEVT *pCfg )
{
   CCTK_TRACED1( "cfg - clrEvtCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      /* Clear external configuration */
      clrEvtXtlCfg( &( pCfg->cfg.xtl ), BOS_FALSE, BOS_FALSE );

      /* Clear internal configuration */
      clrEvtIntCfg( &( pCfg->cfg.in ) );

      memset( pCfg, 0, sizeof( CFMEVT ) );
      pCfg->uGuid = guCeInvalidGuid;
   }
}

/***********************************************************************************
**  FUNCTION:   clrEvtXtlCfg
**
**  PURPOSE:    Clear external (main + extended) event subscription config block
**              and de-allocate its content.
**
**  PARAMETERS:
**              pCfg       - External event subscription config to be cleared.
**              bDftCfg    - True to indicate a user default config.
**              bMainOnly  - True to clear main configuration only.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrEvtXtlCfg( CFGEVTXTL *pCfg, BOS_BOOL bDftCfg, BOS_BOOL bMainOnly )
{
   CCTK_TRACED3( "cfg - clrEvtXtlCfg(%p, %d, %d)", pCfg, bDftCfg, bMainOnly );

   if ( pCfg != NULL )
   {
      /* Clear main configuration */
      memset( &( pCfg->gen ), 0, sizeof( CCTKEVTGEN ) );

      /* Clear extended configuration */
      if ( !bMainOnly )
      {
         /* Delete user-specific config */
         if ( !bDftCfg )
         {
            delGenHdr( pCfg->usr.pGenHdr );
            delRouteLst( pCfg->usr.pObProxyLst );
         }

         /* Clear the rest of the config */
         delSipUsrHost( pCfg->adv.nai.cli.pPreferredId );

         memset( pCfg, 0, sizeof( CFGEVTXTL ) );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   clrEvtIntCfg
**
**  PURPOSE:    Clear internal event subscription config block and
**              de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Internal event subscription configuration to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrEvtIntCfg( CFGEVTINT *pCfg )
{
   CCTK_TRACED1( "cfg - clrEvtIntCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      memset( pCfg, 0, sizeof( CFGEVTINT ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyUsrXtlCfg
**
**  PURPOSE:    Copy external (main + extended) user configuration block.
**
**  PARAMETERS:
**              pSrc - External user config block to be copied.
**              pDst - Copied external user config block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Some parameters in the user config block may be dynamically
**              allocated when copying. clrUsrXtlCfg() must be called when
**              the copied external user config block is not used.
***********************************************************************************/
static CFGSTATUS cpyUsrXtlCfg( const CFGUSRXTL *pSrc, CFGUSRXTL *pDst )
{
   CCTK_TRACED2( "cfg - cpyUsrXtlCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyUsrXtlCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   cpyCallXtlCfg( &( pSrc->dft.call ), &( pDst->dft.call ), BOS_TRUE );
   cpyEvtXtlCfg( &( pSrc->dft.evt ), &( pDst->dft.evt ), BOS_TRUE );
   cpySipUsrHost( &( pSrc->mwi.gen.server ), &( pDst->mwi.gen.server ) );
   cpyHostPort( &( pSrc->reg.gen.aor ), &( pDst->reg.gen.aor ) );
   cpySipHost( &( pSrc->reg.gen.registrar ), &( pDst->reg.gen.registrar ) );
   cpyGenHdr( &( pSrc->usr.loc.genHdr ), &( pDst->usr.loc.genHdr ) );
   cpySvcRoute( & (pSrc->usr.net.svcRoute ), &( pDst->usr.net.svcRoute ) );
   cpyContact( &( pSrc->usr.net.contact ), &( pDst->usr.net.contact ) );
   cpyDiagEvtCfg( &( pSrc->usr.net.diagEvt ), &( pDst->usr.net.diagEvt ) );
   cpyRouteLst( &( pSrc->usr.net.obProxyLst ), &( pDst->usr.net.obProxyLst ) );
   cpySipName( &( pSrc->usr.net.sipName ), &( pDst->usr.net.sipName ) );
   cpyUapEvtCfg( &( pSrc->usr.net.uapEvt ), &( pDst->usr.net.uapEvt ) );
   cpyAuthcInfo( &( pSrc->usr.sec.auth.client ), &( pDst->usr.sec.auth.client ) );
   cpySipHost( &( pSrc->usr.sec.nat.kaCfg.server ),
               &( pDst->usr.sec.nat.kaCfg.server ) );

#if CCTKCFG_REMOTE_USER_CONTROL
   cpyRemUsrXtlCfg ( &( pSrc->remUsr ), &( pDst->remUsr ) );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

   /* Set user-specific default call and event configurations to point to the
   ** ones in the user configuration.
   */
   pDst->dft.call.usr.pGenHdr = &( pDst->usr.loc.genHdr );
   pDst->dft.evt.usr.pGenHdr = &( pDst->usr.loc.genHdr );
   pDst->dft.call.usr.pObProxyLst = &( pDst->usr.net.obProxyLst );
   pDst->dft.evt.usr.pObProxyLst = &( pDst->usr.net.obProxyLst );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cpyCallXtlCfg
**
**  PURPOSE:    Copy external (main + extended) call configuration block.
**
**  PARAMETERS:
**              pSrc - External call config block to be copied.
**              pDst - Copied external call config block.
**              bDftCfg - True to indicate if the cfg being copied is a default
**                        config
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Some parameters in the call config block may be dynamically
**              allocated when copying. clrCallXtlCfg() must be called when
**              the copied external call config block is not used.
***********************************************************************************/
static CFGSTATUS cpyCallXtlCfg( const CFGCALLXTL *pSrc, CFGCALLXTL *pDst,
                                BOS_BOOL bDftCfg )
{
   CCTK_TRACED2( "cfg - cpyCallXtlCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyCallXtlCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->basic.adv.nai.cli.pPreferredId =
      dupSipUsrHost( pSrc->basic.adv.nai.cli.pPreferredId );
   pDst->media.pcSesName = utilStrDup( pSrc->media.pcSesName );
   CFM_VARDUP( pSrc->media.pAudio, pDst->media.pAudio, CCTKMEDAUD );
#if CCTKCFG_VIDEO
   pDst->media.pVideo = dupVideoCfg( pSrc->media.pVideo );
#endif
   CFM_VARDUP( pSrc->media.pFax, pDst->media.pFax, CCTKMEDFAX );
   CFM_VARDUP( pSrc->media.pMediaLst, pDst->media.pMediaLst, CCTKMEDLST );
   cpySipHost( &( pSrc->supp.mohServer ), &( pDst->supp.mohServer ) );

   /* Only required to create new storage for user-specific call config.
   ** No storage is required for default user confg
   */
   if ( !bDftCfg )
   {
      pDst->usr.pGenHdr = dupGenHdr( pSrc->usr.pGenHdr );
      pDst->usr.pObProxyLst = dupRouteLst( pSrc->usr.pObProxyLst );
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   clrNaiCfg
**
**  PURPOSE:    Clear network asserted identity config and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Network asserted identity config to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrNaiCfg( CCTKNAICFG *pCfg )
{
   CCTK_TRACED1( "cfg - clrNaiCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      delSipUsrHost( pCfg->cli.pPreferredId );

      memset( pCfg, 0, sizeof( CCTKNAICFG ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyNaiCfg
**
**  PURPOSE:    Copy network asserted identity configuration.
**
**  PARAMETERS:
**              pSrc - Network asserted identity config to be copied.
**              pDst - Copied network asserted identity config.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the network asserted identity config may be
**              dynamically allocated when copying. clrNaiCfg() must be called when
**              the copied network asserted identity config is not used.
***********************************************************************************/
static CFGSTATUS cpyNaiCfg( const CCTKNAICFG *pSrc, CCTKNAICFG *pDst )
{
   CCTK_TRACED2( "cfg - cpyNaiCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyNaiCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->cli.pPreferredId = dupSipUsrHost( pSrc->cli.pPreferredId );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   delNaiCfg
**
**  PURPOSE:    Delete network asserted identity config and de-allocate its content.
**
**  PARAMETERS:
**              rpCfg - Network asserted identity config to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delNaiCfg( CCTKNAICFG *&rpCfg )
{
   CCTK_TRACED1( "cfg - delNaiCfg(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      clrNaiCfg( rpCfg );

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupNaiCfg
**
**  PURPOSE:    Duplicate network asserted identity config.
**
**  PARAMETERS:
**              pSrc - Network asserted identity config to be duplicated.
**
**
**  RETURNS:    Return the duplicated network asserted identity config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the network asserted identity config may be
**              dynamically allocated when duplicating. delNaiCfg() must be called
**              when the duplicated network asserted identity config is not used.
***********************************************************************************/
static CCTKNAICFG *dupNaiCfg( const CCTKNAICFG *pSrc )
{
   CCTKNAICFG *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKNAICFG );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpyNaiCfg( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0(
               "cfg - dupNaiCfg: Error copying network asserted identity config." );
            delNaiCfg( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupNaiCfg(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

#if CCTKCFG_TLS
/***********************************************************************************
**  FUNCTION:   clrTlsCfg
**
**  PURPOSE:    Clear TLS config block and de-allocate its content.
**
**  PARAMETERS:
**              pTlsCfg - TLS config block to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrTlsCfg( CCTKTLSCFG *pTlsCfg )
{
   CCTK_TRACED1( "cfg - clrTlsCfg(%p)", pTlsCfg );

   if ( pTlsCfg != NULL )
   {
      BRCM_DELETE_ARRAY( pTlsCfg->pcCipher );
      BRCM_DELETE_ARRAY( pTlsCfg->pcPrivKey );
      utilStrArrayDel( pTlsCfg->ppcCertChain );
      utilStrArrayDel( pTlsCfg->ppcTrustedCerts );
      BRCM_DELETE_ARRAY( pTlsCfg->preGenDhPrime.puPrime );

      memset( pTlsCfg, 0, sizeof( CCTKTLSCFG ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyTlsCfg
**
**  PURPOSE:    Copy TLS configuration.
**
**  PARAMETERS:
**              pSrc - TLS configuration to be copied.
**              pDst - Copied TLS configuration.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the TLS config may be dynamically allocated
**              when copying. clrTlsCfg() must be called when the copied
**              TLS config is not used.
***********************************************************************************/
static CFGSTATUS cpyTlsCfg( const CCTKTLSCFG *pSrc, CCTKTLSCFG *pDst )
{
   CCTK_TRACED2( "cfg - cpyTlsCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyTlsCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcCipher = utilStrDup( pSrc->pcCipher );
   pDst->pcPrivKey = utilStrDup( pSrc->pcPrivKey );
   pDst->ppcCertChain =
      utilStrArrayDup( const_cast< const BOS_UINT8 ** >( pSrc->ppcCertChain ) );
   pDst->ppcTrustedCerts =
      utilStrArrayDup( const_cast< const BOS_UINT8 ** >( pSrc->ppcTrustedCerts ) );
   pDst->preGenDhPrime.puPrime =
      utilDataDup( pSrc->preGenDhPrime.puPrime,
                  pSrc->preGenDhPrime.uLen );

   return eCFGSTATUS_SUCCESS;
}
#endif /* #if CCTKCFG_TLS */

/***********************************************************************************
**  FUNCTION:   clrUaIdHdr
**
**  PURPOSE:    Clear UA ID header config block and de-allocate its content.
**
**  PARAMETERS:
**              pHdr - UA ID header config block to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrUaIdHdr( CCTKUAIDHDR *pHdr )
{
   CCTK_TRACED1( "cfg - clrUaIdHdr(%p)", pHdr );

   if ( pHdr != NULL )
   {
      BRCM_DELETE_ARRAY( pHdr->pcSipInstId );
      BRCM_DELETE_ARRAY( pHdr->pcSvrHdr );
      BRCM_DELETE_ARRAY( pHdr->pcUaHdr );

      memset( pHdr, 0, sizeof( CCTKUAIDHDR ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyUaIdHdr
**
**  PURPOSE:    Copy UA ID header config.
**
**  PARAMETERS:
**              pSrc - UA ID header config to be copied.
**              pDst - Copied UA ID header config.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the UA ID header config may be dynamically allocated
**              when copying. clrUaIdHdr() must be called when the copied
**              UA ID header config is not used.
***********************************************************************************/
static CFGSTATUS cpyUaIdHdr( const CCTKUAIDHDR *pSrc, CCTKUAIDHDR *pDst )
{
   CCTK_TRACED2( "cfg - cpyUaIdHdr(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyUaIdHdr: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcSipInstId = utilStrDup( pSrc->pcSipInstId );
   pDst->pcSvrHdr = utilStrDup( pSrc->pcSvrHdr );
   pDst->pcUaHdr = utilStrDup( pSrc->pcUaHdr );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   clrAccessNetworkInfoCfg
**
**  PURPOSE:    Clear P-Access-Network-Info config block and de-allocate its content.
**
**  PARAMETERS:
**              pHdr - P-Access-Network-Info config block to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrAccessNetworkInfoCfg( CCTKPANICFG *pCfg )
{
   CCTK_TRACED1( "cfg - clrAccessNetworkInfoCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      BRCM_DELETE_ARRAY( pCfg->pcNetworkInfo );

      memset( pCfg, 0, sizeof( CCTKPANICFG ) );
   }
}

/***********************************************************************************
**  FUNCTION:   dupAccessNetworkInfoCfg
**
**  PURPOSE:    Duplicate P-Access-Network-Info
**
**  PARAMETERS:
**              pSrc - P-Access-Network-Info config to be duplicated.
**
**
**  RETURNS:    Return the duplicated P-Access-Network-Info config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the P-Access-Network-Info config block may be dynamically
**              allocated when duplicating. clrAccessNetworkInfoCfg() must be called
**              when the duplicated P-Access-Network-Info config is not used.
***********************************************************************************/
static CCTKPANICFG *dupAccessNetworkInfoCfg( const CCTKPANICFG *pSrc )
{
   CCTKPANICFG *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKPANICFG );

      if ( pRes != NULL )
      {
         *pRes = *pSrc;

         /* Copy the ones that require dynamic memory allocation */
         pRes->pcNetworkInfo = utilStrDup( pSrc->pcNetworkInfo );
      }
   }

   CCTK_TRACED2( "cfg - dupAccessNetworkInfoCfg(%p) returns (%p)", pSrc, pRes );

   return pRes;
}


/***********************************************************************************
**  FUNCTION:   cpyEvtXtlCfg
**
**  PURPOSE:    Copy external (main + extended) event subscription config block.
**
**  PARAMETERS:
**              pSrc - External event subscription config block to be copied.
**              pDst - Copied external event subscription config block.
**              bDftCfg - True to indicate if the cfg being copied is a default
**                        config
**
**  RETURNS:    Return status.
**
**  NOTES:      Some parameters in the event subscription config block may be
**              dynamically allocated when copying. clrEvtXtlCfg() must be
**              called when the copied external call config block is not used.
***********************************************************************************/
static CFGSTATUS cpyEvtXtlCfg( const CFGEVTXTL *pSrc, CFGEVTXTL *pDst,
                               BOS_BOOL bDftCfg )
{
   CCTK_TRACED2( "cfg - cpyEvtXtlCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyEvtXtlCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->adv.nai.cli.pPreferredId = dupSipUsrHost( pSrc->adv.nai.cli.pPreferredId );

   /* Only required to create new storage for user-specific event config.
   ** No storage is required for default user confg
   */
   if ( !bDftCfg )
   {
      pDst->usr.pGenHdr = dupGenHdr( pSrc->usr.pGenHdr );
      pDst->usr.pObProxyLst = dupRouteLst( pSrc->usr.pObProxyLst );
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   clrAuthcInfo
**
**  PURPOSE:    Clear client authentication info and de-allocate its content.
**
**  PARAMETERS:
**              pInfo   - Client authentication info to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrAuthcInfo( CCTKAUTHCINFO *pInfo )
{
   CCTK_TRACED1( "cfg - clrAuthcInfo(%p)", pInfo );

   if ( pInfo != NULL )
   {
      BOS_UINT32 uIdx;

      BRCM_DELETE_ARRAY( pInfo->pcAuthName );
      BRCM_DELETE_ARRAY( pInfo->pcPwd );
      for ( uIdx = 0; uIdx < pInfo->rlmlst.uNum; uIdx++ )
      {
         BRCM_DELETE_ARRAY( pInfo->rlmlst.rlm[uIdx].pcPwd );
         BRCM_DELETE_ARRAY( pInfo->rlmlst.rlm[uIdx].pcRlm );
         BRCM_DELETE_ARRAY( pInfo->rlmlst.rlm[uIdx].pcUsrName );
      }

      memset( pInfo, 0, sizeof( CCTKAUTHCINFO ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyAuthcInfo
**
**  PURPOSE:    Copy client authentication info.
**
**  PARAMETERS:
**              pSrc - Client authentication info to be copied.
**              pDst - Copied client authentication info.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the client authentication info may be dynamically
**              allocated when copying. clrAuthcInfo() must be called when the
**              copied client authentication info is not used.
***********************************************************************************/
static CFGSTATUS cpyAuthcInfo( const CCTKAUTHCINFO *pSrc, CCTKAUTHCINFO *pDst )
{
   CCTK_TRACED2( "cfg - cpyAuthcInfo(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyAuthcInfo: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   BOS_UINT32 uIdx;

   pDst->pcAuthName = utilStrDup( pSrc->pcAuthName );
   pDst->pcPwd = utilStrDup( pSrc->pcPwd );
   for ( uIdx = 0; uIdx < pSrc->rlmlst.uNum; uIdx++ )
   {
      pDst->rlmlst.rlm[uIdx].pcPwd = utilStrDup( pSrc->rlmlst.rlm[uIdx].pcPwd );
      pDst->rlmlst.rlm[uIdx].pcRlm = utilStrDup( pSrc->rlmlst.rlm[uIdx].pcRlm );
      pDst->rlmlst.rlm[uIdx].pcUsrName =
         utilStrDup( pSrc->rlmlst.rlm[uIdx].pcUsrName );
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   clrContact
**
**  PURPOSE:    Clear contact config and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Contact config to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrContact( CCTKCONTACT *pCfg )
{
   CCTK_TRACED1( "cfg - clrContact(%p)", pCfg );

   if ( pCfg != NULL )
   {
      BRCM_DELETE_ARRAY( pCfg->hdrOverride.pcAddr );
      BRCM_DELETE_ARRAY( pCfg->hdrOverride.pcHdrParm );
      BRCM_DELETE_ARRAY( pCfg->pcUsrParm );

      memset( pCfg, 0, sizeof( CCTKCONTACT ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyContact
**
**  PURPOSE:    Copy contact configuration.
**
**  PARAMETERS:
**              pSrc - Contact config to be copied.
**              pDst - Copied contact config.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the contact config may be dynamically allocated
**              when copying. clrContact() must be called when the copied contact
**              config is not used.
***********************************************************************************/
static CFGSTATUS cpyContact( const CCTKCONTACT *pSrc, CCTKCONTACT *pDst )
{
   CCTK_TRACED2( "cfg - cpyContact(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyContact: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->hdrOverride.pcAddr = utilStrDup( pSrc->hdrOverride.pcAddr );
   pDst->hdrOverride.pcHdrParm = utilStrDup( pSrc->hdrOverride.pcHdrParm );
   pDst->pcUsrParm = utilStrDup( pSrc->pcUsrParm );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   delContact
**
**  PURPOSE:    Delete contact config and de-allocate its content.
**
**  PARAMETERS:
**              rpCfg - Contact config to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delContact( CCTKCONTACT *&rpCfg )
{
   CCTK_TRACED1( "cfg - delContact(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      clrContact( rpCfg );

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupContact
**
**  PURPOSE:    Duplicate contact config.
**
**  PARAMETERS:
**              pSrc - Contact config to be duplicated.
**
**
**  RETURNS:    Return the duplicated contact config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the contact config may be dynamically allocated
**              when duplicating. delContact() must be called when the duplicated
**              contact config is not used.
***********************************************************************************/
static CCTKCONTACT *dupContact( const CCTKCONTACT *pSrc )
{
   CCTKCONTACT *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKCONTACT );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpyContact( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0(
               "cfg - dupContact: Error copying contact config." );
            delContact( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupContact(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   clrDiagEvtCfg
**
**  PURPOSE:    Clear dialog event config and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Dialog event config to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrDiagEvtCfg( CCTKDIAGEVTCFG *pCfg )
{
   CCTK_TRACED1( "cfg - clrDiagEvtCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      BRCM_DELETE_ARRAY( pCfg->pcSubscriber );

      memset( pCfg, 0, sizeof( CCTKDIAGEVTCFG ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyDiagEvtCfg
**
**  PURPOSE:    Copy dialog event configuration.
**
**  PARAMETERS:
**              pSrc - Dialog event config to be copied.
**              pDst - Copied dialog event config.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the dialog event config may be dynamically allocated
**              when copying. clrDiagEvtCfg() must be called when the copied dialog
**              event config is not used.
***********************************************************************************/
static CFGSTATUS cpyDiagEvtCfg( const CCTKDIAGEVTCFG *pSrc, CCTKDIAGEVTCFG *pDst )
{
   CCTK_TRACED2( "cfg - cpyDiagEvtCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyDiagEvtCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcSubscriber = utilStrDup( pSrc->pcSubscriber );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   delDiagEvtCfg
**
**  PURPOSE:    Delete dialog event config and de-allocate its content.
**
**  PARAMETERS:
**              rpCfg - Dialog event config to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delDiagEvtCfg( CCTKDIAGEVTCFG *&rpCfg )
{
   CCTK_TRACED1( "cfg - delDiagEvtCfg(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      clrDiagEvtCfg( rpCfg );

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupDiagEvtCfg
**
**  PURPOSE:    Duplicate dialog event config.
**
**  PARAMETERS:
**              pSrc - Dialog event config to be duplicated.
**
**
**  RETURNS:    Return the duplicated dialog event config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the dialog event config may be dynamically allocated
**              when duplicating. delDiagEvtCfg() must be called when the duplicated
**              dialog event config is not used.
***********************************************************************************/
static CCTKDIAGEVTCFG *dupDiagEvtCfg( const CCTKDIAGEVTCFG *pSrc )
{
   CCTKDIAGEVTCFG *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKDIAGEVTCFG );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpyDiagEvtCfg( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0(
               "cfg - dupDiagEvtCfg: Error copying dialog event config." );
            delDiagEvtCfg( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupDiagEvtCfg(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   clrUapEvtCfg
**
**  PURPOSE:    Clear user-agent profile event config and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - User-Agent profile event config to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrUapEvtCfg( CCTKUAPEVTCFG *pCfg )
{
   CCTK_TRACED1( "cfg - clrUapEvtCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      if ( pCfg->uNum )
      {
         BOS_UINT32 uIx;
         for ( uIx = 0 ; uIx < pCfg->uNum ; uIx++ )
         {
            BRCM_DELETE_ARRAY( pCfg->pcProfile[ uIx ] );
         }
      }

      memset( pCfg, 0, sizeof( CCTKUAPEVTCFG ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyUapEvtCfg
**
**  PURPOSE:    Copy user-agent profile event configuration.
**
**  PARAMETERS:
**              pSrc - User-agent profile event config to be copied.
**              pDst - Copied user-agent profile event config.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the user-agent profile event config may be dynamically
**              allocated when copying. clrUapEvtCfg() must be called when the
**              copied dialog event config is not used.
***********************************************************************************/
static CFGSTATUS cpyUapEvtCfg( const CCTKUAPEVTCFG *pSrc, CCTKUAPEVTCFG *pDst )
{
   CCTK_TRACED2( "cfg - cpyUapEvtCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyUapEvtCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   if ( pSrc->uNum )
   {
      BOS_UINT32 uIx;

      for ( uIx = 0 ; uIx < pSrc->uNum ; uIx++ )
      {
         pDst->pcProfile[ uIx ] = utilStrDup( pSrc->pcProfile[ uIx ] );
      }
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   delUapEvtCfg
**
**  PURPOSE:    Delete user-agent profile event config and de-allocate its content.
**
**  PARAMETERS:
**              rpCfg - User-agent progile event config to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delUapEvtCfg( CCTKUAPEVTCFG *&rpCfg )
{
   CCTK_TRACED1( "cfg - delUapEvtCfg(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      clrUapEvtCfg( rpCfg );

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupUapEvtCfg
**
**  PURPOSE:    Duplicate user-agent profile event config.
**
**  PARAMETERS:
**              pSrc - User-Agent profile event config to be duplicated.
**
**
**  RETURNS:    Return the duplicated user-agent profile event config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the user-agent profile event config may be dynamically
**              allocated when duplicating. delUapEvtCfg() must be called when the
**              duplicated user-agent profile event config is not used.
***********************************************************************************/
static CCTKUAPEVTCFG *dupUapEvtCfg( const CCTKUAPEVTCFG *pSrc )
{
   CCTKUAPEVTCFG *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKUAPEVTCFG );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpyUapEvtCfg( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0(
               "cfg - dupUapEvtCfg: Error copying dialog event config." );
            delUapEvtCfg( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupUapEvtCfg(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cpyMediaCfg
**
**  PURPOSE:    Copy media config block.
**
**  PARAMETERS:
**              pSrc - Media config block to be copied.
**              pDst - Copied media config block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the media config block may be dynamically allocated
**              when copying. clrMediaCfg() must be called when the copied
**              route list is not used.
***********************************************************************************/
static CFGSTATUS cpyMediaCfg( const CCTKMEDCFG *pSrc, CCTKMEDCFG *pDst )
{
   CCTK_TRACED2( "cfg - cpyMediaCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyMediaCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcSesName = utilStrDup( pSrc->pcSesName );
   CFM_VARDUP( static_cast< const CCTKMEDAUD * >( pSrc->pAudio ),
               pDst->pAudio,
               CCTKMEDAUD );
#if CCTKCFG_VIDEO
   pDst->pVideo = dupVideoCfg( static_cast< const CCTKMEDVID * >( pSrc->pVideo ));
#endif
   CFM_VARDUP( static_cast< const CCTKMEDFAX * >( pSrc->pFax ),
               pDst->pFax,
               CCTKMEDFAX );
   CFM_VARDUP( static_cast< const CCTKMEDLST * >( pSrc->pMediaLst ),
               pDst->pMediaLst,
               CCTKMEDLST );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   delMediaCfg
**
**  PURPOSE:    Delete media config and de-allocate its content.
**
**  PARAMETERS:
**              rpCfg - Media config block to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delMediaCfg( CCTKMEDCFG *&rpCfg )
{
   CCTK_TRACED1( "cfg - delMediaCfg(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      clrMediaCfg( rpCfg );

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupMediaCfg
**
**  PURPOSE:    Duplicate route list.
**
**  PARAMETERS:
**              pSrc - Route list to be duplicated.
**
**
**  RETURNS:    Return the duplicated media config block.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the media config block may be dynamically allocated
**              when duplicating. delMediaCfg() must be called when the duplicated
**              route list is not used.
***********************************************************************************/
static CCTKMEDCFG *dupMediaCfg( const CCTKMEDCFG *pSrc )
{
   CCTKMEDCFG *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKMEDCFG );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpyMediaCfg( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0( "cfg - dupMediaCfg: Error copying config block." );
            delMediaCfg( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupMediaCfg(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

#if CCTKCFG_VIDEO
/***********************************************************************************
**  FUNCTION:   dupVideoCfg
**
**  PURPOSE:    Duplicate video configuration.
**
**  PARAMETERS:
**              pSrc - Video configuration to be duplicated.
**
**
**  RETURNS:    Return the duplicated video configuration.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the video config may be dynamically allocated
**              when duplicating. delVideoCfg() must be called when the duplicated
**              route list is not used.
***********************************************************************************/
static CCTKMEDVID *dupVideoCfg( const CCTKMEDVID *pSrc )
{
   CCTKMEDVID *pRes = NULL;

   CFM_VARDUP( pSrc, pRes, CCTKMEDVID );

   if ( pRes != NULL )
   {
      BOS_UINT32 uIdx;

      for ( uIdx = 0; uIdx < pSrc->gen.uNumCodec; uIdx++ )
      {
         if ( pSrc->gen.codec[uIdx].eType == eCCTKCODEC_H263 &&
              pSrc->gen.codec[uIdx].parm.pH263 != NULL )
         {
            CFM_VARDUP( pSrc->gen.codec[uIdx].parm.pH263,
                        pRes->gen.codec[uIdx].parm.pH263,
                        CCTKH263PARM );
         }
         else if ( pSrc->gen.codec[uIdx].eType == eCCTKCODEC_H264 &&
                   pSrc->gen.codec[uIdx].parm.pH264 != NULL )
         {
            CFM_VARDUP( pSrc->gen.codec[uIdx].parm.pH264,
                        pRes->gen.codec[uIdx].parm.pH264,
                        CCTKH264PARM );
         }
      }
   }

   CCTK_TRACED2( "cfg - dupVideoCfg(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   delVideoCfg
**
**  PURPOSE:    Delete video config and de-allocate its content.
**
**  PARAMETERS:
**              rpLst - Video config to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delVideoCfg( CCTKMEDVID *&rpCfg )
{
   CCTK_TRACED1( "cfg - delVideoCfg(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      BOS_UINT32 uIdx;

      for ( uIdx = 0; uIdx < rpCfg->gen.uNumCodec; uIdx++ )
      {
         if ( rpCfg->gen.codec[uIdx].eType == eCCTKCODEC_H263 )
         {
            BRCM_DELETE( rpCfg->gen.codec[uIdx].parm.pH263 );
         }
         else if ( rpCfg->gen.codec[uIdx].eType == eCCTKCODEC_H264 )
         {
            BRCM_DELETE( rpCfg->gen.codec[uIdx].parm.pH264 );
         }
      }

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}
#endif

/***********************************************************************************
**  FUNCTION:   cpyRoute
**
**  PURPOSE:    Copy routing information.
**
**  PARAMETERS:
**              src - Routing information to be copied.
**              dst - Copied routing information.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the routing information may be dynamically allocated
**              when copying. clrRoute() must be called when the copied routing
**              information is not used.
***********************************************************************************/
static CFGSTATUS cpyRoute( const CCTKROUTE src[], CCTKROUTE dst[] )
{
   CCTK_TRACED2( "cfg - cpyRoute(%p, %p)", src, dst );

   if ( src == NULL || dst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyRoute: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   /* Copy the ones that require dynamic memory allocation */
   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_OBPROXY; uIdx++ )
   {
      cpySipHost( &( src[uIdx].addr ), &( dst[uIdx].addr ) );
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cpyRouteLst
**
**  PURPOSE:    Copy route list.
**
**  PARAMETERS:
**              pSrc - Route list to be copied.
**              pDst - Copied route list.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the route list may be dynamically allocated
**              when copying. clrRouteLst() must be called when the copied
**              route list is not used.
***********************************************************************************/
static CFGSTATUS cpyRouteLst( const CCTKROUTELST *pSrc, CCTKROUTELST *pDst )
{
   CCTK_TRACED2( "cfg - cpyRouteLst(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyRouteLst: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   return cpyRoute( pSrc->route, pDst->route );
}

/***********************************************************************************
**  FUNCTION:   cpySvcRoute
**
**  PURPOSE:    Copy Service-Route header configuration.
**
**  PARAMETERS:
**              pSrc - Service-Route header configuration to be copied.
**              pDst - Copied Service-Route header configuration.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the Service-Route header may be dynamically allocated
**              when copying. clrSvcRoute() must be called when the copied
**              Service-Route header configuration is not used.
***********************************************************************************/
static CFGSTATUS cpySvcRoute( const CCTKSVCROUTE *pSrc, CCTKSVCROUTE *pDst )
{
   CCTK_TRACED2( "cfg - cpySvcRoute(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpySvcRoute: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   return cpyRoute( pSrc->route, pDst->route );
}

/***********************************************************************************
**  FUNCTION:   delRouteLst
**
**  PURPOSE:    Delete route list and de-allocate its content.
**
**  PARAMETERS:
**              rpLst - Route list to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delRouteLst( CCTKROUTELST *&rpLst )
{
   CCTK_TRACED1( "cfg - delRouteLst(%p)", rpLst );

   if ( rpLst != NULL )
   {
      clrRouteLst( rpLst );

      BRCM_DELETE( rpLst );
      rpLst = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   delSvcRoute
**
**  PURPOSE:    Delete Service-Route header config and de-allocate its content.
**
**  PARAMETERS:
**              rpRoute - Service-Route header config to be deleted.
**                        Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delSvcRoute( CCTKSVCROUTE *&rpRoute )
{
   CCTK_TRACED1( "cfg - delSvcRoute(%p)", rpRoute );

   if ( rpRoute != NULL )
   {
      clrSvcRoute( rpRoute );

      BRCM_DELETE( rpRoute );
      rpRoute = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupRouteLst
**
**  PURPOSE:    Duplicate route list.
**
**  PARAMETERS:
**              pSrc - Route list to be duplicated.
**
**
**  RETURNS:    Return the duplicated route list.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the route list may be dynamically allocated
**              when duplicating. delRouteLst() must be called when the duplicated
**              route list is not used.
***********************************************************************************/
static CCTKROUTELST *dupRouteLst( const CCTKROUTELST *pSrc )
{
   CCTKROUTELST *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKROUTELST );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpyRouteLst( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0( "cfg - dupRouteLst: Error copying route list." );
            delRouteLst( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupRouteLst(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   dupSvcRoute
**
**  PURPOSE:    Duplicate Service-Route header configuration.
**
**  PARAMETERS:
**              pSrc - Service-Route header configuration to be duplicated.
**
**
**  RETURNS:    Return the duplicated Service-Route header configuration.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the Service-Route header config may be dynamically
**              allocated when duplicating. delSvcRoute() must be called when the
**              duplicated Service-Route header configuration is not used.
***********************************************************************************/
static CCTKSVCROUTE *dupSvcRoute( const CCTKSVCROUTE *pSrc )
{
   CCTKSVCROUTE *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKSVCROUTE );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpySvcRoute( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0( "cfg - dupSvcRoute: Error copying Service-Route." );
            delSvcRoute( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupSvcRoute(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   clrSipHost
**
**  PURPOSE:    Clear SIP host port address info and de-allocate its content.
**
**  PARAMETERS:
**              pSipHost   - SIP host port address info to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrSipHost( CCTKSIPHOST *pSipHost )
{
   CCTK_TRACED1( "cfg - clrSipHost(%p)", pSipHost );

   if ( pSipHost != NULL )
   {
      clrHostPort( &( pSipHost->hostPort ) );

      memset( pSipHost, 0, sizeof( CCTKSIPHOST ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpySipHost
**
**  PURPOSE:    Copy SIP host port address info.
**
**  PARAMETERS:
**              pSrc - SIP host port address info to be copied.
**              pDst - Copied SIP host port address info.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the SIP host port address info may be dynamically
**              allocated when copying. clrSipHost() must be called when the copied
**              SIP host port address info is not used.
***********************************************************************************/
static CFGSTATUS cpySipHost( const CCTKSIPHOST *pSrc, CCTKSIPHOST *pDst )
{
   CCTK_TRACED2( "cfg - cpySipHost(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpySipHost: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   cpyHostPort( &( pSrc->hostPort ), &( pDst->hostPort ) );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   clrSipName
**
**  PURPOSE:    Clear SIP name info and de-allocate its content.
**
**  PARAMETERS:
**              pName - SIP name info to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrSipName( CCTKSIPNAME *pName )
{
   CCTK_TRACED1( "cfg - clrSipName(%p)", pName );

   if ( pName != NULL )
   {
      BRCM_DELETE_ARRAY( pName->pcDisplay );
      BRCM_DELETE_ARRAY( pName->pcUsrName );

      memset( pName, 0, sizeof( CCTKSIPNAME ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpySipName
**
**  PURPOSE:    Copy SIP name info.
**
**  PARAMETERS:
**              pSrc - SIP name info to be copied.
**              pDst - Copied SIP name info.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the SIP name info may be dynamically allocated
**              when copying. clrSipName() must be called when the copied
**              SIP name info is not used.
***********************************************************************************/
static CFGSTATUS cpySipName( const CCTKSIPNAME *pSrc, CCTKSIPNAME *pDst )
{
   CCTK_TRACED2( "cfg - cpySipName(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpySipName: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcDisplay = utilStrDup( pSrc->pcDisplay );
   pDst->pcUsrName = utilStrDup( pSrc->pcUsrName );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   delSipName
**
**  PURPOSE:    Delete SIP name info and de-allocate its content.
**
**  PARAMETERS:
**              rpName - SIP name info to be deleted.
**                       Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delSipName( CCTKSIPNAME *&rpName )
{
   CCTK_TRACED1( "cfg - delSipName(%p)", rpName );

   if ( rpName != NULL )
   {
      clrSipName( rpName );

      BRCM_DELETE( rpName );
      rpName = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupSipName
**
**  PURPOSE:    Duplicate SIP name information.
**
**  PARAMETERS:
**              pSrc - SIP name info to be duplicated.
**
**
**  RETURNS:    Return the duplicated SIP name info.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the SIP name info may be dynamically allocated
**              when duplicating. delSipName() must be called when the
**              duplicated SIP name info is not used.
***********************************************************************************/
static CCTKSIPNAME *dupSipName( const CCTKSIPNAME *pSrc )
{
   CCTKSIPNAME *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKSIPNAME );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpySipName( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0(
               "cfg - dupSipName: Error copying SIP name info." );
            delSipName( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupSipName(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   clrSipUsrHost
**
**  PURPOSE:    Clear SIP user host port info and de-allocate its content.
**
**  PARAMETERS:
**              pUsrHost - SIP user host port info to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrSipUsrHost( CCTKSIPUSRHOST *pUsrHost )
{
   CCTK_TRACED1( "cfg - clrSipUsrHost(%p)", pUsrHost );

   if ( pUsrHost != NULL )
   {
      BRCM_DELETE_ARRAY( pUsrHost->pcUsrName );
      clrSipHost( &( pUsrHost->sipHost ) );

      memset( pUsrHost, 0, sizeof( CCTKSIPUSRHOST ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpySipUsrHost
**
**  PURPOSE:    Copy SIP user host port info.
**
**  PARAMETERS:
**              pSrc - SIP user host port info to be copied.
**              pDst - Copied SIP user host port info.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the SIP host port info may be dynamically allocated
**              when copying. clrSipUsrHost() must be called when the copied
**              SIP user host port info is not used.
***********************************************************************************/
static CFGSTATUS cpySipUsrHost( const CCTKSIPUSRHOST *pSrc, CCTKSIPUSRHOST *pDst )
{
   CCTK_TRACED2( "cfg - cpySipUsrHost(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpySipUsrHost: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcUsrName = utilStrDup( pSrc->pcUsrName );
   cpySipHost( &( pSrc->sipHost ), &( pDst->sipHost ) );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   delSipUsrHost
**
**  PURPOSE:    Delete SIP user host port info and de-allocate its content.
**
**  PARAMETERS:
**              rpUsrHost - SIP user host port info to be deleted.
**                          Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delSipUsrHost( CCTKSIPUSRHOST *&rpUsrHost )
{
   CCTK_TRACED1( "cfg - delSipUsrHost(%p)", rpUsrHost );

   if ( rpUsrHost != NULL )
   {
      clrSipUsrHost( rpUsrHost );

      BRCM_DELETE( rpUsrHost );
      rpUsrHost = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupSipUsrHost
**
**  PURPOSE:    Duplicate SIP user host port information.
**
**  PARAMETERS:
**              pSrc - SIP user host port info to be duplicated.
**
**
**  RETURNS:    Return the duplicated SIP user host port info.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the SIP user host port info may be dynamically
**              allocated when duplicating. delSipUsrHost() must be called when the
**              duplicated SIP user host port info is not used.
***********************************************************************************/
static CCTKSIPUSRHOST *dupSipUsrHost( const CCTKSIPUSRHOST *pSrc )
{
   CCTKSIPUSRHOST *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKSIPUSRHOST );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpySipUsrHost( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0(
               "cfg - dupSipUsrHost: Error copying SIP user host port info." );
            delSipUsrHost( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupSipUsrHost(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   clrEmgAddr
**
**  PURPOSE:    Clear emergency service address info and de-allocate its content.
**
**  PARAMETERS:
**              pAddr - Emergency service address info to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrEmgAddr( CCTKEMGADDR *pAddr )
{
   CCTK_TRACED1( "cfg - clrEmgAddr(%p)", pAddr );

   if ( pAddr != NULL )
   {
      BRCM_DELETE_ARRAY( pAddr->pcCity );
      BRCM_DELETE_ARRAY( pAddr->pcCountry );
      BRCM_DELETE_ARRAY( pAddr->pcHouseNum );
      BRCM_DELETE_ARRAY( pAddr->pcLocInfo );
      BRCM_DELETE_ARRAY( pAddr->pcPostal );
      BRCM_DELETE_ARRAY( pAddr->pcStreet );
      BRCM_DELETE_ARRAY( pAddr->pcSubDiv );
      BRCM_DELETE_ARRAY( pAddr->pcTimeStamp );
      BRCM_DELETE_ARRAY( pAddr->pcGeoTimeStamp );
      BRCM_DELETE_ARRAY( pAddr->pcFLR );
      BRCM_DELETE_ARRAY( pAddr->pcSrsName );
      BRCM_DELETE_ARRAY( pAddr->pcPos );


      memset( pAddr, 0, sizeof( CCTKEMGADDR ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyEmgAddr
**
**  PURPOSE:    Copy emergency service address info.
**
**  PARAMETERS:
**              pSrc - Emergency service address info to be copied.
**              pDst - Copied emergency service address info.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the emergency service address info may be dynamically
**              allocated when copying. clrEmgAddr() must be called when the copied
**              emergency service address info is not used.
***********************************************************************************/
static CFGSTATUS cpyEmgAddr( const CCTKEMGADDR *pSrc, CCTKEMGADDR *pDst )
{
   CCTK_TRACED2( "cfg - cpyEmgAddr(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyEmgAddr: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcCity = utilStrDup( pSrc->pcCity );
   pDst->pcCountry = utilStrDup( pSrc->pcCountry );
   pDst->pcHouseNum = utilStrDup( pSrc->pcHouseNum );
   pDst->pcLocInfo = utilStrDup( pSrc->pcLocInfo );
   pDst->pcPostal = utilStrDup( pSrc->pcPostal );
   pDst->pcStreet = utilStrDup( pSrc->pcStreet );
   pDst->pcSubDiv = utilStrDup( pSrc->pcSubDiv );
   pDst->pcTimeStamp = utilStrDup( pSrc->pcTimeStamp );
   pDst->pcGeoTimeStamp = utilStrDup( pSrc->pcGeoTimeStamp);
   pDst->pcFLR = utilStrDup( pSrc->pcFLR);
   pDst->pcPos = utilStrDup( pSrc->pcPos);
   pDst->pcSrsName = utilStrDup( pSrc->pcSrsName);


   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   clrGenHdr
**
**  PURPOSE:    Clear generic header config and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Generic header config to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrGenHdr( CCTKGENHDRCFG *pCfg )
{
   CCTK_TRACED1( "cfg - clrGenHdr(%p)", pCfg );

   if ( pCfg != NULL )
   {
      BOS_UINT32 uIdx;

      for ( uIdx = 0; uIdx < pCfg->uNum; uIdx++ )
      {
         if ( pCfg->hdr[uIdx].hdr.eType == eCCTKHDRTYPE_PRIVATE )
         {
            BRCM_DELETE_ARRAY( pCfg->hdr[uIdx].hdr.pcName );
         }
         BRCM_DELETE_ARRAY( pCfg->hdr[uIdx].hdr.pcValue );
      }

      memset( pCfg, 0, sizeof( CCTKGENHDRCFG ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyGenHdr
**
**  PURPOSE:    Copy generic header configuration.
**
**  PARAMETERS:
**              pSrc - Generic header config to be copied.
**              pDst - Copied private header config.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the generic header config may be dynamically allocated
**              when copying. clrGenHdr() must be called when the copied generic
**              header config is not used.
***********************************************************************************/
static CFGSTATUS cpyGenHdr( const CCTKGENHDRCFG *pSrc, CCTKGENHDRCFG *pDst )
{
   CCTK_TRACED2( "cfg - cpyGenHdr(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyGenHdr: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < pSrc->uNum; uIdx++ )
   {
      if ( pDst->hdr[uIdx].hdr.eType == eCCTKHDRTYPE_PRIVATE )
      {
         pDst->hdr[uIdx].hdr.pcName = utilStrDup( pSrc->hdr[uIdx].hdr.pcName );
      }
      else
      {
         pDst->hdr[uIdx].hdr.pcName = NULL;
      }
      pDst->hdr[uIdx].hdr.pcValue = utilStrDup( pSrc->hdr[uIdx].hdr.pcValue );
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   delGenHdr
**
**  PURPOSE:    Delete generic header config and de-allocate its content.
**
**  PARAMETERS:
**              rpCfg - Generic header config to be deleted.
**                      Memory ownership is taken by the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delGenHdr( CCTKGENHDRCFG *&rpCfg )
{
   CCTK_TRACED1( "cfg - delGenHdr(%p)", rpCfg );

   if ( rpCfg != NULL )
   {
      clrGenHdr( rpCfg );

      BRCM_DELETE( rpCfg );
      rpCfg = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   dupGenHdr
**
**  PURPOSE:    Duplicate generic header config.
**
**  PARAMETERS:
**              pSrc - Generic header config to be duplicated.
**
**
**  RETURNS:    Return the duplicated generic header config.
**              Memory ownership is given to the caller of the function.
**
**  NOTES:      Parameters in the generic header config may be dynamically allocated
**              when duplicating. delGenHdr() must be called when the duplicated
**              generic header config is not used.
***********************************************************************************/
static CCTKGENHDRCFG *dupGenHdr( const CCTKGENHDRCFG *pSrc )
{
   CCTKGENHDRCFG *pRes = NULL;

   if ( pSrc != NULL )
   {
      CFM_VARDUP( pSrc, pRes, CCTKGENHDRCFG );

      /* Copy the ones that require dynamic memory allocation */
      if ( pRes != NULL )
      {
         if ( cpyGenHdr( pSrc, pRes ) != eCFGSTATUS_SUCCESS )
         {
            CCTK_TRACEI0(
               "cfg - dupGenHdr: Error copying generic header config." );
            delGenHdr( pRes );
            pRes = NULL;
         }
      }
   }

   CCTK_TRACED2( "cfg - dupGenHdr(%p) returns (%p)", pSrc, pRes );

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   clrHdr
**
**  PURPOSE:    Clear header info and de-allocate its content.
**
**  PARAMETERS:
**              pHdr - Header info to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrHdr( CCTKHDR *pHdr )
{
   CCTK_TRACED1( "cfg - clrHdr(%p)", pHdr );

   if ( pHdr != NULL )
   {
      BRCM_DELETE_ARRAY( pHdr->pcName );
      BRCM_DELETE_ARRAY( pHdr->pcValue );

      memset( pHdr, 0, sizeof( CCTKHDR ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyHdr
**
**  PURPOSE:    Copy header info.
**
**  PARAMETERS:
**              pSrc - Header info to be copied.
**              pDst - Copied header info.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the header info may be dynamically allocated when
**              copying. clrHdr() must be called when the copied header info is not
**              used.
***********************************************************************************/
static CFGSTATUS cpyHdr( const CCTKHDR *pSrc, CCTKHDR *pDst )
{
   CCTK_TRACED2( "cfg - cpyHdr(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyHdr: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcName = utilStrDup( pSrc->pcName );
   pDst->pcValue = utilStrDup( pSrc->pcValue );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   clrHostPort
**
**  PURPOSE:    Clear host port address info and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - Host port address info to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrHostPort( CCTKHOSTPORT *pCfg )
{
   CCTK_TRACED1( "cfg - clrHostPort(%p)", pCfg );

   if ( pCfg != NULL )
   {
      BRCM_DELETE_ARRAY( pCfg->pcAddr );

      memset( pCfg, 0, sizeof( CCTKHOSTPORT ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyHostPort
**
**  PURPOSE:    Copy host port address info.
**
**  PARAMETERS:
**              pSrc - Host port address info to be copied.
**              pDst - Copied host port address info.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the host port address info may be dynamically
**              allocated when copying. clrHostPort() must be called when the copied
**              host port address info is not used.
***********************************************************************************/
static CFGSTATUS cpyHostPort( const CCTKHOSTPORT *pSrc, CCTKHOSTPORT *pDst )
{
   CCTK_TRACED2( "cfg - cpyHostPort(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyHostPort: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcAddr = utilStrDup( pSrc->pcAddr );

   return eCFGSTATUS_SUCCESS;
}

#if CCTKCFG_REMOTE_USER_CONTROL
/***********************************************************************************
**  FUNCTION:   clrRemUsrSysCfg
**
**  PURPOSE:    Clear remote user system config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - remote user system config block to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrRemUsrSysCfg( CCTKREMUSRSYS *pCfg )
{
   CCTK_TRACED1( "cfg - clrRemUsrSysCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      BRCM_DELETE_ARRAY( pCfg->pcDftAssIMPUUserId );
      BRCM_DELETE_ARRAY( pCfg->pcDftIMPIUserId );
      BRCM_DELETE_ARRAY( pCfg->pcDftIMPIUserPassword );
      memset( pCfg, 0, sizeof( CCTKREMUSRSYS ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyRemUsrSysCfg
**
**  PURPOSE:    Copy remote user system configuration.
**
**  PARAMETERS:
**              pSrc - Remote user system configuration to be copied.
**              pDst - Copied remote user system configuration.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the remote user system configuration info may be dynamically
**              allocated when copying. clrRemUsrSysCfg() must be called when the copied
**              remote user system configuration is not used.
***********************************************************************************/
static CFGSTATUS cpyRemUsrSysCfg( const CCTKREMUSRSYS *pSrc, CCTKREMUSRSYS *pDst )
{
   CCTK_TRACED2( "cfg - cpyRemUsrSysCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyRemUsrSysCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcDftAssIMPUUserId = utilStrDup( pSrc->pcDftAssIMPUUserId );
   pDst->pcDftIMPIUserId = utilStrDup( pSrc->pcDftIMPIUserId );
   pDst->pcDftIMPIUserPassword = utilStrDup( pSrc->pcDftIMPIUserPassword );
   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   clrRemUsrXtlCfg
**
**  PURPOSE:    Clear external (main + extended) remote user config block
**              and de-allocate its content.
**
**  PARAMETERS:
**              pCfg       - External remote user config to be cleared.
**              bMainOnly  - True to clear main configuration only.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrRemUsrXtlCfg( CFGUEREMUSR *pCfg, BOS_BOOL bMainOnly )
{
   CCTK_TRACED2( "cfg - clrRemUsrXtlCfg(%p, %d)", pCfg, bMainOnly );

   if ( pCfg != NULL )
   {
      /* Clear main configuration */
      memset( pCfg, 0, sizeof( CCTKREMUSR ) );

      /* Clear extended configuration */
      if ( !bMainOnly )
      {
         /* Clear the rest of the config */
         clrRemUsrCfg( &( pCfg->cfg ) );
         memset( pCfg, 0, sizeof( CFGUEREMUSR ) );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   cpyRemUsrXtlCfg
**
**  PURPOSE:    Copy external (main + extended) remote user configuration block.
**
**  PARAMETERS:
**              pSrc - External remote user config block to be copied.
**              pDst - Copied external remote user config block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Some parameters in the user config block may be dynamically
**              allocated when copying. clrRemUsrXtlCfg() must be called when
**              the copied external remote user config block is not used.
***********************************************************************************/
static CFGSTATUS cpyRemUsrXtlCfg( const CFGUEREMUSR *pSrc, CFGUEREMUSR *pDst )
{
   CCTK_TRACED2( "cfg - cpyRemUsrXtlCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyRemUsrXtlCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   cpyRemUsrCfg( &( pSrc->cfg ), &( pDst->cfg ) );
}


/***********************************************************************************
**  FUNCTION:   clrRemUsrCfg
**
**  PURPOSE:    Clear remote user config block and de-allocate its content.
**
**  PARAMETERS:
**              pCfg - remote user config block to be cleared.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void clrRemUsrCfg( CCTKREMUSRCFG *pCfg )
{
   CCTK_TRACED1( "cfg - clrRemUsrCfg(%p)", pCfg );

   if ( pCfg != NULL )
   {
      BRCM_DELETE_ARRAY( pCfg->pcIMPIUserId );
      BRCM_DELETE_ARRAY( pCfg->pcIMPIPassword );
      memset( pCfg, 0, sizeof( CCTKREMUSRCFG ) );
   }
}

/***********************************************************************************
**  FUNCTION:   cpyRemUsrCfg
**
**  PURPOSE:    Copy remote user configuration.
**
**  PARAMETERS:
**              pSrc - Remote user configuration to be copied.
**              pDst - Copied remote user configuration.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      Parameters in the remote user configuration info may be dynamically
**              allocated when copying. clrRemUsrCfg() must be called when the copied
**              remote user configuration is not used.
***********************************************************************************/
static CFGSTATUS cpyRemUsrCfg( const CCTKREMUSRCFG *pSrc, CCTKREMUSRCFG *pDst )
{
   CCTK_TRACED2( "cfg - cpyRemUsrCfg(%p, %p)", pSrc, pDst );

   if ( pSrc == NULL || pDst == NULL )
   {
      CCTK_TRACEI0( "cfg - cpyRemUsrCfg: Bad parameters." );
      return eCFGSTATUS_BADPARM;
   }

   *pDst = *pSrc;

   /* Copy the ones that require dynamic memory allocation */
   pDst->pcIMPIUserId = utilStrDup( pSrc->pcIMPIUserId );
   pDst->pcIMPIPassword = utilStrDup( pSrc->pcIMPIPassword );
   return eCFGSTATUS_SUCCESS;
}
#endif


/***********************************************************************************
**  FUNCTION:   delSysCfg
**
**  PURPOSE:    De-allocate system configuration block and its content.
**
**  PARAMETERS:
**              rpCfg - System configuration to be de-allocated.
**                      Ownership is given to the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delSysCfg( CFGSYS *&rpCfg )
{
   CCTK_TRACED1( "cfg - delSysCfg(%p)", rpCfg );

   clrSysCfg( rpCfg );
   BRCM_DELETE( rpCfg );
   rpCfg = NULL;
}

/***********************************************************************************
**  FUNCTION:   delUsrCfg
**
**  PURPOSE:    De-allocate user configuration block and its content.
**
**  PARAMETERS:
**              rpCfg - User configuration to be de-allocated.
**                      Ownership is given to the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delUsrCfg( CFMUSR *&rpCfg )
{
   CCTK_TRACED1( "cfg - delUsrCfg(%p)", rpCfg );

   clrUsrCfg( rpCfg );
   BRCM_DELETE( rpCfg );
   rpCfg = NULL;
}

/***********************************************************************************
**  FUNCTION:   delCallCfg
**
**  PURPOSE:    De-allocate call configuration block and its content.
**
**  PARAMETERS:
**              rpCfg - Call configuration to be de-allocated.
**                      Ownership is given to the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delCallCfg( CFMCALL *&rpCfg )
{
   CCTK_TRACED1( "cfg - delCallCfg(%p)", rpCfg );

   clrCallCfg( rpCfg );
   BRCM_DELETE( rpCfg );
   rpCfg = NULL;
}

/***********************************************************************************
**  FUNCTION:   delEvtCfg
**
**  PURPOSE:    De-allocate event subscription configuration block and its content.
**
**  PARAMETERS:
**              rpCfg - Event subscription configuration to be de-allocated.
**                      Ownership is given to the function.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void delEvtCfg( CFMEVT *&rpCfg )
{
   CCTK_TRACED1( "cfg - delEvtCfg(%p)", rpCfg );

   clrEvtCfg( rpCfg );
   BRCM_DELETE( rpCfg );
   rpCfg = NULL;
}

/***********************************************************************************
**  FUNCTION:   initCfgBlk
**
**  PURPOSE:    Initialize the global configuration block and its content.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
static CFGSTATUS initCfgBlk( void )
{
   CCTK_TRACED0( "cfg - initCfgBlk()" );

   memset( &( CFM_CFG ), 0, sizeof( CFMCFGBLK ) );

   CFM_VARNEW( CFM_CFG.pSys, CFGSYS );
   if ( CFM_CFG.pSys == NULL )
   {
      deInitCfgBlk();
      return eCFGSTATUS_NORSC;
   }

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
   {
      CFM_VARNEW( CFM_CFG.pUsr[uIdx], CFMUSR );
      if ( CFM_CFG.pUsr[uIdx] == NULL )
      {
         deInitCfgBlk();
         return eCFGSTATUS_NORSC;
      }
      CFM_CFG.pUsr[uIdx]->uGuid = guCeInvalidGuid;
   }

   for ( uIdx = 0; uIdx < CCTKCFG_MAX_CALL; uIdx++ )
   {
      CFM_VARNEW( CFM_CFG.pCall[uIdx], CFMCALL );
      if ( CFM_CFG.pCall[uIdx] == NULL )
      {
         deInitCfgBlk();
         return eCFGSTATUS_NORSC;
      }
      CFM_CFG.pCall[uIdx]->uGuid = guCeInvalidGuid;
   }

   for ( uIdx = 0; uIdx < CCTKCFG_MAX_EVT; uIdx++ )
   {
      CFM_VARNEW( CFM_CFG.pEvt[uIdx], CFMEVT );
      if ( CFM_CFG.pEvt[uIdx] == NULL )
      {
         deInitCfgBlk();
         return eCFGSTATUS_NORSC;
      }
      CFM_CFG.pEvt[uIdx]->uGuid = guCeInvalidGuid;
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   initExtSysCfg
**
**  PURPOSE:    Initialize system config block with pre-defined extended config.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
static CFGSTATUS initExtSysCfg( void )
{
   CCTK_TRACED0( "cfg - initExtSysCfg()" );

   CCTKPARM eExtParm;
   const void *pExtCfg;
   void *pCfgDup;

   CFGSTATUS eRes = eCFGSTATUS_SUCCESS;
   BOS_UINT32 uIdx;
   for ( uIdx = 0; gCctkDftExtSysCfg[uIdx].eParm != eCCTKPARM_MAX; uIdx++ )
   {
      eExtParm = gCctkDftExtSysCfg[uIdx].eParm;
      pExtCfg = gCctkDftExtSysCfg[uIdx].pCfg;

      if ( !CCTK_IS_EC_SYS( eExtParm ) )
      {
         CCTK_TRACEI1(
            "cfg - initExtSysCfg: Invalid system config parameter %d.", eExtParm );
         eRes = eCFGSTATUS_BADPARM;
         break;
      }

      pCfgDup = cfgDupXtlCfg( eExtParm, pExtCfg );
      if ( pCfgDup == NULL )
      {
         CCTK_TRACEI2(
            "cfg - initExtSysCfg: Error copying config %p with parameter %d.",
            pExtCfg,
            eExtParm );
         eRes = eCFGSTATUS_BADEXTCFG;
         break;
      }

      if ( setSysXtlCfg( eExtParm, pCfgDup ) != eCFGSTATUS_SUCCESS )
      {
         CCTK_TRACEI1(
            "cfg - initExtSysCfg: Error setting extended config %d.", eExtParm );
         cfgDelXtlCfg( eExtParm, pCfgDup );
      }
   }

   return eRes;
}

/***********************************************************************************
**  FUNCTION:   initExtUsrCfg
**
**  PURPOSE:    Initialize user config block with pre-defined extended config.
**
**  PARAMETERS:
**              uIdx - User config block array index.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
static CFGSTATUS initExtUsrCfg( BOS_UINT32 uIdx )
{
   CCTK_TRACED1( "cfg - initExtUsrCfg(%lu)", uIdx );

   if ( uIdx >= CFG_MAX_USR )
   {
      CCTK_TRACEI0( "cfg - initExtUsrCfg: Invalid user config index." );
      return eCFGSTATUS_BADPARM;
   }

   CCTKPARM eExtParm;
   const void *pExtCfg;
   void *pCfgDup;

   CFGSTATUS eRes = eCFGSTATUS_SUCCESS;
   BOS_UINT32 uIndex;
   for ( uIndex = 0; gCctkDftExtUsrCfg[uIndex].eParm != eCCTKPARM_MAX; uIndex++ )
   {
      eExtParm = gCctkDftExtUsrCfg[uIndex].eParm;
      pExtCfg = gCctkDftExtUsrCfg[uIndex].pCfg;

      if ( !CCTK_IS_EC_USR( eExtParm ) )
      {
         CCTK_TRACEI1(
            "cfg - initExtUsrCfg: Invalid user config parameter %d.", eExtParm );
         eRes = eCFGSTATUS_BADPARM;
         break;
      }

      pCfgDup = cfgDupXtlCfg( eExtParm, pExtCfg );
      if ( pCfgDup == NULL )
      {
         CCTK_TRACEI2(
            "cfg - initExtUsrCfg: Error copying config %p with parameter %d.",
            pExtCfg,
            eExtParm );
         eRes = eCFGSTATUS_BADEXTCFG;
         break;
      }

      if ( setUsrXtlCfg( uIdx, eExtParm, pCfgDup ) != eCFGSTATUS_SUCCESS )
      {
         CCTK_TRACEI1(
            "cfg - initExtUsrCfg: Error setting extended config %d.", eExtParm );
         cfgDelXtlCfg( eExtParm, pCfgDup );
      }
   }

   return eRes;
}

/***********************************************************************************
**  FUNCTION:   deInitCfgBlk
**
**  PURPOSE:    De-initialize the global configuration block and clear its content.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void deInitCfgBlk( void )
{
   CCTK_TRACED0( "cfg - deInitCfgBlk()" );

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_EVT; uIdx++ )
   {
      delEvtCfg( CFM_CFG.pEvt[uIdx] );
   }
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_CALL; uIdx++ )
   {
      delCallCfg( CFM_CFG.pCall[uIdx] );
   }
   for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
   {
      delUsrCfg( CFM_CFG.pUsr[uIdx] );
   }

   delSysCfg( CFM_CFG.pSys );

   memset( &( CFM_CFG ), 0, sizeof( CFMCFGBLK ) );
}

/***********************************************************************************
**  FUNCTION:   deInitCfgFw
**
**  PURPOSE:    De-initialize the entire Configuration Framework module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous and MUST only be called within
**                the application task context.
***********************************************************************************/
static CFGSTATUS deInitCfgFw( void )
{
   CCTK_TRACED0( "cfg - deInitCfgFw()" );

   /* Destroy configuration engine */
   BRCM_DELETE( gCfmCtrlBlk.pEngine );
   gCfmCtrlBlk.pEngine = NULL;

   /* De-initialize configuration block */
   deInitCfgBlk();

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   getIdx
**
**  PURPOSE:    Retrieve the configuration resource index from GUID.
**
**  PARAMETERS:
**              eType - Configuration resource type.
**              uGuid - GUID assoicated to the resource.
**
**
**  RETURNS:    Index of the config resource. "-1" if not found.
**
**  NOTES:      None.
***********************************************************************************/
static BOS_SINT32 getIdx( CFGTYPE eType, BOS_UINT32 uGuid )
{
   BOS_SINT32 sRes = -1;

   if ( uGuid != guCeInvalidGuid )
   {
      BOS_UINT32 uIdx;

      switch ( eType )
      {
      case eCFGTYPE_USR:
         {
            for ( uIdx = 0; uIdx < CFG_MAX_USR; uIdx++ )
            {
               if ( CFM_CFG.pUsr[uIdx]->uGuid == uGuid )
               {
                  sRes = uIdx;
                  break;
               }
            }
         }
         break;
      case eCFGTYPE_CALL:
         {
            for ( uIdx = 0; uIdx < CCTKCFG_MAX_CALL; uIdx++ )
            {
               if ( CFM_CFG.pCall[uIdx]->uGuid == uGuid )
               {
                  sRes = uIdx;
                  break;
               }
            }
         }
         break;
      case eCFGTYPE_EVT:
         {
            for ( uIdx = 0; uIdx < CCTKCFG_MAX_EVT; uIdx++ )
            {
               if ( CFM_CFG.pEvt[uIdx]->uGuid == uGuid )
               {
                  sRes = uIdx;
                  break;
               }
            }
         }
         break;
      default:
         CCTK_TRACEI1( "cfg - getIdx: Invalid config type = %d", eType );
         break;
      }
   }

   return sRes;
}

/***********************************************************************************
**  FUNCTION:   setSysXtlCfg
**
**  PURPOSE:    Set external (main + extended) system configuration parameter.
**
**  PARAMETERS:
**              eParm   - Parameter ID.
**              rpCfg   - Configuration value.
**                        Memory ownership is taken by the function on success.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None.
***********************************************************************************/
static CFGSTATUS setSysXtlCfg( CCTKPARM eParm, void *&rpCfg )
{
   CCTK_TRACED2( "cfg - setSysXtlCfg(%d, %p)", eParm, rpCfg );

   /* Main system config is not allowed because it will be set through
   ** CCTK API instead.
   */
   if ( !CCTK_IS_EC_SYS( eParm ) )
   {
      CCTK_TRACEI0( "cfg - setSysXtlCfg: Invalid system config parameter." );
      return eCFGSTATUS_BADPARM;
   }

   if ( rpCfg == NULL )
   {
      CCTK_TRACEI0( "cfg - setSysXtlCfg: NULL config pointer." );
      return eCFGSTATUS_BADPARM;
   }

   switch ( eParm )
   {
   case eCCTKPARM_EC_LOCPORT:
      {
         CFM_CFG.pSys->xtl.net.gen.port = *static_cast< CCTKLOCPORT * >( rpCfg );
         DeletePtr((CCTKLOCPORT**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_TRANSPORT:
      {
         CFM_CFG.pSys->xtl.net.gen.transport =
            *static_cast< CCTKSIGTRANS * >( rpCfg );
         DeletePtr((CCTKSIGTRANS**) &rpCfg);
      }
      break;
#if CCTKCFG_TLS
   case eCCTKPARM_EC_TLSCFG:
      {
         clrTlsCfg( &( CFM_CFG.pSys->xtl.net.gen.tls ) );
         CFM_CFG.pSys->xtl.net.gen.tls = *static_cast< CCTKTLSCFG * >( rpCfg );
         DeletePtr((CCTKTLSCFG**) &rpCfg);
      }
      break;
#endif
   case eCCTKPARM_EC_CNXACTIVITY:
      {
         CFM_CFG.pSys->xtl.net.gen.cnx = *static_cast< CCTKCNXCFG * >( rpCfg );
         DeletePtr((CCTKCNXCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_UAIDHDR:
      {
         clrUaIdHdr( &( CFM_CFG.pSys->xtl.net.prot.gen.uaIdHdr ) );
         CFM_CFG.pSys->xtl.net.prot.gen.uaIdHdr =
            *static_cast< CCTKUAIDHDR * >( rpCfg );
         DeletePtr((CCTKUAIDHDR**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_ROUTINGHDR:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.routingHdr =
            *static_cast< CCTKROUTINGHDR * >( rpCfg );
         DeletePtr((CCTKROUTINGHDR**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_DNSMODE:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.eDnsMode =
            *static_cast< CCTKDNSMODE * >( rpCfg );
         DeletePtr((CCTKDNSMODE**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_SIGBEHAVE:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.sigBehave =
            *static_cast< CCTKSIGBEHAVE * >( rpCfg );
         DeletePtr((CCTKSIGBEHAVE**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_INTEROP:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.uInterop =
            *static_cast< BOS_UINT32 * >( rpCfg );
         DeletePtr((BOS_UINT32**) &rpCfg);
      }
      break;
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
   case eCCTKPARM_EC_IMSCFG:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.imsCfg =
            *static_cast< CCTKIMSCFG * >( rpCfg );
         DeletePtr((CCTKIMSCFG**) &rpCfg);
      }
      break;
#endif
   case eCCTKPARM_EC_GRUUCFG:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.gruu =
            *static_cast< CCTKGRUUCFG * >( rpCfg );
         DeletePtr((CCTKGRUUCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_PAUCFG:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.pau =
            *static_cast< CCTKPAUCFG * >( rpCfg );
         DeletePtr((CCTKPAUCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_PANICFG:
      {
         clrAccessNetworkInfoCfg( &( CFM_CFG.pSys->xtl.net.prot.gen.pani ) );
         CFM_CFG.pSys->xtl.net.prot.gen.pani =
            *static_cast< CCTKPANICFG * >( rpCfg );
         DeletePtr((CCTKPANICFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_TRANSTMR:
      {
         CFM_CFG.pSys->xtl.net.prot.tmr.trans =
            *static_cast< CCTKTRANSTMRCFG *>( rpCfg );
         DeletePtr((CCTKTRANSTMRCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_PKTLOG:
      {
         CFM_CFG.pSys->xtl.loc.debug.pktLog = *static_cast< CCTKPKTLOG *>( rpCfg );
         DeletePtr((CCTKPKTLOG**) &rpCfg);
      }
      break;
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   case eCCTKPARM_EC_TRACE:
      {
         CFM_CFG.pSys->xtl.loc.debug.trace = *static_cast< CCTKTRACE *>( rpCfg );
         DeletePtr((CCTKTRACE**) &rpCfg);
      }
      break;
#endif
   case eCCTKPARM_EC_NATGENCFG:
      {
         CFM_CFG.pSys->xtl.sec.nat.gen.cfg =
            *static_cast< CCTKNATGENCFG *>( rpCfg );
         DeletePtr((CCTKNATGENCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_NATRGCTL:
      {
         CFM_CFG.pSys->xtl.sec.nat.rg.ctl =
            *static_cast< CCTKRGNATCTL *>( rpCfg );
         DeletePtr((CCTKRGNATCTL**) &rpCfg);
      }
      break;
#if CCTKCFG_MEMORY_MONITORING
   case eCCTKPARM_EC_MEMPROTECT:
      {
         CFM_CFG.pSys->xtl.sec.dos.memProt =
            *static_cast< CCTKMEMPROTCFG *>( rpCfg );
         DeletePtr((CCTKMEMPROTCFG**) &rpCfg);
      }
      break;
#endif
#if CCTKCFG_REMOTE_USER_CONTROL
   case eCCTKPARM_EC_REMUSRSYS:
      {
         clrRemUsrSysCfg ( &(CFM_CFG.pSys->xtl.remUsr.gen) );
         CFM_CFG.pSys->xtl.remUsr.gen =
            *static_cast< CCTKREMUSRSYS *>( rpCfg );
         DeletePtr((CCTKREMUSRSYS**) &rpCfg);
      }
      break;
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
#if CCTKCFG_ICE_LITE_SUPPORT
   case eCCTKPARM_EC_ICECTLCFG:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.ice =
            *static_cast< CCTKICECTLCFG *>( rpCfg );
         DeletePtr((CCTKICECTLCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_ICEIPLSTCFG:
      {
         CFM_CFG.pSys->xtl.net.prot.gen.iceAddrLst =
            *static_cast< CCTKIPADDRLST *>( rpCfg );
         DeletePtr((CCTKIPADDRLST**) &rpCfg);
      }
      break;
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
   default:
      {
         CCTK_TRACEI0(
            "cfg - setSysXtlCfg: Parameter is not a system config parameter." );
         return eCFGSTATUS_BADPARM;
      }
      break;
   }

   /* Update internal system configuration */
   updSysIntCfg();

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   setUsrXtlCfg
**
**  PURPOSE:    Set external (main + extended) user configuration parameter.
**
**  PARAMETERS:
**              uIdx    - User config array index.
**              eParm   - Parameter ID.
**              rpCfg   - Configuration value.
**                        Memory ownership is taken by the function on success.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None.
***********************************************************************************/
static CFGSTATUS setUsrXtlCfg( BOS_UINT32 uIdx, CCTKPARM eParm, void *&rpCfg )
{
   CCTK_TRACED3( "cfg - setUsrXtlCfg(%lu, %d, %p)", uIdx, eParm, rpCfg );

   if ( !CCTK_IS_USRPARM( eParm ) )
   {
      CCTK_TRACEI0( "cfg - setUsrXtlCfg: Invalid user config parameter." );
      return eCFGSTATUS_BADPARM;
   }

   if ( uIdx >= CFG_MAX_USR || CFM_CFG.pUsr[uIdx]->uGuid == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - setUsrXtlCfg: Invalid user array index." );
      return eCFGSTATUS_BADPARM;
   }

   if ( rpCfg == NULL )
   {
      CCTK_TRACEI0( "cfg - setUsrXtlCfg: NULL config pointer." );
      return eCFGSTATUS_BADPARM;
   }

   switch ( eParm )
   {
   case eCCTKPARM_MC_SIPNAME:
      {
         clrSipName( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.sipName ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.sipName =
            *static_cast< CCTKSIPNAME * >( rpCfg );
         DeletePtr((CCTKSIPNAME**) &rpCfg);
      }
      break;
   case eCCTKPARM_MC_OBPROXYLST:
      {
         clrRouteLst( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.obProxyLst ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.obProxyLst =
            *static_cast< CCTKROUTELST * >( rpCfg );
         DeletePtr((CCTKROUTELST**) &rpCfg);
      }
      break;
   case eCCTKPARM_MC_MWISVRADDR:
      {
         clrSipUsrHost( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.mwi.gen.server ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.mwi.gen.server =
            *static_cast< CCTKSIPUSRHOST * >( rpCfg );
         DeletePtr((CCTKSIPUSRHOST**) &rpCfg);
      }
      break;
   case eCCTKPARM_MC_MEDIACFG:
      {
         CCTKMEDCFG *pMediaCfg =
            reinterpret_cast< CCTKMEDCFG * >
               ( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.media ) );
         clrMediaCfg( pMediaCfg );
         *pMediaCfg = *static_cast< CCTKMEDCFG * >( rpCfg );
         DeletePtr((CCTKMEDCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_MC_MEDIALIST:
      {
         BRCM_DELETE( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.media.pMediaLst );
         CFM_VARDUP( static_cast< CCTKMEDLST * >( rpCfg ),
                     CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.media.pMediaLst,
                     CCTKMEDLST );
         DeletePtr((CCTKMEDLST**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_SUBEVT:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.loc.uSubEvt =
            *static_cast< BOS_UINT32 * >( rpCfg );
         DeletePtr((BOS_UINT32**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_GENHDRCFG:
      {
         clrGenHdr( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.loc.genHdr ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.loc.genHdr =
            *static_cast< CCTKGENHDRCFG * >( rpCfg );
         DeletePtr((CCTKGENHDRCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_USRPORTCFG:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.portCfg =
            *static_cast< CCTKUSRPORTCFG * >( rpCfg );
         DeletePtr((CCTKUSRPORTCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_DIAGEVTCFG:
      {
         clrDiagEvtCfg( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.diagEvt ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.diagEvt =
            *static_cast< CCTKDIAGEVTCFG * >( rpCfg );
         DeletePtr((CCTKDIAGEVTCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_UAPEVTCFG:
      {
         clrUapEvtCfg( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.uapEvt ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.uapEvt =
            *static_cast< CCTKUAPEVTCFG * >( rpCfg );
         DeletePtr((CCTKUAPEVTCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_USREMGCFG:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.emgCfg =
            *static_cast< CCTKUSREMGCFG * >( rpCfg );
         DeletePtr((CCTKUSREMGCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_USRBRCFG:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.brCfg =
            *static_cast< CCTKUSRBRCFG * >( rpCfg );
         DeletePtr((CCTKUSRBRCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_SVCROUTE:
      {
         clrSvcRoute( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.svcRoute ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.svcRoute =
            *static_cast< CCTKSVCROUTE * >( rpCfg );
         DeletePtr((CCTKSVCROUTE**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_USRCONTACT:
      {
         clrContact( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.contact ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.net.contact =
            *static_cast< CCTKCONTACT * >( rpCfg );
         DeletePtr((CCTKCONTACT**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_AUTHCFG:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.sec.auth.cfg =
            *static_cast< CCTKAUTHCFG * >( rpCfg );
         DeletePtr((CCTKAUTHCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_WHTLST:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.sec.dos.whtLst =
            *static_cast< CCTKWHTLSTCFG * >( rpCfg );
         DeletePtr((CCTKWHTLSTCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_BLKLST:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.usr.sec.dos.blkLst =
            *static_cast< CCTKIPADDRLST * >( rpCfg );
         DeletePtr((CCTKIPADDRLST**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_REGTMR:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.reg.gen.tmr =
            *static_cast< CCTKREGTMR * >( rpCfg );
         DeletePtr((CCTKREGTMR**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_REGEVTCFG:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.reg.gen.regEvt =
            *static_cast< CCTKREGEVTCFG * >( rpCfg );
         DeletePtr((CCTKREGEVTCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_MWICFG:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.mwi.gen.cfg =
            *static_cast< CCTKMWICFG * >( rpCfg );
         DeletePtr((CCTKMWICFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_MWITMR:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.mwi.gen.tmr =
            *static_cast< CCTKMWITMR * >( rpCfg );
         DeletePtr((CCTKMWITMR**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_EMGCALLCFG:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.gen.emgCallCfg =
            *static_cast< CCTKEMGCALLCFG * >( rpCfg );
         DeletePtr((CCTKEMGCALLCFG**) &rpCfg);
      }
      break;
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
   case eCCTKPARM_EC_TELCALL:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.gen.tel =
            *static_cast< CCTKTELCALL * >( rpCfg );
         DeletePtr((CCTKTELCALL**) &rpCfg);
      }
      break;
#endif
   case eCCTKPARM_EC_PRACK:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.adv.ePrack =
            *static_cast< CCTKPRACK * >( rpCfg );
         DeletePtr((CCTKPRACK**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_SESTMR:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.adv.sesTmr =
            *static_cast< CCTKSESTMR * >( rpCfg );
         DeletePtr((CCTKSESTMR**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CALLPRIV:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.adv.priv =
            *static_cast< CCTKPRIVCFG * >( rpCfg );
         DeletePtr((CCTKPRIVCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CALLNAI:
      {
         clrNaiCfg( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.adv.nai ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.adv.nai =
            *static_cast< CCTKNAICFG * >( rpCfg );
         DeletePtr((CCTKNAICFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CONNECTEDID:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.basic.adv.eConnectedId =
            *static_cast< CCTKCONNECTEDID * >( rpCfg );
         DeletePtr((CCTKCONNECTEDID**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CALLHOLD:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.supp.hold =
            *static_cast< CCTKHOLDCFG * >( rpCfg );
         DeletePtr((CCTKHOLDCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CALLXFER:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.supp.xfer =
            *static_cast< CCTKXFERCFG * >( rpCfg );
         DeletePtr((CCTKXFERCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_MEDNEGCFG:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.call.media.neg =
            *static_cast< CCTKMEDNEGCFG * >( rpCfg );
         DeletePtr((CCTKMEDNEGCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_EVTTMR:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.evt.gen.tmr =
            *static_cast< CCTKEVTTMR * >( rpCfg );
         DeletePtr((CCTKEVTTMR**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_EVTPRIV:
      {
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.evt.adv.priv =
            *static_cast< CCTKPRIVCFG * >( rpCfg );
         DeletePtr((CCTKPRIVCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_EVTNAI:
      {
         clrNaiCfg( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.evt.adv.nai ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.dft.evt.adv.nai =
            *static_cast< CCTKNAICFG * >( rpCfg );
         DeletePtr((CCTKNAICFG**) &rpCfg);
      }
      break;
#if CCTKCFG_REMOTE_USER_CONTROL
   case eCCTKPARM_EC_REMUSRCFG:
      {
         clrRemUsrCfg ( &( CFM_CFG.pUsr[uIdx]->cfg.xtl.remUsr.cfg ) );
         CFM_CFG.pUsr[uIdx]->cfg.xtl.remUsr.cfg =
            *static_cast< CCTKREMUSRCFG *>( rpCfg );
         DeletePtr((CCTKREMUSRCFG**) &rpCfg);
      }
      break;
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

   default:
      {
         CCTK_TRACEI0(
            "cfg - setUsrXtlCfg: Parameter is not a user config parameter." );
         return eCFGSTATUS_BADPARM;
      }
      break;
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   setCallXtlCfg
**
**  PURPOSE:    Set external (main + extended) call configuration parameter.
**
**  PARAMETERS:
**              uIdx    - Call config array index.
**              eParm   - Parameter ID.
**              rpCfg   - Configuration value.
**                        Memory ownership is taken by the function on success.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None.
***********************************************************************************/
static CFGSTATUS setCallXtlCfg( BOS_UINT32 uIdx, CCTKPARM eParm, void *&rpCfg )
{
   CCTK_TRACED3( "cfg - setCallXtlCfg(%lu, %d, %p)", uIdx, eParm, rpCfg );

   if ( !CCTK_IS_CALLPARM( eParm ) )
   {
      CCTK_TRACEI0( "cfg - setCallXtlCfg: Invalid call config parameter." );
      return eCFGSTATUS_BADPARM;
   }

   if ( uIdx >= CCTKCFG_MAX_CALL || CFM_CFG.pCall[uIdx]->uGuid == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - setCallXtlCfg: Invalid call array index." );
      return eCFGSTATUS_BADPARM;
   }

   if ( rpCfg == NULL )
   {
      CCTK_TRACEI0( "cfg - setCallXtlCfg: NULL config pointer." );
      return eCFGSTATUS_BADPARM;
   }

   switch ( eParm )
   {
   case eCCTKPARM_MC_OBPROXYLST:
      {
         delRouteLst( CFM_CFG.pCall[uIdx]->cfg.xtl.usr.pObProxyLst );
         CFM_VARDUP( static_cast< CCTKROUTELST * >( rpCfg ),
                     CFM_CFG.pCall[uIdx]->cfg.xtl.usr.pObProxyLst,
                     CCTKROUTELST );
         DeletePtr((CCTKROUTELST**) &rpCfg);
      }
      break;
   case eCCTKPARM_MC_MEDIACFG:
      {
         CCTKMEDCFG *pMediaCfg = reinterpret_cast< CCTKMEDCFG * >
                                    ( &( CFM_CFG.pCall[uIdx]->cfg.xtl.media ) );
         clrMediaCfg( pMediaCfg );
         *pMediaCfg = *static_cast< CCTKMEDCFG * >( rpCfg );
         DeletePtr((CCTKMEDCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_MC_MEDIALIST:
      {
         BRCM_DELETE( CFM_CFG.pCall[uIdx]->cfg.xtl.media.pMediaLst );
         CFM_VARDUP( static_cast< const CCTKMEDLST * >( rpCfg ),
                     CFM_CFG.pCall[uIdx]->cfg.xtl.media.pMediaLst,
                     CCTKMEDLST );
         DeletePtr((CCTKMEDLST**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_GENHDRCFG:
      {
         delGenHdr( CFM_CFG.pCall[uIdx]->cfg.xtl.usr.pGenHdr );
         CFM_VARDUP( static_cast< CCTKGENHDRCFG * >( rpCfg ),
                     CFM_CFG.pCall[uIdx]->cfg.xtl.usr.pGenHdr,
                     CCTKGENHDRCFG );
         DeletePtr((CCTKGENHDRCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_EMGCALLCFG:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.basic.gen.emgCallCfg =
            *static_cast< CCTKEMGCALLCFG * >( rpCfg );
         DeletePtr((CCTKEMGCALLCFG**) &rpCfg);
      }
      break;
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
   case eCCTKPARM_EC_TELCALL:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.basic.gen.tel =
            *static_cast< CCTKTELCALL * >( rpCfg );
         DeletePtr((CCTKTELCALL**) &rpCfg);
      }
      break;
#endif
   case eCCTKPARM_EC_PRACK:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.basic.adv.ePrack =
            *static_cast< CCTKPRACK * >( rpCfg );
         DeletePtr((CCTKPRACK**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_SESTMR:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.basic.adv.sesTmr =
            *static_cast< CCTKSESTMR * >( rpCfg );
         DeletePtr((CCTKSESTMR**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CALLPRIV:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.basic.adv.priv =
            *static_cast< CCTKPRIVCFG * >( rpCfg );
         DeletePtr((CCTKPRIVCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CALLNAI:
      {
         clrNaiCfg( &( CFM_CFG.pCall[uIdx]->cfg.xtl.basic.adv.nai ) );
         CFM_CFG.pCall[uIdx]->cfg.xtl.basic.adv.nai =
            *static_cast< CCTKNAICFG * >( rpCfg );
         DeletePtr((CCTKNAICFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CONNECTEDID:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.basic.adv.eConnectedId =
            *static_cast< CCTKCONNECTEDID * >( rpCfg );
         DeletePtr((CCTKCONNECTEDID**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CALLHOLD:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.supp.hold =
            *static_cast< CCTKHOLDCFG * >( rpCfg );
         DeletePtr((CCTKHOLDCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_CALLXFER:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.supp.xfer =
            *static_cast< CCTKXFERCFG * >( rpCfg );
         DeletePtr((CCTKXFERCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_MEDNEGCFG:
      {
         CFM_CFG.pCall[uIdx]->cfg.xtl.media.neg =
            *static_cast< CCTKMEDNEGCFG * >( rpCfg );
         DeletePtr((CCTKMEDNEGCFG**) &rpCfg);
      }
      break;
   default:
      {
         CCTK_TRACEI0(
            "cfg - setCallXtlCfg: Parameter is not a call config parameter." );
         return eCFGSTATUS_BADPARM;
      }
      break;
   }

   return eCFGSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   setEvtXtlCfg
**
**  PURPOSE:    Set external (main + extended) event subscription configuration
**              parameter.
**
**  PARAMETERS:
**              uIdx    - Event subscription config array index.
**              eParm   - Parameter ID.
**              rpCfg   - Configuration value.
**                        Memory ownership is taken by the function on success.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None.
***********************************************************************************/
static CFGSTATUS setEvtXtlCfg( BOS_UINT32 uIdx, CCTKPARM eParm, void *&rpCfg )
{
   CCTK_TRACED3( "cfg - setEvtXtlCfg(%lu, %d, %p)", uIdx, eParm, rpCfg );

   if ( !CCTK_IS_EVTPARM( eParm ) )
   {
      CCTK_TRACEI0(
         "cfg - setEvtXtlCfg: Invalid event subscription config parameter." );
      return eCFGSTATUS_BADPARM;
   }

   if ( uIdx >= CCTKCFG_MAX_EVT || CFM_CFG.pEvt[uIdx]->uGuid == guCeInvalidGuid )
   {
      CCTK_TRACEI0( "cfg - setEvtXtlCfg: Invalid event subscription array index." );
      return eCFGSTATUS_BADPARM;
   }

   if ( rpCfg == NULL )
   {
      CCTK_TRACEI0( "cfg - setEvtXtlCfg: NULL config pointer." );
      return eCFGSTATUS_BADPARM;
   }

   BOS_BOOL bDelCfg = BOS_TRUE;

   switch ( eParm )
   {
   case eCCTKPARM_MC_OBPROXYLST:
      {
         delRouteLst( CFM_CFG.pEvt[uIdx]->cfg.xtl.usr.pObProxyLst );
         CFM_VARDUP( static_cast< CCTKROUTELST * >( rpCfg ),
                     CFM_CFG.pEvt[uIdx]->cfg.xtl.usr.pObProxyLst,
                     CCTKROUTELST );
         DeletePtr((CCTKROUTELST**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_GENHDRCFG:
      {
         delGenHdr( CFM_CFG.pEvt[uIdx]->cfg.xtl.usr.pGenHdr );
         CFM_VARDUP( static_cast< CCTKGENHDRCFG * >( rpCfg ),
                     CFM_CFG.pEvt[uIdx]->cfg.xtl.usr.pGenHdr,
                     CCTKGENHDRCFG );
         DeletePtr((CCTKGENHDRCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_EVTTMR:
      {
         CFM_CFG.pEvt[uIdx]->cfg.xtl.gen.tmr =
            *static_cast< CCTKEVTTMR * >( rpCfg );
         DeletePtr((CCTKEVTTMR**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_EVTPRIV:
      {
         CFM_CFG.pEvt[uIdx]->cfg.xtl.adv.priv =
            *static_cast< CCTKPRIVCFG * >( rpCfg );
         DeletePtr((CCTKPRIVCFG**) &rpCfg);
      }
      break;
   case eCCTKPARM_EC_EVTNAI:
      {
         clrNaiCfg( &( CFM_CFG.pEvt[uIdx]->cfg.xtl.adv.nai ) );
         CFM_CFG.pEvt[uIdx]->cfg.xtl.adv.nai =
            *static_cast< CCTKNAICFG * >( rpCfg );
         DeletePtr((CCTKNAICFG**) &rpCfg);
      }
      break;
   default:
      {
         CCTK_TRACEI0(
            "cfg - setEvtXtlCfg: Event subscription parameter not valid." );
         return eCFGSTATUS_BADPARM;
      }
      break;
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   updSysIntCfg
**
**  PURPOSE:    Update internal system configuration based on the existing config.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Return status
**
**  NOTES:      None.
***********************************************************************************/
static void updSysIntCfg( void )
{
   CCTK_TRACED0( "cfg - updSysIntCfg()" );

#if CCTKCFG_TLS
   CFM_CFG.pSys->in.bTlsEnabled =
      UTIL_BMP_IS_ENABLED_ALL( CFM_CFG.pSys->xtl.net.gen.transport.uSigTrans,
                               CCTK_TRANSPORT_TLS ) &&
      CFM_CFG.pSys->xtl.net.gen.tls.pcPrivKey != NULL &&
      CFM_CFG.pSys->xtl.net.gen.tls.ppcCertChain != NULL &&
      *CFM_CFG.pSys->xtl.net.gen.tls.ppcCertChain != NULL &&
      CFM_CFG.pSys->xtl.net.gen.tls.ppcTrustedCerts != NULL &&
      *CFM_CFG.pSys->xtl.net.gen.tls.ppcTrustedCerts != NULL;
#else
   CFM_CFG.pSys->in.bTlsEnabled = BOS_FALSE;
#endif
}

/***********************************************************************************
**  FUNCTION:   isSysCfgValid
**
**  PURPOSE:    Validate the main system configuration.
**
**  PARAMETERS:
**              pCfg - Main system configuration block.
**
**
**  RETURNS:    Return true if the config block is valid.
**
**  NOTES:      None.
***********************************************************************************/
static BOS_BOOL isSysCfgValid( const CCTKCFG *pCfg )
{
   CCTK_TRACED1( "cfg - isSysCfgValid(%p)", pCfg );

   BOS_UINT32 uIdx;

   CFM_CHK( pCfg == NULL,
            "cfg - isSysCfgValid: NULL config pointer." );
   CFM_CHK( pCfg->loc.gen.pcSeed == NULL || pCfg->loc.gen.pcSeed[0] == '\0',
            "cfg - isSysCfgValid: Random seed is required." );
   CFM_CHK( pCfg->net.gen.host.uNum > CCTKCFG_MAX_HOSTADDR,
            "cfg - isSysCfgValid: Num of host addresses exceeds." );
   for ( uIdx = 0; uIdx < pCfg->net.gen.host.uNum; uIdx++ )
   {
      CFM_CHK( pCfg->net.gen.host.addr[uIdx].access.uNum > CCTKCFG_MAX_ACCESSNET,
               "cfg - isSysCfgValid: Num of network access list exceeds." );
   }

   return BOS_TRUE;
}

/***********************************************************************************
**  FUNCTION:   isUsrCfgValid
**
**  PURPOSE:    Validate the main user configuration.
**
**  PARAMETERS:
**              pCfg - Main user configuration block.
**
**
**  RETURNS:    Return true if the config block is valid.
**
**  NOTES:      None.
***********************************************************************************/
static BOS_BOOL isUsrCfgValid( const CCTKUSRCFG *pCfg )
{
   CCTK_TRACED1( "cfg - isUsrCfgValid(%p)", pCfg );

   BOS_UINT32 uIdx;

   CFM_CHK( pCfg == NULL,
            "cfg - isUsrCfgValid: NULL config pointer." );
   CFM_CHK( pCfg->usr.sec.auth.client.rlmlst.uNum >= CCTKCFG_MAX_REALM,
            "cfg - isUsrCfgValid: Num of realm list exceeds." );
   for ( uIdx = 0; uIdx < pCfg->usr.sec.auth.client.rlmlst.uNum; uIdx++ )
   {
      CFM_CHK( pCfg->usr.sec.auth.client.rlmlst.rlm[uIdx].pcPwd == NULL,
               "cfg - isUsrCfgValid: Password must be filled in for realm." );
      CFM_CHK( pCfg->usr.sec.auth.client.rlmlst.rlm[uIdx].pcRlm == NULL,
               "cfg - isUsrCfgValid: Realm domain must be filled in for realm." );
      CFM_CHK( pCfg->usr.sec.auth.client.rlmlst.rlm[uIdx].pcUsrName == NULL,
               "cfg - isUsrCfgValid: User name must be filled in for realm." );
   }
   if ( cfgIsSipHostEnabled( pCfg->usr.sec.nat.kaCfg.server ) )
   {
      CFM_CHK( pCfg->usr.sec.nat.kaCfg.uTmr == 0,
               "cfg - isUsrCfgValid: Keep-alive timer must be non-zero." );
   }
   if ( pCfg->usr.net.bSecOnly )
   {
      if ( cfgIsSipHostEnabled( pCfg->dft.call.supp.mohServer ) )
      {
         CFM_CHK( !pCfg->dft.call.supp.mohServer.bSec,
                  "cfg - isUsrCfgValid: MOH server must be secured." );
      }
      if ( cfgIsSipHostEnabled( pCfg->mwi.gen.server.sipHost ) )
      {
         CFM_CHK( !pCfg->mwi.gen.server.sipHost.bSec,
                  "cfg - isUsrCfgValid: MWI server must be secured." );
      }
      if ( cfgIsSipHostEnabled( pCfg->reg.gen.registrar ) )
      {
         CFM_CHK( !pCfg->reg.gen.registrar.bSec,
                  "cfg - isUsrCfgValid: Registrar must be secured." );
      }
      if ( cfgIsSipHostEnabled( pCfg->usr.net.obProxyLst.route[0].addr ) )
      {
         CFM_CHK( !pCfg->usr.net.obProxyLst.route[0].addr.bSec,
                  "cfg - isUsrCfgValid: First outbound proxy must be secured." );
      }
   }

   return BOS_TRUE;
}

/***********************************************************************************
**  FUNCTION:   isXtlCfgValid
**
**  PURPOSE:    Validate external configuration (main + extended config).
**
**  PARAMETERS:
**              pCfg - External configuration block.
**
**
**  RETURNS:    Return true if the config block is valid.
**
**  NOTES:      None.
***********************************************************************************/
static BOS_BOOL isXtlCfgValid( CCTKPARM eParm, const void *pCfg )
{
   CCTK_TRACED2( "cfg - isXtlCfgValid(%d, %p)", eParm, pCfg );

   CFM_CHK( !CCTK_IS_CFGPARM( eParm ),
            "cfg - isXtlCfgValid: Invalid external config parameter." );
   CFM_CHK( pCfg == NULL,
            "cfg - isXtlCfgValid: NULL config pointer." );

   BOS_UINT32 uIdx;
   switch ( eParm )
   {
   case eCCTKPARM_MC_MEDIACFG:
      {
         const CCTKMEDCFG *pMainCfg = static_cast< const CCTKMEDCFG* >( pCfg );
         CFM_CHK( pMainCfg->pAudio != NULL &&
                  pMainCfg->pAudio->gen.uNumCodec > CCTKCFG_MAX_AUDIO_CODEC,
                  "cfg - isXtlCfgValid: Invalid media config." );
#if CCTKCFG_VIDEO
         CFM_CHK( pMainCfg->pVideo != NULL &&
                  pMainCfg->pVideo->gen.uNumCodec > CCTKCFG_MAX_VIDEO_CODEC,
                  "cfg - isXtlCfgValid: Invalid media config." );
#endif
#if CCTKCFG_VIDEO
         CFM_CHK( pMainCfg->pMediaLst != NULL &&
                  ( pMainCfg->pMediaLst->mMediaTypes == 0 ||
                    pMainCfg->pMediaLst->uNumAudCodec > CCTKCFG_MAX_AUDIO_CODEC ||
                    pMainCfg->pMediaLst->uNumVidCodec > CCTKCFG_MAX_VIDEO_CODEC ),
                  "cfg - isXtlCfgValid: Invalid media config." );
#else
         CFM_CHK( pMainCfg->pMediaLst != NULL &&
                  ( pMainCfg->pMediaLst->mMediaTypes == 0 ||
                    pMainCfg->pMediaLst->uNumAudCodec > CCTKCFG_MAX_AUDIO_CODEC ),
                  "cfg - isXtlCfgValid: Invalid media config." );
#endif
#if CCTKCFG_SRTP
         CFM_CHK( pMainCfg->pAudio != NULL &&
                  pMainCfg->pAudio->sec.eSrtpPref != eCCTKSRTPPREF_DISABLED &&
                  pMainCfg->pAudio->sec.cryptoLst.uNumCrypto == 0,
                  "cfg - isXtlCfgValid: Invalid media config." );
#endif
      }
      break;
   case eCCTKPARM_MC_MEDIALIST:
      {
         const CCTKMEDLST *pMainCfg = static_cast< const CCTKMEDLST* >( pCfg );
         CFM_CHK( pMainCfg->mMediaTypes == 0,
                  "cfg - isXtlCfgValid: Invalid media list config." );
      }
      break;
   case eCCTKPARM_EC_SUBEVT:
      {
         const BOS_UINT32 *pExtCfg = static_cast< const BOS_UINT32* >( pCfg );
         CFM_CHK( ( *pExtCfg | CCTK_SUBEVT_ALL ) != CCTK_SUBEVT_ALL,
                  "cfg - isXtlCfgValid: Invalid event subscription config." );
      }
      break;
   case eCCTKPARM_EC_GENHDRCFG:
      {
         const CCTKGENHDRCFG *pExtCfg =
            static_cast< const CCTKGENHDRCFG* >( pCfg );
         CFM_CHK( pExtCfg->uNum >= CCTKCFG_MAX_GENHDRLST,
                  "cfg - isXtlCfgValid: Generic header list exceeds maximum." );
         for ( uIdx = 0; uIdx < pExtCfg->uNum; uIdx++ )
         {
            CFM_CHK( !cfgIsHdrValid( pExtCfg->hdr[uIdx].hdr ),
                     "cfg - isXtlCfgValid: Generic header is invalid." );
            CFM_CHK(
               pExtCfg->hdr[uIdx].hdr.eType == eCCTKHDRTYPE_PRIVATE &&
               strCmpNoCaseSize(
                  reinterpret_cast< const char* >( pExtCfg->hdr[uIdx].hdr.pcName ),
                  "P-",
                  2 ) != 0,
               "cfg - isXtlCfgValid: Private header must begin with 'P-'." );
            CFM_CHK( pExtCfg->hdr[uIdx].uMethod == CCTK_METHOD_NONE,
                     "cfg - isXtlCfgValid: "
                     "Generic header method bitmap cannot be none." );
         }
      }
      break;
   case eCCTKPARM_EC_WHTLST:
      {
         const CCTKWHTLSTCFG *pExtCfg = static_cast< const CCTKWHTLSTCFG* >( pCfg );
         CFM_CHK( pExtCfg->lst.uNum > CCTKCFG_MAX_ADDRLST,
                  "cfg - isXtlCfgValid: White-list exceeds maximum." );
      }
      break;
   case eCCTKPARM_EC_BLKLST:
      {
         const CCTKIPADDRLST *pExtCfg = static_cast< const CCTKIPADDRLST* >( pCfg );
         CFM_CHK( pExtCfg->uNum > CCTKCFG_MAX_ADDRLST,
                  "cfg - isXtlCfgValid: Black-list exceeds maximum." );
      }
      break;
   case eCCTKPARM_EC_CALLHOLD:
      {
         const CCTKHOLDCFG *pExtCfg =
            static_cast< const CCTKHOLDCFG* >( pCfg );
         CFM_CHK( pExtCfg->bLocalHold && !pExtCfg->bUseInactive,
                  "cfg - isXtlCfgValid:"
                  " Inactive mode must be used with local hold mode." );
      }
      break;
   case eCCTKPARM_EC_TRANSPORT:
      {
         const CCTKSIGTRANS *pExtCfg =
            static_cast< const CCTKSIGTRANS* >( pCfg );
         CFM_CHK( ( pExtCfg->uSigTrans | CCTK_TRANSPORT_ALL ) != CCTK_TRANSPORT_ALL,
                    "cfg - isXtlCfgValid: Invalid transport control bitmap." );
      }
      break;
   case eCCTKPARM_EC_CNXACTIVITY:
      {
         const CCTKCNXCFG *pExtCfg = static_cast< const CCTKCNXCFG* >( pCfg );
         CFM_CHK( pExtCfg->uMaxCnx < pExtCfg->uMinCnx,
                  "cfg - isXtlCfgValid: Max num of connections cannot be < min." );
      }
      break;
   case eCCTKPARM_EC_UAIDHDR:
      {
         const CCTKUAIDHDR *pExtCfg = static_cast< const CCTKUAIDHDR* >( pCfg );
         CFM_CHK( ( pExtCfg->pcSipInstId != NULL ) &&
                  strCmpNoCaseSize(
                     reinterpret_cast< const char* >( pExtCfg->pcSipInstId ),
                     "urn:",
                     4 ) != 0,
                  "cfg - isXtlCfgValid: Instance ID must begin with 'urn:'." );
      }
      break;
   case eCCTKPARM_EC_PKTLOG:
      {
         const CCTKPKTLOG *pExtCfg = static_cast< const CCTKPKTLOG* >( pCfg );
         CFM_CHK( !bosIpAddrIsZero( &( pExtCfg->ipPort.addr ) ) &&
                  pExtCfg->ipPort.uPort == 0,
                  "cfg - isXtlCfgValid: Packet logger port needs to be specified" );
      }
      break;
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   case eCCTKPARM_EC_TRACE:
      {
         const CCTKTRACE *pExtCfg = static_cast< const CCTKTRACE* >( pCfg );
         CFM_CHK( pExtCfg->pHandler != NULL &&
                  ( ( pExtCfg->uGroup | CCTK_TRACEGRP_ALL ) != CCTK_TRACEGRP_ALL ||
                    ( pExtCfg->uLevel | CCTK_TRACELVL_ALL ) != CCTK_TRACELVL_ALL ),
                  "cfg - isXtlCfgValid: Invalid trace bitmap settings." );
      }
      break;
#endif
#if CCTKCFG_ICE_LITE_SUPPORT
   case eCCTKPARM_EC_ICEIPLSTCFG:
      {
         const CCTKIPADDRLST *pExtCfg = static_cast< const CCTKIPADDRLST* >( pCfg );
         CFM_CHK( pExtCfg->uNum > CCTKCFG_MAX_ADDRLST,
                  "cfg - isXtlCfgValid: ICE IP Address-list exceeds maximum." );
      }
      break;
#endif
   default:
      {
         /* No checking required for other external config parameters */
      }
      break;
   }

   return BOS_TRUE;
}

/***********************************************************************************
**  FUNCTION:   isXtlUsrCfgValid
**
**  PURPOSE:    Validate external user configuration.
**
**  PARAMETERS:
**              uGuid   - GUID.
**              eParm   - Parameter ID.
**              pCfg    - External config block.
**
**
**  RETURNS:    Return true if the config block is valid.
**
**  NOTES:      None.
***********************************************************************************/
static BOS_BOOL isXtlUsrCfgValid( BOS_UINT32 uGuid,
                                  CCTKPARM eParm,
                                  const void *pCfg )
{
   CCTK_TRACED3( "cfg - isXtlUsrCfgValid(0x%lX, %d, %p)", uGuid, eParm, pCfg );

   switch ( eParm )
   {
   case eCCTKPARM_EC_CALLPRIV:
      {
         const CCTKPRIVCFG *pExtCfg = static_cast< const CCTKPRIVCFG* >( pCfg );

         /* Ensure that we have a preloaded route defined when network level
         ** privacy is used. The first preloaded route is assumed be the privacy
         ** service provider.
         */
         if ( pExtCfg->eLevel == eCCTKPRIVLVL_NETID ||
              pExtCfg->eLevel == eCCTKPRIVLVL_NETHDR ||
              pExtCfg->eLevel == eCCTKPRIVLVL_NETSES ||
              pExtCfg->eLevel == eCCTKPRIVLVL_ALL )
         {
            const CFGUSR *pCfgUsr = CFG_USR( uGuid );
            CFM_CHK( pCfgUsr == NULL,
                     "cfg - isXtlUsrCfgValid: User config block not available" );
            const CCTKROUTELST *pObProxyLst = &pCfgUsr->xtl.usr.net.obProxyLst;
            CFM_CHK( pObProxyLst->route[0].addr.hostPort.pcAddr == NULL,
                     "cfg - isXtlUsrCfgValid: Outbound proxy not configured" );
         }
      }
      break;
   default:
      {
         /* No checking required for other external config parameters */
      }
      break;
   }

   return BOS_TRUE;
}
