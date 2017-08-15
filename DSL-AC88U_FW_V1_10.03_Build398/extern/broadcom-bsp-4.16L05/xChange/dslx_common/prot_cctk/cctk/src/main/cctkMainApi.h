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
**      This file defines the CCTK Main internal API
**
***********************************************************************************/
#ifndef __CCTKMAIN_API__H__INCLUDED__
#define __CCTKMAIN_API__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */

/* ---- Variable Externs -------------------------------------------------------- */
#if ( CCTKCFG_TRACE_MODE != CCTKCFG_TRACE_DISABLED )
extern CCTKTRACEHDLR gpCctkTraceCb;
#endif
#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
extern STraceNode    gCctkTraceNode;
#endif

extern CCTKEVTCB     gCctkEvtCb;

/* ---- Constants and Types ----------------------------------------------------- */
typedef struct TKMSUBINFO
{
   BOS_UINT32 uGuid;
   CCTKSUBEVT eSubType;
   BOS_UINT32 uSubTimeout;
   BOS_UINT8  *pcSubAddr;

} TKMSUBINFO;

typedef struct TKMPUBINFO
{
   BOS_UINT32 uGuid;
   CCTKPUBLISH ePubType;
   BOS_UINT8  *pcSubAddr;

} TKMPUBINFO;

typedef struct TKMMSGINFO
{
   BOS_UINT32 uGuid;
   BOS_UINT8  *pcMsgAddr;
   BOS_UINT8  *pcMsgBody;
   BOS_UINT32 uSize;

} TKMMSGINFO;

typedef struct TKMDTMFINFO
{
   BOS_UINT32 uDtmf;
   BOS_UINT32 uDuration;

} TKMDTMFINFO;

typedef struct TKMFLASHINFO
{
   BOS_UINT8 *pcHfMimeType;
   BOS_UINT8 *pcHfContent;

} TKMFLASHINFO;

typedef enum
{
   eTKMTRANSPORT_DATAGRAM,
   eTKMTRANSPORT_STREAM,
   eTKMTRANSPORT_MAX,
   
} TKMTRANSPORT;


/* Function message ID */
typedef enum TKM_MSGID
{
   /* Asynchronous CCTK API functions */
   eTKM_MSGID_ASYNCFUNC_START,
   eTKM_MSGID_RESET = eTKM_MSGID_ASYNCFUNC_START,
   eTKM_MSGID_CONFIG,
   eTKM_MSGID_SETPARM,
#if CCTKCFG_MEMORY_MONITORING && CCTKCFG_DEBUG
   eTKM_MSGID_SHOWMEMSTATUS,
#endif
#if CCTKCFG_APPLICATION_STATUS
   eTKM_MSGID_SHOWSTATUS,
#endif
   eTKM_MSGID_USERADD,
   eTKM_MSGID_USERREGISTER,
   eTKM_MSGID_USERUNREGISTER,
   eTKM_MSGID_USERREMOVE,
   eTKM_MSGID_EVENTCREATE,
   eTKM_MSGID_EVENTSUBSCRIBE,
   eTKM_MSGID_EVENTUNSUBSCRIBE,
   eTKM_MSGID_EVENTDELETE,
   eTKM_MSGID_CALLCREATE,
   eTKM_MSGID_CALLMAKE,
   eTKM_MSGID_CALLANSWER,
   eTKM_MSGID_CALLFORWARD,
   eTKM_MSGID_CALLHOLD,
   eTKM_MSGID_CALLUNHOLD,
   eTKM_MSGID_CALLTRANSFER,
   eTKM_MSGID_CALLTRANSFERBLIND,
   eTKM_MSGID_CALLMEDIANOTIFY,
   eTKM_MSGID_CALLDELETE,
   eTKM_MSGID_CALLDTMFINFO,
   eTKM_MSGID_CALLFLASHINFO,
   eTKM_MSGID_CALLPRIVINFO,
#if CCTKCFG_REMOTE_USER_CONTROL
   eTKM_MSGID_CALLPROXY,
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
#if CCTKCFG_MESSAGE_PASS_THROUGH
   eTKM_MSGID_MSGPASSTHRU,
   eTKM_MSGID_MSGPASSTHRURSP,
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */
   eTKM_MSGID_KA_SUCCESS,
   eTKM_MSGID_KA_FAILURE,
   eTKM_MSGID_INFOPUBLISH,
   eTKM_MSGID_SVCCTRL,
   eTKM_MSGID_ASYNCFUNC_END = eTKM_MSGID_SVCCTRL,

   /* Synchronous CCTK API functions */
   eTKM_MSGID_SYNCFUNC_START,    
   eTKM_MSGID_GETPARM = eTKM_MSGID_SYNCFUNC_START,
   eTKM_MSGID_SYNCFUNC_END = eTKM_MSGID_GETPARM,

   eTKM_MSGID_MAX
} TKM_MSGID;

/* Message callback */
typedef void ( *TKMMSGCB )      ( BOS_UINT32 uId, void *pData );

/* String conversion macro */
#define CCTK_STR( pcStr )     reinterpret_cast< const BOS_UINT8 * >( pcStr )

/*
** Event callback macro.
*/
#define CCTK_EVTCB( cctkEvtCbParms )                                               \
                              if ( gCctkEvtCb != NULL )                            \
                              {                                                    \
                                 gCctkEvtCb cctkEvtCbParms;                        \
                              }

/* Assert */
#if ( CCTKCFG_ASSERT_MODE == CCTKCFG_ASSERT_ALL )
#define CCTK_ASSERT( expr )   MX_ASSERT( expr )
#elif ( CCTKCFG_ASSERT_MODE == CCTKCFG_ASSERT_CCTK )
#define CCTK_ASSERT( expr )   assert( expr )
#else /* CCTKCFG_ASSERT_MODE == CCTKCFG_ASSERT_DISABLED */
#define CCTK_ASSERT( expr )
#endif

/* 
** Trace functions 
*/
/* CCTK trace message identifier */
#define CCTK_TRACE_ID   1

/* Informational */
#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )

   #define CCTK_TRACEI0( fmt )                                                     \
                        MX_TRACE2( CCTK_TRACE_ID, gCctkTraceNode, fmt )
   #define CCTK_TRACEI1( fmt, a )                                                  \
                        MX_TRACE2( CCTK_TRACE_ID, gCctkTraceNode, fmt, a )
   #define CCTK_TRACEI2( fmt, a, b )                                               \
                        MX_TRACE2( CCTK_TRACE_ID, gCctkTraceNode, fmt, a, b )
   #define CCTK_TRACEI3( fmt, a, b, c )                                            \
                        MX_TRACE2( CCTK_TRACE_ID, gCctkTraceNode, fmt, a, b, c )
   #define CCTK_TRACEI4( fmt, a, b, c, d )                                         \
                        MX_TRACE2( CCTK_TRACE_ID, gCctkTraceNode, fmt, a, b, c, d )

#elif ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_CCTK )

   #define CCTK_TRACEI0( fmt )                                                     \
                        tkmTrace( CCTK_TRACELVL_INFO, fmt )
   #define CCTK_TRACEI1( fmt, a )                                                  \
                        tkmTrace( CCTK_TRACELVL_INFO, fmt, a )
   #define CCTK_TRACEI2( fmt, a, b )                                               \
                        tkmTrace( CCTK_TRACELVL_INFO, fmt, a, b )
   #define CCTK_TRACEI3( fmt, a, b, c )                                            \
                        tkmTrace( CCTK_TRACELVL_INFO, fmt, a, b, c )
   #define CCTK_TRACEI4( fmt, a, b, c, d )                                         \
                        tkmTrace( CCTK_TRACELVL_INFO, fmt, a, b, c, d )

#else /* CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_DISABLED */

   #define CCTK_TRACEI0( fmt )
   #define CCTK_TRACEI1( fmt, a )
   #define CCTK_TRACEI2( fmt, a, b )
   #define CCTK_TRACEI3( fmt, a, b, c )
   #define CCTK_TRACEI4( fmt, a, b, c, d )

#endif

/* Debug */
#if !CCTKCFG_DEBUG || ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_DISABLED )

   #define CCTK_TRACED0( fmt )
   #define CCTK_TRACED1( fmt, a )
   #define CCTK_TRACED2( fmt, a, b )
   #define CCTK_TRACED3( fmt, a, b, c )
   #define CCTK_TRACED4( fmt, a, b, c, d )

#elif ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )

   #define CCTK_TRACED0( fmt )                                                     \
                        MX_TRACE7( CCTK_TRACE_ID, gCctkTraceNode, fmt )
   #define CCTK_TRACED1( fmt, a )                                                  \
                        MX_TRACE7( CCTK_TRACE_ID, gCctkTraceNode, fmt, a )
   #define CCTK_TRACED2( fmt, a, b )                                               \
                        MX_TRACE7( CCTK_TRACE_ID, gCctkTraceNode, fmt, a, b )
   #define CCTK_TRACED3( fmt, a, b, c )                                            \
                        MX_TRACE7( CCTK_TRACE_ID, gCctkTraceNode, fmt, a, b, c )
   #define CCTK_TRACED4( fmt, a, b, c, d )                                         \
                        MX_TRACE7( CCTK_TRACE_ID, gCctkTraceNode, fmt, a, b, c, d )

#else /* CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_CCTK */

   #define CCTK_TRACED0( fmt )                                                     \
                        tkmTrace( CCTK_TRACELVL_DEBUG, fmt )
   #define CCTK_TRACED1( fmt, a )                                                  \
                        tkmTrace( CCTK_TRACELVL_DEBUG, fmt, a )
   #define CCTK_TRACED2( fmt, a, b )                                               \
                        tkmTrace( CCTK_TRACELVL_DEBUG, fmt, a, b )
   #define CCTK_TRACED3( fmt, a, b, c )                                            \
                        tkmTrace( CCTK_TRACELVL_DEBUG, fmt, a, b, c )
   #define CCTK_TRACED4( fmt, a, b, c, d )                                         \
                        tkmTrace( CCTK_TRACELVL_DEBUG, fmt, a, b, c, d )

#endif


/* ---- Function Prototypes ----------------------------------------------------- */

CCTKSTATUS tkmEvent( BOS_UINT32 uUid, BOS_UINT32 uCid, CCTKEVT eEvt, void *pData );
BOS_UINT32 tkmGenerateId( void );
CCTKSTATUS tkmMsgPost( BOS_BOOL bWait,
                       BOS_UINT32 uId,
                       void *pData,
                       TKMMSGCB msgCb );
CCTKSTATUS tkmSetParm( BOS_UINT32 uId, CCTKPARM eParm, const void *pVal );
CCTKSTATUS tkmTimerStart( BOS_UINT32 *pId,
                          BOS_UINT32 uMsec,
                          BOS_BOOL bRecurring,
                          void *pData,
                          TKMMSGCB tmrCb );
CCTKSTATUS tkmTimerStop( BOS_UINT32 uId );
CCTKSTATUS tkmKaFailure( BOS_UINT32 uUid );
CCTKSTATUS tkmKaSuccess( BOS_UINT32 uUid, TKMTRANSPORT eType, BOS_UINT8 *puAddr );
void tkmResetCont( void );

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_CCTK )
void       tkmTrace( BOS_UINT16 uLevel, const char *pFormat, ... );
#endif

#endif /* __CCTKMAIN_API__H__INCLUDED__ */
