/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom
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
*    Filename:pstnCtl.h
*
****************************************************************************
*    Description:
*
*     Public interface for PSTN call setup control module.
*
****************************************************************************/


#ifndef PSTNCTL_H
#define PSTNCTL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>         /* Standard types */
#include <hapi.h>             /* HAPI interface */
#include <bosMsgQ.h>          /* Message queue interface */
#include <bosTask.h>          /* Task interface */
#include <hapi_pte.h>         /* PTE Service */
#include <endpt.h>            /*  */
#include <hdspClass.h>        /* Class Interface for DTMF CLID parms */
#include <memCircBuf.h>       /* Circular buffer API */


/* ---- Constants and Types ---------------------------------------------- */

/* Maximum number of PSTN control endpoints */
#define PSTN_CTL_MAX_CHANNELS    4
/* number of PSTN channels */
#define PSTN_CTL_FXO_CHANNEL     PSTN_CTL_MAX_RELAY
/* number of RELAY controls FXS voice channels */
#define PSTN_CTL_MAX_RELAY    2

#define PSTN_CTL_DO_NOT_SIGNAL_LINE           0xBEBE
#define PSTN_CTL_3_WAY_CONFERENCE             0xBEEF
#define PSTN_CTL_DO_NOT_DISCONNECT_PRIMARY    0xBAAD

/* Task priority for PSTN_CTL */
#ifndef PSTN_CTL_CFG_TASK_PRIORITY
#define PSTN_CTL_CFG_TASK_PRIORITY  BOS_TASK_CLASS_LOW
#endif

/* The PSTN_CTL task will run every 100ms */
#ifndef PSTN_CTL_CFG_TASK_RATE_MS
#define PSTN_CTL_CFG_TASK_RATE_MS   50
#endif

/* Special PSTN_CTL dial strings */
#define PSTN_CONNECTONLY_DLSTRNG    "BYPASS_CMTD_DSGEN"  /* Bypasses dialtone detection, dialstring generation */

/* Wait period for dial tone detection */
#define PSTN_CPTONE_TIMEOUT_PERIOD  5000 /* ms */

/* Maximum number of digits that can be dialed */
#define PSTN_CTL_MAX_DIALSTRING_LEN    HAPI_PTE_GEN_MAX_DIGITS

/* DTMF CLID Rx service buffer size defines */
#define DTMFCLIDRX_MAX_BUFFER_SIZE 80
#define DTMFCLIDRX_MAX_NUMBER_SIZE 20

/* Internal events for DTMF CLID Rx Service (timeouts etc..) */
#define DTMFCLIDRX_EVT_NO_INTERNAL_EVENTS       0x0000
#define DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT       ( 1 << 0 )
#define DTMFCLIDRX_EVT_SESSION_TIMEOUT          ( 1 << 1 )
#define DTMFCLIDRX_EVT_RINGAFTERCLID_TIMEOUT    ( 1 << 2 )
#define DTMFCLIDRX_EVT_DTMF_COLLECT_COMPLETE    ( 1 << 3 )
#define DTMFCLIDRX_EVT_PARTIAL_DTMF_CLID        ( 1 << 4 )
#define DTMFCLIDRX_EVT_VALID_DTMF_CLID          ( 1 << 5 )
#define DTMFCLIDRX_EVT_INVALID_DTMF_CLID        ( 1 << 6 )
#define DTMFCLIDRX_EVT_NEEDRESET                ( 1 << 7 )
#define DTMFCLIDRX_EVT_ERROR                    ( 1 << 8 )
#define DTMFCLIDRX_EVT_DTMF_PARSE_ERROR         ( 1 << 9 )

/* DTMFClidDestroy special all handle */
#define PSTN_CTL_DTMFDESTROY_ALL_HANDLE         0xFF

/* PSTN control handle */
typedef VRG_UINT16   PSTN_CTL_HANDLE;

/* PSTN_CTL Event Structure. Derived from HAUS */
typedef HSZEVT PSTN_CTL_EVT;

/* PSTN CTL DTMF CLID Parms Structure. Derived from HDSP */
typedef CLASS_MSG_DTMF_PARMS PSTN_CTL_DTMF_CLID_PARMS;

/* PSTN State machine states */
typedef enum
{
   PSTN_CTL_CHAN_STATE_IDLE,
   PSTN_CTL_CHAN_STATE_WAIT_DIAL_TONE,
   PSTN_CTL_CHAN_STATE_GENERATE_DTMF
} PSTN_CTL_CHAN_STATE;

/* PSTN State Machine Events */
typedef enum
{
   PSTN_CTL_EVT_CALL_SETUP,
   PSTN_CTL_EVT_CPTONE_DETECT,
   PSTN_CTL_EVT_CPTONE_TIMEOUT,
   PSTN_CTL_EVT_DIAL_COMPLETE,
   PSTN_CTL_EVT_CALL_SETUP_COMPLETE,
   PSTN_CTL_EVT_OFFHOOK,
   PSTN_CTL_EVT_ONHOOK,
   PSTN_CTL_EVT_RINGON,
   PSTN_CTL_EVT_RINGOFF,   
   PSTN_CTL_EVT_RINGEND,
   PSTN_CTL_EVT_DTMF_DIGIT,
   PSTN_CTL_EVT_VALID_DTMFCLID,
   PSTN_CTL_EVT_PARTIAL_DTMFCLID,
   PSTN_CTL_EVT_INVALID_DTMFCLID,
   PSTN_CTL_EVT_ANSWER,
   PSTN_CTL_EVT_NONE,

} PSTN_CTL_EVENT;

/* Event structure for the PSTN_CTL callback function */
typedef struct
{
   PSTN_CTL_EVENT   event;       /* event detected */
   void*            pInfo;       /* event info */

} PSTN_CTL_EVENT_INFO;

/* PSTN Event callback type */
typedef void ( *PSTN_CTL_CALLBACK )
(
   VRG_UINT16      vhdHdl,                /* channel VHD */
   PSTN_CTL_EVENT_INFO   *pEventInfo      /* event info */
);

/* DTMF CLID Info reasons. This mirrors the codes we expect for a country, 
   defined in vrgEndptGlobe.c. The two common ones are "private" and 
   "unavailable" for most countries, while "error" is on used in Denmark. */
typedef enum
{
   DTMFCLIDRX_INFO_NONE = 0,          /* No infocode recieved */
   DTMFCLIDRX_INFO_PRIVATE,           /* "Private" */
   DTMFCLIDRX_INFO_UNAVAILABLE,       /* "Unavailable" */
   DTMFCLIDRX_INFO_ERROR,             /* "Error" */

} DTMFCLIDRX_INFO_REASON;

/* DTMF CLID Rx service state machine states */
typedef enum
{
   DTMFCLIDRX_IDLE = 0,         /* Idle service state */
   DTMFCLIDRX_COLLECTING,       /* Collecting DTMF digits service state */
   DTMFCLIDRX_WAITINGFOREND,    /* Waiting for events service state */
   DTMFCLIDRX_MAXSTATES,        /* Maximum states in enum */

} DTMFCLIDRX_SERVICE_STATE;

/* Current ring state of line */
typedef enum
{
   DTMFCLIDRX_NORING = 0,       /* No ringing detected state */
   DTMFCLIDRX_FIRSTRINGON,      /* First ring on detected state */
   DTMFCLIDRX_FIRSTRINGOFF,     /* First ring off detected state */
   DTMFCLIDRX_SECONDRING,       /* Second ring+ detected state */
   DTMFCLIDRX_RINGEND,          /* Ringing ended state */

} DTMFCLIDRX_RING_STATE;

/* CLID type (before or after first ring) */
typedef enum
{
   DTMFCLIDRX_PRERING_CLID = 0,         /* Pre first ring CLID */
   DTMFCLIDRX_FIRSTRING_CLID,           /* First to Second ring CLID */
   DTMFCLIDRX_INVALID_CLID,             /* Invalid CLID */

} DTMFCLIDRX_CLID_TYPE;

/* Timers used for CLID Rx service. This enum if used as the index to
   a timer array, so the timer array can be expanded for any number of
   timers needed. */
typedef enum
{
   DTMFCLIDRX_DTMF_PAUSE_TIMER = 0,   /* Measures pause time between DTMF digit reception */
   DTMFCLIDRX_SESSION_TIMER,          /* Measures session time for DTMF collection */
   DTMFCLIDRX_RINGAFTERCLID_TIMER,
   DTMFCLIDRX_MAX_TIMERS,

} DTMFCLIDRX_TIMER_TYPE;

/* Timer struct, will use the task rate as timing mechanism to
   increment the counter and check against a timeout value */
typedef struct
{
   VRG_UINT16   counter;        /* Incremented counter based on task rate (in ms) */
   VRG_UINT16   timeout;        /* Timeout value (in ms) */
   VRG_BOOL     running;        /* Whether this timer is currently running */
   VRG_UINT16   timeoutEvent;   /* Event to be associated with this timer for timeout */
   DTMFCLIDRX_TIMER_TYPE   type;   /* Timer type, to differentiate timers */

} DTMFCLIDRX_TIMER;

/* CLID Rx service state information (essentially for the statemachine) */
typedef struct
{
   DTMFCLIDRX_SERVICE_STATE   serviceState;   /* Current state of DTMF CLID Rx Service */
   VRG_BOOL                   offHook;        /* Current hook state (TRUE = offhook, FALSE = onhook) */
   DTMFCLIDRX_RING_STATE      ringState;      /* Current ring state of Pstn line */
   DTMFCLIDRX_CLID_TYPE       clidType;       /* The CLID Type (pre-ring, first ring) */
   DTMFCLIDRX_TIMER           timer[DTMFCLIDRX_MAX_TIMERS];   /* Array of timers for the service */

} DTMFCLIDRX_STATE_INFO;

/* Final CLID Data stored after parsing */
typedef struct
{
   char   callingNumber[DTMFCLIDRX_MAX_NUMBER_SIZE + 1];       /* Calling Number collected */
   char   redirectingNumber[DTMFCLIDRX_MAX_NUMBER_SIZE + 1];   /* Redirecting Number collected */
   DTMFCLIDRX_INFO_REASON   infoRecieved;                      /* Reason field if number unavailable */
   char   clidString[DTMFCLIDRX_MAX_BUFFER_SIZE + 1];          /* Full CLID string detected (in char form) */

} DTMFCLIDRX_PARSED_DATA;

/* CLID collection and parsing struct */
typedef struct
{
   char                     collectBuffer[DTMFCLIDRX_MAX_BUFFER_SIZE + 2];   /* Collection buffer */
   VRG_UINT16               collectBufLen;      /* Length of collected info */
   DTMFCLIDRX_PARSED_DATA   parsedData;         /* parsed data storage */

} DTMFCLIDRX_CLID_INFO;

/* Event info structure for CLID Rx service */
typedef struct
{
   DTMFCLIDRX_PARSED_DATA   clidData;      /* CLID Data to be sent to userspace */
   DTMFCLIDRX_CLID_TYPE     clidType;      /* CLID Type (Pre-ring, first-ring) */

} PSTN_CTL_CLID_EVENT_INFO;

/* CLID Rx service main structure */
typedef struct
{
   VRG_UINT16                 vhdHdl;           /* Handle of Vhd associated with this servive instance */
   VRG_BOOL                   enabled;          /* Shows whether service is enabled */
   DTMFCLIDRX_STATE_INFO      stateInfo;        /* state info of CLID Rx service */
   VRG_BOOL                   eventSent;        /* Tracks whether we sent a CLID event out */
   VRG_UINT16                 internalEvents;   /* For internal events to process (may be a few timeouts) */
   PSTN_CTL_DTMF_CLID_PARMS   *dtmfClidParms;   /* DTMF CLID parms for specific country */
   VRG_COUNTRY                country;          /* Country of operation */
   DTMFCLIDRX_CLID_INFO       clidInfo;         /* DTMF CLID buffers and data */

} PSTN_CTL_DTMFCLIDRX_STATE;

/* PSTN Channel Information */
typedef struct
{
   VRG_UINT16 vhdHdl;              /* channel VHD */
   PSTN_CTL_CHAN_STATE state;      /* current state */
   BOS_TASK_ID tmrTaskId;          /* timer task ID */
   char dialString[ PSTN_CTL_MAX_DIALSTRING_LEN ];   /* digits to be dialed */
   PSTN_CTL_DTMFCLIDRX_STATE dtmfClidRx;             /* DTMF Clid Rx service, per channel */
   unsigned int timeoutCount;

} PSTN_CTL_CHAN_INFO;

/* PSTN_CTL state */
typedef struct
{
   VRG_UINT16        numChannels;   /* Number of initialized channels */
   VRG_COUNTRY       country;       /* Country of operation, for DTMF scheme */
   BOS_TASK_ID       taskId;        /* PSTN task identifier */
   MEMCIRCBUFDRV     evtQ;          /* Task event queue */
   PSTN_CTL_CALLBACK callback;      /* Registered client event callback func */
   PSTN_CTL_CHAN_INFO chanState[ PSTN_CTL_MAX_CHANNELS ];
                                    /* Channel state data */
} PSTN_CTL_STATE;

/* Return codes */
typedef enum
{
   PSTN_CTL_SUCCESS,
   PSTN_CTL_FAILURE
} PSTN_CTL_STATUS;


/* hook status codes */
typedef enum
{
   PSTN_CTL_STAT_OFFHOOK = EPCAS_OFFHOOK,
   PSTN_CTL_STAT_ONHOOK  = EPCAS_ONHOOK
} PSTN_CTL_HOOK_STAT;

/* hook status codes */
typedef enum
{
   PSTN_CTL_RELAY_OFF = 0,
   PSTN_CTL_RELAY_ON = 1
} PSTN_CTL_RELAY_CMD;

/* ---- Function Prototypes ---------------------------------------------- */
PSTN_CTL_STATUS pstnCtlGetHookStatus( PSTN_CTL_HOOK_STAT *status );
PSTN_CTL_STATUS pstnCtlSetRelayStat( PSTN_CTL_RELAY_CMD cmd );

PSTN_CTL_STATUS pstnCtlInit( VRG_COUNTRY country, PSTN_CTL_CALLBACK callback );
PSTN_CTL_STATUS pstnCtlDeinit( void );
PSTN_CTL_STATUS pstnCtlEndptInit( PSTN_CTL_HANDLE vhdHdl );
PSTN_CTL_STATUS pstnCtlSendEvent( PSTN_CTL_EVT *evtp );
PSTN_CTL_STATUS pstnCtlDTMFClidRxInit( VRG_COUNTRY country, PSTN_CTL_HANDLE handle, 
                                       PSTN_CTL_DTMF_CLID_PARMS *dtmfParmsp );
PSTN_CTL_STATUS pstnCtlDTMFClidRxDestroy( PSTN_CTL_HANDLE handle );


#ifdef __cplusplus
}
#endif

#endif      /* PSTNCTL H */
