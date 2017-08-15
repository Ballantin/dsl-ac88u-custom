/****************************************************************************
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
*    Filename: vrgEndpt.h
*
****************************************************************************
*    Description:
*
*      Voice Residential Gateway (VRG) Endpoint abstraction layer.
*
****************************************************************************/

#ifndef VRG_ENDPT_H
#define VRG_ENDPT_H

#include <endpt.h>



#if defined(__cplusplus)
extern "C"
{
#endif

#define  VRG_CLASS_DATE_SIZE         8
#define  VRG_CLASS_MAX_NUMBER_SIZE   20
#define  VRG_CLASS_MAX_NAME_SIZE     16


typedef struct
{
   char  date[VRG_CLASS_DATE_SIZE + 1];          /* date string */
   char  number[VRG_CLASS_MAX_NUMBER_SIZE + 1];  /* phone number string */
   char  name[VRG_CLASS_MAX_NAME_SIZE + 1];      /* name string */
   char  reasonNoNumber;                         /* reason no number (ex. "P") */
   char  reasonNoName;                           /* reason no name (ex. "P") */
} VRG_CLIDRX_DATA;

/*
** VRG endpoint structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'ENDPT_STATE' base-class(structure).
** The base endpt structure specifies a set of function pointers that defines
** the generic endpt API that must be provided by all derived endpt types.
**
** When the VRG endpt initialization function is invoked, the base endpt
** structure function pointers will be intialized to a set of VRG specific
** endpt functions. Therefore, given a pointer to the base endpt struct,
** clients can interface to a generic endpt API that will vector to the
** derived endpt function.
*/
typedef struct
{
   /* Must be first elements of this struct. */
   const ENDPT_FUNCS   *endptFuncs;
   int                  lineId;
   EPTYPE               endptType;

   /* VRG specific endpt state below. */

} VRG_ENDPT_STATE;


/*
** Endpt initialization structure
*/
typedef struct VRG_ENDPT_INIT_CFG
{
   /* current country code based on nvol settings */
   VRG_COUNTRY    country;

   /* current power source - 0 = utility power
   **                        1 = battery power */
   int            currentPowerSource;

   /* pointer to dma pool buffer */
   void*          dma_pool_buffer;
} VRG_ENDPT_INIT_CFG;


/*
*****************************************************************************
** EXTERNAL FUNCTION PROTOTYPES
*****************************************************************************
*/

/*
*****************************************************************************
** FUNCTION:   vrgEndptInit
**
** PURPOSE:    Module initialization for the VRG endpoint. The endpoint
**             module is responsible for controlling a set of endpoints.
**             Individual endpoints are initialized using the vrgEndptInit() API.
**
** PARAMETERS: endptInitCfg             - pointer to endpt configuration block
**             notifyCallback           - Callback to use for event notification
**             packetCallback           - Callback to use for endpt packets
**             getProvisionCallback     - Callback to get provisioned values.
**                                        May be set to NULL.
**             setProvisionCallback     - Callback to get provisioned values.
**                                        May be set to NULL.
**             packetReleaseCallback    - Callback to release ownership of
**                                        endpt packet back to caller
**             taskShutdownCallback     - Callback invoked to indicate endpt
**                                        task shutdown
**             rtpStatsCallback         - Callback invoked to record rtp stats
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS vrgEndptInit
(
   VRG_ENDPT_INIT_CFG        *endptInitCfg,
   endptEventCallback         notifyCallback,
   endptPacketCallback        packetCallback,
   endptGetProvCallback       getProvisionCallback,
   endptSetProvCallback       setProvisionCallback,
   endptPacketReleaseCallback packetReleaseCallback,
   endptTaskShutdownCallback  taskShutdownCallback
);


/*
*****************************************************************************
** FUNCTION:   vrgEndptDeinit
**
** PURPOSE:    VRG endpoint module shutdown - call once during system shutdown.
**             This will shutdown all endpoints and free all resources used by
**             the VRG endpt manager. (i.e. this function should free all resources
**             allocated in vrgEndptInit() and vrgEndptCreate()).
**
** PARAMETERS: none
**
** RETURNS:    EPSTATUS
**             This function should only return an error under catastrophic
**             circumstances. i.e. Something that cannot be fixed by re-invoking
**             the module initialization function.
**
** NOTE:       It is assumed that this function is only called after all endpoint
**             tasks have been notified of a pending application reset, and each
**             one has acknowledged the notification. This implies that each endpoint
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization functions.
**
*****************************************************************************
*/
EPSTATUS vrgEndptDeinit( void );


/*
*****************************************************************************
** FUNCTION:   vrgEndptCreate
**
** PURPOSE:    This function is used to create a VRG endpoint object.
**
** PARAMETERS: physId      (in)  Physical interface.
**             lineId      (in)  Endpoint line identifier.
**             endptState  (out) Created endpt object.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptCreate( int physId, int lineId, VRG_ENDPT_STATE *endptState );


/*
*****************************************************************************
** FUNCTION:   vrgEndptDestroy
**
** PURPOSE:    This function is used to de-allocate a VRG endpoint object.
**
** PARAMETERS: endptState  (mod) Created endpt object.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptDestroy( VRG_ENDPT_STATE *endptState );


/*
*****************************************************************************
** FUNCTION:   vrgEndptGetNumEndPoints
**
** PURPOSE:    Retrieve the number of endpoints in a collection
**
** PARAMETERS: None
**
** RETURNS:    Number of endpoints
**
*****************************************************************************
*/
int vrgEndptGetNumEndpoints( void );


/*
*****************************************************************************
** FUNCTION:   vrgEndptGetNumFxsEndpoints
**
** PURPOSE:    Retrieve the number of FXS endpoints in a collection
**
** PARAMETERS: None
**
** RETURNS:    Number of FXO endpoints
**
*****************************************************************************
*/
int vrgEndptGetNumFxsEndpoints( void );


/*
*****************************************************************************
** FUNCTION:   vrgEndptGetNumFxoEndpoints
**
** PURPOSE:    Retrieve the number of FXO endpoints in a collection
**
** PARAMETERS: None
**
** RETURNS:    Number of FXO endpoints
**
*****************************************************************************
*/
int vrgEndptGetNumFxoEndpoints( void );


/*
*****************************************************************************
** FUNCTION:   vrgEndptGetNumDectEndpoints
**
** PURPOSE:    Retrieve the number of DECT endpoints in a collection
**
** PARAMETERS: None
**
** RETURNS:    Number of DECT endpoints
**
*****************************************************************************
*/
int vrgEndptGetNumDectEndpoints( void );

#ifdef PSTN_LIFE_LINE_SUPPORT
/*
*****************************************************************************
** FUNCTION:   vrgEndptIsPstnLifeLineSupported
**
** PURPOSE:    Determines whether pstn life line is supported
**
** PARAMETERS: None
**
** RETURNS:    true if supported; false otherwise
**
*****************************************************************************
*/
VRG_BOOL vrgEndptIsPstnLifeLineSupported( void );
#endif 

/*
*****************************************************************************
** FUNCTION:   vrgCodecTableInit
**
** PURPOSE:    Initialize codec table to reflect the simultaneous support
**
** PARAMETERS: none
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgCodecTableInit( void );


/*
*****************************************************************************
** FUNCTION:   vrgCodecTableRetrieveRow
**
** PURPOSE:    Return a single row from the codec table
**
** PARAMETERS: index        - row index to return (0-indexed)
**             pNumOfCodecs - pointer to the location for storing the
**                            retrieved row
**
** RETURNS:    none
**
** NOTE:
*****************************************************************************
*/
void vrgCodecTableRetrieveRow( int index, VRG_CODEC_TABLE_ROW *pNumOfCodecs );

/*
*****************************************************************************
** FUNCTION:   vrgEndptGetEndptOperStatus
**
** PURPOSE:    Get endpt operation status.  An endpt becomes un-operational
**             if any of the vhds associated with the endpt is pending a reset
**
** PARAMETERS: lineId - line Id
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptGetEndptOperStatus( int lineId );

#if defined(__cplusplus)
}
#endif


#endif  /* VRG_ENDPT_H  */
