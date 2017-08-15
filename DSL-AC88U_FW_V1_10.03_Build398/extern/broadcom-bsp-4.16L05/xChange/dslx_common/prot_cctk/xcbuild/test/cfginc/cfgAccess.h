/****************************************************************************
*
*  Copyright  2008-2009 Broadcom
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



/* Define the Name of the ENUM Item */
#define  CFGITEM(comp,name)   comp##_##name

/* Define the Name of the CFGITEM for component "comp" */
#define  CFGITEM_TYPE(comp)   comp##_CFG_ITEM

#define CIVIC_ADDRESS_STR_MAX_LEN 32

typedef enum
{
   CFG_ITEM_GET = 0,
   CFG_ITEM_SET,
   CFG_ITEM_GET_CB,    /* used when item is accessed from a callback */
   CFG_ITEM_DEL
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
CFGITEM(SIP,  BasicCallConcept),         /*RST Basic Call Feature*/
CFGITEM(SIP,  AnnouncementConcept),      /*RST Announcement Feature*/
CFGITEM(SIP,  StatusChangeConcept),      /*RST Activation Status Feature*/
CFGITEM(SIP,  NoAnsConcept),             /*RST No Answer Feature*/
CFGITEM(SIP,  CIDConcept),               /*RST Caller ID Feature*/
CFGITEM(SIP,  CIDBlkConcept),            /*RST Caller ID Blocking Feature*/
CFGITEM(SIP,  CIDDisplayConcept),        /*RST Caller ID Display Feature*/
CFGITEM(SIP,  CallFwdConcept),            /*RST Call Forward Feature*/
CFGITEM(SIP,  CallWaitConcept),           /*RST Call Waiting Feature*/
CFGITEM(SIP,  CallHoldConcept),          /*RST Call Hold Feature*/
CFGITEM(SIP,  CallXfrConcept),           /*RST Call Transfer Feature*/
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

/*
  SIP User Information - Accessible using the UserID as the index.
  UserID of NULL will return info on all Users for device.
*/
CFGITEM(SIP,  UserConcept),              /*SIP User configuration group*/


/*
  Concept IDs for Endpoint - Accessible with endpoint number as the index
*/
CFGITEM(EPT,  MWIConcept),    /* This (EPT, MWIConcept) is obsolete.  The related cfgAccess 
                              ** functions have already been removed.  This enum should
                              ** be removed too */
CFGITEM(EPT,  CodecConcept),  /*Endpoint Codec configuration group*/
CFGITEM(EPT,  LineConcept),  /*Endpoint Line configuration group*/
CFGITEM(EPT,  AnnounceConcept), /*Endpoint Announcement configuration group*/
CFGITEM(EPT,  StatusConcept),   /*Endpoint Enabled Status*/
/*Run time list of preferred codecs.  Acessbile with UserID and endpoint number as the index*/
CFGITEM(EPT,  CodecList),              /*SIP User configuration group*/

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
  CFGSIPOPERSTATUS  sOperStatus;  /* operational status*/
  BOS_UINT8  *pcOperStatInfo; /*Info string for operational status*/

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
**     - Three Way Calling
**     - Do Not Disturb
**     - Subscriber PIN
**     - Msg Wait Indicator
**     - Auto Recall
**     - Auto Callback
**     - Busy Line Verify
**     - Emergency Services
**     - SCF
**
*********************************************************************************/


/*API for Digit Map Feature*/

typedef struct CFGSIPDIGITMAPtag
{
  CFGSIPACTSTATUS Act; /*Activation Status*/
  BOS_UINT8  *pABNF; /*Digit Map ABNF (value)*/
} CFGSIPDIGITMAP;


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

} CFGSIPBASICCALL;


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
} CFGSIPCID;

/*API for Call ID Display Feature*/
typedef struct CFGSIPCIDDISPLAYtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_BOOL  bCNDActStat;  /*Activation Status for CND (Call Number Display)*/
  BOS_BOOL  bCNAMDActStat;  /*Activation Status for CNAMD (Call Name Display)*/
  BOS_UINT8 *pcDefCountry;  /*Default Country*/
  BOS_SINT32 sTimeAdj; /* Time adjustment from local invariant time to time at current location (in minutes)*/
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
} CFGSIPCALLFWD;

/*API for Call Waiting Feature*/

typedef struct CFGSIPCALLWAITtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
  BOS_BOOL bNoCallWaitTO; /* Do not time out on call waiting (unanswered) */
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
} CFGSIPCALLXFER;

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
} CFGSIPMWI;

/*API for Auto Recall/Callback Features*/

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

typedef struct CFGSIPTHREEWAYCALLtag
{
  CFGSIPACTSTATUS Act;  /*Activation Status*/
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
  BOS_IP_ADDRESS STUNRelayAddr;   /*STUN Relay Address*/
  BOS_UINT32  uSTUNRelayPort;     /*STUN Relay Port*/
  BOS_UINT8 *pcSTUNRelayDomain;  /*Domain Name (FQDN) of STUN relay if provided. NULL if not provided*/
  CFGSIPCREDENTIAL STUNRelayCred; /*STUN Relay Credentials*/
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

/*API for list of PSCF servers for SIP*/
typedef struct CFGSIPPCSCFtag
{
  BOS_IP_ADDRESS PCSCFAddr; /*PCSCF Address*/
  BOS_UINT8 *pcPCSCFDomain;
  BOS_UINT32  uSIPPort;  /*SIP Port for sending */
  CFGSIPPROTID sProtIdUsed;  /*SIP Protocol*/
  BOS_IP_ADDRESS PCSCFAddrUsed; /*PCSCF Address Used*/
  BOS_UINT32  uTimerT1;  /*SIP Timer T1*/
  BOS_UINT32  uTimerT2;  /*SIP Timer T2*/
  BOS_UINT32  uTimerT4;  /*SIP Timer T4*/
  BOS_UINT32  uTimerTD;  /*SIP Timer D */
  BOS_UINT32  uTimerTB;  /*SIP Timer B */
  BOS_UINT32  uTimerTH;  /*SIP Timer H */
  BOS_UINT32  uTimerTF;  /*SIP Timer F */

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

/* Complete User Configuration Structure
  - To obtain the full list of configured Users, perform
    a "get" action with a NULL userID.
*/
typedef struct CFGSIPUSERCFGtag
{
  CFGSIPUSERIDTYPE sUserType; /*User ID type*/
  BOS_UINT8  *pcUserId; /*public User ID (IMPU)*/
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
  BOS_UINT32 uReRegAllFail; /* Initial registration delay */
  BOS_UINT32 uReRegNotFailed; /* Initial registration delay */
  BOS_UINT32 uReRegMaxTime; /* Initial registration delay */
  
  struct CFGSIPUSERCFGtag *pNext;
} CFGSIPUSERCFG;



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
   eCFG_CODECTYPE_G729,
   eCFG_CODECTYPE_G729E,
   eCFG_CODECTYPE_G726AT16,
   eCFG_CODECTYPE_G726AT24,
   eCFG_CODECTYPE_G726AT32,
   eCFG_CODECTYPE_G726AT40,
   eCFG_CODECTYPE_G722,
   eCFG_CODECTYPE_G723,
   eCFG_CODECTYPE_ILBC,
   eCFG_CODECTYPE_BV16,
   eCFG_CODECTYPE_BV32,
#if CCTKCFG_RED
   eCFG_CODECTYPE_RED,
#endif
   eCFG_CODECTYPE_UNKNOWN

} CFGCODECTYPE;

typedef struct CFGEPTCODECLISTtag
{
   CFGCODECTYPE codecType;
   struct CFGEPTCODECLISTtag *pNext;
}CFGEPTCODECLIST;

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
}CFGEPTCODECCFG;

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


typedef struct CFGEPTLINECFGtag
{
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
}CFGEPTLINECFG;

typedef struct CFGEPTANNOUNCECFGtag
{
  BOS_UINT8  *pcToneIdentifier;
  BOS_UINT8  *pcAudioAnnounceID;
}CFGEPTANNOUNCECFG;

typedef struct CFGEPTSTATUSCFGtag
{
  BOS_BOOL   bEndpointEnabled;
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
  CFG_PROV_IP_MODE_IPV4,
  CFG_PROV_IP_MODE_IPV6
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

typedef struct CFGPROVDEVICECFGtag
{
   BOS_IP_ADDRESS IpAddress; /*Device IP address*/
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
   CFGSIPCIVICADDRESS civicAddress;
   
   CFGPROVLOCINFOTYPE locPref;
   CFGPROVLOCINFOTYPE locType;
   BOS_UINT8  *rawCivicAddress;

   BOS_SINT32 sCallSigDscp;
   BOS_SINT32 sMediaStreamDscp;

   BOS_BOOL bDisableKeepAlive;
   BOS_UINT8 *pcCountryCode;  /*E.164 country code.*/
   BOS_UINT8 *pcDevSesIdent;

}CFGPROVDEVICECFG;

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
} CFGPROVRINGCADENCE;


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


#endif /* __CFG_ACCESS__H__INCLUDED__ */
