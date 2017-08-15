/**********************************************************************************
** Copyright © 2009 Broadcom Corporation
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
**      This module implements the Graphical User Interface into the Call Manager
**      and CCTK 2.0 application.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */

#include "stdafx.h"
#include "tcamClient.h"


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

#define WIDTH(rect)              (rect.right - rect.left)
#define HEIGHT(rect)             (rect.bottom - rect.top)

static TCHAR szTitle[MAX_LOADSTRING];
static TCHAR szWindowClass[MAX_LOADSTRING];
static HANDLE LogFile = INVALID_HANDLE_VALUE;

/* ---- Private Variables ------------------------------------------------------- */

HINSTANCE hInst;
HWND hWnd;
HWND hDlg;
HFONT font;

/* ---- Private Function Prototypes --------------------------------------------- */

static ATOM RegisterWndClass(HINSTANCE hInstance);
static BOOL InitInstance(HINSTANCE, int);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK DlgMain(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
extern HWND CreateDlgMain( HWND hParent);
static void WndLog( char *buf);


/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   WinMain
**
** PURPOSE:    Main entry for running the GUI application.  Invoked by the
**             Microsoft framework.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
int APIENTRY WinMain ( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
   MSG msg;
   WNDCLASSEX wcex;

   LoadString( hInstance, 
               IDS_APP_TITLE,
               szTitle,
               MAX_LOADSTRING );

   LoadString( hInstance,
               IDC_APP,
               szWindowClass,
               MAX_LOADSTRING );


   wcex.cbSize = sizeof (WNDCLASSEX);
   wcex.style = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc = (WNDPROC) WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, (LPCTSTR) IDI_ICON1);
   wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
   wcex.lpszMenuName = (LPCSTR) IDC_APP;
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = NULL;

   RegisterClassEx( &wcex );

   if ( !InitInstance( hInstance, nCmdShow ) )
   {
      return FALSE;
   }

   InitCommonControls();

   hDlg = CreateDlgMain(hWnd);

   while ( GetMessage( &msg, NULL, 0, 0 ) )
   {
      if ( !IsDialogMessage( hDlg, &msg ) )
      {
         TranslateMessage( &msg );
         DispatchMessage( &msg );
      }
   }

   return msg.wParam;
}


/***********************************************************************************
** FUNCTION:   InitInstance
**
** PURPOSE:    Initializes the instance and creates the window.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
BOOL InitInstance ( HINSTANCE hInstance, int nCmdShow )
{
   hInst = hInstance;

   hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL,
                        hInstance, NULL );

   if ( !hWnd )
   {
      return FALSE;
   }

   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );

   return TRUE;
}

/***********************************************************************************
** FUNCTION:   WndProc
**
** PURPOSE:    Process windows events.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
   int wmId;
   int wmEvent;

   switch (message)
   {
      case WM_COMMAND:
      {
         wmId = LOWORD(wParam);
         wmEvent = HIWORD(wParam);
         switch (wmId)
         {
            case IDM_EXIT:
            {            
               DestroyWindow(hWnd);
            }
            break;

            case IDM_ABOUT:
            {
               char buf[400];
               sprintf( buf, "Win32 TCAM Application\n"
                             "Date:        %s\n",
                        __DATE__);
               MessageBox( hDlg, buf, "About TCAM", MB_OK|MB_ICONINFORMATION);
            }
            break;

            case IDM_LOGFILE:
            {
               if (GetMenuState( GetMenu(hWnd), IDM_LOGFILE, 0) & MF_CHECKED)
               {
                  CheckMenuItem(GetMenu(hWnd), IDM_LOGFILE, MF_UNCHECKED);
                  CloseHandle(LogFile);
                  LogFile = INVALID_HANDLE_VALUE;
               }
               else
               {
                  CheckMenuItem(GetMenu(hWnd), IDM_LOGFILE, MF_CHECKED);
                  LogFile = CreateFile("tcamLog.txt", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
                     FILE_ATTRIBUTE_NORMAL, NULL);
                  SetFilePointer(LogFile, 0, NULL, FILE_END);
               }
            }           
            break;

            case IDM_LOGFREEZE:
            {            
               if (GetMenuState( GetMenu(hWnd), IDM_LOGFREEZE, 0) & MF_CHECKED)
               {
                  CheckMenuItem(GetMenu(hWnd), IDM_LOGFREEZE, MF_UNCHECKED);
               }
               else
               {
                  CheckMenuItem(GetMenu(hWnd), IDM_LOGFREEZE, MF_CHECKED);
               }
            }
            break;

            case IDM_LOGCLEAR:
            {            
               ListBox_ResetContent(GetDlgItem(hDlg, IDC_LOGGER));
            }            
            break;

            default:
            {
               if (!appMenu(wmId))
               {
                   return DefWindowProc(hWnd, message, wParam, lParam);
               }
            }
            break;
         }
      }
      break;

      case WM_SIZE:
      {      
         SetWindowPos(hDlg, NULL, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_NOMOVE|SWP_NOZORDER);
      }      
      break;

      case WM_ACTIVATE:
      {      
         if (LOWORD(wParam) != WA_INACTIVE)
         {
            SetFocus (hDlg);
         }
      }
      break;

      case WM_DESTROY:
      {      
         PostQuitMessage(0);
      }      
      break;

      case WMUSR_LOG:
      {      
         WndLog( (char *)lParam);
      }      
      break;

      default:
      {
         return DefWindowProc(hWnd, message, wParam, lParam);
      }
      break;
   }

   return 0;
}


/***********************************************************************************
** FUNCTION:   CreateDlgMain
**
** PURPOSE:    Creates the main dialog.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
HWND CreateDlgMain( HWND hParent)
{
   HWND hDlg;
   RECT rwin;

   hDlg = CreateDialog( hInst, (LPCTSTR) (IDD_DLGMAIN),
                        hParent, (DLGPROC) DlgMain);

   GetWindowRect(hDlg, &rwin);
   SetWindowPos( hWnd, NULL, 0, 0,
                 WIDTH(rwin) + 4,
                 HEIGHT(rwin) + GetSystemMetrics(SM_CYCAPTION)
                  + GetSystemMetrics(SM_CYMENU) + 4,
                 SWP_NOMOVE|SWP_NOZORDER);

   return hDlg;
}


/***********************************************************************************
** FUNCTION:   DlgMain
**
** PURPOSE:    Main dialog handler.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
LRESULT CALLBACK DlgMain(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   RECT winrect, logRect;
   char sBuffer[128];
   int value;

   switch (message)
   {
      case WM_INITDIALOG:
      {
         font = CreateFont( 
                            12, 0, 0, 0, // height, width, escapement, orientation
                            FW_NORMAL, 0, 0, 0, 
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            DEFAULT_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
                            "Lucida Console");

         value = GetProfileInt( "TCAM", "LogFile", 1 );
         if (value)
         {
            PostMessage(hWnd, WM_COMMAND, IDM_LOGFILE, 0); //Enable logfile at startup
         }
         PostMessage(hDlg, WMUSR_INIT, 0, 0); //finish init after dialog displayed
         return TRUE;
      }
      break;

      case WMUSR_INIT:
      {
         SendMessage( GetDlgItem( hDlg, IDC_LOGGER), WM_SETFONT, (WPARAM) font, 0);
         appInit();
         return TRUE;
      }
      break;
   
      case WM_DESTROY:
      {    
         appExit();
         CloseHandle(LogFile);
         strcpy( sBuffer, 
                 ( GetMenuState( GetMenu(hWnd), IDM_LOGFILE, 0) & MF_CHECKED) ?
                   "1" : "0" );
         WriteProfileString( "TCAM", "LogFile", sBuffer );
      }
      break;

      case WM_SIZE:
      {
		   GetWindowRect(hDlg, &winrect);
		   GetWindowRect(GetDlgItem(hDlg, IDC_LOGGER), &logRect);
		   SetWindowPos( GetDlgItem(hDlg, IDC_LOGGER), NULL, 
			              0, 0, LOWORD(lParam)-15,
                       HIWORD(lParam)-(logRect.top - winrect.top)-10,
                       SWP_NOMOVE|SWP_NOZORDER );
      }
      break;

      case WM_COMMAND:
      {
         if (appButton( LOWORD(wParam), HIWORD(wParam)))
         {         
            return TRUE;
         }
      }
      break;

      case WM_TIMER:
      {   
      }
      break;

      case WM_LBUTTONDBLCLK:
      {
      }
      break;

      default:
      {
      }
      break;
   }

   return FALSE;
}


/***********************************************************************************
** FUNCTION:   Log
**
** PURPOSE:    Log message.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void Log( char const *format, ...)
{
   va_list arglist;
   SYSTEMTIME time;
   char msg[MAXSTR];
   char *buf = (char *)malloc(1500);

   va_start(arglist, format);
   vsprintf(msg, format, arglist);
   GetLocalTime(&time);
   sprintf(buf, "%02d:%02d:%02d.%d  %s",
      time.wHour, time.wMinute, time.wSecond, (time.wMilliseconds+50)%1000/100, msg);

   PostMessage(hWnd, WMUSR_LOG, 0, (LPARAM) buf);
}


/***********************************************************************************
** FUNCTION:   WndLog
**
** PURPOSE:    Log a message on the Windows window.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void WndLog( char *buf)
{
   int index;
   DWORD count;

   index = ListBox_AddString( GetDlgItem(hDlg, IDC_LOGGER), buf);
   if (index > 3500)
   {
      /* Windows can eventually run out of memory if we don't trim 
       * the scroll window occasionally 
       */
      while (index-- > 3000)
      {
         ListBox_DeleteString( GetDlgItem(hDlg, IDC_LOGGER), 0);
      }
   }
   
   if (!(GetMenuState( GetMenu(hWnd), IDM_LOGFREEZE, 0) & MF_CHECKED))
   {
      ListBox_SetTopIndex(GetDlgItem(hDlg, IDC_LOGGER), index);
   }

   if (LogFile != INVALID_HANDLE_VALUE)
   {
      WriteFile(LogFile, buf, strlen(buf), &count, NULL );
      WriteFile(LogFile, "\r\n", 2, &count, NULL );
   }
   free( buf);
}


/***********************************************************************************
** FUNCTION:   LogMsg
**
** PURPOSE:    Format a message being logged on the buffer for readability.
**
** PARAMETERS: [...]
**
** RETURNS:    None.
***********************************************************************************/
void LogMsg(CHAR *p)
{
   CHAR buf[1500];
   CHAR  *q;

   // Assumes buf is NULL-terminated, with CR|LF separated lines
   while (*p)
   {
      q = buf;
      while (*p != '\r' && *p != '\n' && *p)  // look for EOL
      {
         if (*p == '%' || *p == '\\')
         {
            *q++ = *p; // escape character
         }
         *q++ = *p++;
      }
      *q = '\0';
      Log(buf);
      if (*p && *p++ == '\r' && *p == '\n')        //skip over EOL
      {      
         p++;
      }
   }
}

