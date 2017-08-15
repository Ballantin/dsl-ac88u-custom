/***************************************************************************
* Copyright (c) 2002 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: bosPrivateEvent.h
*
****************************************************************************
*    Description:
*
*      bos Event module private internal header file
*
****************************************************************************/

#ifndef BOSPRIVATEEVENT_H
#define BOSPRIVATEEVENT_H

#ifndef BOSCFG_H
#include <bosCfg.h>     /* include application-specific BOS config file */
#endif

#ifndef BOSTYPES_H
#include "bosTypes.h"
#endif

#if !BOS_CFG_EVENT
#error bos Event module not selected in bosCfg.h
#endif

#ifdef          __cplusplus
extern  "C" {                           /* C declarations in C++ */
#endif

/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/

/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

/*
*****************************************************************************
** FUNCTION PROTOTYPES
*****************************************************************************
*/


/***************************************************************************/
/**
*  Initializes the gBosEventFunctionName table. This function is called
*  from bosEventInit().
*/

void bosEventInitFunctionNames( void );


/**
 * DESCRIPTION:
 *           - Register a newly-created task with the event module.
 *
 * PARAMETERS:
 *           - taskId (I)
 *                    - Task ID associated with task to register.
 *
 * RETURNS:
 *           - BOS_STATUS_OK
 *              - if successful
 *           - error code
 *              - on failure
 *
 * COMMENTS:
 */
BOS_STATUS bosEventRegisterTask( BOS_TASK_ID *taskId );

/**
 * DESCRIPTION:
 *           - Un-register a previously-registered task with the event module.
 *
 * PARAMETERS:
 *           - taskId (I)
 *                    - Task ID associated with task to register.
 *
 * RETURNS:
 *           - BOS_STATUS_OK
 *              - if successful
 *           - error code
 *              - on failure
 *
 * COMMENTS:
 */
BOS_STATUS bosEventUnRegisterTask( BOS_TASK_ID *taskId );

#ifdef          __cplusplus
}                               /* End of C declarations */
#endif

#endif   /* BOSPRIVATEEVENT_H */

