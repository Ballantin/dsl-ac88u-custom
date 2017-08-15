/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: vrgEndpt.h
*
****************************************************************************
*    Description:
*
*      Voice Residential Gateway (VRG) Endpoint abstraction layer.
*
****************************************************************************/

#ifndef HEARTBEAT_H
#define HEARTBEAT_H


#if defined(__cplusplus)
extern "C"
{
#endif

#include <vrgTypes.h>

/* Heartbeat patient state */
typedef enum
{
   HBSTATE_UNKNOWN,
   HBSTATE_INACTIVE,
   HBSTATE_ACTIVE
} HBSTATE;


/* Heartbeat patient type */
typedef enum
{
   HBPATIENT_TYPE_DEFAULT     = ( 1 << 0 ),
   HBPATIENT_TYPE_POWER       = ( 1 << 1 ),
   HBPATIENT_TYPE_CNX         = ( 1 << 2 ),
   HBPATIENT_TYPE_HOOKSTATE   = ( 1 << 3 ),
   HBPATIENT_TYPE_OOSIGNAL    = ( 1 << 4 ),   /* On/Off signal */
   HBPATIENT_TYPE_CALLERID    = ( 1 << 5 ),   /* Caller ID signal */
   HBPATIENT_TYPE_SIGNAL      = ( 1 << 6 ),   /* All signals that are neither Caller ID or on/off */
   HBPATIENT_TYPE_CNXSIGNAL   = ( 1 << 7 ),   /* Signals on cnx*/
   HBPATIENT_TYPE_TPD         = ( 1 << 8 ),   /* Telephone Port Diagnostics */
   HBPATIENT_TYPE_UNKNOWN     = ( 1 << 9 )

} HBPATIENT_TYPE;


/*
** Patients with NCS activity.  Note that on/off signals are not considered
** 'active' NCS activity as they can last forever.
*/
#define HBPATIENT_TYPE_NCS_ACTIVITY ( HBPATIENT_TYPE_CNX       | \
                                      HBPATIENT_TYPE_CALLERID  | \
                                      HBPATIENT_TYPE_SIGNAL )

/*
** High-impact patients.  Activity on these patients indicate that the
** overall device is active.  High-impact patients is composed by the
** hookstate state and the NCS activity
*/
#define HBPATIENT_TYPE_HIGH_IMPACT  ( HBPATIENT_TYPE_NCS_ACTIVITY | \
                                      HBPATIENT_TYPE_TPD | \
                                      HBPATIENT_TYPE_HOOKSTATE )

/*
** Defines to identify endpoint number
*/
#define HBPATIENT_UNKNOWN_ENDPT   0
#define HBPATIENT_ALL_ENDPT      -1
#define HBPATIENT_ENDPT_ONE       0
#define HBPATIENT_ENDPT_TWO       1


/*
*****************************************************************************
** EXTERNAL FUNCTION PROTOTYPES
*****************************************************************************
*/

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
void hbInit( void );


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
*****************************************************************************
*/
void hbDeInit( void );


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
                        int lineId, int *pid );


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
void hbUnregisterPatient( int pid );


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
void hbUpdatePatientState( int pid, HBSTATE state );


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
HBSTATE hbGetPatientsState( int lineId, HBPATIENT_TYPE patientType );


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
void hbUpdatePatientEndptId( int pid, int lineId );


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
HBSTATE hbGetDeviceActivity( void );


#if defined(__cplusplus)
}
#endif


#endif  /* HEARTBEAT_H  */
