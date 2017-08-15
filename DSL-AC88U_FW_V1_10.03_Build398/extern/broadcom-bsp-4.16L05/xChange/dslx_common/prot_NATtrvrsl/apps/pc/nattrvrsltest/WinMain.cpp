/***************************************************************************
*
*     Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
*     Contains proprietary and confidential information.
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      NATTRVSL Test Client Windows Interface
*
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>
#include <winsock.h>
#include "resource.h"
#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>

#include "WinMain.h"
#include "nattrvrsltest.h"
/* ---- Public Variables ------------------------------------- */
HINSTANCE hInst;                  // current instance
HWND hWnd;                        // main window
HWND hMainDlg;                    // Dialog Window handle

/* ---- Private Constants and Types -------------------------- */
#define WIDTH(rect) (rect.right - rect.left)
#define HEIGHT(rect) (rect.bottom - rect.top)

#define MAX_LOADSTRING     100
#define MAXSTR             4096

/* ---- Private Variables ------------------------------------ */
static HANDLE LogFile = INVALID_HANDLE_VALUE;

const static char *mapReqdTransport[] = 
{
   "NONE",  // not specified
   "UDP",   
   "TCP",
   NULL
};

const static char *mapPortProps[] = 
{
   "NONE",        // not specified
   "NO ALIGN",    // no alignment
   "ODD PORT",    // odd port
   "EVEN PORT",
   "RES NEXT",    // even port reserve next
   NULL
};

const static char *mapOriginatingTransport[] = 
{
   "UDP",   
   "TCP",
   "TLS",
   NULL
};

const static char *mapSdpRole[] = 
{
   "OFFER",   
   "ANSWER",
   NULL
};

/* ---- Private Function Prototypes -------------------------- */
static TCHAR szTitle[MAX_LOADSTRING];   // The title bar text
static TCHAR szWindowClass[MAX_LOADSTRING];      // The title bar text
static HWND CreateMainDlg(HWND hParent);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK MainDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static void WndLog(char *buf);
static BOOL InitInstance(HINSTANCE, int);

static void fillReqdTransportDlg( HWND hDlg, int listId, int dflt );
static void fillPortPropsDlg( HWND hDlg, int listId, int dflt );
static void fillOrgTransportDlg( HWND hDlg, int listId, int dflt );
static void updateAllocateParms(HWND hDlg);
static void updateActiveDestParms(HWND hDlg);
static void updateConnectParms(HWND hDlg);
static void updateSendDataParms(HWND hDlg);
static void updateCndtGatherParms(HWND hDlg);
static void fillSdpRoleDlg( HWND hDlg, int listId, int dflt );
static void updateSdpEncodeParms(HWND hDlg);
static void updateSdpParseParms(HWND hDlg);

/* ---- Functions -------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   WinMain
**
** PURPOSE:    Application entry point
**
** PARAMETERS: none
**
** RETURNS:    0 - Success
**             1 - Failure
**
** NOTE:
*****************************************************************************
*/
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
   MSG msg;
   WNDCLASSEX wcex;
   WSADATA wsaData;
   
   (void)hPrevInstance;
   (void)lpCmdLine;

   // Initialize winsock
   WSAStartup(0x202,&wsaData);
   LoadString(hInstance, IDS_NATTC_TITLE, szTitle, MAX_LOADSTRING);
   LoadString(hInstance, IDS_SC, szWindowClass, MAX_LOADSTRING);


   // Register the window class
   wcex.cbSize = sizeof (WNDCLASSEX);
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = (WNDPROC) WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, (LPCTSTR) IDI_NATTC_ICON);
   wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wcex.lpszMenuName = (LPCSTR) IDR_NATTC_MENU;
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = NULL;
   RegisterClassEx(&wcex);

   // Perform application initialization:
   if (!InitInstance(hInstance, nCmdShow))
   {
      return FALSE;
   }

   InitCommonControls();

   hMainDlg = CreateMainDlg(hWnd);

   // initialize natt test
   nattrvrsltestInit();

   // Main message loop:
   while (GetMessage(&msg, NULL, 0, 0))
   {
      BOOL handled;

      handled = IsDialogMessage(hMainDlg, &msg);

      if (!handled)
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   // Stop winsock
   WSACleanup();
   return msg.wParam;
}

/*
*****************************************************************************
** FUNCTION:   InitInstance 
**
** PURPOSE:    Saves instance handle and creates main window
**
** PARAMETERS: none
**
** RETURNS:    0 - Success
**             1 - Failure
**
** NOTE:
*****************************************************************************
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   // Store instance handle in our global variable
   hInst = hInstance;               

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
         CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

/*
*****************************************************************************
** FUNCTION:   WndProc
**
** PURPOSE:    Processes messages for the main window.
**
** PARAMETERS: none
**
** RETURNS:    
**
** NOTE:
*****************************************************************************
*/
LRESULT CALLBACK WndProc(HWND hWnd, 
                         UINT message, 
                         WPARAM wParam, 
                         LPARAM lParam)
{
   int wmId;
   int wmEvent;
   static char fileName[MAXSTR]="";

   switch (message)
   {
      case WM_COMMAND:
      {
         wmId = LOWORD(wParam);
         wmEvent = HIWORD(wParam);
         // Parse the menu selections:
         switch (wmId)
         {
            case ID_FILE_EXIT:
               DestroyWindow(hWnd);
               break;
            case ID_FILE_LOG:
               if (GetMenuState( GetMenu(hWnd), ID_FILE_LOG, 0) & MF_CHECKED)
               {
                  CheckMenuItem(GetMenu(hWnd), ID_FILE_LOG, MF_UNCHECKED);
                  CloseHandle(LogFile);
               }
               else
               {
                  SYSTEMTIME time;

                  CheckMenuItem(GetMenu(hWnd), ID_FILE_LOG, MF_CHECKED);
                  GetLocalTime(&time);
                  sprintf( fileName, 
                           "nattrvslTestLog %02d-%02d-%02d %02d`%02d`%02d.txt",
                           time.wYear % 100, time.wMonth, time.wDay, time.wHour,
                           time.wMinute, time.wSecond);

                  LogFile = CreateFile(fileName, 
                                       GENERIC_READ|GENERIC_WRITE, 
                                       0, NULL, OPEN_ALWAYS, 
                                       FILE_ATTRIBUTE_NORMAL, NULL);
                  //DWORD error = GetLastError();
                  SetFilePointer(LogFile, 0, NULL, FILE_END);
               }
               break;
            case ID_LOG_FREEZE:
            {
               if (GetMenuState( GetMenu(hWnd), ID_LOG_FREEZE, 0) & MF_CHECKED)
               {
                  CheckMenuItem(GetMenu(hWnd), ID_LOG_FREEZE, MF_UNCHECKED);
               }
               else
               {
                  CheckMenuItem(GetMenu(hWnd), ID_LOG_FREEZE, MF_CHECKED);
               }
            }
            break;
            case ID_LOG_CLEAR:
            {
               ListBox_ResetContent(GetDlgItem(hMainDlg, IDC_LOGGER));
            }
            break;
            case ID_HELP_ABOUT:
            {
               char buffer[100];

               sprintf(buffer, "NATT Client version.");
               MessageBox(hWnd, buffer, "About NATT Client", MB_OK);
            }
            break;
            default:
            {
               // do nothing      
            }
         }
      }
      break;

      case WM_ACTIVATE:
      {
         if (LOWORD(wParam) != WA_INACTIVE)
         {
            SetFocus (hMainDlg);
         }
      }
      break;

      case WM_SIZE:
      {
         SetWindowPos(hMainDlg, NULL, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_NOMOVE|SWP_NOZORDER);
      }
      break;

      case WM_DESTROY:
      {
         PostQuitMessage(0);
      }
      break;

      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


/*
*****************************************************************************
** FUNCTION:   CreateMainDlg
**
** PURPOSE:    Create Main dialog
**
** PARAMETERS: none
**
** RETURNS:    
*
** NOTE:
*****************************************************************************
*/
HWND CreateMainDlg( HWND hParent)
{
   HWND hDlg;
   RECT rwin;

   hDlg = CreateDialog(hInst, (LPCTSTR) (IDD_NATTC_DLG), hParent, (DLGPROC) MainDlgProc);

   GetWindowRect(hDlg, &rwin);
   SetWindowPos( hWnd, NULL, 0, 0, WIDTH(rwin) + 4, 
                 HEIGHT(rwin) + GetSystemMetrics(SM_CYCAPTION) + 
                     GetSystemMetrics(SM_CYMENU) + 4,
                 SWP_NOMOVE|SWP_NOZORDER);

   return hDlg;
}

/*
*****************************************************************************
** FUNCTION:   MainDlgProc
**
** PURPOSE:    Dialog Window Proc
**
** PARAMETERS: none
**
** RETURNS:    0 - Success
**             1 - Failure
**
** NOTE:
*****************************************************************************
*/
LRESULT CALLBACK MainDlgProc(HWND hDlg, 
                             UINT message, 
                             WPARAM wParam, 
                             LPARAM lParam)
{
   static RECT winrectlog, winrectdlg;
   char sBuffer[250];
   char buf[50];
   int index;
   int length;

   switch (message)
   {
      case WM_INITDIALOG:
      {
         //finish init after dialog displayed
         PostMessage(hDlg, WMUSR_INIT, 0, 0); 
         GetWindowRect(hDlg, &winrectdlg);
         GetWindowRect(GetDlgItem(hDlg, IDC_LOGGER), &winrectlog);

         return TRUE;
      }
      break;

      case WMUSR_INIT:     
      {
         hostent *hostentp;
         struct in_addr addr;

         // Fill in own IP address
         gethostname(buf, sizeof(buf));
         hostentp = gethostbyname(buf);
         for (index = 0; hostentp->h_addr_list[index]; index++)
         {
            addr.S_un.S_addr = *(unsigned long *)(hostentp->h_addr_list[index]);
            ComboBox_InsertString(GetDlgItem(hDlg, IDC_LOCAL_IPADDR), 
                                  -1, 
                                  inet_ntoa(addr));
         }
         if (ComboBox_FindStringExact(GetDlgItem(hDlg, IDC_LOCAL_IPADDR), 
                                      -1, 
                                      "127.0.0.1") == CB_ERR)
         {
            ComboBox_InsertString(GetDlgItem(hDlg, IDC_LOCAL_IPADDR), 
                                  -1, 
                                  "127.0.0.1");
         }
         ::GetProfileString("NATC", 
                            "LocalAddr", 
                            "", 
                            sBuffer, 
                            sizeof(sBuffer));
         index = ComboBox_FindStringExact(GetDlgItem(hDlg, 
                                                     IDC_LOCAL_IPADDR), 
                                          -1, 
                                          sBuffer);
         if (index != CB_ERR)
         {
            ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_LOCAL_IPADDR), index);
         }
         else
         {
            ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_LOCAL_IPADDR), 0);
         }
         addr.S_un.S_addr = *(unsigned long *)(hostentp->h_addr_list[0]);   

         ::GetProfileString("NATC", "TlsPort", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_TLS_PORT, sBuffer);
         
         ::GetProfileString("NATC", "TurnUdpPort", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_TURN_UDP_PORT, sBuffer);

         ::GetProfileString("NATC", "TurnTcpPort", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_TURN_TCP_PORT, sBuffer);

         ::GetProfileString("NATC", "TurnTlsPort", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_TURN_TLS_PORT, sBuffer);

         ::GetProfileString("NATC", "TurnServerAddr", inet_ntoa(addr), sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_TURN_SERVER_IPADDR, sBuffer);

         ::GetProfileString("NATC", "TurnServerPort", "3478", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_TURN_SERVER_PORT, sBuffer);

         ::GetProfileString("NATC", "StunServerAddr", inet_ntoa(addr), sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_STUN_SERVER_IPADDR, sBuffer);

         ::GetProfileString("NATC", "StunServerPort", "3478", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_STUN_SERVER_PORT, sBuffer);

         // get local udp port list
         for ( index = 0; index < MAX_UDP_PORTS; index++ )
         {
            ListBox_GetText( GetDlgItem(hDlg, IDC_UDP_PORT_LIST), index, sBuffer );
            sprintf( buf, "SC_UDP_Port%d", index );
            if (::GetProfileString( "NATC", buf, "", sBuffer, sizeof(sBuffer)) > 0)
            {
               int i;
               i = ListBox_AddString(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), sBuffer);
               ListBox_SetTopIndex(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), i);
            }
         }
         // scroll to the top of the list
         if ( ListBox_GetCount(GetDlgItem(hDlg, IDC_UDP_PORT_LIST)) )
         {
            ListBox_SetTopIndex(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), 0);
         }

         gStuncStart = GetProfileInt( "NATC", "StuncStart", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_STUNC_START), gStuncStart);
         gTurncStart = GetProfileInt( "NATC", "TurncStart", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_TURNC_START), gTurncStart);
         gIceStart = GetProfileInt( "NATC", "IceStart", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_START), gIceStart);

         gSyncEvt = GetProfileInt( "NATC", "syncEvt", 0 );
         ::GetProfileString("NATC", "StunMaxWait", "0", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_STUN_MAX_WAIT, sBuffer);
         Button_SetCheck(GetDlgItem(hDlg, IDC_SYNCEVT), gSyncEvt);
         gStuntest_BindingError = GetProfileInt( "NATC", "BindingError", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_BINDING_ERROR), gStuntest_BindingError);
         gStuntest_SharedSecretError = GetProfileInt( "NATC", "SharedSecretError", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_SS_ERROR), gStuntest_SharedSecretError);

         // get bandwidth, lifetime and required IP addr 
         ::GetProfileString("NATC", "TurnBandwidth", "0", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_BANDWIDTH, sBuffer);

         ::GetProfileString("NATC", "TurnLifetime", "0", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_LIFETIME, sBuffer);
         
         ::GetProfileString("NATC", "TurnReqdIPAddr", "", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_REQD_IPADDR, sBuffer);

         ::GetProfileString("NATC", "TurnReqdPort", "0", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_REQD_PORT, sBuffer);

         // Fill combo boxes: transport and Port Props
         gReqdTransport = GetProfileInt("NATC", "TurnReqdTransport", 0) - 1;
         fillReqdTransportDlg(hDlg, IDC_TRANSPORT, gReqdTransport+1);

         gPortProps = GetProfileInt("NATC", "TurnPortProps", 0) - 1;
         fillPortPropsDlg(hDlg, IDC_PORT_PROPS, gPortProps + 1 );

         gTurnOrgTransport = GetProfileInt("NATC", "TurnOrgTransport", 0);
         fillOrgTransportDlg(hDlg, IDC_ORG_TRANSPORT, gTurnOrgTransport );

         ::GetProfileString("NATC", "TurnAllocateHdl", "0", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ALLOCATE_HDL, sBuffer);

         gTurnctest_AllocateError = GetProfileInt("NATC", "TurnAllocateErr", 0);
         Button_SetCheck(GetDlgItem(hDlg, IDC_ALLOCATE_ERR), gTurnctest_AllocateError);

         ::GetProfileString("NATC", "TurnActiveDestIPAddr", "", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ACTIVE_DEST_IPADDR, sBuffer);

         ::GetProfileString("NATC", "TurnActiveDestPort", "0", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ACTIVE_DEST_PORT, sBuffer);

         gTurnctest_ActiveDestError = GetProfileInt("NATC", "TurnActiveDestErr", 0);
         Button_SetCheck(GetDlgItem(hDlg, IDC_ACTIVE_DEST_ERR), gTurnctest_ActiveDestError);

         ::GetProfileString("NATC", "TurnCnxRemoteIPAddr", "", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_CNX_REMOTE_IPADDR, sBuffer);

         ::GetProfileString("NATC", "TurnCnxRemotePort", "0", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_CNX_REMOTE_PORT, sBuffer);

         gTurnctest_ConnectError = GetProfileInt("NATC", "TurnConnectErr", 0);
         Button_SetCheck(GetDlgItem(hDlg, IDC_CONNECT_ERR), gTurnctest_ConnectError);
         ::GetProfileString("NATC", "TurnSendDataRemoteIPAddr", "", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_SEND_DATA_REMOTE_IPADDR, sBuffer);

         ::GetProfileString("NATC", "TurnSendDataRemotePort", "0", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_SEND_DATA_REMOTE_PORT, sBuffer);

         // ICE Specific configurations
         ::GetProfileString("NATC", "IceRtpPort1", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_RTP_PORT1, sBuffer);
         ::GetProfileString("NATC", "IceRtpPort2", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_RTP_PORT2, sBuffer);
         ::GetProfileString("NATC", "IceRtpPort3", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_RTP_PORT3, sBuffer);
         ::GetProfileString("NATC", "IceRtcpPort1", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_RTCP_PORT1, sBuffer);
         ::GetProfileString("NATC", "IceRtcpPort2", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_RTCP_PORT2, sBuffer);
         ::GetProfileString("NATC", "IceRtcpPort3", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_RTCP_PORT3, sBuffer);

         ::GetProfileString("NATC", "IcePrefLocal", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_PREF_LOCAL, sBuffer);
         ::GetProfileString("NATC", "IcePrefHost", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_PREF_HOST, sBuffer);
         ::GetProfileString("NATC", "IcePrefSrflx", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_PREF_SRFLX, sBuffer);
         ::GetProfileString("NATC", "IcePrefPrflx", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_PREF_PRFLX, sBuffer);
         ::GetProfileString("NATC", "IcePrefRelay", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_PREF_RELAY, sBuffer);

         gIceBRtp[0] = GetProfileInt( "NATC", "IceBRtp1", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP1), gIceBRtp[0]);
         gIceBRtp[1] = GetProfileInt( "NATC", "IceBRtp2", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP2), gIceBRtp[1]);
         gIceBRtp[2] = GetProfileInt( "NATC", "IceBRtp3", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP3), gIceBRtp[2]);
         gIceBRtcp[0] = GetProfileInt( "NATC", "IceBRtcp1", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP1), gIceBRtcp[0]);
         gIceBRtcp[1] = GetProfileInt( "NATC", "IceBRtcp2", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP2), gIceBRtcp[1]);
         gIceBRtcp[2] = GetProfileInt( "NATC", "IceBRtcp3", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP3), gIceBRtcp[2]);

         gIcePrefOverride = GetProfileInt( "NATC", "IcePrefOverride", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_PREF_OVERRIDE), gIcePrefOverride);

         gIceLiteMode = GetProfileInt( "NATC", "IceLiteMode", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_LITE_MODE), gIceLiteMode);

         gIceAggressiveNom = GetProfileInt( "NATC", "IceAggressiveNom", 0 );
         Button_SetCheck(GetDlgItem(hDlg, IDC_ICE_AGGRESSIVE_NOM), gIceAggressiveNom);

         ::GetProfileString("NATC", "IceAgentId", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_AGENT_ID, sBuffer);

         ::GetProfileString("NATC", "IceMaxWait", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_MAX_WAIT, sBuffer);

         ::GetProfileString("NATC", "IceSdpAgentId", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_SDP_AGENT_ID, sBuffer);

         gIceSdpRole = GetProfileInt("NATC", "IceSdpRole", 0);
         fillSdpRoleDlg(hDlg, IDC_SDP_ROLE, gIceSdpRole);

         ::GetProfileString("NATC", "IceRSdpAgentId", 0, sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_ICE_RSDP_AGENT_ID, sBuffer);

         gIceRSdpRole = GetProfileInt("NATC", "IceRSdpRole", 0);
         fillSdpRoleDlg(hDlg, IDC_RSDP_ROLE, gIceRSdpRole);

         ::GetProfileString("NATC", "IceSdpLine1", "", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_SDP_TEXT1, sBuffer);
         ::GetProfileString("NATC", "IceSdpLine2", "", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_SDP_TEXT2, sBuffer);
         ::GetProfileString("NATC", "IceSdpLine3", "", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_SDP_TEXT3, sBuffer);
         ::GetProfileString("NATC", "IceSdpLine4", "", sBuffer, sizeof(sBuffer));
         ::SetDlgItemText(hDlg, IDC_SDP_TEXT4, sBuffer);

         return TRUE;
      }
      break;
      case WM_SIZE:
      {
         // Readjust size and position of logger listbox
         SetWindowPos(GetDlgItem(hDlg, IDC_LOGGER), NULL,
                      0, 
                      0, 
                      LOWORD(lParam)-(WIDTH(winrectdlg)-WIDTH(winrectlog)), 
                      HIWORD(lParam)-(HEIGHT(winrectdlg)-HEIGHT(winrectlog)), 
                      SWP_NOMOVE|SWP_NOZORDER);
      }
      break;
      case WM_DESTROY:
      {
         // read our current dialog configuration items and save 
         // them to our profile.
         ::GetDlgItemText(hDlg, IDC_LOCAL_IPADDR, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "LocalAddr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_TLS_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TlsPort", sBuffer);

         ::GetDlgItemText(hDlg, IDC_TURN_UDP_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnUdpPort", sBuffer);

         ::GetDlgItemText(hDlg, IDC_TURN_TCP_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnTcpPort", sBuffer);

         ::GetDlgItemText(hDlg, IDC_TURN_TLS_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnTlsPort", sBuffer);

         // store udp port list
         length = ListBox_GetCount(GetDlgItem(hDlg, IDC_UDP_PORT_LIST));
         for ( index = 0; index < length; index++ )
         {
            ListBox_GetText(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), index, sBuffer);
            sprintf( buf, "SC_UDP_Port%d", index );
            ::WriteProfileString( "NATC", buf, sBuffer );
         }
         // clear the rest of the entries
         for ( ; index < MAX_UDP_PORTS; index++ )
         {
            sprintf( buf, "SC_UDP_Port%d", index );
            ::WriteProfileString( "NATC", buf, NULL );
         }

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_STUNC_START)));
         ::WriteProfileString("NATC", "StuncStart", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_TURNC_START)));
         ::WriteProfileString("NATC", "TurncStart", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_START)));
         ::WriteProfileString("NATC", "IceStart", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_SYNCEVT)));
         ::WriteProfileString("NATC", "syncEvt", sBuffer);

         ::GetDlgItemText(hDlg, IDC_STUN_MAX_WAIT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "StunMaxWait", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_BINDING_ERROR)));
         ::WriteProfileString("NATC", "BindingError", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_SS_ERROR)));
         ::WriteProfileString("NATC", "SharedSecretError", sBuffer);

         ::GetDlgItemText(hDlg, IDC_TURN_SERVER_IPADDR, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnServerAddr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_TURN_SERVER_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnServerPort", sBuffer);

         ::GetDlgItemText(hDlg, IDC_STUN_SERVER_IPADDR, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "StunServerAddr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_STUN_SERVER_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "StunServerPort", sBuffer);

         ::GetDlgItemText(hDlg, IDC_BANDWIDTH, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnBandwidth", sBuffer);

         ::GetDlgItemText(hDlg, IDC_LIFETIME, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnLifetime", sBuffer);

         ::GetDlgItemText(hDlg, IDC_REQD_IPADDR, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnReqdIPAddr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_REQD_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnReqdPort", sBuffer);

         sprintf(sBuffer, "%d", ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_TRANSPORT)));
         ::WriteProfileString("NATC", "TurnReqdTransport", sBuffer);

         sprintf(sBuffer, "%d", ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_ORG_TRANSPORT)));
         ::WriteProfileString("NATC", "TurnOrgTransport", sBuffer);

         sprintf(sBuffer, "%d", ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_PORT_PROPS)));
         ::WriteProfileString("NATC", "TurnPortProps", sBuffer);

         ::GetDlgItemText(hDlg, IDC_ALLOCATE_HDL, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnAllocateHdl", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ALLOCATE_ERR)));
         ::WriteProfileString("NATC", "TurnAllocateErr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_ACTIVE_DEST_IPADDR, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnActiveDestIPAddr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_ACTIVE_DEST_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnActiveDestPort", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ACTIVE_DEST_ERR)));
         ::WriteProfileString("NATC", "TurnActiveDestErr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_CNX_REMOTE_IPADDR, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnCnxRemoteIPAddr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_CNX_REMOTE_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnCnxRemotePort", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_CONNECT_ERR)));
         ::WriteProfileString("NATC", "TurnConnectErr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_SEND_DATA_REMOTE_IPADDR, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnSendDataRemoteIPAddr", sBuffer);

         ::GetDlgItemText(hDlg, IDC_SEND_DATA_REMOTE_PORT, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "TurnSendDataRemotePort", sBuffer);


         // ICE specific configurations
         ::GetDlgItemText(hDlg, IDC_ICE_RTP_PORT1,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceRtpPort1", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_RTP_PORT2,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceRtpPort2", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_RTP_PORT3,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceRtpPort3", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_RTCP_PORT1,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceRtcpPort1", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_RTCP_PORT2,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceRtcpPort2", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_RTCP_PORT3,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceRtcpPort3",sBuffer);

         ::GetDlgItemText(hDlg, IDC_ICE_PREF_LOCAL,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IcePrefLocal", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_PREF_HOST,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IcePrefHost", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_PREF_SRFLX,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IcePrefSrflx", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_PREF_PRFLX,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IcePrefPrflx", sBuffer);
         ::GetDlgItemText(hDlg, IDC_ICE_PREF_RELAY,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IcePrefRelay", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP1)));
         ::WriteProfileString("NATC", "IceBRtp1", sBuffer);
         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP2)));
         ::WriteProfileString("NATC", "IceBRtp2", sBuffer);
         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP3)));
         ::WriteProfileString("NATC", "IceBRtp3", sBuffer);
         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP1)));
         ::WriteProfileString("NATC", "IceBRtcp1", sBuffer);
         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP2)));
         ::WriteProfileString("NATC", "IceBRtcp2", sBuffer);
         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP3)));
         ::WriteProfileString("NATC", "IceBRtcp3", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_PREF_OVERRIDE)));
         ::WriteProfileString("NATC", "IcePrefOverride", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_LITE_MODE)));
         ::WriteProfileString("NATC", "IceLiteMode", sBuffer);

         sprintf(sBuffer, "%d", Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_AGGRESSIVE_NOM)));
         ::WriteProfileString("NATC", "IceAggressiveNom", sBuffer);

         ::GetDlgItemText(hDlg, IDC_ICE_AGENT_ID,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceAgentId", sBuffer);

         ::GetDlgItemText(hDlg, IDC_ICE_MAX_WAIT,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceMaxWait", sBuffer);

         ::GetDlgItemText(hDlg, IDC_ICE_SDP_AGENT_ID,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceSdpAgentId", sBuffer);

         sprintf(sBuffer, "%d", ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_SDP_ROLE)));
         ::WriteProfileString("NATC", "IceSdpRole", sBuffer);

         ::GetDlgItemText(hDlg, IDC_ICE_RSDP_AGENT_ID,sBuffer,sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceRSdpAgentId", sBuffer);

         sprintf(sBuffer, "%d", ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_RSDP_ROLE)));
         ::WriteProfileString("NATC", "IceRSdpRole", sBuffer);

         ::GetDlgItemText(hDlg, IDC_SDP_TEXT1, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceSdpLine1", sBuffer);
         ::GetDlgItemText(hDlg, IDC_SDP_TEXT2, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceSdpLine2", sBuffer);
         ::GetDlgItemText(hDlg, IDC_SDP_TEXT3, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceSdpLine3", sBuffer);
         ::GetDlgItemText(hDlg, IDC_SDP_TEXT4, sBuffer, sizeof(sBuffer));
         ::WriteProfileString("NATC", "IceSdpLine4", sBuffer);

         //TODO gwExit();
         CloseHandle(LogFile);
      }
      break;

      case WM_COMMAND:
      {
         // Startup the Call Client if Start button has been pressed
         if ( HIWORD(wParam) == BN_CLICKED )
         {
            switch( LOWORD(wParam))
            {
               case IDC_UDP_LIST_ADD:
               {
                  // add udp port list
                  ::GetDlgItemText(hDlg, IDC_UDP_ADD, sBuffer, sizeof(sBuffer));
                  index = ListBox_FindStringExact(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), -1, sBuffer);
                  if ((index == LB_ERR) && (sBuffer[0] != '\0'))
                  {
                     if (ListBox_GetCount(GetDlgItem(hDlg, IDC_UDP_PORT_LIST))< MAX_UDP_PORTS )
                     {
                        index = ListBox_AddString(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), sBuffer);
                        ListBox_SetTopIndex(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), index);
                     }
                  }
                  else
                  {
                     if ( sBuffer[0] != '\0' )
                     {
                        MessageBox( hWnd, "Entry already exists", NULL, MB_OK );
                     }
                  }

                  return TRUE;
               }
               break;
               case IDC_UDP_LIST_REMOVE:
               {
                  // Remove UDP Port from list
                  index = ListBox_GetCurSel(GetDlgItem(hDlg, IDC_UDP_PORT_LIST));
                  ListBox_DeleteString(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), index);
                  return TRUE;
               }
               break;
               case IDC_START:
               {
                  /* update global parameters */
                  GetDlgItemText(hDlg, IDC_LOCAL_IPADDR, gLocalHost, NATTRVRSLTEST_ADDR_BUFFER_SIZE);
                  GetDlgItemText(hDlg, IDC_TLS_PORT, sBuffer, sizeof(sBuffer));
                  gTlsPort = atoi( sBuffer );

                  GetDlgItemText(hDlg, IDC_TURN_UDP_PORT, sBuffer, sizeof(sBuffer));
                  gTurnUdpPort = atoi( sBuffer );

                  GetDlgItemText(hDlg, IDC_TURN_TCP_PORT, sBuffer, sizeof(sBuffer));
                  gTurnTcpPort = atoi( sBuffer );

                  GetDlgItemText(hDlg, IDC_TURN_TLS_PORT, sBuffer, sizeof(sBuffer));
                  gTurnTlsPort = atoi( sBuffer );

                  length = ListBox_GetCount( GetDlgItem(hDlg, IDC_UDP_PORT_LIST) );
                  for ( index = 0; index < length; index++ )
                  {
                     ListBox_GetText(GetDlgItem(hDlg, IDC_UDP_PORT_LIST), index, sBuffer);
                     gUdpPort[index] = atoi( sBuffer );
                  }
                  for ( ; index < MAX_UDP_PORTS; index++ )
                  {
                     gUdpPort[index] = 0;
                  }

                  gStuncStart = Button_GetCheck(GetDlgItem(hDlg, IDC_STUNC_START));
                  gTurncStart = Button_GetCheck(GetDlgItem(hDlg, IDC_TURNC_START));
                  gIceStart = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_START));
                  gSyncEvt = Button_GetCheck(GetDlgItem(hDlg, IDC_SYNCEVT));

                  GetDlgItemText(hDlg, IDC_STUN_MAX_WAIT, sBuffer, sizeof(sBuffer));
                  gStunMaxWait = atoi( sBuffer );

                  gStuntest_BindingError = Button_GetCheck(GetDlgItem(hDlg, IDC_BINDING_ERROR));
                  gStuntest_SharedSecretError = Button_GetCheck(GetDlgItem(hDlg, IDC_SS_ERROR));

                  GetDlgItemText(hDlg, IDC_TURN_SERVER_IPADDR, gTurnHost, NATTRVRSLTEST_ADDR_BUFFER_SIZE);
                  GetDlgItemText(hDlg, IDC_TURN_SERVER_PORT, sBuffer, sizeof(sBuffer));
                  gTurnPort = atoi( sBuffer );

                  GetDlgItemText(hDlg, IDC_STUN_SERVER_IPADDR, gStunHost, NATTRVRSLTEST_ADDR_BUFFER_SIZE);
                  GetDlgItemText(hDlg, IDC_STUN_SERVER_PORT, sBuffer, sizeof(sBuffer));
                  gStunPort = atoi( sBuffer );

                  updateAllocateParms(hDlg);
                  updateActiveDestParms(hDlg);
                  updateConnectParms(hDlg);
                  updateSendDataParms(hDlg);

                  // ICE specific parameters
                  updateCndtGatherParms(hDlg);
                  updateSdpEncodeParms(hDlg);
                  updateSdpParseParms(hDlg);

                  nattrvrsltestStart();
               }
               break;
               case IDC_END:
               {
                  nattrvrsltestEnd();
               }
               break;
               case IDC_ICE_DLOG:
               {
                  nattrvrsltestIceDLogSet();
               }
               break;
               case IDC_TEST_SHARED_SECRET_REQ:
               {
                  gSyncEvt = Button_GetCheck(GetDlgItem(hDlg, IDC_SYNCEVT));
                  gStuntest_SharedSecretError = Button_GetCheck(GetDlgItem(hDlg, IDC_SS_ERROR));
                  GetDlgItemText(hDlg, IDC_STUN_MAX_WAIT, sBuffer, sizeof(sBuffer));
                  gStunMaxWait = atoi( sBuffer );
                  nattrvrsltestStuncSharedSecretReq();
               }
               break;
               case IDC_TEST_BINDING_REQ:
               {
                  gSyncEvt = Button_GetCheck(GetDlgItem(hDlg, IDC_SYNCEVT));
                  gStuntest_BindingError = Button_GetCheck(GetDlgItem(hDlg, IDC_BINDING_ERROR));
                  GetDlgItemText(hDlg, IDC_STUN_MAX_WAIT, sBuffer, sizeof(sBuffer));
                  gStunMaxWait = atoi( sBuffer );
                  nattrvrsltestStuncBindingReq();
               }
               break;
               case IDC_TEST_CLEAR_PWD:
               {
                  /* clear username password cache */
                  nattrvrsltestStuncClearUsrPwdCache();
               }
               break;
               case IDC_TEST_SHOW_PWD:
               {
                  /* Show username password cache */
                  nattrvrsltestShowUsrPwdCache();
               }
               break;
               case IDC_TEST_ALLOCATE_REQ:
               {
                  /* Send Allocate Request */
                  updateAllocateParms(hDlg);
                  nattrvrsltestTurncAllocateReq();
               }
               break;
               case IDC_TEST_ACTIVE_DEST_REQ:
               {
                  /* Send Allocate Request */
                  updateActiveDestParms(hDlg);
                  nattrvrsltestTurncActiveDestReq();
               }
               break;
               case IDC_TEST_CONNECT_REQ:
               {
                  /* Send Allocate Request */
                  updateConnectParms(hDlg);
                  nattrvrsltestTurncConnectReq();
               }
               break;
               case IDC_TEST_SEND_DATA_REQ:
               {
                  /* Send Allocate Request */
                  updateSendDataParms(hDlg);
                  nattrvrsltestTurncSendDataReq();
               }
               break;
               case IDC_TEST_DEALLOCATE:
               {
                  updateAllocateParms(hDlg);
                  nattrvrsltestTurncDeallocate();
               }
               break;
               case IDC_TEST_RECV_DATA:
               {
                  nattrvrsltestTurncReceiveData();
               }
               break;
               case IDC_TEST_CONNECT_STATUS_IND:
               {
                  nattrvrsltestTurncConnectStatusInd();
               }
               break;
               case IDC_TEST_CNDT_GATHER:
               {
                  updateCndtGatherParms(hDlg);
                  nattrvrsltestIceGatherCndts();
               }
               break;
               case IDC_TEST_AGENT_CREATE:
               {
                  gIceLiteMode = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_LITE_MODE));
                  gIceAggressiveNom = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_AGGRESSIVE_NOM));
                  nattrvrsltestIcetestCreateAgent();
               }
               break;
               case IDC_TEST_SET_PORT_PARMS:
               {
                  nattrvrsltestIceSetPortParms();
               }
               break;
               case IDC_TEST_AGENT_DELETE:
               {
                  GetDlgItemText(hDlg, IDC_ICE_AGENT_ID, sBuffer, sizeof(sBuffer));
                  gIceAgentId = atoi( sBuffer );
                  nattrvrsltestIceDeleteAgent();
               }
               break;
               case IDC_TEST_AGENT_DUMP:
               {
                  GetDlgItemText(hDlg, IDC_ICE_AGENT_ID, sBuffer, sizeof(sBuffer));
                  gIceAgentId = atoi( sBuffer );
                  nattrvrsltestIceAgentDump();
               }
               break;
               case IDC_TEST_SDP_ENCODE:
               {
                  updateSdpEncodeParms(hDlg);
                  nattrvrsltestIceSdpEncode();
               }
               break;
               case IDC_TEST_SDP_PARSE:
               {
                  updateSdpParseParms(hDlg);
                  nattrvrsltestIceSdpParse();
               }
               break;
               default:
               {
                  // do nothing
               }
            }
         }
      }
      break;

      case WM_TIMER:
         break;

      case WMUSR_LOG:
      {
         WndLog((char *)lParam);
      }
      break;

      default:
      {
         // do nothing
      }
      #if 0
         if ((message >= WMUSR_CALLEVENT) &&
             (message < (WMUSR_CALLEVENT + CCEVT_MAX)))
         {
            callEventProcess ((CCEVT) (message - WMUSR_CALLEVENT),
                              (CCREF) wParam,
                              (CCREASON) lParam);
         }
         break;
      #endif
   }
   return FALSE;
}

/*
*****************************************************************************
** FUNCTION:   Log
**
** PURPOSE:    Create message in log window, with timestamp and post 
**             to msg queue
**
** PARAMETERS: none
**
** RETURNS:    
**
** NOTE:
*****************************************************************************
*/
void Log(char const *format,...)
{
   va_list arglist;
   SYSTEMTIME time;
   char msg[MAXSTR];
   char *buf;

   buf = (char *)malloc(MAXSTR);
   va_start(arglist, format);
   vsprintf(msg, format, arglist);
   GetLocalTime(&time);
   sprintf( buf, "%02d:%02d:%02d.%d  %s", 
            time.wHour, time.wMinute, time.wSecond, time.wMilliseconds/100, 
            msg);
   PostMessage(hMainDlg, WMUSR_LOG, 0, (LPARAM)buf);
}

/*
*****************************************************************************
** FUNCTION:   LogMsg
**
** PURPOSE:    Log multiline message
**
** PARAMETERS: none
**
** RETURNS:    0 - Success
**             1 - Failure
**
** NOTE:
*****************************************************************************
*/
void LogMsg(CHAR *p)
{
   CHAR buf[MAXSTR];
   CHAR  *q;

   // Assumes buf is NULL-terminated, with CR|LF separated lines
   while (*p)
   {
      q = buf;
      while (*p != '\r' && *p != '\n' && *p)  // look for EOL
      {
         if (*p == '%' || *p == '\\')
            *q++ = *p; // escape character
         *q++ = *p++;
      }
      *q = '\0'; 
      Log(buf);
      if (*p && *p++ == '\r' && *p == '\n')         //skip over EOL
         p++;
   }
}

/*
*****************************************************************************
** FUNCTION:   WndLog
**
** PURPOSE:    Display message in log window
**
** PARAMETERS: none
**
** RETURNS:    0 - Success
**             1 - Failure
**
** NOTE:
*****************************************************************************
*/
void WndLog(char *buf)
{
   int index;
   DWORD count;

   index = ListBox_AddString( GetDlgItem(hMainDlg, IDC_LOGGER), buf);
   if (index-- > 5000)
   {
      /* Trim the scroll window */
      ListBox_DeleteString( GetDlgItem(hMainDlg, IDC_LOGGER), 0);
   }
   if (!(GetMenuState( GetMenu(hWnd), ID_LOG_FREEZE, 0) & MF_CHECKED))
   {
      ListBox_SetTopIndex(GetDlgItem(hMainDlg, IDC_LOGGER), index);
   }

   if (LogFile != INVALID_HANDLE_VALUE)
   {
      WriteFile(LogFile, buf, strlen(buf), &count, NULL );
      WriteFile(LogFile, "\r\n", 2, &count, NULL );
   }
   free(buf);
}

/***********************************************************************
*
*  fillReqdTransportDlg -- Fill the Required Transport options in the 
*                          specified Required Transport list box
* 
*  PARAMETERS:
*     hDlg     - dialog box
*     listId   - IDC id
*     dflt     - default choice
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void fillReqdTransportDlg( HWND hDlg, int listId, int dflt )
{
   int i = 0;
   while ( mapReqdTransport[i] )
   {
      ComboBox_InsertString(GetDlgItem (hDlg, listId), -1, mapReqdTransport[i]);
      i++;
   }
   
   ComboBox_SetCurSel (GetDlgItem (hDlg, IDC_TRANSPORT), dflt);
}

/***********************************************************************
*
*  fillOriginatingTransportDlg -- Fill the Originating Transport options 
*                                in the specified Required Transport 
*                                list box
* 
*  PARAMETERS:
*     hDlg     - dialog box
*     listId   - IDC id
*     dflt     - default choice
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void fillOrgTransportDlg( HWND hDlg, int listId, int dflt )
{
   int i = 0;
   while ( mapOriginatingTransport[i] )
   {
      ComboBox_InsertString(GetDlgItem (hDlg, listId), -1, mapOriginatingTransport[i]);
      i++;
   }
   
   ComboBox_SetCurSel (GetDlgItem (hDlg, IDC_ORG_TRANSPORT), dflt);
}


/***********************************************************************
*
*  fillPortPropsDlg -- Fill the Port Props options in the 
*                          specified list box
* 
*  PARAMETERS:
*     hDlg     - dialog box
*     listId   - IDC id
*     dflt     - default choice
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void fillPortPropsDlg( HWND hDlg, int listId, int dflt )
{
   int i = 0;
   while ( mapPortProps[i] )
   {
      ComboBox_InsertString(GetDlgItem (hDlg, listId), -1, mapPortProps[i]);
      i++;
   }
   
   ComboBox_SetCurSel (GetDlgItem (hDlg, IDC_PORT_PROPS), dflt);
}

/***********************************************************************
*
*  fillSdpRoleDlg -- Fill the SDP Role in the specified list box
* 
*  PARAMETERS:
*     hDlg     - dialog box
*     listId   - IDC id
*     dflt     - default choice
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void fillSdpRoleDlg( HWND hDlg, int listId, int dflt )
{
   int i = 0;
   while ( mapSdpRole[i] )
   {
      ComboBox_InsertString(GetDlgItem (hDlg, listId), -1, mapSdpRole[i]);
      i++;
   }
   
   ComboBox_SetCurSel (GetDlgItem (hDlg, listId), dflt);
}

/***********************************************************************
*
*  updateAllocateParms -- Update the globals used in ALLOCATE request.
* 
*  PARAMETERS:
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void updateAllocateParms(HWND hDlg)
{
   char sBuffer[50];
   GetDlgItemText(hDlg, IDC_BANDWIDTH, sBuffer, sizeof(sBuffer));
   gBandwidth = atoi( sBuffer );

   GetDlgItemText(hDlg, IDC_LIFETIME, sBuffer, sizeof(sBuffer));
   gLifetime = atoi( sBuffer );

   GetDlgItemText(hDlg, IDC_REQD_IPADDR, gReqdIPAddr, NATTRVRSLTEST_ADDR_BUFFER_SIZE);
   GetDlgItemText(hDlg, IDC_REQD_PORT, sBuffer, sizeof(sBuffer));
   gReqdPort = atoi( sBuffer );

   gReqdTransport = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_TRANSPORT)) - 1;

   gPortProps = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_PORT_PROPS)) - 1;

   gTurnOrgTransport = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_ORG_TRANSPORT));
   GetDlgItemText(hDlg, IDC_ALLOCATE_HDL, sBuffer, sizeof(sBuffer));
   gAllocateHdl = atoi( sBuffer );

   gTurnctest_AllocateError = Button_GetCheck(GetDlgItem(hDlg, IDC_ALLOCATE_ERR));
}

/***********************************************************************
*
*  updateActiveDestParms -- Update the globals used in Active Dest request.
* 
*  PARAMETERS:
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void updateActiveDestParms( HWND hDlg )
{
   char sBuffer[50];
   gTurnOrgTransport = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_ORG_TRANSPORT));
   GetDlgItemText(hDlg, IDC_ACTIVE_DEST_IPADDR, gActiveDestIPAddr, NATTRVRSLTEST_ADDR_BUFFER_SIZE);

   GetDlgItemText(hDlg, IDC_ACTIVE_DEST_PORT, sBuffer, sizeof(sBuffer));
   gActiveDestPort = atoi( sBuffer );

   gTurnctest_ActiveDestError = Button_GetCheck(GetDlgItem(hDlg, IDC_ACTIVE_DEST_ERR));

}

/***********************************************************************
*
*  updateConnectParms -- Update the globals used in CONNECT request.
* 
*  PARAMETERS:
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void updateConnectParms( HWND hDlg )
{
   char sBuffer[50];
   gTurnOrgTransport = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_ORG_TRANSPORT));
   GetDlgItemText(hDlg, IDC_CNX_REMOTE_IPADDR, gCnxRemoteIPAddr, NATTRVRSLTEST_ADDR_BUFFER_SIZE);

   GetDlgItemText(hDlg, IDC_CNX_REMOTE_PORT, sBuffer, sizeof(sBuffer));
   gCnxRemotePort = atoi( sBuffer );

   gTurnctest_ConnectError = Button_GetCheck(GetDlgItem(hDlg, IDC_CONNECT_ERR));
}

/***********************************************************************
*
*  updateSendDataParms -- Update the globals used in SEND DATA request.
* 
*  PARAMETERS:
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void updateSendDataParms( HWND hDlg )
{
   char sBuffer[50];
   gTurnOrgTransport = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_ORG_TRANSPORT));
   GetDlgItemText(hDlg, IDC_SEND_DATA_REMOTE_IPADDR, gSendDataRemoteIPAddr, NATTRVRSLTEST_ADDR_BUFFER_SIZE);

   GetDlgItemText(hDlg, IDC_SEND_DATA_REMOTE_PORT, sBuffer, sizeof(sBuffer));
   gSendDataRemotePort = atoi( sBuffer );

}

/***********************************************************************
*
*  updateCndtGatherParms -- Update the globals used in Candidate Gathering 
*                         request
* 
*  PARAMETERS:
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void updateCndtGatherParms(HWND hDlg)
{
   char sBuffer[50];
   // ICE specific parameters
   GetDlgItemText(hDlg, IDC_ICE_RTP_PORT1, sBuffer, sizeof(sBuffer));
   gIceRtpPort[0] = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_RTP_PORT2, sBuffer, sizeof(sBuffer));
   gIceRtpPort[1] = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_RTP_PORT3, sBuffer, sizeof(sBuffer));
   gIceRtpPort[2] = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_RTCP_PORT1, sBuffer, sizeof(sBuffer));
   gIceRtcpPort[0] = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_RTCP_PORT2, sBuffer, sizeof(sBuffer));
   gIceRtcpPort[1] = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_RTCP_PORT3, sBuffer, sizeof(sBuffer));
   gIceRtcpPort[2] = atoi( sBuffer );

   GetDlgItemText(hDlg, IDC_ICE_PREF_LOCAL, sBuffer, sizeof(sBuffer));
   gIcePrefLocal = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_PREF_HOST, sBuffer, sizeof(sBuffer));
   gIcePrefHost = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_PREF_SRFLX, sBuffer, sizeof(sBuffer));
   gIcePrefSrflx = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_PREF_PRFLX, sBuffer, sizeof(sBuffer));
   gIcePrefPrflx = atoi( sBuffer );
   GetDlgItemText(hDlg, IDC_ICE_PREF_RELAY, sBuffer, sizeof(sBuffer));
   gIcePrefRelay = atoi( sBuffer );

   gIceBRtp[0] = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP1));
   gIceBRtp[1] = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP2));
   gIceBRtp[2] = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTP3));
   gIceBRtcp[0] = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP1));
   gIceBRtcp[1] = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP2));
   gIceBRtcp[2] = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_B_RTCP3));

   gIcePrefOverride = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_PREF_OVERRIDE));
   gIceLiteMode = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_LITE_MODE));
   gIceAggressiveNom = Button_GetCheck(GetDlgItem(hDlg, IDC_ICE_AGGRESSIVE_NOM));

   GetDlgItemText(hDlg, IDC_ICE_MAX_WAIT, sBuffer, sizeof(sBuffer));
   gIceMaxWait = atoi( sBuffer );

   GetDlgItemText(hDlg, IDC_ICE_AGENT_ID, sBuffer, sizeof(sBuffer));
   gIceAgentId = atoi( sBuffer );
   gStuntest_BindingError = Button_GetCheck(GetDlgItem(hDlg, IDC_BINDING_ERROR));
   gStuntest_SharedSecretError = Button_GetCheck(GetDlgItem(hDlg, IDC_SS_ERROR));
}

/***********************************************************************
*
*  updateSdpEncode -- Update the globals used in Sdp encoding
*                         request
* 
*  PARAMETERS:
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void updateSdpEncodeParms(HWND hDlg)
{
   char sBuffer[50];
   // ICE specific parameters
   GetDlgItemText(hDlg, IDC_ICE_SDP_AGENT_ID, sBuffer, sizeof(sBuffer));
   gIceSdpAgentId = atoi( sBuffer );
   gIceSdpRole = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_SDP_ROLE));
}

/***********************************************************************
*
*  updateSdpParse -- Update the globals used in Sdp parsing
*                         request
* 
*  PARAMETERS:
* 
*  RETURNS:
*     None
*
*  NOTE:
*
***********************************************************************/
static void updateSdpParseParms(HWND hDlg)
{
   char sBuffer[250];
   // ICE specific parameters
   GetDlgItemText(hDlg, IDC_ICE_RSDP_AGENT_ID, sBuffer, sizeof(sBuffer));
   gIceRSdpAgentId = atoi( sBuffer );
   gIceRSdpRole = ComboBox_GetCurSel(GetDlgItem(hDlg, IDC_RSDP_ROLE));

   GetDlgItemText(hDlg, IDC_SDP_TEXT1, gSdpLine1, NATTRVRSLTEST_ADDR_BUFFER_SIZE);
   GetDlgItemText(hDlg, IDC_SDP_TEXT2, gSdpLine2, NATTRVRSLTEST_ADDR_BUFFER_SIZE);
   GetDlgItemText(hDlg, IDC_SDP_TEXT3, gSdpLine3, NATTRVRSLTEST_ADDR_BUFFER_SIZE);
   GetDlgItemText(hDlg, IDC_SDP_TEXT4, gSdpLine4, NATTRVRSLTEST_ADDR_BUFFER_SIZE);
}
