/***************************************************************************
*    Copyright © 2000-2007 Broadcom Corporation                                        
*                                                                           
*    This program is the proprietary software of Broadcom Corporation and/or
*    its licensors, and may only be used, duplicated, modified or           
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized   
*    License").  Except as set forth in an Authorized License, Broadcom    
*    grants no license (express or implied), right to use, or waiver of any 
*    kind with respect to the Software, and Broadcom expressly reserves all 
*    rights in and to the Software and all intellectual property rights     
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO 
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM   
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.                               
*                                                                           
*                                                                           
*    Except as expressly set forth in the Authorized License,               
*                                                                           
*    1.     This program, including its structure, sequence and             
*    organization, constitutes the valuable trade secrets of Broadcom, and  
*    you shall use all reasonable efforts to protect the confidentiality    
*    thereof, and to use this information only in connection with your use  
*    of Broadcom integrated circuit products.                               
*                                                                           
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,          
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR  
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY        
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,    
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,    
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR         
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT  
*    OF USE OR PERFORMANCE OF THE SOFTWARE.                                 
*                                                                           
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL       
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, 
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN  
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR    
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE 
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: casctl.c
*
****************************************************************************
*    Description:
*
*      This module contains the functions of the HAUSWARE Channel Associated
*      Signaling service.
*
****************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <casCtlCfg.h>
#include <bosSleep.h>
#include <bosTask.h>
#include <memCircBuf.h>
#include <casCtl.h>
#include <casCtlProfiles.h>
#include "casCtlStateMachine.h"

#define CAS_MIN_RINGOFF_MS    2000
#define MSB_MASK              0x80000000
#define MS2BYTES_MASK         0xFFFF0000

/* Size of UINT32 (in bits) */
#define UINT32_LENGTH         32

/* Short ring bit duration (ms) */
#define CAS_CTL_PROV_SHORT_RING_BIT_DURATION  100

/* Long ring bit duration (ms) */
#define CAS_CTL_PROV_LONG_RING_BIT_DURATION   50

/* Long ring bit repeatable charcteristics mask */
#define CAS_CTL_PROV_LONG_RING_BIT_REPEAT     0x00008000

/* Long ring repeat state mask */
#define CAS_CTL_PROV_LONG_RING_REPEAT_STATE_MASK  0x00007F00

/* Default ring cadence duration (ms) */
#define CAS_CTL_DEFAULT_RING_CADENCE          6000


#define CAS_TASK_RATE         CAS_CTL_CFG_TASK_RATE_MS

/* Default number of semaphores to be added to semaphore block list */
#define CAS_DEFAULT_BLOCK_SEM_MAX 3

#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
   #define RINGSLOTEMPTY 0xFFFF
   #define MAX_RINGERS_PER_SLOT  (( CAS_CTL_CFG_NUM_ENDPTS + 1 ) / 2 )
   #define DEFAULT_STAGGER_INTERVAL_MSEC 2000

   enum STAGGERRINGCOMMAND
   {
      STAGGERRING_ON,
      STAGGERRING_OFF
   };

   typedef struct
   {
      VRG_BOOL   ringOn[MAX_RINGERS_PER_SLOT];
      VRG_UINT16 ringslot[MAX_RINGERS_PER_SLOT];
      CAS_CTL_RING_PROFILE pattern[MAX_RINGERS_PER_SLOT];
      VRG_UINT32 patternBits[MAX_RINGERS_PER_SLOT];
#   if CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP
      VRG_UINT32 ringCadence[MAX_RINGERS_PER_SLOT];
#   endif /* CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP */
   } CASSTAGGERMEM;

#   if CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP
#      define MAX_NUM_STAGGER_SLOTS 2
#   else
#      define MAX_NUM_STAGGER_SLOTS 3
#   endif /* CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP */
#endif

typedef struct
{
   VRG_UINT16        numChannels;
   CAS_CTL_CALLBACK  callback;
   BOS_TASK_ID       taskId;
   CASPROFILE        profile[CAS_CTL_CFG_NUM_ENDPTS];
   CASSRV            service[CAS_CTL_CFG_NUM_ENDPTS];

#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
   BOS_TIME_MS       staggerMsec;
   VRG_UINT16        maxRingersPerSlot;
   CASSTAGGERMEM     staggerState[MAX_NUM_STAGGER_SLOTS];
   VRG_UINT32        staggerInterval;
#endif
} CASSTATE;

static CASSTATE   casState;

static void StaggerBlk( void );
#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
static CAS_CTL_STATUS casStaggerRing
(
   VRG_UINT16     idx,
   CAS_CTL_RING_PROFILE  *pattern,
   VRG_UINT32     patternBits,
   VRG_UINT16     command,
   VRG_UINT32     cadence
);
#endif

/* Mutex for semaphore block list protection */
static BOS_MUTEX semBlockMutex;

/* Semaphore block */
typedef struct CAS_CTL_SEM_BLOCK
{
   BOS_SEM *semaphore;             /* Semaphore handle */
   struct CAS_CTL_SEM_BLOCK *next; /* Pointer to next semaphore block */
} CAS_CTL_SEM_BLOCK; 

/* Pointer to the head of the semaphore block list */
CAS_CTL_SEM_BLOCK *casCtlSemBlockHead = NULL;

static CAS_CTL_STATUS profileRingOn( VRG_UINT16 idx,
                                     CAS_CTL_RING_PROFILE *profile );
static void casSetRingPattern( CASSTM *ringPattern,
                               const CAS_CTL_PROV_PATTERN *ringPatternBits,
                               VRG_UINT32 clidTxDelay,
                               CAS_CTL_PROV_RING_TYPE ringType );
static CAS_CTL_STATUS casCmdSync( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, VRG_UINT16 op2 );
static CAS_CTL_STATUS casCmdSyncSerial( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, VRG_UINT16 op2 );
static CAS_CTL_STATUS casCmdAsync( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, VRG_UINT16 op2 );
static CAS_CTL_STATUS casCmdDataSync( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, void* op2 );
static CAS_CTL_STATUS casCmdDataAsync( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, void* op2 );
static CAS_CTL_STATUS casCmdResp( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT32 op1, void* op2 );
static void casTaskMain( BOS_TASK_ARG taskArg );
static void casTaskInitCB( BOS_TASK_ARG taskArg );
static void casTaskDeinitCB( BOS_TASK_ARG taskArg );
static CAS_CTL_STATUS casAction( VRG_UINT16 idx, VRG_UINT32 actionState );
static void leftShift( VRG_UINT32 *pCurrentWord, VRG_UINT32 *pLastWord );
static void EventCallback( CASEVT *event, VRG_UINT16 dspNum );
static VRG_UINT16 GetIndexFromHandle( CAS_CTL_HANDLE casHandle );
#if CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP
static VRG_UINT32 casGetRingCadence( CAS_CTL_RING_PROFILE *rgProfile );
static void casSetNewStaggerInterval( VRG_UINT32 rgCadence );
#endif /* CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP */

static CAS_CTL_STATUS casActionGenEvent
(
   VRG_UINT16  idx,
   VRG_UINT32  actionState,
   VRG_UINT32  duration
);

CAS_CTL_STATUS casCtlCreateCmdSempahore( void );
CAS_CTL_STATUS casCtlDestroyCmdSempahoreList( void );
CAS_CTL_SEM_BLOCK *casCtlGetCmdSemaphore( void );
CAS_CTL_STATUS casCtlPutCmdSempahore( CAS_CTL_SEM_BLOCK *casCtlSemBlock );

/* Create an array of pointers to each country's ring profiles */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) gCasCtlRingPatterns_##country,
static const CASSTM *gRingPatterns[ VRG_COUNTRY_MAX ] =
{
   #include <countryArchive.h>
};

static const CASSTM loopPatterns[] =
{
   /*************************************************
   ** Profile 1: Supervisory Disconnect - VMWI     **
   *************************************************/
   {
      /* Loop Counter */
      /* Not used for this pattern */
      0,0,

      {
         /* State 0 */
         {( FXOBITS_SERVICEDENIED | CAS_NEXTSTATE_1 | CASEVENT_LCFO | CASBITS_LCFO ), 300},

         /* State 1 - Return to the idle state */
         {( FXOBITS_ALERTING | CAS_NEXTSTATE_2 | CASBITS_LCF ), 300},

         /* State 2 - Not Used */
         {( FXOBITS_ALERTING | CAS_NEXTSTATE_3 | CASEVENT_OSIEND | CASBITS_LCF ), 2000},

         /* State 3 - Not Used */
         {( FXOBITS_IDLE | CAS_ENDING_STATE | CASEVENT_LCF | CASBITS_LCF ), 0},

         /* State 4 - Not Used */
         {0,0},
         /* State 5 - Not Used */
         {0,0},
         /* State 6 - Not Used */
         {0,0},
         /* State 7 - Not Used */
         {0,0},
         /* State 8 - Not Used */
         {0,0},
         /* State 9 - Not Used */
         {0,0},
         /* State 10 - Not Used */
         {0,0},
         /* State 11 - Not Used */
         {0,0}
      }
   },

   /*************************************************
   ** Profile 2: Supervisory Disconnect - Default  **
   *************************************************/
   {
      /* Loop Counter */
      /* Not used for this pattern */
      0,0,

      {
         /* State 0 */
         {( FXOBITS_SERVICEDENIED | CAS_NEXTSTATE_1 | CASEVENT_LCFO | CASBITS_LCFO ), 300},

         /* State 1 - Activate Slic but remain in service denied mode */
         {( FXOBITS_SERVICEDENIED | CAS_NEXTSTATE_2 | CASEVENT_OSIEND | CASBITS_LCF ), 300},

         /* State 2 - Return to the previous state */
         {( FXOBITS_IDLE | CAS_ENDING_STATE | CASEVENT_LCF | CASBITS_LCF ), 0},

         /* State 3 - Not Used */
         {0,0},
         /* State 4 - Not Used */
         {0,0},
         /* State 5 - Not Used */
         {0,0},
         /* State 6 - Not Used */
         {0,0},
         /* State 7 - Not Used */
         {0,0},
         /* State 8 - Not Used */
         {0,0},
         /* State 9 - Not Used */
         {0,0},
         /* State 10 - Not Used */
         {0,0},
         /* State 11 - Not Used */
         {0,0}
      }
   }
};

/* casCtl command Queue */
static MEMCIRCBUFDRV casCtlCmdQ;


/****************************************************************************
*
*  casCtlSupervisoryDisconnect - Uses ring profile to perform VMWI operation
*
*****************************************************************************
*
*  PURPOSE:
*     Uses ring profile to perform VMWI operation
*
*  PARAMETERS:
*     handle    - the physical interface to control
*     pattern  - ring pattern to generate
*     duration - duration of the OSI (open switch interval) signal
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlSupervisoryDisconnect( CAS_CTL_HANDLE handle, VRG_UINT16 duration, CAS_OSI_PATTERN pattern )
{
   CASPROFILE  casprofile;
   CASCHANNELINFO *channelp;
   VRG_UINT16  idx;

   idx = GetIndexFromHandle( handle );
   channelp = &casState.service[idx].localmem.channelinfo;

   switch ( pattern )
   {
      case CAS_OSI_VMWI:
      {
         /* Copy Ring Pattern to generate */
         casprofile.casstm = loopPatterns[0];
      }
      break;

      default:
      {
         /* Copy Ring Pattern to generate */
         casprofile.casstm = loopPatterns[1];

         /* Enter critical section to preserve state info */
         bosCritSectEnter( &casState.service[idx].localmem.critSect );

         /* Set the Current Hook State so it is restored */

         if ((( channelp->state & CASBITS_MASK ) != CASBITS_RINGING ) &&
             (( channelp->state & CASBITS_MASK ) != CASBITS_LCFO )    && 
             (( channelp->state & CAS_STATEBITS_MASK ) != FXOBITS_SERVICEDENIED ))
         {
            casprofile.casstm.state[2].stateinfo = channelp->state;
            casprofile.casstm.state[2].stateinfo &= ~( CAS_NEXTBITS_MASK | CAS_EVENT_MASK );
            casprofile.casstm.state[2].stateinfo |= ( CAS_ENDING_STATE );            
         }
         else
         {
            casprofile.casstm.state[2].stateinfo = channelp->previousActiveState;
            casprofile.casstm.state[2].stateinfo &= ~( CAS_NEXTBITS_MASK | CAS_EVENT_MASK );
            casprofile.casstm.state[2].stateinfo |= ( CAS_ENDING_STATE );
         }


         /* Exit critical section */
         bosCritSectLeave( &casState.service[idx].localmem.critSect );
      }
      break;
   }

   /* Set the OSI duration */
   casprofile.casstm.state[0].duration = duration;

   /* Set index of which physical interface to control */
   casprofile.index = idx;

   /* Send Ring On */
   return( casCmdDataAsync( idx, CAS_STATECTL, CAS_REQ_NONE, sizeof(casprofile), &casprofile) );
}

/****************************************************************************
*
*  casCtlSupervisoryDisconnectOn - Disconnect the physical interface
*
*****************************************************************************
*
*  PURPOSE:
*     Disconnects the physical interface in preparation for VMWI operation
*
*  PARAMETERS:
*     handle    - the physical interface to control
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlSupervisoryDisconnectOn( CAS_CTL_HANDLE handle )
{
   VRG_UINT16  idx = GetIndexFromHandle( handle );

   /* Have the FXO enter the service denied state */
   casAction( idx, FXOBITS_SERVICEDENIED | CASEVENT_LCFO | CASBITS_LCFO );

   return ( CAS_CTL_SUCCESS );
}

/****************************************************************************
*
*  casCtlSupervisoryDisconnectOff - Reconnect the physical interface
*
*****************************************************************************
*
*  PURPOSE:
*     Reconnects the physical interface after disconnect.
*
*  PARAMETERS:
*     handle    - the physical interface to control
*     alert   - TRUE  - leave the interface in the alert state after reconnect
*               FALSE - leave the interface in the idle state after reconnect
*     offhook   - offhook state
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlSupervisoryDisconnectOff( CAS_CTL_HANDLE handle, VRG_BOOL alert, VRG_BOOL offhook )
{
   VRG_UINT16 idx = GetIndexFromHandle( handle );

   if (offhook)
   {
      /* Have the FXO enter the active state */
      casAction( idx, FXOBITS_ACTIVE | CASEVENT_LCF | CASBITS_LCF );
   }
   else if (alert)
   {
      /* Have the FXO enter the alert state */
      casAction( idx, FXOBITS_ALERTING | CASEVENT_LCF | CASBITS_LCF );
   }
   else
   {
      /* Have the FXO enter the idle state */
      casAction( idx, FXOBITS_IDLE | CASEVENT_LCF | CASBITS_LCF );
   }

   return ( CAS_CTL_SUCCESS );
}

/****************************************************************************
*
*  casCtlRingProfileOn - Generate ring profile
*
*****************************************************************************
*
*  PURPOSE:
*     Generate ring profile
*
*  PARAMETERS:
*     handle         - the physical interface to control
*     pattern        - ring pattern number to generate
*     ringInterleave - when ring interleave feature is enabled, choose whether
*                      or not we want to use it.
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE: This function supports both staggered and non-staggered ringing.
*        The decision to use staggered ringing is made at compile-time
*        based upon the flag CAS_CTL_CFG_STAGGER_RING_SUPPORT
*
****************************************************************************/
CAS_CTL_STATUS casCtlRingProfileOn( CAS_CTL_HANDLE casHandle,
                                    CAS_CTL_RING_PROFILE *pattern,
                                    VRG_BOOL ringInterleave )
{
   VRG_UINT16  idx = GetIndexFromHandle( casHandle );
   CAS_CTL_STATUS status;

#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
   if( ringInterleave )
   {
      VRG_UINT32  rgCadence = 0; /* unused */
#   if CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP
      rgCadence = casGetRingCadence( pattern );
      casSetNewStaggerInterval( rgCadence );
#   endif /* CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP */
      /* Use staggered ringing */
      status = casStaggerRing( idx, pattern, 0, STAGGERRING_ON, rgCadence );
   }
   else
   {
      /* Use non-staggered ringing */
      status = profileRingOn( idx, pattern );
   }
#else
   /* Use non-staggered ringing */
   status = profileRingOn( idx, pattern );
#endif

   return( status );
}


/****************************************************************************
*
*  profileRingOn - Generate ring profile
*
*****************************************************************************
*
*  PURPOSE:
*     Generate ring profile
*
*  PARAMETERS:
*     idx         - the physical interface to control
*     pattern     - static ring pattern number to generate
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE: This function does not support staggered ringing. i.e. it will
*        cause the handset to ringing immediately.
*
****************************************************************************/
static CAS_CTL_STATUS profileRingOn( VRG_UINT16 idx,
                                     CAS_CTL_RING_PROFILE *profile )
{
   CASPROFILE casprofile;

   /* Set ring pattern based on ring pattern bits (if valid) */
   if ( profile->ringType != CAS_CTRL_STATIC_RING )
   {
      casSetRingPattern( &casprofile.casstm, &profile->provBits,
                         profile->clidTxDelay, profile->ringType );
   }
   else
   {
      VRG_COUNTRY       country;
      unsigned int      profileIndex;

      country        = CAS_CTL_RING_PATTERN_COUNTRY( profile->pattern );
      profileIndex   = CAS_CTL_RING_PATTERN_ID     ( profile->pattern );

      XCHG_ASSERT( country <= VRG_COUNTRY_MAX );
      XCHG_ASSERT( gRingPatterns[ country ] != NULL );

      casprofile.casstm = gRingPatterns[ country ][ profileIndex ];
   }

   /* Set index of which physical interface to control */
   casprofile.index = idx;

   /* Send Ring On */
   return( casCmdDataAsync( idx, CAS_STATECTL, CAS_REQ_NONE, sizeof(casprofile), &casprofile) );
}


/****************************************************************************
*
*  casCtlRingOff - Turn off ringing
*
*****************************************************************************
*
*  PURPOSE:
*     Turn off ringing
*
*  PARAMETERS:
*     handle   - the physical interface to control
*     alert   - TRUE  - leave the interface in the alert state after ringing
                FALSE - leave the interface in the idle state after ringing
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlRingOff( CAS_CTL_HANDLE casHandle, VRG_BOOL alert )
{
   VRG_UINT16  idx = GetIndexFromHandle( casHandle );

   if (alert)
   {
      /* Have the FXO enter the alert state */
      casAction( idx, CASBITS_LCF | FXOBITS_ALERTING | CASEVENT_LCF );
   }
   else
   {
      /* Have the FXO enter the idle state */
      casAction( idx, CASBITS_LCF | FXOBITS_IDLE | CASEVENT_LCF );
   }

#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
   /* Clear staggered ringing state */
   casStaggerRing( idx, 0, 0, STAGGERRING_OFF, 0 );
#endif

   return CAS_CTL_SUCCESS;
}

/****************************************************************************
*
*  casCtlChangePolarity - Change Polarity
*
*****************************************************************************
*
*  PURPOSE:
*     Change the polarity of the loop
*
*  PARAMETERS:
*     handle      - the physical interface to control
*     polarity    - the CAS bits CASBITS_LCF or CASBITS_RLCF
*     event       - CAS event to generate, CASEVENT_NOEVT for no event
*     eventDelay  - event will be generated after this delay
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlChangePolarity
(
   CAS_CTL_HANDLE    handle,
   CAS_CTL_POLARITY  polarity,
   CAS_CTL_EVENT     event,
   VRG_UINT32        eventDelay

)
{
   CASSTATUS   casStatus;
   VRG_UINT32  actionState;
   VRG_UINT32  casBits = CASBITS_LCF;
   VRG_UINT32  casEvent;

   VRG_UINT16  idx = GetIndexFromHandle( handle );

   if ( polarity == CAS_CTL_POLARITY_FORWARD )
   {
      casBits = CASBITS_LCF;
   }
   else if ( polarity == CAS_CTL_POLARITY_REVERSE )
   {
      casBits = CASBITS_RLCF;
   }
   else
   {
      XCHG_ASSERT( 0 );
   }

   casEvent = ( event << CAS_EVENT_SHIFT );

   /* Get the current CAS status */
   casCmdResp( idx, CAS_GET_LINESTATUS, sizeof(casStatus), &casStatus);

   actionState  = ( casStatus.state & CAS_STATEBITS_MASK );
   actionState |= ( casBits | casEvent );

   /* Change the state so that new CAS bits are used */
   if ( eventDelay == 0 )
   {
      casAction( idx, actionState );
   }
   else
   {
      casActionGenEvent( idx, actionState, eventDelay );
   }

   return ( CAS_CTL_SUCCESS );
}


#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
/****************************************************************************
*
*  casStaggerRingClean - Clean a staggered ring setting
*
*****************************************************************************
*
*  PURPOSE:
*     Clean (terminate) stagger ring block for a given channel
*
*  PARAMETERS:
*     index       - the physical interface to control
*
*  RETURNS:
*     Nothing
*
****************************************************************************/
void casStaggerRingClean( VRG_UINT16 idx )
{
   /* Clear staggered ringing state */
   casStaggerRing( idx, 0, 0, STAGGERRING_OFF, 0 );
}

/****************************************************************************
*
*  casStaggerRing - Staggered Ringing Commands/events
*
*****************************************************************************
*
*  PURPOSE:
*     Generate ring profile using staggered algorithm
*
*  PARAMETERS:
*     index       - the physical interface to control
*     pattern     - ring pattern number to generate
*     patternBits - provisionable ring pattern, set to MAX_RINGPATTERN_BITS
*                   for static ring patterns
*     command     - should be of type STAGGERRINGCOMMAND. Indicates whether
*                   to turn on or off the ringing
*     cadence     - cadence for this ring.
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE: The staggered ring alorithm splits the 6 second ring cadence into
*        3 timeslots, each of 2 seconds. Ringing will only commence on a
*        timeslot boundary. In order to conserve power, the number
*        of ringing handsets per timeslot is limited to a maximum value
*        (currently, this will be capped at the number of endpoints divided
*        by 2).
*
****************************************************************************/
static CAS_CTL_STATUS casStaggerRing
(
   VRG_UINT16     idx,
   CAS_CTL_RING_PROFILE  *profile,
   VRG_UINT32     patternBits,
   VRG_UINT16     command,
   VRG_UINT32     cadence
)
{
   int             i, j;
   VRG_BOOL        foundslot;
   CASSTAGGERMEM  *casStaggerState;

   foundslot = VRG_FALSE;
   casStaggerState = casState.staggerState;

   switch (command)
   {
      case STAGGERRING_ON:
      {

         /* see which slot does not have two ring's assigned */
         /* if there is a request find next available slot to issue ring */
         for ( j = 0; j < casState.maxRingersPerSlot; j++ )
         {
            for (i = 0; i < MAX_NUM_STAGGER_SLOTS; i++)
            {
               /* if at least one is empty */
               if (( casStaggerState[i].ringslot[j] == RINGSLOTEMPTY ))
               {
                  /* found slot. fill with parameters */
                  casStaggerState[i].ringslot[j] = idx;
                  memcpy( &casStaggerState[i].pattern[j], profile, sizeof(CAS_CTL_RING_PROFILE) );
                  casStaggerState[i].patternBits[j] = patternBits;
#   if CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP
                  casStaggerState[i].ringCadence[j] = cadence;
#   endif /* CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP */
                  foundslot = VRG_TRUE;
                  break;
               }
            }

            if ( foundslot == VRG_TRUE )
            {
               VRG_BOOL ringing = VRG_FALSE;

               /* Look if any interval is ringing already or not. */
               for (i = 0; i < MAX_NUM_STAGGER_SLOTS; i++)
               {
                  for ( j = 0; j < casState.maxRingersPerSlot; j++ )
                  {
                     if( casStaggerState[i].ringOn[j] == VRG_TRUE )
                     {
                        ringing = VRG_TRUE;
                     }
                  }
               }
               /* If no interval is currently ringing, but at least one is lined up
               ** to ring next (it will then be the first one), make sure it will ring
               ** right away. */
               if( ringing == VRG_FALSE )
               {
                  bosCritSectEnter( &casState.service[idx].localmem.critSect );
                  casState.staggerMsec = casState.staggerInterval;
                  bosCritSectLeave( &casState.service[idx].localmem.critSect );
               }
               break;
            }
         }

         if ( !foundslot )
         {
            profileRingOn( idx, profile );
         }
      }
      break;

      case STAGGERRING_OFF:
      {

         for( i = 0; i < MAX_NUM_STAGGER_SLOTS; i++ )
         {
            for ( j = 0; j < casState.maxRingersPerSlot; j++ )
            {
               /* check slot i */
               if (( casStaggerState[i].ringslot[j] == idx ))
               {
                  /* found slot */
                  casStaggerState[i].ringslot[j] = RINGSLOTEMPTY;
                  casStaggerState[i].ringOn[j]= VRG_FALSE;
                  break;
               }
            }
         }
         /* remove from slot */
      }
      break;

      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_CAS, "Stagger Ring: Unrecognized event/command"));
      }
      break;
   }

   return ( CAS_CTL_SUCCESS );
}
#endif


/****************************************************************************
*
*  StaggerBlk - block task for staggered ringing
*
*****************************************************************************
*
*  PURPOSE:
*     Initiates ringing based upon staggered ringing algorithm. Refer to
*     casStaggerRing( ) function header for details.
*
*  PARAMETERS:
*     none
*
*  RETURNS:
*     none
*
*  NOTE:
*
****************************************************************************/
static void StaggerBlk( void )
{
#if CAS_CTL_CFG_STAGGER_RING_SUPPORT

   int j;
   CASSTAGGERMEM  membuf;
   CASSTAGGERMEM  *casStaggerState;

   casStaggerState = casState.staggerState;

   /* only run every staggerInterval milliseconds */
   if( casState.staggerMsec > casState.staggerInterval )
   {
     for ( j = 0; j < casState.maxRingersPerSlot; j++ )
      {
         /* check for pending requests for ring on current slot 0 */
         if ( casStaggerState[0].ringslot[j] != RINGSLOTEMPTY )
         {
            /* ring */
            if ( casStaggerState[0].ringOn[j] == VRG_FALSE )
            {
               profileRingOn(casStaggerState[0].ringslot[j], &casStaggerState[0].pattern[j]);
               casStaggerState[0].ringOn[j] = VRG_TRUE;
            }
         }
      }

      /* update slots
      */
      memcpy( &membuf, &casStaggerState[0], sizeof(CASSTAGGERMEM) );
      for( j = 0 ; j < MAX_NUM_STAGGER_SLOTS - 1 ; j++ )
      {
         memcpy( &casStaggerState[j], &casStaggerState[j+1], sizeof(CASSTAGGERMEM));
      }
      memcpy( &casStaggerState[MAX_NUM_STAGGER_SLOTS - 1], &membuf, sizeof(CASSTAGGERMEM));

      /* reset counter */
      casState.staggerMsec = 0;
   }
   else
   {
      /* increment count */
      casState.staggerMsec += CAS_TASK_RATE;
   }
#endif
}


/****************************************************************************
*
*  casCtlRingOn - Turn on ringing
*
*****************************************************************************
*
*  PURPOSE:
*     Turn on ringing
*
*  PARAMETERS:
*     casHandle   - the physical interface to control
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlRingOn( CAS_CTL_HANDLE casHandle )
{
  VRG_UINT16  idx = GetIndexFromHandle( casHandle );

  /* Have the FXO enter the idle state */
  casAction( idx, FXOBITS_ALERTING | CASEVENT_RINGON | CASBITS_RINGING );

  return CAS_CTL_SUCCESS;
}


/****************************************************************************
*
*  casAction -
*
*****************************************************************************
*
*  PURPOSE:
*
*
*  PARAMETERS:
*     idx       - the physical interface to control
*     actionState -
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
static CAS_CTL_STATUS casAction( VRG_UINT16 idx, VRG_UINT32 actionState )
{
   CASPROFILE casprofile;

   /* Setup state to change to */
   casprofile.casstm.state[0].stateinfo = ( actionState & ~( CAS_NEXTBITS_MASK )) | CAS_ENDING_STATE;
   casprofile.casstm.state[0].duration = 2;

   /* Set index of which physical interface to control */
   casprofile.index = idx;

   return( casCmdDataSync( idx, CAS_STATECTL, CAS_REQ_NONE, sizeof(casprofile), &casprofile));
}


/****************************************************************************
*
*  casActionGenEvent -
*
*****************************************************************************
*
*  PURPOSE:
*
*
*  PARAMETERS:
*     idx         - the physical interface to control
*     actionState -
*     duration    - generate a CASEVENT_TIMER_EXPIRE event after this duration.
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
static CAS_CTL_STATUS casActionGenEvent
(
   VRG_UINT16  idx,
   VRG_UINT32  actionState,
   VRG_UINT32  duration
)
{
   CASPROFILE casprofile;

   /* Setup state to change to for specified duration. */
   casprofile.casstm.state[0].stateinfo = ( actionState & ~( CAS_NEXTBITS_MASK )) | CAS_NEXTSTATE_1;
   casprofile.casstm.state[0].duration  = duration;

   /* Generate timer expired event. */
   casprofile.casstm.state[1].stateinfo = ( actionState & ~( CAS_NEXTBITS_MASK | CAS_EVENT_MASK )) | CASEVENT_TIMER_EXPIRE | CAS_ENDING_STATE;
   casprofile.casstm.state[1].duration  = 0;

   /* Set index of which physical interface to control */
   casprofile.index = idx;

   return( casCmdDataAsync( idx, CAS_STATECTL, CAS_REQ_NONE, sizeof(casprofile), &casprofile));
}


/****************************************************************************
*
*  casCtlIgnoreHookState -
*
*****************************************************************************
*
*  PURPOSE: To instruct case to temporarily ignore the slic state
*
*
*  PARAMETERS:
*     handle   -  The physical interface to control
*     ignore   -  Control variable
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlIgnoreHookState
(
   CAS_CTL_HANDLE                handle,
   VRG_UINT16                    duration
)
{
   /* Retrieve ID of this instance from the handle */
   VRG_UINT16 idx = GetIndexFromHandle( handle );

   /* Save the new control field for future reference */
   casState.service[idx].localmem.channelinfo.ignoreHookState = duration;

   return( CAS_CTL_SUCCESS );
}


/****************************************************************************
*
*  casCtlSetStateGenEvent -
*
*****************************************************************************
*
*  PURPOSE: To change the CAS state and the associated slic state
*
*
*  PARAMETERS:
*     handle   -  The physical interface to control.
*     state    -  The state to change to.
*     duration -  Generate a CASEVENT_TIMER_EXPIRE event after this duration.
*                 If set to 0, no event will be generated.
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlSetStateGenEvent
(
   CAS_CTL_HANDLE                handle,
   CAS_CTL_FXO_INTERFACE_STATE   state,
   VRG_UINT32                    duration
)
{
   VRG_UINT32  actionState;
   VRG_UINT16  idx = GetIndexFromHandle( handle );

   switch (state)
   {
      case CAS_CTL_FXO_SERVICEDENIED:
      {
         actionState = CASBITS_LCFO | FXOBITS_SERVICEDENIED | CASEVENT_LCFO;
      }
      break;

      case CAS_CTL_FXO_IDLE:
      {
         actionState = CASBITS_LCF | FXOBITS_IDLE | CASEVENT_LCF;
      }
      break;

      case CAS_CTL_FXO_ALERTING_RING:
      {
         actionState = CASBITS_RINGING | FXOBITS_ALERTING | CASEVENT_LCF;
      }
      break;

      case CAS_CTL_FXO_ALERTING_OHT:
      {
         actionState = CASBITS_LCF | FXOBITS_ALERTING | CASEVENT_LCF;
      }
      break;

      case CAS_CTL_FXO_ALERTING_OHTR:
      {
         actionState = CASBITS_RLCF | FXOBITS_ALERTING | CASEVENT_RLCF;
      }
      break;

      case CAS_CTL_FXO_ADDRESSING:
      {
         actionState = CASBITS_LCF | FXOBITS_ADDRESSING | CASEVENT_LCF;
      }
      break;

      case CAS_CTL_FXO_ACTIVE:
      {
         actionState = CASBITS_LCF | FXOBITS_ACTIVE | CASEVENT_LCF;
      }
      break;

      default:
      {
         return ( CAS_CTL_FAILURE );
      }
      break;
   }

   if ( duration == 0 )
   {
      return ( casAction( idx, actionState ) );
   }
   else
   {
      return ( casActionGenEvent( idx, actionState, duration ) );
   }

}

/****************************************************************************
*
*  casCtlSetStateFXS -
*
*****************************************************************************
*
*  PURPOSE: To change the CAS state and the associated slic state
*
*
*  PARAMETERS:
*     handle  - the physical interface to control
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlSetStateFXS( CAS_CTL_HANDLE handle, 
                                  CAS_CTL_FXS_INTERFACE_STATE state )
{
#if CAS_CTL_PSTN
   VRG_UINT16  idx = GetIndexFromHandle( handle );

   switch (state)
   {
      case CAS_CTL_FXS_RELEASED:
      {
         return ( casAction( idx, CASBITS_LO | 
                                  FXSBITS_RELEASED | 
                                  CASEVENT_ONHOOK ));
      }
      break;

      case CAS_CTL_FXS_SEIZED:
      {
         return ( casAction( idx, CASBITS_LC | 
                                  FXSBITS_SEIZED | 
                                  CASEVENT_OFFHOOK ));
      }
      break;

      case CAS_CTL_FXS_CLIDRX:
      {
         return ( casAction( idx, CASBITS_LO | 
                                  FXSBITS_CLIDRX | 
                                  CASEVENT_ONHOOK ));
      }
      break;

      default:
      {
         /* Other states may be set internally, but not by external users */
         return ( CAS_CTL_FAILURE );
      }
      break;
   }
#else
   (void) handle;
   (void) state;
   return CAS_CTL_FAILURE;
#endif
}

/****************************************************************************
*
*  casCtlInit - CAS controller init  - call once during system initialization
*
*****************************************************************************
*
*  PURPOSE:
*     CAS controller module init - called once during system initialization.
*     Conceptually, the CAS controller manages the CAS state-machines for
*     a set of endpoints. Individual endpoints should be initialized
*     using the casCtlEndptInit() API.
*
*  PARAMETERS:
*     callback  - callback function
*
*  RETURNS:
*     CAS_CTL_SUCCESS or CAS_CTL_FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlInit( CAS_CTL_CALLBACK callback )
{
   BOS_TASK_CLASS    tskPrio = CAS_CTL_CFG_TASK_PRIORITY;
   VRG_UINT16        i;
#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
   int               j;
#endif

   VRG_LOG_REGISTER( VRG_LOG_MOD_CAS );

   /* Init local storage */
   memset( &casState, 0, sizeof( casState ));

   casState.numChannels = CAS_CTL_CFG_NUM_ENDPTS;
   casState.callback    = callback;

   for( i = 0; i < CAS_CTL_CFG_NUM_ENDPTS; i++ )
   {

      memset( &casState.service[i], 0, sizeof (CASSRV) );
      casState.service[i].callbackp = EventCallback;

      /* Configure to control and monitor an FXO Interface */
      casCmdSync( i, CAS_REQ_NONE, (VRG_UINT16)CAS_CONFIG, (VRG_UINT32)CASTYPE_FXO, i );

   }

#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
   /* Save the maximum number of ringing handsets per timeslot for staggered ringing */
   casState.maxRingersPerSlot = ( CAS_CTL_CFG_NUM_ENDPTS + 1 ) / 2;
   casState.staggerMsec = 0;
   casState.staggerInterval = DEFAULT_STAGGER_INTERVAL_MSEC;

   for( i = 0; i < MAX_NUM_STAGGER_SLOTS; i++ )
   {
      for ( j = 0; j < MAX_RINGERS_PER_SLOT; j++ )
      {
         casState.staggerState[i].ringslot[j] = RINGSLOTEMPTY;
         casState.staggerState[i].ringOn[j]= VRG_FALSE;
      }
   }
#endif

   /* Create queue for storing casCtl commands */
   if ( memCircBufAlloc(&casCtlCmdQ, 10*sizeof(CASCMD)) != BUF_STATUS_OK )
   {
      return ( CAS_CTL_FAILURE );
   }
   
   /* Create the semaphore block mutex */
   if ( bosMutexCreate( "SemBlkMutex", &semBlockMutex ) != BOS_STATUS_OK )
   {
      memCircBufFree(&casCtlCmdQ);
      return ( CAS_CTL_FAILURE );
   }

   for( i = CAS_DEFAULT_BLOCK_SEM_MAX; i; i-- )
   {
      /* If semaphore creation failed, delete the list and exit */
      if ( casCtlCreateCmdSempahore() == CAS_CTL_FAILURE )
      {
         casCtlDestroyCmdSempahoreList();
         memCircBufFree(&casCtlCmdQ);
         bosMutexDestroy( &semBlockMutex );
         return ( CAS_CTL_FAILURE );
      }
   }


   /*
   ** Create a thread "HCAS" from which to run the CAS service(s)
   */
   if ( BOS_STATUS_OK != bosTaskCreateEx(    "HCAS",
                                             (8 * 1024),
                                             tskPrio,
                                             casTaskInitCB,
                                             casTaskMain,
                                             casTaskDeinitCB,
                                             0,
                                             &casState.taskId ))
   {
      casCtlDestroyCmdSempahoreList();
      memCircBufFree(&casCtlCmdQ);
      bosMutexDestroy( &semBlockMutex );
      return ( CAS_CTL_FAILURE );
   }

   return ( CAS_CTL_SUCCESS );
}


/****************************************************************************
*
*  casCtlEndptInit - Per-endpoint init - call once per endpoint to enable CAS
*
*****************************************************************************
*
*  PURPOSE:
*     Per-endpoint init - call once per endpoint to enable CAS
*
*  PARAMETERS:
*     channelType (in)  Indicates interface used, CASTYPE_FXO or CASTYPE_FXS
*     casDriver   (in)  Driver associated with endpoint.
*     handle      (out) Returned handle that should be used in API calls.
*     fxoParms    (in)  FXO parameters (such as timing for hook state, pulse
*                       dial and hook flash, wink interval, etc.)
*
*  RETURNS:
*     SUCCESS
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlEndptInit
(
   CAS_TYPE channelType, 
   XDRV_CAS *casDriver,
   CAS_CTL_HANDLE *handle,
   CAS_CTL_FXO_PARMS *fxoParms,
   CAS_CTL_SLIC_CALLBACK casCallback
)
{
   VRG_UINT16  idx;

   for( idx = 0; idx < CAS_CTL_CFG_NUM_ENDPTS; idx++ )
   {
      if ( !casState.service[ idx ].inUse )
      {
         casState.service[ idx ].inUse = VRG_TRUE;

         /* The return "handle" in just the channel number index. */
         *handle = (CAS_CTL_HANDLE) idx;
         break;
      }
   }

   if ( idx == CAS_CTL_CFG_NUM_ENDPTS )
   {
      return ( CAS_CTL_FAILURE );
   }

   /* Configure to control and monitor either FXO or FXS Interface */
   casCmdAsync( idx, CAS_CONFIG, CAS_REQ_NONE, channelType, idx );

   /* Save Pointer to the CAS Driver Service */
   casState.service[idx].localmem.channelinfo.casDriver = casDriver;

   /* State change notification callback function */
   casState.service[idx].localmem.channelinfo.casCallback = casCallback;

   casState.service[idx].localmem.channelinfo.earlyOffState = 1;
   casState.service[idx].localmem.channelinfo.earlyOnState = 1;

   casState.service[idx].localmem.channelinfo.delayOffState = 1;
   casState.service[idx].localmem.channelinfo.delayOnState = 1;

   /* Configure the FXO monitoring */
   casCmdDataAsync( idx, CAS_CONFIG_FXO, CAS_REQ_NONE, sizeof(CAS_CTL_FXO_PARMS), fxoParms );

   /* Enable the CAS Service */
   casCmdAsync( idx, CAS_ENABLE, CAS_REQ_NONE, VRG_TRUE, 0 );

   /* Do not assume the initial FXO state. Instead, leave the CAS run through
   ** the state machine and detect a valid hook state on its own*/

   return ( CAS_CTL_SUCCESS );
}


/****************************************************************************
*
*  casCtlEndptDeinit - Per-endpoint deinit - call once per endpoint to disable CAS
*
*****************************************************************************
*
*  PURPOSE:
*     Per-endpoint deinit - call once per endpoint to disable CAS
*
*  PARAMETERS:
*     channelType (in)  Indicates interface used, CASTYPE_FXO or CASTYPE_FXS
*     casDriver   (in)  Driver associated with endpoint.
*     handle      (out) Returned handle that should be used in API calls.
*     fxoParms    (in)  FXO parameters (such as timing for hook state, pulse
*                       dial and hook flash, wink interval, etc.)
*
*  RETURNS:
*     SUCCESS
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlEndptDeinit(CAS_CTL_HANDLE handle)
{

   if ( handle >= CAS_CTL_CFG_NUM_ENDPTS )
   {
      return ( CAS_CTL_FAILURE );
   }

   casState.service[handle].inUse = VRG_FALSE;

   casState.service[handle].localmem.channelinfo.casCallback = NULL;

   casState.service[handle].localmem.channelinfo.earlyOffState = 0;
   casState.service[handle].localmem.channelinfo.earlyOnState = 0;

   casState.service[handle].localmem.channelinfo.delayOffState = 0;
   casState.service[handle].localmem.channelinfo.delayOnState = 0;

   VRG_LOG_DBG(( VRG_LOG_MOD_CAS, "CAS shutdown complete!" ));

   VRG_LOG_DEREGISTER( VRG_LOG_MOD_CAS );

   return ( CAS_CTL_SUCCESS );
}



/****************************************************************************
*
*  casCtlDeinit - CAS controller module de-initialization
*
*****************************************************************************
*
*  PURPOSE:
*     CAS module deinit  - call once during system shutdown. This will
*                          shutdown the CAS task and free all resources
*                          used by the CAS controller. (i.e. this function
*                          should free all resources allocated in casCtlInit()
*                          and casCtlEndptInit()).
*
*  PARAMETERS: none
*
*  RETURNS:    CAS_CTL_SUCCESS or CAS_CTL_FAILURE
*              This function should only return an error under catastrophic
*              circumstances. i.e. Something that cannot be fixed by re-invoking
*              the module initialization function.
*
*  NOTE:       It is assumed that this function is only called after the CAS
*              task has been notified of a pending application reset, and it
*              has acknowledged the notification. This implies that the CAS
*              task is currently blocked, waiting to be resumed so that they may
*              complete the shutdown procedure.
*
*              It is also assumed that no task is currently blocked on any OS
*              resource that was created in the module initialization function.
*
****************************************************************************/
CAS_CTL_STATUS casCtlDeinit( void )
{
   VRG_UINT16   chan;

   if ( casState.taskId )
   {
      /* Currently, the CAS task should be blocked in the reset-acknowledgement state.
       * Advance the CAS task to the reset-active state. This will cause the task to
       * resume operation. In this state, the CAS task should perform any operations
       * required for proper shutdown before exiting from it's main task function. */
      bosTaskProceedToShutdown( &casState.taskId );

      /* Wait for the CAS task to complete de-initialization */
      bosTaskWaitForDone( &casState.taskId );
   }

   /* Destroy queue for storing casCtl commands */
   if ( memCircBufFree(&casCtlCmdQ) != BUF_STATUS_OK )
   {
      return ( CAS_CTL_FAILURE );
   }

   /* Destroy the block semaphore list */
   casCtlDestroyCmdSempahoreList();

   /* Destroy the semaphore block mutex */
   bosMutexDestroy( &semBlockMutex );
   
   /* Clean up */
   for ( chan = 0; chan < casState.numChannels; chan++ )
   {
      /* Disable the CAS Service */
      casCmdSync( chan, CAS_REQ_NONE, CAS_ENABLE, VRG_FALSE, 0 );

      /* Cleanup resources. */
      casCmdSync( chan, CAS_REQ_NONE, CAS_EXIT, 0, 0 );
   }

   VRG_LOG_DBG(( VRG_LOG_MOD_CAS, "CAS shutdown complete!" ));

   VRG_LOG_DEREGISTER( VRG_LOG_MOD_CAS );

   return ( CAS_CTL_SUCCESS );
}


/****************************************************************************
*
*  casCmdSync - Synchronously send command to the CAS module (non-extended data)
*
*****************************************************************************
*
*  PURPOSE:
*     Send command to the CAS module (non-extended data)
*
*  PARAMETERS:
*     chan - channel to send the command to
*     cmd  - CAS command
*     op1  - first operand of the CAS command
*     op2  - second operand of the CAS command
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*     This command should either be called before the CAS main task is 
*     created or after the CAS main task is terminated.
*
****************************************************************************/
static CAS_CTL_STATUS casCmdSync( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, VRG_UINT16 op2 )
{
   CASCMD command;
   CAS_CTL_STATUS status;

   /* Allocate memory for the flags field */
   command.flags = malloc( sizeof(VRG_UINT8) );
   if ( !command.flags )
   {
      return ( CAS_CTL_FAILURE );
   }

   command.devhdl = chan;
   command.cmd = cmd;
   command.reqid = reqid;
   *command.flags = 0;
   command.op1 = op1;
   command.op2.val = op2;

   status = casIO( &casState.service[chan].localmem, &command );
   
   /* Free flags field */
   free(command.flags);
   
   return status;
}


/****************************************************************************
*
*  casCmdSyncSerial - Synchronously send command to the CAS module (non-extended data)
*
*****************************************************************************
*
*  PURPOSE:
*     Send command to the CAS module (non-extended data)
*
*  PARAMETERS:
*     chan - channel to send the command to
*     cmd  - CAS command
*     op1  - first operand of the CAS command
*     op2  - second operand of the CAS command
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*     This function lets the CAS task process the command and forward the
*     response. The synchrounous command is set to the CAS task for processing.
*     The CAS task sends a response to the caller when it is done processing
*     the command.
*
****************************************************************************/
static CAS_CTL_STATUS casCmdSyncSerial( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, VRG_UINT16 op2 )
{
   CASCMD command;
   CAS_CTL_STATUS cmdStatus;
   CAS_CTL_SEM_BLOCK *casCtlSemBlock;
   BOS_STATUS status;
   
   /* Allocate memory for the flags field */
   command.flags = malloc( sizeof(VRG_UINT8) );
   if ( !command.flags )
   {
      return ( CAS_CTL_FAILURE );
   }

   /* Get a command semaphore block */
   casCtlSemBlock = casCtlGetCmdSemaphore();
   if ( !casCtlSemBlock )
   {
      free(command.flags);
      return ( CAS_CTL_FAILURE );
   }

   command.devhdl = chan;
   command.cmd = cmd;
   command.reqid = reqid;
   command.status = &cmdStatus;
   *command.flags = 0;
   command.op1 = op1;
   command.op2.val = op2;
   command.semaphore = casCtlSemBlock->semaphore;


   if ( memCircBufWrite(casCtlCmdQ, (char*)&command, sizeof(CASCMD)) != BUF_STATUS_OK )
   {
      free(command.flags);
      /* Return semaphore block to the semaphore block list */
      casCtlPutCmdSempahore(casCtlSemBlock);
      return ( CAS_CTL_FAILURE );
   }

   /* Wait for response from CAS */
   status = bosSemTake( casCtlSemBlock->semaphore );

   /* Return semaphore block to the semaphore block list */
   casCtlPutCmdSempahore(casCtlSemBlock);
   
   /* Free flags field */
   free(command.flags);
   
   return ( status == BOS_STATUS_OK ) ? *command.status : CAS_CTL_FAILURE;
}


/****************************************************************************
*
*  casCmd - Asynchronously send command to the CAS module
*
*****************************************************************************
*
*  PURPOSE:
*     Send command to the CAS module (non-extended data)
*
*  PARAMETERS:
*     chan - channel to send the command to
*     cmd  - CAS command
*     op1  - first operand of the CAS command
*     op2  - second operand of the CAS command
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
static CAS_CTL_STATUS casCmdAsync( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, VRG_UINT16 op2 )
{
   CASCMD command;

   /* Allocate memory for the flags field */
   command.flags = malloc( sizeof(VRG_UINT8) );
   if ( !command.flags )
   {
      return ( CAS_CTL_FAILURE );
   }

   command.devhdl = chan;
   command.cmd = cmd;
   command.reqid = reqid;
   *command.flags = CAS_CMD_FLAG_ASYNC;
   command.op1 = op1;
   command.op2.val = op2;

   if ( memCircBufWrite(casCtlCmdQ, (char*)&command, sizeof(CASCMD)) != BUF_STATUS_OK )
   {
      free(command.flags);
      return ( CAS_CTL_FAILURE );
   }

   return ( CAS_CTL_SUCCESS );
}


/****************************************************************************
*
*  casCmdResp - Send command with an expected response to the CAS module
*
*****************************************************************************
*
*  PURPOSE:
*     Send command with response to the CAS module
*
*  PARAMETERS:
*     chan - channel to send the command to
*     cmd  - CAS command
*     op1  - size of the response data
*     op2  - pointer to the response data
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*     This is a synchronous call.
*
****************************************************************************/
static CAS_CTL_STATUS casCmdResp( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT32 op1, void* op2 )
{
   return casCmdDataSync(chan, cmd, CAS_REQ_NONE, op1, op2);
}


/****************************************************************************
*
*  casCmdDataSync - Send a synchronous command with  data to the CAS module
*
*****************************************************************************
*
*  PURPOSE:
*     Send a synchronous command with  data to the CAS module
*
*  PARAMETERS:
*     chan - channel to send the command to
*     cmd  - CAS command
*     op1  - size of the  data to be sent
*     op2  - pointer to the  data
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*     Since the command is handled asynchronously, we make a copy of the 
*      data. The allocated data must be freed when the command 
*     handler has finished processing the command.
****************************************************************************/
static CAS_CTL_STATUS casCmdDataSync( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, void* op2 )
{
   CASCMD command;
   CAS_CTL_STATUS cmdStatus; 
   CAS_CTL_SEM_BLOCK *casCtlSemBlock;
   BOS_STATUS status;
   void *bmbuffer;

   /* Allocate memory for the flags field */
   command.flags = malloc( sizeof(VRG_UINT8) );
   if ( !command.flags )
   {
      return ( CAS_CTL_FAILURE );
   }

   /* Make a copy of the data so the command can be processed synchronously */
   bmbuffer = malloc( op1 );
   if ( !bmbuffer )
   {
      free(command.flags);
      return ( CAS_CTL_FAILURE );
   }
   
   memcpy( bmbuffer, op2, op1 );

   /* Get a command semaphore block */
   casCtlSemBlock = casCtlGetCmdSemaphore();
   if ( !casCtlSemBlock )
   {
      free(command.flags);
      free(bmbuffer);
      return ( CAS_CTL_FAILURE );
   }

   /* Contruct the command */
   command.devhdl = chan;
   command.cmd = cmd;
   command.reqid = reqid;
   command.status = &cmdStatus;
   *command.flags = CAS_CMD_FLAG_EXTENDTED;
   command.op1 = op1;
   command.op2.ref = bmbuffer;
   command.semaphore = casCtlSemBlock->semaphore;
   
   /* Write the message to the circular buffer */
   if ( memCircBufWrite(casCtlCmdQ, (char*)&command, sizeof(CASCMD)) != BUF_STATUS_OK )
   {
      free(command.flags);
      free(bmbuffer);
      /* Return semaphore block to the semaphore block list */
      casCtlPutCmdSempahore(casCtlSemBlock);
      return ( CAS_CTL_FAILURE );
   }
   
   /* Wait for response from CAS */
   status = bosSemTake( casCtlSemBlock->semaphore );

   /* Return semaphore block to the semaphore block list */
   casCtlPutCmdSempahore(casCtlSemBlock);
   
   if ( status == BOS_STATUS_OK )
   {
      /* If buffer is modified copy content back to original */
      if ( CAS_CMD_FLAG_IS_MODIFIED(command.flags) )
      {
         memcpy(op2, command.op2.ref, op1);
      }
      
      /* Free buffer if CAS no longer needs it */
      if ( CAS_CMD_FLAG_IS_DROP(command.flags) )
      {
         if ( command.op2.ref ) 
         {
            free(command.op2.ref);
         }
      }
   }

   /* Free flags field */
   free(command.flags);
      
   return ( status == BOS_STATUS_OK ) ? *command.status : CAS_CTL_FAILURE;
}


/****************************************************************************
*
*  casCmdDataAsync - Send an asynchronous command with  data to the CAS module
*
*****************************************************************************
*
*  PURPOSE:
*     Send an asynchronous command with  data to the CAS module
*
*  PARAMETERS:
*     chan - channel to send the command to
*     cmd  - CAS command
*     op1  - size of the  data to be sent
*     op2  - pointer to the  data
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*     Since the command is handled asynchronously, we make a copy of the 
*      data. The allocated data must be freed when the command 
*     handler has finished processing the command.
*
****************************************************************************/
static CAS_CTL_STATUS casCmdDataAsync( VRG_UINT16 chan, VRG_UINT16 cmd, VRG_UINT16 reqid, VRG_UINT32 op1, void* op2 )
{
   CASCMD command;

   void *bmbuffer;
   
   /* Allocate memory for the flags field */
   command.flags = malloc( sizeof(VRG_UINT8) );
   if ( !command.flags )
   {
      return ( CAS_CTL_FAILURE );
   }

   /* Make a copy of the data so the command can be processed synchronously */
   bmbuffer = malloc( op1 );
   if ( !bmbuffer )
   {
      free(command.flags);
      return ( CAS_CTL_FAILURE );
   }

   memcpy( bmbuffer, op2, op1 );

   command.devhdl = chan;
   command.cmd = cmd;
   command.reqid = reqid;
   *command.flags = (CAS_CMD_FLAG_ASYNC | CAS_CMD_FLAG_EXTENDTED);
   command.op1 = op1;
   command.op2.ref = bmbuffer;

   if ( memCircBufWrite(casCtlCmdQ, (char*)&command, sizeof(CASCMD)) != BUF_STATUS_OK )
   {
      free(command.flags);
      free(bmbuffer);
      return ( CAS_CTL_FAILURE );
   }

   return ( CAS_CTL_SUCCESS );
}


/****************************************************************************
*
*  casTaskInitCB
*
*****************************************************************************
*
*  PURPOSE:    CAS task initialization function. This function will be
*              automatically called by the OS prior to invoking the
*              CAS task's main function. Task OS resource allocation and
*              initialization may be performed here.
*
*  PARAMETERS: none
*
*  RETURNS:    none
*
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
*
****************************************************************************/
static void casTaskInitCB( BOS_TASK_ARG taskArg )
{
   /* Nothing to do here */
}


/****************************************************************************
*
*  casTaskDeinitCB
*
*****************************************************************************
*
*  PURPOSE:    CAS task deinitialization function. This function will be
*              automatically called by the OS after the CAS task's main
*              function has exited. Any task OS resources allocated by the
*              task initialization function (casTaskInitCB) should be
*              de-allocated here.
*
*  PARAMETERS: none
*
*  RETURNS:    none
*
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
*
****************************************************************************/
static void casTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   /* Nothing to do here */
}


/****************************************************************************
*
*  casTaskMain - Main loop for the CAS service task
*
*****************************************************************************
*
*  PURPOSE:
*     Main loop for the CAS service
*
*  PARAMETERS:
*     context -
*
*  RETURNS:
*     Nothing
*
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
*
****************************************************************************/
static void casTaskMain( BOS_TASK_ARG context )
{
   int         chan;
   int         i;
   int         casHistLength;
   BOS_STATUS  status;
   XDRV_CAS   *casDriver;
   CASCMD      command;
   CAS_CTL_STATUS cmdStatus;

   while(1)
   {
      status = bosSleep( CAS_TASK_RATE );

      if ( status == BOS_STATUS_RESET )
      {
         /* The CAS task has been notified that a reset is pending. Acknowledge
            * the notification and then block until the CAS task is resumed. */
         bosTaskResetAck();


         /* The CAS task should stop running by exiting from this main function */
         return;
      }
      else if ( status == BOS_STATUS_EXIT )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_CAS, "Exiting task HCAS" ));
         return;
      }

      /* Process commands stored in casCtlCmdQ */
      while( memCircBufRead(casCtlCmdQ, (char*)&command, sizeof(CASCMD)) == BUF_STATUS_OK )
      {
         cmdStatus = casIO(&casState.service[command.devhdl].localmem, &command);
         
         /* Check if asynchronous command */
         if ( CAS_CMD_FLAG_IS_ASYNC(command.flags) )
         {
            /* Free extended data if CAS no longer needs it */
            if ( CAS_CMD_FLAG_IS_EXTENDED(command.flags) && CAS_CMD_FLAG_IS_DROP(command.flags) )
            {
               if ( command.op2.ref ) 
               {
                  free(command.op2.ref);
               }
            }
            
            /* Free flags field */
            free(command.flags);
         }
         else
         {
            /* Copy the status to the command and wakeup the calling thread */
            *command.status = cmdStatus;
            bosSemGive( command.semaphore );
         }
      }

      /* The CAS task is in the running state, perform normal CAS processing. */
      for( chan = 0; chan < casState.numChannels; chan++ )
      {
         if (casState.service[chan].localmem.channelinfo.castype == CAS_TYPE_INACTIVE)
            continue;

         /* Verify enabled */
         if( casState.service[chan].localmem.status & CAS_ENABLED )
         {
            /* Get the size of the CAS history logged at the CAS history block rate */
            if (casState.service[chan].localmem.channelinfo.castype == CAS_TYPE_FXO)
            {
               casDriver = casState.service[chan].localmem.channelinfo.casDriver;
               casHistLength = xdrvCasNumHookSamples( casDriver );
            }
            else
            {
               casHistLength = 1;
            }

            /* Process available history */
            for ( i=0; i < casHistLength; i++ )
            {
               casBlk( &casState.service[chan] );
            }
         }
      }

      /* Staggered ringing state machine */
      StaggerBlk();
   }
}


/****************************************************************************
*
*  casSetRingPattern - Set ring profile according to provisioned bit pattern
*
*****************************************************************************
*
*  PURPOSE:
*     Set ring profile according to provisioned bit pattern
*
*  PARAMETERS:
*     ringPattern       - Ring pattern state machine to update
*     ringPatternBits   - Provisioned ring pattern bits
*     clidTxDelay       - Delay to insert after first ring off
*     ringType          - ring type (short or long ring cadence)
*
*  RETURNS:
*     Nothing
*
*  NOTE: For provisioned ring values to provide caller ID, the "ring-off"
*        period should be ~4.0 sec.  This 4 second window must be found
*        within the specified ring pattern and not between two repeating
*        ring cadence.
*
****************************************************************************/
static void casSetRingPattern( CASSTM *ringPattern,
                               const CAS_CTL_PROV_PATTERN *ringPatternBits,
                               VRG_UINT32 clidTxDelay,
                               CAS_CTL_PROV_RING_TYPE ringType )
{
   VRG_UINT32            fxoBits;
   VRG_UINT32            nextStateBits;
   VRG_UINT16            ringStateBits;
   VRG_UINT16            casBits;
   VRG_UINT16            intervalNum;
   VRG_UINT16            stateNum;
   VRG_UINT16            nCompoundStates;
   int                   bIntervalToneOn;
   int                   numIntervals;
   VRG_BOOL              bFirstRingOffIsSet;
   VRG_BOOL              bRepeatingPattern;
   VRG_BOOL              bLastInterval;
   VRG_UINT32            thisBit;
   VRG_UINT32            nextBit;
   CAS_CTL_PROV_PATTERN  patternBits;   /* working version of original ringPatternBits */
   int                   bitDuration;

   /* Pointer to the most and least significant word of the ring pattern
   ** for easier access */
   VRG_UINT32           *pFirstWord;
   VRG_UINT32           *pLastWord;
   
   VRG_UINT16           repeatState = 0;

   /* Update CAS control variables based on the ring type */
   switch ( ringType )
   {
      /* Short ring cadence */
      case CAS_CTRL_SHORT_BRIEF_RING:
      case CAS_CTRL_SHORT_NORMAL_RING:
      {
         /* Determine the maximum number of intervals */
         numIntervals = CAS_CTL_PROV_SHORT_RING_MAX_INTERVAL;

         /* Specify the bit duration (in ms) */
         bitDuration = CAS_CTL_PROV_SHORT_RING_BIT_DURATION;

         /* 1st unused bit (61st) identifies whether this is a non-repeating
         ** pattern */
         bRepeatingPattern = !( ringPatternBits->shortRingPattern.pattern2 & 0x08 );

         /* Mask out unused bits.  Only the first 60 bits contain valid ring
         ** cadence information */
         patternBits.shortRingPattern.pattern1 = ringPatternBits->shortRingPattern.pattern1;
         patternBits.shortRingPattern.pattern2 = ringPatternBits->shortRingPattern.pattern2 & ~(0xF);

         /* Update pointers for easy access */
         pFirstWord = &(patternBits.shortRingPattern.pattern1);
         pLastWord = &(patternBits.shortRingPattern.pattern2);

         /* If the ring pattern is non-zero, align it such that is starts with a tone on.
         ** Leading zeros are ignored so shorter ring-pattern can be specified. */
         if ( *pFirstWord || *pLastWord )
         {
            /* Shift recursively until we reach a tone on */
            while ( !( *pFirstWord & MSB_MASK ) )
            {
               leftShift( pFirstWord, pLastWord );
               numIntervals--;
            }
         }
      }
      break;

      /* Long ring cadence */
      case CAS_CTRL_LONG_BRIEF_RING:
      case CAS_CTRL_LONG_NORMAL_RING:
      {
         int            numWords;
         int            bitOffset;
         VRG_UINT32     mask;
         int            i;
         VRG_UINT32    *pCurrentWord;

         /* Determine the maximum number of intervals.  The first octet of
         ** the provisioned bit pattern represents the length in bits of
         ** the cadence */
         numIntervals = ( ringPatternBits->longRingPattern.pattern1 & MS2BYTES_MASK ) >> 16;
         XCHG_ASSERT( numIntervals <= CAS_CTL_PROV_LONG_RING_MAX_INTERVAL );

         /*
         ** If the provisioned bit pattern is of 0-bits long, update the internal
         ** CAS state machine accordingly and return immediately
         */
         if ( numIntervals == 0 )
         {
            ringPattern->state[0].duration  = 0;
            ringPattern->state[0].stateinfo = ( FXOBITS_IDLE | CAS_ENDING_STATE | CASBITS_LCF );
            return;
         }

         /* Specify the time interval for each bit */
         bitDuration = CAS_CTL_PROV_LONG_RING_BIT_DURATION;


         /* 1st bit of the third octet identifies whether this is a non-repeating
         ** pattern */
         bRepeatingPattern = !( ( ringPatternBits->longRingPattern.pattern1 &
                                  CAS_CTL_PROV_LONG_RING_BIT_REPEAT ) >> 15 );
         /* Last 7 bits of the third octet is the ring state that the repeat pattern should start */                         
         repeatState = ((ringPatternBits->longRingPattern.pattern1 & CAS_CTL_PROV_LONG_RING_REPEAT_STATE_MASK )) >> 8;
                       
         patternBits.longRingPattern = ringPatternBits->longRingPattern;

         /* Mask out unused bits:
         **    1. First 3 octets do not contain any ring profile information
         **    2. Information specified 'numIntervals' bits after the second
         **       octet should be discarded since it does not contain any
         **       valid ring cadence information.
         **/

         /*
         ** 1. Remove the first 3 octets
         */
         i = 0;
         while ( i < 24 )
         {
            leftShift( &(patternBits.longRingPattern.pattern1), &(patternBits.longRingPattern.pattern10) );
            i++;
         }

         /* Update pointer to point to the first word with valid ring cadence */
         pFirstWord = &(patternBits.longRingPattern.pattern1);

         /*
         ** 2. Mask out all bits after 'numIntervals' bits from the second octet.
         **    To do so, we need to determine:
         **          - number of valid words within 'numIntervals'
         **          - bit index into the last valid word within 'numIntervals'
         */

         /* Determine the number of valid words within numIntervals */
         numWords = numIntervals / UINT32_LENGTH;

         /* Determine the offset into the last valid word within numIntervals */
         bitOffset = numIntervals % UINT32_LENGTH;

         /* Discard all invalid bits within the last valid word */
         if ( bitOffset > 0 )
         {
            /* Create a bitmask to discard all invalid bits */
            mask = ~(VRG_UINT32)( (1 << (32 - bitOffset)) -1 );

            /* Update pointer to point to the last valid word within numIntervals */
            pLastWord = pFirstWord + numWords;

            /* Remove all invalid bits from the last word */
            *pLastWord &= mask;
         }

         /* Last valid bit lies on a 32-bit boundary */
         else
         {
            if ( numWords > 0 )
            {
               /* Update pointer to point to the last valid word within numIntervals */
               pLastWord = pFirstWord + numWords - 1;
            }
            else
            {
               /* Ring cadence is 0-bits long */
               pLastWord = pFirstWord;
            }
         }

         /* Discard all information stored after the last valid word */
         pCurrentWord = pLastWord;
         while ( pCurrentWord != &( patternBits.longRingPattern.pattern10 ) )
         {
            pCurrentWord++;
            *pCurrentWord = 0;
         }
      }
      break;

      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_CAS, "Unrecognized package - cannot set ring pattern"));
         return;
      }
      break;
   }

   bFirstRingOffIsSet = VRG_FALSE;
   stateNum = 0;

   /* Set ring pattern based on the 100ms interval bits */
   for ( intervalNum = 0; intervalNum < numIntervals; intervalNum++ )
   {
      /* This bit represents at a minimum 1 state, possibly more if subsequent
       * bits are equal. */
      nCompoundStates = 1;

      bIntervalToneOn = *pFirstWord & MSB_MASK;

      if ( bIntervalToneOn )
      {
         ringStateBits = CASEVENT_RINGON;
         casBits = CASBITS_RINGING;
      }
      else
      {
         ringStateBits = CASEVENT_RINGOFF;
         casBits = CASBITS_LCF;
      }

      /* if this is not the last bit of the ring pattern, group any upcoming
         bits that are the same as the current bit into one state */
      while ( intervalNum < numIntervals - 1 )
      {
         thisBit = *pFirstWord & MSB_MASK;
         nextBit = ( *pFirstWord << 1 ) & MSB_MASK;

         if ( thisBit == nextBit )
         {
            /* left shift and update corresponding counters */
            leftShift( pFirstWord, pLastWord );

            nCompoundStates++;
            intervalNum++;
         }
         else
         {
            break;
         }
      }

      /* Check if ringoff has not been set yet and this is an 'off' interval > MIN_RINGOFF_MS */
      if ( !bFirstRingOffIsSet &&
           !bIntervalToneOn &&
           (( nCompoundStates * bitDuration ) >= CAS_MIN_RINGOFF_MS ))
      {
         /* 2 sec "ring-off" period - provide the caller ID */
         bFirstRingOffIsSet = VRG_TRUE;
         ringStateBits = CASEVENT_FIRST_RINGOFF;
      }

      if (( ringStateBits == CASEVENT_FIRST_RINGOFF ) &&
          ( clidTxDelay < CAS_MIN_RINGOFF_MS ) &&
          ( clidTxDelay != 0 ))
      {
         fxoBits = FXOBITS_ALERTING;

         /* Set state info */
         nextStateBits = (VRG_UINT16)(CAS_NEXTSTATE_1 + (stateNum << CAS_NEXTBITS_SHIFT));

         /* The state duration is determined by the clidTxDelay */
         ringPattern->state[stateNum].duration = clidTxDelay;
         ringPattern->state[stateNum].stateinfo = ( fxoBits | nextStateBits | ringStateBits | casBits );

         /* Increment stateNum */
         stateNum++;
         /* Increment the repeat state as a new state for caller id is added */
         if ( stateNum <= repeatState)
         {
            repeatState++;
         } 
      }

      fxoBits = FXOBITS_ALERTING;
      bLastInterval = ( intervalNum == ( numIntervals - 1 ));
      if ( bLastInterval && bRepeatingPattern )
      {
         /* End of repeating pattern, next state loops back to specified state */
         nextStateBits = ( VRG_UINT16 )(repeatState << CAS_NEXTBITS_SHIFT);   
      }
      else
      {
         nextStateBits = (VRG_UINT16)(CAS_NEXTSTATE_1 + (stateNum << CAS_NEXTBITS_SHIFT));
      }

      /*
      ** Set state duration and info
      */
      if (( ringStateBits == CASEVENT_FIRST_RINGOFF ) &&
          ( clidTxDelay < CAS_MIN_RINGOFF_MS ) &&
          ( clidTxDelay != 0 ))
      {
         /* The timer expire event must be checked */
         ringStateBits = CASEVENT_TIMER_EXPIRE;

         /* The state duration must be offset by the clidTxDelay */
         ringPattern->state[stateNum].duration = bitDuration * nCompoundStates - clidTxDelay;
         ringPattern->state[stateNum].stateinfo = ( fxoBits | nextStateBits | ringStateBits | casBits );
      }
      else
      {
         /* The state duration is determined by the number of grouped states */
         ringPattern->state[stateNum].duration = bitDuration * nCompoundStates;
         ringPattern->state[stateNum].stateinfo = ( fxoBits | nextStateBits | ringStateBits | casBits );
      }

      /* left shift, and increment stateNum */
      leftShift( pFirstWord, pLastWord );
      stateNum++;


      if ( bLastInterval && !bRepeatingPattern )
      {
         /*
         ** End of non-repeating pattern, transition back to IDLE mode again
         ** (to save power) by adding dummy state at the end of the state
         ** machine.
         */
         ringPattern->state[stateNum].duration  = 0;
         ringPattern->state[stateNum].stateinfo = FXOBITS_IDLE | CAS_ENDING_STATE | CASBITS_RINGING;

         if ( ( ringType == CAS_CTRL_SHORT_BRIEF_RING ) ||
              ( ringType == CAS_CTRL_LONG_BRIEF_RING ) )
         {
            /* If this is a ring-splash, we need to append a 'ring splash complete'
            ** event to the last state to indicate that ring splash is done */
            ringPattern->state[stateNum].stateinfo |= CASEVENT_BRIEF_RING;
         }
      }
   } /* End for intervalnum < numIntervals */
} /* End casSetRingPattern */


/****************************************************************************
*
*  leftShift - Perform left shift operation
*
*****************************************************************************
*
*  PURPOSE:
*     Perform left shift operation from pCurrentWord up to and including
*     pLastWord
*
*  PARAMETERS:
*     pCurrentWord - first word to perform left shift operation
*     pLastWord    - last word to perform left shift operation
*
*  RETURNS:
*     Nothing
*
****************************************************************************/
static void leftShift( VRG_UINT32 *pCurrentWord, VRG_UINT32 *pLastWord )
{
   VRG_UINT32  *pNextWord;
   VRG_UINT32  *pWord;

   /* Perform left shift operation */
   pWord = pCurrentWord;

   while ( pWord != pLastWord )
   {
      *pWord <<= 1;

      pNextWord = pWord + 1;
      if ( *pNextWord & MSB_MASK )
      {
         *pWord |= 0x1;
      }

      pWord++;
   }

   /* Shift last word */
   *pLastWord <<= 1;
}


/*
*****************************************************************************
** FUNCTION:   EventCallback
**
** PURPOSE:    This callback receives all events generated by the internal CAS
**             state machine. It translates the internal event into one that
**             is part of the public CAS control API, and invokes the user
**             supplied callback.
**
** PARAMETERS: event    - state machine generated event.
**             notused  -
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void EventCallback( CASEVT *eventp, VRG_UINT16 notused )
{
   CAS_CTL_EVENT_TYPE   eventType   = eventp->evt;
   CAS_CTL_EVENT        event       = eventp->op1;
   CAS_CTL_HANDLE       handle      = eventp->op2.val;

   casState.callback( handle, eventType, event );
}


/*
*****************************************************************************
** FUNCTION:   GetIndexFromHandle
**
** PURPOSE:    To translate an abstract CAS control handle into a channel
**             number index. A handle is returned to the client when casCtlEndptInit()
**             is called. This handle should be used by the client in all
**             subsequent API calls to the CAS control module. This function
**             returns the channel number index associated with the handle for
**             internal use.
**
** PARAMETERS: casHandle   (in)  CAS control handle.
**
** RETURNS:    Channel number that can be used to index array of instance memory.
**
** NOTE:
*****************************************************************************
*/
static VRG_UINT16 GetIndexFromHandle( CAS_CTL_HANDLE casHandle )
{
   /*
   ** The CAS control handle and channel number index are currently one and
   ** the same thing.
   */
   return ( (VRG_UINT16) casHandle );
}


#if CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP
/****************************************************************************
*
*  casSetNewStaggerInterval - Set the staggered interval to be used by the
*                             staggered block controller.
*
*****************************************************************************
*
*  PURPOSE:
*     Calculate the current ring cadence from the ring profile
*
*  PARAMETERS:
*     rgCadence    - ring cadence to consider for interval calculation
*
*  RETURNS:
*     Nothing
*
****************************************************************************/
static void casSetNewStaggerInterval( VRG_UINT32 rgCadence )
{
   VRG_UINT32 cadence = rgCadence;
   BOS_CRITSECT critSect;
   int i, j;
   CASSTAGGERMEM  *casStaggerState = casState.staggerState;

   /* Check if we already have a ring active with a given cadence.  If so, compare
   ** that cadence to the current one and setup the stagger interval timeout based on
   ** the minimum of these two cadences.
   */
   for (i = 0; i < MAX_NUM_STAGGER_SLOTS; i++)
   {
      for ( j = 0; j < casState.maxRingersPerSlot; j++ )
      {
         if (( casStaggerState[i].ringslot[j] != RINGSLOTEMPTY ))
         {
            if( casStaggerState[i].ringCadence[j] < rgCadence )
            {
               cadence = casStaggerState[i].ringCadence[j];
            }
         }
      }
   }

   bosCritSectEnter( &critSect );
   casState.staggerInterval = cadence / MAX_NUM_STAGGER_SLOTS;
   bosCritSectLeave( &critSect );
}


/****************************************************************************
*
*  casGetRingCadence - Calculate the ring cadence from the ring profile
*
*****************************************************************************
*
*  PURPOSE:
*     Calculate the current ring cadence from the ring profile
*
*  PARAMETERS:
*     rgProfile    - ring profile information
*
*  RETURNS:
*     Calculated ring cadence
*
****************************************************************************/
static VRG_UINT32 casGetRingCadence( CAS_CTL_RING_PROFILE *rgProfile )
{
   int                   numIntervals;
   int                   bitDuration;
   VRG_UINT32           *pFirstWord;
   VRG_UINT32           *pLastWord;
   CAS_CTL_PROV_PATTERN  patternBits;
   VRG_UINT32            rgCadence = 0;

   switch( rgProfile->ringType )
   {
      /* Static ring cadence */
      case CAS_CTRL_STATIC_RING:
      {
         VRG_COUNTRY       country;
         unsigned int      profileIndex;
         int               i;

         country        = CAS_CTL_RING_PATTERN_COUNTRY( rgProfile->pattern );
         profileIndex   = CAS_CTL_RING_PATTERN_ID     ( rgProfile->pattern );
         i              = 0;

         while( gRingPatterns[country][profileIndex].state[i].stateinfo != 0 )
         {
            rgCadence += gRingPatterns[ country ][ profileIndex ].state[i].duration;
            i++;
         }
      }
      break;

      /* Short ring cadence */
      case CAS_CTRL_SHORT_BRIEF_RING:
      case CAS_CTRL_SHORT_NORMAL_RING:
      {
         numIntervals = CAS_CTL_PROV_SHORT_RING_MAX_INTERVAL;
         bitDuration = CAS_CTL_PROV_SHORT_RING_BIT_DURATION;

         /* Mask out unused bits.  Only the first 60 bits contain valid ring
         ** cadence information */
         patternBits.shortRingPattern.pattern1 = rgProfile->provBits.shortRingPattern.pattern1;
         patternBits.shortRingPattern.pattern2 = rgProfile->provBits.shortRingPattern.pattern2 & ~(0xF);

         /* Update pointers for easy access */
         pFirstWord = &(patternBits.shortRingPattern.pattern1);
         pLastWord = &(patternBits.shortRingPattern.pattern2);

         /* If the ring pattern is non-zero, align it such that is starts with a tone on.
         ** Leading zeros are ignored so shorter ring-pattern can be specified. */
         if ( *pFirstWord || *pLastWord )
         {
            /* Shift recursively until we reach a tone on */
            while ( !( *pFirstWord & MSB_MASK ) )
            {
               leftShift( pFirstWord, pLastWord );
               numIntervals--;
            }
         }

         rgCadence = numIntervals * bitDuration;
      }
      break;

      /* Long ring cadence */
      case CAS_CTRL_LONG_BRIEF_RING:
      case CAS_CTRL_LONG_NORMAL_RING:
      {
         numIntervals = ( rgProfile->provBits.longRingPattern.pattern1 & MS2BYTES_MASK ) >> 16;
         bitDuration = CAS_CTL_PROV_LONG_RING_BIT_DURATION;
      }
      break;
   }

   if( rgCadence == 0 )
   {
      rgCadence = CAS_CTL_DEFAULT_RING_CADENCE;
      VRG_LOG_DBG(( VRG_LOG_MOD_CAS, "Null ring cadence, assigning default"));
   }

   VRG_LOG_DBG(( VRG_LOG_MOD_CAS, "Calculated ring cadence: %d msecs", rgCadence));
   return rgCadence;
}
#endif /* CAS_CTL_CFG_STAGGER_RING_DYNAMIC_SETUP */


/****************************************************************************
*
*  casCtlEnableFastSlicStateMode - Enable or disable fast SLIC mode Changes
*
*****************************************************************************
*
*  PURPOSE:
*     Enable or disable fast SLIC mode Changes.
*
*  PARAMETERS:
*     handle    - the physical interface to control
*     enable    - TRUE: enable fast SLIC mode changes 
*               - FALSE: disable fast SLIC mode changes 
*
*  RETURNS:
*     SUCCESS or FAILURE
*
****************************************************************************/
CAS_CTL_STATUS casCtlEnableFastSlicStateMode( CAS_CTL_HANDLE handle, VRG_BOOL enable )
{
   VRG_UINT16  idx = GetIndexFromHandle( handle );

   if( (casState.service[idx].localmem.channelinfo.casDriver != NULL ) && 
       (!xdrvCasEnableFastSlicStateMode( casState.service[idx].localmem.channelinfo.casDriver, enable )) )
   {
      return CAS_CTL_SUCCESS;
   }
   else
   {
      return CAS_CTL_FAILURE;
   }
}


/****************************************************************************
*
*  casCtlSendCommand - Send a command for CAS to process
*
*****************************************************************************
*
*  PURPOSE:
*     Send a command for CAS to process
*
*  PARAMETERS:
*     reqId    - Command request ID
*     mode     - Command mode (synchronous or asynchronous)
*     op1      - Command OP1
*     op2      - Command OP2 
*
*  RETURNS:
*     SUCCESS or FAILURE
*
****************************************************************************/
CAS_CTL_STATUS casCtlSendCommand(CAS_REQ_ID reqId, CAS_CMD_MODE mode, VRG_UINT32 op1, VRG_UINT16 op2)
{
   VRG_UINT16 cmd;
   
   if ( reqId > CAS_REQ_SLIC_START && reqId < CAS_REQ_SLIC_END )
   {
      cmd = CAS_SLIC_CMD;
   }
   else if ( reqId > CAS_REQ_DAA_START && reqId < CAS_REQ_DAA_END )
   {
      cmd = CAS_DAA_CMD;   
   }
   else
   {
      cmd = CAS_APM_CMD;   
   }

   return ( mode == CAS_CMD_MODE_ASYNC ) 
          ? casCmdAsync( 0, cmd, reqId, op1, op2 ) 
          : casCmdSyncSerial( 0, cmd, reqId, op1, op2 );   
}


/****************************************************************************
*
*  casCtlSendExtendedCommand - Send an extended command for CAS to process
*
*****************************************************************************
*
*  PURPOSE:
*     Send a extended command for CAS to process
*
*  PARAMETERS:
*     reqId    - Command request ID
*     mode     - Command mode (synchronous or asynchronous)
*     parms    - Extended command parameters
*
*  RETURNS:
*     SUCCESS or FAILURE
*
****************************************************************************/
CAS_CTL_STATUS casCtlSendExtendedCommand(CAS_REQ_ID reqId, CAS_CMD_MODE mode, CAS_CTL_EXT_CMD_PARM *parms)
{
   VRG_UINT16 cmd;
   
   if ( reqId > CAS_REQ_SLIC_START && reqId < CAS_REQ_SLIC_END )
   {
      cmd = CAS_SLIC_CMD;
   }
   else if ( reqId > CAS_REQ_DAA_START && reqId < CAS_REQ_DAA_END )
   {
      cmd = CAS_DAA_CMD;   
   }
   else
   {
      cmd = CAS_APM_CMD;   
   }

   return ( mode == CAS_CMD_MODE_ASYNC) 
          ? casCmdDataAsync( 0, cmd, reqId, sizeof(CAS_CTL_EXT_CMD_PARM), (void*)parms ) 
          : casCmdDataSync( 0, cmd, reqId, sizeof(CAS_CTL_EXT_CMD_PARM), (void*)parms );  
}


/****************************************************************************
*
*  casCtlCreateCmdSempahore - 
*
*****************************************************************************
*
*  PURPOSE:
*     Create a command semaphore block and add it to the command semaphore 
*     block list
*
*  PARAMETERS:
*     None
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlCreateCmdSempahore()
{
   BOS_SEM *semaphore;
   CAS_CTL_SEM_BLOCK *newCasCtlSemBlock;

   bosMutexAcquire( &semBlockMutex ); 
   {
      /* Allocate a place holder for the samaphore */
      semaphore = (BOS_SEM*)malloc( sizeof(BOS_SEM) );
      if ( !semaphore )
      {
         bosMutexRelease( &semBlockMutex ); 
         return ( CAS_CTL_FAILURE );
      }
      
      /* Create block semaphore */
      if ( bosSemCreate("CasBlkSem", 0, 1, semaphore) != BOS_STATUS_OK )
      {
         free(semaphore);
         bosMutexRelease( &semBlockMutex ); 
         return ( CAS_CTL_FAILURE );
      }

      /* Allocate a new CAS control semaphore block */   
      newCasCtlSemBlock = (CAS_CTL_SEM_BLOCK*)malloc( sizeof(CAS_CTL_SEM_BLOCK) );
      if ( !newCasCtlSemBlock )
      {
         bosSemDestroy(semaphore);
         free(semaphore);
         bosMutexRelease( &semBlockMutex ); 
         return ( CAS_CTL_FAILURE );
      }
  
      newCasCtlSemBlock->semaphore = semaphore;
  
      /* Add entry to list */
      newCasCtlSemBlock->next = casCtlSemBlockHead;
      casCtlSemBlockHead = newCasCtlSemBlock;
   }
   bosMutexRelease( &semBlockMutex );
   
   return ( CAS_CTL_SUCCESS );
} /* casCtlCreateCmdSempahore */


/****************************************************************************
*
*  casCtlDestroyCmdSempahoreList - 
*
*****************************************************************************
*
*  PURPOSE:
*     Destroy the command semaphore block list
*
*  PARAMETERS:
*     None
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlDestroyCmdSempahoreList()
{
   CAS_CTL_SEM_BLOCK *currentCasCtlSemBlock;

   bosMutexAcquire( &semBlockMutex ); 
   {
      /* Loop through the entire semaphore block list */
      while( casCtlSemBlockHead )
      {
         currentCasCtlSemBlock = casCtlSemBlockHead; 
         casCtlSemBlockHead = casCtlSemBlockHead->next;
      
         /* Destroy the block semaphore and free that allocated memory */
         bosSemDestroy(currentCasCtlSemBlock->semaphore);
         free(currentCasCtlSemBlock->semaphore);
         free(currentCasCtlSemBlock);
      }  
   }
   bosMutexRelease( &semBlockMutex );  
         
   return ( CAS_CTL_SUCCESS );
} /* casCtlDestroyCmdSempahoreList */


/****************************************************************************
*
*  casCtlGetCmdSemaphore - 
*
*****************************************************************************
*
*  PURPOSE:
*     Get a command semaphore block from the command sempahore block list
*
*  PARAMETERS:
*     None
*
*  RETURNS:
*     Pointer to the semaphore block or NULL on failure.
*
*  NOTE: 
*     The function searches for a command semaphore block in the command 
*     semaphore block list. If the list is empty, it creates a new semaphore 
*     block.
*
****************************************************************************/
CAS_CTL_SEM_BLOCK *casCtlGetCmdSemaphore()
{
   CAS_CTL_SEM_BLOCK *casCtlSemBlock;
 
   bosMutexAcquire( &semBlockMutex ); 
   {
      if ( !casCtlSemBlockHead )
      {
         /* Semaphore block list is empty. Create a new semaphore */
         /* We support recursive mutexes */
         //bosMutexRelease( &semBlockMutex );
         if ( casCtlCreateCmdSempahore() != CAS_CTL_SUCCESS )
         {
            bosMutexRelease( &semBlockMutex );
            return NULL;
         } 
         //bosMutexAcquire( &semBlockMutex ); 
      }

      /* If we get to this point, the list contains atleast one semaphore */
      casCtlSemBlock = casCtlSemBlockHead;
      casCtlSemBlockHead = casCtlSemBlockHead->next;
   }
   bosMutexRelease( &semBlockMutex );

   return casCtlSemBlock;    
} /* casCtlGetCmdSemaphore */


/****************************************************************************
*
*  casCtlPutCmdSempahore - 
*
*****************************************************************************
*
*  PURPOSE:
*     Return the command sempahore block back to the command semaphore block list
*
*  PARAMETERS:
*     casCtlSemBlock - Pointer to the command semaphore block to be resturned to 
*                      the command semaphore block list
*
*  RETURNS:
*     SUCCESS or FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casCtlPutCmdSempahore(CAS_CTL_SEM_BLOCK *casCtlSemBlock)
{
   /* Sanity check */
   if ( !casCtlSemBlock )
   {
      return ( CAS_CTL_FAILURE );
   }

   bosMutexAcquire( &semBlockMutex );
   {
      casCtlSemBlock->next = casCtlSemBlockHead;
      casCtlSemBlockHead = casCtlSemBlock;
   }
   bosMutexRelease( &semBlockMutex );
   
   return ( CAS_CTL_SUCCESS );   
} /* casCtlPutCmdSempahore */
