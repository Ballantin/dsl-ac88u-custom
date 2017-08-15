/**********************************************************************************
** Copyright (c) 2008-2012 Broadcom Corporation
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
**      This file implements the GUI call client application.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include "gui.h"

#include <cmgrApi.h>
#include <bosInit.h>
#include <bosTask.h>
#include <bosSleep.h>
#include <xchgAssert.h>
#include <cfgAccess.h>

/* CCTK Backdoor access */
/* Note Client application should interface to CMGR only. The backdoor access to
 * CCTK is for debugging purposes only:
 *    - updating Trace Level and Group configurations
 */
#include <cctkApi.h>


/* Win32 configuration and CCTKCFG_USE_CABLE_SIMULATOR.
**
** When the CCTKCFG_USE_CABLE_SIMULATOR is defined as part of the application
** configuration in cctkExtCfgCustom.h module, the application is expected to
** be used in conjunction with the PCSim2 simulator platform which is used for
** PacketCable 2.0 certification (IMS certification).
**
** In such case, the following configuration is expected to be setup as part
** of the Win32 GUI.  This assumes that the pending configuration of the device
** UE on the PCSim2 platform matches those expectations.
**
**   Port = make sure to use something other than 5060 since the PCSim2 will
**          run on that port - assuming PCSim2 and Win32 application are running
**          on the same machine.
**   Phone = set it to be 900 such that the user created on the Win32 application
**           will be '9001'.
**   Password = set it to NULL (empty) string such that the Win32 application will
**              use 'broadcom' for password.
**   Registrar = set it to 'pclab.com'.
**   Outbound Proxy = set it to the IP address of the PCSim2.
**
** Note that currently only one user is created on the Win32 platform to interact
** with the PCSim2, this can be changed easily if there is a need to simulate more
** than one user per Win32 application.
**
** Note also that it is possible to run concurrent Win32 application on the same
** platform, each of which can support one user by default and up to two.  When
** running concurent Win32 application however, make sure that each uses a different
** 'Port' (uniquely assigned).  Also, the 'Phone' user must be unique, note that the
** phone user is created by adding '1' to the end of the value configured in the Win32
** GUI.
*/

/* By default, the application will boot up with two users configured - unless
** CCTKCFG_USE_CABLE_SIMULATOR is used - in other cases, in order to only have one
** user enabled, enable the following #define.
*/
#define CALL_CLIENT_CFG_ONE_USER 0

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

#define    _STR( arg )           { arg, #arg }
#define    CMMAP__INVALID        "Invalid"

typedef struct
{
   BOS_UINT32 uId;
   BOS_UINT32 uMap;

} CALLCLIENTMAP;

typedef struct
{
   BOS_UINT32 uId;
   char *pMap;

} CALLCLIENTTXTMAP;

typedef struct
{
   BOS_BOOL bInUse;
   BOS_BOOL bFocus;
   BOS_UINT32 uLine;

} CALLCLIENTCNX;


CALLCLIENTMAP appButtons[] =
{
      { IDC_BUTTON1,             eEPTEVT_DTMF1 },
      { IDC_BUTTON2,             eEPTEVT_DTMF2 },
      { IDC_BUTTON3,             eEPTEVT_DTMF3 },
      { IDC_BUTTON4,             eEPTEVT_DTMF4 },
      { IDC_BUTTON5,             eEPTEVT_DTMF5 },
      { IDC_BUTTON6,             eEPTEVT_DTMF6 },
      { IDC_BUTTON7,             eEPTEVT_DTMF7 },
      { IDC_BUTTON8,             eEPTEVT_DTMF8 },
      { IDC_BUTTON9,             eEPTEVT_DTMF9 },
      { IDC_BUTTON0,             eEPTEVT_DTMF0 },
      { IDC_BUTTON_ASTER,        eEPTEVT_DTMFS },
      { IDC_BUTTON_HASH,         eEPTEVT_DTMFH },
      { IDC_FLASH,               eEPTEVT_FLASH },
      { IDC_FAX1,                eEPTEVT_FAX },
      { IDC_FAXEND1,             eEPTEVT_T38_STOP },
      { IDC_FAXFAIL1,            eEPTEVT_T38_FAILURE },
      { IDC_BUTTON31,            eEPTEVT_DTMF1 },
      { IDC_BUTTON32,            eEPTEVT_DTMF2 },
      { IDC_BUTTON33,            eEPTEVT_DTMF3 },
      { IDC_BUTTON34,            eEPTEVT_DTMF4 },
      { IDC_BUTTON35,            eEPTEVT_DTMF5 },
      { IDC_BUTTON36,            eEPTEVT_DTMF6 },
      { IDC_BUTTON37,            eEPTEVT_DTMF7 },
      { IDC_BUTTON38,            eEPTEVT_DTMF8 },
      { IDC_BUTTON39,            eEPTEVT_DTMF9 },
      { IDC_BUTTON30,            eEPTEVT_DTMF0 },
      { IDC_BUTTON_ASTER3,       eEPTEVT_DTMFS },
      { IDC_BUTTON_HASH3,        eEPTEVT_DTMFH },
      { IDC_FLASH3,              eEPTEVT_FLASH },
      { IDC_FAX3,                eEPTEVT_FAX },
      { IDC_FAXEND3,             eEPTEVT_T38_STOP },
      { IDC_FAXFAIL3,            eEPTEVT_T38_FAILURE },
      { CMGR_INVALID_HANDLE,     CMGR_INVALID_HANDLE }
};

CALLCLIENTMAP appButtonLine[] =
{
      { IDC_BUTTON1,             0 },
      { IDC_BUTTON2,             0 },
      { IDC_BUTTON3,             0 },
      { IDC_BUTTON4,             0 },
      { IDC_BUTTON5,             0 },
      { IDC_BUTTON6,             0 },
      { IDC_BUTTON7,             0 },
      { IDC_BUTTON8,             0 },
      { IDC_BUTTON9,             0 },
      { IDC_BUTTON0,             0 },
      { IDC_BUTTON_ASTER,        0 },
      { IDC_BUTTON_HASH,         0 },
      { IDC_FLASH,               0 },
      { IDC_FAX1,                0 },
      { IDC_FAXEND1,             0 },
      { IDC_FAXFAIL1,            0 },
      { IDC_BUTTON31,            1 },
      { IDC_BUTTON32,            1 },
      { IDC_BUTTON33,            1 },
      { IDC_BUTTON34,            1 },
      { IDC_BUTTON35,            1 },
      { IDC_BUTTON36,            1 },
      { IDC_BUTTON37,            1 },
      { IDC_BUTTON38,            1 },
      { IDC_BUTTON39,            1 },
      { IDC_BUTTON30,            1 },
      { IDC_BUTTON_ASTER3,       1 },
      { IDC_BUTTON_HASH3,        1 },
      { IDC_FLASH3,              1 },
      { IDC_FAX3,                1 },
      { IDC_FAXEND3,             1 },
      { IDC_FAXFAIL3,            1 },
      { CMGR_INVALID_HANDLE,     CMGR_INVALID_HANDLE }
};

CALLCLIENTMAP appPicCtrlLine[] =
{
      { IDC_HANDSET_ONHOOK,      0 },
      { IDC_HANDSET_OFFHOOK,     0 },
      { IDC_HANDSET_ONHOOK3,     1 },
      { IDC_HANDSET_OFFHOOK3,    1 },
      { CMGR_INVALID_HANDLE,     CMGR_INVALID_HANDLE }
};

CALLCLIENTTXTMAP ccSigMap[] =
{
   _STR( eEPTSIG_BUSY ),
   _STR( eEPTSIG_CONF ),
   _STR( eEPTSIG_DIAL ),
   _STR( eEPTSIG_OHWARN ),
   _STR( eEPTSIG_RINGING ),
   _STR( eEPTSIG_RING0 ),
   _STR( eEPTSIG_RING1 ),
   _STR( eEPTSIG_RING2 ),
   _STR( eEPTSIG_RING3 ),
   _STR( eEPTSIG_RING4 ),
   _STR( eEPTSIG_RING5 ),
   _STR( eEPTSIG_RING6 ),
   _STR( eEPTSIG_RING7 ),
   _STR( eEPTSIG_CALLID_RINGING ),
   _STR( eEPTSIG_CALLID_RING0 ),
   _STR( eEPTSIG_CALLID_RING1 ),
   _STR( eEPTSIG_CALLID_RING2 ),
   _STR( eEPTSIG_CALLID_RING3 ),
   _STR( eEPTSIG_CALLID_RING4 ),
   _STR( eEPTSIG_CALLID_RING5 ),
   _STR( eEPTSIG_CALLID_RING6 ),
   _STR( eEPTSIG_CALLID_RING7 ),
   _STR( eEPTSIG_REORDER ),
   _STR( eEPTSIG_SPLASH ),
   _STR( eEPTSIG_RINGBACK ),
   _STR( eEPTSIG_STUTTER ),
   _STR( eEPTSIG_CALLW1 ),
   _STR( eEPTSIG_CALLW2 ),
   _STR( eEPTSIG_CALLW3 ),
   _STR( eEPTSIG_CALLW4 ),
   _STR( eEPTSIG_SAS_CAS1 ),
   _STR( eEPTSIG_SAS_CAS2 ),
   _STR( eEPTSIG_SAS_CAS3 ),
   _STR( eEPTSIG_SAS_CAS4 ),
   _STR( eEPTSIG_SPECIAL_DIAL ),
   _STR( eEPTSIG_DENY ),
   { CMGR_INVALID_HANDLE, "TONE_UNKNOWN" }
};

CALLCLIENTTXTMAP ccSigActionMap[] =
{
   { eCMGREPSIGACT_START,     "START"  },
   { eCMGREPSIGACT_STOP,      "STOP"   },
   { eCMGREPSIGACT_REPEAT,    "REPEAT" },
   { CMGR_INVALID_HANDLE,     "INVALID" }
};

CALLCLIENTTXTMAP ccPubEvtMap[] =
{
   _STR( eCMGREVT_ONHOOK ),
   _STR( eCMGREVT_OFFHOOK ),
   _STR( eCMGREVT_LINE_ACTIVE ),
   _STR( eCMGREVT_LINE_INACTIVE ),
   _STR( eCMGREVT_MWI_ON ),
   _STR( eCMGREVT_MWI_OFF ),
   _STR( eCMGREVT_OPER_OK ),
   _STR( eCMGREVT_OPER_NOK ),
   _STR( eCMGREVT_OPER_NOK_FORBIDDEN),
   _STR( eCMGREVT_CALL_STATS ),
   _STR( eCMGREVT_GLOB_STATS ),
   _STR( eCMGREVT_NO_SEC ),
   _STR( eCMGREVT_CALL_CONNECT ),
   _STR( eCMGREVT_CALL_ENDED ),
#if CMGRCFG_DEBUG_INFO
   _STR( eCMGREVT_DEBUG_INFO ),
#endif
   { CMGR_INVALID_HANDLE, "UNKNOWN EVENT" }
};

CALLCLIENTTXTMAP ccSesEvtMap[] =
{
   _STR( eCMGRSESCMD_CREATE ),
   _STR( eCMGRSESCMD_UPDATE ),
   _STR( eCMGRSESCMD_MUTE ),
   _STR( eCMGRSESCMD_TERMINATE ),
   _STR( eCMGRSESCMD_STATS ),
   { CMGR_INVALID_HANDLE, "UNKNOWN EVENT" }
};

int cmgrIntStateBox[] =
{
   IDC_STATE,
   IDC_STATE3,
};

int appLCD[] =
{
   IDC_DISPLAY,
   IDC_DISPLAY3,
};

int appCtlTone[] =
{
   IDC_TONE,
   IDC_TONE3,
};

int appCtlToneAction[] =
{
   IDC_EPT_ACTION,
   IDC_EPT_ACTION3,
};

int appPubEvt[] =
{
   IDC_PUBLISH,
   IDC_PUBLISH3,
};

int appSesEvt[] =
{
   IDC_SESSION,
   IDC_SESSION3,
};

int appProvEvt[] =
{
   IDC_PROVIS,
   IDC_PROVIS3,
};

int appMwiLed[] =
{
   IDC_MWI_LED,
   IDC_MWI_LED3,
   IDC_MWI_LED_OFF,
   IDC_MWI_LED_OFF3
};

/*
 * The following list mirrors the internal states defined in priv/cmFsm.h
 * This is added for debugging purposes only.
 */
char *cmgrIntStateList[]=
{
   "IDLE",
   "ACCEPTED",
   "DIALING",
   "RINGBACK",
   "RINGING",
   "TALK",
   "FLASH",
   "RELEASING",
   "ERROR",

   "INVALID"
};

/* ---- Private Variables ------------------------------------------------------- */

unsigned char EthernetAddress[7] = "\x00\xE0\x4C\x03\x1C\xD2";
// unsigned char ProfileAuid[64] = "com.cablelabs.pkt.rst.featureset.1";
unsigned char ProfileAuid[64] = "";
unsigned char Tone1[64] = "file:///PacketCableRST/ot";
unsigned char Tone2[64] = "file:///PacketCableRST/dl";
unsigned char Tone3[64] = "file:///PacketCableRST/zz";
unsigned char SessionIdentifier[64] = "CMGR_CCTK_20___PC_CLIENT";
unsigned char HotLineUri[64] = "sip:hotline@192.168.1.1";
unsigned char DomainVar[64] = "";

char LocalAddr[2][MAX_STR];    // Local IP address (v4 and v6)
char LocalFqdn[MAX_STR];       // Local FQDN
char ProxyAddr[MAX_STR];       // SIP Proxy IP address
char RegistrarAddr[MAX_STR];   // SIP Registrar IP address
char OBProxyAddr[MAX_STR];     // SIP Outbound proxy
char MWIAddr[MAX_STR];         // MWI Server
char NATAddr[MAX_STR];         // NAT IP Address
unsigned int LocalSipPort;     // SIP listening port
unsigned int SipPort;          // SIP port
unsigned int ProxyPort;        // SIP Proxy port
unsigned int RegistrarPort;    // SIP Registrar port
unsigned int OBProxyPort;      // Outbound Proxy port
unsigned int MWIPort;          // MWI Server port
unsigned int NATPort;          // Nat
char PhoneExt[MAX_STR];        // Phone Extension
char Password[MAX_STR];        // Password
char DigitMapAbnf[MAX_DIGITMAP_STR]; // Digit Map ABNF
char User[CMGRCFG_USER_MAX][MAX_STR]; // Username
char PubIdent[CMGRCFG_USER_MAX][2*MAX_STR]; // Public identity
int RegTimer;                  // Registration Timer
int InterfaceIdx;                // Interface Index
BOS_BOOL CallWaiting;              // CallWaiting feature 
BOS_BOOL LooseRouting;             // Loose Routing used with OB proxy
int LineFocus;
BOS_BOOL bLine1RingerOn = BOS_FALSE;
BOS_BOOL bLine2RingerOn = BOS_FALSE;
BOS_BOOL bRingVisualInitialized = BOS_FALSE;

#define SPDIAL_MAP_CODE_1  "5"
#define SPDIAL_MAP_STR_1   "1234"
#define SPDIAL_MAP_CODE_2  "12"
#define SPDIAL_MAP_STR_2   "6042338630"

/*
char  digitMapAbnf[2048] =  "domain = \"@10.1.0.7\"\r\n\r\n"                               \
                            "MAP MainTable = \r\n"                                         \
                            "   \"(=PhoneNumbers)\" : MAKE-CALL (\"sip:\" #1 =domain)\r\n" \
                            "   \"x{-15}T\"         : REORDER\r\n\r\n"                     \
                            "MAP PhoneNumbers =\r\n\r\n"                                   \
                            "   \"2x{4}\"     : RETURN(#0)\r\n\r\n\0";
*/

CALLCLIENTCNX CnxInUse[ CMGRCFG_MEDSES_MAX ];


CFGSIPUSERCFG cfgUsr[ CMGRCFG_USER_MAX ];
CFGSIPDIGITMAP cfgDigMap[ CMGRCFG_USER_MAX ];
CFGSIPDIGITMAPEXTVAR cfgDigMapExtVar;
CFGSIPPCSCF cfgPcscf[ CMGRCFG_USER_MAX ];
CFGSIPUSERENDPNTMAP cfgEpt[ CMGRCFG_USER_MAX ];
CFGSIPUSERDOMAINLIST cfgDomain[ CMGRCFG_USER_MAX ];

/* SIP Remote user */
CFGSIPREMOTEUSERSYSCFG cfgRemUsrSysCfg;
CFGSIPREMOTEUSERCFG cfgRemUsrCfg[ CMGRCFG_USER_MAX ];
char RemoteUser[CMGRCFG_USER_MAX][MAX_STR]; // Username
BOS_UINT8 uCfgRemCredAddr[ CMGRCFG_USER_MAX ][ MAX_STR ];


BOS_UINT8 uCfgActiveDomain[ CMGRCFG_USER_MAX ][ MAX_STR ];

CFGPROVDEVICECFG cfgDevice;
CFGEPTCODECCFG cfgCodec[ CMGRCFG_USER_MAX ];
CFGEPTLINECFG cfgLine[ CMGRCFG_USER_MAX ];
CFGSIPMWI cfgMwi;
CFGSIPCID cfgCid;
CFGSIPCIDDISPLAY cfgClidDsp;
CFGSIPAUTOCALL cfgAc;
CFGSIPAUTOCALL cfgAr;
CFGSIPEMERGE cfgEmergency;
CFGSIPCALLFWD cfgCfv;
CFGSIPDND cfgDnd;
CFGSIPTHREEWAYCALL cfg3Way;
CFGSIPCALLXFER cfgXfer;
CFGSIPBASICCALL cfgBasic;
CFGSIPNOANS cfgNoAns;
CFGSIPCIDBLOCK cfgCidBlk;
CFGSIPCALLHOLD cfgHold;
CFGSIPCOT cfgCot;
CFGEPTCODECLIST cfgCodecLst;
CFGEPTCODECLIST cfgCodecLst2;
CFGEPTCODECLIST cfgCodecLst3;
CFGEPTCODECLIST cfgCodecLst4;
CFGEPTCODECLIST cfgCodecLst5;
CFGEPTCODECLIST cfgCodecLst6;
CFGEPTCODECLIST cfgCodecLst7;
CFGSIPCIDDEL cfgCidDel;
CFGSIPHOTLINE cfgHotLine;
CFGSIPCALLWAIT cfgCWait;
CFGSIPACTSTAT cfgActStat;
CFGSIPHELDMEDIA cfgHeld;

CMGREPEVT cmgrEptEvt = NULL;
BOS_BOOL gbCmgrOperational = BOS_FALSE;

int gMWI_LED_ON_hdl = 0;
int gMWI_LED_OFF_hdl = 0;
int gMWI_LED_ON3_hdl = 0;
int gMWI_LED_OFF3_hdl = 0;

int gHandset_Onhook_Line1 = 0;
int gHandset_Offhook_Line1 = 0;
int gHandset_Onhook_Line2 = 0;
int gHandset_Offhook_Line2 = 0;

int gBRCM_LOGO_hdl = 0;
int gBRCM_LOGO_REV_hdl = 0;
int gBRCM_LOGO3_hdl = 0;
int gBRCM_LOGO3_REV_hdl = 0;

/* ---- Private Function Prototypes --------------------------------------------- */
#ifdef __cplusplus
extern "C"
#endif

void appStart(void);
INT_PTR CALLBACK appDlgConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void cmgrStatusCb ( BOS_UINT32 uRid, CMGRSTATUS eStatus );
void cmgrEptCtlCb ( CMGREPCMD *pCmd );
void cmgrEvtCb ( CMGREVT eEvt, BOS_UINT32 uUsr, void *pData );
void cmgrCfgInit ( void );

BOS_STATUS cmgrProvCb( PROV_CFG_ITEM cfgItem,
                       PROV_CFG_IDX *pCfgIx,
                       void *pData,
                       CFG_ACCESS_ACT eCfgAct );

BOS_UINT32 cmgrSesCtlCb( CMGRSESCMD eCmd,
                         BOS_UINT32 uPhy,
                         BOS_UINT32 uHdl,
                         void *pData );

BOS_UINT32 cmgrFindUsrCfgBlk( BOS_UINT8 *pUsr );
BOS_UINT32 cmgrFindPcscfCfgBlk( BOS_UINT8 *pUsr );
BOS_UINT32 cmgrFindRemoteUsrCfgBlk( BOS_UINT8 *pUsr );

BOS_STATUS cfgAccessProvSecConceptGet (CFGSECTLSCFG **ppEntry);
BOS_STATUS cfgAccessProvSecConceptSet (CFGSECTLSCFG *pEntry);
BOS_STATUS cfgAccessProvSecConceptDel (CFGSECTLSCFG *pEntry);
void appDisplay( int endpt, int row, int col, char *text);
void appDisplayClear( int line );
void setCctkTrace( void );
void initMwiLed( void );
void setMwiLed( int usr, int state );
void initHandset( void );
void setHandset( int usr, int offhook );
void initRingingVisual( void );
void setRingingVisual( int usr, int state );
void cadenceThread( void );

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   appDisplayClear
**
** PURPOSE:    Clears the display information of a specific line.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void appDisplayClear( int line )
{
   ListBox_ResetContent(GetDlgItem(hDlg, appLCD[line]));
   ListBox_AddString(GetDlgItem(hDlg, appLCD[line]), "");
   ListBox_AddString(GetDlgItem(hDlg, appLCD[line]), "");
}


/***********************************************************************************
** FUNCTION:   callClientMapGetId
**
** PURPOSE:    Maps an identified resource to another one in an ID based map.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT32 callClientMapGetId ( CALLCLIENTMAP *pMap, BOS_UINT32 uId )
{
   BOS_UINT32 uRet = CMGR_INVALID_HANDLE;

   if ( pMap )
   {
      while ( (pMap->uId != CMGR_INVALID_HANDLE) &&
              (pMap->uId != uId) )
      {
         pMap++;
      }
      uRet = pMap->uMap;
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   callClientMapGetStr
**
** PURPOSE:    Maps an identified resource to its human readable expression.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, NULL pointer if no valid
**             mapping can be obtained.
***********************************************************************************/
char *callClientMapGetStr ( CALLCLIENTTXTMAP *pMap, BOS_UINT32 uId )
{
   char *pRet = (char *)CMMAP__INVALID;

   if ( pMap )
   {
      while ( (pMap->uId != CMGR_INVALID_HANDLE) &&
              (pMap->uId != uId) )
      {
         pMap++;
      }
      pRet = pMap->pMap;
   }

   return pRet;
}


/***********************************************************************************
** FUNCTION:   callClientMapFocusCall
**
** PURPOSE:    Maps the focus call to the line.
**
** PARAMETERS: uLine - the line identifier to retrieve the focus call from.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT32 callClientMapFocusCall ( BOS_UINT32 uLine )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX ; uIx++ )
   {
      if ( CnxInUse[ uIx ].bInUse &&
           CnxInUse[ uIx ].bFocus &&
           CnxInUse[ uIx ].uLine == uLine )
      {
         return uIx;
      }
   }

   return CMGR_INVALID_HANDLE;
}


/***********************************************************************************
** FUNCTION:   appInit
**
** PURPOSE:    Initializes the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void appInit(void)
{
   int rc;
   SYSTEMTIME time;
   HFONT font;

   GetLocalTime(&time);

   font = CreateFont( 
      14, 0, 0, 0, // height, width, escapement, orientation
      FW_MEDIUM, 0, 0, 0, 
      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
      DEFAULT_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
      "Lucida Console");
   SendMessage( GetDlgItem( hDlg, IDC_DISPLAY), WM_SETFONT, (WPARAM) font, 0);
   SendMessage( GetDlgItem( hDlg, IDC_DISPLAY3), WM_SETFONT, (WPARAM) font, 0);

   Log("-----  CallManager Startup %02d/%02d/%02d ------------",
       time.wMonth, time.wDay, time.wYear);

   rc = GetProfileInt("CCMGR", "LogPkts", 1);
   if (rc)
   {
      PostMessage(hWnd, WM_COMMAND, IDM_PKTS, 0); //Enable packet logging at startup
   }
   rc = GetProfileInt("CCMGR", "LogHdrOnly", 1);
   if (rc)
   {
      PostMessage(hWnd, WM_COMMAND, IDM_PKTHDR, 0); //Enable packet header logging at startup
   }

   initMwiLed();
   initHandset();
   initRingingVisual();




   memset ( CnxInUse, 0, sizeof( CnxInUse ) );
   appSetup();
}


/***********************************************************************************
** FUNCTION:   appSetup
**
** PURPOSE:    Configures the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void appSetup(void)
{
   int rc;

   Log("CallManager configure dialog");

   rc = DialogBox( hInst, (LPCTSTR) IDD_DLGCFGSIP, hDlg, (DLGPROC) appDlgConfig);
}

/***********************************************************************************
** FUNCTION:   appStart
**
** PURPOSE:    Starts the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void appStart(void)
{
   int i;
   char *pcChar;
   CMGRCFGBLK cmgrCfg;
   BOS_TASK_ID cadenceThreadId;

   // load configurations from profile
   GetProfileString("CCMGR", "LocalAddress", 0, LocalAddr[0], sizeof(LocalAddr[0]));
   GetProfileString("CCMGR", "LocalFQDN", 0, LocalFqdn, sizeof(LocalFqdn));

   GetProfileString("CCMGR", "PhoneExt", 0, PhoneExt, MAX_STR);
   sprintf( (char *)User[0], "%s1", PhoneExt );
   sprintf( (char *)User[1], "%s2", PhoneExt );

   GetProfileString("CCMGR", "Password", 0, Password, MAX_STR);
#ifdef CCTKCFG_USE_CABLE_SIMULATOR
   if ( !strlen ( Password ) )
   {
      sprintf( (char *)Password, "broadcom" );
   }
#else
   if ( !strlen ( Password ) )
   {
      sprintf( (char *)Password, "broadcom" );
   }
#endif

   /* Decode multiple lines for digit map */
   GetProfileString("CCMGR", "DigitMapAbnf", 0, DigitMapAbnf, sizeof(DigitMapAbnf));
   pcChar = strstr( DigitMapAbnf, "\t\\" );
   while ( pcChar != NULL )
   {
      *pcChar = '\r';
      *(pcChar + 1) = '\n';
      pcChar = strstr( DigitMapAbnf, "\t\\" );
   }

   LocalSipPort = (unsigned int)GetProfileInt("CCMGR", "LocalSipPort", 0);
   RegTimer = (int)GetProfileInt("CCMGR", "RegistrationTimer", 0);

   GetProfileString("CCMGR", "ProxyAddr", "", ProxyAddr, MAX_STR);
   ProxyPort = (unsigned int)GetProfileInt("CCMGR", "ProxyPort", 0);

   GetProfileString("CCMGR", "RegistrarAddr", "", RegistrarAddr, MAX_STR);
   RegistrarPort = (unsigned int)GetProfileInt("CCMGR", "RegistrarPort", 0);

   sprintf( (char *)PubIdent[0], "%s1@%s", PhoneExt, RegistrarAddr );
   sprintf( (char *)PubIdent[1], "%s2@%s", PhoneExt, RegistrarAddr );

   GetProfileString("CCMGR", "OutboundProxyAddr", "", OBProxyAddr, MAX_STR);
   OBProxyPort = (unsigned int)GetProfileInt("CCMGR", "OutboundProxyPort", 0);
   GetProfileString("CCMGR", "MWIServerAddr", "", MWIAddr, MAX_STR);
   MWIPort = (unsigned int)GetProfileInt("CCMGR", "MWIServerPort", 0);

   GetProfileString("CCMGR", "NATAddr", "", NATAddr, MAX_STR);
   NATPort = (unsigned int)GetProfileInt("CCMGR", "NATPort", 0);

   InterfaceIdx = (unsigned int)GetProfileInt("CCMGR", "InterfaceIdx", 0);

   LooseRouting = (BOOL)GetProfileInt("CCMGR", "looseRouting", 0);
   CallWaiting = (BOOL)GetProfileInt("CCMGR", "callWaiting", 0);

   /* set client default trace levels and groups */
   Button_SetCheck( GetDlgItem( hDlg, IDC_TRACE_INFO ), BST_CHECKED );
   Button_SetCheck( GetDlgItem( hDlg, IDC_TRACE_WARN ), BST_CHECKED );
   Button_SetCheck( GetDlgItem( hDlg, IDC_TRACE_DEBUG ), BST_CHECKED );
   Button_SetCheck( GetDlgItem( hDlg, IDC_TRACE_GRP_CCTK ), BST_CHECKED );

   /* Initializes the configuration information that the CMGR will use.
   */
   cmgrCfgInit();

   bosInit();

   /* Setup and initializes the call manager.
   */
   cmgrCfg.cmgrEptCtlCb = &cmgrEptCtlCb;
   cmgrCfg.cmgrEvtCb = &cmgrEvtCb;
   cmgrCfg.cmgrProvCb = &cmgrProvCb;
   cmgrCfg.cmgrSesCtlCb = &cmgrSesCtlCb;

   cmgrInit ( &cmgrStatusCb );
   cmgrStartup ( &cmgrStatusCb, &cmgrCfg );
   setCctkTrace();

   SetFocus( GetDlgItem( hDlg, IDC_HANDSET_ONHOOK));

   for (i = 0; i < CMGRCFG_USER_MAX; i++)
   {
      appDisplayClear ( i );
      appDisplay( i, 1, 15, User[i]);
      appDisplay( i, 2, 1, "SYSTEM STARTING...");
   }

   /* Create Thread A */
   if ( bosTaskCreate( "CDNC",
                       4*1024,
                       BOS_TASK_CLASS_LOW,
                       (BOS_TASK_ENTRY)cadenceThread,
                       (BOS_TASK_ARG *)NULL,
                       &cadenceThreadId ) != BOS_STATUS_OK )
   {
      BOS_ASSERT( BOS_FALSE );
   }

}


/***********************************************************************************
** FUNCTION:   appExit
**
** PURPOSE:    Exits the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void appExit(void)
{
   Log( "Exit" );
   WriteProfileString("CCMGR", "LogPkts", 
      (GetMenuState( GetMenu(hWnd), IDM_PKTS, 0) & MF_CHECKED) ? "1" : "0");
   WriteProfileString("CCMGR", "LogHdrOnly", 
      (GetMenuState( GetMenu(hWnd), IDM_PKTHDR, 0) & MF_CHECKED) ? "1" : "0");
}


/***********************************************************************************
** FUNCTION:   appButton
**
** PURPOSE:    Button press on the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
BOS_BOOL appButton(int id, int msg)
{
   (void)msg;

   BOS_UINT32 line;
   BOS_UINT32 call = CMGR_INVALID_HANDLE;
   BOS_UINT32 key = 0;

   /* Show memory status */
   if ( id == IDC_MEMSHOW )
   {
      CMGRMEMSTATS memStats;

      memStats.bCctkOnly = 
         Button_GetCheck( GetDlgItem( hDlg, IDC_MEMSHOW_CCTKONLY ) );
      memStats.bTraceTable =
         Button_GetCheck( GetDlgItem( hDlg, IDC_MEMSHOW_TABLE ) );

      cmgrCmd ( eCMCMD_MEMSTATS, (void *) &memStats );
      return BOS_TRUE;
   }

   /* Show application data status */
   if ( id == IDC_DATASHOW )
   {
      CMGRSHOWSTATUS showStatus;

      showStatus.bShowCmgr = 
         Button_GetCheck( GetDlgItem( hDlg, IDC_DATASHOW_CMGR ) );
      showStatus.bShowCctk =
         Button_GetCheck( GetDlgItem( hDlg, IDC_DATASHOW_CCTK ) );

      cmgrCmd ( eCMCMD_STATUS, (void *) &showStatus );
      return BOS_TRUE;
   }

   if ( id == IDC_UPDATE_TRACE )
   {
      setCctkTrace();
   }

   if ( id == IDC_RESET )

   {

      cmgrCmd ( eCMCMD_RESTART, NULL );
      return BOS_TRUE;
   }
   

   if ( id == IDC_SHUTDOWN )

   {

      cmgrCmd ( eCMCMD_SHUTDOWN, NULL );
      return BOS_TRUE;
   }



   if ( id == IDC_RECONFIG )

   {
      CMGRRECONFIG reConfig;

      /* Apply a device reconfiguration.
      */
      memset ( (void *) &reConfig,
               0,
               sizeof ( CMGRRECONFIG ) ); 
      reConfig.eCfgAction = eCMCFGACT_CFGDEV;

      cmgrCmd ( eCMCMD_NEWCONFIG, (void *) &reConfig );
      return BOS_TRUE;
   }


   key = callClientMapGetId( appButtons, id );
   if (key == CMGR_INVALID_HANDLE)
   {
      return BOS_FALSE;
   }   

   line = callClientMapGetId( appButtonLine, id );
   
   switch (id)
   {
      case IDC_FAX1:
      case IDC_FAXEND1:
      case IDC_FAXFAIL1:
      case IDC_FAX3:
      case IDC_FAXEND3:
      case IDC_FAXFAIL3:
      {   
         call = callClientMapFocusCall( line );
      }
      break;
      
      default:
      break;
   }
   
   if ( cmgrEptEvt )
   {
      Log( "CMGR-EP-EVT: Line = %d - Call = %d - Event = %d", line, call, key );
      cmgrEptEvt ( key, line, call, NULL );
      
      LineFocus = line;
      return BOS_TRUE;

   }
   else
   {
      return BOS_FALSE;
   }
}


/***********************************************************************************
** FUNCTION:   appMenu
**
** PURPOSE:    Menu handling from the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
BOS_BOOL appMenu(int id)
{
   switch (id)
   {
      case IDM_PKTS:
      {      
         if (GetMenuState( GetMenu(hWnd), IDM_PKTS, 0) & MF_CHECKED)
         {
            CheckMenuItem(GetMenu(hWnd), IDM_PKTS, MF_UNCHECKED);
         }
         else
         {
            CheckMenuItem(GetMenu(hWnd), IDM_PKTS, MF_CHECKED);
         }
      }
      break;

      case IDM_PKTHDR:
      {
         if (GetMenuState( GetMenu(hWnd), IDM_PKTHDR, 0) & MF_CHECKED)
         {
            CheckMenuItem(GetMenu(hWnd), IDM_PKTHDR, MF_UNCHECKED);
         }
         else
         {
            CheckMenuItem(GetMenu(hWnd), IDM_PKTHDR, MF_CHECKED);
         }
      }      
      break;

      case IDM_START:
      {
         appStart();
      }      
      break;

      case IDM_SETUP:
      {      
         appSetup();
      }      
      break;

      default:
         return BOS_FALSE;
   }
   return BOS_TRUE;
}


/***********************************************************************************
** FUNCTION:   appDlgConfig
**
** PURPOSE:    Configuration window processing at the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
INT_PTR CALLBACK appDlgConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   (void)lParam;
   char sBuffer[MAX_STR];
   char sDigitMapBuffer[MAX_DIGITMAP_STR];
   char *pcChar;

   switch (message)
   {
      case WM_INITDIALOG:
      {      
         PostMessage(hDlg, WMUSR_INIT, 0, 0); //finish init after dialog displayed
      }
      break;

      case WMUSR_INIT:
      {
         int val;

         GetProfileString("CCMGR", "LocalAddress", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_LOCALADDR, sBuffer);

         GetProfileString("CCMGR", "LocalFQDN", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_LOCALFQDN, sBuffer);

         GetProfileString("CCMGR", "PhoneExt", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_PHONEXT, sBuffer);

         GetProfileString("CCMGR", "Password", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_PASSWORD, sBuffer);

         /* Decode multiple lines for digit map */
         GetProfileString("CCMGR", "DigitMapAbnf", 0, sDigitMapBuffer, MAX_DIGITMAP_STR);
         pcChar = strstr( sDigitMapBuffer, "\t\\" );
         while ( pcChar != NULL )
         {
            *pcChar = '\r';
            *(pcChar + 1) = '\n';
            pcChar = strstr( sDigitMapBuffer, "\t\\" );
         }
         SetDlgItemText(hDlg, IDC_DIGITMAPABNF, sDigitMapBuffer);

         val = GetProfileInt("CCMGR", "LocalSipPort", 0);
         SetDlgItemInt(hDlg, IDC_LOCALSIPPORT, val, FALSE);

         val = GetProfileInt("CCMGR", "RegistrationTimer", 0);
         SetDlgItemInt(hDlg, IDC_REGTIMER, val, FALSE);
    
         GetProfileString("CCMGR", "ProxyAddr", "", sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_PROXYIP, sBuffer);

         val = GetProfileInt("CCMGR", "ProxyPort", 0);
         SetDlgItemInt(hDlg, IDC_PROXYPORT, val, FALSE);

         GetProfileString("CCMGR", "RegistrarAddr", "", sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_REGISTRARIP, sBuffer);

         val = GetProfileInt("CCMGR", "RegistrarPort", 0);
         SetDlgItemInt(hDlg, IDC_REGISTRARPORT, val, FALSE);

         GetProfileString("CCMGR", "OutboundProxyAddr", "", sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_OBPROXYIP, sBuffer);

         val = GetProfileInt("CCMGR", "OutboundProxyPort", 0);
         SetDlgItemInt(hDlg, IDC_OBPROXYPORT, val, FALSE);

         GetProfileString("CCMGR", "MWIServerAddr", "", sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_MWIIP, sBuffer);

         val = GetProfileInt("CCMGR", "MWIServerPort", 0);
         SetDlgItemInt(hDlg, IDC_MWIPORT, val, FALSE);

         GetProfileString("CCMGR", "NATAddr", "", sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_NATIP, sBuffer);

         val = GetProfileInt("CCMGR", "NATPort", 0);
         SetDlgItemInt(hDlg, IDC_NATPORT, val, FALSE);
 
         val = GetProfileInt("CCMGR", "InterfaceIdx", 0);
         SetDlgItemInt(hDlg, IDC_INTERFACEIDX, val, FALSE);

         LooseRouting = GetProfileInt("CCMGR", "looseRouting", 0);
         Button_SetCheck(GetDlgItem(hDlg, IDC_LRPREF), LooseRouting);

         CallWaiting = GetProfileInt("CCMGR", "callWaiting", 0);
         Button_SetCheck(GetDlgItem(hDlg, IDC_CALLWAITING), CallWaiting);
      }
      break;

      case WM_COMMAND:
      {
         if (LOWORD(wParam) == IDOK)
         {
            EndDialog(hDlg, TRUE);
         }
         if (LOWORD(wParam) == IDCANCEL)
         {
            EndDialog(hDlg, FALSE);
         }
      }
      break;
   
      case WM_DESTROY:
      {
         GetDlgItemText(hDlg, IDC_LOCALADDR, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "LocalAddress", sBuffer);

         GetDlgItemText(hDlg, IDC_LOCALFQDN, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "LocalFQDN", sBuffer);

         GetDlgItemText(hDlg, IDC_PHONEXT, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "PhoneExt", sBuffer);

         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_LOCALSIPPORT, NULL, FALSE));
         WriteProfileString("CCMGR", "LocalSipPort", sBuffer); 

         GetDlgItemText(hDlg, IDC_PASSWORD, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "Password", sBuffer);

         /* Encode multiple lines for digit map */
         GetDlgItemText(hDlg, IDC_DIGITMAPABNF, sDigitMapBuffer, MAX_DIGITMAP_STR);
         pcChar = strstr( sDigitMapBuffer, "\r\n" );
         while ( pcChar != NULL )
         {
            *pcChar = '\t';
            *(pcChar + 1) = '\\';
            pcChar = strstr( sDigitMapBuffer, "\r\n" );
         }
         WriteProfileString("CCMGR", "DigitMapAbnf", sDigitMapBuffer);

         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_REGTIMER, NULL, FALSE)); 
         WriteProfileString("CCMGR", "RegistrationTimer", sBuffer);

         GetDlgItemText(hDlg, IDC_PROXYIP, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "ProxyAddr", sBuffer);

         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_PROXYPORT, NULL, FALSE));
         WriteProfileString("CCMGR", "ProxyPort", sBuffer);

         GetDlgItemText(hDlg, IDC_REGISTRARIP, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "RegistrarAddr", sBuffer);

         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_REGISTRARPORT, NULL, FALSE)); 
         WriteProfileString("CCMGR", "RegistrarPort", sBuffer);

         GetDlgItemText(hDlg, IDC_OBPROXYIP, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "OutboundProxyAddr", sBuffer);

         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_OBPROXYPORT, NULL, FALSE));
         WriteProfileString("CCMGR", "OutboundProxyPort", sBuffer);
      
         GetDlgItemText(hDlg, IDC_MWIIP, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "MWIServerAddr", sBuffer);

         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_MWIPORT, NULL, FALSE));
         WriteProfileString("CCMGR", "MWIServerPort", sBuffer);

         GetDlgItemText(hDlg, IDC_NATIP, sBuffer, MAX_STR);
         WriteProfileString("CCMGR", "NATAddr", sBuffer);

         sprintf(sBuffer, "%d", GetDlgItemInt(hDlg, IDC_NATPORT, NULL, FALSE)); 
         WriteProfileString("CCMGR", "NATPort", sBuffer);
         
         sprintf(sBuffer, "%d", ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_SRTPLIST)));
         WriteProfileString("CCMGR", "srtpPref", sBuffer);

         sprintf(sBuffer, "%d", ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_PRACKLIST)));
         WriteProfileString("CCMGR", "prackPref", sBuffer);

         sprintf( sBuffer, "%d", ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_TRANSPORTLIST)));
         WriteProfileString("CCMGR", "transPref", sBuffer);

         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_INTERFACEIDX, NULL, FALSE));
         WriteProfileString("CCMGR", "InterfaceIdx", sBuffer);

         sprintf( sBuffer, "%d", Button_GetCheck( GetDlgItem(hDlg, IDC_LRPREF)));
         WriteProfileString("CCMGR", "looseRouting", sBuffer);

         sprintf( sBuffer, "%d", Button_GetCheck( GetDlgItem(hDlg, IDC_CALLWAITING)));
         WriteProfileString("CCMGR", "callWaiting", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck( GetDlgItem(hDlg, IDC_AUTOSTART)));
         WriteProfileString("CCMGR", "autoStart", sBuffer);
      }
      break;

      default:
        return FALSE;
   }
   return TRUE;
}


#if 0
/***********************************************************************************
** FUNCTION:   appState
**
** PURPOSE:    Update the dialog state for the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void appState( int line, char *state)
{
   SetDlgItemText( hDlg, appCtlState[line], state + 5);
}
#endif

/***********************************************************************************
** FUNCTION:   cmgrIntState
**
** PURPOSE:    Update the dialog state for the application.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void cmgrIntState( int line, char *state)
{
   SetDlgItemText( hDlg, cmgrIntStateBox[line], state);
}

/***********************************************************************************
** FUNCTION:   appTone
**
** PURPOSE:    Update the tone information for a tone being played.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void appTone( int line, int type)
{
   (void)line;
   (void) type;

   char *tone = "";
   char *action = "";
   
   if (strlen(tone) <= 7)
   {
      tone = "no tone";
   }
   
   if (strlen(action) <= 7)
   {
      tone = "no action";
   }
   
   SetDlgItemText( hDlg, appCtlTone[line], tone + 7);
   SetDlgItemText( hDlg, appCtlToneAction[line], action + 9);
}

/***********************************************************************************
** FUNCTION:   appDisplay
**
** PURPOSE:    Update the display information of a specific line.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void appDisplay( int endpt, int row, int col, char *text)
{
   char buf[65];
   strcpy( &buf[0],  "                ");
   strcpy( &buf[16], "                ");
   strcpy( &buf[32], "                ");
   strcpy( &buf[48], "                ");
   ListBox_GetText(GetDlgItem(hDlg, appLCD[endpt]), row-1, buf);
   buf[strlen(buf)] = ' '; // remove null terminator
   strcpy( &buf[col-1], text);
   ListBox_DeleteString(GetDlgItem(hDlg, appLCD[endpt]), row-1);
   ListBox_InsertString(GetDlgItem(hDlg, appLCD[endpt]), row-1, buf);
}


/***********************************************************************************
** FUNCTION:   appCmgrDeInit
**
** PURPOSE:    Terminate cleanup of the Call Manager application.
**
** PARAMETERS: None
**
** RETURNS:    None.
***********************************************************************************/
void appCmgrDeInit( void )
{
   cmgrDeinit();
}


/***********************************************************************************
**  FUNCTION:   cmgrStatusCb
**
**  PURPOSE:    Callback from Call Manager to deliver status information to the
**              outer application.
**
**  PARAMETERS: uRid - resource identifier, may be used in some specific status
**                     information when necessary.
**              eStatus - the status reported.
**
**  RETURNS:    Nothing.
**
***********************************************************************************/
void cmgrStatusCb ( BOS_UINT32 uRid, CMGRSTATUS eStatus )
{
   Log( "cmgrStatusCb: uRid = 0x%x - Status = %d", uRid, eStatus );

   switch ( eStatus )
   {
      case eCMGRSTATUS_STARTUP_SUCCESS:
      {
         gbCmgrOperational = BOS_TRUE;
      }
      break;

      case eCMGRSTATUS_SHUTDOWN_COMPLETE:
      {
         PostMessage( hDlg, WMUSR_CMGRDEINIT, 0, 0);
         appDisplay( 0, 2, 1, "SYSTEM SHUT DOWN");
         appDisplay( 1, 2, 1, "SYSTEM SHUT DOWN");

      }
      /* Fall through intentional.
      */
      case eCMGRSTATUS_STARTUP_FAILED:
      {
         gbCmgrOperational = BOS_FALSE;
      }
      break;

      case eCMGRSTATUS_SHUTDOWN_FAILURE:
      case eCMGRSTATUS_CRITICAL_ERROR:
      case eCMGRSTATUS_COMMAND_SUCCESS:
      case eCMGRSTATUS_COMMAND_FAILED:
      case eCMGRSTATUS_DEINIT_COMPLETE:
      case eCMGRSTATUS_DEINIT_FAILURE:
      default:
      break;
   }
}

/***********************************************************************************
** FUNCTION:   cmgrEptCtlCb
**
** PURPOSE:    Callback functionality for endpoint action control outside of
**             the Call Manager.
**
** PARAMETERS: pCmd - the command to be passed to the endpoint.  This is a
**                    generic command interface which can be used for many
**                    purposes.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
void cmgrEptCtlCb ( CMGREPCMD *pCmd )
{
   char *pClid = NULL;
   char buf[128];

   switch ( pCmd->eAction )
   {
      case eCMGRPHYCMD_INIT:
      {
         Log( "cmgrEptCtlCb: Initialized with %p", pCmd->cmd.init.evt );
         cmgrEptEvt = pCmd->cmd.init.evt;
      }
      break;

      case eCMGRPHYCMD_DEINIT:
      {
         Log( "cmgrEptCtlCb: De-initialized" );
         /* No more events from endpoint are expected.
         */
         cmgrEptEvt = NULL;
      }
      break;

      case eCMGRPHYCMD_CREATE:
      {
         Log( "cmgrEptCtlCb: Creating physical endpoint %d", pCmd->uPhy );
      }
      break;

      case eCMGRPHYCMD_DESTROY:
      {
         Log( "cmgrEptCtlCb: Destroying physical endpoint %d", pCmd->uPhy );
      }
      break;

      case eCMGRPHYCMD_SIGNAL:
      {
         /* Start or stop tones.
         */
         BOS_UINT32 value;
         Log( "cmgrEptCtlCb: Endpoint [%d], Signal [%d], action [%d], seshdl [0x%x], apphdl [0x%x]", pCmd->uPhy, (EPTSIG)pCmd->cmd.sig.uSig,  pCmd->cmd.sig.eAct, pCmd->cmd.sig.uSes, pCmd->cmd.sig.uApp );
         switch ( (EPTSIG)pCmd->cmd.sig.uSig )
         {
            case eEPTSIG_CALLID:
            {
               pClid = (char *)pCmd->cmd.sig.pData;

               if ( pClid[13+1] == 'P' )
               {
                  appDisplay(pCmd->uPhy, 2, 1, "Private");
               }
               else
               {
                  appDisplay(pCmd->uPhy, 2, 1, strtok( pClid+13+1, "\"" ));
                  appDisplay(pCmd->uPhy, 2, strlen(pClid+13+1)+1, strtok( NULL, "," ));
               }
            }
            break;
            case eEPTSIG_RINGING:
            case eEPTSIG_RING0:
            case eEPTSIG_RING1:
            case eEPTSIG_RING2:
            case eEPTSIG_RING3:
            case eEPTSIG_RING4:
            case eEPTSIG_RING5:
            case eEPTSIG_RING6:
            case eEPTSIG_RING7:
            case eEPTSIG_CALLID_RINGING:
            case eEPTSIG_CALLID_RING0:
            case eEPTSIG_CALLID_RING1:
            case eEPTSIG_CALLID_RING2:
            case eEPTSIG_CALLID_RING3:
            case eEPTSIG_CALLID_RING4:
            case eEPTSIG_CALLID_RING5:
            case eEPTSIG_CALLID_RING6:
            case eEPTSIG_CALLID_RING7:
            {
               switch ( pCmd->cmd.sig.eAct )
               {
                  case eCMGREPSIGACT_START:
                  /* Treat this as start for now. */
                  case eCMGREPSIGACT_REPEAT:
                  {
                     pCmd->uPhy == 0?(bLine1RingerOn = BOS_TRUE):(bLine2RingerOn = BOS_TRUE);
                  }
                  break;

                  case eCMGREPSIGACT_STOP:
                  default:
                  {
                     pCmd->uPhy == 0?(bLine1RingerOn = BOS_FALSE):(bLine2RingerOn = BOS_FALSE);
                  }
                  break;
               }
            }
            break;

            default:
            {
               switch ( pCmd->cmd.sig.eAct )
               {
                  case eCMGREPSIGACT_START:
                  /* Treat this as start for now. */
                  case eCMGREPSIGACT_REPEAT:
                  {
                     value = 1; /* Signal ON */
                  }
                  break;

                  case eCMGREPSIGACT_STOP:
                  default:
                  {
                     value = 0; /* Signal OFF */
                  }
                  break;
               }
            }
            break;
         }

         /* Apply the desired signal on the endpoint.
         */
         sprintf( buf, "%s-%d",callClientMapGetStr( ccSigMap, pCmd->cmd.sig.uSig ), pCmd->cmd.sig.uSig );
         SetDlgItemText( hDlg,
                         appCtlTone[pCmd->uPhy],
                         buf );
         SetDlgItemText( hDlg, 
                         appCtlToneAction[pCmd->uPhy], 
                         callClientMapGetStr( ccSigActionMap, pCmd->cmd.sig.eAct ));
      }
      break;

      default:
      break;
   }
}


/***********************************************************************************
** FUNCTION:   cmgrEvtCb
**
** PURPOSE:    Callback functionality for event publication outside of
**             the Call Manager.
**
** PARAMETERS: eEvt  - the event of interest published.
**             uUsr  - the user for which this event is being published.
**             pData - additional data (may be NULL) associated to the
**                     published event.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
void cmgrEvtCb ( CMGREVT eEvt, BOS_UINT32 uUsr, void *pData )
{
   (void) uUsr;
   (void) pData;

#if CMGRCFG_DEBUG_INFO
   if ( eEvt != eCMGREVT_DEBUG_INFO )
   {
#endif
      Log( "cmgrEvtCb: User(0x%x), eEvt (%d)%s", uUsr, eEvt, callClientMapGetStr( ccPubEvtMap, eEvt ) );
#if CMGRCFG_DEBUG_INFO
   }
#endif

   if ( (uUsr <= 1) 
#if CMGRCFG_DEBUG_INFO
         && 
         (eEvt != eCMGREVT_DEBUG_INFO)
#endif
         ) 
   {
      SetDlgItemText( hDlg,
                      appPubEvt[uUsr],
                      callClientMapGetStr( ccPubEvtMap, eEvt )
                      );
   }

   switch ( eEvt )
   {
      case eCMGREVT_ONHOOK:
      case eCMGREVT_OFFHOOK:
      case eCMGREVT_LINE_ACTIVE:
      case eCMGREVT_LINE_INACTIVE:
      case eCMGREVT_CALL_STATS:
      case eCMGREVT_GLOB_STATS:
      case eCMGREVT_CALL_CONNECT:
      case eCMGREVT_CALL_ENDED:
      break;
      case eCMGREVT_OPER_OK:
      {
         appDisplay( uUsr, 2, 1, "Hello & Welcome!");
      }
      break;
      case eCMGREVT_OPER_NOK:
      {
         appDisplay( uUsr, 2, 1, "OUT OF SERVICE");
      }
      break;
      case eCMGREVT_OPER_NOK_FORBIDDEN:
      {
         appDisplay( uUsr, 2, 1, "OUT OF SERVICE: FORBIDDEN");
      }
      break;
      case eCMGREVT_MWI_ON:
      {
         setMwiLed( uUsr, 1 );
      }
      break;
      case eCMGREVT_MWI_OFF:
      {
         setMwiLed( uUsr, 0 );
      }
      break;
#if CMGRCFG_DEBUG_INFO
      case eCMGREVT_DEBUG_INFO:
      {
         if ( uUsr <= 1 )
         {
            /* CMGR internal state information */
            cmgrIntState( uUsr, cmgrIntStateList[(int)(pData)]);
         }
         else
         {
            Log( "cmgrEvtCb: Unknown User(0x%x), state %s", uUsr, cmgrIntStateList[(int)(pData)] );
         }
      }
      break;
#endif
      default:
      break;
   }
}


/****************************************************************************
** FUNCTION:   cmgrProvCb
**
** PURPOSE:    Callback functionality for provisioning control outside of
**             the Call Manager.
**
** PARAMETERS: cfgItem  - the provisioning item of interest.
**             pCfgIx   - the index associated with the provisioning item.
**             pData    - pointer to the placeholder for the provisioning
**                        information.
**             eCfgAct  - the provisioning action to take.
**
** RETURNS:    BOS_STATUS_OK on success, BOS_STATUS_ERROR otherwise.
**
** NOTE:    
*****************************************************************************/
BOS_STATUS cmgrProvCb( PROV_CFG_ITEM cfgItem,
                       PROV_CFG_IDX *pCfgIx,
                       void *pData,
                       CFG_ACCESS_ACT eCfgAct )
{
   BOS_STATUS res = BOS_STATUS_OK;

   if ( pCfgIx )
   {
      Log( "cmgrProvCb: Item (%d) - Id (%d, %s) - Action (%d)",
        cfgItem,
        pCfgIx->uIndex,
        (pCfgIx->pcCharIdx != NULL) ? (const char *)pCfgIx->pcCharIdx : "(null)",
        eCfgAct );
   }
   else
   {
      Log( "cmgrProvCb: Item (%d) - Id (NULL) - Action (%d)",
        cfgItem,
        eCfgAct );
   }

   switch ( eCfgAct )
   {
      case CFG_ITEM_GET:
      {
         switch ( cfgItem )
         {
            case CFGITEM(PRV, DevConcept):
            {
               *((CFGPROVDEVICECFG **)pData) = &cfgDevice;
            }
            break;

            case CFGITEM(EPT,  CodecConcept):
            {
               BOS_UINT32 uId = pCfgIx->uIndex;
               
               if ( uId != CMGR_INVALID_HANDLE )
               {
                  *((CFGEPTCODECCFG **)pData) = &cfgCodec[uId];
               }
               else
               {
                  res = BOS_STATUS_ERR;
               }
            }
            break;

            case CFGITEM(EPT,  LineConcept):
            {
               BOS_UINT32 uId = pCfgIx->uIndex;
               
               if ( uId != CMGR_INVALID_HANDLE )
               {
                  *((CFGEPTLINECFG **)pData) = &cfgLine[uId];
               }
               else
               {
                  res = BOS_STATUS_ERR;
               }
            }
            break;

            case CFGITEM(EPT,  CodecList):
            {
               *((CFGEPTCODECLIST **)pData) = &cfgCodecLst;
            }
            break;

            case CFGITEM(SIP, DigitMapConcept):
            {
               BOS_UINT32 uId = cmgrFindUsrCfgBlk ( pCfgIx->pcCharIdx );
               
               if ( uId != CMGR_INVALID_HANDLE )
               {
                  *((CFGSIPDIGITMAP **)pData) = &cfgDigMap[uId];
                  SetDlgItemText( hDlg, appProvEvt[uId], "SIPDigitMapConcept - GET" );
               }
               else
               {
                  res = BOS_STATUS_ERR;
               }
            }
            break;

            case CFGITEM(SIP,  DigitMapExtVarConcept):
            {
               /* Only support one such variable, used for the domain setting from
               ** within the digit map.  No other need for now.
               */
               *((CFGSIPDIGITMAPEXTVAR **)pData) = &cfgDigMapExtVar;
            }
            break;

            case CFGITEM(SIP, PCSCFConcept):
            {
               BOS_UINT32 uId = cmgrFindPcscfCfgBlk ( pCfgIx->pcCharIdx );
               
               if ( uId != CMGR_INVALID_HANDLE )
               {
                  *((CFGSIPPCSCF **)pData) = &cfgPcscf[uId];
                  SetDlgItemText( hDlg, appProvEvt[uId], "SIPPCSCFConcept - GET" );
               }
               else
               {
                  res = BOS_STATUS_ERR;
               }
            }
            break;

            case CFGITEM(SIP, UserConcept):
            {
               if ( pCfgIx->pcCharIdx == NULL )
               {
                  /* Initial query, we want to return *all* the possible users.
                  */
#if defined(CCTKCFG_USE_CABLE_SIMULATOR) || CALL_CLIENT_CFG_ONE_USER
                  cfgUsr[0].pNext = NULL;
#else
                  cfgUsr[0].pNext = &cfgUsr[1];
#endif
                  cfgUsr[1].pNext = NULL;
                  
                  *((CFGSIPUSERCFG **)pData) = &cfgUsr[0];
               }
               else
               {
                  BOS_UINT32 uId = cmgrFindUsrCfgBlk ( pCfgIx->pcCharIdx );
                  
                  if ( uId != CMGR_INVALID_HANDLE )
                  {
                     cfgUsr[uId].pcActiveDomain =
                        reinterpret_cast< BOS_UINT8* >( &uCfgActiveDomain[uId][0] );
                     *((CFGSIPUSERCFG **)pData) = &cfgUsr[uId];
                     SetDlgItemText( hDlg, appProvEvt[uId], "UserConcept - GET" );
                  }
                  else
                  {
                     res = BOS_STATUS_ERR;
                  }
               }                  
            }
            break;

            case CFGITEM(PRV, SecConcept):
            {
               cfgAccessProvSecConceptGet((CFGSECTLSCFG **)pData);
            }
            break;

            case CFGITEM( SIP, MWIConcept ):
            {
               *((CFGSIPMWI **)pData) = &cfgMwi;   
            }
            break;

            case CFGITEM( SIP,  CIDConcept ):
            {
               *((CFGSIPCID **)pData) = &cfgCid;   
            }
            break;

            case CFGITEM(SIP,  CIDDisplayConcept):
            {
               *((CFGSIPCIDDISPLAY **)pData) = &cfgClidDsp;   
            }
            break;

            case CFGITEM( SIP,  AutoCallbackConcept ):
            {
               *((CFGSIPAUTOCALL **)pData) = &cfgAc;   
            }
            break;

            case CFGITEM( SIP,  AutoRecallConcept ):
            {
               *((CFGSIPAUTOCALL **)pData) = &cfgAr;   
            }
            break;

            case CFGITEM( SIP,  EServicesConcept ):
            {
               *((CFGSIPEMERGE **)pData) = &cfgEmergency;   
            }
            break;

            case CFGITEM( SIP,  CallFwdConcept ):
            {
               *((CFGSIPCALLFWD **)pData) = &cfgCfv;   
            }
            break;

            case CFGITEM( SIP,  DNDConcept ):
            {
               *((CFGSIPDND **)pData) = &cfgDnd;   
            }
            break;

            case CFGITEM( SIP,  ThreeWayCallConcept ):
            {
               *((CFGSIPTHREEWAYCALL **)pData) = &cfg3Way;   
            }
            break;

            case CFGITEM( SIP,  CallXfrConcept ):
            {
               *((CFGSIPCALLXFER **)pData) = &cfgXfer;   
            }
            break;

            case CFGITEM( SIP,  BasicCallConcept ):
            {
               *((CFGSIPBASICCALL **)pData) = &cfgBasic;   
            }
            break;

            case CFGITEM( SIP,  RemoteUserEntryConcept ):
            {
               BOS_UINT32 uId = cmgrFindRemoteUsrCfgBlk ( pCfgIx->pcCharIdx );
               if ( uId != CMGR_INVALID_HANDLE )
               {
                  *((CFGSIPREMOTEUSERCFG **)pData) = &cfgRemUsrCfg[uId];   
               }
               else
               {
                  res = BOS_STATUS_ERR;
               }
            }
            break;
            case CFGITEM( SIP,  RemoteUserSysConcept ):
            {
               *((CFGSIPREMOTEUSERSYSCFG **)pData) = &cfgRemUsrSysCfg;
            }
            break;
            case CFGITEM( SIP,  NoAnsConcept ):
            {
               *((CFGSIPNOANS **)pData) = &cfgNoAns;   
            }
            break;

            case CFGITEM( SIP,  CIDBlkConcept ):
            {
               *((CFGSIPCIDBLOCK **)pData) = &cfgCidBlk;   
            }
            break;

            case CFGITEM( SIP,  CIDDelConcept ):
            {
               *((CFGSIPCIDDEL **)pData) = &cfgCidDel;   
            }
            break;

            case CFGITEM( SIP,  CallHoldConcept ):
            {
               *((CFGSIPCALLHOLD **)pData) = &cfgHold;   
            }
            break;

            case CFGITEM( SIP,  COTConcept ):
            {
               *((CFGSIPCOT **)pData) = &cfgCot;   
            }
            break;

            case CFGITEM( SIP,  HotLineConcept ):
            {
               *((CFGSIPHOTLINE **)pData) = &cfgHotLine;   
            }
            break;

            case CFGITEM( SIP,  CallWaitConcept ):
            {
               *((CFGSIPCALLWAIT **)pData) = &cfgCWait;   
            }
            break;

            case CFGITEM( SIP, StatusChangeConcept ):
            {
               *((CFGSIPACTSTAT **)pData) = &cfgActStat;
            }
            break;

            case CFGITEM( SIP, HeldMediaConcept ):
            {
               *((CFGSIPHELDMEDIA **)pData) = &cfgHeld;
            }
            break;

            /* Cannot query configuration item at this time, or item invalid.
            */
            default:
            {
               res = BOS_STATUS_ERR;
            }
            break;
         }
      }
      break;

      case CFG_ITEM_DEL:
      {
         /* Nothing to be done since all configuration data has been
         ** statically allocated.
         */
         switch ( cfgItem )
         {
			   case CFGITEM(PRV, SecConcept):
		      {
			      return cfgAccessProvSecConceptDel((CFGSECTLSCFG *)pData);
		      }
		      break;  
            default:
            {
            }
            break;
         }
      }
      break;

      case CFG_ITEM_SET:
      {
         if ( (cfgItem == CFGITEM(SIP,  UserConcept)) &&
              (pData && ((CFGSIPUSERCFG *)pData)->pcActiveDomain) )
         {
            BOS_UINT32 uId;

            if ( (uId = cmgrFindUsrCfgBlk ( ((CFGSIPUSERCFG *)pData)->pcUserId )) !=
                  CMGR_INVALID_HANDLE )
            {
               /* use a temp buffer for copy.
               ** Since CFG_ITEM_GET obtains just the pointer uCfgActiveDomain,
               ** clear uCfgActiveDomain before copying, will also clear
               ** ((CFGSIPUSERCFG *)pData)->pcActiveDomain value.
               */
               char temp[256];
               strcpy( temp,
                      (const char *) ((CFGSIPUSERCFG *)pData)->pcActiveDomain );
               memset ( uCfgActiveDomain[uId], 0, MAX_STR ); 
               strcpy ( (char *) uCfgActiveDomain[uId],
                        (const char *) temp );

               Log( "cmgrProvCb: Setting active domain (%s) for user %s (0x%d)",
                    ((CFGSIPUSERCFG *)pData)->pcActiveDomain,
                    ((CFGSIPUSERCFG *)pData)->pcUserId, uId );

               SetDlgItemText( hDlg, appProvEvt[uId], "UserConcept - SET" );
            }
         }
         else if ( (cfgItem == CFGITEM(SIP,  RemoteUserEntryConcept)) && pData )
         {
            /* check if we have an existing entry using Index */
            BOS_UINT32 uId = CMGR_INVALID_HANDLE;
            if ( pCfgIx )
            {
               uId = cmgrFindRemoteUsrCfgBlk ( pCfgIx->pcCharIdx );
            }
            /* check if we have an existing entry with the same user id */
            if (  uId == CMGR_INVALID_HANDLE && pData )
            {
               uId = cmgrFindRemoteUsrCfgBlk ( ((CFGSIPREMOTEUSERCFG *)pData)->pcUserId );
            }
            /* no entry found */
            if ( uId == CMGR_INVALID_HANDLE )
            {
               /* Find the next free index */
               for ( uId = 0 ; uId < CMGRCFG_USER_MAX ; uId++ )
               {
                  if ( cfgRemUsrCfg[ uId ].pcUserId == NULL )
                  {
                     break;
                  }
               }
            }

            if ( uId == CMGR_INVALID_HANDLE )
            {
               res = BOS_STATUS_ERR;
            }
            else
            {
               cfgRemUsrCfg[ uId ].pcUserId = reinterpret_cast< BOS_UINT8* >( RemoteUser[uId] );
               memset ( (void *) cfgRemUsrCfg[uId].pcUserId, 0, MAX_STR );
               strcpy ( (char *) cfgRemUsrCfg[uId].pcUserId,
                        (const char *) ((CFGSIPREMOTEUSERCFG *)pData)->pcUserId );
               memset ( (void *) cfgRemUsrCfg[uId].pcAddr, 0, MAX_STR );
               strcpy ( (char *) cfgRemUsrCfg[uId].pcAddr,
                        (const char *) ((CFGSIPREMOTEUSERCFG *)pData)->pcAddr );
               cfgRemUsrCfg[uId].uSipPort = ((CFGSIPREMOTEUSERCFG *)pData)->uSipPort;
               Log( "cmgrProvCb: Setting remote user configuration (%s:%u) for user %s (0x%d)",
                        cfgRemUsrCfg[uId].pcAddr, cfgRemUsrCfg[uId].uSipPort, 
                        cfgRemUsrCfg[uId].pcUserId, uId );
            }
         }
      }
      break;

      case CFG_ITEM_GET_CB:
      default:
      break;
   }

   return res;
}


/****************************************************************************
** FUNCTION:   cmgrSesCtlCb
**
** PURPOSE:    Callback functionality for session control outside of
**             the Call Manager.
**
** PARAMETERS: eCmd  - the session command to apply.
**             uPhy  - the physical interface associated with the session.
**             uHdl  - the handle for which this command applies.
**             pData - additional data (may be NULL) necessary to complete
**                     the action.
**
** RETURNS:    The reference index for the session of interest as allocated
**             by the outside world.
**
** NOTE:
*****************************************************************************/
BOS_UINT32 cmgrSesCtlCb( CMGRSESCMD eCmd,
                         BOS_UINT32 uPhy,
                         BOS_UINT32 uHdl,
                         void *pData )
{
   BOS_UINT32 uRet = CMGR_INVALID_HANDLE;   

   (void) pData;

   SetDlgItemText( hDlg,
                   appSesEvt[uPhy],
                   callClientMapGetStr( ccSesEvtMap, eCmd )
                   );

   switch ( eCmd )
   {
      case eCMGRSESCMD_CREATE:
      {
         BOS_UINT32 uIx;
         /*Find and allocate and empty connection block if available.
         */
         for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX ; uIx++ )
         {
            if ( !CnxInUse[ uIx ].bInUse && (uRet == CMGR_INVALID_HANDLE) )
            {
               CnxInUse[ uIx ].bInUse = BOS_TRUE;
               CnxInUse[ uIx ].bFocus = BOS_TRUE;
               CnxInUse[ uIx ].uLine = uPhy;
               uRet = uIx;
               break;
            }
         }

         if ( uRet == CMGR_INVALID_HANDLE )
         {
            Log( "cmgrSesCtlCb: Failed to create cnx for %d", uHdl );
         }
         else
         {
            Log( "cmgrSesCtlCb: CREATE CONNECTION %d FOR %d",
                 uRet, uPhy );
         }
      }
      break;

      case eCMGRSESCMD_UPDATE:
      {
         Log( "cmgrSesCtlCb: UPDATE CONNECTION %d ON %d",
              uHdl, uPhy );
      }
      break;

      case eCMGRSESCMD_TERMINATE:
      {
         CnxInUse[ uHdl ].bInUse = BOS_FALSE;
         CnxInUse[ uHdl ].bFocus = BOS_FALSE;
         /* Clear LCD status line */
         appDisplayClear( uPhy );
         appDisplay( uPhy, 1, 15, User[uPhy]);
         Log( "cmgrSesCtlCb: TERMINATE CONNECTION %d ON %d",
              uHdl, uPhy );
      }
      break;

      default:
      break;
   }

   return uRet;   
}


/****************************************************************************
** FUNCTION:   cmgrFindUsrCfgBlk
**
** PURPOSE:    Finds a user configuration block based on the user name.
**
** PARAMETERS: pUsr - the user name to lookup.
**
** RETURNS:    The index of the corresponding user configuration block for
**             this user, or CMGR_INVALID_HANDLE if none located.
**
** NOTE:
*****************************************************************************/
BOS_UINT32 cmgrFindUsrCfgBlk ( BOS_UINT8 *pUsr )
{
   BOS_UINT32 uRes = CMGR_INVALID_HANDLE;
   BOS_UINT32 uIdx;

   if ( pUsr )
   {
      for ( uIdx = 0 ; uIdx < CMGRCFG_USER_MAX ; uIdx++ )
      {
         if ( (uRes == CMGR_INVALID_HANDLE) &&
              (cfgUsr[ uIdx ].pcUserId) &&
              (!strcmp ( (char *)cfgUsr[ uIdx ].pcUserId, (char *)pUsr )) )
         {
            uRes = uIdx;
            break;
         }
      }
   }

   return uRes;
}


/****************************************************************************
** FUNCTION:   cmgrFindPcscfCfgBlk
**
** PURPOSE:    Finds a PCSCF configuration block based on the domain name.
**
** PARAMETERS: pDomain - the domain name to lookup.
**
** RETURNS:    The index of the corresponding PCSCF configuration block for
**             this user, or CMGR_INVALID_HANDLE if none located.
**
** NOTE:
*****************************************************************************/
BOS_UINT32 cmgrFindPcscfCfgBlk( BOS_UINT8 *pDomain )
{
   BOS_UINT32 uRes = CMGR_INVALID_HANDLE;
   BOS_UINT32 uIdx;

   if ( pDomain )
   {
      for ( uIdx = 0 ; uIdx < CMGRCFG_USER_MAX ; uIdx++ )
      {
         if ( (uRes == CMGR_INVALID_HANDLE) &&
              (cfgDomain[ uIdx ].pcDomain) &&
              (!strcmp ( (char *)cfgDomain[ uIdx ].pcDomain, (char *)pDomain )) )
         {
            uRes = uIdx;
            break;
         }
      }
   }

   return uRes;
}

/****************************************************************************
** FUNCTION:   cmgrFindRemoteUsrCfgBlk
**
** PURPOSE:    Finds a Remote User configuration block based on the domain name.
**
** PARAMETERS: pUsr - the user name to lookup.
**
** RETURNS:    The index of the corresponding remote user configuration block for
**             this user, or CMGR_INVALID_HANDLE if none located.
**
** NOTE:
*****************************************************************************/
BOS_UINT32 cmgrFindRemoteUsrCfgBlk( BOS_UINT8 *pUsr )
{
   BOS_UINT32 uRes = CMGR_INVALID_HANDLE;
   BOS_UINT32 uIdx;

   if ( pUsr )
   {
      for ( uIdx = 0 ; uIdx < CMGRCFG_USER_MAX ; uIdx++ )
      {
         if ( (uRes == CMGR_INVALID_HANDLE) &&
              (cfgRemUsrCfg[ uIdx ].pcUserId) &&
              (!strcmp ( (char *)cfgRemUsrCfg[ uIdx ].pcUserId, (char *)pUsr )) )
         {
            uRes = uIdx;
            break;
         }
      }
   }

   return uRes;
}

/****************************************************************************
** FUNCTION:   cmgrCfgInit
**
** PURPOSE:    Initializes the configuration information that the CMGR
**             application will query during run time.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************/
void cmgrCfgInit ( void )
{
   BOS_IP_ADDRESS bosIp;
   BOS_STATUS res;

   bosIpAddrCreateFromStr( LocalAddr[0], &bosIp );
   memcpy ( &cfgDevice.IpAddress, &bosIp, sizeof ( BOS_IP_ADDRESS ) );
   cfgDevice.pcFQDN = (LocalFqdn[0] != '\0') ? 
      reinterpret_cast< BOS_UINT8* >( &LocalFqdn[0] ) : NULL;
   cfgDevice.pMAC = EthernetAddress;
   cfgDevice.bDevEnabled = BOS_TRUE;
   cfgDevice.uSIPRecvPort = LocalSipPort;
   cfgDevice.bDisableKeepAlive = BOS_FALSE;
   cfgDevice.sSilenceOn = CFG_SILENCE_SUPP_NO_SDP;
   cfgDevice.pcDevSesIdent = SessionIdentifier;

   cfgCodec[ 0 ].bRTCPXRepEnable = BOS_FALSE;
   cfgCodec[ 0 ].bT38Enable = BOS_FALSE;
   cfgCodec[ 0 ].bV152Enable = BOS_FALSE;
   cfgCodec[ 0 ].sG711PktPeriod = 20;
   cfgCodec[ 0 ].sRTCPRate = 50;
   cfgCodec[ 0 ].pcPubReportAddr = (LocalAddr[0] != '\0') ? reinterpret_cast< BOS_UINT8* >( &LocalAddr[0] ) : NULL;
   cfgCodec[ 0 ].bDtmfRelayEnabled = BOS_TRUE;
   cfgCodec[ 0 ].uTelEvtPayloadNum = 0;

   cfgCodec[ 1 ].bRTCPXRepEnable = BOS_FALSE;
   cfgCodec[ 1 ].bT38Enable = BOS_TRUE;
   cfgCodec[ 1 ].bV152Enable = BOS_FALSE;
   cfgCodec[ 1 ].sG711PktPeriod = 20;
   cfgCodec[ 1 ].sRTCPRate = 50;
   cfgCodec[ 1 ].pcPubReportAddr = (LocalAddr[0] != '\0') ? reinterpret_cast< BOS_UINT8* >( &LocalAddr[0] ) : NULL;
   cfgCodec[ 1 ].bDtmfRelayEnabled = BOS_FALSE;
   cfgCodec[ 1 ].uTelEvtPayloadNum = 0;

   memset ( &cfgLine[ 0 ], 0, sizeof ( CFGEPTLINECFG ) ); 
   memset ( &cfgLine[ 1 ], 0, sizeof ( CFGEPTLINECFG ) ); 
   cfgLine[ 0 ].bFaxOnly = BOS_FALSE;
   cfgLine[ 0 ].uFaxWaitTime = 10000;

   cfgCodecLst.codecType = eCFG_CODECTYPE_AMRWB;
   cfgCodecLst.pNext = &cfgCodecLst2;
   cfgCodecLst2.codecType = eCFG_CODECTYPE_PCMU;
   cfgCodecLst2.pNext = &cfgCodecLst3;
   cfgCodecLst3.codecType = eCFG_CODECTYPE_G729E;
   cfgCodecLst3.pNext = &cfgCodecLst4;
   cfgCodecLst4.codecType = eCFG_CODECTYPE_PCMA;
   cfgCodecLst4.pNext = &cfgCodecLst5;
   cfgCodecLst5.codecType = eCFG_CODECTYPE_G723;
   cfgCodecLst5.pNext = &cfgCodecLst6;
   cfgCodecLst6.codecType = eCFG_CODECTYPE_G729;
   cfgCodecLst6.pNext = &cfgCodecLst7;
   cfgCodecLst7.codecType = eCFG_CODECTYPE_G722;
   cfgCodecLst7.pNext = NULL;

   cfgMwi.Act.bCfgStatus = BOS_FALSE;
   cfgMwi.uSubDuration = 30000;

   cfgCid.Act.bCfgStatus = BOS_TRUE;
#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_STANDALONE)
   cfgCid.sPPS = CFG_SIP_CID_PPS_PUBLIC;
#else
   cfgCid.sPPS = CFG_SIP_CID_PPS_PUBLIC;
#endif

   cfgClidDsp.Act.bCfgStatus = BOS_TRUE;
   cfgClidDsp.bCIDCWBlock = BOS_FALSE;
   cfgClidDsp.bCNAMDActStat = BOS_TRUE;
   cfgClidDsp.bCNDActStat = BOS_TRUE;
   cfgClidDsp.sTimeAdj = -1254; /* Testing. 20h, 54min */

   cfgAc.Act.bCfgStatus = BOS_TRUE;
   cfgAc.uTimer = 30000;
   cfgAc.uMaxSubRec = 2;
   cfgAc.uMaxSubSend = 2;

   cfgAr.Act.bCfgStatus = BOS_TRUE;
   cfgAr.uTimer = 30000;
   cfgAr.uMaxSubRec = 2;
   cfgAr.uMaxSubSend = 2;

   cfgEmergency.Act.bCfgStatus = BOS_TRUE;
   cfgEmergency.uMediaDSCPVal = 15;
   cfgEmergency.uNwHoldTimer = 40000;
   cfgEmergency.uHowlTimer = 30000;
   cfgEmergency.bEstab3WC = BOS_TRUE;
   cfgEmergency.bNoLocInfo = BOS_TRUE;

   cfgCfv.Act.bCfgStatus = BOS_TRUE;
   cfgCfv.pcAUID = ProfileAuid;

   cfgDnd.Act.bCfgStatus = BOS_TRUE;
   cfgDnd.pcAUID = ProfileAuid;

   cfg3Way.Act.bCfgStatus = BOS_TRUE;

   cfgXfer.Act.bCfgStatus = BOS_FALSE;
   cfgXfer.uNotifyTO = 30000;

   cfgNoAns.Act.bCfgStatus = BOS_TRUE;
   cfgNoAns.uTODuration = 180000;

   cfgCidBlk.Act.bCfgStatus = BOS_TRUE;
   cfgCidBlk.pcConfTone = NULL;
   cfgCidBlk.pcErrTone = NULL;

   cfgCidDel.Act.bCfgStatus = BOS_TRUE;
   cfgCidDel.pcConfTone = NULL;
   cfgCidDel.pcErrTone = NULL;

   cfgHold.Act.bCfgStatus = BOS_FALSE;

   cfgCot.Act.bCfgStatus = BOS_TRUE;

   cfgHotLine.Act.bCfgStatus = BOS_FALSE;
   cfgHotLine.pHotLineUri = reinterpret_cast< BOS_UINT8* >( HotLineUri );
   cfgHotLine.uOffHookTimer = 2000;

   cfgCWait.Act.bCfgStatus = BOS_TRUE;

   cfgHeld.Act.bCfgStatus = BOS_TRUE;
   cfgHeld.bHeldMediaEnabled = BOS_FALSE;

   cfgActStat.Act.bCfgStatus = BOS_TRUE;
   cfgActStat.uChgRegExp = static_cast< BOS_UINT32 >( RegTimer * 1000 );

   cfgBasic.Act.bCfgStatus = BOS_TRUE;
   cfgBasic.pPermSeqTone1 = reinterpret_cast< BOS_UINT8* >( Tone1 );
   cfgBasic.pPermSeqTone2 = reinterpret_cast< BOS_UINT8* >( Tone2 );
   cfgBasic.pPermSeqTone3 = reinterpret_cast< BOS_UINT8* >( Tone3 );
   cfgBasic.pTermOHErrSig = reinterpret_cast< BOS_UINT8* >( Tone1 );
   cfgBasic.uByeDelay = 500;
   cfgBasic.uOrigDTTimer = 30000;
   cfgBasic.uOrigLITimer = 16000;
   cfgBasic.uLORTimer = 0;
   cfgBasic.uPermSeqTimer1 = 0;
   cfgBasic.uPermSeqTimer2 = 0;
   cfgBasic.uPermSeqTimer3 = 0;
   cfgBasic.uTermErrSigTimer = 0;
   cfgBasic.uNEMDSCPValueSig = 0xFF;
   cfgBasic.uNEMDSCPValueMedia = 0xFF;
   cfgBasic.bMapRegEvtRej2Deact = BOS_TRUE;
   cfgBasic.bHfDisabled = BOS_TRUE;
   cfgBasic.bIgnoreHfNoFeat = BOS_TRUE;
   cfgBasic.bIssueNetDisc = BOS_TRUE;
   cfgBasic.uNetDiscDelay = 500;
   cfgBasic.bPresetMedSes = BOS_TRUE;

   cfgDomain[ 0 ].pcDomain = reinterpret_cast< BOS_UINT8* >( &RegistrarAddr[0] );
   cfgDomain[ 0 ].pNext = NULL;
   cfgDomain[ 1 ].pcDomain = reinterpret_cast< BOS_UINT8* >( &RegistrarAddr[0] );
   cfgDomain[ 1 ].pNext = NULL;

#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_IMS)
   res = bosIpAddrCreateFromStr( OBProxyAddr, &bosIp );
   cfgPcscf[ 0 ].uSIPPort = OBProxyPort;
   cfgPcscf[ 1 ].uSIPPort = OBProxyPort;
#else
   res = bosIpAddrCreateFromStr( RegistrarAddr, &bosIp );
   cfgPcscf[ 0 ].uSIPPort = RegistrarPort;
   cfgPcscf[ 1 ].uSIPPort = RegistrarPort;
#endif
   if ( res == BOS_STATUS_OK )
   {
      memcpy ( &cfgPcscf[ 0 ].PCSCFAddr, &bosIp, sizeof ( BOS_IP_ADDRESS ) );
      cfgPcscf[ 0 ].pcPCSCFDomain = NULL;

      memcpy ( &cfgPcscf[ 1 ].PCSCFAddr, &bosIp, sizeof ( BOS_IP_ADDRESS ) );
      cfgPcscf[ 1 ].pcPCSCFDomain = NULL;
   }
   else
   {
#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_IMS)
      cfgPcscf[ 0 ].pcPCSCFDomain = reinterpret_cast<BOS_UINT8 *> (OBProxyAddr);
#else
      cfgPcscf[ 0 ].pcPCSCFDomain = reinterpret_cast<BOS_UINT8 *> (RegistrarAddr);
#endif
   }
   cfgPcscf[ 0 ].pNext = NULL;
   cfgPcscf[ 1 ].pNext = NULL;

   cfgPcscf[ 0 ].uSigTrans = CFG_SIG_TRANS_UDP | CFG_SIG_TRANS_TCP;
#if CMGRCFG_TLS
   cfgPcscf[ 0 ].uSigTrans |= CFG_SIG_TRANS_TLS;
#endif
   cfgPcscf[ 0 ].bIgnoreTcpFallBack = BOS_TRUE;
   cfgPcscf[ 0 ].bBackupRecovery = BOS_TRUE;

   cfgEpt[ 0 ].sEndpntNum = 0;
   cfgEpt[ 0 ].bIn = BOS_TRUE;
   cfgEpt[ 0 ].bOut = BOS_TRUE;
   cfgEpt[ 0 ].pNext = NULL;
   cfgEpt[ 1 ].sEndpntNum = 1;
   cfgEpt[ 1 ].bIn = BOS_TRUE;
   cfgEpt[ 1 ].bOut = BOS_TRUE;
   cfgEpt[ 1 ].pNext = NULL;

   sprintf ( (char *) DomainVar, "@%s",
             (cfgPcscf[ 0 ].pcPCSCFDomain == NULL) ?
#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_IMS)
               OBProxyAddr :
#else
               RegistrarAddr:
#endif
                  (char *) cfgPcscf[ 0 ].pcPCSCFDomain );
   cfgDigMapExtVar.Act.bCfgStatus = BOS_TRUE;
   cfgDigMapExtVar.Act.pcAdminStatInfo = NULL;
   cfgDigMapExtVar.Act.pcOperStatInfo = NULL;
   cfgDigMapExtVar.pValue = DomainVar;

   cfgDigMap[ 0 ].Act.bCfgStatus = BOS_TRUE;
   cfgDigMap[ 0 ].Act.pcAdminStatInfo = NULL;
   cfgDigMap[ 0 ].Act.pcOperStatInfo = NULL;
   cfgDigMap[ 0 ].pABNF = reinterpret_cast< BOS_UINT8* >( DigitMapAbnf );
   cfgDigMap[ 1 ].Act.sAdminStatus = CFG_SIP_ADMIN_STATUS_ACTIVE;
   cfgDigMap[ 1 ].Act.sOperStatus = CFG_SIP_OPER_STATUS_ACTIVE;
   cfgDigMap[ 1 ].Act.pcAdminStatInfo = NULL;
   cfgDigMap[ 1 ].Act.pcOperStatInfo = NULL;
   cfgDigMap[ 1 ].pABNF = reinterpret_cast< BOS_UINT8* >( DigitMapAbnf );

   cfgUsr[ 0 ].Act.bCfgStatus = BOS_TRUE;
   cfgUsr[ 0 ].Act.pcAdminStatInfo = NULL;
   cfgUsr[ 0 ].Act.pcOperStatInfo = NULL;
   cfgUsr[ 0 ].sUserType = CFG_SIP_USER_TYPE_USERNAME;
#ifdef CCTKCFG_USE_CABLE_SIMULATOR
   cfgUsr[ 0 ].pcUserId = reinterpret_cast< BOS_UINT8* >( &PubIdent[0][0] );
   cfgUsr[ 0 ].pcDisplayInfo = NULL;
#else
   cfgUsr[ 0 ].pcUserId = reinterpret_cast< BOS_UINT8* >( &User[0][0] );
   cfgUsr[ 0 ].pcDisplayInfo = reinterpret_cast< BOS_UINT8* >( &User[0][0] );
#endif
   cfgUsr[ 0 ].pDomainList = &cfgDomain[0];
   cfgUsr[ 0 ].bSigSecurity = BOS_FALSE;
#ifdef CCTKCFG_USE_CABLE_SIMULATOR
   cfgUsr[ 0 ].pcPrivId = reinterpret_cast< BOS_UINT8* >( &PubIdent[0][0] );
#else
   cfgUsr[ 0 ].pcPrivId = reinterpret_cast< BOS_UINT8* >( &User[0][0] );
#endif
   cfgUsr[ 0 ].pEndPntMap = &cfgEpt[0];
   cfgUsr[ 0 ].credentials.CredType = CFG_SIP_USER_CRED_TYPE_PASSWORD;
   cfgUsr[ 0 ].credentials.pCredVal = reinterpret_cast< BOS_UINT8* >( &Password[0] );
   cfgUsr[ 0 ].pcActiveDomain = reinterpret_cast< BOS_UINT8* >( &uCfgActiveDomain[0][0] );
   cfgUsr[ 0 ].uInitRegDelay = 0;
   cfgUsr[ 0 ].uRegRetryTimerMin = 10;
   cfgUsr[ 0 ].uRegRetryTimerMax = 100;
   cfgUsr[ 0 ].pNext = NULL;

   cfgUsr[ 1 ].Act.bCfgStatus = BOS_TRUE;
   cfgUsr[ 1 ].Act.pcAdminStatInfo = NULL;
   cfgUsr[ 1 ].Act.pcOperStatInfo = NULL;
   cfgUsr[ 1 ].pcUserId = reinterpret_cast< BOS_UINT8* >( &User[1][0] );
   cfgUsr[ 1 ].sUserType = CFG_SIP_USER_TYPE_USERNAME;
   cfgUsr[ 1 ].pcDisplayInfo = reinterpret_cast< BOS_UINT8* >( &User[1][0] );
   cfgUsr[ 1 ].pDomainList = &cfgDomain[1];
   cfgUsr[ 1 ].bSigSecurity = BOS_FALSE;
   cfgUsr[ 1 ].pcPrivId = reinterpret_cast< BOS_UINT8* >( &User[1][0] );
   cfgUsr[ 1 ].pEndPntMap = &cfgEpt[1];
   cfgUsr[ 1 ].credentials.CredType = CFG_SIP_USER_CRED_TYPE_PASSWORD;
   cfgUsr[ 1 ].credentials.pCredVal = reinterpret_cast< BOS_UINT8* >( &Password[0] );
   cfgUsr[ 1 ].pcActiveDomain = reinterpret_cast< BOS_UINT8* >( &uCfgActiveDomain[1][0] );
   cfgUsr[ 1 ].pNext = NULL;
   
   {
      BOS_UINT32 uIdx;
      for ( uIdx = 0 ; uIdx < CMGRCFG_USER_MAX ; uIdx++ )
      {
         cfgRemUsrCfg[uIdx].pcUserId = NULL;
         cfgRemUsrCfg[uIdx].pcAddr = reinterpret_cast< BOS_UINT8* >( uCfgRemCredAddr[uIdx] );
      }
      cfgRemUsrSysCfg.bEnable = BOS_TRUE;
      cfgRemUsrSysCfg.sAdmissionMode = CFG_SIP_REMOTE_USER_ADMISSION_ALL;
      cfgRemUsrSysCfg.uDftAssociatedIMPUIndexRef = 0;
      cfgRemUsrSysCfg.pcDftAssociatedUserId = NULL;
      cfgRemUsrSysCfg.uDftIMPIIndexRef = 0;
      cfgRemUsrSysCfg.pcDftIMPIUserUserId = NULL;
      cfgRemUsrSysCfg.dftCredentials.CredType = CFG_SIP_USER_CRED_TYPE_NONE;
      cfgRemUsrSysCfg.dftCredentials.pCredVal = NULL;
   }
}


#ifdef __cplusplus
extern "C"
{
#endif
/****************************************************************************
** FUNCTION:   cctkTraceHandler
**
** PURPOSE:    Trace handler for CCTK and SIP stack traces.
**
** PARAMETERS: uLevel - the trace level of interest.
**             uGroup - the trace group generating this trace.
**             pcTraceStr - the formatted trace (string), NULL terminated
**
** RETURNS:    Nothing.
**
** NOTE:       Filtering on registered trace level and group per the
**             configuration information has already happened at this time.
*****************************************************************************/
void cctkTraceHandler( BOS_UINT16 uLevel,
                       BOS_UINT16 uGroup,
                       const BOS_UINT8 *pcTraceStr )
{
   Log( "Lvl %u Grp %u [%s]", 
        uLevel, 
        uGroup, 
        reinterpret_cast< const char* >( pcTraceStr ) );
}


/****************************************************************************
** FUNCTION:   cmgrLogHandler
**
** PURPOSE:    Trace handler for CMGR traces.
**
** PARAMETERS: fmt - format string.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************/
void cmgrLogHandler ( char *fmt, ... )
{
   va_list arglist;
   SYSTEMTIME time;
   char msg[MAXSTR];
   char *buf = (char *)malloc(1500);

   va_start(arglist, fmt);
   vsprintf(msg, fmt, arglist);
   GetLocalTime(&time);
   sprintf(buf, "%02d:%02d:%02d.%d  %s",
      time.wHour, time.wMinute, time.wSecond, (time.wMilliseconds+50)%1000/100, msg);

   PostMessage(hWnd, WMUSR_LOG, 0, (LPARAM) buf);
}

#ifdef __cplusplus
}
#endif

/*****************************************************************************
** cfgAccessSec functions -- copy from cfgAccessSec.c
*****************************************************************************/
#define CXC_SIG_TLS 1
#define USE_INTERNAL_TEST_CERTS

#define MAX_CERTIFICATE_CHAIN_LEN  4
#define MAX_PRIVATE_KEY_PEM_LEN    2048
#define MAX_SIP_TLS_CIPHER_LEN     16
#define DEFAULT_SIP_TLS_CIPHER     "aRSA"

/* we have no logApi */
#include <assert.h>
#define LOG_CRIT(a)   assert(NULL)

#if CXC_SIG_TLS
#ifdef USE_INTERNAL_TEST_CERTS
/* certificate chain MTA Real with 3 certificates */
static char MTARootCert[] =
"-----BEGIN CERTIFICATE-----\n"
"MIIDzjCCAragAwIBAgIQQxyiy1KqRx3vZu9Frms3+zANBgkqhkiG9w0BAQUFADBv\n"
"MQswCQYDVQQGEwJVUzESMBAGA1UEChMJQ2FibGVMYWJzMRQwEgYDVQQLEwtQYWNr\n"
"ZXRDYWJsZTE2MDQGA1UEAxMtUGFja2V0Q2FibGUgUm9vdCBEZXZpY2UgQ2VydGlm\n"
"aWNhdGUgQXV0aG9yaXR5MB4XDTAxMDIwMTAwMDAwMFoXDTMxMDEzMTIzNTk1OVow\n"
"bzELMAkGA1UEBhMCVVMxEjAQBgNVBAoTCUNhYmxlTGFiczEUMBIGA1UECxMLUGFj\n"
"a2V0Q2FibGUxNjA0BgNVBAMTLVBhY2tldENhYmxlIFJvb3QgRGV2aWNlIENlcnRp\n"
"ZmljYXRlIEF1dGhvcml0eTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n"
"ALiJLS9OlkPGZGXPyUeVWoXINAYoOvLi7FtyMTqOyJNaK8yQVrWwMs6NS037oRna\n"
"aoTqEdPwNOD66ZW2r1WuWQ54nEoJdQaZS7YKHWhG/WL3iLo8nc+ojVIn6vkGwfpa\n"
"Emmy5o/pZV5U0Ro4UwPDT99xzC5KoVqw4UpQCgaQjL9hPbJVw8yLPgktQiGouCX7\n"
"CBzRXGFuWuaSqTkyCaXtw/l1HuequfMVewSX9UFpe3sYAEDOhoRc9WZFQqdqtVHn\n"
"u98hgKqroaC0gW92o9aro8HUnoT/qFkj4SXmUeaAqhN7hSDqz6TKnaeQeDLDXno6\n"
"4f7pRfQe3n6kX0jXkOMseL0CAwEAAaNmMGQwEgYDVR0TAQH/BAgwBgEB/wIBATAO\n"
"BgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYEFJGUIH7VcbmwAGFJUb04XG6m7WB8MB8G\n"
"A1UdIwQYMBaAFJGUIH7VcbmwAGFJUb04XG6m7WB8MA0GCSqGSIb3DQEBBQUAA4IB\n"
"AQCJ/8eiebrZXN5a5aNN2cExJ69nCrCkXIhkzaMdWx2zw84yjyBjtv4+U0a7lTgW\n"
"mJzFP6A1FJ95OheAq9qr8BBcwIK5trIAuw7ynSj4rfexKNrM3pm4bTccbd3i1Eim\n"
"eh/xb0F1xd6B6aAOiiQjx+kjtNLBnt1IjQEA0aUziOVJEMff5iAgsCsAlGE2/fHp\n"
"FTQTAHnj8+nJLOUj7Q6amLqvGrCs0MQXFTcVqk4+Cwtsm3zFohXytc0r7gz8Vo9G\n"
"sh2oBOidsfQqhWLk4xxoZycqza/KmNt+CrDiMKok4sX4jp7hHxdSfF056Hwz/qvO\n"
"LKdCLEGwGOKhwnragSaHTNxX\n"
"-----END CERTIFICATE-----\n";

static char MTAManCert[] =
"-----BEGIN CERTIFICATE-----\n"
"MIIEGDCCAwCgAwIBAgIQP1f1YvFIcrX6xT8a+iLqrzANBgkqhkiG9w0BAQUFADBv\n"
"MQswCQYDVQQGEwJVUzESMBAGA1UEChMJQ2FibGVMYWJzMRQwEgYDVQQLEwtQYWNr\n"
"ZXRDYWJsZTE2MDQGA1UEAxMtUGFja2V0Q2FibGUgUm9vdCBEZXZpY2UgQ2VydGlm\n"
"aWNhdGUgQXV0aG9yaXR5MB4XDTAyMDMxMzAwMDAwMFoXDTIyMDMxMjIzNTk1OVow\n"
"gbgxCzAJBgNVBAYTAkNBMR0wGwYDVQQKExRCcm9hZGNvbSBDb3Jwb3JhdGlvbjEZ\n"
"MBcGA1UECBMQQnJpdGlzaCBDb2x1bWJpYTERMA8GA1UEBxMIUmljaG1vbmQxFDAS\n"
"BgNVBAsTC1BhY2tldENhYmxlMRgwFgYDVQQLEw9SaWNobW9uZCBPZmZpY2UxLDAq\n"
"BgNVBAMTI0Jyb2FkY29tIENhbmFkYSBMdGQuIFBhY2tldENhYmxlIENBMIIBIjAN\n"
"BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAtYVtwQa6g12aQv9k0g5AkkToqiDE\n"
"NTDFSgl5pSs/PNZFaWEeCtPz8NTBBwyr5WDq5g9kCO1+mgS+XLuRheRFmAD+JnOh\n"
"W34/QOGmsK8wSESiHxCuyTwCeLNlJLa6cqW/nIlOfnPcwjzaXNY6DqGw2R/j0q9v\n"
"NxfcK2EQ+8heMcBjRab1KemxaqFIUc9ni3GRJK+YBNG1IcipDlYB1SwyM8u/GGnH\n"
"xLkRNkC02EbxLEAnDKLlkLPRqxzsbNp0dOZxTimNnh6xut1M/pRJogOqKK/tDsE2\n"
"ioqxV3uZil2h9utM2rlCiRr+ct/xrAxRcRxa9RtHykOq9DtfkEjdEpar8QIDAQAB\n"
"o2YwZDASBgNVHRMBAf8ECDAGAQH/AgEAMA4GA1UdDwEB/wQEAwIBBjAdBgNVHQ4E\n"
"FgQUbG0uIdfaMgxZ6YexkebAZjJmVREwHwYDVR0jBBgwFoAUkZQgftVxubAAYUlR\n"
"vThcbqbtYHwwDQYJKoZIhvcNAQEFBQADggEBAK4ON1i4gsC+7o2W4BUnJJyWK5sr\n"
"rbdUh1/Z6xrEM7eMwyUrLZsGD1oId8I5MqHhU8HX2qpxFcUr8FG1ZxPdZalIkIoy\n"
"aZzuN2J0ZLglMaGaxSEpic8YOZ2HrSsvEssy+Rm/Fmv6zqJwMXlOrsUoU8L+Eeb8\n"
"onwPyhm737/uM9Bv1iWh5toicS/V2DWU2yu0idWHwpvqoQx425Y8sKQq7aDPa6UA\n"
"evCNYeaSIk8ggltVv0ex6BtxRff8NLJreEHoJZcx52oeIkiBXcRcTJyEDa/rZSKP\n"
"ccfxwUpHfvLycVOlx8WU/Og5/mA6Ps1dpe2xezVGTyPxjVwETBmbqEIr/z8=\n"
"-----END CERTIFICATE-----\n";

static char MTADevCert[] =
"-----BEGIN CERTIFICATE-----\n"
"MIIDgTCCAmmgAwIBAgIFEBhoAiEwDQYJKoZIhvcNAQEFBQAwgbgxCzAJBgNVBAYT\n"
"AkNBMR0wGwYDVQQKExRCcm9hZGNvbSBDb3Jwb3JhdGlvbjEZMBcGA1UECBMQQnJp\n"
"dGlzaCBDb2x1bWJpYTERMA8GA1UEBxMIUmljaG1vbmQxFDASBgNVBAsTC1BhY2tl\n"
"dENhYmxlMRgwFgYDVQQLEw9SaWNobW9uZCBPZmZpY2UxLDAqBgNVBAMTI0Jyb2Fk\n"
"Y29tIENhbmFkYSBMdGQuIFBhY2tldENhYmxlIENBMB4XDTA1MDYyMzE4NTEyNVoX\n"
"DTI1MDYyMzE4NTEyNVowgZkxCzAJBgNVBAYTAkNBMREwDwYDVQQKEwhCcm9hZGNv\n"
"bTELMAkGA1UECBMCQkMxEjAQBgNVBAcTCVZhbmNvdXZlcjEUMBIGA1UECxMLUGFj\n"
"a2V0Q2FibGUxETAPBgNVBAsTCEJDTTkzMzQ4MREwDwYDVQQLEwhCcm9hZGNvbTEa\n"
"MBgGA1UEAxMRMDA6MTA6MTg6Njg6MDI6MjEwgZ8wDQYJKoZIhvcNAQEBBQADgY0A\n"
"MIGJAoGBAMcA2htGC9AWKjyj7YV4oKiXnRM7ZKdiBdXbOkaKrbSdfHtTdDzGTzsO\n"
"czqoIqrtSgVxVmkI2iZT+hGkY9y4DnMtkCVyLqFBXzewiyj+hN8mz4fzO+QBm2PA\n"
"MGGHi1vM8G46ZZ1kG0n9MN2NUDBIgacGRuYbRyAs+eBJN0HuZnfRAgMBAAGjMzAx\n"
"MA4GA1UdDwEB/wQEAwIFoDAfBgNVHSMEGDAWgBRsbS4h19oyDFnph7GR5sBmMmZV\n"
"ETANBgkqhkiG9w0BAQUFAAOCAQEAoBOW2KOnQTFPWuu+W36C53s3Apu1/KojxFes\n"
"5rKUxFxQlc3U/3ldr4a6W0CKI0zs9nbw6qsQI4yG7J56AgPdASoJNCAT53/YUaRo\n"
"idV4j6Ls5uZr96MwtOAD/cLQLCzbpfepcbbdtZWWdLvTUjss6sSFKSsf1CMtxz1j\n"
"fIKKBz5JbUITIqvT6Ik//UUQHGouodIvRWfY7Zy6vIu1q4u9EzdoEz9/PTK1gWlk\n"
"GCd265XRpriKxQOlIwMSKMNihK8kPzTHCW2lyvoYZT2Gk6Sgz6Onqb6sYQIBQ4jQ\n"
"oqEHKE0kfbPy/FLwwNcihlT6YagNvyR7j04aYmGelI6cHjWyIg==\n"
"-----END CERTIFICATE-----\n";

static char MTADevPrivKey[] =
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIICXAIBAAKBgQDHANobRgvQFio8o+2FeKCol50TO2SnYgXV2zpGiq20nXx7U3Q8\n"
"xk87DnM6qCKq7UoFcVZpCNomU/oRpGPcuA5zLZAlci6hQV83sIso/oTfJs+H8zvk\n"
"AZtjwDBhh4tbzPBuOmWdZBtJ/TDdjVAwSIGnBkbmG0cgLPngSTdB7mZ30QIDAQAB\n"
"AoGAL13NjDZU83eDICHolB9NJjPIKF63q3uSZf+jVE0H4Ximu3f2r8yH6UecN7bz\n"
"YXnPZ5I9fgykWAEfAXn+ZvfMQESCUi63PObfEb+hEmPesUeJ/xFttIuWyUJAOlkK\n"
"K9JcqKpJbVPaXGTxlWGmUuwq8RWWukQQJOs0NtihJN0W8MECQQD6/R0HprCvt9Yq\n"
"fIQ0r1HB8EstgQFHDEHlaB5bkhqxXIahl5xvTCVeu7TSSqHR6tZ3vkq0GVG6L6xS\n"
"dPrH5F77AkEA8dAUNz/ieehgU3vmxTtqpivnDNgt3cLnoGxp73L279ZwsmoiDc9W\n"
"iE4JaMtlsI26U5xreWz3KNKcqsjxDcPZUQJAQ4SfsJU45qxPorJIZlvbZI6dyqnv\n"
"jAtSFsKBmCSfpaK8XPAKFy6mxus7425MRcok06WPNw+wmqOEedIaPHfEzQJBAOPO\n"
"gM0QDV+F0QG1T1HuSOnloFUUcBlkXexfVmmz5/lJD4C1xgE2nLHquqiUPefXjXrp\n"
"eMK7Khm2WMv4ZEfwgxECQFKC1QGrHiOnw6N0rRaRT8AYEkKJBmW3oDwk+m3NFMmg\n"
"H8V9M61wrXi1/4Tw0jigLEWv2iLoc+fXPZCiIfVenDY=\n"
"-----END RSA PRIVATE KEY-----\n";

/* NA (CableLab) Test Tel Root Cert */
static char TelRootCert[] =
"-----BEGIN CERTIFICATE-----\n"
"MIIDszCCApugAwIBAgIBAjANBgkqhkiG9w0BAQUFADBOMQswCQYDVQQGEwJVUzES\n"
"MBAGA1UEChMJQ2FibGVMYWJzMSswKQYDVQQDEyJDYWJsZUxhYnMgU2VydmljZSBQ\n"
"cm92aWRlciBSb290IENBMB4XDTAzMDIxODE5MjYyOVoXDTMzMDIxODE5MjYyOVow\n"
"TjELMAkGA1UEBhMCVVMxEjAQBgNVBAoTCUNhYmxlTGFiczErMCkGA1UEAxMiQ2Fi\n"
"bGVMYWJzIFNlcnZpY2UgUHJvdmlkZXIgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEB\n"
"BQADggEPADCCAQoCggEBAKHjUbVsRoJ+Zrhf8vPy11YrPCtkpKsIzjx2lLbLhKDH\n"
"LwJp66vJ4priWr1S/XCpjU751vDGy8p4S4/lnv4j0CMjRuI7buKxMJg4J6kTzmgi\n"
"6he6qMopMxJbI/Vvn1FgCnC8Qm3G1XOLeoErw3RXOHlhvLuQKf5o1cIETP+OR9mZ\n"
"Nihbul6PC4OWbbTj7wr1NFE3Zgp2xWoTQgL8QR8A+FfpeqH/+tNTtaIjFc6HbkGk\n"
"IkK+CTFeiyoelxWhFSocpuUztbEp25oSV+raZjX+ydNUdYzn8XuzpWluDweWsbvU\n"
"yULqA18L1xOfM8AIX9OJZBiLcmH5eMwXBUb1whTD4PsCAwEAAaOBmzCBmDAOBgNV\n"
"HQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUbHCWB4twJK+A\n"
"X+QyXT+aKfQ8fEkwVgYDVR0RBE8wTaRLMEkxRzBFBgNVBAMTPkNBQkxFTEFCUyBH\n"
"RU5FUkFURUQgVEVTVCBST09UIEZPUiBFUVVJUE1FTlQgVEVTVCBQVVJQT1NFUyBP\n"
"TkxZMA0GCSqGSIb3DQEBBQUAA4IBAQB4Y4sgG7S4lR64gc7mrqplr8noI7U4lORZ\n"
"qfTEP8bM+EDYauFFFSXLJ7uAOBbEd1h/KJXB819kOMa+Uzm40zsw5C+0O3DO0jt6\n"
"kxRYaQQjHO7m6f4KcpKnb0WZ1t5+Kfi9vogUuUedGAV9ayvB2HQpVnlNsgTK9HI1\n"
"v2C/R9gzwrlmarvS8ZalIaFnh+LJ1vS9WLIFiOBmULHPpoviJG8oTppcr+b20olR\n"
"pgbCdndk/mUHmbuHZqTKtkKD4N88bVIzSFerFiusDTSJlAtdiby/E5edLANprkD/\n"
"KGKLT7n+tg5gNEax6i7DEJngQKxwQ1xjODPaB7HsVe1c2dlYLluu\n"
"-----END CERTIFICATE-----\n";
#endif /* certificate chain MTA Real with 3 certificates */
#endif
/*
*****************************************************************************
**
** FUNCTION:   cfgAccessProvSecConceptDel
**
** PURPOSE:    Free the memory associated with this instance of
**             CFGSECTLSCFG data
**
** PARAMETERS:  pValue - pointer to the instance
**
**
** RETURNS:    BOS_STATUS_OK if successful.
**
*****************************************************************************
*/

BOS_STATUS cfgAccessProvSecConceptDel (CFGSECTLSCFG *pValue)
{
   int i = 0;
   if (pValue == NULL)
   {
      return BOS_STATUS_ERR;
   }

   /*free any pointers within the structure first*/
   if (pValue->ppcCertChain)
   {
      for (i = 0; pValue->ppcCertChain[i] != NULL; i++)
      {
         free(pValue->ppcCertChain[i]);
      }
      free(pValue->ppcCertChain);
   }

   if (pValue->ppcTrustedCerts)
   {
      for (i = 0; pValue->ppcTrustedCerts[i] != NULL; i++)
      {
         free(pValue->ppcTrustedCerts[i]);
      }
      free(pValue->ppcTrustedCerts);
   }

   if (pValue->pcPrivKey) {
      free(pValue->pcPrivKey);
   }
   
   if (pValue->pcCipher)
   {
      free(pValue->pcCipher);
   }
   
   if (pValue->preGenDhPrime.puPrime)
   {
      free(pValue->preGenDhPrime.puPrime);
   }

   /*free the rest*/
   free(pValue);

   return BOS_STATUS_OK;
}

/*
*****************************************************************************
**
** FUNCTION:   cfgAccessProvSecConceptGet
**
** PURPOSE:    GET security TLS data
**
** PARAMETERS:  pEntry - pointer to hold the data
**
**
** RETURNS:   BOS_STATUS_OK if successful.
**
*****************************************************************************
*/

BOS_STATUS cfgAccessProvSecConceptGet (CFGSECTLSCFG **ppEntry)
{
#if CXC_SIG_TLS
#ifndef USE_INTERNAL_TEST_CERTS
   BOS_STATUS status;
#endif
#endif
   CFGSECTLSCFG *pInfo;
   pInfo = (CFGSECTLSCFG *) malloc (sizeof(CFGSECTLSCFG));
   if (pInfo == NULL)
   {
      return BOS_STATUS_ERR;
   }
   /* clear the newly allocated memory */
   memset(pInfo, 0, sizeof(CFGSECTLSCFG));

#if CXC_SIG_TLS
   /* allocate memory to hold the certs chains */
   /* local certificate chain should be 4-certificates chain, + 1 for NULL termination. */
   /* trusted certicates are TelRoot and EmtaRoot */
   pInfo->ppcCertChain = (BOS_UINT8 **) calloc (MAX_CERTIFICATE_CHAIN_LEN + 1, sizeof(BOS_UINT8*));
   pInfo->ppcTrustedCerts = (BOS_UINT8 **) calloc (MAX_CERTIFICATE_CHAIN_LEN + 1, sizeof(BOS_UINT8*));   
   pInfo->pcPrivKey = (BOS_UINT8 *) malloc (MAX_PRIVATE_KEY_PEM_LEN);  
   pInfo->pcCipher = (BOS_UINT8 *) malloc (MAX_SIP_TLS_CIPHER_LEN);

   if (pInfo->ppcCertChain == NULL || pInfo->ppcTrustedCerts == NULL || pInfo->pcPrivKey == NULL || 
       pInfo->pcCipher == NULL)
   {
      LOG_CRIT((LOG_MOD_SIP,"cfgAccessProvSecConceptGet- Unable to malloc "));
      return BOS_STATUS_ERR;
   }
   memset(pInfo->ppcCertChain, 0, sizeof(BOS_UINT8*) * (MAX_CERTIFICATE_CHAIN_LEN + 1));
   memset(pInfo->ppcTrustedCerts, 0, sizeof(BOS_UINT8*) * (MAX_CERTIFICATE_CHAIN_LEN + 1));

   /* Get Certificates Chains */
#ifdef USE_INTERNAL_TEST_CERTS
   pInfo->ppcTrustedCerts[0] = (BOS_UINT8 *) malloc (sizeof(MTARootCert));
   pInfo->ppcTrustedCerts[1] = (BOS_UINT8 *) malloc (sizeof(TelRootCert));
   pInfo->ppcTrustedCerts[2] = NULL;
   pInfo->ppcCertChain[0] = (BOS_UINT8 *) malloc (sizeof(MTARootCert));
   pInfo->ppcCertChain[1] = (BOS_UINT8 *) malloc (sizeof(MTAManCert));
   pInfo->ppcCertChain[2] = (BOS_UINT8 *) malloc (sizeof(MTADevCert));
   pInfo->ppcCertChain[3] = NULL;
   
   if (pInfo->ppcCertChain[0] == NULL || pInfo->ppcCertChain[1] == NULL || pInfo->ppcCertChain[2] == NULL ||
       pInfo->ppcTrustedCerts[0] == NULL || pInfo->ppcTrustedCerts[1] == NULL)
   {
      LOG_CRIT((LOG_MOD_SIP,"cfgAccessProvSecConceptGet- Unable to malloc "));
      return BOS_STATUS_ERR;
   } 
   memcpy(pInfo->ppcTrustedCerts[0], MTARootCert, sizeof(MTARootCert));
   memcpy(pInfo->ppcTrustedCerts[1], TelRootCert, sizeof(TelRootCert));
   memcpy(pInfo->ppcCertChain[0], MTARootCert, sizeof(MTARootCert));
   memcpy(pInfo->ppcCertChain[1], MTAManCert, sizeof(MTAManCert));
   memcpy(pInfo->ppcCertChain[2], MTADevCert, sizeof(MTADevCert));
   memcpy(pInfo->pcPrivKey, MTADevPrivKey, sizeof(MTADevPrivKey));
#else   
   status = sipSecCertsRetrieve(pInfo->ppcTrustedCerts, pInfo->ppcCertChain, pInfo->pcPrivKey);
   if ( status != BOST_STATUS_OK)
   {
      LOG_CRIT((LOG_MOD_SIP,"cfgAccessProvSecConceptGet() - Failed to get TLS certificates."));
   }
#endif   
   /* Set cipher to our default string */
   strncpy((char *)pInfo->pcCipher, DEFAULT_SIP_TLS_CIPHER, MAX_SIP_TLS_CIPHER_LEN);
   pInfo->bClientAuth = BOS_TRUE;
   pInfo->bServerAuth = BOS_TRUE;
   pInfo->preGenDhPrime.puPrime = NULL; /* at this time, we always create the puPrime in real time, so we are not malloc a Prime buffer */
   pInfo->preGenDhPrime.uLen = 0;
#else
   /* an empty default CFGSECTLSCFG */
   pInfo->ppcCertChain = NULL; 
   pInfo->ppcTrustedCerts = NULL;
   pInfo->pcPrivKey = NULL;
   pInfo->pcCipher = NULL;
   pInfo->bClientAuth = BOS_FALSE;
   pInfo->bServerAuth = BOS_FALSE;
   pInfo->preGenDhPrime.puPrime = NULL;
   pInfo->preGenDhPrime.uLen = 0;
#endif  
   *ppEntry = pInfo;
   return BOS_STATUS_OK;
}


/*
*****************************************************************************
**
** FUNCTION:   cfgAccessProvSecConceptSet
**
** PURPOSE:    SET security TLS data
**
** PARAMETERS:  pEntry - pointer holding the data
**
**
** RETURNS:   BOS_STATUS_OK if successful.
**
*****************************************************************************
*/

BOS_STATUS cfgAccessProvSecConceptSet (CFGSECTLSCFG *pEntry)
{
   /* Nothing to do for now */
   (void)(pEntry);
   return BOS_STATUS_OK;
}

/*
*****************************************************************************
**
** FUNCTION:   setCctkTrace
**
** PURPOSE:    Set Tracing level and Group Filtering
**
** PARAMETERS:  none
**
** RETURNS:   none
**
*****************************************************************************
*/
void setCctkTrace( void )
{
   CCTKTRACE traceParms;
   BOS_UINT16 uTraceLevel;
   BOS_UINT16 uTraceGroup;

   uTraceLevel = 
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_INFO ))? 
                                             CCTK_TRACELVL_INFO: 0) |
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_WARN ))? 
                                             CCTK_TRACELVL_WARNING: 0) |
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_DEBUG ))? 
                                             CCTK_TRACELVL_DEBUG: 0);

   uTraceGroup = 
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_GRP_CCTK ))? 
                                             CCTK_TRACEGRP_CCTK: 0) |
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_GRP_SCE ))? 
                                             CCTK_TRACEGRP_SCE: 0) |
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_GRP_TRANSPORT ))? 
                                             CCTK_TRACEGRP_TRANSPORT: 0) |
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_GRP_SDP ))? 
                                             CCTK_TRACEGRP_SDP: 0) |
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_GRP_SIP ))? 
                                             CCTK_TRACEGRP_SIP: 0) |
      (Button_GetCheck( GetDlgItem( hDlg, IDC_TRACE_GRP_MISC ))? 
                                             CCTK_TRACEGRP_MISC: 0);


      traceParms.pHandler = cctkTraceHandler;
      traceParms.uLevel = uTraceLevel;
      traceParms.uGroup = uTraceGroup;
      cctkSetParm( CCTK_INVALID_HANDLE, eCCTKPARM_EC_TRACE, (void *)&traceParms );
}

/*
*****************************************************************************
**
** FUNCTION:   initMwiLed
**
** PURPOSE:    initialize MWI LED handles
**
** PARAMETERS:  none
**
** RETURNS:   none
**
*****************************************************************************
*/
void initMwiLed( void )
{
   // Get handles to MWI LEDs
   gMWI_LED_ON_hdl = SendMessage( GetDlgItem( hDlg, IDC_MWI_LED ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gMWI_LED_OFF_hdl = SendMessage( GetDlgItem( hDlg, IDC_MWI_LED_OFF ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gMWI_LED_ON3_hdl = SendMessage( GetDlgItem( hDlg, IDC_MWI_LED3 ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gMWI_LED_OFF3_hdl = SendMessage( GetDlgItem( hDlg, IDC_MWI_LED_OFF3 ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   setMwiLed( 0, 0 );
   setMwiLed( 1, 0 );
}

/*
*****************************************************************************
**
** FUNCTION:   setMwiLed
**
** PURPOSE:    Set MWI LED
**
** PARAMETERS:  none
**
** RETURNS:   none
**
*****************************************************************************
*/
void setMwiLed( int usr, int state )
{
   int mwi_handle;
   int idc_handle;

   if ( usr == 0 )
   {
      idc_handle = IDC_MWI_LED_OFF;
      mwi_handle = state?gMWI_LED_ON_hdl:gMWI_LED_OFF_hdl;
   }
   else
   {
      idc_handle = IDC_MWI_LED_OFF3;
      mwi_handle = state?gMWI_LED_ON3_hdl:gMWI_LED_OFF3_hdl;
   }
   SendMessage( GetDlgItem( hDlg, idc_handle ), 
                   STM_SETIMAGE, 
                   IMAGE_BITMAP, 
                   mwi_handle );
}

/*
*****************************************************************************
**
** FUNCTION:   initHandset
**
** PURPOSE:    initialize Handset Picture control handles
**
** PARAMETERS:  none
**
** RETURNS:   none
**
*****************************************************************************
*/
void initHandset( void )
{
   // Get handles to Handset images
   gHandset_Onhook_Line1 = SendMessage( GetDlgItem( hDlg, IDC_HANDSET_ONHOOK ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gHandset_Offhook_Line1 = SendMessage( GetDlgItem( hDlg, IDC_HANDSET_OFFHOOK ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gHandset_Onhook_Line2 = SendMessage( GetDlgItem( hDlg, IDC_HANDSET_ONHOOK3 ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gHandset_Offhook_Line2 = SendMessage( GetDlgItem( hDlg, IDC_HANDSET_OFFHOOK3 ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   setHandset( 0, 0 );
   setHandset( 1, 0 );

}

/*
*****************************************************************************
**
** FUNCTION:   setHandset
**
** PURPOSE:    Set Handset on /off hook image
**
** PARAMETERS:  none
**
** RETURNS:   none
**
*****************************************************************************
*/
void setHandset( int usr, int offhook )
{
   int handset_bmp_handle;
   int idc_handle;

   idc_handle = usr==0?IDC_HANDSET_ONHOOK:IDC_HANDSET_ONHOOK3;
   if ( offhook )
   {
      handset_bmp_handle = usr==0?gHandset_Offhook_Line1:gHandset_Offhook_Line2;
   }
   else
   {
      handset_bmp_handle = usr==0?gHandset_Onhook_Line1:gHandset_Onhook_Line2;
   }

   SendMessage( GetDlgItem( hDlg, idc_handle ), 
                STM_SETIMAGE, 
                IMAGE_BITMAP, 
                handset_bmp_handle );
}

/*
*****************************************************************************
**
** FUNCTION:   initRingingVisual
**
** PURPOSE:    initialize Ringing visual effect.
**
** PARAMETERS:  none
**
** RETURNS:   none
**
*****************************************************************************
*/
void initRingingVisual( void )
{
   // Get handles to BRCM Logos
   gBRCM_LOGO_hdl = SendMessage( GetDlgItem( hDlg, IDC_BRCM_LOGO ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gBRCM_LOGO_REV_hdl = SendMessage( GetDlgItem( hDlg, IDC_BRCM_LOGO_REV ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gBRCM_LOGO3_hdl = SendMessage( GetDlgItem( hDlg, IDC_BRCM_LOGO3 ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   gBRCM_LOGO3_REV_hdl = SendMessage( GetDlgItem( hDlg, IDC_BRCM_LOGO3_REV ), STM_GETIMAGE, IMAGE_BITMAP, 0 );
   setRingingVisual( 0, 0 );
   setRingingVisual( 1, 0 );

   bRingVisualInitialized = BOS_TRUE;

   bLine1RingerOn = BOS_FALSE;
   bLine2RingerOn = BOS_FALSE;
}

/*
*****************************************************************************
**
** FUNCTION:   setRingingVisual
**
** PURPOSE:    Set Ringing Visual
**
** PARAMETERS:  none
**
** RETURNS:   none
**
*****************************************************************************
*/
void setRingingVisual( int usr, int state )
{
   int brcm_logo_handle;
   int idc_handle;

   if ( usr == 0 )
   {
      idc_handle = IDC_BRCM_LOGO;
      brcm_logo_handle = state?gBRCM_LOGO_REV_hdl:gBRCM_LOGO_hdl;
   }
   else
   {
      idc_handle = IDC_BRCM_LOGO3;
      brcm_logo_handle = state?gBRCM_LOGO3_REV_hdl:gBRCM_LOGO3_hdl;
   }

   SendMessage( GetDlgItem( hDlg, idc_handle ), 
                   STM_SETIMAGE, 
                   IMAGE_BITMAP, 
                   brcm_logo_handle );
}

/***********************************************************************************
** FUNCTION:   appPicCtrlNotify
**
** PURPOSE:    Picture Control Notification Handler
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
BOS_BOOL appPicCtrlNotify(int id, int msg)
{
   BOS_UINT32 evt;
   BOS_UINT32 line;
   int state = 0;

   if ( msg == STN_DBLCLK )
   {
      if ( (id == IDC_HANDSET_ONHOOK) || (id == IDC_HANDSET_ONHOOK3) )
      {
         static int hookState_Line1 = 0;
         static int hookState_Line2 = 0;
         line = callClientMapGetId( appPicCtrlLine, id );

         if ( (line == CMGR_INVALID_HANDLE) )
         {
            return BOS_FALSE;
         }

         if ( id == IDC_HANDSET_ONHOOK )
         {
            hookState_Line1 ^= 1;
            evt = hookState_Line1?eEPTEVT_OFFHOOK:eEPTEVT_ONHOOK;
         }
         else
         {
            hookState_Line2 ^= 1;
            evt = hookState_Line2?eEPTEVT_OFFHOOK:eEPTEVT_ONHOOK;
         }

         if ( evt == eEPTEVT_OFFHOOK )
         {
            state = 1;
         }

         if ( cmgrEptEvt )
         {
            Log( "CMGR-EP-EVT: Line = %d - Call = %d - Event = %d", line, CMGR_INVALID_HANDLE , evt );
            cmgrEptEvt ( evt, line, CMGR_INVALID_HANDLE, NULL );
            LineFocus = line;
         }
         setHandset( line, state );
         return BOS_TRUE;
      }
   }

   return BOS_FALSE;
}

/***********************************************************************************
** FUNCTION:   cadenceThread
**
** PURPOSE:    Ringing Cadence Thread
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void cadenceThread( void )
{
   int state=0; 

   for ( ;; )
   {
      if ( bRingVisualInitialized )
      {
         state = ~state;
         if ( bLine1RingerOn )
         {
            setRingingVisual( 0, state );
         }
         else
         {
            setRingingVisual( 0, 0 );
         }

         if ( bLine2RingerOn )
         {
            setRingingVisual( 1, state );
         }
         else
         {
            setRingingVisual( 1, 0 );
         }
      }

      bosSleep( 100 );
   }
}

