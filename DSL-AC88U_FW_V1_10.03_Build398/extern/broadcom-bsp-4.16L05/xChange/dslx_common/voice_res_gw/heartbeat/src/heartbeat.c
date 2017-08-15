/***************************************************************************
*    Copyright 2005  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: heartbeat.c
*
****************************************************************************
*    Description:
*
*      Heartbeat monitoring module.
*
****************************************************************************/


/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/
#include <heartbeatCfg.h>
#include <heartbeat.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <vrgEndptCfg.h>
#include <xchgAssert.h>
#include <bosMutex.h>
#include <bosEvent.h>
#include <bosTask.h>
#include <bosSleep.h>
#include <globalCfg.h>

#include <boardHalPower.h>
#include <boardHalSlic.h>

/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/

#define HB_PATIENTS_PER_ENDPT     6
#define HB_PATIENTS_PER_CNX       1
#define HB_PATIENTS_PER_MTA       1
#define HB_MAX_PATIENTS           (( VRG_ENDPT_CFG_NUM_CNX * HB_PATIENTS_PER_CNX ) + \
                                   ( VRG_ENDPT_CFG_NUM_ENDPT * HB_PATIENTS_PER_ENDPT ) + \
                                   ( HB_PATIENTS_PER_MTA ))

#define HB_INACTIVITY_DELAY_MS    2000
#define HB_SLEEP_DELAY            100
#define HB_DEBUG 0

/*
#if HB_DEBUG
#define HB_LOG VRG_LOG
#else
#define HB_LOG(a)
#endif
*/

/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

/* Heartbeat patient info */
typedef struct
{
   HBSTATE        patientState;
   HBPATIENT_TYPE patientType;
   int            patientLineId;
} HBPATIENT_INFO;


/* Heartbeat module control block */
typedef struct
{
   VRG_BOOL       bInited;
   VRG_UINT32     nRegisteredPatients;
   VRG_UINT32     nActivePatients;
   HBSTATE        notifiedState;
   HBSTATE        endpointState;
   HBPATIENT_INFO patientInfo[ HB_MAX_PATIENTS ];
   BOS_MUTEX      hbMutex;
   VRG_BOOL       bExtDeviceActive;
} HBCB;

/*
 ***************************************************************************
 * EXPORTED VARIABLES
 ***************************************************************************
*/


/*
*****************************************************************************
** LOCAL VARIABLES
*****************************************************************************
*/

static HBCB hbcb;

/* heartbeat process task id */
static BOS_TASK_ID heartbeatTaskId;

/* heartbeat event set */
static BOS_EVENT_SET hbEventSet;

/* Maximum number of events reported to the heartbeat module */
#define MAX_HB_EVTS  3
static BOS_EVENT hbEvt[ MAX_HB_EVTS ];

static BOS_EVENT  *pActiveEvt;
static BOS_EVENT  *pInactiveEvt;
static BOS_EVENT  *pWakeupEvt;

/* Single event */
#define HB_EVT_ACTIVE   ( 1 << 0 )
#define HB_EVT_INACTIVE ( 1 << 1 )
#define HB_EVT_WAKEUP   ( 1 << 2 )

/* Multiple events */
#define HB_EVT_INACTIVE_ACTIVE_WAKEUP  ( HB_EVT_ACTIVE | HB_EVT_INACTIVE | HB_EVT_WAKEUP )


#if HB_DEBUG

/* Patient name map for debug */
typedef struct HBPATIENTMAP
{
   HBPATIENT_TYPE hbPatientType;    /* Heartbeat patient type */
   const char*    hbPatientName;    /* Heartbeat patient name */
} HBPATIENTMAP;

static const HBPATIENTMAP hbPatientMap[] =
{
   { HBPATIENT_TYPE_DEFAULT,     "Default patient type" },
   { HBPATIENT_TYPE_POWER,       "AC power" },
   { HBPATIENT_TYPE_CNX,         "Cnx state" },
   { HBPATIENT_TYPE_HOOKSTATE,   "Endpt hookstate" },
   { HBPATIENT_TYPE_OOSIGNAL,    "Endpt On/Off signal" },
   { HBPATIENT_TYPE_CALLERID,    "Endpt Caller ID signal" },
   { HBPATIENT_TYPE_SIGNAL,      "Endpt signal" },
   { -1,                         "Unknown signal" }
};

#endif


/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

static void hbCheckEndpointActivity( void );
static void hbCheckDeviceActivity( void );
static void hbSetDeviceActivity( HBSTATE state );

static void hbTaskInitCB( BOS_TASK_ARG taskArg );
static void hbTaskDeinitCB( BOS_TASK_ARG taskArg );
static void hbTaskMain( BOS_TASK_ARG taskArg );

static void hbMapReceivedEvts( VRG_UINT16 *pEvtMask );


/*
*****************************************************************************
** FUNCTION:   hbInit
**
** PURPOSE:    Module initialization for the heartbeat module.
**             This module is responsible for monitoring the activity state
**             of a set of registered "patients". And reporting overall device
**             activity or inactivity.
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void hbInit( void )
{
   /* Clear the control block */
   memset( &hbcb, 0, sizeof( HBCB ));

   bosMutexCreate( "hbMu", &hbcb.hbMutex );

   /*
   ** Create a thread "HRTBEAT" from which to process the VRG Events
   */
   bosTaskCreateEx( "HRTBEAT",
                    (4 * 1024),
                    HEARTBEAT_CFG_TASK_PRIORITY,
                    hbTaskInitCB,
                    hbTaskMain,
                    hbTaskDeinitCB,
                    0,
                    &heartbeatTaskId );


   if( !heartbeatTaskId )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HBT, "Error creating HRTBEAT task"));
      return;
   }

   /* Default notified state is ACTIVE - if we become inactive we'll notify that */
   hbcb.notifiedState = HBSTATE_ACTIVE;

   /* Default endpoint state is ACTIVE - if we become inactive we'll notify that */
   hbcb.endpointState = HBSTATE_ACTIVE;

   hbcb.bInited = VRG_TRUE;

   hbcb.bExtDeviceActive = VRG_TRUE;

   VRG_LOG_INFO((VRG_LOG_MOD_HBT,  "HEARTBEAT: Initialized!" ));
}


/*
*****************************************************************************
** FUNCTION:   hbDeinit
**
** PURPOSE:    Heartbeat module shutdown - call once during system shutdown.
**             This will shutdown the heartbeat module and free all resources used.
**
** PARAMETERS: none
**
** RETURNS:    none
**
** NOTE:       It is assumed that this function is only called after the heartbeat
**             task has been notified of a pending application reset, and it
**             has acknowledged the notification. This implies that the heartbeat
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization function.
**
*****************************************************************************
*/
void hbDeInit( void )
{
   hbcb.bInited = VRG_FALSE;
   hbcb.notifiedState = HBSTATE_UNKNOWN;

   if ( heartbeatTaskId )
   {
      /* Currently, the heartbeat task should be blocked in the reset-acknowledgement state.
       * Advance the heartbeat task to the reset-active state. This will cause the task to
       * resume operation. In this state, the heartbeat task should perform any operations
       * required for proper shutdown before exiting from it's main task function. */
      bosTaskProceedToShutdown( &heartbeatTaskId );

      /* Wait for the heartbeat task to complete de-initialization */
      bosTaskWaitForDone( &heartbeatTaskId );
   }

   bosMutexDestroy( &hbcb.hbMutex );

   VRG_LOG_INFO((VRG_LOG_MOD_HBT,  "HEARTBEAT: De-initialized!" ));
}


/*****************************************************************************
** FUNCTION:   hbTaskInitCB
**
** PURPOSE:    Heartbeat task initialization function. This function will be
**             automatically called by the OS prior to invoking HRTBEAT
**             task's main function. Task OS resource allocation and
**             initialization may be performed here.
**
** PARAMETERS: taskArg - task arguments for initialization
**
** RETURNS:    Nothing
**
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
**
*****************************************************************************/
static void hbTaskInitCB( BOS_TASK_ARG taskArg )
{
   int    i;

   /* Create event set for synchronization */
   bosEventSetCreate( &heartbeatTaskId, &hbEventSet );

   /* Create events and add to event set */
   for ( i = 0; i < MAX_HB_EVTS; i++ )
   {
      bosEventCreate( &heartbeatTaskId, &hbEvt[i] );
      bosEventSetAddEvent( &hbEventSet, &hbEvt[i] );
   }

   /* Update pointers for easier access */
   pActiveEvt = &hbEvt[0];
   pInactiveEvt = &hbEvt[1];
   pWakeupEvt = &hbEvt[2];
}


/*****************************************************************************
** FUNCTION:   hbTaskDeinitCB
**
** PURPOSE:    Heartbeat task deinitialization function. This function will be
**             automatically called by the OS after HRTBEAT task's main
**             function has exited. Any task OS resources allocated by the
**             task initialization function (hbTaskInitCB)
**             should be de-allocated here.
**
** PARAMETERS: taskArg - task arguments for deinitialization
**
** RETURNS:    Nothing
**
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
**
*****************************************************************************/
static void hbTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   int   i;

   /* Destroy events */
   for ( i = 0; i < MAX_HB_EVTS; i++ )
   {
      bosEventDestroy( &hbEvt[i] );
   }

   /* Destroy event set */
   bosEventSetDestroy( &hbEventSet );

   pActiveEvt = NULL;
   pInactiveEvt = NULL;
   pWakeupEvt = NULL;
}


/****************************************************************************
*
*  hbTaskMain - HRTBEAT Task
*
*  PURPOSE:
*     This is a task which processes various HRTBEAT events.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
*
****************************************************************************/
static void hbTaskMain( BOS_TASK_ARG taskArg )
{
   BOS_STATUS  status;
   VRG_UINT16  evtMask;

   XCHG_ASSERT( pActiveEvt != NULL );
   XCHG_ASSERT( pInactiveEvt != NULL );
   XCHG_ASSERT( pWakeupEvt != NULL );

   while ( 1 )
   {
      evtMask = 0;

      /* Wait indefinitely for any triggering events */
      status = bosEventSetTimedReceive( &hbEventSet, BOS_EVENT_SET_WAIT_MODE_ANY, HB_SLEEP_DELAY );
      if ( status == BOS_STATUS_RESET )
      {
         /* A reset has been initiated. The heartbeat task should stop
         ** running by exiting from this main task function. */
         bosTaskResetAck();
         return;
      }
      else if ( status == BOS_STATUS_EXIT )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_HBT, "Exiting task HRTBEAT" ));
         return;
      }
      else if ( status != BOS_STATUS_OK )
      {
         /* If the status is not OK, it means we've timed out. In this case, just try again. */
         continue;
      }

      /* Map received event(s) into bitmask for easier access.  Note that
      ** multiple events may have been received. */
      hbMapReceivedEvts( &evtMask );
      switch ( evtMask )
      {
         /* Only the 'Wakeup' event was received.  This indicates that we have
         ** slept long enough for the asynch commands to complete.  Thus, it
         ** is safe to disable external devices now */
         case HB_EVT_WAKEUP:
         {
            bosMutexAcquire( &hbcb.hbMutex );

            if ( ( hbcb.nActivePatients == 0 ) &&
                 ( hbcb.bExtDeviceActive == VRG_TRUE ) )
            {
               boardHalPowerSavingsEnable();
               hbcb.bExtDeviceActive = VRG_FALSE;
            }

            bosMutexRelease( &hbcb.hbMutex );
         }
         break;

         /* Only the 'Inactive' event was received.  Sleep for a delay to allow
         ** any async commands to complete */
         case HB_EVT_INACTIVE:
         {
            /* Wait for inactivity timeout before disabling external devices */
            status = bosSleep( HB_INACTIVITY_DELAY_MS );
            if ( status == BOS_STATUS_RESET )
            {
               /* A reset has been initiated. The heartbeat task should stop
               ** running by exiting from this main task function. */
               bosTaskResetAck();
               return;
            }
            bosEventSend( pWakeupEvt );
         }
         break;

         /* Received multiple events consisting of 'Inactive', 'Active', and
         ** 'Wakeup'.  This scenario will only happen when, during the sleep delay,
         ** an activity as well as an inactivity happened, i.e.:
         **
         **    INACTIVE -> SLEEP -> ACTIVE -> INACTIVE -> WAKE
         **
         ** For this case, we want to sleep some more since we need to enforce
         ** that there's ample delay between the last inactive event and
         ** the time we disable external devices */
         case HB_EVT_INACTIVE_ACTIVE_WAKEUP:
         {
            bosEventSend( pInactiveEvt );
         }
         break;

         /* For all other received events, no processing is required.  The
         ** external devices can stay in the current states */
         default:
         {
         }
         break;
      }
   }
}


/*
*****************************************************************************
** FUNCTION:   hbRegisterPatient
**
** PURPOSE:    Register a patient to be monitored by the heartbeat module.
**             This patient can now update it's state via the Update function.
**
** PARAMETERS: initialState   [IN]  Initial state for this patient
**             hbType         [IN]  heartbeat patient type
**             lineId         [IN]  line associated with this patient
**                                  NOTE: If the patient is not assiocated
**                                        with a particular endpt, specify
**                                        HBPATIENT_ALL_ENDPT for this parm.
**
**                                        If the endpt is unknown but will
**                                        become available at a later time,
**                                        then specify HBPATIENT_UNKNOWN_ENDPT
**                                        for this parm and update the endpt
**                                        via hbUpdatePatientEndptId() when 
**                                        it is known.
**             pid            [OUT] Patient ID assigned to this patient
**
** RETURNS:    none
**
*****************************************************************************
*/
void hbRegisterPatient( HBSTATE initialState, HBPATIENT_TYPE hbType, 
                        int lineId, int *pid )
{
   XCHG_ASSERT( hbcb.bInited );
   XCHG_ASSERT( hbcb.nRegisteredPatients < HB_MAX_PATIENTS );

   bosMutexAcquire( &hbcb.hbMutex );

   /* Allocate next patient ID slot and initialize it accordingly */
   *pid = hbcb.nRegisteredPatients;
   hbcb.patientInfo[ *pid ].patientState = initialState;
   hbcb.patientInfo[ *pid ].patientType = hbType;
   hbcb.patientInfo[ *pid ].patientLineId = lineId;

   if ( initialState == HBSTATE_ACTIVE )
   {
      hbcb.nActivePatients++;
   }

   /* Increment # of registered patients */
   hbcb.nRegisteredPatients++;

#if HB_DEBUG
   const HBPATIENTMAP *pPatientMap = hbPatientMap;

   /* Map the prov item ID */
   while ( ( pPatientMap->hbPatientType != hbType ) &&
           ( pPatientMap->hbPatientType != HBPATIENT_TYPE_UNKNOWN ) )
   {
      pPatientMap++;
   }

   if ( pPatientMap->hbPatientType != HBPATIENT_TYPE_UNKNOWN )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_HBT,  "HEARTBEAT: Patient (%s) %d registered (%d of %d max registered)",
               pPatientMap->hbPatientName, *pid, hbcb.nRegisteredPatients,
               HB_MAX_PATIENTS ));
   }
#endif

   /* Update overall device activity */
   hbCheckDeviceActivity();

   bosMutexRelease( &hbcb.hbMutex );
}


/*
*****************************************************************************
** FUNCTION:   hbUnregisterPatient
**
** PURPOSE:    Unregister a patient from the heartbeat module.
**
** PARAMETERS: pid            [IN] Patient ID assigned to this patient
**
** RETURNS:    none
**
*****************************************************************************
*/
void hbUnregisterPatient( int pid )
{
   XCHG_ASSERT( hbcb.bInited );

   bosMutexAcquire( &hbcb.hbMutex );

   if ( hbcb.patientInfo[ pid ].patientState == HBSTATE_ACTIVE )
   {
      hbcb.nActivePatients--;
   }

   hbcb.patientInfo[ pid ].patientState = HBSTATE_INACTIVE;
   hbcb.patientInfo[ pid ].patientType = HBPATIENT_TYPE_UNKNOWN;
   hbcb.patientInfo[ pid ].patientLineId = HBPATIENT_UNKNOWN_ENDPT;

   hbcb.nRegisteredPatients--;

   bosMutexRelease( &hbcb.hbMutex );
}


/*
*****************************************************************************
** FUNCTION:   hbUpdatePatientState
**
** PURPOSE:    Updates the state of the specified patient and checks for overall
**             device activity or inactivity.
**
** PARAMETERS: pid      [IN]  The patient ID to update
**             state    [IN]  The new state for this patient
**
** RETURNS:    none
**
** NOTE:       Setting a patient state to the same state results in a NOP.
**             Setting the last patient's state with a different state then
**             it's peers to active/inactive will result in a device activity
**             change event to active/inactive being notified.
**
*****************************************************************************
*/
void hbUpdatePatientState( int pid, HBSTATE state )
{
   XCHG_ASSERT( hbcb.bInited );

   bosMutexAcquire( &hbcb.hbMutex );

   /* No state change is just a NOP */
   if ( hbcb.patientInfo[ pid ].patientState != state )
   {
      hbcb.patientInfo[ pid ].patientState = state;

      /* State has changed, update accordingly */
      if ( state == HBSTATE_INACTIVE )
      {
         VRG_LOG_DBG((VRG_LOG_MOD_HBT,  "HEARTBEAT: Patient %d Before -> INACTIVE (%d active of %d total monitored patients)",
                  (int)pid, (int)hbcb.nActivePatients, (int)hbcb.nRegisteredPatients ));

         /* This patient went inactive - decrement the number of active patients */
         XCHG_ASSERT( hbcb.nActivePatients > 0 );
         hbcb.nActivePatients--;
         VRG_LOG_DBG((VRG_LOG_MOD_HBT,  "HEARTBEAT: Patient %d After -> INACTIVE (%d active of %d total monitored patients)",
                  (int)pid, (int)hbcb.nActivePatients, (int)hbcb.nRegisteredPatients ));
      }
      else
      {
         VRG_LOG_DBG((VRG_LOG_MOD_HBT,  "HEARTBEAT: Patient %d Before -> ACTIVE (%d active of %d total monitored patients)",
                  (int)pid, (int)hbcb.nActivePatients, (int)hbcb.nRegisteredPatients ));

         /* This patient went active - increment the number of active patients */
         XCHG_ASSERT( hbcb.nActivePatients < hbcb.nRegisteredPatients );
         hbcb.nActivePatients++;
         VRG_LOG_DBG((VRG_LOG_MOD_HBT,  "HEARTBEAT: Patient %d After -> ACTIVE (%d active of %d total monitored patients)",
                  (int)pid, (int)hbcb.nActivePatients, (int)hbcb.nRegisteredPatients ));
      }

      /* Check all device activity to determine if DSP, etc. can be disabled */
      hbCheckDeviceActivity();

      /* Check endpoint activity to determine if slics can be disabled */
      hbCheckEndpointActivity();
   }

   bosMutexRelease( &hbcb.hbMutex );
}


/*
*****************************************************************************
** FUNCTION:   hbCheckEndpointActivity
**
** PURPOSE:    Check if the endpoints has become active or inactive.
**
** PARAMETERS: none
**
** RETURNS:    none
** 
** IMPORTANT NOTE!!! The call to this function should be performed
** from a protected section, e.g. section protected with mutexes. 
**
*****************************************************************************
*/
static void hbCheckEndpointActivity( void )
{
   int i;
   int powerStatusActive = -1;
   int hookStateActive = -1;

   XCHG_ASSERT( hbcb.bInited );

   /* Loop through all active patients */
   for ( i = 0; i < HB_MAX_PATIENTS; i++ )
   {
      if ( ( hbcb.patientInfo[i].patientType & HBPATIENT_TYPE_POWER ) &&
           ( hbcb.patientInfo[i].patientLineId == HBPATIENT_ALL_ENDPT ) )
      {
         if ( hbcb.patientInfo[i].patientState == HBSTATE_INACTIVE )
         {
            powerStatusActive = 0;
         }
         else
         {
            powerStatusActive = 1;
         }
      }

      if ( ( hbcb.patientInfo[i].patientType & HBPATIENT_TYPE_HOOKSTATE ) &&
           ( hbcb.patientInfo[i].patientLineId == HBPATIENT_ENDPT_TWO ) )
      {
         if ( hbcb.patientInfo[i].patientState == HBSTATE_INACTIVE )
         {
            hookStateActive = 0;
         }
         else
         {
            hookStateActive = 1;
         }
      }
   }

   /* Check if we have to notify of a endpoint state change */
   if (( hookStateActive == 0 ) && ( powerStatusActive == 0 ) && ( hbcb.endpointState == HBSTATE_ACTIVE ))
   {
      /* Disable SLICs where necessary */
      boardHalSlicPowerSave(HBPATIENT_ENDPT_TWO, VRG_TRUE );
      hbcb.endpointState = HBSTATE_INACTIVE;
   }
   else if ((( hookStateActive == 1 ) || ( powerStatusActive == 1 )) && ( hbcb.endpointState == HBSTATE_INACTIVE ))
   {
      /* Reenable SLICs where necessary */
      boardHalSlicPowerSave(HBPATIENT_ENDPT_TWO, VRG_FALSE );
      hbcb.endpointState = HBSTATE_ACTIVE;
   }

}


/*
*****************************************************************************
** FUNCTION:   hbCheckDeviceActivity
**
** PURPOSE:    Check if the overall device has become active or inactive.
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
static void hbCheckDeviceActivity( void )
{
   XCHG_ASSERT( hbcb.bInited );

   /* Check if we have to notify of an overall device state change */
   if (( hbcb.nActivePatients == 0 ) && ( hbcb.notifiedState != HBSTATE_INACTIVE ))
   {
      /* All patients are "dead" - need to inform the next of kin :) */
      VRG_LOG_INFO((VRG_LOG_MOD_HBT,  "HEARTBEAT: Device has become INACTIVE! (all %d patients dead)", (int)hbcb.nRegisteredPatients ));
      hbSetDeviceActivity( HBSTATE_INACTIVE );
   }
   else if (( hbcb.nActivePatients > 0 ) && ( hbcb.notifiedState != HBSTATE_ACTIVE ))
   {
      /* Let them know someone's alive! */
      VRG_LOG_INFO((VRG_LOG_MOD_HBT,  "HEARTBEAT: Device has become ACTIVE!" ));
      hbSetDeviceActivity( HBSTATE_ACTIVE );
   }

}


/*
*****************************************************************************
** FUNCTION:   hbSetDeviceActivity
**
** PURPOSE:    Set the overall device activity to active or inactive.
**
** PARAMETERS: state    [IN]  New device activity state to set
**
** RETURNS:    none
**
*****************************************************************************
*/
static void hbSetDeviceActivity( HBSTATE state )
{
   XCHG_ASSERT( state != HBSTATE_UNKNOWN );
   XCHG_ASSERT( hbcb.notifiedState != state );

   bosMutexAcquire( &hbcb.hbMutex );

   hbcb.notifiedState = state;

   switch ( state )
   {
      case HBSTATE_ACTIVE:
      {
         /* Device has become active, immediately take action */
         VRG_LOG_INFO((VRG_LOG_MOD_HBT,  "HEARTBEAT: DEVICE ACTIVE NOW!" ));
         if ( hbcb.bExtDeviceActive == VRG_FALSE )
         {
            boardHalPowerSavingsDisable();
            hbcb.bExtDeviceActive = VRG_TRUE;
         }

         /* Notify heartbeat module of activity */
         bosEventSend( pActiveEvt );
      }
      break;

      case HBSTATE_INACTIVE:
      {
         /* Device has become inactive, send event to trigger action after fixed delay */
         VRG_LOG_INFO((VRG_LOG_MOD_HBT,  "HEARTBEAT: DEVICE INACTIVE NOW!" ));

         /* Notify heartbeat module of inactivity */
         bosEventSend( pInactiveEvt );
      }
      break;

      default:
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HBT,  "hbSetDeviceActivity: ERROR - can't set state to something other than ACTIVE|INACTIVE!" ));
         XCHG_ASSERT(( state == HBSTATE_ACTIVE ) || ( state == HBSTATE_INACTIVE ));
      }
      break;
   }

   bosMutexRelease( &hbcb.hbMutex );
}


/*
*****************************************************************************
** FUNCTION:   hbGetDeviceActivity
**
** PURPOSE:    Get the overall device activity of active or inactive.
**
** PARAMETERS: None
**
** RETURNS:    Overall device activity state
**
*****************************************************************************
*/
HBSTATE hbGetDeviceActivity( void )
{
   return( hbcb.notifiedState );
}


/*
*****************************************************************************
** FUNCTION:   hbMapReceivedEvts
**
** PURPOSE:    Map received events into the proper bitmask
**
** PARAMETERS: pEvtBitMask - mapped bit mask
**
** RETURNS:    none
**
*****************************************************************************
*/
static void hbMapReceivedEvts( VRG_UINT16 *pEvtBitMask )
{
   int   i;

   /* Loop through all possible events */
   for ( i = 0; i < MAX_HB_EVTS; i++ )
   {
      if ( bosEventSetIsEventSet( &hbEventSet, &hbEvt[i] ) == VRG_TRUE )
      {
         *pEvtBitMask |= ( 1 << i );
      }
   }
}


/*
*****************************************************************************
** FUNCTION:   hbGetPatientsState
**
** PURPOSE:    Retrieve the state of all patients of a specific type.
**
** PARAMETERS: lineId - line ID assiocated with the patient(s)
**             patientType - type of patient to retrieve info for
**
** RETURNS:    Returns the conglomerated patient state.  If all of the
**             patients are inactive, then the returned state is 'inactive'.
**             If any of the patients are active, then 'active' state is returned.
**
*****************************************************************************
*/
HBSTATE hbGetPatientsState( int lineId, HBPATIENT_TYPE patientType )
{
   VRG_UINT32   i;

   /* Return immediately if there are no active patients */
   if ( hbcb.nActivePatients == 0 )
   {
      return ( HBSTATE_INACTIVE );
   }

   /* Retrieve patient state for all endpoints */
   if ( lineId == HBPATIENT_ALL_ENDPT )
   {
      /* Loop through all active patients */
      for ( i = 0; i < HB_MAX_PATIENTS; i++ )
      {
         if ( ( hbcb.patientInfo[i].patientState == HBSTATE_ACTIVE ) &&
              ( hbcb.patientInfo[i].patientType & patientType ) )
         {
            return ( HBSTATE_ACTIVE );
         }
      }
   }

   /* Retrieve patient state for a particular endpt */
   else
   {
      /* Loop through all active patients */
      for ( i = 0; i < HB_MAX_PATIENTS; i++ )
      {
         if ( ( hbcb.patientInfo[i].patientState == HBSTATE_ACTIVE ) &&
              ( hbcb.patientInfo[i].patientType & patientType ) &&
              ( hbcb.patientInfo[i].patientLineId == lineId ) )
         {
            return ( HBSTATE_ACTIVE );
         }
      }
   }

   return ( HBSTATE_INACTIVE );
}


/*
*****************************************************************************
** FUNCTION:   hbUpdatePatientEndptId
**
** PURPOSE:    Update the endpt number assiocated with a patient
**
** PARAMETERS: pid    - patient id
**             lineId - new line id assiocated with the patient
**
** RETURNS:    none
**
*****************************************************************************
*/
void hbUpdatePatientEndptId( int pid, int lineId )
{
   XCHG_ASSERT( pid < HB_MAX_PATIENTS );

   /* update patient's endpt number */
   hbcb.patientInfo[ pid ].patientLineId = lineId;
}

