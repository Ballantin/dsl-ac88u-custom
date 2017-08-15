/***************************************************************************
*
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
* :>
*
****************************************************************************
*
*    Filename: casctl.h
*
****************************************************************************
*    Description:
*
*     Public interface for Channel Associated Signaling (CAS) control module.
*
****************************************************************************/


#ifndef CASCTL_H
#define CASCTL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <xdrvCas.h>
#include <vrgCountry.h>

/* ---- Constants and Types ---------------------------------------------- */

/* Enable PSTN endpoint support */
#define CAS_CTL_PSTN    1

/*
** Default values for CAS timers. These can be used by the client to initialize
** the "CAS_CTL_FXO_PARMS" struct that is used to configure each endpt.
*/

#define CAS_CTL_FXO_WINK_ENABLED      0


/* FXO On-hook parameters */
#if CAS_CTL_FXO_WINK_ENABLED
   #define CAS_CTL_DEFAULT_FXO_WINK_MSEC        90    /* A Wink is between 100-350ms */
   #define CAS_CTL_DEFAULT_FXO_POSTWINK_MSEC    30    /* Debounce 30ms after a wink */
   #define CAS_CTL_DEFAULT_FXO_CONNECT_MSEC     350   /* Connect if > 350 ms */
#else
   #define CAS_CTL_DEFAULT_FXO_WINK_MSEC        200   /* Wink not used */
   #define CAS_CTL_DEFAULT_FXO_POSTWINK_MSEC    200   /* Wink not used */
   #define CAS_CTL_DEFAULT_FXO_CONNECT_MSEC     150   /* Connect if > 150 ms */
#endif
#define CAS_CTL_DEFAULT_FXO_DEGLITCH_MSEC       200   /* Remove glitches caused by L9500 SLIC
                                                       * offhook to onhook transition. */

/* FXO Off-hook parameters */
#define CAS_CTL_DEFAULT_FXO_MIN_FLASH_MSEC      280   /* A Flash is between 300-1100 ms */
#define CAS_CTL_DEFAULT_FXO_MAX_FLASH_MSEC      1100
#define CAS_CTL_DEFAULT_FXO_DISCONNECT_MSEC     1200  /* Disconnect if > 1200 ms */

/* Pulse Dial characteristics */
#define CAS_CTL_DEFAULT_FXO_MINMAKE_MSEC        20
#define CAS_CTL_DEFAULT_FXO_MAXMAKE_MSEC        200
#define CAS_CTL_DEFAULT_FXO_MINBREAK_MSEC       20
#define CAS_CTL_DEFAULT_FXO_MAXBREAK_MSEC       200
#define CAS_CTL_DEFAULT_FXO_INTERDIGIT_MSEC     260

/* Ignore Hookstate */
#define CAS_IGNOREHOOKSTATE_ON                  0xffff  /* Value to ignore hookstate; 
                                                         * ignore hookstate will be disabled 
                                                         * when this duration expires, which prevents
                                                         * endpt being stuck in ignore hookstate mode */
#define CAS_IGNOREHOOKSTATE_OFF                 0       /* Value to disable ignore hookstate */

/* CAS control handle */
typedef VRG_UINT16   CAS_CTL_HANDLE;

typedef enum
{
    CAS_TYPE_INACTIVE,
    CAS_TYPE_FXO,
    CAS_TYPE_FXS,
} CAS_TYPE;


/* CAS Function call results */
typedef enum CAS_CTL_STATUS
{
   CAS_CTL_SUCCESS,
   CAS_CTL_FAILURE
} CAS_CTL_STATUS;


/* CAS OSI Patterns */
typedef enum CAS_OSI_PATTERN
{
   CAS_OSI_VMWI = 0,
   CAS_OSI_DEFAULT
} CAS_OSI_PATTERN;


/*
** Combine country and ring id together into single value.
*/
#define  CAS_CTL_MAKE_RING_PATTERN( country, ringId ) (( (country) << 8 ) | ringId )

/*
** Given a ring pattern, determine which country it identifies.
*/
#define  CAS_CTL_RING_PATTERN_COUNTRY( pattern ) ((VRG_COUNTRY)(( (pattern) >> 8 ) & 0x00FF ))

/*
** Given a ring pattern, determine which ring id it identifies.
*/
#define  CAS_CTL_RING_PATTERN_ID( pattern ) ( (pattern) & 0x00FF )



/*
** Japanese ring profiles ids.
*/
typedef enum CAS_CTL_JAPAN_RING_ID
{
   CAS_CTL_JAPAN_RING_ID_IR = 0,
   CAS_CTL_JAPAN_RING_ID_CAR,

   CAS_CTL_JAPAN_NUM_RING_PATTERNS
} CAS_CTL_JAPAN_RING_ID;



/*
** Cyprus ring profile ids.
*/
typedef enum CAS_CTL_CYPRUS_RING_ID
{
   CAS_CTL_CYPRUS_RING_ID_RG = 0,
   CAS_CTL_CYPRUS_RING_ID_RS,

   CAS_CTL_CYPRUS_NUM_RING_PATTERNS
}  CAS_CTL_CYPRUS_RING_ID;


#define CAS_CTL_MAKE_RING_ENUM( country, id )   CAS_CTL_RING_##country##_##id = \
         CAS_CTL_MAKE_RING_PATTERN( VRG_COUNTRY_##country, CAS_CTL_##country##_RING_ID_##id )

/*
** Ring patterns that are created based upon the countries and ring profile
** ids defined above. These enumerated types should be used to specify a
** specific ring profile when calling 'casCtlRingProfileOn()'. The enumerated
** types will be named 'CAS_CTL_RING_country_id', e.g. 'CAS_CTL_RING_JAPAN_CAR'.
*/
typedef enum CAS_CTL_RING_PATTERN
{

#if VRG_COUNTRY_CFG_JAPAN
   CAS_CTL_MAKE_RING_ENUM( JAPAN, IR  ),
   CAS_CTL_MAKE_RING_ENUM( JAPAN, CAR ),
#endif

#if VRG_COUNTRY_CFG_CYPRUS
   CAS_CTL_MAKE_RING_ENUM( CYPRUS, RG  ),
   CAS_CTL_MAKE_RING_ENUM( CYPRUS, RS  ),
#endif


   /* Added a dummy value in the end*/
   CAS_CTL_RING_MAX
} CAS_CTL_RING_PATTERN;


/*
** These events are the event types that are sent to the client app through
** the supplied callback.
*/
typedef enum CAS_CTL_EVENT_TYPE
{
   /* A detection event has occured. */
   CAS_CTL_DETECT_EVENT,

   /* A state machine control event has occured. */
   CAS_CTL_STATECTL_EVENT,

   /* A Pulse Digit was detected. */
   CAS_CTL_PULSEDIAL_EVENT,

   /* A brief ring completion event has occurred. */
   CAS_CTL_BRIEF_RING_COMPLETE_EVENT

} CAS_CTL_EVENT_TYPE;


/*
** These events are sent to the client app through the supplied callback.
*/
typedef enum
{
   CAS_CTL_EVENT_NONE = 0,       /* No event */
   CAS_CTL_EVENT_ONHOOK,         /* On-hook */
   CAS_CTL_EVENT_OFFHOOK,        /* Off-hook */
   CAS_CTL_EVENT_WINK,           /* Wink */
   CAS_CTL_EVENT_FLASH,          /* Flash */
   CAS_CTL_EVENT_RINGON,         /* Ring On (rising edge of ring envelope) */
   CAS_CTL_EVENT_RINGOFF,        /* Ring Off (fall edge of ring envelope) */
   CAS_CTL_EVENT_FIRST_RINGOFF,  /* First Ring Off (fall edge of ring envelope) */
   CAS_CTL_EVENT_LCF,            /* Loop current feed */
   CAS_CTL_EVENT_RLCF,           /* Reverse loop current feed */
   CAS_CTL_EVENT_LCFO,           /* Loop current off (open-open) */
   CAS_CTL_EVENT_EARLY_OFFHOOK,  /* Early Off-hook */
   CAS_CTL_EVENT_EARLY_ONHOOK,   /* Early On-hook */
   CAS_CTL_EVENT_DELAY_OFFHOOK,  /* Delayed Off-hook */
   CAS_CTL_EVENT_DELAY_ONHOOK,   /* Delayed On-hook */
   CAS_CTL_EVENT_PULSECOMPLETE,  /* Pulse dialstring completed */
   CAS_CTL_EVENT_OSI_END,        /* Open switch interval has ended */
   CAS_CTL_EVENT_TIMER_EXPIRE,   /* Timer expiry */
   CAS_CTL_EVENT_BRIEF_RING,      /* Brief ring completed */
   CAS_CTL_EVENT_RINGEND,        /* Ring ended */

} CAS_CTL_EVENT;


/* CAS Interface States */
typedef enum
{
   CAS_CTL_FXO_SERVICEDENIED = 0,  /* No loop current provided, no hook detection */
   CAS_CTL_FXO_IDLE,               /* On-hook detected(standby loop current provided) */
   CAS_CTL_FXO_ALERTING,           /* On-hook detected(ringing + on-hook tx path) */
   CAS_CTL_FXO_ALERTING_RING = CAS_CTL_FXO_ALERTING,
   CAS_CTL_FXO_ALERTING_OHT,
   CAS_CTL_FXO_ALERTING_OHTR,
   CAS_CTL_FXO_ADDRESSING,         /* Off-hook detected(pulse dial detection enabled) */
   CAS_CTL_FXO_ACTIVE,             /* Off-hook detected(flash detection enabled) */
   CAS_CTL_FXO_JAPAN_ALERTING,               /* On-hook detected(ringing + on-hook tx path) */
   CAS_CTL_FXO_JAPAN_ALERTING_CLID,          /* On-hook detected(ringing + on-hook tx path + CLID) */
   CAS_CTL_FXO_JAPAN_ALERTING_CLID_OFFHOOK   /* Off-hook detected(ringing + off-hook tx path + CLID) */
} CAS_CTL_FXO_INTERFACE_STATE;

typedef enum
{
   CAS_CTL_FXS_RELEASED = 0,         /* loop open */
   CAS_CTL_FXS_SEIZED,               /* loop closed */
   CAS_CTL_FXS_CLIDRX,               /* receive caller ID */
   CAS_CTL_FXS_RING_ON,              /* ring signal on */
} CAS_CTL_FXS_INTERFACE_STATE;

typedef enum CAS_CTL_POLARITY
{
   /* Loop current feed. */
   CAS_CTL_POLARITY_FORWARD,

   /* Reverse loop current feed. */
   CAS_CTL_POLARITY_REVERSE
} CAS_CTL_POLARITY;


/* CAS control callback */
typedef void ( *CAS_CTL_CALLBACK )
(
   CAS_CTL_HANDLE       handle,
   CAS_CTL_EVENT_TYPE   eventType,
   CAS_CTL_EVENT        event
);


/*
** Identify the ring type.  The definition of a ring type depends on the
** duration of the ring cadence.  For the 6-seconds (short) ring duration, the
** ring cadence is defined as a 61-bit entity.  For the 13.2-seconds (long)
** ring duration, the ring cadence is a 288-bits entity.  Refer to
** CAS_CTL_PROV_PATTERN for more details.
*/
typedef enum CAS_CTL_PROV_RING_TYPE
{
   CAS_CTRL_SHORT_BRIEF_RING = 1,   /* PC (provisioned) SigMIB defined ring style - brief duration */
   CAS_CTRL_SHORT_NORMAL_RING,      /* PC (provisioned) SigMIB defined ring style - normal duration */
   CAS_CTRL_LONG_BRIEF_RING,        /* EuroPC provisioned) SigMIB defined ring style - brief duration */
   CAS_CTRL_LONG_NORMAL_RING,       /* EuroPC provisioned) SigMIB defined ring style - normal duration */
   CAS_CTRL_STATIC_RING             /* Non provisioned static ring profile */

} CAS_CTL_PROV_RING_TYPE;

/*
** CAS command request IDs
*/
typedef enum CAS_REQ_ID
{
   CAS_REQ_NONE = 0,
   
   /* SLIC command request IDs */
   CAS_REQ_SLIC_START,
   CAS_REQ_SLIC_BOOTED_LOOP_CURRENT,
   CAS_REQ_SLIC_SET_RING_PARMS,
   CAS_REQ_SLIC_PHASE_REVERSAL_CONTROL,
   CAS_REQ_SLIC_IS_OFFHOOK,
   CAS_REQ_SLIC_SET_POWER_SOURCE,
   CAS_REQ_SLIC_END,
   
   /* DAA command request IDs */
   CAS_REQ_DAA_START,
   CAS_REQ_DAA_MODE_CONTROL,
   CAS_REQ_DAA_GET_HOOK_STATUS,
   CAS_REQ_DAA_END,
   
   /* APM command request IDs */
   CAS_REQ_APM_START,
   CAS_REQ_APM_SET_RING_PARMS,   
   CAS_REQ_APM_PULSE_SIGNAL,   
   CAS_REQ_APM_END,

} CAS_REQ_ID;

/*
** CAS command mode
*/
typedef enum CAS_CMD_MODE
{
   CAS_CMD_MODE_ASYNC,
   CAS_CMD_MODE_SYNC,

} CAS_CMD_MODE;

/*
** Provisioned short ring pattern (6 sec)
**    This is a 64-bit pattern representing a 6 second ring pattern. The first
**    60 MSb represent the ring pattern. A set bit indicates that a ring signal
**    should be applied, a clear bit indicates that no ring signal should be
**    applied. Each bit corresponds to 100 ms. The 61st bit is set to 0 for a
**    repeating pattern, and 1 for a one-shot pattern. The last three bits are
**    reserved for future use. Leading zeros in the pattern are ignored and
**    can be used to shorten the ring pattern length.
**
**    This ring pattern is used by all L-line package ring cadences.
*/
typedef struct CAS_CTL_PROV_SHORT_RING_PATTERN
{
   VRG_UINT32 pattern1;
   VRG_UINT32 pattern2;

} CAS_CTL_PROV_SHORT_RING_PATTERN;

#define  CAS_CTL_PROV_SHORT_RING_MAX_INTERVAL        60


/*
** Provisioned long ring pattern  (14.8 sec)
**    This is a 320-bit pattern representing a 14.8 second ring pattern.  The
**    first two octets of the bit string represents the length in bits of the
**    ring cadence duration.  The third octet is used to represent repeatable
**    characteristics and the ring state where the repeat pattern starts. 
**    0xxxxxxx means repeatable, and 1xxxxxxx means non repeatable.  
**    The least 7 bits represent the ring state where the repeat pattern starts
**    when repeat pattern set to true (i.e. MSB is set to 0).
**    Each bit after the third octet represents 50ms,
**    with 1 representing a ring and 0 for silence.  The first bit of the
**    fourth octet is the first bit of the ring cadence.  Only 296 bits can be
**    set to represent a total cadence cycle.  Leading zeros in the ring
**    pattern are ignored and can be used to shorten the ring pattern length.
**
**    This ring pattern is used by standard ring, ring splash, and all
**    E-line package ring cadences.
*/
typedef struct CAS_CTL_PROV_LONG_RING_PATTERN
{
   VRG_UINT32  pattern1;
   VRG_UINT32  pattern2;
   VRG_UINT32  pattern3;
   VRG_UINT32  pattern4;
   VRG_UINT32  pattern5;
   VRG_UINT32  pattern6;
   VRG_UINT32  pattern7;
   VRG_UINT32  pattern8;
   VRG_UINT32  pattern9;
   VRG_UINT32  pattern10;

} CAS_CTL_PROV_LONG_RING_PATTERN;

#define  CAS_CTL_PROV_LONG_RING_MAX_INTERVAL         296


/* Provisionable ring pattern */
typedef union CAS_CTL_PROV_PATTERN
{
   /* 6 sec ring pattern - each bit represents a 100 ms duration of the ring cadence */
   CAS_CTL_PROV_SHORT_RING_PATTERN     shortRingPattern;

   /* 13.2 sec ring pattern - each bit represents a 50 ms duration of the ring cadence */
   CAS_CTL_PROV_LONG_RING_PATTERN      longRingPattern;

} CAS_CTL_PROV_PATTERN;


/* Ringing on-hook profile state */
typedef struct CAS_CTL_RING_PROFILE
{
   /* DO NOT CHANGE THE ORDER. Add at the end */
   CAS_CTL_PROV_PATTERN provBits;      /* MUST BE FIRST: provisioned ring pattern bit-field  */
   CAS_CTL_RING_PATTERN pattern;       /* ring profile type                   */
   CAS_CTL_PROV_RING_TYPE ringType;    /* ring type (short or long) associated with
                                       ** the provisioned ring pattern        */
   VRG_UINT32           clidTxDelay;
} CAS_CTL_RING_PROFILE;


/*
** Cas Control State Machine Values - default values can be found above.
*/
typedef struct CAS_CTL_FXO_PARMS
{
   VRG_UINT32  minHookFlash;           /* minimum hook flash interval         */
   VRG_UINT32  maxHookFlash;           /* maximum hook flash interval         */
   VRG_UINT32  pulseMinMake;           /* pulse dialing min make interval     */
   VRG_UINT32  pulseMaxMake;           /* pulse dialing max make interval     */
   VRG_UINT32  pulseMinBreak;          /* pulse dialing min break interval    */
   VRG_UINT32  pulseMaxBreak;          /* pulse dialing max break interval    */
   VRG_UINT32  minConnect;             /* minimum connection interval         */
   VRG_UINT32  minDisconnect;          /* minimum disconnect interval         */
   VRG_UINT32  minWink;                /* minimum hook wink interval          */
   VRG_UINT32  minPostWink;            /* minimum hook post-wink interval     */
   VRG_UINT32  earlyActiveHookInterval;/* early hook state change interval for active state */
   VRG_UINT32  delayActiveHookInterval;/* delay in active state after offhook detected */

} CAS_CTL_FXO_PARMS;

/* CAS callback function */
typedef int (*CAS_CTL_SLIC_CALLBACK)( VRG_UINT32 channel, VRG_UINT32 ringing );

/* CAS extended command parameter struct */
typedef struct CAS_CTL_EXT_CMD_PARM
{
   void *drvHdl;
   int  arguments[10];
   
} CAS_CTL_EXT_CMD_PARM;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

CAS_CTL_STATUS casCtlInit( CAS_CTL_CALLBACK callback );
CAS_CTL_STATUS casCtlDeinit( void );
CAS_CTL_STATUS casCtlEndptInit( CAS_TYPE channelType, XDRV_CAS *casDriver, CAS_CTL_HANDLE *handle, CAS_CTL_FXO_PARMS *fxoParms, CAS_CTL_SLIC_CALLBACK casCallback );
CAS_CTL_STATUS casCtlEndptDeinit( CAS_CTL_HANDLE handle );
CAS_CTL_STATUS casCtlRingProfileOn( CAS_CTL_HANDLE handle, CAS_CTL_RING_PROFILE *pattern, VRG_BOOL ringInterleave );
CAS_CTL_STATUS casCtlRingOff( CAS_CTL_HANDLE handle, VRG_BOOL alert );
CAS_CTL_STATUS casCtlRingOn( CAS_CTL_HANDLE handle );
CAS_CTL_STATUS casCtlSupervisoryDisconnect( CAS_CTL_HANDLE handle, VRG_UINT16 duration, CAS_OSI_PATTERN pattern );
CAS_CTL_STATUS casCtlSupervisoryDisconnectOn( CAS_CTL_HANDLE handle );
CAS_CTL_STATUS casCtlSupervisoryDisconnectOff( CAS_CTL_HANDLE handle, VRG_BOOL alert, VRG_BOOL offhook );
CAS_CTL_STATUS casCtlIgnoreHookState( CAS_CTL_HANDLE handle, VRG_UINT16 duration );
CAS_CTL_STATUS casCtlEnableFastSlicStateMode( CAS_CTL_HANDLE handle, VRG_BOOL enable );

#define casCtlSetState( hdl, state )   casCtlSetStateGenEvent( (hdl), (state), 0 )
CAS_CTL_STATUS casCtlSetStateGenEvent
(
   CAS_CTL_HANDLE                handle,
   CAS_CTL_FXO_INTERFACE_STATE   state,
   VRG_UINT32                    duration
);
CAS_CTL_STATUS casCtlSetStateFXS( CAS_CTL_HANDLE handle, CAS_CTL_FXS_INTERFACE_STATE state );

CAS_CTL_STATUS casCtlChangePolarity
(
   CAS_CTL_HANDLE    handle,
   CAS_CTL_POLARITY  polarity,
   CAS_CTL_EVENT     event,
   VRG_UINT32        eventDelay
);

CAS_CTL_STATUS casCtlSendCommand(CAS_REQ_ID reqId, CAS_CMD_MODE mode, VRG_UINT32 op1, VRG_UINT16 op2);
CAS_CTL_STATUS casCtlSendExtendedCommand(CAS_REQ_ID reqId, CAS_CMD_MODE mode, CAS_CTL_EXT_CMD_PARM *parms);

#ifdef __cplusplus
}
#endif

#endif      /* CASCTL H */
