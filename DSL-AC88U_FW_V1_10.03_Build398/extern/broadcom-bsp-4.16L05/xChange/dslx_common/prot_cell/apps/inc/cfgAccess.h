/****************************************************************************
*
*  Copyright (c) 2008-2013 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: cfgAccess.h
*
****************************************************************************
*    Description:
*
*      Implementation of the Configuration Database Access Functionality for SIP client
*      software.
*
****************************************************************************/

#ifndef __CFG_ACCESS__H__INCLUDED__
#define __CFG_ACCESS__H__INCLUDED__

#include <bosTypes.h>
#include <bosIpAddr.h>
// add
//#include <cctkCfg.h>
// add
//#include <globalCfg.h>
//#include <cmmtaint.h>



/* Define the Name of the ENUM Item */
#define  CFGITEM(comp,name)   comp##_##name

/* Define the Name of the CFGITEM for component "comp" */
#define  CFGITEM_TYPE(comp)   comp##_CFG_ITEM

#define CIVIC_ADDRESS_STR_MAX_LEN 32
#define GEO_LOCATION_STR_MAX_LEN  CIVIC_ADDRESS_STR_MAX_LEN

/* Define the signaling transport acceptabel types.
*/
#define CFG_SIG_TRANS_UDP  0x00000001
#define CFG_SIG_TRANS_TCP  0x00000002
#define CFG_SIG_TRANS_TLS  0x00000004

/* Define the max number of failover outbound proxies that
** can be configured.
*/
#define CFG_MAX_ADDRLST    2

#if GLOBAL_CFG_IPv6_SUPPORT
#define DSS_ID_MAX_LEN 128
#endif

typedef enum
{
   CFG_ITEM_GET = 0,
   CFG_ITEM_SET,
   CFG_ITEM_GET_CB,    /* used when item is accessed from a callback */
   CFG_ITEM_DEL,
   CFG_ITEM_INIT       /* used to initialize a given concept */
} CFG_ACCESS_ACT;

/*Configuration access (non-SNMP access) definitions*/
/****************************************************************
  The following is defining all neccessary data defs for
  configration database Access Functionality for SIP
****************************************************************/
typedef  enum {
CFG_ITEM_ZeroItem = 0,

/*
  Concept IDs for SIP client.  These are grouped items and are used in the callback for notifying of changes.
*/

/*
  RST Feature Concepts - Accessible using the UserID as the index.
*/
CFGITEM(SIP,  DigitMapConcept),          /*RST Digit Map Feature*/
CFGITEM(SIP,  DigitMapExtVarConcept),    /*RST Digit Map External Variable Feature*/
CFGITEM(SIP,  BasicCallConcept),         /*RST Basic Call Feature*/
CFGITEM(SIP,  InternalCallConcept),      /*RST Internal Call Feature*/
CFGITEM(SIP,  AnnouncementConcept),      /*RST Announcement Feature*/
CFGITEM(SIP,  StatusChangeConcept),      /*RST Activation Status Feature*/
CFGITEM(SIP,  NoAnsConcept),             /*RST No Answer Feature*/
CFGITEM(SIP,  CIDConcept),               /*RST Caller ID Feature*/
CFGITEM(SIP,  CIDBlkConcept),            /*RST Caller ID Blocking Feature*/
CFGITEM(SIP,  CIDDisplayConcept),        /*RST Caller ID Display Feature*/
CFGITEM(SIP,  CallFwdConcept),           /*RST Call Forward Feature*/
CFGITEM(SIP,  CallFwdBusyConcept),       /*RST Call Forward Busy Feature*/
CFGITEM(SIP,  CallFwdNoAnsConcept),      /*RST Call Forward No-Answer Feature*/
CFGITEM(SIP,  CallWaitConcept),          /*RST Call Waiting Feature*/
CFGITEM(SIP,  CallHoldConcept),          /*RST Call Hold Feature*/
CFGITEM(SIP,  CallXfrConcept),           /*RST Call Transfer Feature*/
CFGITEM(SIP,  CallBarrConcept),          /*RST Call Barring Feature*/
CFGITEM(SIP,  CIDDelConcept),            /*RST Caller ID Delivery Feature*/
CFGITEM(SIP,  DNDConcept),               /*RST Do not Disturb Feature*/
CFGITEM(SIP,  MWIConcept),               /*RST Message Waiting Indicator Feature*/
CFGITEM(SIP,  AutoRecallConcept),        /*RST Auto Recall Feature*/
CFGITEM(SIP,  AutoCallbackConcept),      /*RST Auto Callback Feature*/
CFGITEM(SIP,  BLVConcept),               /*RST Busy Line Verify Feature*/
CFGITEM(SIP,  EServicesConcept),         /*RST Emergency Services Feature*/
CFGITEM(SIP,  SCFConcept),               /*RST SCF feature*/
CFGITEM(SIP,  ThreeWayCallConcept),      /*RST Three Way Calling Feature*/
CFGITEM(SIP,  SubscrProgPinConcept),     /*RST Subscriber Programmable PIN Feature*/
CFGITEM(SIP,  ACRConcept),               /*RST Anonymous Call Rejection Feature*/
CFGITEM(SIP,  SolBlkConcept),            /*RST Solicitor Blocking Feature*/
CFGITEM(SIP,  DisAlertConcept),          /*RST Distinct Alerting Feature*/
CFGITEM(SIP,  SpDialConcept),            /*RST Speed Dialing Feature*/
CFGITEM(SIP,  COTConcept),               /*RST COT Feature*/
CFGITEM(SIP,  HeldMediaConcept),         /*RST Held Media Feature*/
CFGITEM(SIP,  LocalSpeedDialingConcept), /*RST Local Speed Dialing Feature*/
CFGITEM(SIP,  HotLineConcept),           /*HotLine Feature */
CFGITEM(SIP,  UnsolicitedMWIConcept),    /*Unsolicited MWI Concept*/
CFGITEM(SIP,  UnsolicitedUAProfileConcept),/*Unsolicited UA Profile Concept*/
CFGITEM(SIP,  FeatureCfg),               /*Item for reverse look up of PROV_CFG_ITEM
                                           Index is a feature ID string*/
CFGITEM(SIP,  FeatureAct),               /*Activation status of feature.  PROV_CFG_ITEM and User are
                                           indices.*/

/*
  SIP servers and Operator data - Accessible using the Domain name as the index.
*/
CFGITEM(SIP,  OperatorDomainConcept),    /*SIP Operator Information Group*/
CFGITEM(SIP,  DNSConcept),               /*SIP DNS servers Info*/
CFGITEM(SIP,  PCSCFConcept),             /*SIP PCSCF servers Info*/
CFGITEM(SIP,  BSFConcept),               /*SIP BSF servers info*/
CFGITEM(SIP,  PCSCFCritEvtCB),         /*Signals critical change via callback.  Cannot get/set*/

/*
  SIP User Information - Accessible using the UserID as the index.
  UserID of NULL will return info on all Users for device.
*/
CFGITEM(SIP,  UserConcept),              /*SIP User configuration group*/

/*
  SIP remote user Feature 
*/
CFGITEM(SIP,  RemoteUserSysConcept),     /*SIP remote user base configuration */
CFGITEM(SIP,  RemoteUserEntryConcept),   /*SIP remote user configuration -
                                             indexed by the UserID */

/*
  SIP Dect Status Settings
*/
CFGITEM(SIP,  DectStatusConcept),       /*SIP Dect Status Configuration */

/*
  Concept IDs for Endpoint - Accessible with endpoint number as the index
*/
CFGITEM(EPT,  MWIConcept),    /* This (EPT, MWIConcept) is obsolete.  The related cfgAccess 
                              ** functions have already been removed.  This enum should
                              ** be removed too */
CFGITEM(EPT,  CodecConcept),    /*Endpoint Codec configuration group*/
CFGITEM(EPT,  CodecVidConcept), /*Endpoint Codec (Video) configuration group*/
CFGITEM(EPT,  LineConcept),     /*Endpoint Line configuration group*/
CFGITEM(EPT,  AnnounceConcept), /*Endpoint Announcement configuration group*/
CFGITEM(EPT,  StatusConcept),   /*Endpoint Enabled Status*/
CFGITEM(EPT,  CritEvtCB),       /*Used to signal critical item change through CB - Cannot GET/SET */
/*Run time list of preferred codecs.  Acessbile with UserID and endpoint number as the index*/
CFGITEM(EPT,  CodecList),       /*SIP User configuration group*/
CFGITEM(EPT,  CodecVidList),    /*SIP User configuration group*/

/*
  Ids for Device Config - no ID necessary for per device information
*/
CFGITEM(PRV, DevConcept),
CFGITEM(PRV, ServerConcept),
CFGITEM(PRV, RealmConcept),
CFGITEM(PRV, SecConcept),
CFGITEM(PRV, Status),
CFGITEM(PRV, CfgFile),
CFGITEM(PRV, SysInfo),
CFGITEM(PRV, RingCadenceConcept),
CFGITEM(PRV, DevInternationalConcept),
CFGITEM(PRV, PulseSignalConcept),  /*index is CFGPULSESIGTYPE*/
CFGITEM(PRV, ToneConcept), /*index is CFGTONETYPE*/
CFGITEM(PRV, DectFPConcept),
CFGITEM(PRV, DectPPConcept),/*index is pp index*/
/*
 Ids for Security Config
*/
/*Still to come*/

/* MUST be the last Item */
CFGITEM(CFG, Unknown)
} PROV_CFG_ITEM;


typedef struct PROV_CFG_IDXtag
{
  BOS_UINT8 *pcCharIdx; /*ASCII string index (ie. User Name, Realm name)*/
  BOS_UINT32 uIndex;    /*Integer index (ie. Endpoint number, tone ID)*/
  struct PROV_CFG_IDXtag *pNext;
} PROV_CFG_IDX;

/*
**=============================================================================
** Function:   cfgAccess()
** Purpose:    Main CfgAccess function to serve SIP client
** Parameters:
**         cfgItem - ID of item being accessed
**         pcCharIdx - Index for an item.  This can be an ASCII string or a byte string.  Depends
**                     on the item being accessed.
**         pCfgItemDataValue - pointer to structure containing value. Calling function
**                                 required to free memory.
**             cfgAccessDir - CFG_ITEM_GET or CFG_ITEM_SET.
** Return:     BOS_STATUS_OK if OK, BOS_STATUS_ERROR - otherwise
**=============================================================================
 */
BOS_STATUS CfgAccess (
      PROV_CFG_ITEM        cfgItem,
      PROV_CFG_IDX         *pCfgIndex,
      void                 *pCfgItemDataValue,
      CFG_ACCESS_ACT       cfgAccessDir);


/*Structure for representing Activation Status and Info.  Used for several concepts*/
typedef enum
{
   CFG_SIP_ADMIN_STATUS_ACTIVE = 1,
   CFG_SIP_ADMIN_STATUS_INACTIVE
} CFGSIPADMINSTATUS;

typedef enum
{
   CFG_SIP_OPER_STATUS_ACTIVE = 1,
   CFG_SIP_OPER_STATUS_INACTIVE,
   CFG_SIP_OPER_STATUS_NOTPRESENT,
   CFG_SIP_OPER_STATUS_UNKNOWN
} CFGSIPOPERSTATUS;


typedef struct
{
  CFGSIPADMINSTATUS  sAdminStatus;  /* Network Activation Status */
  BOS_UINT8   *pcAdminStatInfo;  /*Info string for Network Admin Status*/
  CFGSIPOPERSTATUS  sOperStatus;  /* operational status reported to network*/
  BOS_UINT8  *pcOperStatInfo; /*Info string for operational status*/
  BOS_BOOL bCfgStatus; /*Config status - can this config be used?*/
}CFGSIPACTSTATUS;


/********************************************************************************
**                       Structures for RST Features
**   The structures for RST features are acessed on a per USer ID basis.
**   So, when using CfgAccess(), provide the User ID in the pcCharIdx parameter.
**
**   The list of features supported is:
**
**     - Digit Map
**     - Basic Call
**     - Announcement
**     - Status Change
**     - No Answer Timeout
**     - Caller Id
**     - Caller Id Display
**     - Caller Id Blocking
**     - Caller Id Delivery
**     - Call Forwarding
**     - Call Waiting
**     - Call Hold
**     - Call Transfer
**     - Call Barring
**     - Three Way Calling
**     - Do Not Disturb
**     - Subscriber PIN
**     - Msg Wait Indicator
**     - Auto Recall
**     - Auto Callback
**     - Busy Line Verify
**     - Emergency Services
**     - SCF
**     - HotLine 
**
*********************************************************************************/


/*API for Digit Map Feature*/

typedef struct CFGSIPDIGITMAPtag
{
  CFGSIPACTSTATUS Act; /*Activation Status*/
  BOS_UINT8  *pABNF; /*Digit Map ABNF (value)*/
} CFGSIPDIGITMAP;


/*API for Digit Map External Variable Feature*/
#define CFG_DM_EXT_VAR_VAL_MAX_SIZE  256
typedef struct CFGSIPDIGITMAPEXTVARtag
{
  CFGSIPACTSTATUS Act; /*Activation Status*/
  BOS_UINT8  *pValue;  /*Value of the External Variable*/
} CFGSIPDIGITMAPEXTVAR;


/*API structure for Basic Call Feature*/

typedef struct CFGSIPBASICCALLtag
{
  CFGSIPACTSTATUS Act;    /*Activation Status*/
  BOS_UINT8 *pcPrefCodecList;       /*Preferred Codec List*/
  BOS_UINT32 uByeDelay;   /*Bye Delay Timer*/
  BOS_UINT32 uOrigDTTimer;  /*Origination Dial Tone Timer*/
  BOS_UINT32 uOrigLITimer;  /*Origination Long Interdigit Timer*/
  BOS_UINT32 uOrigRBKTimer;  /*Origination Ringback Timer*/
  BOS_UINT8 *pTermOHErrSig;   /*Termination mode off-hook error signal*/
  BOS_UINT32 uTermErrSigTimer;  /*Termination mode off-hook error signal timer*/
  BOS_UINT8 *pPermSeqTone1; /*Permanent sequence tone 1*/
  BOS_UINT32 uPermSeqTimer1;  /*Permanent sequence tone 1 timer*/
  BOS_UINT8 *pPermSeqTone2; /*Permanent sequence tone 2*/
  BOS_UINT32 uPermSeqTimer2;  /*Permanent sequence tone 2 timer*/
  BOS_UINT8 *pPermSeqTone3; /*Permanent sequence tone 3*/
  BOS_UINT32 uPermSeqTimer3;  /*Permanent sequence tone 3 timer*/
  BOS_UINT8 *pPermSeqTone4; /*Permanent sequence tone 4*/
  BOS_UINT32 uPermSeqTimer4;  /*Permanent sequence tone 4 timer*/
  BOS_UINT32 uLORTimer; /*Lockout Reset Timer*/
  BOS_UINT32 uNEMDSCPValueMedia; /*Non emergency media DSCP value*/
  BOS_UINT32 uNEMDSCPValueSig; /*Non emergency sig DSCP value*/
  BOS_BOOL bIssueNetDisc; /*Control over Network Disconnect signal*/
  BOS_BOOL bIgnoreHfNoFeat; /* Control whether to ignore hook flash if no feature
                            ** can make use of it. */
  BOS_UINT8 *pHoldRemTone; /* On hold reminder tone. */
  BOS_BOOL bHfDisabled; /* Control whether to disable hookflash */
  BOS_BOOL bEndCallsOnOos; /* Control whether to terminate established calls when
                           ** out-of-service event is reported */
  BOS_BOOL bHfTermOosCall; /* Control whether to terminate out-of-service calls on
                          ** hookflash */
  BOS_BOOL bRestoreOnHoldTimeout; /* Control whether to restore or terminate the current
                              ** call when a hold attempt fails due to signalling timeout
                              */
  BOS_BOOL bMapRegEvtRej2Deact; /* Control over reg-event mapping */
  BOS_UINT32 uNetDiscDelay; /*Delay for Net Disconnect Signal*/
  BOS_BOOL bInterDigTmrOnRecall; /* Control whether to apply interdigit timer
                                 ** on recall feature or not. */
  BOS_BOOL bDisableRegEvtSub; /*Control of subscription to reg event package*/                               
  BOS_BOOL bPresetMedSes; /* Control whether to preset the media session */
  BOS_BOOL bInDialogDtmf; /* Control whether to send DTMF within dialog */
  BOS_BOOL bReportPartialDS; /* Control whether to send unmatched dialstring with the onhook 
                             ** event callback */
  BOS_BOOL bHfReported; /* Control whether to report hookflash via SIP INFO*/
  BOS_UINT8 *pcHFMIMEType;/* HookFlash MIME Type*/
  BOS_UINT8 *pcHFInfoContent;/* HookFlash Info Content*/
  BOS_BOOL bRRFlash; /* Control whether to allow Register Recall (Euro) style hookflash followed 
                     ** by DTMF signalling to activate features such as CW or 3WC */

} CFGSIPBASICCALL;

/*API structure for Basic Call Feature*/
typedef struct CFGSIPINTERNALCALLtag
{        
   BOS_UINT8 * pcIntCallClidName;     /* This is the clid name that will be displayed on a internal call callees */   
   BOS_UINT8 * pcIntCallClidNumber;   /* This is the clid number that will be displayed on a internal call callees */   
} CFGSIPINTERNALCALL;

/*API structures for Announcements Feature*/

/*This structure represents the Announcement MAP entry URI corresponding to a response code.
  It is returned as part of the announcements feature query*/
typedef struct CFGSIPANCRSPCODEtag
{
  BOS_UINT32  uAncRspCode;  /*Response Code*/
  BOS_UINT8   *pcAncURI;   /*URI string for response code*/
  struct CFGSIPANCRSPCODEtag *pNext;
} CFGSIPANCRSPCODE;

/*This structure represents the Announcement Media MAP entry corresponding to a an announcement
  identifier. It is returned as part of the announcements feature query*/
typedef struct CFGSIPANCMEDIAMAPtag
{
  BOS_UINT8 *pcMediaId;  /*Announcement Identifier*/
  BOS_UINT8 *pcMediaURI; /*URI for announcement identifier*/
  BOS_UINT32  uMediaCachMaxAge; /*Announcement Media Cache Maximum Age*/
  struct CFGSIPANCMEDIAMAPtag *pNext;
} CFGSIPANCMEDIAMAP;

/*This structure represents the Local Media entries. It is returned as part
of the announcements feature query*/
typedef struct CFGSIPANCLOCALMEDIAtag
{
  BOS_UINT8 *pcMediaURI;  /*URI for the Local Media*/
  BOS_UINT8 *pcMediaType; /*Media Type*/
  BOS_UINT8 *pcMediaData; /*Media Data entry*/
  struct CFGSIPANCLOCALMEDIAtag *pNext;
} CFGSIPANCLOCALMEDIA;

/*This structure represents the RST announcements feature*/
typedef struct CFGSIPANCtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT8   *pcPrefLang;/*Preferred language for UE announcements*/
  BOS_UINT8 *pcResURI;  /*Announcement Resource URI for the media server*/
  BOS_UINT8 *pcDomain;  /*Announcement Domain*/
  BOS_UINT8 *pcPath;  /*Announcement Path*/
  BOS_UINT8 *pcMIMEType;/*Announcement MIME Type*/
  CFGSIPANCRSPCODE  *pAncRspCodeList; /*Response code and URI list*/
  CFGSIPANCMEDIAMAP *pAncMediaMap;  /*Network based announcement Media MAP entries*/
  CFGSIPANCLOCALMEDIA *pAncLocalMedia;/*Network announcement Local Media list*/
} CFGSIPANC;

/*API for ACT STAT Feature*/

typedef struct CFGSIPACTSTATtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT32 uChgRegExp;  /*UE ActStatus Registration Expiration*/
} CFGSIPACTSTAT;


/*API for No Answer Feature*/
typedef struct CFGSIPNOANStag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT32 uTODuration; /*No Answer Timeout Duration*/
} CFGSIPNOANS;


/*API for Call ID Feature*/
typedef enum
{
   CFG_SIP_CID_PPS_ANONYMOUS = 1,
   CFG_SIP_CID_PPS_PUBLIC,
   CFG_SIP_CID_PPS_ID,
   CFG_SIP_CID_PPS_LOCAL

} CFGSIPCIDPPS;

typedef struct CFGSIPCIDtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  CFGSIPCIDPPS  sPPS;   /*Permanent Presentation ActStatus*/
  BOS_BOOL bNotCritical;/*Critical advertized (default) or not*/
} CFGSIPCID;

/*API for Call ID Display Feature*/
#define MAXLEN_CIDDISPLAY_MOVE_TIME 11
typedef struct CFGSIPCIDDISPLAYtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_BOOL  bCNDActStat;  /*Activation Status for CND (Call Number Display)*/
  BOS_BOOL  bCNAMDActStat;  /*Activation Status for CNAMD (Call Name Display)*/
  BOS_UINT8 *pcDefCountry;  /*Default Country*/
  BOS_SINT32 sTimeAdj; /* Time adjustment from local invariant time to time at current location (in minutes)*/
  BOS_SINT32 sDSTTimeAdj; /* DST adjustment (in minutes)*/
  BOS_BOOL bCIDCWBlock; /* Activation status for Call Waiting Call ID Display */
} CFGSIPCIDDISPLAY;


/*API for Call ID Blocking Feature*/

typedef struct CFGSIPCIDBLOCKtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT8 *pcConfTone;/*Confirmation tone after vertical feature code*/
  BOS_UINT8 *pcErrTone; /*Error tone after vertical feature code*/
} CFGSIPCIDBLOCK;

/*API for Call Delivery Feature*/

typedef struct CFGSIPCIDDELtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT8 *pcConfTone;/*Confirmation tone after vertical feature code*/
  BOS_UINT8 *pcErrTone; /*Error tone after vertical feature code*/
} CFGSIPCIDDEL;


/*API for Call Forward Feature*/

typedef struct CFGSIPCALLFWDtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_BOOL  bRingReminder;  /*Call Forward Ring Reminder on/off*/
  BOS_UINT32  uSubDuration; /*Subscription duration*/
  BOS_UINT8   *pcAUID;  /*app unique ID*/
  BOS_BOOL  bSpecialDialTone; /*Special Conditions dial tone when forwarded*/
  BOS_BOOL bIgnoreCntCheck; /* Ignore counter check */
  BOS_UINT8 uCount; /*Call forward count*/
  BOS_UINT8 *pcFwdToUri;  /*Forward to URI*/
  BOS_UINT8 *pcFwdToDString;  /*Forward to DialString*/
  BOS_BOOL  bUseFwdDString;
  BOS_BOOL bUsrSet;  /*User enabled call forward*/
  BOS_BOOL bRSOnInitCfv;  /* Play RS on first CFV NOTIFY, note that any NOTIFY 
                          ** with cfv count set to 1 will be considered an 
                          ** initial CFV
                          */
} CFGSIPCALLFWD;


/*API for Call Forward Busy Feature*/

typedef struct CFGSIPCALLFWDBUSYtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
} CFGSIPCALLFWDBUSY;


/*API for Call Forward No-Answer Feature*/

typedef struct CFGSIPCALLFWDNOANStag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
} CFGSIPCALLFWDNOANS;


/*API for Call Waiting Feature*/

typedef struct CFGSIPCALLWAITtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_BOOL bNoCallWaitTO; /* Do not time out on call waiting (unanswered) */
  BOS_BOOL bCallWaitCancel; /*Call Wait Cancel Enabled*/
  BOS_UINT32 uCallWaitDisconnectTiming; /*CallWaitDisconnectTiming*/
  BOS_BOOL bNoCallWaitTracking; /* Do not track call waiting session */
} CFGSIPCALLWAIT;

/*API for Call Hold Feature*/

typedef struct CFGSIPCALLHOLDtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT8 *pcConfirm; /*Call Hold Feature activation/deactivation confirmation indicator*/
} CFGSIPCALLHOLD;

/*API for Call Transfer Feature*/

typedef struct CFGSIPCALLXFERtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT32  uNotifyTO;  /*Notify Timeout*/
  BOS_BOOL    bInDialogRefer; /*Enable/Disable in-dialog refer support*/
  BOS_BOOL    bIncomingOnly; /* invoke call transfer only if 1st call leg was incoming */
} CFGSIPCALLXFER;

/*API for Call Barring Feature*/

typedef struct CFGSIPCALLBARRtag
{
   CFGSIPACTSTATUS Act; /* Activation Status */
   BOS_BOOL bBarrDm;    /* Block only calls matched in the barring rules list */
   BOS_BOOL bBarrAll;   /* Block all outgoing calls */
   BOS_UINT8 *pcBarringPIN; /* Call barring PIN */
} CFGSIPCALLBARR;

/*API for Do Not Disturb Feature*/

typedef struct CFGSIPDNDtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT8 *pcActConfirm;  /*Do Not Disturb Activation Confirmation Indicator*/
  BOS_UINT8 *pcDeActConfirm;/*Do Not Disturb DeActivation Confirmation Indicator*/
  BOS_UINT8   *pcAUID;  /*app unique ID*/
} CFGSIPDND;


/*API for Network MWI Feature*/

typedef struct CFGSIPMWItag
{
  CFGSIPACTSTATUS Act; /*Activation Status*/
  BOS_UINT32  uSubDuration; /*MWI subscription duration*/
  BOS_BOOL bMwiSrvSet; /* Server for MWI support. */
  BOS_IP_ADDRESS mwiSrvAddr; /* MWI Server Address */
  BOS_UINT8 *pcMwiSrvDomain; /* MWI Server FQDN */
  BOS_UINT32 uSIPPort;  /* MWI Server Port */
  BOS_BOOL bStutterTone; /* Audible Message Waiting Indication control. */
  BOS_BOOL bRemoveVMWI; /* Visual Message Waiting Indication control. */
  BOS_BOOL bClearMWI; /* MWI state that is read/write - true=indicator off, false=indicator on*/
  BOS_BOOL bNoOffhookMWI;  /* instructs the call manager to hold on to VMWI if user is offhook 
                              and only send it when user goes onhook */
} CFGSIPMWI;

/*API for Auto Recall and Auto Callback Feature*/

typedef struct CFGSIPAUTOCALLtag
{
  CFGSIPACTSTATUS Act;/*Activation Status*/
  BOS_UINT32  uTimer; /*Auto recall/callback timer (minutes)*/
  BOS_UINT32  uSpRingDuration;/*Auto recall/callback special ring duration (number of rings)*/
  BOS_UINT32  uSpRngRetryTime;/*Auto recall/callback special ringing retry wait interval*/
  BOS_UINT32  uSpRngRetries;  /*Auto recall/callback special ring retries*/
  BOS_UINT32  uMaxSubSend;  /*Auto Recall/callback max number of simultaneous subscribes to send*/
  BOS_UINT32  uMaxSubRec; /*Auto Recall/callback max number of simultaneous subscribes to honor*/
} CFGSIPAUTOCALL;

/*API for Busy Line Verify Feature*/

typedef struct CFGSIPBLVtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT8 *pcOperId;  /*BLV Operator ID*/
} CFGSIPBLV;

typedef struct CFGSIPEMERGEtag
{
  CFGSIPACTSTATUS Act; /*Activation Status*/
  BOS_UINT32  uNwHoldTimer; /*Network Hold timer (minutes)*/
  BOS_UINT32  uHowlTimer;   /*Howler Timer*/
  BOS_UINT32  uMediaDSCPVal;/*Emergency Media DSCP value*/
  BOS_UINT32  uSigDSCPVal;/*Emergency Sig DSCP value*/
  BOS_BOOL    bNoLocInfo; /* Disable location information */
  BOS_BOOL    bEstab3WC; /* Allow creation of a 3-way call by local user. */
  BOS_BOOL    bNwHoldDisable; /* Disable support for network hold. */
  BOS_BOOL    bNwHoldBypassChk; /* Ignore incorrectly formatted hold response. */
  BOS_BOOL    bEndCallAcptIncEmerg; /* End all calls and accept incoming emergency call */
} CFGSIPEMERGE;

/*API for SCF configuration*/

typedef struct CFGSIPSCFtag
{
  CFGSIPACTSTATUS Act; /*Activation Status*/
  BOS_BOOL  bRingReminder; /*Call Forward ring reminder*/
  BOS_UINT8   *pcAUID;  /*app unique ID*/
  BOS_UINT8 uCount; /*Call forward count*/
} CFGSIPSCF;

/*API for Three Way Calling Feature*/
typedef enum
{
   CFG_SIP_CONFERENCING_LOCAL = 1,
   CFG_SIP_CONFERENCING_REFER_SERVER,
   CFG_SIP_CONFERENCING_REFER_PARTICIPANTS,
} CFGSIPCONFOPTION;

typedef struct CFGSIPTHREEWAYCALLtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_UINT32 uCallDisconnectTiming;
  BOS_BOOL   bToggleEnable;  /* Enable support for toggling calls if hook flash is pressed
                             ** while in 3wc.  Note that when enabled, hook flash will no 
                             ** longer disconnect the second leg of the 3wc, instead it will
                             ** terminate the 3wc while putting the first leg on hold.
                             ** Another hook flash will put the second leg on hold and unhold
                             ** the first leg.
                             */
  BOS_UINT8   *pConfUri;
  CFGSIPCONFOPTION eConfOption;
} CFGSIPTHREEWAYCALL;

/*API for Subscriber Programmable PIN Feature*/

typedef struct CFGSIPSUBSCRPROGPINtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
} CFGSIPSUBSCRPROGPIN;

/*API for ACR Feature*/

typedef struct CFGSIPACRtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_BOOL bLocalAcrEnabled;  /*Local ACR enabled*/
} CFGSIPACR;

/*API for Solicitor Blocking Feature*/

typedef struct CFGSIPSOLBLKtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
} CFGSIPSOLBLK;

/*API for Distinct Alerting Feature*/

typedef struct CFGSIPDISALERTtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
} CFGSIPDISALERT;

/*API for Speed Dialing Feature*/

typedef struct CFGSIPSPDIALRtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
} CFGSIPSPDIAL;

/*API for Customer Originated Call Trace Feature*/

typedef struct CFGSIPCOTtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
} CFGSIPCOT;

/*API for Held Media Feature*/
typedef struct CFGSIPHELDMEDIAtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_BOOL  bHeldMediaEnabled; /*Held Media Enabled*/
} CFGSIPHELDMEDIA;


typedef struct CFGSIPLOCALSPEEDDIALENTRYtag
{
   BOS_UINT8 *pcSpeedDialLocalMapCode;
   BOS_UINT8 *pcSpeedDialLocalMapDigitString;
   struct CFGSIPLOCALSPEEDDIALENTRYtag *pNext;   
} CFGSIPLOCALSPEEDDIALENTRY;

/*API for Local Speed Dialing Feature*/
typedef struct CFGSIPLOCALSPEEDDIALINGtag
{
   CFGSIPACTSTATUS Act;  /*Activation Status*/
   CFGSIPLOCALSPEEDDIALENTRY *pSpeedDialList;   
} CFGSIPLOCALSPEEDDIALING;

/*API for HotLine Feature*/

typedef struct CFGSIPHOTLINEtag
{
  CFGSIPACTSTATUS Act;      /*Activation Status*/
  BOS_UINT8  *pHotLineUri;  /*HotLine URI*/
  BOS_UINT32 uOffHookTimer; /*Offhook timer*/
  BOS_BOOL  bDisableHfFeat; /*Disables hookflash features */
} CFGSIPHOTLINE;


/*
   API for Unsolicited Notify Features:
    CFGITEM(SIP,  UnsolicitedMWIConcept)
    CFGITEM(SIP,  UnsolicitedUAProfileConcept)
*/
typedef struct CFGSIPUNSOLICITEDNOTIFYtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
} CFGSIPUNSOLICITEDNOTIFY;


/***************************************************************
**      SIP Operator Information
**      This is information related to a particular domain and
**      is accessed via the domain name.
**
****************************************************************/
/*General structure for SIP credentials*/
typedef enum
{
  CFG_SIP_USER_CRED_TYPE_OTHER = 1,
  CFG_SIP_USER_CRED_TYPE_NONE,
  CFG_SIP_USER_CRED_TYPE_PASSWORD,
  CFG_SIP_USER_CRED_TYPE_PRESHAREDKEY,
  CFG_SIP_USER_CRED_TYPE_CERTIFICATE
} CFGSIPCREDTYPE;

typedef struct
{
  CFGSIPCREDTYPE  CredType;
  BOS_UINT8   *pCredVal;
} CFGSIPCREDENTIAL;
/*API for Operator Info configuration*/

typedef struct CFGSIPOPERATORtag
{
  /*BOS_UINT8  *pcDomain;*/  /*Remove this?*/
  BOS_IP_ADDRESS STUNServerAddr;  /*Address of STUN server*/
  BOS_UINT32  uSTUNServerPort;    /*Port number of STUN server*/
  BOS_UINT8 *pcSTUNServerDomain;  /*Domain Name (FQDN) of STUN server if provided. NULL if not provided*/
  BOS_IP_ADDRESS TURNAddr;   /*TURN Address*/
  BOS_UINT32  uTURNPort;     /*TURN Port*/
  BOS_UINT8 *pcTURNDomain;  /*Domain Name (FQDN) of TURN if provided. NULL if not provided*/
  CFGSIPCREDENTIAL TURNCred; /*TURN Credentials*/
} CFGSIPOPERATOR;

/*API for list of DNS servers for SIP*/

typedef struct CFGSIPDNSSERVERStag
{
  BOS_IP_ADDRESS DnsAddr;   /*DNS IP Address*/
  BOS_UINT8 *pcDnsDomain;
  struct CFGSIPDNSSERVERStag *pNext;
} CFGSIPDNSSERVERS;

/* for Protocols used for SIP */
typedef enum
{
   CFG_SIP_PROT_ID_OTHER = 1,
   CFG_SIP_PROT_ID_UDP = 2,
   CFG_SIP_PROT_ID_TCP = 3,
   CFG_SIP_PROT_ID_TLS = 4
} CFGSIPPROTID;

typedef enum
{
 CFG_PCSCSF_DISC_SUCCESS = 1,
 CFG_PCSCSF_DISC_FAILURE = 2  
}CFGPCSCSFDISCOVERYSTATE ;

typedef struct CFGPCSCFTARGETADDRLISTtag
{
  BOS_IP_ADDRESS ipAddr;   /*IP Address*/
  struct CFGPCSCFTARGETADDRLISTtag *pNext;
} CFGPCSCFTARGETADDRLIST;

/* Outbound proxy ip-address and port
*/
typedef struct CFGSIPOBPROXY
{
   BOS_UINT8   *pcAddr;    /* Outbound proxy server */
   BOS_UINT32  uPort;      /* OBProxy port */
} CFGSIPOBPROXY;

/* Ordered list of outbound proxies
*/
typedef struct CFGSIPOBPROXYLST
{
   CFGSIPOBPROXY  obProxy[CFG_MAX_ADDRLST];  /* Outbound proxy address and port. */
   BOS_UINT32     uNum;                      /* Number of valid(non-zero) outbound proxies */ 
} CFGSIPOBPROXYLST;

/*API for list of PSCF servers for SIP*/
typedef struct CFGSIPPCSCFtag
{
  BOS_IP_ADDRESS PCSCFAddr; /*PCSCF Address*/
  BOS_UINT8 *pcPCSCFDomain;
  BOS_UINT32  uSIPPort;  /*SIP Port for sending */
  CFGSIPPROTID sProtIdUsed;  /*SIP Protocol*/
  BOS_IP_ADDRESS PCSCFAddrUsed; /*PCSCF Address Used*/
  CFGSIPOBPROXYLST obProxyLst; /* Ordered outbound proxy list. The first proxy in
                               ** the list is the default primary proxy, and the
                               ** the remaining are failover proxies to be tried in
                               ** corresponding order.
                               */
  BOS_UINT32  uTimerT1;  /*SIP Timer T1*/
  BOS_UINT32  uTimerT2;  /*SIP Timer T2*/
  BOS_UINT32  uTimerT4;  /*SIP Timer T4*/
  BOS_UINT32  uTimerTD;  /*SIP Timer D */
  BOS_UINT32  uTimerTB;  /*SIP Timer B */
  BOS_UINT32  uTimerTH;  /*SIP Timer H */
  BOS_UINT32  uTimerTF;  /*SIP Timer F */

  BOS_UINT32  uSigTrans; /* Signaling transport bitmap. */
  BOS_BOOL    bIgnoreTcpFallBack; /* Control use of TCP as transport protocol for
                                  ** UDP signaling over the MTU size.
                                  */
  BOS_BOOL    bBackupRecovery; /* Control use of the internal backup and
                               ** recovery feature.
                               */
                                                             
  BOS_UINT32  uMaxTime;         /* 'max-time' SIP Registration Recovery Timer (in msec) */
  BOS_UINT32  uBaseTimeAllFailed;  /* 'base-time (if all failed)' SIP Registration Recovery Timer (in msec) */
  BOS_UINT32  uBaseTimeAllNotFailed;  /* 'base-time (if all have not failed)' SIP Registration Recovery Timer (in msec)*/
  BOS_UINT32  uSubscribeRetry;  /* retry period for initial SUBSCRIBE due to error responses */

  BOS_UINT32  uRegRetryTimerOn403; /*registration retry timer (in seconds) on 403 response*/

  /*Data for PCSCF Discovery*/
  CFGPCSCSFDISCOVERYSTATE pcscfDiscState;
  CFGPCSCFTARGETADDRLIST *pTargetAddrList;
  time_t  uAddrListExpiryTime;
  BOS_UINT32 uDiscoveryRetryMin;
  BOS_UINT32 uDiscoveryRetryMax;
  BOS_UINT32 uDiscoveryRetryNegativeTTL;

  BOS_UINT32 uPrimaryRecoveryTimerMin; /* The min value (in seconds) to attempt 
                                       ** to register back to the 'primary' server
                                       ** Used with uPrimaryRecoveryTimerMax where
                                       ** value is calculated as rand(MinTmr, MaxTmr).
                                       ** Set to 0 to disable using the random timer.
                                       */
  BOS_UINT32 uPrimaryRecoveryTimerMax; /* The max value (in seconds) to attempt 
                                       ** to register back to the 'primary' server
                                       ** Used with uPrimaryRecoveryTimerMin where
                                       ** value is calculated as rand(MinTmr, MaxTmr).
                                       ** Set to 0 to disable using the random timer.
                                       */
  struct CFGSIPPCSCFtag *pNext;
} CFGSIPPCSCF;

/*API for BSF configuration*/

typedef struct CFGSIPBSFtag
{
  BOS_IP_ADDRESS BSFAddr; /*BSF Address*/
  BOS_UINT8 *pcBSFDomain;
  BOS_UINT8  *pcASType;   /*AS Type string*/
  struct CFGSIPPCSCFtag *pNext;
} CFGSIPBSF;

/***************************************************************
**      SIP User Information
**      This is information related to a particular User and
**      is accessed via the user name.
**
****************************************************************/
/*User to endpt mapping.  Included as part of complete user configuration
and not accessible otherwise*/
typedef struct CFGSIPUSERENDPNTMAPtag
{
  BOS_UINT32 sEndpntNum;  /*Line number of endpoint*/
  BOS_BOOL   bIn;    /*User is inbound for this endpoint*/
  BOS_BOOL   bOut; /*User is outbound for this endpoint*/
  struct CFGSIPUSERENDPNTMAPtag *pNext;
} CFGSIPUSERENDPNTMAP;

/*Domain list for a given user.  Included as part of complete user configuration
and not accessible otherwise*/
typedef struct CFGSIPUSERDOMAINLISTtag
{
  BOS_UINT8  *pcDomain;  /*Domain name for user*/
  BOS_UINT32  uDomainID;  /*ID for this domain, can be used to query DNS server list*/
  struct CFGSIPUSERDOMAINLISTtag *pNext;
} CFGSIPUSERDOMAINLIST;

/*Enum for User Types*/
typedef enum
{
  CFG_SIP_USER_TYPE_OTHER = 1,
  CFG_SIP_USER_TYPE_GRUU,
  CFG_SIP_USER_TYPE_PUBLICIDENTITY,
  CFG_SIP_USER_TYPE_PRIVATEIDENTITY,
  CFG_SIP_USER_TYPE_PUBLICPRIVATEPAIR,
  CFG_SIP_USER_TYPE_USERNAME,
  CFG_SIP_USER_TYPE_MACADDRESS,
  CFG_SIP_USER_TYPE_PACKETCABLEIDENTITY
} CFGSIPUSERIDTYPE;

/*Enum for User registration status*/
typedef enum
{
  CFG_SIP_USER_REG_OK = 1,
  CFG_SIP_USER_REG_NOK,
  CFG_SIP_USER_REG_NOT_PRESENT,
  CFG_SIP_USER_REG_UNKNOWN,
  CFG_SIP_USER_REG_FORBIDDEN
} CFGSIPUSERREGSTATUSTYPE;

/* Enum for user type */
typedef enum
{
   CFG_SIP_USER_TERMINAL,
   CFG_SIP_USER_LINE,
} CFGSIPTERMTYPE;


typedef enum
{
   CFG_SILENCE_SUPP_ON = 1,
   CFG_SILENCE_SUPP_OFF, /*Silence suppression off*/
   CFG_SILENCE_SUPP_NO_SDP, /*Silence suppression off, do not report in SDP*/
   CFG_SILENCE_SUPP_ON_NO_RTP_MAP  /*Silence suppression on, do not report CN in rtp map*/
}CFGSILENCESETTING;

/* Enum for SRTP option type */
typedef enum
{
   CFG_SIP_SRTP_DISABLED,
   CFG_SIP_SRTP_OPTIONAL,
   CFG_SIP_SRTP_MANDATORY,
   CFG_SIP_SRTP_MAX
} CFGSIPSRTPPREF;


/* Complete User Configuration Structure
  - To obtain the full list of configured Users, perform
    a "get" action with a NULL userID.
*/
typedef struct CFGSIPUSERCFGtag
{
  CFGSIPUSERIDTYPE sUserType; /*User ID type*/
  BOS_UINT8  *pcUserId; /*public User ID (IMPU)*/
  BOS_BOOL    bUserIdFromTempInfo;  /* Flag to indicate that IMPU is based on temporary info */
  BOS_UINT8  *pcDisplayInfo; /*User Display Info*/
  CFGSIPUSERDOMAINLIST *pDomainList; /*List of Domains for User*/
  CFGSIPACTSTATUS Act; /*Activation Status*/
  BOS_BOOL   bSigSecurity; /*Flag for secure SIP signaling support*/
  CFGSIPUSERIDTYPE sPrivIdType;
  BOS_UINT8  *pcPrivId; /*private ID (IMPI)*/
  CFGSIPCREDENTIAL credentials; /*User Credentials*/
  CFGSIPUSERENDPNTMAP *pEndPntMap; /*List of endpoint mappings for User*/
  BOS_UINT8 *pcActiveDomain;  /*Domain in use by this User*/
  BOS_UINT32 uActiveDomain; /* Domain index in use by this User */
  BOS_UINT32 uInitRegDelay; /* Initial registration delay */
  BOS_UINT32 uReRegMaxTime;
  BOS_UINT32 uRegRetryTimerMin; /*registration backoff retry timer min ( in ms )*/
  BOS_UINT32 uRegRetryTimerMax; /*registration backoff retry timer max ( in ms )*/
  CFGSIPUSERREGSTATUSTYPE sRegStatus;
  BOS_UINT32 uExp;      /* Expiration time */
  BOS_UINT32 uReReg;    /* Re-registration time */
  BOS_BOOL   bPrimary;  /* Indicates if registered to primary proxy or not */
  BOS_UINT32 uIMPUIndex;    /* IMPU index for this user in IMPU Table */  
  BOS_UINT8  *pcPAURIs; /* P-Associated-URI list */
  CFGSILENCESETTING eSilSupp;
  CFGSIPSRTPPREF eSrtpPref;

  struct CFGSIPUSERCFGtag *pNext;
} CFGSIPUSERCFG;

/* CCTK user configure structure
*/
typedef struct CFGSIPCCTKCFGtag
{
   BOS_UINT32 uHdl; /* Public handle used to identify this user */
   BOS_UINT8 *pcUserId; /* Public User ID */
   BOS_UINT8 *pcDisplayInfo; /* User Display info */
   CFGSIPUSERDOMAINLIST *pDomainList; /*List of Domains for User*/
   BOS_BOOL bSigSecurity; /* Flag for secure SIP signaling */
   CFGSIPUSERIDTYPE sPrivIdType;
   BOS_UINT8  *pcPrivId; /*private ID (IMPI)*/
   BOS_UINT8  *pcDefDialNum; /* Default src number to use if no dialstr was given
                             ** in ALERT_CALL request. */
   CFGSIPCREDENTIAL credentials; /*User Credentials*/
   BOS_UINT8 *pcActiveDomain;  /*Domain in use by this User*/
   BOS_UINT32 uActiveDomain; /* Domain index in use by this User */
   BOS_UINT32 uInitRegDelay; /* Initial registration delay */
   BOS_UINT32 uReRegMaxTime;
   BOS_UINT32 uRegRetryTimerMin; /*registration backoff retry timer min*/
   BOS_UINT32 uRegRetryTimerMax; /*registration backoff retry timer max*/
   CFGSIPUSERREGSTATUSTYPE sRegStatus;
   BOS_BOOL bCctkUsrEnabled; /* Flag for whether this CCTK user is enabled or not */
   CFGSILENCESETTING eSilSupp;

   struct CFGSIPCCTKCFGtag *pNext;
} CFGSIPCCTKCFG;

typedef struct CFGROUTINGTABLEtag
{
   BOS_UINT32 uTerminal;
   BOS_UINT32 uLine;
} CFGROUTINGTABLE;

/* CCTK routing table structure
*/
typedef struct CFGSIPCCTKROUTINGTABLEtag
{
   BOS_UINT32 uLength;                 /* The total length of the associated line routing table */
   CFGROUTINGTABLE* pLineAssocTable;   /* The line association table. */
} CFGSIPCCTKROUTINGTABLE;

/***************************************************************
**      SIP Remote User Information
**      This is information related to a particular SIP remote user
**      and is accessed via the user name.
**
****************************************************************/
/* API for the Remote User Configuration */
typedef struct CFGSIPREMOTEUSERCFGtag
{
   BOS_UINT32 uIndex;        /* eueMgmtIMGUsrIndex for this user */
   BOS_UINT8 *pcUserId;      /* remote user Uuid */
   BOS_UINT8 *pcAddr;        /* address of the remote user */
   BOS_UINT32 uSipPort;      /* port of the remote user */
   
   BOS_UINT32 uAssociatedIMPUIndexRef;  /* SIP remote user associated IMPU Index */
   BOS_UINT8 *pcAssociatedUserId; /* local user id associated with this remote 
                                  ** based on eueMgmtIMGUsrAssociatedIMPUIndexRef */   
   
   BOS_UINT32 uIMPIIndexRef;      /* SIP remote user associated IMPI Index */                                  
   BOS_UINT8 *pcIMPIUserUserId;   /* User Id from IMPI table */
   CFGSIPCREDENTIAL credentials;  /* User Credentials from IMPI table 
                                  ** based on eueMgmtIMGUsrIMPIIndexRef */
                                 
   BOS_BOOL   bDynCreated; 
   BOS_UINT8  *pcDisplayInfo; /*User Display Info*/   
   CFGSIPACTSTATUS Act; /*Activation Status*/
   BOS_UINT8  *pcAdditionalInfo; /*User Display Info*/  

   struct CFGSIPREMOTEUSERCFGtag *pNext;
} CFGSIPREMOTEUSERCFG;

typedef enum
{
   CFG_SIP_REMOTE_USER_ADMISSION_NONE = 0,
   CFG_SIP_REMOTE_USER_ADMISSION_PRECONFIG = 1,
   CFG_SIP_REMOTE_USER_ADMISSION_LAN_USER = 2,
   CFG_SIP_REMOTE_USER_ADMISSION_ALL = 3
} CFGSIPREMOTEUSERADMISSION;

typedef struct CFGSIPREMOTEUSERSYSCFGtag
{
   BOS_BOOL  bEnable;
   CFGSIPREMOTEUSERADMISSION sAdmissionMode;
   BOS_UINT32 uDftAssociatedIMPUIndexRef;  /* Default SIP remote user associated IMPU Index */
   BOS_UINT8 *pcDftAssociatedUserId;  /* Default SIP remote user associated IMPU */
   
   BOS_UINT32 uDftIMPIIndexRef;     /* Default SIP remote user associated IMPI Index */                                  
   BOS_UINT8 *pcDftIMPIUserUserId;      /* Default User Id from IMPI table */
   CFGSIPCREDENTIAL dftCredentials;     /* Default User Credentials from IMPI table 
                                        ** based on eueMgmtIMGUsrIMPIIndexRef */   
} CFGSIPREMOTEUSERSYSCFG;

typedef struct CFGSIPLINESETTINGSCFGtag
{
  /* dect line settings list - read from prov by dect*/
  BOS_UINT32 uLSLLineId;
  BOS_UINT8  *pcLSLLineName;
  BOS_UINT8  *pcLSLAttachedHandsets;
  BOS_UINT8  *pcLSLDialingPrefix;
  BOS_UINT32 uLSLFPMelody;
  BOS_UINT32 uLSLFPVolume;
  BOS_SINT32 sLSLBlockedNB;
  BOS_SINT32 sLSLMultiCalls;
  BOS_SINT32 sLSLIntrusionCall;
  struct CFGSIPLINESETTINGSCFGtag *pNext;
} CFGSIPLINESETTINGSCFG;


/***************************************************************
**      Endpoint  Information
**
****************************************************************/

/*Enum for Codec Types*/
typedef enum
{
   eCFG_CODECTYPE_OTHER = 1,
   eCFG_CODECTYPE_PCMU,
   eCFG_CODECTYPE_PCMA,
   eCFG_CODECTYPE_G711_LINEAR,
   eCFG_CODECTYPE_LINPCM,
   eCFG_CODECTYPE_G728,
   eCFG_CODECTYPE_G729,
   eCFG_CODECTYPE_G729E,
   eCFG_CODECTYPE_G726AT16,
   eCFG_CODECTYPE_G726AT24,
   eCFG_CODECTYPE_G726AT32,
   eCFG_CODECTYPE_G726AT40,
   eCFG_CODECTYPE_G722,       /* G722 - no rate specified.  Assume 64kbps */
   eCFG_CODECTYPE_G722AT48,
   eCFG_CODECTYPE_G722AT56,
   eCFG_CODECTYPE_G722AT64,
   eCFG_CODECTYPE_G7221,
   eCFG_CODECTYPE_G723,
   eCFG_CODECTYPE_ILBC,
   eCFG_CODECTYPE_BV16,
   eCFG_CODECTYPE_BV32,
   eCFG_CODECTYPE_GSMAMR,
   eCFG_CODECTYPE_GSMEFR,
   eCFG_CODECTYPE_AMRWB,
   eCFG_CODECTYPE_LINPCMWB,
#if CCTKCFG_RED
   eCFG_CODECTYPE_RED,
#endif
   eCFG_CODECTYPE_UNKNOWN

} CFGCODECTYPE;

/*Enum for Video Codec Types - Extends the CFGCODECTYPE*/
typedef enum
{
   eCFG_CODECVIDTYPE_H263 = eCFG_CODECTYPE_UNKNOWN + 1,
   eCFG_CODECVIDTYPE_H264,
   eCFG_CODECVIDTYPE_MPEG4,
   eCFG_CODECVIDTYPE_UNKNOWN

} CFGCODECVIDTYPE;


typedef struct CFGEPTCODECLISTtag
{
   CFGCODECTYPE codecType;
   struct CFGEPTCODECLISTtag *pNext;
}CFGEPTCODECLIST;

typedef struct CFGEPTCODECVIDLISTtag
{
   CFGCODECVIDTYPE codecType;
   struct CFGEPTCODECVIDLISTtag *pNext;
}CFGEPTCODECVIDLIST;

/* AMR Ingress codec parameters, used by CFGEPTCODECCFG */
typedef struct CFGAMRINtag
{
   BOS_BOOL       bOctetAlign;
   BOS_UINT16     uModeSet;
   BOS_BOOL       bModeChangeNeighbor;
   BOS_BOOL       bModeChangePeriod;
   BOS_UINT16     uMaxRed;
   BOS_UINT16     uPtime;
} CFGAMRIN;

typedef struct CFGEPTCODECCFGtag
{
  BOS_SINT32 sG711PktPeriod;
  BOS_BOOL  bT38Enable;
  BOS_BOOL  bV152Enable;
  BOS_UINT8 *pcPubReportAddr;
  BOS_BOOL  bRTCPXRepEnable;
  BOS_SINT32 sRTCPRate;
  BOS_BOOL  bDtmfRelayEnabled;
  BOS_BOOL  bRTCPUseNextPort;
  BOS_UINT8 uTelEvtPayloadNum;
  BOS_BOOL  bSilenceDisable;
  BOS_BOOL  bStatInCallEnd;
  CFGAMRIN  amrNbCfg;
  CFGAMRIN  amrWbCfg;
#if CCTKCFG_RED
  BOS_BOOL  bREDEnable;
#endif
}CFGEPTCODECCFG;

typedef struct CFGEPTCODECVIDCFGtag
{
   BOS_UINT32 uH264PktMode;
   BOS_UINT32 uH264Profile;
   BOS_UINT32 uH264Level;
   BOS_UINT32 uH264MaxMbps;
   BOS_UINT32 uH264MaxBr;

   BOS_UINT32 uH263Profile;
   BOS_UINT32 uH263Level;

} CFGEPTCODECVIDCFG;

typedef enum
{
  CFG_EPT_HOOKSTATE_ONHOOK = 1,
  CFG_EPT_HOOKSTATE_ONHOOKWITHACTIVITY,
  CFG_EPT_HOOKSTATE_OFFHOOK
} CFGEPTHOOKSTATE;

typedef enum
{
  CFG_EPT_MWI_ONHOOK_FSK = 1,
  CFG_EPT_MWI_ONHOOK_DTMF = 2
} CFGEPTMWIONHOOKIND;

typedef enum
{
  CFG_EPT_MWI_OFFHOOK_TONE = 1,
  CFG_EPT_MWI_OFFHOOK_ANC = 2
} CFGEPTMWIOFFHOOKIND;


typedef enum
{
  CFG_EPT_ANALOG_ALARM_CFG_NONE = 1,
  CFG_EPT_ANALOG_ALARM_CFG_PREEMPTIVE = 2,
  CFG_EPT_ANALOG_ALARM_CFG_SIMULTANEOUS = 3
} CFGEPTANALOGALARMCFGSTATE;

typedef enum
{
  CFG_EPT_MDITYPE_POTS = 1,
  CFG_EPT_MDITYPE_DECTPP
} CFGEPTMDITYPE;

typedef struct CFGEPTLINECFGtag
{
  CFGSIPTERMTYPE eTermType;
  BOS_SINT32 sNetDiscTime;
  BOS_BOOL   bAnswerSupervision;
  BOS_SINT32 sPrLossDAdB;
  BOS_SINT32 sPrLossADdB;
  BOS_UINT32 uDtmfMinPlayout;
  CFGEPTHOOKSTATE sHookState;
  BOS_BOOL   bFaxDetection;
  BOS_SINT32 sQosPreconditions;
  BOS_SINT32 sDCBiasReapplyTime;
  CFGEPTMWIONHOOKIND sMwiOnHook;
  CFGEPTMWIOFFHOOKIND sMwiOffHook;
  BOS_BOOL bFaxOnly;
  BOS_UINT32 uFaxWaitTime;
  /*International parameters that are per line*/
  BOS_UINT32 uCallWaitingDelay;
  BOS_UINT32 uMinHookFlash;
  BOS_UINT32 uMaxHookFlash;
  BOS_UINT32 uPulseDialInterdigitTime;
  BOS_UINT32 uPulseDialMinMakeTime;
  BOS_UINT32 uPulseDialMaxMakeTime;
  BOS_UINT32 uPlseDialMinBreakTime;
  BOS_UINT32 uPulseDialMaxBreakTime;
  /* Analog Alarm Cfg is per ifIndex */
  CFGEPTANALOGALARMCFGSTATE sAnalogAlarmCfgState;
  CFGEPTMDITYPE sMdiType;
}CFGEPTLINECFG;

typedef struct CFGEPTANNOUNCECFGtag
{
  BOS_UINT8  *pcToneIdentifier;
  BOS_UINT8  *pcAudioAnnounceID;
}CFGEPTANNOUNCECFG;

typedef struct CFGEPTSTATUSCFGtag
{
  BOS_BOOL   bEndpointEnabled;
  BOS_BOOL   bRegistered;  /*Endpoint has associated registered users*/
}CFGEPTSTATUSCFG;

/***************************************************************
**      Provisioning  Information
****************************************************************/
typedef enum
{
  CFG_PROV_RESET_TKT_NONE = 0,
  CFG_PROV_RESET_TKT_PROV,
  CFG_PROV_RESET_TKT_CMS,
  CFG_PROV_RESET_TKT_BOTH,
} CFGPROVRESETTICKETS;

typedef enum
{
  CFG_PROV_IP_MODE_UNKNOWN = 0,
  CFG_PROV_IP_MODE_SS_IPv4 = 1,
  CFG_PROV_IP_MODE_SS_IPv6 = 2,
  CFG_PROV_IP_MODE_DS_IPv4 = 5,
  CFG_PROV_IP_MODE_DS_IPv6 = 6
} CFGPROVIPMODE;

typedef enum
{
  CFG_PROV_LOC_INFO_PREF_UNKNOWN = 0,
  CFG_PROV_LOC_INFO_PREF_CIVIC,
  CFG_PROV_LOC_INFO_PREF_GEO
} CFGPROVLOCINFOTYPE;

/*API for Emergency Services Feature*/
typedef struct CFGSIPCIVICADDRESStag
{
   BOS_UINT8   *country;  /* 2 chars, CA, US, etc. */
   BOS_UINT8   *A1;      /* national subdivision (state, province, region) */
   BOS_UINT8   *A3;      /* city, township */
   BOS_UINT8   *A6;      /* group of streets below neighborhood level */
   BOS_UINT8   *HNO;     /* house number */
   BOS_UINT8   *LOC;     /* Additional location information */
   BOS_UINT8   *PC;      /* postal code */
   BOS_UINT8   *civicTimeStamp;
} CFGSIPCIVICADDRESS;

typedef struct CFGSIPGEOINFOtag
{
   BOS_UINT8   *srsName;
   BOS_UINT8   *pos;
   BOS_UINT8   *FLR;      /* floor */
   BOS_UINT8   *geoTimeStamp;
} CFGSIPGEOINFO;

/*API for Location Info*/
typedef struct CFGSIPLOCATIONRAWINFOtag
{
   BOS_UINT8   *pcRawBuf;  /*hex buffer*/
   BOS_UINT32  uRawBufLen; /*buffer len*/
} CFGSIPLOCATIONRAWINFO;

typedef struct CFGSIPLOCATIONINFOtag
{
   CFGPROVLOCINFOTYPE  locationInfoPref;
   CFGSIPCIVICADDRESS  civicInfo;
   CFGSIPGEOINFO       geoInfo;
} CFGSIPLOCATIONINFO;

typedef enum
{
  CFG_KEEPALIVE_SETTING_ON = 1,
  CFG_KEEPALIVE_SETTING_OFF,
  CFG_KEEPALIVE_SETTING_CONDITIONAL 
}CFGKEEPALIVESETTINGTYPE;

typedef enum
{
   CFG_PREF_CANDIDATE_IPV4=1, /*prefer IPV4 for media*/
   CFG_PREF_CANDIDATE_IPV6 /*prefer IPV6 for media*/
}CFGPREFERREDCANDIDATEPAIR;


typedef struct CFGPROVDEVICECFGtag
{
   BOS_IP_ADDRESS IpAddress; /*Device IP address - Primary */
   BOS_IP_ADDRESS SecIpAddress; /*Device IP address - Secondary */
   BOS_UINT8  uInterface; /* Device interface. */
   
   BOS_UINT8  *pcFQDN;  /*FQDN of device*/
   BOS_UINT8  *pMAC;   /*MAC address - byte string*/
   BOS_BOOL   bDevEnabled;  /*device level Enabled*/
   BOS_UINT8  *pcTypeIdentifier; /*type identifier - used in DHCP option 60*/
   BOS_UINT8  *pcSerialNum;
   BOS_UINT8  *pcSwVer;
   BOS_UINT32 uProvTimer;
   BOS_UINT32 uProvCounter;
   BOS_UINT32 uCorrelationId;
   CFGPROVRESETTICKETS resetTickets;
   CFGPROVIPMODE ipMode;
   BOS_UINT32  uSIPRecvPort;  /*SIP Port for receiving*/
   CFGSIPLOCATIONINFO  locInfo;
   CFGPROVLOCINFOTYPE locType;
   CFGSIPLOCATIONRAWINFO civicAddressRaw; /*raw civic address info*/
   CFGSIPLOCATIONRAWINFO geoConfigRaw; /*geo config address info*/
   BOS_SINT32 sNtpTimeAdj;     /* the time that needs to be adjusted from the current
                               ** time of day time to get the NTP time */

   BOS_SINT32 sCallSigDscp;
   BOS_SINT32 sMediaStreamDscp;

   BOS_UINT16 uRtpMinPort; /* RTP min port */
   BOS_UINT16 uRtpMaxPort; /* RTP max port */
   BOS_UINT16 uRtcpMinPort; /* RTCP min port */
   BOS_UINT16 uRtcpMaxPort; /* RTCP max port */
   BOS_UINT16 uT38MinPort; /* T38 min port */
   BOS_UINT16 uT38MaxPort; /* T38 max port */

   BOS_BOOL bDisableKeepAlive;
   CFGKEEPALIVESETTINGTYPE sKeepAliveSetting;
   BOS_UINT32 uKeepAliveMaxInterval;
   BOS_UINT32 uKeepAliveFailureThreshold;
   
   BOS_UINT8 *pcCountryCode;  /*E.164 country code.*/
   BOS_UINT8 *pcDevSesIdent;
   BOS_UINT32  uCnxInactivityTimer;
   BOS_UINT8 *pcDevDomain;  /*Domain from Option 15*/
   BOS_BOOL bNoToneOutOfSrv; /*play no tones when user/line out of service*/
   BOS_BOOL bOperNokIffNoTgtsAvail;     /* Report eCMGREVT_OPER_NOK if and only if registration 
                                        ** fails with all the known targets */


   BOS_BOOL bIncludeNetworkInfo;
   BOS_UINT8 *pcNetworkInfo;
   
   BOS_BOOL bEarlyMedia;
   
   CFGPREFERREDCANDIDATEPAIR  sPreferredCandidatePair;

   BOS_BOOL bRandomReReg;      
   
   BOS_UINT32 uSessionExpiry;
   BOS_UINT32 uSessionMinimum;
   
   BOS_BOOL bDisableSubscribe;
   BOS_BOOL bBargeInEnabled;

}CFGPROVDEVICECFG;

typedef enum
{
   eCFG_DEV_CIDMODE_DURINGRINGINGETS = 1,
   eCFG_DEV_CIDMODE_DTASETS,
   eCFG_DEV_CIDMODE_RPASETS,
   eCFG_DEV_CIDMODE_LRASETS,
   eCFG_DEV_CIDMODE_LRETS
} CFGPROVDEVCIDMODE;

typedef enum
{
   eCFG_DEV_CIDSIGPROTOCOL_FSK = 1,
   eCFG_DEV_CIDSIGPROTOCOL_DTMF
} CFGPROVDEVCIDSIGPROTOCOL;

typedef enum
{
   eCFG_DEV_VMWIMODE_DTASETS = 1,
   eCFG_DEV_VMWIMODE_RPASETS,
   eCFG_DEV_VMWIMODE_LRASETS,
   eCFG_DEV_VMWIMODE_OSI,
   eCFG_DEV_VMWIMODE_LRETS
} CFGPROVDEVVMWIMODE;

typedef struct CFGPROVDEVINTERNATIONALCFGtag
{
   /*Per device Parameters for internationalization*/
   BOS_UINT32 uPowerRingFrequency; /*in Hertz*/
   CFGPROVDEVCIDMODE sCidMode;
   BOS_SINT32 sCidSigProtocol;
   BOS_UINT32 uCidAfterRing;
   BOS_UINT32 uCidAfterDTAS;
   BOS_UINT32 uCidAfterRPAS;
   BOS_UINT32 uRingAfterCID;
   BOS_UINT32 uCidDTASAfterLR;
   CFGPROVDEVVMWIMODE sVmwiMode;
   BOS_UINT32 uVmwiAfterDTAS;
   BOS_UINT32 uVmwiAfterRPAS;
   BOS_UINT32 uVmwiDTASAfterLR;
}CFGPROVDEVINTERNATIONALCFG;
 
typedef enum
{
  CFG_PROV_RESULT_PASS = 1,
  CFG_PROV_RESULT_INPROGRESS,
  CFG_PROV_RESULT_FAILCONFIGFILEERROR,
  CFG_PROV_RESULT_PASSWITHWARNINGS,
  CFG_PROV_RESULT_PASSWITHINCOMPLETEPARSING,
  CFG_PROV_RESULT_FAILUREINTERNALERROR,
  CFG_PROV_RESULT_FAILOTHERREASON
} CFGPROVRESULTTYPE;

typedef enum
{
  CFG_PROV_STATE_OPERATIONAL = 1,
  CFG_PROV_STATE_WAITINGFORSETINFO,
  CFG_PROV_STATE_WAITINGFORTFTPADDR,
  CFG_PROV_STATE_WAITINGFORCFGFILE
} CFGPROVSTATETYPE;

typedef struct CFGPROVSTATUStag
{
  CFGPROVSTATETYPE sState; /*ProvState - progress of provisioning process*/
  CFGPROVRESULTTYPE sResult; /*Provisioning Status - result of the process*/
}CFGPROVSTATUS;

typedef struct CFGPROVSERVERStag
{
  BOS_IP_ADDRESS  priDhcp;
  BOS_IP_ADDRESS  secDhcp;
#if GLOBAL_CFG_IPv6_SUPPORT
  BOS_UINT8  priDhcpDssId[DSS_ID_MAX_LEN];
  int        priDhcpDssIdLen;
  BOS_UINT8  secDhcpDssId[DSS_ID_MAX_LEN];
  int        secDhcpDssIdLen;
#endif
  BOS_IP_ADDRESS  priDns;
  BOS_IP_ADDRESS  secDns;
  BOS_IP_ADDRESS  timeServer;
  BOS_IP_ADDRESS  sysLogServer;
  BOS_UINT8 *pcProvServerFQDN;
  BOS_UINT8  *pcKerbRealmName;
  BOS_IP_ADDRESS  dhcpServerUsed; /*server who address was obtained from*/
}CFGPROVSERVERS;

typedef struct CFGPROVCFGFILEtag
{
  BOS_UINT8  *pcName;
  BOS_UINT8  *pcHash;
  BOS_UINT32 uHashLen;
  BOS_UINT8  *pcKey;
  BOS_UINT32 uKeyLen;
}CFGPROVCFGFILE;

typedef struct CFGPROVSECtag
{
  BOS_UINT32 uAPSolicitedKeyTO;  /*in seconds*/
  BOS_UINT32 uAPUnSolicitedKeyMaxTO; /*seconds*/
  BOS_UINT32 uAPUnSolicitedKeyNomTO; /*seconds*/
  BOS_UINT32 uAPUnSolicitedMaxRetries;
  BOS_UINT32 uASUnSolicitedKeyMaxTO;
  BOS_UINT32 uASUnSolicitedKeyNomTO;
  BOS_UINT32 uASUnSolicitedMaxRetries;
  BOS_UINT32 uPkInitGracePeriod;
  BOS_UINT32 uPTgsGracePeriod;
  BOS_UINT8  *pcRealmOrgName;
}CFGPROVSEC;

typedef struct CFGSECCERTStag
{
  BOS_UINT8  *pManufacturerCert;
  BOS_UINT8  *pDevCert;
  BOS_UINT8  *pDevTelRootCert;
}CFGSECCERTS;

typedef struct CFGPROVSYSINFOtag
{
  BOS_UINT8  *pcSysDescr;
}CFGPROVSYSINFO;

typedef struct CFGPROVRINGCADENCEtag
{
  BOS_UINT8 *pcR0Cadence;
  BOS_UINT8 *pcR1Cadence;
  BOS_UINT8 *pcR2Cadence;
  BOS_UINT8 *pcR3Cadence;
  BOS_UINT8 *pcR4Cadence;
  BOS_UINT8 *pcR5Cadence;
  BOS_UINT8 *pcR6Cadence;
  BOS_UINT8 *pcR7Cadence;
  BOS_UINT8 *pcRgCadence;
  BOS_UINT8 *pcRsCadence;
  BOS_UINT8 *pcCountryCadence; /*Index of 0-127 required to query*/
} CFGPROVRINGCADENCE;

/*Country specific tones*/
/*Enum for tone Types*/
typedef enum
{
   eCFG_TONETYPE_BUSY = 1,
   eCFG_TONETYPE_CONFIRMATION,
   eCFG_CONSTVAL_PKTCSIGDEVTONETYPE_DIAL,
   eCFG_CONSTVAL_PKTCSIGDEVTONETYPE_MESSAGEWAITING,
   eCFG_TONETYPE_OFFHOOKWARNING,
   eCFG_TONETYPE_RINGBACK,
   eCFG_TONETYPE_REORDER,
   eCFG_TONETYPE_STUTTERDIAL,
   eCFG_TONETYPE_CALLWAITING1,
   eCFG_TONETYPE_CALLWAITING2,
   eCFG_TONETYPE_CALLWAITING3,
   eCFG_TONETYPE_CALLWAITING4,
   eCFG_TONETYPE_ALERTINGSIGNAL,
   eCFG_TONETYPE_SPECIALDIAL,
   eCFG_TONETYPE_SPECIALINFO,
   eCFG_TONETYPE_RELEASE,
   eCFG_TONETYPE_CONGESTION,
   eCFG_TONETYPE_USERDEFINED1,
   eCFG_TONETYPE_USERDEFINED2,
   eCFG_TONETYPE_USERDEFINED3,
   eCFG_TONETYPE_USERDEFINED4
} CFGTONETYPE;

typedef struct CFGPROVSIGMULTIFREQTONEGROUPtag
{
     /* First four frequency components */
   BOS_UINT32   uFirstFrequency;
   BOS_UINT32   uSecondFrequency;
   BOS_UINT32   uThirdFrequency;
   BOS_UINT32   uFourthFrequency;
   /* Frequency mixing mode */
   BOS_UINT32   uFreqMode;
   BOS_UINT32   uFreqModePrtg;
   /* We only have 1 tone-on and tone-off period for now */
   BOS_UINT32   uFirstToneOn;
   BOS_UINT32   uFirstToneOff;
   /* Db level of the signal */
   BOS_SINT32   sDbLevel;
   /* Number of times to repeat the current frequency group */
   BOS_UINT32   uFreqGrpRepeatCount;
 
} CFGPROVSIGMULTIFREQTONEGROUP;

#define CFG_PROV_SIG_MAX_MULTI_FREQ_TONE_NUMBER 8
typedef struct CFGPROVSIGTONEtag
{
   /* Number of times to repeat the entire tone */
   BOS_UINT32        uWholeToneRepeatCount;
   /* Indicate steady characteristics at the end */
   BOS_BOOL          bSteady;
   /* Number of frequency group in the signal */
   BOS_UINT32        uNumFreqGroup;
   /* Frequency group data */
   CFGPROVSIGMULTIFREQTONEGROUP   multiFreqGroup[CFG_PROV_SIG_MAX_MULTI_FREQ_TONE_NUMBER];

} CFGPROVSIGTONE;

/*Enum for pulse signal types*/
typedef enum
{
   eCFG_PULSESIGNALTYPE_INITIALRING = 1,
   eCFG_PULSESIGNALTYPE_PULSELOOPCLOSE,
   eCFG_PULSESIGNALTYPE_PULSELOOPOPEN,
   eCFG_PULSESIGNALTYPE_ENABLEMETERPULSE,
   eCFG_PULSESIGNALTYPE_METERPULSEBURST,
   eCFG_PULSESIGNALTYPE_PULSENOBATTERY,
   eCFG_PULSESIGNALTYPE_PULSENORMALPOLARITY,
   eCFG_PULSESIGNALTYPE_PULSEREDUCEDBATTERY,
   eCFG_PULSESIGNALTYPE_PULSEREVERSEPOLARITY
} CFGPULSESIGNALTYPE;

typedef struct CFGPROVPULSESIGNALtag
{
   BOS_SINT32 sType;
   BOS_SINT32 sFrequency;
   BOS_SINT32 sDbLevel;
   BOS_UINT32 uDuration;
   BOS_UINT32 uInterval;
   BOS_UINT32 uRepeatCount;
} CFGPROVPULSESIGNAL;


/***************************************************************
**      Security  Information
****************************************************************/
typedef struct CFGSECDHPRIMEtag
{
   BOS_UINT8      *puPrime;            /* Pre-generated Diffie-Hellman prime with length
                                       ** "uLen".  Set "puPrime" to NULL to generate the
                                       ** prime at real time. Prime generation can
                                       ** be a CPU intensive task.
                                       */
   BOS_UINT32     uLen;                /* Prime length (in bits) for generating
                                       ** Diffie-Hellman keys. Set to 0 for default
                                       ** prime length, i.e. 256.
                                       */
}CFGSECDHPRIME;

typedef struct CFGSECTLSCFGtag
{
   BOS_UINT8      **ppcCertChain;      /* Local certificate chain in PEM format
                                       ** (NULL terminated in order CA, int1, int2,
                                       ** ..., local).
                                       */
   BOS_UINT8      **ppcTrustedCerts;   /* Trusted certificates in PEM format
                                       ** (NULL terminated).
                                       */
   BOS_UINT8      *pcPrivKey;          /* Private key associated with the local
                                       ** certificate in PEM format.
                                       */
   BOS_UINT8      *pcCipher;           /* Rule set that indicates which ciphers are
                                       ** allowed or not. E.g. "aRSA+SHA1" for
                                       ** cipher with RSA algorithm with SHA1
                                       ** digest. If set to NULL, all supported
                                       ** cipher suites would be used.
                                       */
   BOS_BOOL       bClientAuth;         /* True to enable TLS client peer
                                       ** authentication.
                                       */
   BOS_BOOL       bServerAuth;         /* True to enable TLS server peer
                                       ** authentication.
                                       */
   CFGSECDHPRIME  preGenDhPrime;       /* Pre-generated Diffie-Hellman prime.
                                       ** Set "puPrime" to NULL to generate the
                                       ** prime at real time. Prime generation can
                                       ** be a CPU intensive task.
                                       */
}CFGSECTLSCFG;

/*Enum for Dect PP Status*/
typedef enum
{
   /* NOTE: no status API on FP, so can only set to idleLocked 
   **       or activeLocked when registered
   */
   CFG_DECT_PP_STATE_UNKNOWN = 1,
   CFG_DECT_PP_STATE_IDLE_UNLOCKED,  /*not supported */
   CFG_DECT_PP_STATE_ACTIVE_UNLOCKED,/*not supported */
   CFG_DECT_PP_STATE_IDLE_LOCKED,    
   CFG_DECT_PP_STATE_ACTIVE_LOCKED,
   CFG_DECT_PP_STATE_NO_FP_IN_RANGE, /*not supported */
   CFG_DECT_PP_STATE_WAITING_EASY_PAIRING,/*not supported */
   CFG_DECT_PP_STATE_EASY_PAIRING_FAILED,/*not supported */
   CFG_DECT_PP_STATE_EASY_PIN_FAILED/*not supported */
} CFGDECTPPSTATE;

typedef struct CFGPROVDECTPPCFGtag
{
   /*Dect PP parameters, (all set to PROV by Dect)*/
   BOS_UINT8   *pcPPIPEI;
   BOS_UINT32  uTermId;
   CFGDECTPPSTATE sPPState;

}CFGPROVDECTPPCFG;


/* Callback Function to inform interested party about related configuration changes*/
typedef void  (*CFGACCCBF)   ( PROV_CFG_ITEM cfgItemId,
                               PROV_CFG_IDX  *cfgIndex,
                               void* pcfgVal
                              );


/*
*****************************************************************************
**
** FUNCTION:   cfgAccessCBRegister
**
** PURPOSE:    Register the callback function to inform
**             of SIP CFG changes
**
** PARAMETERS:
**
**
** RETURNS:    0 if successful.
**             otherwise there was an error in registering the callback.
**
*****************************************************************************
*/
int cfgAccessCBRegister(CFGACCCBF CBfunc);

/*
*****************************************************************************
**
** FUNCTION:   cfgAccessCBClear
**
** PURPOSE:    Clears the callback function to inform
**             of SIP configuration changes.
**             Do this if you no longer want to be informed.
**
** PARAMETERS:
**
**
** RETURNS:    0 if successful.
**
*****************************************************************************
*/
int cfgAccessCBClear(CFGACCCBF CBfunc);

/*
**=============================================================================
** Function:   CfgAccessCB()
** Purpose:    Function to report a configuration item change to clients
** Parameters:  cfgItem - Item that has been changed
**              pValue - value
**              cfgIndex - index of item (if applicable)
**
** Return:     BOS_STATUS_OK if OK, BOS_STATUS_ERROR - otherwise
**=============================================================================
 */
BOS_STATUS CfgAccessCB(PROV_CFG_ITEM cfgItem, void* pValue, PROV_CFG_IDX   *cfgIndex);


/*
**=============================================================================
** Function:   cfgAccessInit()
** Purpose:    Init Cfg module
** Parameters:
**
** Return:     BOS_STATUS_OK if OK, BOS_STATUS_ERROR - otherwise
**=============================================================================
 */
BOS_STATUS cfgAccessInit( void );

/*
**=============================================================================
** Function:   cfgAccessDeInit()
** Purpose:    Init Cfg module
** Parameters:
**
** Return:     BOS_STATUS_OK if OK, BOS_STATUS_ERROR - otherwise
**=============================================================================
 */
BOS_STATUS cfgAccessDeInit(void);

/*Function to test cfgAccessInterface*/
void cfgAccessTest( void );

/*
*****************************************************************************
** FUNCTION:   cfgAccessRegisterMibCBFuncs
**
** PURPOSE:    Called to register the MIB callback functions for the
**             configuration module
**
** PARAMETERS: NONE
**
** RETURNS:    BOS_STATUS_OK on success
**
** NOTE:
*****************************************************************************
*/
BOS_STATUS cfgAccessRegisterMibCBFuncs( void );

/*
*****************************************************************************
** FUNCTION:   cfgAccessUnRegisterMibCBFuncs
**
** PURPOSE:    Called to unregister the MIB callback functions for the
**             configuration module
**
** PARAMETERS: NONE
**
** RETURNS:    BOS_STATUS_OK on success
**
** NOTE:
*****************************************************************************
*/
BOS_STATUS cfgAccessUnRegisterMibCBFuncs( void );

/*
*****************************************************************************
**
** FUNCTION:   cfgIdxDeleteList
**
** PURPOSE:    Delete the index list provided to the callback function
** PARAMETERS:
**
**
** RETURNS:    0 if successful.
**
*****************************************************************************
*/
void cfgIdxDeleteList(PROV_CFG_IDX *pList);

/*
*****************************************************************************
**
** FUNCTION:   getProfileIndexFromUserIndex
**
** PURPOSE:    Translate IMPU index to application profile index.
**
** PARAMETERS:  uUserIdx 
**
**
** RETURNS:  pktcEUEUsrAppMapAppIndexRef 
**
*****************************************************************************
*/
BOS_UINT32 getProfileIndexFromUserIndex ( BOS_UINT32 uUserIdx);

/*****************************************************************************
**
** FUNCTION:    retrieveRSTAppProfileToFeatEntry()
**
** PURPOSE:     retrieve info from pktcEUERSTAppProfileToFeatEntry 
**
** PARAMETERS:  index - pktcEUERSTAppProfileIndex, 
**              index2 - pktcEUERSTAppFeatIndex
**              buf    - buffer to store the info
**
** RETURNS:     0 - success, 1 - otherwise
**
******************************************************************************/
int retrieveRSTAppProfileToFeatEntry(BOS_UINT32 index, 
                                     BOS_UINT32 index2, 
                                     char * pBuf);

/*****************************************************************************
**
** FUNCTION:    retrieveRSTDigitMapProfileEntry()
**
** PURPOSE:     retrieve info from pktcEUERSTDigitMapProfileEntry 
**
** PARAMETERS:  index - pktcEUERSTDMIndex, 
**              buf    - buffer to store the info
**
** RETURNS:     0 - success, 1 - otherwise
**
******************************************************************************/
int retrieveRSTDigitMapProfileEntry(BOS_UINT32 index, char * pBuf);

/*
*****************************************************************************
**
** FUNCTION:   populateEndPntMapList
**
** PURPOSE:   populate the CFGSIPUSERENDPNTMAP structure which indicates
**            how a user is mapped to physical endpoints.  
**
** PARAMETERS:  pcAddInfo - MIB containing the mapping information 
**
**
** RETURNS:  populated structure
**
*****************************************************************************
*/

CFGSIPUSERENDPNTMAP* populateEndPntMapList( BOS_UINT8 *pcAddInfo);

/*
*****************************************************************************
**
** FUNCTION:   deleteEndPntMapList
**
** PURPOSE:     recursive deletion of CFGSIPUSERENDPNTMAP
**
** PARAMETERS:  pList - List
**
**
** RETURNS:   nothing
**
*****************************************************************************
*/
void deleteEndPntMapList(CFGSIPUSERENDPNTMAP *pList);

/*
*****************************************************************************
** FUNCTION:   retrieveUserIdFromOpIdx
**
** PURPOSE:  returns the UserId (IMPU Id) based on the Domain index  
**
** PARAMETERS: 
**
** RETURNS:    BOS_STATUS_OK on success
**
** NOTE:
*****************************************************************************
*/
PROV_CFG_IDX *retrieveUserIdFromOpIdx( BOS_UINT32  uOpIdx);

#endif /* __CFG_ACCESS__H__INCLUDED__ */
