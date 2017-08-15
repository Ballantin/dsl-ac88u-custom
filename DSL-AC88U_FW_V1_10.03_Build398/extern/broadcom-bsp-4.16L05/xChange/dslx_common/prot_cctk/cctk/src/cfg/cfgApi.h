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
**      This file contains the Configuration Framework API.
**
***********************************************************************************/

#ifndef __CFG_API__H__INCLUDED__
#define __CFG_API__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Constants and Types ----------------------------------------------------- */
/* Maximum number of user resources available for the Configuration Framework */
#if CCTKCFG_PROXY_FAILOVER
/* (1) Main user config
** (2) User config associated with the backup user (for the backup UASSP which
**     attempts to re-establish a connection with the primary target).
**     NOTE: This backup user from the point of view of CE (and CCTK) is an internal
**     user, hence it is not counted as part of the CCTKCFG_MAX_USR which is the
**     'external' view of the user support, rather the backup user is internally
**     created when necessary and associated with the user.
** (3) 'Permanent' duplicate of the original user config which is kept around in
**     order to be able to always go back to the very initial user configuration in
**     case it gets overwritten by other process (since during registration either
**     of the main user config or backup user config may end up being changed due to
**     network signaling content).
*/
#define CFG_MAX_USR                 ( 3 * (CCTKCFG_MAX_USR + CCTKCFG_MAX_VIRTUAL_USR) )
#else
#define CFG_MAX_USR                 ( CCTKCFG_MAX_USR + CCTKCFG_MAX_VIRTUAL_USR )
#endif

/* Config retrieving macros */
#define CFG_SYS                                                                    \
   ( static_cast< const CFGSYS * > ( cfgGet( eCFGTYPE_SYS,   guCeInvalidGuid ) ) )
#define CFG_USR( uGuid )                                                           \
   ( static_cast< const CFGUSR * > ( cfgGet( eCFGTYPE_USR,   uGuid ) ) )
#define CFG_CALL( uGuid )                                                          \
   ( static_cast< const CFGCALL * >( cfgGet( eCFGTYPE_CALL,  uGuid ) ) )
#define CFG_EVT( uGuid )                                                           \
   ( static_cast< const CFGEVT * > ( cfgGet( eCFGTYPE_EVT,   uGuid ) ) )

/* Return status macros */
#define CFGSTATUS_IS_SUCCESS( eStatus )                                            \
                                    ( eStatus >= eCFGSTATUS_SUCCESS_START &&       \
                                      eStatus <= eCFGSTATUS_SUCCESS_END )
#define CFGSTATUS_IS_FAIL( eStatus )                                               \
                                    ( eStatus >= eCFGSTATUS_FAIL_START &&          \
                                      eStatus <= eCFGSTATUS_FAIL_END )

/* Return status */
typedef enum CFGSTATUS
{
   /* Successful return status */
   eCFGSTATUS_SUCCESS_START,
                                       /* Success */
   eCFGSTATUS_SUCCESS = eCFGSTATUS_SUCCESS_START,
   eCFGSTATUS_SUCCESS_END = eCFGSTATUS_SUCCESS,

   /* Failure return status */
   eCFGSTATUS_FAIL_START,
                                       /* Bad state */
   eCFGSTATUS_BADSTATE = eCFGSTATUS_FAIL_START,
   eCFGSTATUS_BADPARM,                 /* Bad parameter(s) */
   eCFGSTATUS_BADEXTCFG,               /* Bad extended config */
   eCFGSTATUS_NOTCMPL,                 /* Not complete yet */
   eCFGSTATUS_NORSC,                   /* Not enough resource */
   eCFGSTATUS_RSCLEAK,                 /* Resource leak */
   eCFGSTATUS_INTERR,                  /* Internal error */
   eCFGSTATUS_FAIL_END = eCFGSTATUS_INTERR,

   eCFGSTATUS_MAX                      /* Status count (not used) */
} CFGSTATUS;

/* Configuration ID type */
typedef enum CFGTYPE
{
   eCFGTYPE_SYS,                       /* System */
   eCFGTYPE_USR,                       /* User */
   eCFGTYPE_CALL,                      /* Call */
   eCFGTYPE_EVT,                       /* Event */

   eCFGTYPE_MAX                        /* Type count (not used) */
} CFGTYPE;

/* -------------- CALL CONFIG -------------- */
/* CallXtl.User (optional)
** This structure contains user configuration which can be overriden per call.
** Parameters in this structure MUST be pointers.
** The parameters MUST initially point to the parameters in the user configuration
** (refer to cfgAddUsr()).
*/
typedef struct CFGCEUSR
{
   /* Main */
   CCTKROUTELST      *pObProxyLst;     /* Outbound proxy list */

   /* Extended */
   CCTKGENHDRCFG     *pGenHdr;         /* Generic header configuration */
} CFGCEUSR;

/* CallXtl.Basic.General */
typedef struct CFGCEBASGEN
{
   /* Main (must be in order) */
   void              *dummy;           /* Add main configuration here */

   /* Extended */
   CCTKEMGCALLCFG    emgCallCfg;       /* Emergency call configuration */
#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
   CCTKTELCALL       tel;              /* Telephone call configuration */
#endif
} CFGCEBASGEN;

/* CallXtl.Basic.Advanced */
typedef struct CFGCEBASADV
{
   /* Main (must be in order) */

   /* Extended */
   CCTKPRACK         ePrack;           /* PRACK support configuration */
   CCTKSESTMR        sesTmr;           /* Session timer configuration */
   CCTKPRIVCFG       priv;             /* Privacy configuration */
   CCTKNAICFG        nai;              /* Network asserted identity configuration */
   CCTKCONNECTEDID   eConnectedId;     /* Connected ID configuration */
   // [TBD] Add precondition support
} CFGCEBASADV;

/* CallXtl.Basic */
typedef struct CFGCEBAS
{
   CFGCEBASGEN       gen;              /* General configuration */
   CFGCEBASADV       adv;              /* Advanced service configuration */
} CFGCEBAS;

/* CallXtl.Supplementary */
typedef struct CFGCESUPP
{
   /* Main (must be in order) */
   CCTKSIPHOST       mohServer;        /* Music-on-hold server */

   /* Extended */
   CCTKHOLDCFG       hold;             /* Call hold configuration */
   CCTKXFERCFG       xfer;             /* Call transfer configuration */
   // [TBD] Add call forwarding config
} CFGCESUPP;

/* CallXtl.Media */
typedef struct CFGCEMED
{
   /* Main (must be in order) */
   BOS_IP_ADDRESS    ipaddr;           /* Media IP address */
   BOS_UINT8         *pcSesName;       /* Controls the s= content.  By default (when
                                       ** set to NULL), the s= line is set to "-"
                                       ** as per RFC 3264.
                                       */
   CCTKMEDAUD        *pAudio;          /* Audio cap configuration and policies,
                                       ** set to NULL if not available.
                                       */
#if CCTKCFG_VIDEO
   CCTKMEDVID        *pVideo;          /* Video cap configuration and policies,
                                       ** set to NULL if not available.
                                       */
#endif
   CCTKMEDFAX        *pFax;            /* Fax cap configuration and policies,
                                       ** set to NULL if not available.
                                       */
   CCTKMEDLST        *pMediaLst;       /* Offer media list, if different from the
                                       ** configured capabilities.  The list is
                                       ** used when the user would like a subset of
                                       ** the caps configured, for example, using
                                       ** only audio when both audio and T.38 are
                                       ** supported.
                                       */
   BOS_SINT32        sNtpTimeAdj;      /* the time that needs to be adjusted from 
                                       ** the current time of day time to get the 
                                       ** NTP time
                                       */
   /* Extended */
   CCTKMEDNEGCFG     neg;              /* Negotiation configuration */
} CFGCEMED;

/* CallXtl */
typedef struct CFGCALLXTL
{
   CFGCEUSR          usr;              /* User configuration (optional) */
   CFGCEBAS          basic;            /* Basic call configuration */
   CFGCESUPP         supp;             /* Supplementary call configuration */
   CFGCEMED          media;            /* Media configuration */
} CFGCALLXTL;

/* CallInt */
typedef struct CFGCALLINT
{
   BOS_UINT32        uUsrId;           /* User associated to the call (GUID) */
} CFGCALLINT;

/* CallFull */
typedef struct CFGCALL
{
   CFGCALLXTL        xtl;              /* External (main + extended) config */
   CFGCALLINT        in;               /* Internal configuration */
} CFGCALL;

/* -------------- EVENT SUBSCRIPTION CONFIG -------------- */
/* EvtXtl.User (optional)
** This structure contains user configuration which can be overriden per event
** subscription. Parameters in this structure MUST be pointers.
** The parameters MUST initially point to the parameters in the user configuration
** (refer to cfgAddUsr()).
*/
typedef struct CFGEEUSR
{
   /* Main */
   CCTKROUTELST      *pObProxyLst;     /* Outbound proxy list */

   /* Extended */
   CCTKGENHDRCFG     *pGenHdr;         /* Generic header configuration */
} CFGEEUSR;

/* EvtXtl.General */
typedef struct CFGEEGEN
{
   /* Main (must be in order) */
   void              *dummy;           /* Add main config here */

   /* Extended */
   CCTKEVTTMR        tmr;              /* Event subscription timer configuration */
} CFGEEGEN;

/* EvtXtl.Advanced */
typedef struct CFGEEADV
{
   /* Main (must be in order) */

   /* Extended */
   CCTKPRIVCFG       priv;             /* Privacy configuration */
   CCTKNAICFG        nai;              /* Network asserted identity configuration */
} CFGEEADV;

/* EvtXtl */
typedef struct CFGEVTXTL
{
   CFGEEUSR          usr;              /* User configuration (optional) */
   CFGEEGEN          gen;              /* Genaral configuration */
   CFGEEADV          adv;              /* Advanced service configuration */
} CFGEVTXTL;

/* EvtInt */
typedef struct CFGEVTINT
{
   BOS_UINT32        uUsrId;           /* User associated to the event subscription
                                       ** (GUID) 
                                       */
} CFGEVTINT;

/* EvtFull */
typedef struct CFGEVT
{
   CFGEVTXTL         xtl;              /* External (main + extended) config */
   CFGEVTINT         in;               /* Internal configuration */
} CFGEVT;

/* -------------- USER CONFIG -------------- */
/* UserXtl.User.Network */
typedef struct CFGUEUSRNET
{
   /* Main (must be in order) */
   CCTKSIPNAME       sipName;          /* SIP name information */
   BOS_BOOL          bSecOnly;         /* True to indicate this is a secured user */
   CCTKROUTELST      obProxyLst;       /* Outbound proxy list */
   BOS_UINT32        uDnsId;           /* User identifier for DNS lookup */

   /* Extended */
   CCTKUSRPORTCFG    portCfg;          /* User local port configuration */
   CCTKDIAGEVTCFG    diagEvt;          /* Dialog event configuration */
   CCTKUAPEVTCFG     uapEvt;           /* User-agent profile event configuration */
   CCTKUSREMGCFG     emgCfg;           /* User support for emergency call */
   CCTKUSRBRCFG      brCfg;            /* User support for backup and recovery */
   CCTKSVCROUTE      svcRoute;         /* Service-Route header config override */
   CCTKCONTACT       contact;          /* User contact configuration */
} CFGUEUSRNET;

/* UserXtl.User.Local */
typedef struct CFGUEUSRLOC
{
   /* Main (must be in order) */

   /* Extended */
   BOS_UINT32        uSubEvt;          /* Supported subscription events */
   CCTKGENHDRCFG     genHdr;           /* Generic header configuration */
} CFGUEUSRLOC;

/* UserXtl.User.Security.Auth */
typedef struct CFGUEUSECAUTH
{
   /* Main (must be in order) */
   CCTKAUTHCINFO     client;           /* Client authentication information */

   /* Extended */
   CCTKAUTHCFG       cfg;              /* Authentication configuration */
} CFGUEUSECAUTH;

/* UserXtl.User.Security.DoS */
typedef struct CFGUEUSECDOS
{
   /* Main (must be in order) */

   /* Extended */
   CCTKWHTLSTCFG     whtLst;           /* White-list configuration */
   CCTKIPADDRLST     blkLst;           /* Black-list configuration */
} CFGUEUSECDOS;

/* UserXtl.User.Security.Nat */
typedef struct CFGUEUSECNAT
{
   /* Main (must be in order) */
   CCTKNATKACFG      kaCfg;            /* Keep-alive configuration */

   /* Extended */
} CFGUEUSECNAT;

/* UserXtl.User.Security */
typedef struct CFGUEUSRSEC
{
   CFGUEUSECAUTH     auth;             /* Authentication configuration */
   CFGUEUSECDOS      dos;              /* DoS configuration */
   CFGUEUSECNAT      nat;              /* NAT configuration */
} CFGUEUSRSEC;

/* UserXtl.User */
typedef struct CFGUEUSR
{
   CFGUEUSRNET       net;              /* Network configuration */
   CFGUEUSRLOC       loc;              /* Local configuration */
   CFGUEUSRSEC       sec;              /* Security configuration */
} CFGUEUSR;

/* UserXtl.Registration.General */
typedef struct CFGUEREGGEN
{
   /* Main (must be in order) */
   CCTKSIPHOST       registrar;        /* Address of the registrar */
   CCTKHOSTPORT      aor;              /* Address of record */

   /* Extended */
   CCTKREGTMR        tmr;              /* Timer configuration */
   CCTKREGEVTCFG     regEvt;           /* Registration state event package */
} CFGUEREGGEN;

/* UserXtl.Registration */
typedef struct CFGUEREG
{
   CFGUEREGGEN       gen;              /* General configuration */
} CFGUEREG;

/* UserXtl.MWI.General */
typedef struct CFGUEMWIGEN
{
   /* Main (must be in order) */
   CCTKSIPUSRHOST    server;           /* Address of the MWI server */
   BOS_BOOL          bUnsolNtfy;       /* True to accept unsolicited NOTIFY */

   /* Extended */
   CCTKMWICFG        cfg;              /* MWI configuration */
   CCTKMWITMR        tmr;              /* MWI timer configuration */
} CFGUEMWIGEN;

/* UserXtl.MWI */
typedef struct CFGUEMWI
{
   CFGUEMWIGEN       gen;              /* General configuration */
} CFGUEMWI;

/* UserXtl.Default */
typedef struct CFGUEDFT
{
   CFGCALLXTL  call;                   /* Default call configuration */
   CFGEVTXTL   evt;                    /* Default event subscription config */
} CFGUEDFT;

#if CCTKCFG_REMOTE_USER_CONTROL
/* UserXtl.RemoteUser */
typedef struct CFGUEREMUSR
{
   /* Main (must be in order) */      /* matching CCTKREMUSR */
   BOS_BOOL        bRemUsr;           /* flag indicating if this is a remote user */   

   /* Extended */
   CCTKREMUSRCFG   cfg;
} CFGUEREMUSR;
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

/* UserXtl */
typedef struct CFGUSRXTL
{
   CFGUEUSR          usr;              /* User configuration */
   CFGUEREG          reg;              /* Registration configuration */
   CFGUEMWI          mwi;              /* MWI configuration */
   CFGUEDFT          dft;              /* User default configuration */
#if CCTKCFG_REMOTE_USER_CONTROL
   CFGUEREMUSR       remUsr;           /* Remote user configuration */
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
} CFGUSRXTL;

/* UserInt */
typedef struct CFGUSRINT
{
   MX_NS ISceUserConfig
                     *pSceUsrCfg;      /* SCE user configuration handle */
   BOS_BOOL          bDeferEvtAdd;     /* True to indicate the function status
                                       ** event for cctkUserAdd() is deferred until
                                       ** persistent connection configuration is
                                       ** completed. This flag will be reset after
                                       ** the function status event is sent.
                                       */
   BOS_BOOL          bDeferEvtRemove;  /* True to indicate the function status
                                       ** event for cctkUserRemove() is deferred
                                       ** until persistent connection termination is
                                       ** completed. This flag will be reset after
                                       ** the function status event is sent.
                                       */
   CCTKHDR           cachedAuth;       /* To keep a cached copy of the latest
                                       ** Authorization header used by the user that
                                       ** we may insert in subsequent requests that
                                       ** would normally not contain any.
                                       */
} CFGUSRINT;

/* UserFull */
typedef struct CFGUSR
{
   CFGUSRXTL         xtl;              /* External (main + extended) config */
   CFGUSRINT         in;               /* Internal configuration */
} CFGUSR;

/* -------------- SYSTEM CONFIG -------------- */
/* SystemXtl.Network.General */
typedef struct CFGSENETGEN
{
   /* Main (must be in order) */
   CCTKHOSTINFO      host;             /* Host address information */

   /* Extended */
   CCTKLOCPORT       port;             /* Local port configuration */
   CCTKSIGTRANS      transport;        /* Transport configuration */
#if CCTKCFG_TLS
   CCTKTLSCFG        tls;              /* TLS configuration */
#endif
   CCTKCNXCFG        cnx;              /* Connection activity configuration */
} CFGSENETGEN;

/* SystemXtl.Network.Protocol.General */
typedef struct CFGSENPROTGEN
{
   /* Main (must be in order) */
   CCTKEMGADDR       emgAddr;          /* Emergency service address information */

   /* Extended */
   CCTKUAIDHDR       uaIdHdr;          /* User agent identity header config */
   CCTKROUTINGHDR    routingHdr;       /* Routing-related header configuration */
   CCTKDNSMODE       eDnsMode;         /* DNS query policy */
   CCTKSIGBEHAVE     sigBehave;        /* SIP signaling behavioral configuration */
   BOS_UINT32        uInterop;         /* SIP inter-operability configuration */
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
   CCTKIMSCFG        imsCfg;           /* IMS-specific configuration */
#endif
   CCTKGRUUCFG       gruu;             /* GRUU configuration */
   CCTKPAUCFG        pau;              /* P-Asscoiated-URI configuration */
   CCTKPANICFG       pani;             /* P-Access-Network-Info configuration */
#if CCTKCFG_ICE_LITE_SUPPORT
   CCTKICECTLCFG     ice;              /* ICE control configuration */
   CCTKIPADDRLST     iceAddrLst;       /* ICE local IP-Address list */
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
} CFGSENPROTGEN;

/* SystemXtl.Network.Protocol.Timer */
typedef struct CFGSENPROTTMR
{
   /* Main (must be in order) */

   /* Extended */
   CCTKTRANSTMRCFG   trans;            /* Transaction timer configuration */
} CFGSENPROTTMR;

/* SystemXtl.Network.Protocol */
typedef struct CFGSENETPROT
{
   CFGSENPROTGEN     gen;              /* General protocol configuration */
   CFGSENPROTTMR     tmr;              /* Protocol timer configuration */
} CFGSENETPROT;

/* SystemXtl.Network */
typedef struct CFGSENET
{
   CFGSENETGEN       gen;              /* General configuration */
   CFGSENETPROT      prot;             /* Protocol configuration */
} CFGSENET;

/* SystemXtl.Local.General */
typedef struct CFGSELOCGEN
{
   /* Main (must be in order) */
   BOS_UINT8         *pcSeed;          /* Random seed for tag/ID generation */

   /* Extended */
} CFGSELOCGEN;

/* SystemXtl.Local.Debug */
typedef struct CFGSELOCDEBUG
{
   /* Main (must be in order) */

   /* Extended */
   CCTKPKTLOG        pktLog;           /* Packet logger via UDP messages */
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
   CCTKTRACE         trace;            /* Tracing configuration */
#endif
} CFGSELOCDEBUG;

/* SystemXtl.Local */
typedef struct CFGSELOC
{
   CFGSELOCGEN       gen;              /* General configuration */
   CFGSELOCDEBUG     debug;            /* Debug configuration */
} CFGSELOC;


/* SystemXtl.Security.NAT.RG */
typedef struct CFGSESNATRG
{
   /* Main (must be in order) */

   /* Extended */
   CCTKRGNATCTL     ctl;              /* RG NAT control configuration */
} CFGSESNATRG;


/* SystemXtl.Security.NAT.General */
typedef struct CFGSESNATGEN
{
   /* Main (must be in order) */

   /* Extended */
   CCTKNATGENCFG     cfg;              /* General NAT configuration */
} CFGSESNATGEN;

/* SystemXtl.Security.NAT */
typedef struct CFGSESECNAT
{
   CFGSESNATGEN      gen;              /* General configuration */
   CFGSESNATRG       rg;               /* RG configuration */
} CFGSESECNAT;

/* SystemXtl.Security.DoS */
typedef struct CFGSESECDOS
{
   /* Main (must be in order) */

   /* Extended */
#if CCTKCFG_MEMORY_MONITORING
   CCTKMEMPROTCFG    memProt;          /* Memory allocation protection */
#else
   void              *dummy;           /* To fill in this structure */
#endif
} CFGSESECDOS;

/* SystemXtl.Security */
typedef struct CFGSESEC
{
   CFGSESECNAT       nat;              /* NAT configuration */
   CFGSESECDOS       dos;              /* DoS configuration */
} CFGSESEC;


/* SystemXtl.RemoteUsr */
typedef struct CFGSEREMUSR
{
#if CCTKCFG_REMOTE_USER_CONTROL
   CCTKREMUSRSYS    gen;              /* General configuration */
#endif /* CCTKCFG_REMOTE_USER_CONTROL */      
} CFGSEREMUSR;


/* SystemXtl */
typedef struct CFGSYSXTL
{
   CFGSENET          net;              /* Network configuration */
   CFGSELOC          loc;              /* Local configuration */
   CFGSESEC          sec;              /* Security configuration */
   CFGSEREMUSR       remUsr;           /* SIP remote user system configuration */
} CFGSYSXTL;

/* SystemInt */
typedef struct CFGSYSINT
{
   BOS_BOOL          bTlsEnabled;      /* True to indicate all TLS information is
                                       ** available and TLS support can be enabled.
                                       */
} CFGSYSINT;

/* SystemFull */
typedef struct CFGSYS
{
   CFGSYSXTL         xtl;              /* External (main + extended) config */
   CFGSYSINT         in;               /* Internal configuration */
} CFGSYS;

/* ---- Variable Externs -------------------------------------------------------- */


/* ---- Function Prototypes ----------------------------------------------------- */
/* Configuration Framework (de)initialization routines */
CFGSTATUS      cfgInit( void );
CFGSTATUS      cfgDeInit( void );
BOS_BOOL       cfgIsAllCfgRel( void );
BOS_BOOL       cfgIsFuncAllowed( TKM_MSGID eFuncId );

/* Configuration duplication routines */
CCTKCFG        *cfgDupSystem( const CCTKCFG *pSrc );
void           cfgDelSystem( CCTKCFG *&rpCfg );
CCTKUSRCFG     *cfgDupUsrCfg( const CCTKUSRCFG *pSrc );
void           cfgDelUsrCfg( CCTKUSRCFG *&rpCfg );
void           *cfgDupXtlCfg( CCTKPARM eParm, const void *pSrc );
void           cfgDelXtlCfg( CCTKPARM eParm, void *&rpCfg );

/* Configuration operation routines */
CFGSTATUS      cfgStartUp( MX_NS CEventDriven *pMainFramework );
CFGSTATUS      cfgShutDown( void );
CFGSTATUS      cfgReset( void );
CFGSTATUS      cfgSetupSystem( CCTKCFG *&rpSysCfg );
CFGSTATUS      cfgAddUsr( BOS_UINT32 uGuid, CCTKUSRCFG *&rpUsrCfg );
CFGSTATUS      cfgDupUsr( BOS_UINT32 uGuid, BOS_UINT32 uGuidSrc );
CFGSTATUS      cfgRemoveUsr( BOS_UINT32 uGuid );
CFGSTATUS      cfgAddCall( BOS_UINT32 uGuid, BOS_UINT32 uGuidSrc );
CFGSTATUS      cfgRemoveCall( BOS_UINT32 uGuid );
CFGSTATUS      cfgUpdCallUsr( BOS_UINT32 uGuid, BOS_UINT32 uGuidCurrUser, 
                              BOS_UINT32 uGuidNewUser);
CFGSTATUS      cfgAddEvt( BOS_UINT32 uGuid, BOS_UINT32 uGuidSrc );
CFGSTATUS      cfgRemoveEvt( BOS_UINT32 uGuid );
CFGSTATUS      cfgSetXtlCfg( BOS_UINT32 uGuid, CCTKPARM eParm, void *&rpCfg );
CFGSTATUS      cfgAddRealm( BOS_UINT32 uGuidUsr, const BOS_UINT8 *pcRealm );

/* Configuration retrieving routines */
const void     *cfgGet( CFGTYPE eType, BOS_UINT32 uGuid );
CFGTYPE        cfgGetType( BOS_UINT32 uGuid );
BOS_UINT32     cfgGetUsrId( BOS_UINT32 uGuid );

/* Other utility routines */
BOS_BOOL       cfgIsAddrValid( const BOS_UINT8 *pcAddr );
BOS_BOOL       cfgIsHdrValid( const CCTKHDR &rHdr );
BOS_BOOL       cfgIsHostPortEnabled( const CCTKHOSTPORT &rHostPort );
BOS_BOOL       cfgIsHostPortIpv6( const CCTKHOSTPORT &rHostPort );
BOS_BOOL       cfgIsNaiEnabled( const CCTKNAICFG &rNaiCfg );
BOS_BOOL       cfgIsSipHostEnabled( const CCTKSIPHOST &rSipHost );
BOS_BOOL       cfgIsSipHostIpv6( const CCTKSIPHOST &rSipHost );

/* Configuration conversion routines */
MX_NS IUri     *cfgCvtAddrToMx( const BOS_UINT8 *pcCctk );
int            cfgCvtDnsModeToMx( CCTKDNSMODE eCctk );
MX_NS CSipHeader
               *cfgCvtHdrToMx( const CCTKHDR &rCctk );
MX_NS CString  *cfgCvtHdrNameToMx( const CCTKHDR &rCctk );
CFGSTATUS      cfgCvtHostPortToMx( const CCTKHOSTPORT &rCctk, 
                                   MX_NS CHostPort &rMx );
CFGSTATUS      cfgCvtIpAddrToMx( const BOS_IP_ADDRESS &rCctk, 
                                 MX_NS CSocketAddr &rMx );
CFGSTATUS      cfgCvtIpPortToMx( const CCTKIPPORT &rCctk, MX_NS CSocketAddr &rMx );
uint16_t       cfgCvtLocPortToMx( const CCTKLOCPORT &rCctk, BOS_BOOL bSec );
BOS_UINT32     cfgCvtMethodFromMx( MX_NS ESipMethod eMx );
MX_NS ISceUserIdentityConfig::ENetworkAssertedIdMode
               cfgCvtNaiModeToMx( const CCTKNAICFG &rCctk );
MX_NS CNameAddr
               *cfgCvtNameAddrToMx( const CCTKSIPHOST &rCctkSipHost,
                                    const CCTKSIPNAME &rCctkSipName );
MX_NS CNameAddr
               *cfgCvtNameAddrToMx( const CCTKHOSTPORT &rCctkHostPort,
                                    BOS_BOOL bCctkSec,
                                    const CCTKSIPNAME &rCctkSipName );
MX_NS IUaSspCallConfig::ERel1xxConfig
               cfgCvtPrackCfgToMx( CCTKPRACK eCctk );
MX_NS IUaSspCallConfig::EConnectedIdConfig
               cfgCvtConnectedIdCfgToMx( CCTKCONNECTEDID eCctk );
MX_NS ISceUserIdentityConfig::EPrivacyMode
               cfgCvtPrivLvlToMx( CCTKPRIVLVL eCctk, BOS_BOOL bCritical, BOS_BOOL bIdRequired );
CFGSTATUS      cfgCvtRealmToMx( const CCTKRLM &rCctk,
                                MX_NS CString &rMxRealm,
                                MX_NS CString &rMxUser,
                                MX_NS CString &rMxPwd );
CFGSTATUS      cfgCvtRealmToMx( const BOS_UINT8 *pcCctkRealm,
                                const BOS_UINT8 *pcCctkUser,
                                const BOS_UINT8 *pcCctkPwd,
                                MX_NS CString &rMxRealm,
                                MX_NS CString &rMxUser,
                                MX_NS CString &rMxPwd );
MX_NS CSipHeader
               *cfgCvtRouteToMx( const CCTKROUTE cctk[] );
MX_NS CSipHeader 
               *cfgCvtRouteLstToMx( const CCTKROUTELST &rCctk );
MX_NS IUaSspCallConfig::ESessionRefreshConfig
               cfgCvtSesRefreshCfgToMx( CCTKSESTMRMODE eCctk );
MX_NS CSipUri  *cfgCvtSipUriToMx( const CCTKHOSTPORT &rCctkHostPort,
                                  BOS_BOOL bCctkSec,
                                  const BOS_UINT8 *pcCctkUsrName );
CFGSTATUS      cfgCvtSockAddrFromMx( const MX_NS CSocketAddr &rMxSockAddr,
                                     CCTKIPPORT &rCctkIpPort );
MX_NS CSipHeader
               *cfgCvtSvcRouteToMx( const CCTKSVCROUTE &rCctk );
#if CCTKCFG_TLS
CFGSTATUS      cfgCvtTlsCfgToMx( const CCTKTLSCFG &rCctk, 
                                 MX_NS CTlsContext &rMxClient,
                                 MX_NS CTlsContext &rMxServer );
#endif
#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
BOS_UINT16     cfgCvtTraceGrpFromMx( uint32_t uMx );
BOS_UINT16     cfgCvtTraceLvlFromMx( EMxTraceLevel eMx );
EMxTraceLevel  cfgCvtTraceLvlToMx( BOS_UINT16 uCctk );
#endif
int            cfgCvtTransportToMx( BOS_UINT32 uCctk );

#endif /* __CFG_API__H__INCLUDED__ */
