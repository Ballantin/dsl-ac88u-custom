/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom
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
**      This file contains the CMGR main configuration.
**
***********************************************************************************/

#ifndef __CMGR_MAIN_CFG__H__INCLUDED__
#define __CMGR_MAIN_CFG__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <cmgrCfg.h>
#include <bosTypes.h>

/* ---- Constants and Types ----------------------------------------------------- */

/* Define any constant used for size/array within the Call Manager.  User
** application should make note and try to use similar data set.
*/
#define CMGR_COLLECTED_DIGIT_LEN                   63
#define CMGR_USER_IDENTIFIER_LEN                   63
#define CMGR_IP_ADDRESS_BYTES_SIZE                 16
#define CMGR_IP_ADDRESS_STRING_LEN                 BOS_IP_ADDRESS_MAX_STR_LEN_V6
#define CMGR_URI_STRING_LEN                        127

/* Following are *hardcoded* timeout values for call progress tones as those
** are either not configurable or need some valid default value in case we fail
** to properly retrieve them.
**
** Timers values are defined in milliseconds.
*/
#define CMGR_DIAL_TONE_TIMEOUT                     10000
#define CMGR_RING_TONE_TIMEOUT                     30000
#define CMGR_RINGBACK_TONE_TIMEOUT                 240000
#define CMGR_ON_HOOK_GUARD_TIMEOUT                 500
#define CMGR_FLASH_WAIT_TIMEOUT                    500
#define CMGR_INTERDIGIT_SHORT_TONE_TIMEOUT         4000
#define CMGR_INTERDIGIT_LONG_TONE_TIMEOUT          16000
#define CMGR_DIGIT_HELD_TIMEOUT                    2000
#define CMGR_OFF_HOOK_WARN_TIMEOUT                 20000
#define CMGR_CALL_WAIT_TIMEOUT                     30000
#define CMGR_AUTO_CALL_TIMEOUT                     1800000
#define CMGR_WARM_LINE_TIMEOUT                     2000
#define CMGR_AUTO_RECOVER_TIMEOUT                  10000
#define CMGR_IMMEDIATE_TIMEOUT                     100
#define CMGR_DEFAULT_DTMF_DURATION                 250

#define CMGR_MSEC_2_SEC_DIVIDER                    1000

/* Default RTP and RTCP port range start.
**
** Unless explicitely configured, the RTP port in usage within the CMGR are
** numbered from this start offset and their numbers is determined by the maximum
** number of calls that can be managed at once.
**
** Also, unless explicitely configured, RTCP ports are assumed to be RTP port + 1.
**
** Note that we define two ranges, one is for audio (and image/fax) media and one
** is for video media.  Only when CMGRCFG_VIDEO_MEDIA is enabled would the latter
** matter.
*/
#define CMGR_RTP_AUD__PORT_OFFSET_START            53456
#define CMGR_RTP_VID__PORT_OFFSET_START            53556
#define CMGR_RTCP_AUD__PORT_OFFSET_START           54456
#define CMGR_RTCP_VID__PORT_OFFSET_START           CMGR_RTCP_AUD__PORT_OFFSET_START


/* Preamble allocated in front of a tone definition for tones that are coming
** set as a string value.
**
** This is a temporary fix until the API into the endpoint allows for proper tone
** and announcement support.
*/
#define CMGR_TONE_PREAMBLE_FILE_IDENTIFIER          "file:///PacketCableRST/"
#define CMGR_TONE_PREAMBLE_HTTP_IDENTIFIER          "http://127.0.0.1/"
#define CMGR_TONE_PREAMBLE_BELLCORE_IDENTIFIER      "Bellcore-d"

/* CMGR status information.
**
**   This is a generic enum used to carry status of various form but essentially
**   meant to signal 'device' specific (or global) information.  Other report do
**   exists and are carried through specialized callbacks when necesary.
**
**   Status reported are generally self explanatory from looking at the name.
*/
typedef enum
{
   eCMGRSTATUS_INIT_COMPLETE,
   eCMGRSTATUS_INIT_FAILURE,
   eCMGRSTATUS_STARTUP_SUCCESS,
   eCMGRSTATUS_STARTUP_FAILED,
   eCMGRSTATUS_SHUTDOWN_COMPLETE,
   eCMGRSTATUS_SHUTDOWN_FAILURE,
   eCMGRSTATUS_DEINIT_COMPLETE,
   eCMGRSTATUS_DEINIT_FAILURE,
   eCMGRSTATUS_CRITICAL_ERROR,          /* Any kind of critical error that cannot */
                                        /* be recovered from.  Typically when it  */
                                        /* happens, one should destroy and create */
                                        /* again the Call Manager.                */

   eCMGRSTATUS_COMMAND_SUCCESS,         /* The command of interest is passed as */
   eCMGRSTATUS_COMMAND_FAILED,          /* the RID of the callback function.    */

} CMGRSTATUS;

#if CMGRCFG_ANNOUNCEMENT
/* CMGR annoucement action */
typedef enum
{
   eCMGRANNCMD_PLAY,
   eCMGRANNCMD_STOP,

} CMGRANNCMD;

/* CMGR annoucement reason */
typedef enum
{
   eCMGRANNCMD_NOSRV,            /* Not in-service */
   eCMGRANNCMD_NOOP,             /* Not operational */
   eCMGRANNCMD_DIS,              /* Disabled */

} CMGRANNRES;

/* CMGR announcement status block. */
typedef struct
{
   BOS_UINT32 uHdl;
   CMGRANNCMD eCmd;
   CMGRANNINF eInf;

} CMGRANNSTA;
#endif

/* CMGR event publication
**
**   This is the list of events reported by different module of the Call Manager
**   in order to either inform the controlling application of what is happening,
**   or to force the controlling application to execute some third party action.
**
**   Those events can either be 'spontaneous'-ly triggered by the Call Manager
**   during its normal processing, or they may be triggered following an action
**   from the external controlling application (i.e. a query for example).
*/
typedef enum
{
   /* Physical handle */
   eCMGREVT_ONHOOK,                 /* Line on hook */
   eCMGREVT_OFFHOOK,                /* Line off hook */
   eCMGREVT_LINE_ACTIVE,            /* Line activity - call, tones, loopback */
   eCMGREVT_LINE_INACTIVE,          /* Line inactive */

   /* User handle */
   eCMGREVT_MWI_ON,
   eCMGREVT_MWI_OFF,
   eCMGREVT_OPER_OK,                /* User operational */
   eCMGREVT_OPER_NOK,               /* User non operational */
   eCMGREVT_OPER_PROGRESS,          /* User operation progress - used for unreg/registering states */
   eCMGREVT_OPER_ACTIVECALLS,       /* Number of active calls on the line changed */
   eCMGREVT_CALL_STATS,
   eCMGREVT_GLOB_STATS,
   eCMGREVT_NO_SEC,
   eCMGREVT_DIALPLAN_ERROR,
   eCMGREVT_OPER_NOK_FORBIDDEN,    /* User non operationaldue to 403 forbidden*/
   eCMGREVT_MESSAGE,
   eCMGREVT_MSG_HDL,                /* Message handle mapping */
   eCMGREVT_MSG_RSP,

   /* Application handle */
   eCMGREVT_CALL_NEW,               /* New call (in or out) */
   eCMGREVT_CALL_RINGBACK,          /* Outgoing call ringing on remote */
   eCMGREVT_CALL_CONNECT,           /* Call connected event */
   eCMGREVT_CALL_ENDED,             /* Call ended event */
   eCMGREVT_SVC_APPLY,              /* Service applied event */
   eCMGREVT_SVC_DENY,               /* Service denied event */
   eCMGREVT_CONF_ESTAB,             /* Conference established event */
   eCMGREVT_CALL_XFER,              /* Call Transfer event */

#if CMGRCFG_DEBUG_INFO
   eCMGREVT_DEBUG_INFO,              /* Debug use only */
#endif

   eCMGREVT_TARGETS_UPDATE,         /* Proxy target update */
   eCMGREVT_PUBLISH_TARGET,          /* Publish target update */
   eCMGREVT_SUB_OK,                 /*Subscription INFO*/
   eCMGREVT_SUB_NOK

} CMGREVT;

/* CMGR -> Network Requests.
** To be used when CCTK is not used to communicate
** with the network side.
** Only used for Circuit switch 2G/3G calls currently.
*/
typedef enum
{
   CMGRNETREQ_REGISTER,       /* Sets up the callback to receive CMNETEVT events */
   CMGRNETREQ_CALL_MAKE,      /* Make outgoing call */
   CMGRNETREQ_CALL_ANSWER,    /* Answer Incoming call */
   CMGRNETREQ_CALL_RELEASE,   /* Terminate call */
   
} CMGRNETREQ;

/* Network -> CMGR events.
*/
typedef enum
{
   eCMNETEVT_NEWCALL,                   /* New call (create resource) */
   eCMNETEVT_ENDCALL,                   /* Call ended (free resource) */
   eCMNETEVT_ALERT,                     /* Alert user of incoming call */
   eCMNETEVT_ALERTED,                   /* Remote end is ringing */
   eCMNETEVT_ANSWER,                    /* Call answered by remote */
   eCMNETEVT_DISCONNECT,                /* Remote initiated release */
   eCMNETEVT_BUSY,                      /* Remote end is busy */
} CMNETEVT;

/* Caller identification information.
*/
typedef struct CMNETCLIDINFO
{
   BOS_UINT8     cName[CCTK_CLID_LEN + 1];
   BOS_UINT8     cNumber[CCTK_CLID_LEN + 1];
} CMNETCLIDINFO;

/* Caller information.
*/
typedef struct CMNETCALLINFO
{
   CMNETCLIDINFO  clid;
   BOS_UINT8     cErrorInfo[CCTK_STR_LEN + 1];
   BOS_BOOL      bLoopBack;
   BOS_BOOL      bEmergency;
   BOS_BOOL      bAnonymous;
   BOS_BOOL      bMedKeepAlive;
   BOS_UINT32    uJoin;
} CMNETCALLINFO;

typedef void ( *CMGRNETEVT ) ( CMNETEVT eEvent, BOS_UINT32 uCid, void *pData );

#if CMGRCFG_FIREWALL_CONTROL
/* CMGR firewall control. */
typedef enum
{
   eCMGRFWCMD_OPEN,
   eCMGRFWCMD_CLOSE,

} CMGRFWCMD;

#define UNIQUE_ID(cid, port) (cid << 16) | port /* Creates a unique ID for a firewall filter name */

/* CMGR firewall protocol. */
typedef enum
{
   eCMGRFWPROT_UDP,
   eCMGRFWPROT_TCP,
   eCMGRFWPROT_UDP_TCP,

} CMGRFWPROT;

/* CMGR firewall control structure.
**
**   Defines the information to be passed to the external firewall control.
*/
typedef struct
{
   BOS_IP_ADDRESS ipAdd;
   BOS_UINT16     uPort;
   CMGRFWPROT     eProt;

} CMGRFWINFO;
#endif

/* CMGR reset control. */
typedef enum
{
   eCMGRRESCTL_IMMEDIATE,             /* Reset is immediate, there is
                                      ** no specific termination of the network
                                      ** resources that may be in use at the
                                      ** time of the action.
                                      */
   eCMGRRESCTL_GRACEFULNONBLOCKING,   /* Reset is graceful but non blocking,
                                      ** meaning network resources are being
                                      ** made aware once of their clearing using
                                      ** appropriate signaling, but there is no
                                      ** wait on the local resources which are
                                      ** terminated right away.
                                      */
   eCMGRRESCTL_GRACEFULBLOCKING,      /* Reset is graceful and blocking,
                                      ** meaning proper termination of network
                                      ** resources including indication of
                                      ** termination and wait for acknowledgment
                                      ** prior to cleaning the local resources.
                                      **
                                      ** *** NOT IMPLEMENTED CURRENTLY ***
                                      */

} CMGRRESCTL;

/* CMGR session control command. */
typedef enum
{
   eCMGRSESCMD_CREATE,
   eCMGRSESCMD_CREATE_INT,
   eCMGRSESCMD_CREATE_BARGEIN,
   eCMGRSESCMD_UPDATE,
   eCMGRSESCMD_UNMUTE_INT,
   eCMGRSESCMD_MUTE,
   eCMGRSESCMD_MUTE_INT,
   eCMGRSESCMD_TERMINATE,
   eCMGRSESCMD_TERMINATE_INT,
   eCMGRSESCMD_TERMINATE_BARGEIN,
   eCMGRSESCMD_STATS,
   eCMGRSESCMD_ACTMONTHRES,

} CMGRSESCMD;

/* CMGR session control status */
typedef enum
{
   eCMGRSESSTA_SUCCESS,
   eCMGRSESSTA_INVALID_SES,
   eCMGRSESSTA_NO_RESOURCE,
   eCMGRSESSTA_TEMPORARY_FAIL,
   eCMGRSESSTA_PERMANENT_FAIL,

} CMGRSESSTA;

/* CMGR command issued by the controlling application. */
typedef enum
{
   eCMCMD_RESTART,          /* Restart the CMGR - includes CCTK restart. */
   eCMCMD_SHUTDOWN,         /* Shutdown the CMGR - includes CCTK shutdown. */
   eCMCMD_STARTUP,          /* Startup the CMGR - internal usage only. */
   eCMCMD_NEWCONFIG,        /* New configuration information available. */
   eCMCMD_REGISTER,         /* Engage in registration. */
   eCMCMD_DEREGISTER,       /* Terminate registration/services. */
   eCMCMD_CNXSTATUS,        /* Status of a connection action. */
   eCMCMD_CNXSTATS,         /* Statistics information related to connection. */
   eCMCMD_MEMSTATS,         /* Statistics information on memory usage. */
#if CMGRCFG_ANNOUNCEMENT
   eCMCMD_ANNSTATUS,        /* Annoucement status information. */
#endif
   eCMCMD_PLACECALL,        /* Place a call through the CMGR. */
   eCMCMD_ENDCALL,          /* Terminate a call through the CMGR. */
   eCMCMD_APPCALLHDL,       /* Setup the application handle to CMGR. */
   eCMCMD_ANSCALL,          /* Answer the call through the CMGR. */
   eCMCMD_HOLDSVC,          /* Apply hold service through the CMGR. */
   eCMCMD_CONFSVC,          /* Apply conference service through the CMGR. */
   eCMCMD_BXFERSVC,         /* Apply blind xfer service through the CMGR. */
   eCMCMD_CXFERSVC,         /* Apply consultative xfer service through the CMGR. */
   eCMCMD_DTMFPT,           /* Apply DTMF 'passthrough' service through the CMGR. */
   eCMCMD_CLEARMWI,         /* Clear (turn off) MWI condition for a user. */
   eCMCMD_SETMWI,           /* Set (turn on) MWI condition for a user. */
   eCMCMD_STATUS,           /* Provide status information on CMGR and CCTK managed
                            ** data.  Requires log to be enabled to show the
                            ** information. */
   eCMCMD_DATALNK,          /* Data link status report to the CMGR. */
   eCMCMD_SVCSTART,         /* Start a specific service. */
   eCMCMD_SVCEND,           /* End a specific service. */
#if CMGRCFG_PASS_THRU_MSG
   eCMCMD_MESSAGE,          /* Pass through message sent via CMGR. */
   eCMCMD_MSG_RESP,         /* Response to a pass through mesasge received
                            ** via CMGR. */
#endif

#if CMGRCFG_DECT
   eCMCMD_START_DIGCOLL,    /* Reset and start digit collection for this user */
   eCMCMD_TERMINATE_CALL,   /* Terminate a specific call with the given call id. */
   eCMCMD_REJECT_CALL,      /* Reject a call for a given user and call id */
   eCMCMD_SET_PREF_LINE,    /* Set the user's preferred SIP line */
   eCMCMD_TOGGLE_CALL,      /* Toggles to the given call */
   eCMCMD_CONFERENCE_CALL,  /* CAT-iq 2.0 specific conference call */
   eCMCMD_XFER_CALL,        /* CAT-iq 2.0 specific transfer call */
   eCMCMD_HOLD_CALL,        /* CAT-iq 2.0 specific hold call */
   eCMCMD_INTERCEPT_CALL,   /* Call interception */
   eCMCMD_INTERNAL_CALL,    /* DECT internal call */
   eCMCMD_REPLACE_CALL,     /* Replace active call with calll waiting call */
   eCMCMD_RELEASE_USR,      /* Release all calls on a user */
#endif /* CMGRCFG_DECT */

} CMGRCMD;

/* CMGR services - to use with service apply/deny event. */
typedef enum
{
   eCMGRSVC_LOCAL_HOLD,
   eCMGRSVC_REMOTE_HOLD,
   eCMGRSVC_LOCAL_UNHOLD,
   eCMGRSVC_REMOTE_UNHOLD,
   eCMGRSVC_CONF,
   eCMGRSVC_BXFER,
   eCMGRSVC_CXFER,

} CMGRSVC;

/* CMGR connection status information block. */
typedef struct
{
   BOS_UINT32 uCnx;
   CMGRSESCMD eCmd;
   CMGRSESSTA eSta;

} CMGRCNX;

/* CMGR physical interface action. */
typedef enum
{
   eCMGRPHYCMD_INIT,        /* Initializes the physical resource handling module. */
   eCMGRPHYCMD_DEINIT,      /* De-initializes the above mentionned. */
   eCMGRPHYCMD_CREATE,      /* Create a physical resource. */
   eCMGRPHYCMD_DESTROY,     /* Destroy a physical resource. */
   eCMGRPHYCMD_SIGNAL,      /* Pass a signal to a physical resource. */

} CMGRPHYCMD;

/* CMGR signal action for signaling to physical interface. */
typedef enum
{
   eCMGREPSIGACT_START,     /* Start the referenced signal. */
   eCMGREPSIGACT_STOP,      /* Stop the referenced signal. */
   eCMGREPSIGACT_REPEAT,    /* Repeat the referenced signal. */

} CMGREPSIGACT;

/* CMGR user configuration action type that is pending for this user. */
typedef enum
{
   eCMCFGACT_NOACTION,
   eCMCFGACT_CFGDEV,
   eCMCFGACT_CFGUSRNONET,
   eCMCFGACT_CFGUSRWITHNET,
   eCMCFGACT_CFGUSRTERM,
   eCMCFGACT_CFGUSRREADD,  /* Internal use only. */
   eCMCFGACT_CFGROUTING,    /* Update Line Associations  */
#if CMGRCFG_DECT
   eCMCFGACT_CFGDECTSHUTDOWN, /* DECT shutdown  */
#endif /* CMGRCFG_DECT */

} CMCFGACT;

/* CMGR new call type used for callback event */
typedef enum
{
   eCMGRCALLTYPE_INCOMING,
   eCMGRCALLTYPE_OUTGOING
} CMGRCALLTYPE;

/* CMGR call ended reason */
typedef enum
{
   eCMENDRSN_NORMAL,
   eCMENDRSN_USER_BUSY,
   eCMENDRSN_USER_REJECTION,
   eCMENDRSN_USER_UNKNOWN,
   eCMENDRSN_CALL_RESTRICTION,
   eCMENDRSN_INSUFFICIENT_RESOURCES,
   eCMENDRSN_UNEXPECTED_MESSAGE,
   eCMENDRSN_SERVICE_NOT_IMPLEMENTED,
   eCMENDRSN_INVALID_IDENTITY,
   eCMENDRSN_TIMER_EXPIRY, 
   eCMENDRSN_NEGOTIATION_FAILED,
} CMGRENDRSN;

typedef struct
{
   CMGRCALLTYPE eType;
   BOS_UINT32   uSrc;      /* Source terminal for internal calls */
} CMGRCALLDATA;

typedef struct
{
   BOS_UINT32 uNumActiveCalls;
   BOS_UINT32 uMaxActiveCalls;
} CMGRLINESTAT;

/* CMGR endpoint initialization block. */
typedef void ( *CMGREPEVT )( BOS_UINT32 uEvt,
                             BOS_UINT32 uPhy,
                             BOS_UINT32 uSes,
                             void *pData );

/* Note that the following structure contains the minimal required set.  This could
** be enhanced as required, for example, if the CMGR is required to manage the
** actual datapath content, one can install a callback here to hook up into data
** packets.
*/
typedef struct
{
   CMGREPEVT evt;          /* Callback from physical layer with an event. */

} CMGREPINIT;

/* CMGR endpoint command signaling block. */
typedef struct
{
   BOS_UINT32 uSes;
   BOS_UINT32 uSig;
   BOS_UINT32 uApp;
   BOS_UINT32 uCid;  /* The call ID associated with this signal, if applicable */
   CMGREPSIGACT eAct;
   void *pData;

} CMGREPSIG;

/* CMGR physical interface (endpoint) control block. */
typedef struct
{
   CMGRPHYCMD eAction;
   BOS_UINT32 uPhy;
   union
   {
      CMGREPSIG sig;
      CMGREPINIT init;

   } cmd;

} CMGREPCMD;

/* CMGR memory statistics command. */
typedef struct
{
   BOS_BOOL bCctkOnly;
   BOS_BOOL bTraceTable;

} CMGRMEMSTATS;

/* CMGR media information. */
typedef struct
{
   CCTKMEDINFO media;
   BOS_UINT32  uDscp;
   BOS_BOOL    bEmergency;
   BOS_BOOL    bActKeepAlive;
   BOS_UINT32  uAppIx;

} CMGRMEDINFO;

/* CMGR reconfiguration command. */
typedef struct
{
   BOS_UINT8 uUid[ CMGR_USER_IDENTIFIER_LEN + 1 ];
   CMCFGACT eCfgAction;

} CMGRRECONFIG;

/* CMGR call placement command. */
typedef struct
{
   BOS_UINT8 uDest[ CMGR_URI_STRING_LEN + 1 ];
   BOS_UINT8 uUid[ CMGR_USER_IDENTIFIER_LEN + 1 ];
   BOS_UINT32 uAppIx;

} CMGRPLACECALL;

/* CMGR call conference command. */
typedef struct
{
   BOS_UINT32 uSesHdl;
   BOS_UINT32 uAppIx;
   BOS_BOOL bLocalConf; /* Local vs. network conference */
} CMGRCONFSVC;

/* CMGR call set application handle command. */
typedef struct
{
   BOS_UINT32 uSesHdl;
   BOS_UINT32 uAppIx;

} CMGRAPPCALLHDL;

/* CMGR status report command. */
typedef struct
{
   BOS_BOOL bShowCmgr;
   BOS_BOOL bShowCctk;

} CMGRSHOWSTATUS;

/* CMGR service control command. */
typedef struct
{
   BOS_UINT8 uUid[ CMGR_USER_IDENTIFIER_LEN + 1 ];
   BOS_UINT32 service;

} CMGRSVCACT;

/* CMGR Target information.
*/
typedef struct CMGRTARGETINFO
{
   BOS_UINT8      uNumTargets;
   BOS_IP_ADDRESS targetipAddr[CMGRCFG_TARGETS_MAX];
   BOS_UINT16     uTargetPort[CMGRCFG_TARGETS_MAX];

} CMGRTARGETINFO;

/* CMGR pass through message command. */
typedef struct
{
   CMGRPLACECALL toDest;
   BOS_UINT8 *puMsg;
   BOS_UINT32 uSize;

} CMGRPASSTHRUMSG;

/* CMGR hold service command. */
typedef struct
{
   BOS_BOOL bHold;
   BOS_UINT32 uAppIx;
   BOS_UINT8 uUid[ CMGR_USER_IDENTIFIER_LEN + 1 ];
   BOS_UINT32 uCid;
} CMGRHOLDSVC;

/* CMGR internal call service command. */
typedef struct
{
   BOS_UINT32 uPhy;
   BOS_UINT32 uPhyRemote;
} CMGRINTERNALCALL;

#if CMGRCFG_DECT
/* CMGR Start Digit Collection call command. */
typedef struct
{
   BOS_UINT32 uPhy;
} CMGRSTRTDIGCOLL;

/* CMGR terminate call command. */
typedef struct
{
   BOS_UINT32 uCid;
} CMGRTERMCALL;

/* CMGR reject call command. */
typedef struct
{
   BOS_UINT32 uCid;
   BOS_UINT32 uPhy;
} CMGRREJCALL;

/* CMGR preferred line command. */
typedef struct
{
   BOS_UINT32 uLine;
   BOS_UINT32 uPhy;
} CMGRSETPREFLINE;

typedef struct
{
   BOS_UINT32 uPhy;
   BOS_UINT32 uCid;
} CMGRTOGGLECALL;

typedef struct
{
   BOS_UINT32 uCid1;
   BOS_UINT32 uCid2;
   BOS_UINT32 uPhy;
} CMGRCONFCALL;

typedef struct
{
   BOS_UINT32 uCid1;
   BOS_UINT32 uCid2;
   BOS_UINT32  uPhy;
} CMGRXFERCALL;

typedef struct
{
   BOS_BOOL bHold;
   BOS_UINT32 uPhy;
   BOS_UINT32 uCid;
} CMGRHOLDCALL;

typedef struct
{
   BOS_UINT32 uCid; /* The call id of the call into which we are attempting to intrude */
   BOS_UINT32 uPhy;  /* Our user id */
   BOS_UINT32 uPhyOther; /* The other user's id */
} CMGRINTERCEPTCALL;

typedef struct
{
   BOS_UINT32 uPhy;
} CMGRRELUSR;

typedef struct
{
   BOS_UINT32 uCid;
   BOS_UINT32 uPhy;
} CMGRREPLACECALL;

#endif /* CMGRCFG_DECT */

/* CMGR Out Of Service Info. */
typedef struct
{
   BOS_UINT8  uUid[ CMGR_USER_IDENTIFIER_LEN + 1 ];
   BOS_UINT32 uReRegTime;

} CMGROOSINFO;

/* ---- Variable Externs -------------------------------------------------------- */
/* ---- Function Prototypes ----------------------------------------------------- */

#endif /* __CMGR_MAIN_CFG__H__INCLUDED__ */
