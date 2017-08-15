/**********************************************************************************
** Copyright © 2009 - 2010 Broadcom Corporation
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
**      This file implements the GUI TCAM client application.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include "gui.h"
#include <tcamApi.h>
#include <bosInit.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

#define    _STR( arg )           { arg, #arg }


/* ---- Private Variables ------------------------------------------------------- */

unsigned char EthernetAddress[7] = "\x00\xE0\x4C\x03\x1C\xD2";

char LocalAddr[MAX_STR]; 
unsigned int LocalPort;
char PcscfAddr[MAX_STR];
unsigned int PcscfPort;
char IMPU[MAX_STR];
char IMPI[MAX_STR];
char Password[MAX_STR];
char RemIMPU[MAX_STR];

CFGSIPUSERCFG cfgUsr;
CFGSIPUSERCFG cfgRemUsr;
CFGSIPPCSCF cfgPcscf;
CFGSIPUSERENDPNTMAP cfgEpt;
CFGSIPUSERDOMAINLIST cfgDomain;
CFGSIPREMOTECFG cfgRemCred;
BOS_UINT8 uCfgRemCredAddr[ MAX_STR ];
BOS_UINT8 uCfgActiveDomain[ MAX_STR ];

CFGPROVDEVICECFG cfgDevice;
CFGEPTCODECCFG cfgCodec;
CFGEPTCODECVIDCFG cfgCodecVid;
CFGEPTLINECFG cfgLine;
CFGSIPBASICCALL cfgBasic;
CFGEPTCODECLIST cfgCodecLst;
CFGEPTCODECVIDLIST cfgCodecVidLst;

GUISTATE geGuiState;
BOS_UINT8 guGuiRemoteParty[ MAX_STR ];
BOS_UINT8 guTcamCmd[ MAX_STR * 2 ];


/* ---- Private Function Prototypes --------------------------------------------- */
#ifdef __cplusplus
extern "C"
#endif

void appStart(void);
INT_PTR CALLBACK appDlgConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


BOS_STATUS tcamProvCb( PROV_CFG_ITEM cfgItem,
                       PROV_CFG_IDX *pCfgIx,
                       void *pData,
                       CFG_ACCESS_ACT eCfgAct );

void tcamEptCtlCb( BOS_UINT32 eCmd,
                   BOS_UINT32 uPhy,
                   BOS_UINT32 uHdl,
                   void *pData );

void tcamMsgCb( BOS_UINT8 *pMsg );

void appDisplay( int row, int col, char *text );
void appDisplayClear( void );

char * tcamGetCharFromId ( int id );
void cfgInit ( void );

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   appDisplayClear
**
** PURPOSE:    Clear the display from the dialog main window.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void appDisplayClear( void )
{
   ListBox_ResetContent( GetDlgItem( hDlg, IDC_DISPLAY ) );
}


/***********************************************************************************
** FUNCTION:   appInit
**
** PURPOSE:    Initializes the application.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void appInit( void )
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

   Log("-----  TCAM Client Startup %02d/%02d/%02d ------------",
       time.wMonth, time.wDay, time.wYear);

   rc = GetProfileInt("TCAM", "LogPkts", 1);
   if (rc)
   {
      PostMessage(hWnd, WM_COMMAND, IDM_PKTS, 0); //Enable packet logging at startup
   }
   rc = GetProfileInt("TCAM", "LogHdrOnly", 1);
   if (rc)
   {
      PostMessage(hWnd, WM_COMMAND, IDM_PKTHDR, 0); //Enable packet header logging at startup
   }

   appSetup();
}


/***********************************************************************************
** FUNCTION:   appSetup
**
** PURPOSE:    Configures the application.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void appSetup(void)
{
   int rc;

   Log("TCAM client configure dialog");

   rc = DialogBox ( hInst,
                    (LPCTSTR) IDD_DLGCFGSIP,
                    hDlg,
                    (DLGPROC) appDlgConfig );
}

/***********************************************************************************
** FUNCTION:   appStart
**
** PURPOSE:    Starts the application.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void appStart( void )
{
   /* Profile configuration.
   **
   **       A) Local Interface.
   **       B) User settings.
   **       C) P-CSCF settings.
   */
   GetProfileString( "TCAM", "LocalAddress", 0,
                     LocalAddr, sizeof(LocalAddr) );
   LocalPort = (unsigned int) GetProfileInt( "TCAM", "LocalPort", 0 );

   GetProfileString( "TCAM", "IMPU", 0,
                     IMPU, MAX_STR );
   GetProfileString( "TCAM", "IMPI", 0,
                     IMPI, MAX_STR );
   GetProfileString( "TCAM", "Password", 0,
                     Password, MAX_STR );

   GetProfileString( "TCAM", "PcscfAddr", 0,
                     PcscfAddr, MAX_STR );
   PcscfPort = (unsigned int) GetProfileInt( "TCAM", "PcscfPort", 0 );

   
   /* Initial GUI state.
   */
   geGuiState = eGUI_IDLE;

   /* Initializes the configuration information that the application will use.
   */
   cfgInit();

   /* Initializes BOS module support.
   */
   bosInit();

   /* Initializes the TCAM application.
   */
   tcamInit();

   appDisplayClear();
   appDisplay( 1, 1, "Welcome to TCAM - Started and Ready" );
}


/***********************************************************************************
** FUNCTION:   appExit
**
** PURPOSE:    Exits the application.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void appExit( void )
{
   Log( "Exit" );
   WriteProfileString("TCAM", "LogPkts", 
      (GetMenuState( GetMenu(hWnd), IDM_PKTS, 0) & MF_CHECKED) ? "1" : "0");
   WriteProfileString("TCAM", "LogHdrOnly", 
      (GetMenuState( GetMenu(hWnd), IDM_PKTHDR, 0) & MF_CHECKED) ? "1" : "0");
}


/***********************************************************************************
** FUNCTION:   tcamGetCharFromId
**
** PURPOSE:    Returns a character associated with a keypad.
**
** PARAMETERS: id - the button identifier
**
** RETURNS:    None.
***********************************************************************************/
char guTcamChar[2];
char * tcamGetCharFromId( int id )
{
   switch ( id )
   {
      case IDC_BUTTON0:
         sprintf ( guTcamChar, "0" );
      break;
      case IDC_BUTTON1:
         sprintf ( guTcamChar, "1" );
      break;
      case IDC_BUTTON2:
         sprintf ( guTcamChar, "2" );
      break;
      case IDC_BUTTON3:
         sprintf ( guTcamChar, "3" );
      break;
      case IDC_BUTTON4:
         sprintf ( guTcamChar, "4" );
      break;
      case IDC_BUTTON5:
         sprintf ( guTcamChar, "5" );
      break;
      case IDC_BUTTON6:
         sprintf ( guTcamChar, "6" );
      break;
      case IDC_BUTTON7:
         sprintf ( guTcamChar, "7" );
      break;
      case IDC_BUTTON8:
         sprintf ( guTcamChar, "8" );
      break;
      case IDC_BUTTON9:
         sprintf ( guTcamChar, "9" );
      break;
      case IDC_BUTTON_HASH:
         sprintf ( guTcamChar, "#" );
      break;
      case IDC_BUTTON_ASTER:
         sprintf ( guTcamChar, "*" );
      break;

      default:
         sprintf ( guTcamChar, "?" );
      break;
   }

   return guTcamChar;
}


/***********************************************************************************
** FUNCTION:   appButton
**
** PURPOSE:    Button press on the application.
**
** PARAMETERS: id - the button identifier
**             msg - the message identifier
**
** RETURNS:    None.
***********************************************************************************/
BOS_BOOL appButton( int id, int msg )
{
   (void)msg;

   switch ( id )
   {
      case IDC_BUTTON1:
      case IDC_BUTTON2:
      case IDC_BUTTON3:
      case IDC_BUTTON4:
      case IDC_BUTTON5:
      case IDC_BUTTON6:
      case IDC_BUTTON7:
      case IDC_BUTTON8:
      case IDC_BUTTON9:
      case IDC_BUTTON0:
      case IDC_BUTTON_ASTER:
      case IDC_BUTTON_HASH:
      {
         /* If we are in 'idle' state, this is collecting digit to
         ** place a call presumably.
         **
         ** Otherwise ignore (for now).
         */
         if ( geGuiState == eGUI_IDLE )
         {
            strcat ( (char *) guGuiRemoteParty,
                     (char *) tcamGetCharFromId( id ) );

            appDisplay( 2, 1, (char *)guGuiRemoteParty );
         }
      }
      break;

      case IDC_BUTTON_SEND:
      {
         if ( geGuiState == eGUI_IDLE )
         {
            geGuiState = eGUI_CALLING;

            sprintf ( (char *) guTcamCmd,
                      "DIAL, %s, %s, %s",
                      cfgUsr.pcUserId,
                      "TCAM-TEST",
                      guGuiRemoteParty );

            appDisplay( 3, 1, "Calling..." );

            tcamCmd ( guTcamCmd );
         }
         else if ( geGuiState == eGUI_CALLED )
         {
            geGuiState = eGUI_CONNECTED;

            sprintf ( (char *) guTcamCmd,
                      "ANSWER, %s, %s",
                      cfgUsr.pcUserId,
                      "TCAM-TEST" );

            appDisplay( 3, 1, "Answering..." );

            tcamCmd ( guTcamCmd );
         }
      }
      break;

      case IDC_BUTTON_END:
      {
         if ( geGuiState != eGUI_IDLE )
         {
            /* Send command to clean up.
            */
            sprintf ( (char *) guTcamCmd,
                      "HANGUP, %s, %s",
                      cfgUsr.pcUserId,
                      "TCAM-TEST" );

            appDisplay( 3, 1, "Ending..." );

            tcamCmd ( guTcamCmd );
         }

         geGuiState = eGUI_IDLE;
         memset ( guGuiRemoteParty,
                  0,
                  sizeof ( guGuiRemoteParty ) );
      }
      break;

      case IDC_BUTTON_UP:
      case IDC_BUTTON_OK:
      case IDC_BUTTON_RIGHT:
      case IDC_BUTTON_LEFT:
      case IDC_BUTTON_DOWN:
      default:
         /* Do nothing for now.
         */
         appDisplay( 3, 1, "Do nothing..." );
      break;
   }
   
   return BOS_TRUE;
}


/***********************************************************************************
** FUNCTION:   appMenu
**
** PURPOSE:    Menu handling from the application.
**
** PARAMETERS: id - the identifier of the menu item selected.
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

   switch (message)
   {
      case WM_INITDIALOG:
      {      
         PostMessage(hDlg, WMUSR_INIT, 0, 0);
      }
      break;

      case WMUSR_INIT:
      {
         int val;

         GetProfileString("TCAM", "LocalAddress", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_LOCALADDR, sBuffer);
         val = GetProfileInt("TCAM", "LocalPort", 0);
         SetDlgItemInt(hDlg, IDC_LOCALPORT, val, FALSE);

         GetProfileString("TCAM", "IMPU", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_IMPU, sBuffer);
         GetProfileString("TCAM", "IMPI", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_IMPI, sBuffer);
         GetProfileString("TCAM", "Password", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_PASSWORD, sBuffer);

         GetProfileString("TCAM", "PcscfAddr", 0, sBuffer, MAX_STR);
         SetDlgItemText(hDlg, IDC_PCSCFADDR, sBuffer);
         val = GetProfileInt("TCAM", "PcscfPort", 0);
         SetDlgItemInt(hDlg, IDC_PCSCFPORT, val, FALSE);
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
         WriteProfileString("TCAM", "LocalAddress", sBuffer);
         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_LOCALPORT, NULL, FALSE));
         WriteProfileString("TCAM", "LocalPort", sBuffer); 

         GetDlgItemText(hDlg, IDC_IMPU, sBuffer, MAX_STR);
         WriteProfileString("TCAM", "IMPU", sBuffer);
         GetDlgItemText(hDlg, IDC_IMPI, sBuffer, MAX_STR);
         WriteProfileString("TCAM", "IMPI", sBuffer);
         GetDlgItemText(hDlg, IDC_PASSWORD, sBuffer, MAX_STR);
         WriteProfileString("TCAM", "Password", sBuffer);


         GetDlgItemText(hDlg, IDC_PCSCFADDR, sBuffer, MAX_STR);
         WriteProfileString("TCAM", "PcscfAddr", sBuffer);
         sprintf( sBuffer, "%d", GetDlgItemInt(hDlg, IDC_PCSCFPORT, NULL, FALSE));
         WriteProfileString("TCAM", "PcscfPort", sBuffer); 
      }
      break;

      default:
        return FALSE;
   }
   return TRUE;
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
void appDisplay( int row, int col, char *text )
{
   char buf[129];
   memset ( buf, 0, sizeof(buf) );
   ListBox_GetText( GetDlgItem(hDlg, IDC_DISPLAY), row-1, buf);
   buf[strlen(buf)] = ' ';
   strcpy( &buf[col-1], text);
   ListBox_DeleteString(GetDlgItem(hDlg, IDC_DISPLAY), row-1);
   ListBox_InsertString(GetDlgItem(hDlg, IDC_DISPLAY), row-1, buf);
}


/****************************************************************************
** FUNCTION:   tcamProvCb
**
** PURPOSE:    Callback functionality for provisioning control outside of
**             the TCAM.
**
** PARAMETERS: cfgItem  - the provisioning item of interest.
**             pCfgIx   - the index associated with the provisioning item.
**             pData    - pointer to the placeholder for the provisioning
**                        information.
**             eCfgAct  - the provisioning action to take.
**
** RETURNS:    BOS_STATUS_OK on success, BOS_STATUS_ERR otherwise.
**
** NOTE:    
*****************************************************************************/
BOS_STATUS tcamProvCb( PROV_CFG_ITEM cfgItem,
                       PROV_CFG_IDX *pCfgIx,
                       void *pData,
                       CFG_ACCESS_ACT eCfgAct )
{
   BOS_STATUS res = BOS_STATUS_OK;

   Log( "tcamProvCb: Item (%d) - Id (%d, %s) - Action (%d)",
        cfgItem,
        pCfgIx->uIndex,
        (pCfgIx->pcCharIdx != NULL) ? (const char *)pCfgIx->pcCharIdx : "(null)",
        eCfgAct );

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
               *((CFGEPTCODECCFG **)pData) = &cfgCodec;
            }
            break;

            case CFGITEM(EPT,  CodecVidConcept):
            {
               *((CFGEPTCODECVIDCFG **)pData) = &cfgCodecVid;
            }
            break;

            case CFGITEM(EPT,  LineConcept):
            {
               *((CFGEPTLINECFG **)pData) = &cfgLine;
            }
            break;

            case CFGITEM(EPT,  CodecList):
            {
               *((CFGEPTCODECLIST **)pData) = &cfgCodecLst;
            }
            break;

            case CFGITEM(EPT,  CodecVidList):
            {
               *((CFGEPTCODECVIDLIST **)pData) = &cfgCodecVidLst;
            }
            break;

            case CFGITEM(SIP, PCSCFConcept):
            {
               *((CFGSIPPCSCF **)pData) = &cfgPcscf;
            }
            break;

            case CFGITEM(SIP, UserConcept):
            {
               /* Make sure we reset this information in case
               ** overwritten by other process since we are keeping
               ** static configuration information as opposed to
               ** dynamically create the data queried.
               */
               if ( !cfgUsr.pcActiveDomain )
               {
                  cfgUsr.pcActiveDomain = uCfgActiveDomain;
               }
               *((CFGSIPUSERCFG **)pData) = &cfgUsr;
            }
            break;

            case CFGITEM( SIP,  BasicCallConcept ):
            {
               *((CFGSIPBASICCALL **)pData) = &cfgBasic;   
            }
            break;

            case CFGITEM( SIP,  RemoteConcept ):
            {
               *((CFGSIPREMOTECFG **)pData) = &cfgRemCred;   
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
      }
      break;

      case CFG_ITEM_SET:
      {
         if ( (cfgItem == CFGITEM(SIP,  RemoteConcept)) && pData )
         {
            sprintf ( (char *) cfgRemCred.pcAddr,
                      "%s",
                      ((CFGSIPREMOTECFG *)pData)->pcAddr );
            cfgRemCred.uSipPort = ((CFGSIPREMOTECFG *)pData)->uSipPort;
         }
         else if ( cfgItem == CFGITEM(SIP,  UserConcept) )
         {
            if ( pCfgIx->pNext != NULL )
            {
               /* This is the creation (SET) of a remote user, based on the
               ** passed in local user credentials.
               */
               if ( !strcmp ( (const char *) cfgUsr.pcUserId,
                              (const char *) pCfgIx->pNext->pcCharIdx ) )
               {
                  memcpy ( (void *) &cfgRemUsr,
                           (const void *) &cfgUsr,
                           sizeof ( cfgUsr ) );

                  sprintf( RemIMPU,
                           "%s",
                           pCfgIx->pcCharIdx );
                  cfgRemUsr.pcUserId = reinterpret_cast< BOS_UINT8* >( RemIMPU );
                  cfgRemUsr.pcDisplayInfo = reinterpret_cast< BOS_UINT8* >( RemIMPU );
               }
            }
            else if ( pData && ((CFGSIPUSERCFG *)pData)->pcActiveDomain )
            {
               memset ( uCfgActiveDomain, 0, MAX_STR );
               strcpy ( (char *) uCfgActiveDomain,
                        (const char *) ((CFGSIPUSERCFG *)pData)->pcActiveDomain );
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
** FUNCTION:   tcamEptCtlCb
**
** PURPOSE:    Callback functionality for session control outside of
**             the TCAM.
**
** PARAMETERS: eCmd  - the session command to apply.
**             uPhy  - the physical interface associated with the session.
**             uHdl  - the handle for which this command applies.
**             pData - additional data (may be NULL) necessary to complete
**                     the action.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************/
void tcamEptCtlCb( BOS_UINT32 eCmd,
                   BOS_UINT32 uPhy,
                   BOS_UINT32 uHdl,
                   void *pData )
{
   (void) pData;
   (void) uHdl;
   (void) uPhy;

   switch ( (CMGRSESCMD) eCmd )
   {
      case eCMGRSESCMD_CREATE:
      {
      }
      break;

      case eCMGRSESCMD_UPDATE:
      {
      }
      break;

      case eCMGRSESCMD_TERMINATE:
      {
      }
      break;

      default:
      break;
   }
}


/****************************************************************************
** FUNCTION:   tcamEptCtlCb
**
** PURPOSE:    Callback functionality for messaging outside of
**             the TCAM.
**
** PARAMETERS: puMsg - the message passed.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************/
void tcamMsgCb( BOS_UINT8 *puMsg )
{
   BOS_UINT8 *pAction = NULL;
   BOS_UINT8 *pUsr = NULL;
   BOS_UINT8 *pCallId = NULL;
   BOS_UINT8 *pParam = NULL;

   pAction = (BOS_UINT8*) strTok( (char **) &puMsg, "," );
   pUsr = (BOS_UINT8*) strTok( (char **) &puMsg, ",\r\n\0" );
   pCallId = (BOS_UINT8*) strTok( (char **) &puMsg, ",\r\n\0" );
   pParam = (BOS_UINT8*) strTok( (char **) &puMsg, ",\r\n\0" );

   if ( (strstr ( (const char *) pAction, "NEWCALL" ) != NULL) &&
        (geGuiState == eGUI_IDLE) )
   {
      geGuiState = eGUI_CALLED;
      sprintf ( (char *) guGuiRemoteParty,
                "%s",
                (char *) pParam );

      appDisplay( 2, 1, (char *)guGuiRemoteParty );
      appDisplay( 3, 1, "Is Calling..." );

      /* Set the local handle back to TCAM for any future actions
      ** or events associated with this call.
      */
      sprintf ( (char *) guTcamCmd,
                "SETHANDLE, %s, %s, %s",
                cfgUsr.pcUserId,
                "TCAM-TEST",
                pCallId );

      tcamCmd ( guTcamCmd );
   }
   else if ( (strstr ( (const char *) pAction, "ACTIVE" ) != NULL) &&
             (geGuiState == eGUI_CALLING) )
   {
      geGuiState = eGUI_CONNECTED;

      appDisplay( 3, 1, "Connected..." );
   }
   else if ( (strstr ( (const char *) pAction, "HANGUP" ) != NULL) )
   {
      geGuiState = eGUI_IDLE;
      memset ( (void *) guGuiRemoteParty,
               0,
               sizeof (guGuiRemoteParty) );

      appDisplay( 2, 1, (char *)guGuiRemoteParty );
      appDisplay( 3, 1, "Terminated..." );
   }
}


/****************************************************************************
** FUNCTION:   cfgInit
**
** PURPOSE:    Initializes the configuration information that the TCAM
**             application will query during run time.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************/
void cfgInit ( void )
{
   BOS_IP_ADDRESS bosIp;
   BOS_STATUS res;

   bosIpAddrCreateFromStr( LocalAddr, &bosIp );
   memcpy ( &cfgDevice.IpAddress, &bosIp, sizeof ( BOS_IP_ADDRESS ) );
   cfgDevice.pcFQDN = NULL;
   cfgDevice.pMAC = EthernetAddress;
   cfgDevice.bDevEnabled = BOS_TRUE;
   cfgDevice.uSIPRecvPort = LocalPort;
   cfgDevice.bDisableKeepAlive = BOS_TRUE;

   cfgCodec.bRTCPXRepEnable = BOS_FALSE;
   cfgCodec.bT38Enable = BOS_FALSE;
   cfgCodec.bV152Enable = BOS_FALSE;
   cfgCodec.sG711PktPeriod = 20;
   cfgCodec.sRTCPRate = 50;
   cfgCodec.bDtmfRelayEnabled = BOS_TRUE;
   cfgCodec.uTelEvtPayloadNum = 0;

   cfgCodecVid.uH264Profile = 66;
   cfgCodecVid.uH264PktMode = 0;
   cfgCodecVid.uH264Level = 13;
   cfgCodecVid.uH264MaxMbps = 1200;
   cfgCodecVid.uH264MaxBr = 1000;
      
   cfgCodecLst.codecType = eCFG_CODECTYPE_PCMU;
   cfgCodecLst.pNext = NULL;

#if CMGRCFG_VIDEO_MEDIA
   cfgCodecVidLst.codecType =  eCFG_CODECVIDTYPE_H264;
#else
   cfgCodecVidLst.codecType = eCFG_CODECVIDTYPE_UNKNOWN;
#endif
   cfgCodecVidLst.pNext = NULL;

   cfgBasic.Act.bCfgStatus = BOS_TRUE;
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

   cfgDomain.pcDomain = reinterpret_cast< BOS_UINT8* >(PcscfAddr);
   cfgDomain.pNext = NULL;

   res = bosIpAddrCreateFromStr( PcscfAddr, &bosIp );
   cfgPcscf.uSIPPort = PcscfPort;
   if ( res == BOS_STATUS_OK )
   {
      memcpy ( &cfgPcscf.PCSCFAddr, &bosIp, sizeof ( BOS_IP_ADDRESS ) );
      cfgPcscf.pcPCSCFDomain = NULL;
   }
   else
   {
      cfgPcscf.pcPCSCFDomain = reinterpret_cast<BOS_UINT8 *> (PcscfAddr);
   }
   cfgPcscf.pNext = NULL;

   cfgPcscf.uSigTrans = CFG_SIG_TRANS_UDP | CFG_SIG_TRANS_TCP;
   cfgPcscf.bIgnoreTcpFallBack = BOS_TRUE;
   cfgPcscf.bBackupRecovery = BOS_TRUE;

   cfgEpt.sEndpntNum = 0;
   cfgEpt.bIn = BOS_TRUE;
   cfgEpt.bOut = BOS_TRUE;
   cfgEpt.pNext = NULL;

   cfgUsr.Act.bCfgStatus = BOS_TRUE;
   cfgUsr.Act.pcAdminStatInfo = NULL;
   cfgUsr.Act.pcOperStatInfo = NULL;
   cfgUsr.sUserType = CFG_SIP_USER_TYPE_USERNAME;
   cfgUsr.pcUserId = reinterpret_cast< BOS_UINT8* >( IMPU );
   cfgUsr.pcDisplayInfo = reinterpret_cast< BOS_UINT8* >( IMPU );
   cfgUsr.pcPrivId = reinterpret_cast< BOS_UINT8* >( IMPI );
   cfgUsr.pEndPntMap = &cfgEpt;
   cfgUsr.credentials.CredType = CFG_SIP_USER_CRED_TYPE_PASSWORD;
   cfgUsr.credentials.pCredVal = reinterpret_cast< BOS_UINT8* >( Password );
   cfgUsr.pDomainList = &cfgDomain;
   cfgUsr.pcActiveDomain = reinterpret_cast< BOS_UINT8* >( uCfgActiveDomain );
   cfgUsr.uInitRegDelay = 0;
   cfgUsr.pNext = NULL;

   cfgRemCred.pcAddr = uCfgRemCredAddr;
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

