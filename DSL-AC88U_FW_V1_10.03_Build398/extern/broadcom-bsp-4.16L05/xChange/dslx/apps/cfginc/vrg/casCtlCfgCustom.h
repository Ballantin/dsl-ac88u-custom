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
*    Filename: casCtlCfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in casCtlCfg.h
*
****************************************************************************/

#ifndef CAS_CTL_CFG_CUSTOM_H
#define CAS_CTL_CFG_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <globalCfg.h>


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/

/* Task priority for CAS */
#define CAS_CTL_CFG_TASK_PRIORITY                  MTA_TASK_HARDWARE


/* Maximum number of supported endpoints */
#define CAS_CTL_CFG_NUM_ENDPTS                     GLOBAL_CFG_MAX_ENDPT


/* Set these values to 1 to enable support for any set of static, country
** specific ring patterns. Set to 0 if ring patterns are not used or are
** dynamically provisioned */
#define CAS_CTL_CFG_NORTH_AMERICAN_RING_PROFILES   1
#define CAS_CTL_CFG_JAPANESE_RING_PROFILES         1
#define CAS_CTL_CFG_UK_RING_PROFILES               1
#define CAS_CTL_CFG_CHINA_RING_PROFILES            1

/* Egress packets coming from the network are looped back in the DSP 
** and sent back to the network as ingress packets */
#define HOOK_EVENTS_SIMULATED_THROUGH_COMMAND_LINE 0

#ifdef __cplusplus
    }
#endif

#endif  /* CAS_CTL_CFG_CUSTOM_H  */
